#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>

#include "../utilities/file_utils.h"

#define N 20

int click_buttons(int buttons[20], int target, int iter, int depth, int button_arr_length, int last_val)
{
    if (iter == depth)
    {
        return last_val == target;
    }

    for (int i = 0; i < button_arr_length; i++)
    {
        int new_val = last_val ^ buttons[i];
        if (click_buttons(buttons, target, iter + 1, depth, button_arr_length, new_val))
        {
            return 1;
        }
    }
    return 0;
}

void parse_joltages(char *buffer, int joltages[][N], size_t *i, int curr_row, int *curr_joltage)
{
    (*i)++;
    int value = 0;

    while (buffer[*i] != '}')
    {
        if (buffer[*i] == ',')
        {
            (*i)++;
            continue;
        }

        char *end;
        long number = strtol(&buffer[*i], &end, 10);

        if (end == &buffer[*i])
        {
            (*i)++;
            continue;
        }

        value |= (1 << number);
        *i = (int)(end - buffer);
    }

    joltages[curr_row][*curr_joltage] = value;
}

void parse_buttons(char *buffer, int buttons[][N], size_t *i, int curr_row, int *curr_button)
{
    (*i)++;
    int value = 0;

    while (buffer[*i] != ')')
    {
        if (buffer[*i] == ',')
        {
            (*i)++;
            continue;
        }

        char *end;
        long number = strtol(&buffer[*i], &end, 10);

        if (end == &buffer[*i])
        {
            (*i)++;
            continue;
        }

        value |= (1 << number);
        *i = (int)(end - buffer);
    }

    buttons[curr_row][*curr_button] = value;
    (*curr_button)++;
}

int main(void)
{
    // size_t size = 0;
    // char *buffer = read_text_file("input.txt", &size);

    // size_t rows = 0;
    // for (size_t i = 0; i < size; i++)
    // {
    //     if (buffer[i] == '\n')
    //         rows++;
    // }

    // if (size > 0 && buffer[size - 1] != '\n')
    //     rows++;

    // int joltages[rows];
    // memset(joltages, 0, sizeof joltages);

    // int buttons[rows][20];
    // memset(buttons, 0, sizeof buttons);

    // size_t curr_row = 0;
    // int curr_button = 0;
    // int curr_joltage = 0;

    // for (size_t i = 0; i < size; i++)
    // {
    //     if (buffer[i] == '\n')
    //     {
    //         curr_button = 0;
    //         curr_row++;
    //         continue;
    //     }

    //     if (buffer[i] == '{')
    //     {
    //         parse_joltages(buffer, joltages, &i, curr_row, &curr_joltage);
    //     }

    //     if (buffer[i] == '(')
    //     {
    //         parse_buttons(buffer, buttons, &i, curr_row, &curr_button);
    //     }
    // }

    // for (size_t i = 0; i < rows; i++)
    // {
    //     for (size_t j = 0; j < 20; j++)
    //     {
    //         printf("%d", joltages[i][&j]);
    //     }
    // }

    // int depth_sums = 0;

    // for (size_t i = 0; i < rows; i++)
    // {
    //     int column_size_row = 0;
    //     while (1)
    //     {
    //         if (buttons[i][column_size_row] == 0)
    //         {
    //             break;
    //         }
    //         column_size_row++;
    //     }

    //     int depth = 1;

    //     while (1)
    //     {

    //         if (click_buttons(buttons[i], answers[i], 0, depth, column_size_row, 0))
    //         {
    //             break;
    //         }

    //         depth++;
    //     }
    //     depth_sums += depth;
    // }
    // assert(depth_sums == 444);
    return 0;
}
