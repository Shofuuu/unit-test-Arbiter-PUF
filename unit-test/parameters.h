/*
* Document : parameters.h
* Date     : 18/10/2022 - 01:08:39
* Author   : Muhammad Shofuwan Anwar
*/

#ifndef PARAMETERS_H
#define PARAMETERS_H

//#define GLOBAL_DEBUG_ENABLED    1
#define MAX_BUFFER_BYTE         4

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/time.h>

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
    
    uint8_t resuming;
    uint32_t unfinished;
};

/* Uart Parameters Set Attribute */
void uparsetattr (struct parameters *p);

/* string config */
int cpy (char *dst, const char *src, size_t size);
size_t len (const char *str);

/* Debug purpose */
extern int __glob_dbg_cnt;

/* Bit manipulator */
void uint64str (uint8_t *dst, uint64_t src); // 64-bit to single string
void struint64 (uint64_t *dst, uint8_t *src); // single string to 64-bit

/* Debug purpose */
void dbgmsg (const char *label, const char *msg, ...);

/* 64-bit randomizer challenge */
void rdchgb (uint64_t *challenge); // Random Challenge Bits 

#endif // PARAMETERS_H



