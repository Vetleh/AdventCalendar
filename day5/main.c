#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

    char range_start_number[30];
    char range_end_number[30];
    int reading_start = 1;
    int number_buffer_index = 0;

    int ingredients_start = 0;

    for (int j = 0; j < i; j++)
    {
        if (buffer[j] == '\n' && buffer[j + 1] == '\n')
        {
            ingredients_start = j + 2;
            break;
        }
    }

    char curr_ingredient[30];
    int counter = 0;
    int fresh_ingredients = 0;

    for (long j = ingredients_start; j < i - 1; j++)
    {
        if (buffer[j] != '\n')
        {
            curr_ingredient[counter] = buffer[j];
            counter++;
            continue;
        }
        else
        {
            counter = 0;
        }

        long ingredient = atol(curr_ingredient);

        for (long k = 0; k < ingredients_start - 2; k++)
        {
            if (buffer[k] == '-')
            {
                reading_start = 0;
                number_buffer_index = 0;
                continue;
            }
            if (buffer[k] == '\n')
            {
                reading_start = 1;
                number_buffer_index = 0;
                long start_range = atol(range_start_number);
                long end_range = atol(range_end_number);

                if (ingredient >= start_range && ingredient <= end_range)
                {
                    fresh_ingredients++;
                    memset(range_start_number, '\0', sizeof(range_start_number));
                    memset(range_end_number, '\0', sizeof(range_end_number));
                    memset(curr_ingredient, '\0', sizeof(curr_ingredient));
                    counter = 0;
                    break;
                }

                memset(range_start_number, '\0', sizeof(range_start_number));
                memset(range_end_number, '\0', sizeof(range_end_number));
                memset(curr_ingredient, '\0', sizeof(curr_ingredient));
                counter = 0;
                continue;
            }

            if (reading_start)
            {
                range_start_number[number_buffer_index] = buffer[k];
                number_buffer_index++;
            }
            else
            {
                range_end_number[number_buffer_index] = buffer[k];
                number_buffer_index++;
            }
        }

        memset(range_start_number, '\0', sizeof(range_start_number));
        memset(range_end_number, '\0', sizeof(range_end_number));
        memset(curr_ingredient, '\0', sizeof(curr_ingredient));
    }

    long number_ranges[i][2];
    for (int j = 0; j < i; j++)
    {
        number_ranges[j][0] = 0;
        number_ranges[j][1] = 0;
    }
    reading_start = 1;
    int number_range_counter = 0;
    long fresh_range_sum = 0;
    number_buffer_index = 0;
    for (long k = 0; k < ingredients_start - 1; k++)
    {
        if (buffer[k] == '-')
        {
            reading_start = 0;
            number_buffer_index = 0;
            continue;
        }
        if (buffer[k] == '\n' || buffer[k] == '\0')
        {
            reading_start = 1;
            number_buffer_index = 0;
            long start_range = atol(range_start_number);
            long end_range = atol(range_end_number);
            number_ranges[number_range_counter][0] = start_range;
            number_ranges[number_range_counter][1] = end_range;
            number_range_counter++;

            memset(range_start_number, '\0', sizeof(range_start_number));
            memset(range_end_number, '\0', sizeof(range_end_number));
            continue;
        }

        if (reading_start)
        {
            range_start_number[number_buffer_index] = buffer[k];
            number_buffer_index++;
        }
        else
        {
            range_end_number[number_buffer_index] = buffer[k];
            number_buffer_index++;
        }
    }

    int iter = 0;

    qsort(number_ranges, i, sizeof number_ranges[0], compareRows);

    for (long j = 0; j < i; j++)
    {
        if (number_ranges[j][0] == 0)
            continue;

        iter++;
    }
    int skipidx = 0;
    for (long j = 0; j < i; j++)
    {
        if (number_ranges[j][0] == 0)
            continue;

        long start = number_ranges[j][0];
        long end = number_ranges[j][1];

        for (long k = j + 1; k < i; k++)
        {
            long start_check = number_ranges[k][0];
            long end_check = number_ranges[k][1];
            if (end >= start_check)
            {
                if (end_check >= end)
                {
                    end = end_check;
                }

                j++;
            }
            else
            {
                break;
            }
        }

        fresh_range_sum += (end - start) + 1;
    }

    printf("Number of fresh ingredients: %d ", fresh_ingredients);
    printf("Sum fresh range: %ld ", fresh_range_sum);

    return 0;
}