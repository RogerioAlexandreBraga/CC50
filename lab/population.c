#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int tamanho_inicial, tamanho_final;
    int anos = 0;

    do
    {
        tamanho_inicial = get_int("Start Size: ");
    }
    while (tamanho_inicial < 9);

    do
    {
        tamanho_final = get_int("End Size: ");
    }
    while (tamanho_inicial > tamanho_final);

    if (tamanho_inicial == tamanho_final)
    {
        printf("Years: 0");
    }
    else
    {
        do
        {
            tamanho_inicial = (tamanho_inicial + tamanho_inicial / 3) - tamanho_inicial / 4;
            anos++;
        }
        while (tamanho_inicial < tamanho_final);
        printf("Years: %i\n", anos);
    }
}
