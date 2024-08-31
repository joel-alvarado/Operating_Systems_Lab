#ifndef STR_HANDLER_H
#define STR_HANDLER_H

/**
 * @brief Searches and replaces all occurences of find in src with replacement
 * in dst.
 *
 * @param src: source string
 * @param dst: replaced string
 * @param find: char to find
 * @param replacement: char to replace with
 */
void Replace(char *src, char *dst, char find, char replacement);

/**
 * @brief Searches and inserts ins into all occurences of find from src into
 * dst.
 *
 * @param src
 * @param dst
 * @param find
 * @param ins
 */
void Insert(char *src, char *dst, char find, char *ins);

/**
 * @brief Replace char src in string dst at position pos.
 *
 * @param dst
 * @param src
 * @param pos
 */
void ReplaceAt(char *dst, char src, int pos);

/**
 * @brief Insert string src into string dst at position pos.
 *
 * @param dst
 * @param src
 * @param pos
 */
void InsertAt(char *dst, char *src, int pos);

#endif /* STR_HANDLER_H */