#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>

char *read_text_file(const char *path, size_t *out_size)
{
    if (out_size)
        *out_size = 0;

    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;

    if (fseek(f, 0, SEEK_END) != 0)
    {
        fclose(f);
        return NULL;
    }

    long sz = ftell(f);
    if (sz < 0)
    {
        fclose(f);
        return NULL;
    }

    rewind(f);

    char *buf = (char *)malloc((size_t)sz + 1);
    if (!buf)
    {
        fclose(f);
        return NULL;
    }

    size_t read = fread(buf, 1, (size_t)sz, f);
    fclose(f);

    buf[read] = '\0';

    if (out_size)
        *out_size = read;
    return buf;
}
