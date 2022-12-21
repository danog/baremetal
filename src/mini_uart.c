#include "mini_uart.h"
#include "reg.h"

#define UART_DR (UART_BASE+0x0)
#define UART_FR (UART_BASE+0x18)

#define UART_IBRD   (UART_BASE+0x24)
#define UART_FBRD   (UART_BASE+0x28)
#define UART_LCRH   (UART_BASE+0x2C)
#define UART_CR     (UART_BASE+0x30)
#define UART_IMSC     (UART_BASE+0x38)
#define UART_ICR    (UART_BASE+0x44)

// UART clock baud rate divisor register
// Why is this undocumented??
#define CM_UARTDIV (0x7e1010f4-BUS_OFFSET)

// UART control register
// Why is this undocumented??
#define CM_UARTCTL 0x7e1010f0


// cucumber moment
#define CM_PASSWORD 0x5a000000
#define CM_SRC_OSC			1

#define CM_UARTCTL_FRAC_SET                                0x00000200
#define CM_UARTCTL_ENAB_SET                                0x00000010

void uart_init(void)
{
    unsigned int selector;
    
    selector = get32(GPFSEL1);
    selector &= ~(7<<12);                   // clean gpio14
    selector |= 4<<12;                      // set alt0 for gpio14
    selector &= ~(7<<15);                   // clean gpio15
    selector |= 4<<15;                      // set alt0 for gpio 15
    put32(GPFSEL1,selector);

    put32(GPPUD,0);
    delay(150);
    put32(GPPUDCLK0,(1<<14)|(1<<15));
    delay(150);
    put32(GPPUDCLK0,0);

	put32(UART_CR, 0);
	put32(UART_IMSC, 0);

    //put32(CM_UARTDIV, CM_PASSWORD | 0x6666);
	//put32(CM_UARTDIV, CM_PASSWORD | CM_SRC_OSC | CM_UARTCTL_FRAC_SET | CM_UARTCTL_ENAB_SET);

	put32(UART_IBRD, 26);
	put32(UART_FBRD, 3);
    put32(UART_LCRH, (3 << 5) | (1 << 4));
	put32(UART_CR, 0x301);
}

void uart_send(char c)
{
    while (get32(UART_FR) & (1<<5));
    put32(UART_DR, c);
}
char uart_recv()
{
    while (get32(UART_FR) & (1<<4));
    return put32(UART_DR) & 0xFF;
}

void uart_send_string(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        uart_send(str[i]);
    }
}