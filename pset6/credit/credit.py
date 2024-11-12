# Importa a função get_int da biblioteca cs50
from cs50 import get_int

def main():
    # Solicita o número do cartão ao usuário
    while True:
        cardnumber = get_int("Card Number: ")
        if cardnumber >= 0:
            break

    # Conta o número de dígitos
    count = 0
    digits = cardnumber
    while digits > 0:
        digits //= 10
        count += 1

    # Verifica se o número de dígitos é válido
    if count not in [13, 15, 16]:
        print("INVALID")
        return

    # Armazena os dígitos do cartão em uma lista
    number = []
    for _ in range(count):
        number.append(cardnumber % 10)
        cardnumber //= 10

    # Cria uma cópia dos números originais
    originalnumber = number.copy()

    # Multiplica cada segundo dígito por 2
    for i in range(1, count, 2):
        number[i] *= 2

    # Soma os dígitos dos números resultantes
    v = 0
    for i in range(count):
        v += (number[i] % 10) + (number[i] // 10 % 10)

    # Verifica o tipo de cartão
    if count == 13 and originalnumber[12] == 4 and v % 10 == 0:
        print("VISA")
    elif count == 15 and originalnumber[14] == 3 and v % 10 == 0 and (originalnumber[13] == 4 or originalnumber[13] == 7):
        print("AMEX")
    elif count == 16:
        if originalnumber[15] == 4 and v % 10 == 0:
            print("VISA")
        elif originalnumber[15] == 5 and v % 10 == 0 and originalnumber[14] in [1, 2, 3, 4, 5]:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()
