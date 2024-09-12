#include <stddef.h>
#ifndef TERMLIB_H
#define TERMLIB_H

#define INPUT_MAX_LEN 256
#define MAX_CMD_AMOUNT 64
#define MAX_CMD_LENGTH 64
#define MAX_CMD_FLAG_AMOUNT 64
#define MAX_CMD_FLAG_LENGTH 64

/**
 * @brief Split string src at char split. Will put each token as a seperate
 * string into dst.
 *
 * @param dst
 * @param size
 * @param src
 * @param split
 */
void ssplit(char **dst, size_t size, char *src, char split);

/**
 * @brief Strips src of leading and trailing whitespaces and puts it in dst.
 *
 * @param dst
 * @param src
 */
void strip(char *dst, char *src);

#endif /* TERMLIB_H */