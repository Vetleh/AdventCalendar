#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fptr;
    char filename[] = "input.txt";

    fptr = fopen(filename, "r");

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    rewind(fptr);

    char *buffer = (char *)malloc(file_size + 1);

    int i = 0;
    int line_length = 0;
    int ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        if (ch == '\n')
        {
            line_length = 0;
        }
        else
        {
            line_length++;
        }
        buffer[i++] = (char)ch;
    }
    buffer[i] = '\0';

    line_length++; // account for newline character
    int items_removed = 0;

    while (1)
    {
        int has_changes = 0;
        for (int j = 0; j < (i / line_length) + 1; j++)
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
                if (j + 1 < (i / line_length) + 1)
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

    printf("valid count: %d\n", items_removed);

    fclose(fptr);
    free(buffer);

    return 0;
}