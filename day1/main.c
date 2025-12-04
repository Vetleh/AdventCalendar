#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

    int current_position = 50;
    char command[5];
    int command_counter = 0;
    int rotations = 0;
    int points_to_zero = 0;
    for (i = 0; i < file_size + 1; i++)
    {
        int last_position = current_position;
        if (buffer[i] == '\n' || buffer[i] == '\0')
        {
            int movement = atoi(command + 1);
            if (command[0] == 'R')
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

            if (current_position == 0)
            {
                points_to_zero++;
            }

            command_counter = 0;
            memset(command, '\0', sizeof(command));
        }
        else
        {
            command[command_counter] = buffer[i];
            command_counter++;
        }
    }
    printf("Points to zero: %d\n", points_to_zero);
    printf("Rotations: %d\n", rotations);
}