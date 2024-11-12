from flask import Flask, render_template, request, redirect
from cs50 import SQL

app = Flask(__name__)
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Adicionar novo aniversário ao banco de dados
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
        return redirect("/")

    else:
        # Consultar todos os aniversários do banco de dados
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)
