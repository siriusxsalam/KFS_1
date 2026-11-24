#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

#define SCREEN_COUNT    3
#define SCREEN_TERMINAL 0
#define SCREEN_SYSTEM   1
#define SCREEN_DEBUG    2

enum vga_color
{
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GRAY = 7,
    VGA_DARK_GRAY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_YELLOW = 14,
    VGA_WHITE = 15
};

void    screen_clear(void);
void    screen_write(const char *txt);
void    screen_write_char(char character);
void    screen_set_color(u8    foreground, u8 background);
void    screen_set_cursor(u16   x, u16 y);
void    screen_switch(u8    screen_id);
u8      screen_get_current(void);

#endif