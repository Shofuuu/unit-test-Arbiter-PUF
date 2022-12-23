/*
* Document : modeop.c
* Date     : 16/12/2022 - 00:25:24
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* Operation modes from user request
*/

#include "modeop.h"

void logfwr (struct parameters *p) {
    union data_bits_u challenge;
    union data_bits_u response;

    printf("[Info:logfwr] Port location set to : %s\r\n", p->port);
    printf("[Info:logfwr] Logging to : %s\r\n", p->fname);

    if (p->resuming) {
        printf("[Info:logfwr] Resuming previous work..\r\n");

        if (acsbl(p) != 0) {
            printf("[Err:logfwr] There's no saved jobs!\r\n");
            return;
        }

        ctylog(p);

        if ((( (float)((p->unfinished)) / (float)((p->n)+1) ) * 100) > 100) {
            printf("[Info:logfwr] No jobs left.\r\n");
            return;
        }

        printf("[Info:logfwr] k val : %d, n val : %d\r\n", p->k, p->n);
        printf("[Info:logfwr] Last work progress : %d [%0.1f%%]\r\n", (p->unfinished), (( (float)((p->unfinished)) / (float)((p->n)+1) ) * 100));
        printf("[Info:logfwr] Remaining job : %d\r\n", ((p->n)-(p->unfinished)));
    } else {
        wlog(p, &challenge, &response, HEAD_DOCS);
    }
    
    union data_bits_u *average = malloc(sizeof(average) * (p->n));
    union data_bits_u result;

    char *str_print = malloc(80 * sizeof(char)); // string buffer to display string
    int ret = 0;
    float percent = 0;
    uint8_t first_itr = 1;
    uint8_t byte_average = 0;

    for (uint32_t avgcnt = 0;avgcnt<(p->n);avgcnt++) {
        /* Check, if there is last session
         * and then continuing the progress by
         * changing the avgcnt (average counter)
         * to the last job line. first_itr
         * used to check whether the avgcnt
         * already set to unfinished line or not.
         * */
        if (p->resuming && first_itr) {
            avgcnt = p->unfinished;

            first_itr = 0;
        }

        percent = ((float)avgcnt / ((float)(p->n)-1)) * 100.0;
        randomizer(&challenge);

        for (uint8_t itrcnt = 0;itrcnt<(p->k);itrcnt++) {
            if (uart_begin(p) < 0) {
                printf("[Err:logfwr] Failed to initialize uart!\r\n");
                return;
            }

            if ((ret = uart_iostream(p, &challenge, &response)) < 0) {
                printf("[Err:logfwr] I/O error while trying communicate in uart!, (%d)\r\n", ret);
                return;
            }

           average[itrcnt].data[0] = response.data[0];
           average[itrcnt].data[1] = response.data[1];
        }

        /* averaging process */
        for (uint8_t x=0;x<16;x++) {
            for (uint8_t y=0;y<8;y++) {
                for (uint8_t z=0;z<(p->k);z++) {
                    byte_average += ((average[z].byte[x] & (1 << (7-y))) ? 1:0);
                }

                byte_average = (uint8_t)((((float)byte_average/(float)(p->k))*10.0) >= 5 ? 1:0);

                if (byte_average) {
                    result.byte[x] |= (1 << (7-y));
                }

                byte_average = 0;
            }
        }

        wlog(p, &challenge, &result, BODY_DOCS);

        sprintf(
            str_print, "[Info:requesting] CHG:RSP (%lX%lX > %lX%lX) : [%0.1f%%]  ", 
            challenge.data[0], challenge.data[1], 
            result.data[0], result.data[1],
            percent
        ); // extra space needed to clear screen buffer
        printf("%s", str_print);
        
        for (int b=0;b<len((const char*)str_print);b++) {
            printf("\b");
        }
        fflush(stdout);

        result.data[0] = 0;
        result.data[1] = 0;
    }

    printf("\r\n[Info:logfwr] Done.\r\n");

    //free(str_print);
}

