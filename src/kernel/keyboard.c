#include <stdint.h>
#include "keyboard.h"

#define KEYBOARD_DATA_PORT      0x60
#define KEYBOARD_STATUS_PORT    0x64

static inline uint8_t   inb(uint16_t    port) {
    uint8_t value;

    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));

    return  value;
}

static const char   normal_table[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3',
    [0x05] = '4', [0x06] = '5', [0x07] = '6',
    [0x08] = '7', [0x09] = '8', [0x0A] = '9',
    [0x0B] = '0', 
    
    [0x0C] = '-', [0x0D] = '=',
    [0x0E] = '\b', [0x0F] = '\t',
    
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e',
    [0x13] = 'r', [0x14] = 't', [0x15] = 'y',
    [0x16] = 'u', [0x17] = 'i', [0x18] = 'o',
    [0x19] = 'p',
    
    [0x1A] = '[', [0x1B] = ']',
    [0x1C] = '\n',
    
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd',
    [0x21] = 'f', [0x22] = 'g', [0x23] = 'h',
    [0x24] = 'j', [0x25] = 'k', [0x26] = 'l',
    
    [0x27] = ';', [0x28] = '\'',
    [0x29] = '`', [0x2B] = '\\',
    
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c',
    [0x2F] = 'v', [0x30] = 'b', [0x31] = 'n',
    [0x32] = 'm',
    
    [0x33] = ',', [0x34] = '.', [0x35] = '/',
    
    [0x39] = ' ',
};

static const char   shift_table[128] = {
    [0x02] = '!', [0x03] = '@', [0x04] = '#',
    [0x05] = '$', [0x06] = '%', [0x07] = '^',
    [0x08] = '&', [0x09] = '*', [0x0A] = '(',
    [0x0B] = ')', 
    
    [0x0C] = '_', [0x0D] = '+',
    [0x0E] = '\b', [0x0F] = '\t',
    
    [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E',
    [0x13] = 'R', [0x14] = 'T', [0x15] = 'Y',
    [0x16] = 'U', [0x17] = 'I', [0x18] = 'O',
    [0x19] = 'P',
    
    [0x1A] = '{', [0x1B] = '}',
    [0x1C] = '\n',
    
    [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D',
    [0x21] = 'F', [0x22] = 'G', [0x23] = 'H',
    [0x24] = 'J', [0x25] = 'K', [0x26] = 'L',
    
    [0x27] = ':', [0x28] = '"',
    [0x29] = '~', [0x2B] = '|',
    
    [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C',
    [0x2F] = 'V', [0x30] = 'B', [0x31] = 'N',
    [0x32] = 'M',
    
    [0x33] = '<', [0x34] = '>', [0x35] = '?',
    
    [0x39] = ' ',
};

int keyboard_getchar(void) {
    static int  shift_pressed   = 0;
    static int  caps_lock       = 0;
    static int  skip_extended   = 0;
    static int  alt_pressed     = 0;

    while (1) {
        // Check whether keyboard data is available
        if (!(inb(KEYBOARD_STATUS_PORT) & 1))
            continue;

        uint8_t scancode = inb(KEYBOARD_DATA_PORT);

        // Extended scan codes prefix
        if (scancode == 0xE0 || scancode == 0xE1) {
            skip_extended = 1;
            continue;
        }

        if (skip_extended) {
            skip_extended = 0;
            continue;
        }

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 1;
            continue;
        }
        
        if (scancode == 0xAA || scancode == 0xB6) {
            shift_pressed = 0;
            continue;
        }

        if (scancode == 0x38) {
            alt_pressed = 1;
            continue;
        }
        
        if (scancode == 0xB8) {
            alt_pressed = 0;
            continue;
        }

        if (scancode == 0x3A) {
            caps_lock = !caps_lock;
            continue;
        }

        // Ignore key releases
        if (scancode & 0x80)
            continue;

        // Screen switching shortcuts
        if (alt_pressed) {
            if (scancode == 0x02)
                return KEY_SCREEN_1;
                
            if (scancode == 0x03)
                return KEY_SCREEN_2;
            
            if (scancode == 0x04)
                return KEY_SCREEN_3;
        }

        // Ignore scan codes outside our table
        if (scancode >= 128)
            continue;

        // Translate scan code into ASCII
        char    character;
        
        if (shift_pressed)
            character = shift_table[scancode];
        else
            character = normal_table[scancode];

        if (caps_lock && character >= 'a' && character <= 'z')
            character -= 32;
        else if (caps_lock && shift_pressed &&
                 character >= 'A' && character <= 'Z')
            character += 32;

        if (character != 0)
            return character;
    }
}
