#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
const int N = 2096;

// #define log(info, str) \
//     do {\
//         fprintf(tmp, "%s%s\n", info, str);\
//         fflush(tmp);\
//     } while (0)
struct tm *getNowTime();
void getlog(const char *info, const char *str);
void send_header(const int status, 
                char const *type, 
                char const *file_type, 
                char const *protocol);
void send_file(char *file);
void send_error(const int status, char const *info);
void checkHeader(const int argc, char const *argv[]);


int main(int argc, char const *argv[])
{
    char line[N], method[N], path[N], protocol[N];
    char *file, ich;
    FILE *fp;


    check(argc, argv);
    if(fgets(line, N, stdin) == NULL)
        send_error(404, "file doesn't exsist!");
    if(sscanf(line, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3)
        send_error(400, "fenjie error");
    while(fgets(line, N, stdin) != NULL)
        if(strcmp(line, "\r\n"))
            break;


    // choose method
    if(stricmp(method, "GET") != 0)
        send_error(400, "method error");


    // get file name
    if(path[0] != '/')
        send_error(400, "path error");
    file = path + 1;
    
    if(*file == '\0') send_file("index.html");
    else {
        send_file(file);
    }

    fflush(stdout);
    return 0;
}


struct tm *getNowTime()
{
    time_t timep;
    time(&timep);
    struct tm *p = localtime(&timep);
    return p;
}
void getlog(const char *info,const char *str)
{
    FILE *tmp = fopen("log.txt", "a");
    if(tmp == NULL) {
        perror("fopen tmp.txt error");
        exit(1);
    }
    
    fprintf(tmp, "%d/%d/%d-%d:%d:%d: ", 
        getNowTime()->tm_year + 1900,
        getNowTime()->tm_mon,
        getNowTime()->tm_mday,
        getNowTime()->tm_hour,
        getNowTime()->tm_min,
        getNowTime()->tm_sec);
    do {
        fprintf(tmp, "%s%s\n", info, str);
        fflush(tmp);
    } while (0);
    fclose(tmp);
}

void send_error(const int status, char const *info)
{
    printf("HTTP/1.1 %d ERROR\n", status);
    printf("content-Type: text/html\n\n");

    printf("%d %s",status, info);
    exit(1);
}

void send_header(const char *protocol,
                const int status, 
                const char *type, 
                const char *file_type, 
                const char *msg)
{
    printf("%s %d %s\n", protocol, status, type);
    printf("content-Type: %s;charset=utf8\n\n", file_type);
}
void check(const int argc, char const *argv[])
{
    if(argc != 2)
        send_error(500, "no input");
    if(chdir(argv[1]) == -1)
        send_error(500, "service error");
}

void send_file(char *file)
{
    int ich;
    FILE *fp = fopen(file, "r");
    if(fp == NULL) send_error(404, file);

    printf("HTTP/1.1 200 ok\n");
    printf("content-Type: text/plain;charset=utf8\n\n");
    while((ich = getc(fp)) != EOF)
        putchar(ich);
    fclose(fp);
}