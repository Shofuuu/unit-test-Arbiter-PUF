/*
* Document : uart.h
* Date     : 17/10/2022 - 23:56:01
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* UART communication controller
*/

#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <fcntl.h> // File controls (O_RDWR)
#include <termios.h> // POSIX standard terminal control
#include <unistd.h> // write(), read(), close()

#include "parameters.h"

int uart_begin (struct parameters *p);
int uart_iostream (struct parameters *p);

#endif // UART_H



