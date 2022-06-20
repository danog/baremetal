#include "mini_uart.h"
#include "reg.h"
#include "utils.h"

#define ARM_IDVAL        0x364D5241

void uart_hex(int i) {
    uart0_send_string("0x");
    for (int x = 0; x < 8; x++) {
        int c = 48 + ((i & 0xF0000000) >> 28); 
        if (c == 48+10) {
            uart0_send('A');
        } else if (c == 48+11) {
            uart0_send('B');
        } else if (c == 48+12) {
            uart0_send('C');
        } else if (c == 48+13) {
            uart0_send('D');
        } else if (c == 48+14) {
            uart0_send('E');
        } else if (c == 48+15) {
            uart0_send('F');
        } else {
            uart0_send(c);
        }
        i <<= 4;
    }
    uart0_send_string("\r\n");
}
void kernel_main(void)
{
	/*while(get32(ARM_ID) != ARM_IDVAL);
	delay(500);

    uart0_init();
	delay(1000);*/
    uart0_send_string("Hello, world!\r\n");

    /*while (1) {
        uart0_send(uart0_recv());
    }*/
}
