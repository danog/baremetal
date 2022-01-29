#include "mini_uart.h"
#include "reg.h"

#define UART_DR (UART_BASE+0x0)
#define UART_FR (UART_BASE+0x18)

#define UART_IBRD   (UART_BASE+0x24)
#define UART_FBRD   (UART_BASE+0x28)
#define UART_LCRH   (UART_BASE+0x2C)
#define UART_CR     (UART_BASE+0x30)
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
    selector |= 4<<12;                      // set alt1 for gpio14
    selector &= ~(7<<15);                   // clean gpio15
    selector |= 4<<15;                      // set alt1 for gpio 15
    put32(GPFSEL1,selector);

	put32(UART_CR, 0);

    put32(GPPUD,0);
    delay(150);
    put32(GPPUDCLK0,(1<<14)|(1<<15));
    delay(150);
    put32(GPPUDCLK0,0);

    /*put32(AUX_ENABLES,1);                   //Enable mini uart (this also enables access to its registers)
    put32(AUX_MU_CNTL_REG,0);               //Disable auto flow control and disable receiver and transmitter (for now)
    put32(AUX_MU_IER_REG,0);                //Disable receive and transmit interrupts
    put32(AUX_MU_LCR_REG,3);                //Enable 8 bit mode
    put32(AUX_MU_MCR_REG,0);                //Set RTS line to be always high
    put32(AUX_MU_BAUD_REG,270);             //Set baud rate to 115200

    put32(AUX_MU_CNTL_REG,3);               //Finally, enable transmitter and receiver*/
    
	put32(CM_UARTDIV, CM_PASSWORD | 0x6666);
	put32(CM_UARTDIV, CM_PASSWORD | CM_SRC_OSC | CM_UARTCTL_FRAC_SET | CM_UARTCTL_ENAB_SET);

	put32(UART_ICR, 0x7FF);
	put32(UART_IBRD, 1);
	put32(UART_FBRD, 40);
	put32(UART_LCRH, 0x70);
	put32(UART_CR, 0x301);
}

void uart_send(char c)
{
     //while(!(get32(AUX_MU_LSR_REG) & 0x20));
     //put32(AUX_MU_IO_REG, c);
     
    while(get32(UART_FR) & 0x20);
    put32(UART_DR, c);
}
char uart_recv()
{
    //while (!(get32(AUX_MU_LSR_REG) & 0x1));
    //return get32(AUX_MU_IO_REG) & 0xFF;

    while(get32(UART_FR) & 0x10);
    return put32(UART_DR) & 0xFF;
}

void uart_send_string(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        uart_send(str[i]);
    }
}