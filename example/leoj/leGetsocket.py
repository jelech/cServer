import socket
def leSocket(addr, queSize):
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_socket.bind(addr)
    listen_socket.listen(queSize)
    print("Serving HTTP on port {port} ...".format(port=PORT))
    return listen_socket