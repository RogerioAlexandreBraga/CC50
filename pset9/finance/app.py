import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime, timezone

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
# No application.py
app.secret_key = 'sua_chave_secreta'  # Necessário para flash messages
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Create new table, and index (for efficient search later on) to keep track of stock orders, by each user
db.execute("CREATE TABLE IF NOT EXISTS orders (id INTEGER, user_id NUMERIC NOT NULL, symbol TEXT NOT NULL, \
            shares NUMERIC NOT NULL, price NUMERIC NOT NULL, timestamp TEXT, PRIMARY KEY(id), \
            FOREIGN KEY(user_id) REFERENCES users(id))")
db.execute("CREATE INDEX IF NOT EXISTS orders_by_user_id_index ON orders (user_id)")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")







@app.route("/")
@login_required
def index():
    # Obter ações do usuário agrupadas por símbolo
    stocks = db.execute("""
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
    """, session["user_id"])

    # Obter saldo atual
    cash = db.execute("SELECT cash FROM users WHERE id = ?",
                     session["user_id"])[0]["cash"]

    # Obter preços atuais e calcular totais
    grand_total = cash
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["total"] = stock["price"] * stock["total_shares"]
        grand_total += stock["total"]

    return render_template("index.html",
                         stocks=stocks,
                         cash=cash,
                         grand_total=grand_total)








@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # Verificar se o símbolo foi fornecido
        symbol = request.form.get("symbol").upper()
        if not symbol:
            return apology("must provide symbol")

        # Verificar se a quantidade foi fornecida e é válida
        try:
            shares = int(request.form.get("shares"))
            if shares <= 0:
                return apology("must provide positive number of shares")
        except:
            return apology("invalid number of shares")

        # Consultar preço atual da ação
        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol")

        # Calcular custo total
        price = quote["price"]
        total_cost = price * shares

        # Verificar se usuário tem saldo suficiente
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]

        if cash < total_cost:
            return apology("can't afford")

        # Atualizar saldo do usuário
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                  cash - total_cost, session["user_id"])

        # Registrar a transação
        db.execute("""INSERT INTO transactions
                     (user_id, symbol, shares, price, type)
                     VALUES (?, ?, ?, ?, 'buy')""",
                     session["user_id"], symbol, shares, price)

        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")








@app.route("/history")
@login_required
def history():
    # Obter todas as transações do usuário
    transactions = db.execute("""
        SELECT symbol, shares, price, timestamp, type
        FROM transactions
        WHERE user_id = ?
        ORDER BY timestamp DESC
    """, session["user_id"])

    # Calcular o valor total de cada transação
    for transaction in transactions:
        transaction["total"] = transaction["shares"] * transaction["price"]

    return render_template("history.html", transactions=transactions)








@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")





@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")






@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("deve fornecer o símbolo da ação", 400)

        stock = lookup(symbol)
        if not stock:
            return apology("símbolo de ação inválido", 400)

        return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")





@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    # check username and password
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")
    if username == "" or len(db.execute('SELECT username FROM users WHERE username = ?', username)) > 0:
        return apology("Invalid Username: Blank, or already exists")
    if password == "" or password != confirmation:
        return apology("Invalid Password: Blank, or does not match")
    # Add new user to users db (includes: username and HASH of password)
    db.execute('INSERT INTO users (username, hash) \
            VALUES(?, ?)', username, generate_password_hash(password))
    # Query database for username
    rows = db.execute("SELECT * FROM users WHERE username = ?", username)
    # Log user in, i.e. Remember that this user has logged in
    session["user_id"] = rows[0]["id"]
    # Redirect user to home page
    return redirect("/")






@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        # Verificar se o símbolo foi fornecido
        symbol = request.form.get("symbol").upper()
        if not symbol:
            return apology("must provide symbol")

        # Verificar se a quantidade foi fornecida e é válida
        try:
            shares = int(request.form.get("shares"))
            if shares <= 0:
                return apology("must provide positive number of shares")
        except:
            return apology("invalid number of shares")

        # Verificar se usuário tem ações suficientes
        stocks = db.execute("""
            SELECT SUM(shares) as total_shares
            FROM transactions
            WHERE user_id = ? AND symbol = ?
            GROUP BY symbol
        """, session["user_id"], symbol)

        if not stocks or stocks[0]["total_shares"] < shares:
            return apology("not enough shares")

        # Obter preço atual
        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol")

        # Calcular valor da venda
        price = quote["price"]
        total_sale = price * shares

        # Atualizar saldo do usuário
        db.execute("""
            UPDATE users
            SET cash = cash + ?
            WHERE id = ?
        """, total_sale, session["user_id"])

        # Registrar a venda (note o valor negativo de shares)
        db.execute("""
            INSERT INTO transactions
            (user_id, symbol, shares, price, type)
            VALUES (?, ?, ?, ?, 'sell')
        """, session["user_id"], symbol, -shares, price)

        flash("Sold!")
        return redirect("/")

    else:
        # Obter ações que o usuário possui para o menu dropdown
        stocks = db.execute("""
            SELECT symbol, SUM(shares) as total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
        """, session["user_id"])

        return render_template("sell.html", stocks=stocks)










def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


def own_shares():
    """Helper function: Which stocks the user owns, and numbers of shares owned. Return: dictionary {symbol: qty}"""
    user_id = session["user_id"]
    owns = {}
    query = db.execute("SELECT symbol, shares FROM orders WHERE user_id = ?", user_id)
    for q in query:
        symbol, shares = q["symbol"], q["shares"]
        owns[symbol] = owns.setdefault(symbol, 0) + shares
    # filter zero-share stocks
    owns = {k: v for k, v in owns.items() if v != 0}
    return owns

def time_now():
    """HELPER: get current UTC date and time"""
    now_utc = datetime.now(timezone.utc)
    return str(now_utc.date()) + ' @time ' + now_utc.time().strftime("%H:%M:%S")






# No helpers.py ou application.py
def usd(value):
    """Format value as USD."""
    return f"{value:,.2f}"





# Criar tabela transactions se não existir
db.execute("""
    CREATE TABLE IF NOT EXISTS transactions (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        user_id INTEGER NOT NULL,
        symbol TEXT NOT NULL,
        shares INTEGER NOT NULL,
        price NUMERIC NOT NULL,
        timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
        type TEXT NOT NULL,
        FOREIGN KEY (user_id) REFERENCES users(id)
    )
""")
