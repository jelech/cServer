#include "server.h"

void leoj_cb(struct evhttp_request* req, void* arg)
{
    char* s = "This is the test buf";
    const struct evhttp_uri* evhttp_uri = evhttp_request_get_evhttp_uri(req);
    char url[8192];
    evhttp_uri_join(evhttp_uri, url, 8192);
 
    printf("accept req url:%s\n", url);
 
    struct evbuffer* evbuf = evbuffer_new();
    if (!evbuf)
    {
        printf("create evbuffer failed!\n");
        return ;
    }
 
    evbuffer_add_printf(evbuf, "Server response. Your req url is %s", url);
    evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
    evbuffer_free(evbuf);
}

void generic_cb(struct evhttp_request* req, void* arg)
{
    char *s = "leoj buf";
    const struct evhttp_uri *evhttp_uri = evhttp_request_get_evhttp_uri(req);
    char url[8192];

    evhttp_uri_join(evhttp_uri, url, 8192);
    printf("accept req url:%s\n", url);

    char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);
    printf("post info:%s\n", post_data);

    evhttp_add_header(req->output_headers, "Server", "myhttpd v 0.0.1");
    evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");

    struct evbuffer *evbuf = evbuffer_new();
    if(!evbuf) {
        perror("evbuffer error!");
        return;
    }

    evbuffer_add_printf(evbuf, "Server response. Your req url is %s", url);
    evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
    evbuffer_free(evbuf);
}
int main(int argc, char const *argv[])
{
    int ret;
    short http_port = 8887;
    char *http_addr = "0.0.0.0";

    struct event_base *base;
    struct evhttp *server;

    // 创建基座，插入server事件
    base = event_base_new();
    if((server = evhttp_new(base)) == NULL){
        perror("open server error");
        return -1;
    }

    // 打开socket并绑定到server上
    if((ret = evhttp_bind_socket(server, http_addr, http_port)) != 0) {
        perror("open socket&bind error");
        return -1;
    }

    evhttp_set_cb(server, "./leoj", leoj_cb, (void *)"arg");
    evhttp_set_gencb(server, generic_cb, NULL);

    printf("http server Running");

    event_base_dispatch(base);
    evhttp_free(server);

    return 0;
}
