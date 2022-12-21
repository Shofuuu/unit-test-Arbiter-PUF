/*
* Document : parameters.h
* Date     : 18/10/2022 - 01:08:39
* Author   : Muhammad Shofuwan Anwar
*/

#ifndef PARAMETERS_H
#define PARAMETERS_H

//#define GLOBAL_DEBUG_ENABLED    1
#define MAX_BUFFER_BYTE         4
#define DATAWIDTH_BIT           128
#define DATAWIDTH_BYTE          16

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/time.h>

struct parameters{
    char *fname;
    char *fmode;

    char *crpfname;
    char *format;

    char *port;
    int baudrate;
    int serial;

    int obytes;
    
    uint8_t k; // average
    uint32_t n;  // logging
    
    uint8_t resuming;
    uint32_t unfinished;
};

union data_bits_u {
    uint8_t byte[DATAWIDTH_BYTE];
    uint64_t data[DATAWIDTH_BYTE/8];
};

/* Uart Parameters Set Attribute */
void uparsetattr (struct parameters *p);

/* string config */
int cpy (char *dst, const char *src, size_t size);
size_t len (const char *str);

/* 64-bit randomizer challenge */
void randomizer (union data_bits_u *c); // Random Challenge Bits 

/* file utility */
#define BUFFER_TEXT_FLINES  210

uint32_t flines (const char *flname);

/* string formating */
void strcrpbin (union data_bits_u *d, char *str);

#endif // PARAMETERS_H



