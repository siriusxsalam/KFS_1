#include "screen.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

// #define VGA_BLACK           0
// #define VGA_BLUE            1
// #define VGA_GREEN           2
// #define VGA_CYAN            3
// #define VGA_RED             4
// #define VGA_MAGENTA         5
// #define VGA_BROWN           6
// #define VGA_LIGHT_GRAY      7
// #define VGA_DARK_GRAY       8
// #define VGA_LIGHT_BLUE      9
// #define VGA_LIGHT_GREEN     10
// #define VGA_LIGHT_CYAN      11
// #define VGA_LIGHT_RED       12
// #define VGA_LIGHT_MAGENTA   13
// #define VGA_YELLOW          14
// #define VGA_WHITE           15

typedef struct  
{
    u16 buffer[80 * 25];
    u16 cursor_x;
    u16 cursor_y;
    u8  foreground;
    u8  background;
    u8  initialized;
} virtual_screen_t;

static virtual_screen_t screens[SCREEN_COUNT];
static u8               current_screen = SCREEN_TERMINAL;

static u16              cursor_x = 0;
static u16              cursor_y = 0;

static u8               foreground = VGA_LIGHT_GRAY;
static u8               background = VGA_BLACK;

static volatile u16     *video = (volatile u16 *)VGA_MEMORY;



static inline void  outb(u16   port, u8   value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void screen_update_cursor(void) {
    u16    position = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(position & 0xFF));
    
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((position >> 8) & 0xFF));
}

static u16 vga_entry(char character) {
    u8 attribute = (background << 4) | foreground;

    return ((u16)attribute << 8) | (u8)character;
}

static void screen_initialize(u8    screen_id) {
    virtual_screen_t    *screen = &screens[screen_id];

    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        u8  attribute = (VGA_BLACK << 4) | VGA_LIGHT_GRAY;
        screen->buffer[i] = ((u16)attribute << 8) | ' ';
    }

    screen->cursor_x = 0;
    screen->cursor_y = 0;
    screen->foreground = VGA_LIGHT_GRAY;
    screen->background = VGA_BLACK;
    screen->initialized = 1;
}

void    screen_set_color(u8    foreg, u8  backg) {
    foreground = foreg & 0x0F;
    background = backg & 0x0F;
}

void    screen_set_cursor(u16  x, u16 y) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        cursor_x = x;
        cursor_y = y;

        screen_update_cursor();
    }
}

void    screen_clear(void) {
    for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++)
        video[i] = vga_entry(' ');
    cursor_x = 0;
    cursor_y = 0;

    screen_update_cursor();
}

static void screen_scroll(void) {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++)
            video[(y - 1) * VGA_WIDTH + x] = video[y * VGA_WIDTH + x];
    }

    for (int x = 0; x < VGA_WIDTH; x++)
        video[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ');
    
    cursor_y = VGA_HEIGHT - 1;
}

static void screen_put_char(char character) {
    if (character == '\b') {
        // Move one position backwards
        if (cursor_x > 0)
            cursor_x--;
        else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }

        //Erase the character at the cursor
        video[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ');
    }
    else if (character == '\n') {
        cursor_x = 0;
        cursor_y++;
    }
    else if (character == '\r')
        cursor_x = 0;
    else {
        video[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(character);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT)
        screen_scroll();

    screen_update_cursor();
}

void screen_write_char(char character)
{
    screen_put_char(character);
}

void    screen_write(const char *txt) {
    for (int i = 0; txt[i] != '\0'; i++)
        screen_put_char(txt[i]);
}

void    screen_switch(u8    screen_id) {
    if (screen_id >= SCREEN_COUNT || screen_id == current_screen)
        return;

    // Save the currently displayed screen
    virtual_screen_t    *old_screen = &screens[current_screen];

    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        old_screen->buffer[i] = video[i];

    old_screen->cursor_x    = cursor_x;
    old_screen->cursor_y    = cursor_y;
    old_screen->foreground  = foreground;
    old_screen->background  = background;
    old_screen->initialized = 1;

    // Initialize the destination screen if this is its first use
    if (!screens[screen_id].initialized)
        screen_initialize(screen_id);

    // Switch to the destination screen
    current_screen = screen_id;

    virtual_screen_t    *new_screen = &screens[current_screen];

    for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        video[i] = new_screen->buffer[i];

    cursor_x    = new_screen->cursor_x;
    cursor_y    = new_screen->cursor_y;
    foreground  = new_screen->foreground;
    background  = new_screen->background;

    screen_update_cursor();
}

u8  screen_get_current(void) {
    return current_screen;
}