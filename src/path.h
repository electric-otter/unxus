// src/path.h
#ifndef UNXUS_PATH_H
#define UNXUS_PATH_H

#include <stddef.h>

// Normalize a path in-place (removes "./", "//", etc.)
void path_normalize(char *path);

// Join two paths into result (with normalization)
void path_join(const char *base, const char *add, char *result, size_t maxlen);

#endif // UNXUS_PATH_H
