#define _SVID_SOURCE

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iconv.h>
#include<ctype.h>
#include<sys/stat.h>
#include<dirent.h>

#include "debug.h"
#include "sendData.h"
#include "encode.h"

#ifndef N
    #define N 2096
#endif