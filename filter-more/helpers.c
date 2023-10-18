#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Avg of RGB values might not always be int
            int avg = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = image[h][w].rgbtGreen = image[h][w].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++) // Run up till halfway mark of array
        {
            RGBTRIPLE temp = image[h][w];
            image[h][w] = image[h][width - (w + 1)];
            image[h][width - (w + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image, park new pixel values in copy, blur then insert back into original image
    RGBTRIPLE temp[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][w];
        }
    }

    // Loop through pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int totalRed, totalGreen, totalBlue;
            totalRed = totalGreen = totalBlue = 0;
            float neighbourCount = 0.0;

            // Find each pixel's neighbours
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = h + x;
                    int currentY = w + y;

                    // Check if pixel's neighbour are VALID
                    if (currentX < 0 || currentX > height - 1 || currentY < 0 || currentY > width - 1)
                    {
                        continue;
                    }

                    // Add sum of R, G, B-values from VALID neighbours
                    totalRed += image[currentX][currentY].rgbtRed;
                    totalGreen += image[currentX][currentY].rgbtGreen;
                    totalBlue += image[currentX][currentY].rgbtBlue;

                    // Count no. of VALID neighbours
                    neighbourCount++;
                }

                // Calculate avg of VALID neighbours
                temp[h][w].rgbtRed = round(totalRed / neighbourCount);
                temp[h][w].rgbtGreen = round(totalGreen / neighbourCount);
                temp[h][w].rgbtBlue = round(totalBlue / neighbourCount);
            }
        }
    }
    // Copy new pixels into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
            image[h][w].rgbtBlue = temp[h][w].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image called temp, park new pixel values in temp then insert back into original image
    RGBTRIPLE temp[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][w];
        }
    }

    // Initialise the two 2D arrays Gx and Gy
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Loop through each row and col
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int redX = 0;
            int greenX = 0;
            int blueX = 0;
            int redY = 0;
            int greenY = 0;
            int blueY = 0;

            // Find neighbour pixels. Relate 2D array of neighbour pixels to Gx array.
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    int currentX = h - 1 + x;
                    int currentY = w - 1 + y;

                    // Check if pixel's neighbour are VALID
                    if (currentX < 0 || currentX > height - 1 || currentY < 0 || currentY > width - 1)
                    {
                        continue;
                    }

                    // Calculate each Gx value
                    redX += image[currentX][currentY].rgbtRed * Gx[x][y];
                    greenX += image[currentX][currentY].rgbtGreen * Gx[x][y];
                    blueX += image[currentX][currentY].rgbtBlue * Gx[x][y];

                    // Calculate each Gy value
                    redY += image[currentX][currentY].rgbtRed * Gy[x][y];
                    greenY += image[currentX][currentY].rgbtGreen * Gy[x][y];
                    blueY += image[currentX][currentY].rgbtBlue * Gy[x][y];
                }
            }
            // Find sqrt of Gx and Gy, round to nearest int
            int red = round(sqrt((redX * redX) + (redY * redY)));
            int green = round(sqrt((greenX * greenX) + (greenY * greenY)));
            int blue = round(sqrt((blueX * blueX) + (blueY * blueY)));

            // Cap sqrt value at 255
            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            // Copy values into temp image
            temp[h][w].rgbtRed = red;
            temp[h][w].rgbtGreen = green;
            temp[h][w].rgbtBlue = blue;
        }
    }

    // Copy new pixels into original image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
            image[h][w].rgbtBlue = temp[h][w].rgbtBlue;
        }
    }

    return;
}