void logcrp (struct parameters *p) {
    if (p->n <= 0) {
        printf("[Err:logcrp] Please specify the desired total CRP!\r\n");
        return;
    }

    FILE *file = fopen(p->fname, "w");
    union data_bits_u challenge;

    if (file == NULL) {
        printf("[Err:logcrp] Can\'t write file to the destination!\r\n");
        return;
    }

    printf("[Info:logcrp] Output set to : %s\r\n", p->fname);
    printf("[Info:logcrp] Generating %d Challenge Keys\r\n", (p->n));

    char *str_tmp = malloc(sizeof(char) * BUFFER_TMP_TEXT);
    float percent = 0.0;
    int total = 0;

    while (total < (p->n)) {
        randomizer(&challenge);
        for (uint8_t a=0;a<16;a++) {
            for (uint8_t b=0;b<8;b++) {
                fprintf(file, "%d", ( (challenge.byte[a] & (1 << (7 - b))) ? 1:0));
            }
        }

        fprintf(file, " : ");
        for (uint8_t a=0;a<16;a++) {
            fprintf(file, "%c%X", (challenge.byte[a] <= 0xF ? '0':'\0'), challenge.byte[a]);
        }
        fprintf(file, "\n");

        percent = (((float)total/(float)((p->n)-1)) * 100.0);
        sprintf(
            str_tmp, "[Info:generating] CRP (%lX%lX) : [%0.1f%%]  ", 
            challenge.data[0], challenge.data[1], 
            percent
        );
        printf("%s", str_tmp);

        for (uint8_t x=0;x<len((const char*)str_tmp);x++) {
            printf("\b");
        }
        fflush(stdout);

        total++;
    }

    printf("\r\n");

    fclose(file);
    free(str_tmp);
}

void logfeed (struct parameters *p) {
    union data_bits_u challenge;
    union data_bits_u response;

    FILE *file = fopen(p->crpfname, "a+");
    char file_str[BUFFER_TEXT_LOGFEED] = {0};

    printf("[Info:logfeed] Output set to \"%s\"\r\n", p->fname);
    printf("[Info:logfeed] k value set to %d\r\n", p->k);
    printf("[Info:logfeed] Reading the generated Challenge Keys\r\n");

    uint32_t file_lines = flines(p->crpfname); // total lines of the CRP file
    uint32_t lines_count = 0; // counting the lines count
    int ret = 0; // uart stream error return code
    uint8_t byte_average = 0;

    char *str_tmp = malloc(sizeof(char) * 80);
    float percent = 0.0;
    p->n = file_lines;

    union data_bits_u *average = malloc(sizeof(average) * file_lines);
    union data_bits_u result;

    printf("[Info:logfeed] Total lines : %d\r\n", file_lines);
    printf("[Info:logfeed] Begin request and writing to the output.\r\n");

    wlog(p, &challenge, &response, HEAD_DOCS);

    while (fgets(file_str, BUFFER_TEXT_LOGFEED, file) != NULL) {
        strcrpbin(&challenge, file_str);

        for (uint8_t y=0;y<(p->k);y++) {
            if (uart_begin(p) < 0) {
                printf("[Err:logfeed] Failed to initialize uart!\r\n");
                return;
            }

            if ((ret = uart_iostream(p, &challenge, &response)) < 0) {
                printf("[Err:logfeed] I/O error while trying communicate in uart!, (%d)\r\n", ret);
                return;
            }

            average[lines_count].data[0] = response.data[0];
            average[lines_count].data[1] = response.data[1];
        }
        
        /*for (uint8_t x=0;x<16;x++) {
            printf("%c%X", (average[lines_count].byte[x] < 0xF ? '0':'\0'), average[lines_count].byte[x]);
        }
        printf("\r\n");*/

        /* averaging process */
        for (uint8_t x=0;x<16;x++) {
            for (uint8_t y=0;y<8;y++) {
                for (uint8_t z=0;z<(p->k);z++) {
                    byte_average += ((average[z].byte[x] & (1 << (7-y))) ? 1:0);
                }
                
                byte_average = (uint8_t)((((float)byte_average/(float)(p->k))*10.0) >= 5 ? 1:0);

                if (byte_average) {
                    result.byte[x] |= (1 << (7-y));
                }

                byte_average = 0;
            }
        }

        wlog(p, &challenge, &result, BODY_DOCS);

        sprintf(
            str_tmp, "[Info:feeding] CHG:CRP (%lX%lX > %lX%lX) [%0.1f%%]  ",
            challenge.data[0], challenge.data[1],
            result.data[0], result.data[1],
            percent
        );

        printf("%s", str_tmp);
        for (uint8_t x=0;x<len(str_tmp);x++) {
            printf("\b");
        }
        fflush(stdout);

        result.data[0] = 0;
        result.data[1] = 0;

        percent = ((float)(lines_count+2)/(float)file_lines)*100.0;
        lines_count++;
    }

    printf("\r\n[Info:logfeed] Done.\r\n");
    
    free(str_tmp);
}



