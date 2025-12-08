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

    int rows = 0;
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

    int row_width = 0;
    for (size_t i = 0; i < size; i++)
    {
        row_width++;
        if (buffer[i] == '\n')
        {
            break;
        }
    }

    int cols = 0;
    for (size_t i = 0; i < size && buffer[i] != '\n'; i++)
    {
        if (buffer[i] != ' ' && (i == 0 || buffer[i - 1] == ' '))
        {
            cols++;
        }
    }

    int num_rows = rows - 1;
    long numbers[num_rows][cols];
    char *operation[cols];

    char *tok = strtok(buffer, " \n");
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
    int current_operation_index = 0;
    long sum_column = 0;
    for (int i = 0; i < row_width; i++)
    {
        char *current_operation = operation[current_operation_index];
        char number_string[num_rows + 1];
        int k = 0;

        for (int j = 0; j < num_rows; j++)
        {
            char ch = buffer[j * row_width + i];
            if (ch != ' ' && ch != '\n' && ch != '\0')
            {
                number_string[k++] = ch;
            }
        }

        number_string[k] = '\0';
        if (k == 0)
        {
            sum_total += sum_column;
            sum_column = 0;
            current_operation_index++;
        }
        else
        {
            switch (current_operation[0])
            {
            case '+':
                sum_column += atol(number_string);
                break;
            case '*':
                if (sum_column == 0)
                    sum_column = 1;
                sum_column *= atol(number_string);
                break;
            }
        }
    }
    assert(sum_total == 11708563470209);
    free(buffer);
    return 0;
}
