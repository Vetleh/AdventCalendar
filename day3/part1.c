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

    int joltage_sum = 0;
    int last_highest_joltage_start = 0;
    int last_highest_joltage_end = 0;

    char number[1];

    for (int j = 0; j <= size; j++)
    {
        number[0] = buffer[j];

        if (buffer[j] == '\n' || buffer[j] == '\0')
        {
            joltage_sum += last_highest_joltage_start * 10 + last_highest_joltage_end;
            last_highest_joltage_start = 0;
            last_highest_joltage_end = 0;
            continue;
        }

        if (atol(number) > last_highest_joltage_start && buffer[j + 1] != '\n' && buffer[j + 1] != '\0')
        {
            last_highest_joltage_start = atol(number);
            last_highest_joltage_end = 0;
        }
        else if (atol(number) > last_highest_joltage_end)
        {
            last_highest_joltage_end = atol(number);
        }
    }

    assert(joltage_sum == 17535);

    return 0;
}