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

    p->port = malloc(13 * sizeof(char));
    p->port = "/dev/ttyUSB0";
    p->baudrate = 9600;
    p->serial = 0;

    p->challenge = malloc(8 * sizeof(char));
    p->challenge = malloc(8 * sizeof(char));

    p->challenge = NULL;
    p->response = NULL;
    p->obytes = 0;

    p->k = 8;
    p->n = 100;
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

/* Bit manipulator */
void chbstr (uint8_t *dst, uint64_t src) { // Challenge byte64 to single string
    uint8_t *tmp = malloc(8 * sizeof(dst));

    for (int x=0;x<8;x++) {
        tmp[7-x] = (src & ((uint64_t)0x00000000000000ff << (x*8))) >> (x*8);
        dst[7-x] = tmp[7-x];
    }

    free(tmp);
}

void rspstr (uint64_t *dst, uint8_t *src) { // Response single string to byte64
    *dst = 0;

    for (int x=0;x<8;x++) {
        *dst |= (uint64_t)( (uint64_t)src[7-x] << (x*8));
        dbgmsg("rspstr", "tmp process : %lX\r\n", dst);
    }
}

/* Debugging purpose */
int __glob_dbg_cnt = 0;

void dbgmsg (const char *label, const char *msg, ...) {
    va_list argument;
    char buff[256];

    va_start(argument, msg);
    vsprintf(buff, msg, argument);
    va_end(argument);

#ifdef GLOBAL_DEBUG_ENABLED
    printf("[Debug:%s:%d] ", label, __glob_dbg_cnt);
    printf("%s", buff);
    
    __glob_dbg_cnt++;
#endif // GLOBAL_DEBUG_ENABLED
}

/* 64-bit randomizer challenge */
void rdchgb (uint64_t *challenge) { // Random Challenge Bits
    uint64_t tmp = 0;
    uint8_t rnum = 0;
    time_t t;

    srand(time(&t));
    
    for (int x=0;x<8;x++) {
        rnum = (rand()%255); // just random things

        if (rnum == 0xFF) {
            rnum = ((rand()%1024) - (rand()%321))%255; // just random things
        }
    
        tmp |= ((uint64_t)rnum << (x*8));
        dbgmsg("rdchgb", "rnum : %X, tmp : %lX\r\n", (unsigned int)rnum, tmp);
    }

    *challenge = tmp;
}



