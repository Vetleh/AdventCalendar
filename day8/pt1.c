#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>

#include "../utilities/file_utils.h"

typedef struct
{
    int a, b;
    unsigned long d2;
} Edge;

typedef long coord3_t[3];

static void update_topN(int *top, int n, int x)
{
    for (int i = 0; i < n; i++)
    {
        if (x >= top[i])
        {
            for (int k = n - 1; k > i; k--)
                top[k] = top[k - 1];
            top[i] = x;
            return;
        }
    }
}

static coord3_t *parse_coords(char *buffer, size_t size, size_t *out_rows)
{
    int rows = 1;
    for (size_t i = 0; i < size; i++)
        if (buffer[i] == '\n')
            rows++;

    long (*coords)[3] = malloc((size_t)rows * sizeof *coords);
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

    *out_rows = (size_t)point_count;
    return coords;
}

static unsigned long dist(long x1, long y1, long z1, long x2, long y2, long z2)
{
    long dx = x2 - x1;
    long dy = y2 - y1;
    long dz = z2 - z1;

    long acc = 0;
    acc += dx * dx;
    acc += dy * dy;
    acc += dz * dz;

    return acc;
}

static int cmp_edge(const void *p, const void *q)
{
    const Edge *e1 = (const Edge *)p;
    const Edge *e2 = (const Edge *)q;
    if (e1->d2 < e2->d2)
    {
        return -1;
    }

    if (e1->d2 > e2->d2)
    {
        return 1;
    }
    return 0;
}

static int find_root(int *parent, int node)
{
    while (parent[node] != node)
    {
        parent[node] = parent[parent[node]];
        node = parent[node];
    }
    return node;
}

static void union_by_size(int *parent, int *component_size, int node_a, int node_b)
{
    int root_a = find_root(parent, node_a);
    int root_b = find_root(parent, node_b);

    if (root_a == root_b)
        return;

    if (component_size[root_a] < component_size[root_b])
    {
        int tmp = root_a;
        root_a = root_b;
        root_b = tmp;
    }

    parent[root_b] = root_a;
    component_size[root_a] += component_size[root_b];
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    size_t count = 0;
    long (*coords)[3] = parse_coords(buffer, size, &count);

    int n = count;

    size_t m = (size_t)n * (size_t)(n - 1) / 2;
    Edge *edges = malloc(m * sizeof *edges);

    size_t idx = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            edges[idx].a = i;
            edges[idx].b = j;
            edges[idx].d2 = dist(
                coords[i][0], coords[i][1], coords[i][2],
                coords[j][0], coords[j][1], coords[j][2]);
            idx++;
        }
    }

    qsort(edges, m, sizeof *edges, cmp_edge);

    int *parent = malloc((size_t)n * sizeof *parent);
    int *size_array = malloc((size_t)n * sizeof *size_array);

    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
        size_array[i] = 1;
    }

    int amount_to_choose = 1000;

    for (int k = 0; k < amount_to_choose; k++)
    {
        union_by_size(parent, size_array, edges[k].a, edges[k].b);
    }

    int top[3] = {0, 0, 0};

    for (int i = 0; i < n; i++)
    {
        if (find_root(parent, i) == i)
        {
            update_topN(top, 3, size_array[i]);
        }
    }

    assert(top[0] * top[1] * top[2] == 84968);

    return 0;
}
