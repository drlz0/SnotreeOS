#ifndef SERIAL_SERIAL_H
#define SERIAL_SERIAL_H

#include <stdio.h>
#include "../arch/i386/io.h"

#define PORT                            0x3F8

int init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);
void write_serial_string(const char *str);
void serial_putchar(char c);
void serial_puts(const char *str);

#endif