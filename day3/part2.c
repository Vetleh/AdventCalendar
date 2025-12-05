#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int compareRows(const void *a, const void *b)
{
    const long (*rowA)[2] = a;
    const long (*rowB)[2] = b;

    if ((*rowA)[0] < (*rowB)[0])
        return -1;
    if ((*rowA)[0] > (*rowB)[0])
        return 1;
    return 0;
}

int main()
{
    FILE *fptr;
    char filename[] = "input.txt";

    fptr = fopen(filename, "r");

    if (!fptr)
    {
        perror("fopen");
        return 1;
    }

    if (fseek(fptr, 0, SEEK_END) != 0)
    {
        perror("fseek");
        fclose(fptr);
        return 1;
    }

    long file_size = ftell(fptr);
    if (file_size < 0)
    {
        perror("ftell");
        fclose(fptr);
        return 1;
    }

    rewind(fptr);

    char *buffer = (char *)malloc(file_size + 1);
    long i = 0;
    long ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        buffer[i++] = (char)ch;
    }
    buffer[i] = '\0';

    long joltage_sum = 0;
    long choices_arr[12] = {0};
    long joltage_arr[12] = {0};

    char number[2];

    int digits_in_string = 0;
    while (1)
    {
        if (buffer[digits_in_string] == '\n')
        {
            break;
        }
        digits_in_string++;
    }

    int idx_processing = 0;
    int iteration_space = digits_in_string - 11;

    for (int j = 0; j <= i; j += digits_in_string + 1)
    {
        for (int k = 0; k < 12; k++)
        {
            int start = k;
            if (k > 0)
            {
                start = choices_arr[k - 1] + 1;
            }

            // printf("start: %d choices: %ld\n", start, choices_arr[k - 1]);

            for (int l = start; l < k + iteration_space; l++)
            {
                number[0] = buffer[j + l];
                number[1] = '\0';
                if (joltage_arr[k] < atol(number))
                {

                    if (atol(number) > 9)
                    {
                        joltage_arr[k] = floor(atol(number) / 10);
                        continue;
                    }

                    joltage_arr[k] = atol(number);
                    choices_arr[k] = l;
                }
            }
        }

        for (int k = 0; k < 12; k++)
        {
            // printf("%ld", joltage_arr[k]);
            joltage_sum += joltage_arr[k] * pow(10, 11 - k);
            joltage_arr[k] = 0;
        }
        // printf("\n");

        idx_processing = 0;
        continue;
    }

    printf("%ld", joltage_sum);

    return 0;
}