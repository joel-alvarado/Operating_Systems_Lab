#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H

#define LINE_LENGTH_MAX 40
#define LINE_AMOUNT_MAX 3

/**
 * @brief Get user input from console.
 *
 */
void GetUserInput(char **buf);

/**
 * @brief Align buf to the left of the margin.
 *
 * @param buf
 */
void AlignTextLeft(char **buf);

/**
 * @brief Aligns buf to the right of the margin.
 *
 * @param buf
 */
void AlignTextRight(char **buf);

/**
 * @brief Aligns buf with justified spaces between each word.
 *
 * @param buf
 */
void AlignTextJustify(char **buf);

/**
 * @brief Inserts char src into string dst at position pos.
 *
 * @param dst
 * @param src
 * @param pos
 */
void InsertAt(char *dst, char src, int pos);

#endif /* TEXT_HANDLER_H */