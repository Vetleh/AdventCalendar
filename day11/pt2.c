#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#include "../utilities/file_utils.h"

#define MAX_EDGES 25

static int find_idx(char node_names[][4], size_t rows, const char *name)
{
    for (size_t i = 0; i < rows; i++)
    {
        if (strcmp(node_names[i], name) == 0)
            return (int)i;
    }
    return -1;
}

uint64_t follow_edge(
    char node_names[][4],
    char edges[][MAX_EDGES][4],
    size_t rows,
    const char *curr_node,
    int fft,
    int dac,
    int visited[],
    uint64_t memo[][4])
{
    if (strcmp(curr_node, "out") == 0)
    {
        return (fft && dac);
    }

    if (strcmp(curr_node, "fft") == 0)
    {
        fft = 1;
    }

    if (strcmp(curr_node, "dac") == 0)
    {
        dac = 1;
    }

    int idx = find_idx(node_names, rows, curr_node);

    if (visited[idx])
    {
        return 0;
    }

    int state = (fft << 1) | dac;

    if (memo[idx][state] != 1)
    {
        return memo[idx][state];
    }

    visited[idx] = 1;

    uint64_t sum = 0;
    for (int edge = 0; edge < MAX_EDGES && edges[idx][edge][0] != '\0'; edge++)
    {
        sum += follow_edge(node_names, edges, rows, edges[idx][edge],
                           fft, dac, visited, memo);
    }

    visited[idx] = 0;

    memo[idx][state] = sum;
    return sum;
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    size_t rows = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
            rows++;
    }

    if (size > 0 && buffer[size - 1] != '\n')
        rows++;

    char node_names[rows][4];
    memset(node_names, '\0', sizeof(node_names));

    char edges[rows][MAX_EDGES][4];
    memset(edges, 0, sizeof(edges));

    uint64_t memo[rows][4];
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            memo[i][j] = 1;
        }
    }

    size_t row = 0;

    char *saveptr_line;
    char *line = strtok_r(buffer, "\n", &saveptr_line);

    while (line && row < rows)
    {
        node_names[row][0] = line[0];
        node_names[row][1] = line[1];
        node_names[row][2] = line[2];
        node_names[row][3] = '\0';

        char *colon = strchr(line, ':');

        if (colon)
        {
            char *rest = colon + 1;

            while (*rest == ' ')
                rest++;

            char *saveptr_edge;
            char *tok = strtok_r(rest, " ", &saveptr_edge);
            int e = 0;

            while (tok && e < MAX_EDGES)
            {
                edges[row][e][0] = tok[0];
                edges[row][e][1] = tok[1];
                edges[row][e][2] = tok[2];
                edges[row][e][3] = '\0';

                e++;
                tok = strtok_r(NULL, " ", &saveptr_edge);
            }
        }

        row++;
        line = strtok_r(NULL, "\n", &saveptr_line);
    }

    int visited[rows];
    memset(visited, 0, sizeof(visited));

    uint64_t sum = follow_edge(node_names, edges, rows, "svr", 0, 0, visited, memo);

    assert(sum == 349322478796032);

    return 0;
}
