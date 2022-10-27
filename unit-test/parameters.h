/*
* Document : parameters.h
* Date     : 18/10/2022 - 01:08:39
* Author   : Muhammad Shofuwan Anwar
*/

#ifndef PARAMETERS_H
#define PARAMETERS_H

//#define GLOBAL_DEBUG_ENABLED    1
#define MAX_BUFFER_BYTE         4

#define MOVEXY(x,y)             printf("\033[%d;%dH", x, y)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>

struct parameters{
    char *fname;
    char *fmode;

    char *port;
    int baudrate;
    int serial;

    uint8_t *challenge;
    uint8_t *response;
    int obytes;
    
    uint8_t k; // average
    uint32_t n;  // logging
};

/* Uart Parameters Set Attribute */
void uparsetattr (struct parameters *p);

/* string config */
int cpy (char *dst, const char *src, size_t size);
size_t len (const char *str);

/* Debug purpose */
extern int __glob_dbg_cnt;

/* Bit manipulator */
void chbstr (uint8_t *dst, uint64_t src); // Challenge byte64 to single string
void rspstr (uint64_t *dst, uint8_t *src); // Response single string to byte64

/* Debug purpose */
void dbgmsg (const char *label, const char *msg, ...);

/* 64-bit randomizer challenge */
void rdchgb (uint64_t *challenge); // Random Challenge Bits 

#endif // PARAMETERS_H



