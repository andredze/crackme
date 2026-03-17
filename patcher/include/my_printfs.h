#ifndef __ERROR_H__
#define __ERROR_H__

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#define COLOR_RED       "\033[1;31m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_RESET     "\033[1;0m"

//——————————————————————————————————————————————————————————————————————————————————————————

#define BEGIN do {

#define END   } while (0)

//——————————————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(text, ...)                                                              \
        BEGIN                                                                            \
        fprintf(stderr, COLOR_RED "In %s:%d from %s:\nERROR: " text "\n" COLOR_RESET,    \
                 __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);                \
        END

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef DEBUG
    #define DPRINTF(text, ...)                                              \
            BEGIN                                                           \
            fprintf(stderr, COLOR_YELLOW text COLOR_RESET, ##__VA_ARGS__);  \
            END
#else
    #define DPRINTF(...)  ;
#endif /* DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* __ERROR_H__ */
