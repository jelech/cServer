#include "include/headers.h"

void send_post();
void send_get(char*);

int main(int argc, char const *argv[])
{
    char line[N], method[N], path[N], protocol[N], idx[N];
    
    // printf("debuf%d\n", argc);
    // printf("ss%s\n", argv[1]);
    if(argc != 2)
        send_error(500, "Service Error", "Path didn't input");
    if(chdir(argv[1]) == -1)
        send_error(500, "Service Error", "Not a correct path");

    if(fgets(line, N, stdin) == NULL)
        send_error(400, "Bad Request", "Header error");
    if(sscanf(line, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3)
        send_error(400, "Bad Request", "Release header error");
    while(fgets(line, N, stdin) != NULL && !strcmp(line, "\r\n")) // 获取剩余的无用头数据
        continue;

    
    // choose method
    if(strcmp(strupr(method), "GET") == 0) {
        send_get(path);
    }
    else if (strcmp(strupr(method), "POST") == 0) {
        char ret[1024];
        // while (fgets(line, N, stdin) != NULL) // 获取剩余的无用头数据
        //     strcat(ret, line);
            // printf("%s\n", line);
        
        printf( "HTTP/1.1 200 oK\n");
        printf("Content-Type: text/text;charset=utf8\r\n");
        // printf("%s???\n", ret);
        // send_post(line);
        // send_get(path);
    }
    else {
        send_error(400, "Bad Request", "method Error");
    }

    fflush(stdout);
    // getlog("finished once http get","");
    return 0;
}

void send_get(char *path)
{
    struct stat sb;
    char *file;
    if(path[0] != '/')
        send_error(400,"Bad Request" ,"Path error");
    file = path + 1;
    if(file[0] == '\0')
        file = "index.html";

    // demo 去掉get请求中, '?'后面的字符 
    // getParameter()

    // 比较前面几个字符，如果为异常字符，则直接跳转到主页
    int len = sizeof(file); 
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
}

void send_post(char * line)
{
    printf("HTTP/1.1 200 ok\n");
    printf("Content-Type: text/html;charset=utf8\n\n");
    while(fgets(line, N, stdin) != NULL && !strcmp(line, "\r\n")) // 获取剩余的无用头数据
        printf("%s\n", line);
}


