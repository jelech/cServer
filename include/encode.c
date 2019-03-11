#include "encode.h"

extern int hexit(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    return -1;
}
extern void strencode(const char *from, size_t fromSize, char *to) // unicode的中文转utf8
{
    int nlen;
    for (nlen = 0; *from != '\0' && nlen + 4 < fromSize; ++from) {
        if (isalnum(*from) || strchr("/.-~", *from) != (char*)0) {
            *to = *from;
            ++to;
            ++nlen;
        }
        else { //需要转义
            sprintf(to, "%%%02x", (int)*from & 0xff);
            to += 3;
            nlen += 3;
        }
    }
    to = '\0';
}
extern void strdecode(const char *from, char *to, size_t tosize) // utf8转Unicode的中文
{
    for (; *from != '\0'; ++from) {
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2])) {
            *to = hexit(from[1]) * 16 + hexit(from[2]);
            from += 2;
        }
        else {
            *to = *from;
        }
    }
    *to = '\0';
}
