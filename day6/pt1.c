#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../utilities/file_utils.h"

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);
    assert(buffer);

    // count rows properly
    int rows = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
            rows++;
    }

    if (size > 0 && buffer[size - 1] != '\n')
        rows++;

    int cols = 0;
    for (size_t i = 0; i < size && buffer[i] != '\n'; i++)
    {
        if (buffer[i] != ' ' && (i == 0 || buffer[i - 1] == ' '))
            cols++;
    }

    int num_rows = rows - 1;
    long numbers[num_rows][cols];
    char *operation[cols];

    char *tok = strtok(buffer, " \n");
    // TODO write numbers on same line for faster memory access?
    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            numbers[r][c] = atol(tok);
            tok = strtok(NULL, " \n");
        }
    }

    for (int c = 0; c < cols; c++)
    {
        operation[c] = tok;
        tok = strtok(NULL, " \n");
    }

    long sum_total = 0;

    for (int c = 0; c < cols; c++)
    {
        char curr_operation = operation[c][0];
        long sum_column = curr_operation == '*';

        for (int r = 0; r < num_rows; r++)
        {
            switch (curr_operation)
            {
            case '+':
                sum_column += numbers[r][c];
                break;
            case '*':
                sum_column *= numbers[r][c];
                break;
            }
        }
        sum_total += sum_column;
    }
    assert(sum_total == 8108520669952);
    free(buffer);
    return 0;
}
