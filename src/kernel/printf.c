#include "printf.h"
#include "screen.h"
#include <stdarg.h>

static void print_unsigned(u32 number, u32 base)
{
    char digits[] = "0123456789abcdef";
    char buffer[32];
    int i = 0;

    if (number == 0)
    {
        screen_write("0");
        return;
    }

    while (number > 0)
    {
        buffer[i++] = digits[number % base];
        number /= base;
    }

    while (i > 0)
        screen_write_char(buffer[--i]);
}

void kprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format != '%')
        {
            screen_write_char(*format);
            format++;
            continue;
        }

        format++;

        switch (*format)
        {
            case 's':
            {
                const char *str = va_arg(args, const char *);
                screen_write(str ? str : "(null)");
                break;
            }

            case 'c':
            {
                char character = (char)va_arg(args, int);
                screen_write_char(character);
                break;
            }

            case 'd':
            {
                i32 number = va_arg(args, i32);

                if (number < 0)
                {
                    screen_write("-");
                    print_unsigned(0u - (u32)number, 10);
                }
                else
                    print_unsigned((u32)number, 10);

                break;
            }

            case 'u':
            {
                u32 number = va_arg(args, u32);
                print_unsigned(number, 10);
                break;
            }

            case 'x':
            {
                u32 number = va_arg(args, u32);
                print_unsigned(number, 16);
                break;
            }

            case '%':
                screen_write("%");
                break;

            default:
                screen_write("%");
                if (*format)
                {
                    char character[2] = {*format, '\0'};
                    screen_write(character);
                }
                break;
        }

        if (*format)
            format++;
    }

    va_end(args);
}
