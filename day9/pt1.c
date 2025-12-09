#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>

#include "../utilities/file_utils.h"

long rectangle_size(long x1, long y1, long x2, long y2)
{
    return (labs(x1 - x2) + 1) * (labs(y1 - y2) + 1);
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);
    size_t count = 0;

    size_t rows = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
            rows++;
    }

    if (size > 0 && buffer[size - 1] != '\n')
        rows++;

    long (*coords)[2] = malloc((size_t)rows * sizeof *coords);
    int point_count = 0;

    char *line = strtok(buffer, "\n");
    while (line && point_count < rows)
    {
        char *c1 = strchr(line, ',');
        char *c2 = strchr(c1 + 1, ',');

        coords[point_count][0] = atol(line);
        coords[point_count][1] = atol(c1 + 1);

        point_count++;
        line = strtok(NULL, "\n");
    }

    long largest_area = 0;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = i + 1; j < rows; j++)
        {
            long size = rectangle_size(coords[i][0], coords[i][1], coords[j][0], coords[j][1]);
            if (size > largest_area)
            {
                largest_area = size;
            }
        }
    }

    // assert(largest_area == 4741848414);

    return 0;
}
