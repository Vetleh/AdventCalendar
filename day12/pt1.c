#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../utilities/file_utils.h"
#define GRIDS 6

static long parse(const char *p)
{
    char tmp[3];
    tmp[0] = p[0];
    tmp[1] = p[1];
    tmp[2] = '\0';
    return strtol(tmp, NULL, 10);
}

static long down_to_mult3(long x)
{
    long r = x % 3;
    if (r < 0)
        r += 3;
    return x - r;
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    int data_idx = 0;
    size_t start_line = 30;
    size_t rows = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            if (rows == start_line - 1)
                data_idx = i + 1;
            rows++;
        }
    }

    if (size > 0 && buffer[size - 1] != '\n')
        rows++;

    char edges[rows][GRIDS][3];
    memset(edges, 0, sizeof(edges));

    int width = 25;

    int valid_count = 0;

    for (size_t i = 0; i < rows - start_line; i++)
    {
        const char *row = data_idx + buffer + i * width;

        long a = parse(row + 0);
        long b = parse(row + 3);

        long x = down_to_mult3(a);
        long y = down_to_mult3(b);

        long sum =
            parse(row + 7) + parse(row + 10) + parse(row + 13) +
            parse(row + 16) + parse(row + 19) + parse(row + 22);

        long sum_optimistic = sum * 9;

        if (sum_optimistic <= x * y)
        {
            valid_count++;
        }
    }

    assert(valid_count == 472);
}
