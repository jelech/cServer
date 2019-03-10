#include "bin/headers.h"

char *strupr(char *str); // change the string to lower;
char *get_mine_type(const char *type);

int main(int argc, char const *argv[])
{
    char line[N], method[N], path[N], protocol[N], idx[N];
    char *file, ich;
    struct stat sb;
    struct dirent **dl;
    size_t len;
    FILE *fp;
    
    if(argc != 2)
        send_error(500, "Service Error", "Path didn't input");
    if(chdir(argv[1]) == -1)
        send_error(500, "Service Error", "Not a correct path");

    if(fgets(line, N, stdin) == NULL)
        send_error(400, "Bad Request", "Header error");
    if(sscanf(line, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3)
        send_error(400, "Bad Request", "Release header error");

    while(fgets(line, N, stdin) != NULL)
        if(strcmp(line, "\r\n"))
            break;

    getlog(line, path);
    
    // choose method
    if(strcmp(strupr(method), "GET") != 0)
        send_error(400, "Bad Request", "Not GET method");


    // get file name
    if(path[0] != '/')
        send_error(400,"Bad Request" ,"Path error");
    file = path + 1;
    if(file[0] == '\0'){
        file = "index.html";
        // send_file(file);
    }

    // demo 去掉get请求中, '?'后面的字符 
    // getParameter()

    // 比较前面几个字符，如果为异常字符，则直接跳转到主页
    len = sizeof(file); 
    if(file[0] == '/' || strcmp(file, ".") == 0
                      || strncmp(file, "../", 3) == 0
                      || strstr(file, "/../") != NULL
                      || strcmp(&file[len - 3], "/..") == 0) {
        send_error(400, "Bad Request", "error request path");
    }

    char rePath[N];
    if (stat(file, &sb) == -1)
        send_error(404, "error", file);

    if (S_ISDIR(sb.st_mode)) { //是目录
        send_dir(file);
    } else { // 是文件
        send_file(file);
    }

    fflush(stdout);
    getlog("finished once http get","");
    return 0;
}


char *strupr(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len; ++i) 
        if (islower(str[i]))
            str[i] = toupper(str[i]);

    return str;
}


char *get_mine_type(const char *type)
{
    if (strcmp(type, "html") == 0) {
        return "text/html;charset=utf8;";
    }
    if (strcmp(type, "css") == 0) {
        return "text/css;charset=utf8;";
    }
    if (strcmp(type, "jpg") == 0) {
        return "image/jpg;charset=utf8;";
    }
    if (strcmp(type, "png") == 0) {
        return "image/png;charset=utf8;";
    }
    return "text/plain;charset=utf8;";
}
