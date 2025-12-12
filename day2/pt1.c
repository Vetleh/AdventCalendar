#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../utilities/file_utils.h"

#define MAX_ROWS 256
#define MAX_LEN 32

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);
    int count = 0;

    char *pairs[MAX_ROWS][2];

    char *pair = strtok(buffer, ",");
    while (pair && count < MAX_ROWS)
    {
        char *dash = strchr(pair, '-');
        if (!dash)
            break;

        *dash = '\0';

        pairs[count][0] = pair;
        pairs[count][1] = dash + 1;

        count++;
        pair = strtok(NULL, ",");
    }

    char str[20];
    long sum_invalid_pt1 = 0;

    for (int i = 0; i < count; i++)
    {
        for (long k = atol(pairs[i][0]); k <= atol(pairs[i][1]); k++)
        {
            snprintf(str, sizeof str, "%ld", k);
            int str_len = strlen(str);
            if (str_len % 2 != 0)
            {
                continue;
            }

            int half_length = str_len / 2;
            if (memcmp(str, str + half_length, half_length) == 0)
            {
                sum_invalid_pt1 += k;

                long step = pow(10.0, half_length);
                k += step;
            };
        }
    }

    assert(sum_invalid_pt1 == 21898734247);
}
