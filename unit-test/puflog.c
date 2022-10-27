/*
* Document : puflog.c
* Date     : 17/10/2022 - 23:53:15
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* Main "*.c" file for puf unit-test
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parameters.h"
#include "uart.h"
#include "log.h"

#define MAX_HELPMSG 12

void logfwr (struct parameters *p);

int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("[Err:argc] Please specify at least one argument!\r\n");
        exit(-1);
    }

    const char *helpmsg[MAX_HELPMSG] = {
        "Help information :\r\n",
        " puflog [h, o, n, k, p, b]\r\n",
        "\nExample :\r\n",
        " puflog -o [output]\r\n",
        " puflog -o [output] -n [iteration log]\r\n",
        " puflog -o [output] -n [iteration log] -k [iteration avg]\r\n",
        " puflog -o [output] -k [iteration avg]\r\n",
        "\nInformation :\r\n",
        " \"k\" has default value 8\r\n",
        " \"n\" has default value 100\r\n",
        " \"p\" has default value /dev/ttyUSB0\r\n",
        " \"b\" has default value 9600\r\n"
    };

    char opt = 0;
    int valid = 0;

    struct parameters p;
    uparsetattr(&p);

    while( (opt = getopt(argc, argv, "ho:n:k:p:b:")) != -1 ){
        switch(opt){
            /* Help message */
            case 'h':
                for(int x=0;x<MAX_HELPMSG;x++) printf("%s", helpmsg[x]);
                valid = 0;
            break;

            /* Set the output file destination */
            case 'o':
                p.fname = optarg;
                valid++;
            break;

            /* Set "k" value for total averaging process iteration CRP */
            case 'k':
                p.k = atoi(optarg);
                valid++;
            break;

            /* Set "n" value for iteration logging process */
            case 'n':
                p.n = atoi(optarg);
                valid++;
            break;

            case 'p':
                p.port = optarg;
                valid++;
            break;

            case 'b':
                p.baudrate = atoi(optarg);
                valid++;
            break;

            case '?':
                if('o' == optopt) printf("[Err:getopt] File name is not valid!\r\n");
                else if('n' == optopt) printf("[Err:getopt] Implicit iteration for logging is needed.\r\n");
                else if('k' == optopt) printf("[Err:getopt] Default \"k\" option is 8, implicit value needed.\r\n");
                else if('p' == optopt) printf("[Err:getopt] Port location must be specified!\r\n");
                else if('b' == optopt) printf("[Err:getopt] Baudrate speed can't be empty!\r\n");
                else printf("[Err:getopt] Invalid option caught!\r\n");

                return -1;
            break;
        }
    }

    if(valid > 0){
        if(p.fname == NULL){
            printf("[Err:fname] File name can\'t be NULL\r\n");
            return -1;
        }else{
            logfwr(&p);
        }
    }

    return 0;
}

void logfwr (struct parameters *p) {
    printf("[Info:logfwr] Port location set to : %s\r\n", p->port);
    printf("[Info:logfwr] Logging to : %s\r\n", p->fname);
    printf("[Info:logfwr] k val : %d, n val : %d\r\n", p->k, p->n);

    uint8_t *challenge = malloc(8 * sizeof(uint8_t));
    uint64_t response = 0;
    uint64_t rchg = 0, pre_rchg = 0;
    uint64_t *avg_crp = malloc((p->k) * sizeof(uint64_t));
    uint8_t tmp_rsp[8];
    char *str_print = malloc(80 * sizeof(char));

    int ret = 0;
    float percent = 0;

    wlog(p, HEAD_DOCS);

    for (uint32_t x=0;x<(p->n);x++) {
        percent = ((float)x / ((float)(p->n)-1)) * 100.0;
        rdchgb(&rchg);

        while (1) {
            if (rchg == pre_rchg) {
                rdchgb(&rchg);
            } else {
                pre_rchg = rchg;
                break;
            }
        }

        dbgmsg("logfwr", "generated : %lX\r\n", rchg);

        chbstr(challenge, rchg);
        p->challenge = challenge;

        for (uint8_t y=0;y<(p->k);y++) {
            if (uart_begin(p) < 0) {
                printf("[Err:logfwr] Failed to initialize uart!\r\n");
                return;
            }

            if ((ret = uart_iostream(p)) < 0) {
                printf("[Err:logfwr] I/O error while trying communicate in uart!, (%d)\r\n", ret);
                return;
            }

            for (int i=0;i<8;i++) {
                tmp_rsp[i] = p->response[i];
                avg_crp[i] += tmp_rsp[i];
            }
        }

        for (int a=0;a<8;a++) {
            tmp_rsp[a] = (avg_crp[a]/(p->k));
        }

        rspstr(&response, tmp_rsp);
        wlog(p, BODY_DOCS);

        sprintf(str_print, "[Info:generating] CHG:RSP (%lX > %lX) : [%0.1f] ", rchg, response, percent);
        printf("%s", str_print);
        
        for (int b=0;b<len((const char*)str_print);b++) {
            printf("\b");
        }
        fflush(stdout);
    }

    printf("\r\n");
}



