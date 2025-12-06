#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../utilities/file_utils.h"
#define MAX_TOKENS 5000

int main()
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    int current_position = 50, last_position = 50, rotations = 0, points_to_zero = 0;
    char seperator = '\n';
    char delim[2] = {seperator, '\0'};

    char *tokens[MAX_TOKENS];
    int count = 0;

    char *tok = strtok(buffer, delim);
    while (tok && count < MAX_TOKENS)
    {
        tokens[count++] = tok;
        tok = strtok(NULL, delim);
    }

    for (int i = 0; i < count; i++)
    {
        points_to_zero += current_position == 0;
        last_position = current_position;

        int movement = atoi(tokens[i] + 1);

        if (tokens[i][0] == 'R')
        {
            current_position += movement;
            rotations += floor(current_position / 100);
        }
        else
        {
            current_position -= movement;
            rotations += -floor(current_position / 100);
            if (current_position <= 0 && last_position != 0)
            {
                rotations++;
            }
        }

        current_position = current_position % 100;

        if (current_position < 0)
        {
            current_position += 100;
        }
    }

    assert(points_to_zero == 1092);
    assert(rotations == 6616);
}