#pragma once

void uart_init(void);
void uart_send(char c);
char uart_recv();
void uart_send_string(const char *str);