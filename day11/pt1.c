#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>

#include "../utilities/file_utils.h"

#define MAX_EDGES 20

int follow_edge(char node_names[10][4], char edges[10][MAX_EDGES][4], size_t rows, char curr_node[4])
{
    if (strcmp(curr_node, "out") == 0)
    {
        return 1;
    }
    int idx = 0;
    for (size_t i = 0; i < rows; i++)
    {
        if (strcmp(node_names[i], curr_node) == 0)
        {
            idx = i;
            break;
        }
    }

    int counter = 0;
    int sum = 0;
    while (1)
    {
        if (edges[idx][counter][0] == '\0')
        {
            break;
        }
        sum += follow_edge(node_names, edges, rows, edges[idx][counter]);
        counter++;
    }
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

    int sum = follow_edge(node_names, edges, rows, "you");

    assert(sum == 585);

    return 0;
}
