#include "include/headers.h"

int main(int argc, char const *argv[])
{
    char line[N], method[N], path[N], protocol[N], idx[N];
    char *file;
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
    
    // choose method
    if(strcmp(strupr(method), "GET") != 0)
        send_error(400, "Bad Request", "Not GET method");


    // get file name
    if(path[0] != '/')
        send_error(400,"Bad Request" ,"Path error");
    file = path + 1;
    if(file[0] == '\0')
        file = "index.html";

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

    if (stat(file, &sb) == -1)
        send_error(404, "error", file);

    if (S_ISDIR(sb.st_mode))
        send_dir(file);
    else
        send_file(file);

    fflush(stdout);
    getlog("finished once http get","");
    return 0;
}