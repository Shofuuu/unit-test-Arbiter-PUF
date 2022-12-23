/*
* Document : parameters.c
* Date     : 18/10/2022 - 01:08:10
* Author   : Muhammad Shofuwan Anwar
*/

#include "parameters.h"

/* Uart Parameter Set Attribute */
void uparsetattr (struct parameters *p) {
    p->fname = NULL;
    p->fmode = "a";

    p->crpfname = NULL;
    p->format = NULL;

    // p->port = malloc(13 * sizeof(char));
    p->port = "/dev/ttyUSB0";
    p->baudrate = 9600;
    p->serial = 0;

    p->obytes = 0;

    p->k = 8;
    p->n = 100;

    p->resuming = 0;
    p->unfinished = 0;
}

/* String purpose */
int cpy (char *dst, const char *src, size_t size) {
    int valid = 0;

    if (size < 1) valid = -1;

    for (int x=0;x<size;x++) {
        *dst++ = *src++;
    }

    return valid;
}

size_t len (const char *str) {
    int count = 0;

    while (*str++) {
        count++;
    }

    return count;
}

/* 128-bit randomizer challenge */
void randomizer (union data_bits_u *c) { // Random Challenge Bits
    struct timeval t;
    uint16_t rnum = 0, a = 0, b = 0;
    uint16_t mirror = 0;
    uint8_t zeros = 0;

    gettimeofday(&t, NULL);
    srand(t.tv_usec);

    for (uint8_t x=0;x<16;x++) {
        if (x % 3) {
            gettimeofday(&t, NULL);
            srand(t.tv_usec);
        }

        rnum = rand()%65535;
        b = rnum;
        a = rnum - (rnum/5);

        for (uint8_t y=0;y<16;y++) {
            mirror |= ((b-a) & (1 << y));
        }

        for (uint8_t y=0;y<4;y++) {
            if ( ( mirror & (1 << (15-y)) ) ) zeros = 1;
        }

        if (zeros) {
            mirror = (uint8_t)(a * (uint8_t)(rnum/3.0));

            if (mirror < 0x0F) {
                mirror = ((uint8_t)(b - (uint8_t)(rnum/2.0)) + x);
            }

            zeros = 0;
        }

        c->byte[x] = (uint8_t)mirror;

        a += 2;
        b -= 3;
        mirror = 0;
    }
}

/* File utility */
uint32_t flines (const char *fname) {
    FILE *file = fopen(fname, "a+");
    char str_tmp[BUFFER_TEXT_FLINES];
    uint32_t lines = 0;

    // memset(str_tmp, '\0', BUFFER_TEXT_FLINES);

    while (fgets(str_tmp, BUFFER_TEXT_FLINES, file) != NULL) {
        lines++;
    }

    fclose(file);

    return lines;
}

/* String formatting */
void strcrpbin (union data_bits_u *d, char *str) {
    for (uint8_t x=0;x<16;x++) {
        d->byte[x] = 0;

        for (uint8_t y=0;y<8;y++) {
            if (str[y+(8*x)] == '1') {
                d->byte[x] |= (1 << (7-y));
            }
        }
    }

    /*for (uint8_t x=0;x<16;x++) {
        printf("%c%X", (d->byte[x] < 0xF ? '0':'\0'), d->byte[x]);
    }

    printf("\r\n");*/
}



