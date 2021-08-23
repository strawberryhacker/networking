// Author: strawberryhacker

#include "print.h"
#include "registers.h"
#include "clock.h"
#include "gpio.h"

//--------------------------------------------------------------------------------------------------

static void print_char(char c) {
    while ((UART0->SR & (1 << 9)) == 0);
    UART0->THR = c;
}

//--------------------------------------------------------------------------------------------------

void print_init() {
    enable_peripheral_clock(7);

    set_pin_function(GPIOA,  9, PIN_FUNCTION_A);
    set_pin_function(GPIOA, 10, PIN_FUNCTION_A);

    UART0->MR = 4 << 9;
    UART0->BRGR = (u16)(120000000 / 16 / 921600);
    UART0->CR = 1 << 4 | 1 << 6;
}

//--------------------------------------------------------------------------------------------------

void print(const char* text, ...) {
    static char print_buffer[1024];

    va_list arguments;
    va_start(arguments, text);
    int size = format_string(text, print_buffer, 1024, arguments);

    for (int i = 0; i < size; i++) {
        // Some terminals like Linux's screen doen not work properly without the CR-LF sequence.
        if (print_buffer[i] == '\n') {
            print_char('\r');
        }

        print_char(print_buffer[i]);
    }
}
