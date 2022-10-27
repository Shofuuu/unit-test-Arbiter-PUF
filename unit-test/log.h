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

int wlog (struct parameters *p, unsigned char mode);

#endif // LOG_H
