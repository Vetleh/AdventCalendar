#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../utilities/file_utils.h"

static long follow_timeline(const char *buffer, size_t size, int start_idx, int cols, long *memo)
{
    if (memo[start_idx] != -1)
        return memo[start_idx];

    for (int idx = start_idx; idx < (int)size; idx += cols)
    {
        if (buffer[idx] == '^')
        {
            long left = follow_timeline(buffer, size, idx + cols - 1, cols, memo);
            long right = follow_timeline(buffer, size, idx + cols + 1, cols, memo);

            memo[start_idx] = left + right;
            return memo[start_idx];
        }
    }

    memo[start_idx] = 1;
    return 1;
}

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    int cols = 0;
    while (1)
    {
        if (buffer[cols] == '\n')
        {
            cols++;
            break;
        }
        cols++;
    }

    long *memo = malloc(size * sizeof(long));

    for (size_t i = 0; i < size; i++)
    {
        memo[i] = -1;
    }

    long res = 0;

    for (int i = 0; i < (int)size; i++)
    {
        if (buffer[i] == 'S')
        {
            int start = i + cols;
            res = follow_timeline(buffer, size, start, cols, memo);
            break;
        }
    }

    assert(res == 15811946526915);

    free(memo);
    free(buffer);
    return 0;
}
