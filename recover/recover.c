#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // If not exactly one command-line argument, remind correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file card.raw
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Declare variables and files
    bool found_jpg = false;     // Flag if JPEG has been found
    int jpg_count = 0;  // Counter for no. of JPEG found
    BYTE buffer[BLOCK_SIZE];    // Create variable buffer to store a block of data from forensic image
    char *filename = malloc(8 * sizeof(char));  // Allocate memory for filename ###.jpg
    FILE *output_file = NULL;   // File pointer to output data to

    // Read forensic image block by block into buffer, repeat until end of card
    while (fread(buffer, BLOCK_SIZE, 1, raw_file) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            // Close the previous JPEG file, if one was open
            if (found_jpg)
            {
                fclose(output_file);
            }
            else
            {
                found_jpg = true;
            }

            // Open a new JPEG file
            sprintf(filename, "%03i.jpg", jpg_count);   // Generate name for new JPEG file and store it in ptr filename
            output_file = fopen(filename, "w");     // Open new JPEG file with generated name

            if (output_file == NULL)
            {
                fclose(raw_file);
                printf("Could not create %s.\n", filename); // Print error if new JPEG file cannot be created
                return 3;
            }
            jpg_count++;
        }
        // Write current block to current JPEG file if one is open
        if (found_jpg)
        {
            fwrite(buffer, BLOCK_SIZE, 1, output_file);
        }
    }

    free(filename);     // Ensure no memory leak

    // Close the forensic file if one was open
    fclose(raw_file);
    if (found_jpg)
    {
        fclose(output_file);
    }

    return 0;
}