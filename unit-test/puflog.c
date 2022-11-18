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

#define MAX_HELPMSG 16

void logfwr (struct parameters *p);

int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("[Err:argc] Please specify at least one argument!\r\n");
        exit(-1);
    }

    const char *helpmsg[MAX_HELPMSG] = {
        "Help information :\r\n",
        " puflog [h, o, n, k, p, b, i, f]\r\n",
        "\nExample :\r\n",
        " puflog -o [output]\r\n",
        " puflog -o [output] -n [iteration log]\r\n",
        " puflog -o [output] -n [iteration log] -k [iteration avg]\r\n",
        " puflog -o [output] -k [iteration avg]\r\n",
        " puflog -o [output] -c\r\n",
        " puflog -o [output] -i [generated CRP file] -f [format number]\r\n",
        "\nInformation :\r\n",
        " \"k\" has default value 8\r\n",
        " \"n\" has default value 100\r\n",
        " \"p\" has default value /dev/ttyUSB0\r\n",
        " \"b\" has default value 9600\r\n",
        " \"c\" used to continue last work\r\n",
        " \"f\" available formats are \"bin\" (binary) and \"dec\" (decimal)\r\n"
    };

    char opt = 0;
    int valid = 0;

    struct parameters p;
    uparsetattr(&p);

    while( (opt = getopt(argc, argv, "hco:n:k:p:b:f:i:")) != -1 ){
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

            case 'c':
                p.resuming = 1;
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
    
    if (p->resuming) {
        printf("[Info:logfwr] Resuming previous work..\r\n");

        if (acsbl(p) != 0) {
            printf("[Err:logfwr] There's no saved jobs!\r\n");
            return;
        }

        ctylog(p);

        if ((((float)((p->unfinished)) / (float)((p->n)-1)) * 100) > 100) {
            printf("[Info:logfwr] No jobs left.\r\n");
            return;
        }

        printf("[Info:logfwr] Resuming k at %d\r\n", p->k);
        printf("[Info:logfwr] Resuming n at %d\r\n", p->n);
        printf("[Info:logfwr] Last work progress %0.1f%%\r\n", (((float)((p->unfinished)) / (float)((p->n)-1)) * 100));
    } else {
        wlog(p, HEAD_DOCS);
    }
    
    uint8_t *challenge = malloc(8 * sizeof(uint8_t));
    uint64_t response = 0;
    uint64_t rchg = 0, pre_rchg = 0;
    uint64_t *avg_crp = malloc((p->k) * sizeof(uint64_t));
    uint8_t tmp_rsp[8];
    char *str_print = malloc(80 * sizeof(char)); // string buffer to display string

    int ret = 0;
    float percent = 0;
    uint8_t first_itr = 1; 

    for (uint32_t avgcnt = 0;avgcnt<(p->n);avgcnt++) {
        if (p->resuming && first_itr) {
            avgcnt = p->unfinished;

            first_itr = 0;
        }

        percent = ((float)avgcnt / ((float)(p->n)-1)) * 100.0;
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

        uint64str(challenge, rchg);
        p->challenge = challenge;

        uint8_t crp_idx = 0;

        for (uint8_t itrcnt = 0;itrcnt<(p->k);itrcnt++) {
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
            }

            struint64(&response, tmp_rsp);
            avg_crp[crp_idx] = response;

            printf("RESPONSE [%d] : ", crp_idx);
            for (int x=0;x<64;x++) {
                printf("%d", ((response & (1 << (63-x))) > 0 ? 1:0));
            }
            printf(" : %lX \r\n", response);

            crp_idx++;
        }

        uint64_t rspdata = 0;
        uint64_t rspavg = 0;
        uint8_t tmpbits = 0;

        for (int y=0;y<64;y++){  // 64-bits Response
            for (int x=0;x<crp_idx;x++) {
                rspdata = avg_crp[x];

                for (int z=0;z<crp_idx;z++) {
                    tmpbits += ((rspdata & (1 << y)) > 0 ? 1:0);
                }
                
                tmpbits /= crp_idx;
                if (tmpbits) rspavg |= (1 << y);
                tmpbits = 0;
            }
        }
        
        printf("EXPECTED : %lX\r\n", rspavg);
        uint64str(tmp_rsp, rspavg);
        p->response = tmp_rsp;

        printf("OUTPUT : ");
        for (int x=0;x<8;x++) {
            printf("%lX", p->response[x]);
        }
        printf("\r\n");

        wlog(p, BODY_DOCS);

        /*sprintf(str_print, "[Info:generating] CHG:RSP (%lX > %lX) : [%0.1f%%]  ", rchg, avg_crp[0], percent); // extra space needed to clear screen buffer
        printf("%s", str_print);
        
        for (int b=0;b<len((const char*)str_print);b++) {
            printf("\b");
        }
        fflush(stdout);*/
    }

    printf("\r\n");
}



