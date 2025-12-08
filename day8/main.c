#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../utilities/file_utils.h"

static long calculate_distance(long x1, long y1, long z1, long x2, long y2, long z2)
{
    double dx_squared = pow(x2 - x1, 2);
    double dy_squared = pow(y2 - y1, 2);
    double dz_squared = pow(z2 - z1, 2);

    return sqrt(dx_squared + dy_squared + dz_squared);
}

static long (*parse_coords(char *buffer, size_t size, size_t *out_rows))[3]
{
    int rows = 1;
    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            rows++;
        }
    }

    long (*coords)[3] = malloc(rows * sizeof *coords);
    int point_count = 0;

    char *line = strtok(buffer, "\n");
    while (line && point_count < rows)
    {
        char *c1 = strchr(line, ',');
        char *c2 = strchr(c1 + 1, ',');

        coords[point_count][0] = atol(line);
        coords[point_count][1] = atol(c1 + 1);
        coords[point_count][2] = atol(c2 + 1);

        point_count++;
        line = strtok(NULL, "\n");
    }

    *out_rows = point_count;
    return coords;
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    size_t count = 0;
    long (*coords)[3] = parse_coords(buffer, size, &count);

    for (size_t i = 0; i < count; ++i)
    {
        printf("%zu: %ld, %ld, %ld\n", i,
               coords[i][0], coords[i][1], coords[i][2]);
    }

    free(coords);
    free(buffer);
    return 0;
}
