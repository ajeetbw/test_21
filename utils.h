/**
 * @file utils.h
 * @brief Helper utility functions for safe console input.
 */
#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Clears standard input buffer to prevent infinite loops on bad input.
 */
void clear_input_buffer();

/**
 * @brief Safely reads an integer from the user.
 * 
 * @param prompt The message to display to the user.
 * @return The validated integer.
 */
int read_integer(const char* prompt);

/**
 * @brief Safely reads a double from the user.
 * 
 * @param prompt The message to display to the user.
 * @return The validated double.
 */
double read_double(const char* prompt);

/**
 * @brief Safely reads a string from the user, removing the trailing newline.
 * 
 * @param prompt The message to display to the user.
 * @param buffer The character array to store the string.
 * @param max_length The maximum number of characters to read.
 */
void read_string(const char* prompt, char* buffer, int max_length);

#endif // UTILS_H