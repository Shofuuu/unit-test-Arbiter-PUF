/*
* Document : modeop.h
* Date     : 16/12/2022 - 00:25:18
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* Operation modes from user request
*/

#ifndef MODEOP_H
#define MODEOP_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parameters.h"
#include "uart.h"
#include "log.h"

#define GENERATE_CRP    0xF0
#define FEED_CRP        0xF1

#define BUFFER_TMP_TEXT     100
#define BUFFER_TEXT_LOGFEED 190

void logfwr (struct parameters *p); // log file write
void logcrp (struct parameters *p); // log crp (challenge response pair)
void logfeed (struct parameters *p); // log feed from generated CRP files

#endif // MODEOP_H
