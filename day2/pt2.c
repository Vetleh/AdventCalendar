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
    long sum_invalid_pt2 = 0;
    int partition = 1;

    for (int i = 0; i < count; i++)
    {
        for (long j = atol(pairs[i][0]); j <= atol(pairs[i][1]); j++)
        {
            snprintf(str, sizeof str, "%ld", j);
            int str_len = strlen(str);
            while (1)
            {
                if (partition > 5)
                    break;

                if (str_len % partition != 0)
                {
                    partition++;
                    continue;
                }

                if (partition == str_len)
                    break;

                int is_invalid = 1;

                for (int k = partition; k < str_len; k += partition)
                {
                    if (memcmp(str, str + k, partition) != 0)
                    {
                        is_invalid = 0;
                        break;
                    }
                }

                if (is_invalid)
                {
                    sum_invalid_pt2 += j;
                    if (str_len % 2)
                    {
                        long step = pow(10.0, str_len / 2);
                        j += step;
                    }

                    break;
                }

                partition++;
            }
            partition = 1;
        }
    }

    assert(sum_invalid_pt2 == 28915664389);
}
