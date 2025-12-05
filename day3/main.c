#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int compareRows(const void *a, const void *b)
{
    const long (*rowA)[2] = a;
    const long (*rowB)[2] = b;

    if ((*rowA)[0] < (*rowB)[0])
        return -1;
    if ((*rowA)[0] > (*rowB)[0])
        return 1;
    return 0;
}

int main()
{
    FILE *fptr;
    char filename[] = "input.txt";

    fptr = fopen(filename, "r");

    if (!fptr)
    {
        perror("fopen");
        return 1;
    }

    if (fseek(fptr, 0, SEEK_END) != 0)
    {
        perror("fseek");
        fclose(fptr);
        return 1;
    }

    long file_size = ftell(fptr);
    if (file_size < 0)
    {
        perror("ftell");
        fclose(fptr);
        return 1;
    }

    rewind(fptr);

    char *buffer = (char *)malloc(file_size + 1);
    long i = 0;
    long ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        buffer[i++] = (char)ch;
    }
    buffer[i] = '\0';

    int joltage_sum = 0;
    int last_highest_joltage_start = 0;
    int last_highest_joltage_end = 0;

    char number[1];

    for (int j = 0; j <= i; j++)
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

    printf("%d", joltage_sum);

    return 0;
}