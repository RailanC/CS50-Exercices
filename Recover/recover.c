#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");

    // Create a buffer for a block of data
    BYTE buffer[512];
    int count_jpeg = 0;
    // While there's still data left to read from the memory card
    char filename[8] = {0};
    FILE *img = NULL;
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Create JPEGs from the data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
                fclose(img);

            sprintf(filename, "%03i.jpg", count_jpeg++);
            img = fopen(filename, "w");
        }
        if (img != NULL)
        {
            fwrite(buffer, sizeof(BYTE) * 512, 1, img);
        }
    }
    if (img != NULL)
        fclose(img);

    fclose(card);
}
