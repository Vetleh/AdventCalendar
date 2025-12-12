#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../utilities/file_utils.h"

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);
    int line_length = 0;
    while (1)
    {
        if (buffer[line_length] == '\n')
        {
            line_length++;
            break;
        }
        line_length++;
    }

    int items_removed = 0;

    int column_length = (int)(size / line_length) + 1;

    for (int j = 0; j < column_length; j++)
    {
        int line_before = (j - 1) * line_length;
        int current_line = j * line_length;
        int line_after = (j + 1) * line_length;
        for (int k = 0; k < line_length; k++)
        {
            int neighbors = 0;
            if (buffer[j * line_length + k] != '@')
            {
                continue;
            }

            // above
            if (j - 1 >= 0)
            {
                for (int m = -1; m < 2; m++)
                {
                    if (k + m >= 0)
                    {
                        if (buffer[line_before + k + m] == '@')
                            neighbors++;
                    }
                }
            }

            // same line
            for (int m = -1; m < 2; m++)
            {
                if (k + m >= 0 && m != 0)
                {
                    if (buffer[current_line + k + m] == '@')
                        neighbors++;
                }
            }

            // below
            if (j + 1 < column_length)
            {
                for (int m = -1; m < 2; m++)
                {
                    if (k + m >= 0)
                    {
                        if (buffer[line_after + (k + m)] == '@')
                            neighbors++;
                    }
                }
            }

            if (neighbors < 4)
            {
                items_removed++;
            }
        }
    }

    assert(items_removed == 1516);

    return 0;
}
