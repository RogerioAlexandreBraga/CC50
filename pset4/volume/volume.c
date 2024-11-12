// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    //abre o arquivo de saida
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // factor recebe o arguento qque ira determinar a multiplicação do volume
    float factor = atof(argv[3]);

    uint8_t header[44];

    //ler o cabeçalho(os primeiros 44bytes usando uint8_t)do aruivo input
    fread(header ,sizeof(uint8_t),HEADER_SIZE ,input);
    //escrever o cabeçalho no arquivo output
    fwrite(header, sizeof(uint8_t),HEADER_SIZE, output);

    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, input))
        {
            //em sequencia multiplicar o valor lido de input pelo argumento "factor"
            buffer = (int16_t)((float)buffer * factor);

            // escrever o reultado no arquivo output
            fwrite (&buffer, sizeof(int16_t), 1, output);
        }

    // Close files
    fclose(input);
    fclose(output);
}
