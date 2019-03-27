import os

HOST, PORT = '', 8888
SERVER_ADDRESS = (HOST, PORT)
REQUEST_QUEUE_SIZE = 4
SITE_PATH = 'site/'
BAD_REQUEST = b"\nHTTP/1.1 400 false\n\nerror request\n"

def response_get(client_connection, path):
    http_response = BAD_REQUEST
    if len(path) == 0:
        path = '/'
    if os.path.isdir(path):
        if path[-1] != '/':
            path += '/'
        path += "index.html"
    path = SITE_PATH + path[1:]

    if os.path.isfile(path):
        http_response = b"\nHTTP/1.1 200 ok\n\n"
        with open(path) as f:
            http_response += bytes(f.read(), "utf8")

    client_connection.sendall(http_response)
    client_connection.close()
    return http_response