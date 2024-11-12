#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int calcular_pontuacao(string palavra);

int main(void)
{
    string palavra1 = get_string("Player 1: ");
    string palavra2 = get_string("Player 2: ");

    int pontuacao1 = calcular_pontuacao(palavra1);
    int pontuacao2 = calcular_pontuacao(palavra2);

    if (pontuacao1 > pontuacao2)
    {
        printf("Player 1 wins!\n");
    }
    else if (pontuacao1 < pontuacao2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int calcular_pontuacao(string palavra)
{
    int pontuacao = 0;
    int letras[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    for (int i = 0, n = strlen(palavra); i < n; i++)
    {
        if (isalpha(palavra[i]))
        {
            pontuacao += letras[toupper(palavra[i]) - 'A'];
        }
    }
    return pontuacao;
}
