#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iconv.h>
#include<ctype.h>
const int N = 2096;

// #define log(info, str) \
//     do {\
//         fprintf(tmp, "%s%s\n", info, str);\
//         fflush(tmp);\
//     } while (0)
struct tm *getNowTime();
static void send_header(const char *protocol, const int status, const char *type, const char *file_type, const char *msg);
static void send_error(const int status, char const *title, char *text);
static void strencode(const char *from, char *to, size_t tosize);
static void strdecode(const char *from, char *to, size_t tosize);
static void getlog(const char *info, const char *more);
static char *get_mine_type(const char *type);
static void send_file(char *file);

static void check(const int argc, char const *argv[]);
static char *strupr(char *str); // change the string to lower;

int main(int argc, char const *argv[])
{
    char line[N], method[N], path[N], protocol[N];
    char *file, ich;
    FILE *fp;


    check(argc, argv);
    if(fgets(line, N, stdin) == NULL)
        send_error(400, "Bad Request", "Header error");
    if(sscanf(line, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3)
        send_error(400, "Bad Request", "Release header error");
    while(fgets(line, N, stdin) != NULL)
        if(strcmp(line, "\r\n"))
            break;

    getlog(method, path);
    // choose method
    if(strcmp(strupr(method), "GET") != 0)
        send_error(400, "Bad Request", "Not GET method");


    // get file name
    if(path[0] != '/')
        send_error(400,"Bad Request" ,"Path error");
    file = path + 1;
    
    if(*file == '\0') send_file("index.html");
    else {
        send_file(file);
    }

    fflush(stdout);

    getlog("finished once http get","");
    return 0;
}


// struct tm *getNowTime()
// {
//     time_t timep;
//     time(&timep);
//     struct tm *p = localtime(&timep);
//     return p;
// }
void getlog(const char *info,const char *more)
{
    FILE *tmp = fopen("../logs/mailError.txt", "a");
    if(tmp == NULL) {
        perror("fopen tmp.txt error");
        exit(1);
    }

    time_t timep;
    time(&timep);
    struct tm *p = localtime(&timep);

    fprintf(tmp, "%d/%d/%d-%02d:%02d:%02d: ", 
        p->tm_year + 1900,
        p->tm_mon,
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

void send_error(const int status, char const *title, char *text)
{
    printf("HTTP/1.1 %d %s\n", status, title);
    printf("content-Type: text/html\n\n");
    printf("<h1>%d %s: %s</h1>",status, title, text);

    // sprintf(text, "error<%d>:%s", status, title);

    getlog("error:", " ");
    exit(1);
}

void send_header(const char *protocol, const int status, const char *type, const char *file_type, const char *msg)
{
    printf("%s %d %s\n", protocol, status, type);
    printf("content-Type: %s;charset=utf8\n\n", file_type);

}

void send_file(char *file)
{
    int ich;
    FILE *fp = fopen(file, "r");
    if(fp == NULL) send_error(404,"File not found","File not exsist");

    printf("HTTP/1.1 200 ok\n");
    // printf("content-Type: text/%s\n\n", get_mine_type(file));
    printf("content-Type: text/html;charset=utf8\n\n");
    while((ich = getc(fp)) != EOF)
        putchar(ich);
    fclose(fp);
}
void check(const int argc, char const *argv[])
{
    if(argc != 2)
        send_error(500, "Service Error", "Path didn't input");
    if(chdir(argv[1]) == -1)
        send_error(500, "Service Error", "Not a correct path");
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

static void strencode(const char *from, char *to, size_t tosize)
{
    iconv_t cd=iconv_open("UTF-8", "UNICODE");
    char *outbuf = (char *) malloc(tosize * 4);
    iconv(cd, &from, (size_t*)&tosize, &to, &tosize);
    free(outbuf);
    iconv_close(cd);
}
static void strdecode(const char *from, char *to, size_t tosize)
{

}