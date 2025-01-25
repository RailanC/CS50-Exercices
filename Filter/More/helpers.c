#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float avg = 0;
    int red_color = 0;
    int green_color = 0;
    int blue_color = 0;

    for (int col = 0; col < height; col++)
        for (int row = 0; row < width; row++)
        {
            red_color = image[col][row].rgbtRed;
            green_color = image[col][row].rgbtGreen;
            blue_color = image[col][row].rgbtBlue;

            avg = round(round(red_color + green_color + blue_color) / 3);

            image[col][row].rgbtRed = avg;
            image[col][row].rgbtGreen = avg;
            image[col][row].rgbtBlue = avg;
        }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    for (int col = 0; col < height; col++)
        for (int row = 0; row < (width % 2 == 0 ? width / 2 : (width - 1) / 2); row++)
        {
            // Swap pixels
            RGBTRIPLE pixel_copy = image[col][row];
            image[col][row] = image[col][width - (row + 1)];
            image[col][width - (row + 1)] = pixel_copy;
        }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int col_copy = 0; col_copy < height; col_copy++)
    {
        for (int row_copy = 0; row_copy < width; row_copy++)
        {
            copy[col_copy][row_copy] = image[col_copy][row_copy];
        }
    }

    for (int col = 0; col < height; col++)
    {
        for (int row = 0; row < width; row++)
        {

            float sum_red = 0;
            float sum_green = 0;
            float sum_blue = 0;
            float div = 0;

            int start_row_box = (col - 1 < 0) ? 0 : col - 1;
            int end_row_box = (col + 1 >= height) ? height - 1 : col + 1;
            int start_co_box = (row - 1 < 0) ? 0 : row - 1;
            int end_col_box = (row + 1 >= width) ? width - 1 : row + 1;

            for (int y = start_row_box; y <= end_row_box; y++)
            {
                for (int x = start_co_box; x <= end_col_box; x++)
                {
                    sum_red += copy[y][x].rgbtRed;
                    sum_green += copy[y][x].rgbtGreen;
                    sum_blue += copy[y][x].rgbtBlue;
                    div++;
                }
            }

            image[col][row].rgbtRed = round((sum_red / div));
            image[col][row].rgbtGreen = round((sum_green / div));
            image[col][row].rgbtBlue = round((sum_blue / div));
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE copy[height][width];
    for (int col_copy = 0; col_copy < height; col_copy++)
    {
        for (int row_copy = 0; row_copy < width; row_copy++)
        {
            copy[col_copy][row_copy] = image[col_copy][row_copy];
        }
    }
    for (int col = 0; col < height; col++)
    {
        for (int row = 0; row < width; row++)
        {
            int gx_red = 0;
            int gy_red = 0;
            int gx_blue = 0;
            int gy_blue = 0;
            int gx_green = 0;
            int gy_green = 0;

            for (int y = -1; y < 2; y++)
            {
                for (int x = -1; x < 2; x++)
                {
                    if ((col + y < 0 || col + y > height - 1) ||
                        (row + x < 0 || row + x > width - 1))
                    {
                        continue;
                    }
                    gx_red += copy[y + col][x + row].rgbtRed * gx[y + 1][x + 1];
                    gy_red += copy[y + col][x + row].rgbtRed * gy[y + 1][x + 1];
                    gx_blue += copy[y + col][x + row].rgbtBlue * gx[y + 1][x + 1];
                    gy_blue += copy[y + col][x + row].rgbtBlue * gy[y + 1][x + 1];
                    gx_green += copy[y + col][x + row].rgbtGreen * gx[y + 1][x + 1];
                    gy_green += copy[y + col][x + row].rgbtGreen * gy[y + 1][x + 1];
                }
            }

            int red = round(sqrt((gx_red * gx_red) + (gy_red * gy_red)));
            int blue = round(sqrt((gx_blue * gx_blue) + (gy_blue * gy_blue)));
            int green = round(sqrt((gx_green * gx_green) + (gy_green * gy_green)));

            image[col][row].rgbtRed = red > 255 ? 255 : red;
            image[col][row].rgbtBlue = blue > 255 ? 255 : blue;
            image[col][row].rgbtGreen = green > 255 ? 255 : green;
        }
    }
    return;
}
