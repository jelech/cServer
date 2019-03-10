#ifndef _ENCODE_H_
    #define _ENCODE_H_
    extern void strencode(const char *from, size_t fromSize, char *to);
    extern void strdecode(const char *from, char *to, size_t tosize);
#endif