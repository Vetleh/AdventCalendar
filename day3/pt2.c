#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#include "../utilities/file_utils.h"

int main(void)
{
    size_t size = 0;
    char *buffer = read_text_file("input.txt", &size);

    long joltage_sum = 0;
    long choices_arr[12] = {0};
    long joltage_arr[12] = {0};

    char number[2];

    int digits_in_string = 0;
    while (1)
    {
        if (buffer[digits_in_string] == '\n')
        {
            break;
        }
        digits_in_string++;
    }

    int iteration_space = digits_in_string - 11;

    for (size_t j = 0; j <= size; j += digits_in_string + 1)
    {
        for (int k = 0; k < 12; k++)
        {
            int start = k;
            if (k > 0)
            {
                start = choices_arr[k - 1] + 1;
            }

            for (int l = start; l < k + iteration_space; l++)
            {
                number[0] = buffer[j + l];
                number[1] = '\0';
                if (joltage_arr[k] < atol(number))
                {

                    if (atol(number) > 9)
                    {
                        joltage_arr[k] = floor(atol(number) / 10);
                        continue;
                    }

                    joltage_arr[k] = atol(number);
                    choices_arr[k] = l;
                }
            }
        }

        for (int k = 0; k < 12; k++)
        {
            joltage_sum += joltage_arr[k] * pow(10, 11 - k);
            joltage_arr[k] = 0;
        }
        continue;
    }

    assert(joltage_sum == 173577199527257);

    return 0;
}
