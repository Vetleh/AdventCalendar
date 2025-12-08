#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../utilities/file_utils.h"

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

#define MAX_RANGES 200
#define MAX_INGS 1000

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    long range[MAX_RANGES][2];
    char *ingredients[MAX_INGS];
    size_t range_count = 0, ingredients_count = 0;

    char *seperator = strstr(buffer, "\n\n");

    *seperator = '\0';
    char *ranges_part = buffer;
    char *ingredients_offset = seperator + 2;

    char *line = strtok(ranges_part, "\n");
    while (line && range_count < MAX_RANGES)
    {
        char *dash = strchr(line, '-');
        *dash = '\0';

        range[range_count][0] = atol(line);
        range[range_count][1] = atol(dash + 1);

        range_count++;
        line = strtok(NULL, "\n");
    }

    line = strtok(ingredients_offset, "\n");
    while (line && ingredients_count < MAX_INGS)
    {
        ingredients[ingredients_count++] = line;
        line = strtok(NULL, "\n");
    }

    long fresh_range_sum = 0;

    qsort(range, range_count, sizeof range[0], compareRows);

    for (size_t i = 0; i < range_count; i++)
    {
        long start = range[i][0];
        long end = range[i][1];

        for (size_t j = i + 1; j < range_count; j++)
        {
            long start_check = range[j][0];
            long end_check = range[j][1];
            if (end >= start_check)
            {
                if (end_check >= end)
                {
                    end = end_check;
                }

                i++;
            }
            else
            {
                break;
            }
        }

        fresh_range_sum += (end - start) + 1;
    }

    assert(fresh_range_sum == 357907198933892);

    return 0;
}
