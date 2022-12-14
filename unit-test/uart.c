/*
* Document : uart.c
* Date     : 17/10/2022 - 23:55:50
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* UART communication controller
*/

#include "uart.h"

int uart_begin (struct parameters *p) {
    int serial_port = open((const char*)p->port, O_RDWR);

    if (serial_port < 0) {
        return -1;
    }

    p->serial = serial_port;

    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0) {
        return -2;
    }

    /* configure the uart protocol hardware setup */
    tty.c_cflag &= ~CSTOPB; // No stop bits
    tty.c_cflag &= ~PARENB; // No parity bits 
    tty.c_cflag &= ~CSIZE; // Clear character size
    tty.c_cflag |= CS8; // Character size 8 bit
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable READ and ignore ctrl lines

    tty.c_lflag &= ~ICANON; // Disable canonical mode (Inputs doesn't require new line for transmit)
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT, and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn-off software flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes (e.g raw data)

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriege return/line feed

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received
    tty.c_cc[VMIN] = 0;

    cfsetispeed(&tty, p->baudrate);
    cfsetospeed(&tty, p->baudrate);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        return -3;
    }

    return 0;
}

int uart_iostream (struct parameters *p, union data_bits_u *c, union data_bits_u *r) {
    if (p->serial < 0) {
        return -1;
    }

    int tmp_bytes = 0;

    /* Writing half data (64 bits) */
    write(p->serial, &c->data[0], sizeof(c->byte));
    tmp_bytes = read(p->serial, &r->data[0], sizeof(r->byte));

    if (tmp_bytes < 0) {
        return -2;
    }

    /* Writing the last data (64 bits) */
    write(p->serial, &c->data[1], sizeof(c->byte));
    tmp_bytes = read(p->serial, &r->data[1], sizeof(r->byte));

    if (tmp_bytes < 0) {
        return -3;
    }

    close(p->serial);
    return 0;
}



