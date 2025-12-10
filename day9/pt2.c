#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>

#include "../utilities/file_utils.h"

#define N 100000

long rectangle_size(long x1, long y1, long x2, long y2)
{
    return (labs(x1 - x2) + 1) * (labs(y1 - y2) + 1);
}

static inline void set_X(char (*coords_map)[N], long x, long y, long *row_min, long *row_max, long *col_min, long *col_max)
{
    coords_map[x][y] = 'X';

    if (y < row_min[x])
    {
        row_min[x] = y;
    }

    if (y > row_max[x])
    {
        row_max[x] = y;
    }

    if (x < col_min[y])
    {
        col_min[y] = x;
    }

    if (x > col_max[y])
    {
        col_max[y] = x;
    }
}

void follow_point(char (*coords_map)[N],
                  long x1, long y1, long x2, long y2,
                  long *row_min, long *row_max,
                  long *col_min, long *col_max)
{
    if (x1 == x2)
    {
        long step = (y2 > y1) ? 1 : -1;
        long y = y1;

        while (y != y2)
        {
            set_X(coords_map, x1, y, row_min, row_max, col_min, col_max);
            y += step;
        }

        set_X(coords_map, x1, y2, row_min, row_max, col_min, col_max);
    }
    else
    {
        long step = (x2 > x1) ? 1 : -1;
        long x = x1;

        while (x != x2)
        {
            set_X(coords_map, x, y1, row_min, row_max, col_min, col_max);
            x += step;
        }

        set_X(coords_map, x2, y1, row_min, row_max, col_min, col_max);
    }
}

static inline int is_valid_point(const long *row_min, const long *row_max, const long *col_min, const long *col_max, long x, long y)
{
    return (row_min[x] <= y) && (row_max[x] >= y) && (col_min[y] <= x) && (col_max[y] >= x);
}

int valid_rectangle(const long *row_min, const long *row_max, const long *col_min, const long *col_max, long x1, long y1, long x2, long y2)
{
    long minx = (x1 < x2) ? x1 : x2;
    long maxx = (x1 > x2) ? x1 : x2;
    long miny = (y1 < y2) ? y1 : y2;
    long maxy = (y1 > y2) ? y1 : y2;

    if (!is_valid_point(row_min, row_max, col_min, col_max, minx, miny))
    {
        return 0;
    }

    if (!is_valid_point(row_min, row_max, col_min, col_max, minx, maxy))
    {
        return 0;
    }

    if (!is_valid_point(row_min, row_max, col_min, col_max, maxx, miny))
    {
        return 0;
    }

    if (!is_valid_point(row_min, row_max, col_min, col_max, maxx, maxy))
    {
        return 0;
    }

    for (long x = minx; x <= maxx; x++)
    {
        if (!is_valid_point(row_min, row_max, col_min, col_max, x, miny))
        {
            return 0;
        }

        if (!is_valid_point(row_min, row_max, col_min, col_max, x, maxy))
        {
            return 0;
        }
    }

    for (long y = miny + 1; y <= maxy - 1; y++)
    {
        if (!is_valid_point(row_min, row_max, col_min, col_max, minx, y))
        {
            return 0;
        }

        if (!is_valid_point(row_min, row_max, col_min, col_max, maxx, y))
        {
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    size_t rows = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            rows++;
        }
    }

    if (size > 0 && buffer[size - 1] != '\n')
    {
        rows++;
    }

    long (*coords)[2] = malloc(rows * sizeof *coords);

    char (*coords_map)[N] = malloc((size_t)N * sizeof *coords_map);

    memset(coords_map, '.', (size_t)N * sizeof *coords_map);

    long *row_min = malloc((size_t)N * sizeof(long));
    long *row_max = malloc((size_t)N * sizeof(long));
    long *col_min = malloc((size_t)N * sizeof(long));
    long *col_max = malloc((size_t)N * sizeof(long));

    for (long i = 0; i < N; i++)
    {
        row_min[i] = N;
        row_max[i] = -1;
        col_min[i] = N;
        col_max[i] = -1;
    }

    size_t point_count = 0;
    char *line = strtok(buffer, "\n");
    while (line && point_count < rows)
    {
        char *c1 = strchr(line, ',');
        coords[point_count][0] = atol(line);
        coords[point_count][1] = atol(c1 + 1);

        point_count++;
        line = strtok(NULL, "\n");
    }

    for (size_t i = 0; i < rows; i++)
    {
        follow_point(coords_map,
                     coords[i][0], coords[i][1],
                     coords[(i + 1) % rows][0], coords[(i + 1) % rows][1],
                     row_min, row_max, col_min, col_max);
    }

    long largest_area = 0;

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = rows - 1; j > i; j--)
        {
            long area = rectangle_size(
                coords[i][0], coords[i][1],
                coords[j][0], coords[j][1]);

            if (area <= largest_area)
                continue;

            if (valid_rectangle(row_min, row_max, col_min, col_max,
                                coords[i][0], coords[i][1],
                                coords[j][0], coords[j][1]))
            {
                largest_area = area;
            }
        }
    }

    assert(largest_area == 1508918480);

    return 0;
}
