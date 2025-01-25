// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void write_output_header(FILE *in, FILE *out);
void write_output_body(FILE *in, FILE *out, float fact);

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

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    write_output_header(input, output);

    // TODO: Read samples from input file and write updated data to output file
    write_output_body(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

void write_output_header(FILE *in, FILE *out)
{
    uint8_t header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, in);
    fwrite(header, HEADER_SIZE, 1, out);
}

void write_output_body(FILE *in, FILE *out, float fact)
{
    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, in))
    {
        // Update volume of sample
        buffer *= fact;

        // Write updated sample to new file
        fwrite(&buffer, sizeof(int16_t), 1, out);
    }
}
