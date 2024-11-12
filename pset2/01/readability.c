#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    string texto = get_string("Texto: ");
    int letras = 0;
    int palavras = 1;
    int sentencas = 0;

    int n = 0;

    while (texto[n] != '\0')
    {
        int ascii = tolower(texto[n]);

        if (ascii == 32)
        {
            palavras++;
        }
        else if (ascii >= 97 && ascii <= 122)
        {
            letras++;
        }
        else if (ascii == 33 || ascii == 46 || ascii == 63)
        {
            sentencas++;
        }
        n++;
    }

    float L = ((float) letras / palavras) * 100;    // Número médio de letras / 100 palavras
    float S = ((float) sentencas / palavras) * 100; // Número médio de sentenças / 100 palavras

    int indice = round(0.0588 * L - 0.296 * S - 15.8);

    if (indice >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (indice < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", indice);
    }
}

