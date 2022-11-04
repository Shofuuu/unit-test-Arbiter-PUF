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
        fprintf(file, "Created at : ,%s\n", gtime);
        fprintf(file, "k value : %d, n value : %d\n", p->k, p->n);
        fprintf(file, "Challenge,Response\n");

        free(gtime);
    }

    fclose(file);

    return valid;
}

int ctylog (struct parameters* p) {
    FILE *file = fopen(p->fname, "a+");
    int valid = 0;
    char tmp_str[78];

    if (file == NULL) {
        valid = -1;
    }

    uint32_t lines = 0;
    char *newstr = malloc(30 * sizeof(char));

    while (fgets(tmp_str, 78, file) != NULL) {
        if (lines == 2) {
            cpy(newstr, (const char*)tmp_str, len((const char*)tmp_str));
        }

        lines++;
    }

    uint32_t *tmp_avg_itr = malloc(2 * sizeof(uint32_t));

    parstr((const char*)newstr, tmp_avg_itr);

    p->k = tmp_avg_itr[0];
    p->n = tmp_avg_itr[1];
    p->unfinished = (lines - 4); // 4 is first label and title of the document

    fclose(file);
    free(newstr);
    free(tmp_avg_itr);

    return valid;
}

void parstr (const char *str, uint32_t *value) {
    /* count total space character */
    uint8_t space = 0;

    for (uint8_t x = 0;x<len(str);x++) {
        if (32 == str[x]) {
            space++;
        }
    }

    char *tmp = malloc(sizeof(char) * (len(str) - space));
    uint8_t index = 0;

    /* remove all of the space character */
    for (uint8_t x=0;x<len(str);x++) {
        if (str[x] != 32) {
            tmp[index] = str[x];
            index++;
        }
    }

    char tmp_kval[10] = {0};
    char tmp_nval[10] = {0};

    uint8_t en_cp = 0;// enable copy
    uint8_t start_index = 0;

    index = 0;

    /* find the first number (k value) */
    for (uint8_t x=0;x<len((const char*)tmp);x++) {
        if (tmp[x] == 58 || tmp[x] == 44) {
            if (tmp[x] == 58){
                en_cp = 1;
                continue;
            }
            else {
                start_index = x;
                break;
            }
        }

        if (en_cp) {
            tmp_kval[index] = tmp[x];
            index++;
        }
    }

    index = 0;
    en_cp = 0;
    value[0] = atoi(tmp_kval);

    for (uint8_t x=start_index;x<len((const char*)tmp);x++) {
        if (tmp[x] == 58) {
            en_cp = 1;
            continue;
        }

        if (en_cp) {
            tmp_nval[index] = tmp[x];
            index++;
        }
    }

    value[1] = atol(tmp_nval);

    free(tmp);
}

int acsbl (struct parameters *p) {
    FILE *file = fopen(p->fname, "r");

    if (file == NULL) {
        return -1;
    } else {
        return 0;
    }
}



