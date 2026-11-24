#include "screen.h"
#include "keyboard.h"
#include "printf.h"

static void initialize_screens(void) {
    screen_clear();

    // Screen 2: System info
    screen_switch(SCREEN_SYSTEM);
    screen_clear();
    
    // Screen 3: Debug info
    screen_switch(SCREEN_DEBUG);
    screen_clear();

    //Return to the original terminal
    screen_switch(SCREEN_TERMINAL);
}

void kernel_main(void) {

    initialize_screens();

    kprintf("42\n");

    while (1) {
        int character = keyboard_getchar();

        // Handle virtlual screen shortcuts
        if (character == KEY_SCREEN_1) {
            screen_switch(SCREEN_TERMINAL);
            continue;
        }
        
        if (character == KEY_SCREEN_2) {
            screen_switch(SCREEN_SYSTEM);
            continue;
        }
        
        if (character == KEY_SCREEN_3) {
            screen_switch(SCREEN_DEBUG);
            continue;
        }

        // Handle Backspace
        if (character == '\b')
            screen_write_char('\b');
        else if (character == '\n')
            screen_write_char('\n');
        else if (character > 0 && character <= 255)
            screen_write_char((char)character);
    }
}
