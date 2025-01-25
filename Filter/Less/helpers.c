#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            float avarage = round(
                round(image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
            // Update pixel values
            image[i][j].rgbtRed = avarage;
            image[i][j].rgbtGreen = avarage;
            image[i][j].rgbtBlue = avarage;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepiaRed = 0;
    float sepiaGreen = 0;
    float sepiaBlue = 0;
    int originalRed = 0;
    int originalGreen = 0;
    int originalBlue = 0;
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute sepia values
            originalRed = image[i][j].rgbtRed;
            originalGreen = image[i][j].rgbtGreen;
            originalBlue = image[i][j].rgbtBlue;

            sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // Update pixel with sepia values
            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width % 2 == 0 ? width / 2 : (width - 1) / 2); j++)
        {
            // Swap pixels
            RGBTRIPLE pixel_copy = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = pixel_copy;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int r = 0; r < height; r++)
    {
        for (int v = 0; v < width; v++)
        {
            copy[r][v] = image[r][v];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            float sum_red = 0;
            float sum_green = 0;
            float sum_blue = 0;
            float div = 0;

            int start_row = (i - 1 < 0) ? 0 : i - 1;
            int end_row = (i + 1 >= height) ? height - 1 : i + 1;
            int start_col = (j - 1 < 0) ? 0 : j - 1;
            int end_col = (j + 1 >= width) ? width - 1 : j + 1;

            for (int y = start_row; y <= end_row; y++)
            {
                for (int x = start_col; x <= end_col; x++)
                {
                    sum_red += copy[y][x].rgbtRed;
                    sum_green += copy[y][x].rgbtGreen;
                    sum_blue += copy[y][x].rgbtBlue;
                    div++;
                }
            }

            image[i][j].rgbtRed = round((sum_red / div));
            image[i][j].rgbtGreen = round((sum_green / div));
            image[i][j].rgbtBlue = round((sum_blue / div));
        }
    }
    return;
}
