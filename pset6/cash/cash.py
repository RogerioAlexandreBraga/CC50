# Importa a função get_float da biblioteca cs50
from cs50 import get_float
import math

def main():
    # Solicita o valor do troco ao usuário
    while True:
        troco = get_float("Troco Devido: ")
        if troco > 0:
            break

    # Converte o troco para centavos
    centavos = round(troco * 100)
    quantidade = 0

    # Define os valores das moedas
    moedas = [25, 10, 5, 1]

    # Calcula a quantidade de moedas necessárias
    for moeda in moedas:
        while centavos >= moeda:
            centavos -= moeda
            quantidade += 1

    # Imprime a quantidade de moedas
    print(quantidade)

if __name__ == "__main__":
    main()
