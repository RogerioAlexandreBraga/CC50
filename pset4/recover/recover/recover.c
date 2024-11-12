#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// cria constantes globais uteis e define tipos que serão usados
const int chunk_size = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // filtro de uso
    if (argc != 2)
    {
        printf("Usage: ./recover <filename.raw>\n");
        return 1;
    }
    // abrindo o arquivo enquanto filtra o seu uso
    // OBS: é necessario fechar o arquivo quando terminar
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Empty file /Reading error\n");
        return 1;
    }

    // criação variaveis pra efetuar a leitura e modificação de dados pela função fread
    // OBS: "sempre que usar 'malloc' deve usar o 'free' em algum momento"
    BYTE buffer[chunk_size];
    char *recovered_jpeg_name = malloc(8);
    int jpeg_count = 0;
    FILE *recovered_file;
    int writing = 0;

    // ler em blocos de 512 bytes procurando o padrão dos primeiros bytes
    while (fread(&buffer, 1, chunk_size, file))
    {
        // essa é a condição para o inicio de um arquivo jpeg e o final do jpeg anterior ao mesmo tempo
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff)  && (buffer[3] & 0xf0) == 0xe0)
        {
            // se o inicio de um arquivo jpeg foi encontrado enquanto se fazia
            // a escrita do anterior, significa que o anterior acabou,
            // portanto fechamos o arquivo recuperado e dizemos que a nova escrita
            // ainda não começou
            if (writing)
            {
                fclose(recovered_file);
                writing = 0;
            }
            // aqui uma variavel char* (string) separado anteriormente deve receber um
            // valor de nome pela função sprintf como requisitado no comando da questão,
            // e o nome deve seguir a contagem de arquivos recuperados (jpeg_count)
            sprintf(recovered_jpeg_name, "%03i.jpg", jpeg_count);

            // agora que já temos o nome do arquivo, devemos escrever nele,
            // criando o arquivo propriamente dito
            // OBS: deve ser fechado no fim do codigo
            recovered_file = fopen(recovered_jpeg_name, "w");

            // dizemos que a leitura começou
            writing = 1;

            // escrevemos o primeiro "chunk" de memoria no arquivo,
            // pois ele começa com os buffers requisitados, indicando que faz parte do jpeg
            fwrite(buffer, 1, chunk_size, recovered_file);

            // efetuamos a contagem
            jpeg_count++;
        }

        // se o buffer analisado não for o inicio de um jpeg, ele pode não ser nada, ou ser
        // o meio de um jpeg
        else
        {
            // como os jpeg estão justapostos, isto é, não tem nada entre eles, se a contagem começou, significa que
            // o buffer em questão é o meio de um jpeg (também siginifica que tem um "recovered_file" aberto)
            if (jpeg_count > 0)
            {
                // portante salvamos o buffer no arquivo em recuperação aberto
                fwrite(buffer, 1, chunk_size, recovered_file);
            }
        }
    }
    // o ultimo arquivo não termina com o inicio do proximo (obviamente), portanto
    // é necessário fecha-lo depois do fim do loop
    fclose(recovered_file);

    // a variavel não será mais necessária, portanto deve ser liberada
    free(recovered_jpeg_name);

    // o programa acabou, o arquivo original não será mais necessário, então deve ser fechado
    fclose(file);
    return 0;
}
