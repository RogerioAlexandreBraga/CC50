# Importa a função get_string da biblioteca cs50
from cs50 import get_string
import math

def main():
    texto = get_string("Texto: ")
    letras = 0
    palavras = 1
    sentencas = 0

    for char in texto:
        if char.isalpha():
            letras += 1
        elif char.isspace():
            palavras += 1
        elif char in ['.', '!', '?']:
            sentencas += 1

    L = (letras / palavras) * 100  # Número médio de letras / 100 palavras
    S = (sentencas / palavras) * 100  # Número médio de sentenças / 100 palavras

    indice = round(0.0588 * L - 0.296 * S - 15.8)

    if indice >= 16:
        print("Grade 16+")
    elif indice < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {indice}")

if __name__ == "__main__":
    main()
