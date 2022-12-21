/*
* Document : log.h
* Date     : 17/10/2022 - 23:50:34
* Author   : Muhammad Shofuwan Anwar
*
* -- Additional Info -- 
* Logging testing hardware activity
* on average CRP test
*/

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>

#include "parameters.h"

#define BODY_DOCS   0xFE
#define HEAD_DOCS   0xFF

int wlog (struct parameters *p, union data_bits_u *c, union data_bits_u *r, unsigned char mode);
int ctylog (struct parameters *p); // continue log, get the total lines and continue the process

void parstr (const char* str, uint32_t *value); // parse str, parsing the value of k and n from a single string
int acsbl (struct parameters *p); // is the file accessible?

#endif // LOG_H
