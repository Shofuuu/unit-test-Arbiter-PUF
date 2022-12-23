/*
* Document : puflog.c
* Date     : 17/10/2022 - 23:53:15
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* Main "*.c" file for puf unit-test
*/

#define MAX_HELPMSG 19

#define MODE_FULL   0xF0
#define MODE_EXPORT 0xF1
#define MODE_FEED   0xF2

/* 
 * special header just for collecting
 * dependencies component to tidy up
 * the codes
 * */
#include "modeop.h"

int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("[Err:argc] Please specify at least one argument!\r\n");
        exit(-1);
    }

    const char *helpmsg[MAX_HELPMSG] = {
        "Help information :\r\n",
        " puflog [h, o, c, n, k, p, b, f, x, e]\r\n",
        "\nExample :\r\n",
        " puflog -o [output]\r\n",
        " puflog -o [output] -n [iteration log]\r\n",
        " puflog -o [output] -n [iteration log] -k [iteration avg]\r\n",
        " puflog -o [output] -k [iteration avg]\r\n",
        " puflog -o [output] -c\r\n",
        " puflog -o [output] -e -n [total CRP]\r\n",
        " puflog -o [output] -f [CRP location] -k [iteration avg]\r\n",
        "\nInformation :\r\n",
        " \"k (average)\" has default value 8\r\n",
        " \"n (iteration)\" has default value 100\r\n",
        " \"p (port)\" has default value /dev/ttyUSB0\r\n",
        " \"b (baudrate)\" has default value 9600\r\n",
        " \"c (continue)\" used to continue last work\r\n",
        " \"x (format)\" available formats are \"bin\" (binary) and \"hex\" (hexadecimal)\r\n",
        " \"f (feed)\" feed the logger with the generated challenge keys files format\r\n",
        " \"e (export)\" export the Challenge Keys\r\n"
    };

    char opt = 0;
    uint8_t valid = 0;
    uint8_t mode = MODE_FULL;

    struct parameters p;
    uparsetattr(&p);

    while( (opt = getopt(argc, argv, "hceo:n:k:p:b:f:x:")) != -1 ){
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
            break;

            case 'b':
                p.baudrate = atoi(optarg);
            break;

            case 'c':
                p.resuming = 1;
                valid++;
            break;

            case 'f':
                p.crpfname = optarg;
                mode = MODE_FEED;
                valid++;
            break;

            case 'e':
                mode = MODE_EXPORT;
                valid++;
            break;

            case 'x':
                p.format = optarg;
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

    if (valid > 0) {
        if (p.fname == NULL) {
            printf("[Err:fname] File name can\'t be NULL\r\n");
            return -1;
        } else {
            switch (mode) {
                case MODE_FULL:
                    logfwr(&p);
                break;

                case MODE_EXPORT:
                    logcrp(&p);
                break;

                case MODE_FEED:
                    logfeed(&p);
                break;

                default:
                    printf("[Err:mode] Invalid mode operation, please kindly check the help message \'-h\' for further information!\r\n");
                    return -1;
                break;
            }
        }
    }

    return 0;
}



