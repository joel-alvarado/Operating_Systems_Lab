#ifndef NUMLIB_H
#define NUMLIB_H

/**
 * @brief Puts the word representation of num in str (ex: 11528 => "one one five
 * two eight").
 *
 * @param num
 *
 * @return char*
 */
char *NumToStr(int num);

/**
 * @brief Returns the amount of digits in num.
 *
 * @param num
 * @return int
 */
int NumLen(int num);

#endif /* NUMLIB_H */