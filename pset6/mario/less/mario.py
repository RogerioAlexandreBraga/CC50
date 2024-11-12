# Importa a função get_int da biblioteca cs50
from cs50 import get_int

def main():
    # Solicita a altura ao usuário
    while True:
        altura = get_int("Altura: ")
        if 1 <= altura <= 8:
            break

    # Loop para cada linha
    for linha in range(altura):
        # Imprime os espaços
        for espacos in range(altura - linha - 1):
            print(" ", end="")

        # Imprime os hashes
        for coluna in range(linha + 1):
            print("#", end="")

        # Pula para a próxima linha
        print()

if __name__ == "__main__":
    main()

