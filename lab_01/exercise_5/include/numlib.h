#ifndef NUMLIB_H
#define NUMLIB_H

/**
 * @brief Puts the word representation of num in str (ex: 11528 => "one one five
 * two eight").
 *
 * @param num
 * @param str
 *
 * @return char*
 *
 * @details This method is *supposed* to store the memory block of the new
 * string into str. However, in C, you cannot malloc() on an existing pointer.
 * This means you would need to malloc the necessary memory BEFORE calling this
 * method, which would conflict with the guidelines of this exercsie, which say
 * to allocate the memory in the method itself.
 */
char *NumToStr(int num, char *str);

#endif /* NUMLIB_H */