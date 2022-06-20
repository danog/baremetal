#include "mini_uart.h"
#include "reg.h"
#include "utils.h"

void kernel_main(void)
{
    uart0_init();
    uart0_send_string("Hello, world!\r\n");

    while (1) {
        uart0_send(uart0_recv());
    }
}
