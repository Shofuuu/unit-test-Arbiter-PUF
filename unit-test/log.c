/*
* Document : log.c
* Date     : 17/10/2022 - 23:45:48
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* Logging testing hardware activity
* on average CRP test
*/

#include "log.h"

int wlog (struct parameters *p, unsigned char mode) {
    FILE *file;
    int valid = 0;

    if (mode == BODY_DOCS) {
        p->fmode = (char*)"a";
        file = fopen(p->fname, p->fmode);
        
        if (file == NULL) valid = -1;
        
        for (int x=0;x<8;x++) {
            fprintf(file, "%c%X ", (p->challenge[x] <= 15 ? '0':'\0'), p->challenge[x]);
        }

        fprintf(file, ",");

        for (int x=0;x<8;x++) {
            fprintf(file, "%c%X ", (p->response[x] <= 15 ? '0':'\0'), p->response[x]);
            dbgmsg("wlog", "Response : %X\r\n", (unsigned int)p->response[x]);
        }

        fprintf(file, "\n");
    } else {
        p->fmode = (char*)"w";
        file = fopen(p->fname, p->fmode);

        if (file == NULL) valid = -1;

        size_t size = 22; // length of the buffer about date and clock
        char *gtime = malloc(sizeof(char) * size);

        time_t ntime;
        struct tm *ntm;

        if ((ntime = time(NULL)) == -1) valid = -2;
        if ((ntm = localtime(&ntime)) == NULL) valid = -3;

        strftime(gtime, size, "%d/%m/%Y - %H:%M:%S", ntm);

        fprintf(file, "FPGA Arbiter PUF CRP,\n");
        fprintf(file, "Created at : %s,\n", gtime);
        fprintf(file, "Challenge,Response\n");

        free(gtime);
    }

    fclose(file);

    return valid;
}



