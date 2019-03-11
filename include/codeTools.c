#include "codeTools.h"
extern char *strupr(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len; ++i) 
        if (islower(str[i]))
            str[i] = toupper(str[i]);

    return str;
}
extern char *get_mine_type(const char *type)
{
    if (strcmp(type, "html") == 0) {
        return "text/html;charset=utf8;";
    }
    if (strcmp(type, "css") == 0) {
        return "text/css;charset=utf8;";
    }
    if (strcmp(type, "jpg") == 0) {
        return "image/jpg;";
    }
    if (strcmp(type, "png") == 0) {
        return "image/png;";
    }
    return "text/plain;";
}
