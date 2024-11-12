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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            //new sepia value
           int sepiaRed = round(0.393 * red + 0.769 * green + 0.189 * blue);
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }
           int sepiaGreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
           int sepiaBlue = round(0.272 * red + 0.534 * green + 0.131 * blue);
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
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
