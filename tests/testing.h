#ifndef TESTING_H
#define TESTING_H

#define COLOR_RED   "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

#define CHECK(func, cond)                                        \
    if (!(cond)) {                                               \
        printf("Test "func": "COLOR_RED"Failed"COLOR_RESET"\n"); \
        printf(COLOR_RED"Rule: "#cond""COLOR_RESET"\n");         \
        exit(1);                                                 \
    }

#define CHECK_SUCCESS(func) \
    printf("Test "func": "COLOR_GREEN"Success"COLOR_RESET"\n");

#endif
