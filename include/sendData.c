#include "sendData.h"
extern void send_header(const char *protocol, const int status, const char *type, const char *file_type, const char *msg)
{
    printf("%s %d %s\n", protocol, status, type);
    printf("Content-Type: %s;charset=utf8\n\n", file_type);
}
extern void send_file(char *file)
{
    int ich;
    char fileType[N];

    
    FILE *fp = fopen(file, "r");
    if(fp == NULL) send_error(404,"File not found","File not exsist");

    sscanf(file, "%*[^.].%s", fileType);
    printf("HTTP/1.1 200 ok\n");
    printf("content-Type: %s\n\n", get_mine_type(fileType));
    while((ich = getc(fp)) != EOF)
        putchar(ich);
    fclose(fp);
    exit(1);
}
extern void send_dir(const char *file)
{
    char rePath[N], indexPath[N];
    struct dirent **dl;
    struct stat sb;
    int n;
    if(file[strlen(file) - 1] != '/')
        sprintf(rePath, "%s/", file);
    else
        sprintf(rePath, "%s", file);

    size_t len = strlen(rePath);

    snprintf(indexPath, sizeof(indexPath), "%sindex.html", rePath);
    if (stat(indexPath, &sb) >= 0) {
        send_file(indexPath);
    } else { // 展示目录，注意建议不进行展示
        n = scandir(rePath, &dl, NULL, alphasort); // 第三个为过滤器
        if (n < 0) {
            getlog("scandir error:", rePath);
        }
        else {
            send_header("http/1.1", 200, "ok", "html", "ok");
            for (int i = 2; i != n; ++i) {
                file_infos(rePath, dl[i]->d_name);
            }
        }
    }
    exit(1);
}
extern void send_error(const int status, const char *title, char *text)
{
    printf("HTTP/1.1 %d %s\n", status, title);
    printf("Content-Type: text/html;charset=utf8\n\n");
    printf("<h1>%d %s: %s</h1>",status, title, text);
    getlog("error:", " ");
    exit(1);
}
void file_infos(char *dir, char *name)
{
    /*
        bugs: 子目录访问有问题
    */
    char encoded_name[N];
    char path[N];
    char timestr[16];
    struct stat sb;

    strencode(name, sizeof(name), encoded_name);
    snprintf(path, sizeof(path), "%s%s", dir, name);
    
    if (lstat(path, &sb) < 0) {
        perror("lstat error");
    }
    else {
        strftime(timestr, sizeof(timestr), "%d%b%Y %H:%M", localtime(&sb.st_mtime));
        printf("<a href=\"../%s\">%32s</a>\t\t\t\t%15s\t%14ld<br>\n", path, name, timestr, (int64_t)sb.st_size);
    }
    fflush(stdout);
}