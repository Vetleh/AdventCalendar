#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>

char *read_text_file(const char *path, size_t *out_size);
#endif
