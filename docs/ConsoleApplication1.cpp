// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// main.cpp
#define _CRT_SECURE_NO_WARNINGS // preprocessor used to have access to fopen() function

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp.h" // header file with important data structures

int main(int argc, char* argv[])
{
    if (argc != 2) // program must be called with only one command-line argument
    {
        printf("Usage: SSD1306 infile.bmp\n");
        return 1;
    }

    char* infile = argv[1]; // extract infile's name

    FILE* inptr = fopen(infile, "rb"); // Open in binary mode
    if (inptr == NULL) // abort if fopen returns NULL pointer
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    FILE* outptr = fopen("output.txt", "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create output file.\n");
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr); // extract file header

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr); // extract info header

    if (bf.bfType != 0x4D42 || bf.bfOffBits != 54 || bi.biSize != 40 ||// check that the format is correct
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    if (bi.biHeight % 8 != 0)
    {
        printf("Height must be a multiple of 8.\n");
        return 5;
    }

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // calculate corresponding padding

    // Dynamic memory allocation of our buffer
    uint8_t** buffer = (uint8_t**)malloc(bi.biWidth * sizeof(uint8_t*));
    for (int i = 0; i < bi.biWidth; i++)
    {
        buffer[i] = (uint8_t*)malloc(bi.biHeight * sizeof(uint8_t));
    }

    for (int y = 0; y < bi.biHeight; y++) // for every scan line
    {
        for (int x = 0; x < bi.biWidth; x++) // iterate over each pixel
        {
            RGBTRIPLE pixel; // buffer
            fread(&pixel, sizeof(RGBTRIPLE), 1, inptr); // read from bmp to buffer
            uint8_t value = (pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3; // apply black-and-white filter
            if (value < 128) // if below a certain threshold
                value = 0; // it's off
            else
                value = 1; // if above, it's on

            buffer[x][bi.biHeight - y - 1] = value; // reconstruct the image bottom to top
        }
        fseek(inptr, padding, SEEK_CUR); // skip the padding in the bmp file
    }

    uint8_t next = 0; // variable used to allign the bytes in the output .txt file
    int byteNumber = 0; // counter used to inform the user of the size of the array
    fprintf(outptr, "const uint8_t bitmap[] = { // size %d bytes, width %d, height %d.\n", (bi.biHeight / 8) * bi.biWidth, bi.biWidth, bi.biHeight / 8);
    // NOTE: The driver writes the bitmap left to right and then top to bottom
    for (int page = 0; page < bi.biHeight / 8; page++) // pages are like "rows" but 8-pixels tall
    {
        for (int x = 0; x < bi.biWidth; x++)
        {
            uint8_t hex = 0; // byte used to construct the hexadecimal value corresponding to that page
            for (int y = page * 8, order = 0; y < (page + 1) * 8; y++, order++)
            {
                // remember that the top pixel in the page is LSB and the bottom is MSB
                hex |= (buffer[x][y] << order); // bitwise or, starting with LSB bits
            }
            fprintf(outptr, "0x%02X", hex); // write that to the .txt file in hexadecimal format
            byteNumber++; // add one byte to the counter
            next++; // each row in the text file contains 16 hex values
            if (next == 16)
            {
                next = 0;
                fprintf(outptr, ",\n");
            }
            else
            {
                fprintf(outptr, ", ");
            }
        }
    }
    if (next == 0)
    {
        fseek(outptr, -3, SEEK_CUR);  // Move back 2 bytes
    }
    else
    {
        fseek(outptr, -2, SEEK_CUR);  // Move back 2 bytes
    }
    fprintf(outptr, "\n};");
    printf("Execution successful.\n");
    printf("The array has a size of %d bytes.\n", byteNumber); // print the number of bytes long

    // Free memory
    for (int i = 0; i < bi.biWidth; i++)
    {
        free(buffer[i]);
    }
    free(buffer);

    fclose(inptr);
    fclose(outptr);

    return 0;
}
