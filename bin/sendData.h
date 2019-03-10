extern void send_header(const char *protocol, const int status, const char *type, const char *file_type, const char *msg);
extern void send_error(const int status, char const *title, char *text);
extern void send_dir(const char *file);
extern void send_file(char *file);
extern void file_infos(char *dir, char *name);