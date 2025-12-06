#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../utilities/file_utils.h"

int main()
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

    while (1)
    {
        int has_changes = 0;
        for (int j = 0; j < (size / line_length) + 1; j++)
        {
            for (int k = 0; k < line_length; k++)
            {
                int neighbors = 0;
                if (buffer[j * line_length + k] != '@')
                {
                    continue;
                }

                int line_before = (j - 1) * line_length;
                int current_line = j * line_length;
                int line_after = (j + 1) * line_length;

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
                if (j + 1 < (size / line_length) + 1)
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
                    has_changes = 1;
                    buffer[current_line + k] = '.';
                    items_removed++;
                }
            }
        }
        if (!has_changes)
        {
            break;
        }
    }

    assert(items_removed == 9122);

    return 0;
}