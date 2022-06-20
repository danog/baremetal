#pragma once

void uart0_init(void);
void uart0_send(char c);
char uart0_recv();
void uart0_send_string(const char *str);

void uart1_init(void);
void uart1_send(char c);
char uart1_recv();
void uart1_send_string(const char *str);