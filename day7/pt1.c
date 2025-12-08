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

    int res = 0;

    int s_idx = 0;

    int row_width = 0;
    for (size_t i = 0; i < size; i++)
    {
        row_width++;
        if (buffer[i] == '\n')
        {
            break;
        }
    }
    int rows = size / row_width;

    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            continue;
        }
        if (i - row_width > 0 && buffer[i - row_width] == '|' || buffer[i - row_width] == 'S')
        {
            if (buffer[i] == '^')
            {
                res++;
                buffer[i - 1] = '|';
                buffer[i + 1] = '|';
            }
            else
            {
                buffer[i] = '|';
            }
        }
    }

    assert(res == 1553);
    free(buffer);
    return 0;
}
