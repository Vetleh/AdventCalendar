#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
    FILE *fptr;
    char filename[] = "input.txt";

    fptr = fopen(filename, "r");

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    rewind(fptr);

    char *buffer = (char *)malloc(file_size + 1);

    int i = 0;
    int ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        buffer[i++] = (char)ch;
    }
    buffer[i] = '\0';
    int number_buffer_index = 0;
    int reading_start = 1;
    char range_start_number[50];
    char range_end_number[50];
    char number_to_check[50];
    long sum = 0;
    long sum2 = 0;

    for (int j = 0; j < i + 1; j++)
    {
        if (buffer[j] == '-')
        {
            reading_start = 0;
            number_buffer_index = 0;
            continue;
        }
        if (buffer[j] == ',' || buffer[j] == '\0')
        {
            reading_start = 1;
            number_buffer_index = 0;
            long start_range = atol(range_start_number);
            long end_range = atol(range_end_number);

            for (long k = start_range; k <= end_range; k++)
            {
                sprintf(number_to_check, "%ld", k);
                int length_of_number = strlen(number_to_check);

                // Part 1
                if (length_of_number % 2 != 0)
                {
                    continue;
                }

                int half_length = length_of_number / 2;
                for (int m = 0; m < half_length; m++)
                {
                    if (number_to_check[m] != number_to_check[m + half_length])
                    {
                        break;
                    }
                    else if (m == half_length - 1)
                    {
                        sum += k;
                    }
                }
            }
            memset(range_start_number, '\0', sizeof(range_start_number));
            memset(range_end_number, '\0', sizeof(range_end_number));
            continue;
        }
        if (reading_start)
        {
            range_start_number[number_buffer_index] = buffer[j];
            number_buffer_index++;
        }
        else
        {
            range_end_number[number_buffer_index] = buffer[j];
            number_buffer_index++;
        }
    }
    printf("Sum part 1: %ld\n", sum);
    printf("Sum part 2: %ld\n", sum2);
}