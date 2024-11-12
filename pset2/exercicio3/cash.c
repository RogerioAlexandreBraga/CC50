#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    float troco;
    int centavos, indice;
    int quantidade = 0;

    int moedas[] = {25, 10, 5, 1};
    do
    {
        printf("Troco Devido: ");
        scanf("%f", &troco);
    }
    while (troco <= 0);

    centavos = round(troco * 100);

    for (indice = 0; indice < 5; indice++)
    {
        while (centavos - moedas[indice] >= 0 && centavos > 0)
        {
            centavos = centavos - moedas[indice];
            quantidade++;
        }
    }
    printf("%i\n", quantidade);
    return 0;
}
