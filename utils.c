/**
 * @file utils.c
 * @brief Implementations of helper utility functions.
 */
#include <stdio.h>
#include <string.h>
#include "utils.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int read_integer(const char* prompt) {
    int value;
    printf("%s", prompt);
    
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Please enter a valid whole integer.\n");
        clear_input_buffer();
        printf("%s", prompt);
    }
    
    clear_input_buffer();
    return value;
}

double read_double(const char* prompt) {
    double value;
    printf("%s", prompt);
    
    while (scanf("%lf", &value) != 1) {
        printf("Invalid input. Please enter a valid decimal number.\n");
        clear_input_buffer();
        printf("%s", prompt);
    }
    
    clear_input_buffer();
    return value;
}

void read_string(const char* prompt, char* buffer, int max_length) {
    printf("%s", prompt);
    
    if (fgets(buffer, max_length, stdin) != NULL) {
        size_t len = strlen(buffer);
        // Replace trailing newline with null terminator
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Buffer filled but line didn't end, clear remainder
            clear_input_buffer();
        }
    }
}