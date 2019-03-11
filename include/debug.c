#include "debug.h"

// #define log(info, str) \
//     do {\
//         fprintf(tmp, "%s%s\n", info, str);\
//         fflush(tmp);\
//     } while (0)
// struct tm *getNowTime()
// {
//     time_t timep;
//     time(&timep);
//     struct tm *p = localtime(&timep);
//     return p;
// }
extern void getlog(const char *info,const char *more)
{
    FILE *tmp = fopen("../logs/mainError.txt", "a");
    if(tmp == NULL) {
        perror("fopen tmp.txt error");
        exit(1);
    }

    time_t timep;
    time(&timep);
    struct tm *p = localtime(&timep);

    fprintf(tmp, "%d/%d/%d-%02d:%02d:%02d: ", 
        p->tm_year + 1900,
        p->tm_mon + 1,
        p->tm_mday,
        p->tm_hour,
        p->tm_min,
        p->tm_sec);
    do {
        fprintf(tmp, "%s%s\n", info, more);
        fflush(tmp);
    } while (0);
    fclose(tmp);
}