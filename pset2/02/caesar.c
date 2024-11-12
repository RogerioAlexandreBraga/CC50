#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool apenas_digitos(string texto);
char rotacionar(char p, int k);

int main(int argc, string argv[])
{
    if (argc != 2 || !apenas_digitos(argv[1]))
    {
        printf("Uso: ./caesar chave\n");
        return 1;
    }
    int k = atoi(argv[1]);
    string texto_plano = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(texto_plano); i < n; i++)
    {
        printf("%c", rotacionar(texto_plano[i], k));
    }
    printf("\n");
    return 0;
}

bool apenas_digitos(string texto)
{
    for (int i = 0, n = strlen(texto); i < n; i++)
    {
        if (!isdigit(texto[i]))
        {
            return false;
        }
    }
    return true;
}

char rotacionar(char p, int k)
{
    if (!isalpha(p))
    {
        return p;
    }
    char base = isupper(p) ? 'A' : 'a';
    return (p - base + k) % 26 + base;
}
