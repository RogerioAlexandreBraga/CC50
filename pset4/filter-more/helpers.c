#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //loop matriz da imagem
    for (int i = 0; i < height; i++)
    {
        for ( int j = 0; j < width; j++)
        {
            int Blue = image[i][j].rgbtBlue;
            int Green = image[i][j].rgbtGreen;
            int Red = image[i][j].rgbtRed;

            //calculo para transformar as cores dos bytes em tons de cinza
            int gs = round((Blue + Green + Red)/3.0);

            //atribuir o valor de gs para os byts
            image[i][j].rgbtBlue = gs;
            image[i][j].rgbtGreen = gs;
            image[i][j].rgbtRed = gs;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //loop matriz da imagem
    for (int i = 0; i < height; i++)
    {
        for ( int j = 0; j < width/2; j++)
        {
           RGBTRIPLE copy = image[i][j];
           image[i][j] = image[i][width - (j + 1)];
           image[i][width - (j + 1)] = copy;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //criar imagemm de copia
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    //loop vertical e horizntal
    for (int i =0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //variaveis
            int count;
            float sum_red;
            float sum_blue;
            float sum_green;
            sum_red = sum_blue = sum_green = count = 0;
            //loop ds pixels vertical e horizontal
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    //cheka se esta dentro da imagem
                    if ((i + k) >= 0 && (i + k) < height)
                    {
                         if ((j + l) >= 0 && (j + l) < width)
                        {
                            // somas
                            sum_red += copy[i + k][j + l].rgbtRed;
                            sum_blue += copy[i + k][j + l].rgbtBlue;
                            sum_green += copy[i + k][j + l].rgbtGreen;
                            count++;
                        }
                    }
                }
            }
            // cria o pixel com o valor medio
            image[i][j].rgbtRed = round(sum_red / (float)count);
            image[i][j].rgbtBlue = round(sum_blue / (float)count);
            image[i][j].rgbtGreen = round(sum_green / (float)count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //criar imagemm de copia
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    //matrizes de borda
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //loop vertical e horizntal
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //declaração de variaveis Gx
            float gx_red;
            float gx_blue;
            float gx_green;
            //declaração de variaveis Gy
            float gy_red;
            float gy_blue;
            float gy_green;
            gx_red = gx_blue = gx_green = gy_red = gy_blue = gy_green = 0;
            //loop para checar os pixels
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    //condições
                    if (i + k < 0 || i + k >= height)
                    {
                       continue;
                    }
                     if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }

                    // recebem valores Gx
                    gx_red += copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                    gx_blue += copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                    gx_green += copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];

                    // recebem valores Gy
                    gy_red += copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                    gy_blue += copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                    gy_green += copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];

                  }
            }
              // calculo de arredondamento de raiz das corees
            int red = round(sqrt(gx_red * gx_red + gy_red * gy_red));
            int blue = round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));
            int green = round(sqrt(gx_green * gx_green + gy_green * gy_green));

            //para que não gere erros de cor o limite é de 255
            if (red > 255)
            {
                red = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            image[i][j].rgbtRed = red;
            image[i][j].rgbtBlue = blue;
            image[i][j].rgbtGreen = green;
        }
    }
    return;
}
