import errno
import socket
import signal
import subprocess
import os

SERVER_ADDRESS = (HOST, PORT) = '', 8888
REQUEST_QUEUE_SIZE = 10
SITE_PATH = 'site/'

def grim_reaper(signum, frame):
    pid, status = os.wait()

def response_get(path):
    http_response = b"\nHTTP/1.1 404 false\n\nfile isn't exists\n"
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
    return http_response

def response_post(http_params, data):
    http_response = b"\nHTTP/1.1 400 false\n\nerror request\n"
    
    # 登陆用户名
    username = http_params.partition("username=")[2].partition("&")[0]
    # data写入文件
    new_file_name = username + ".cpp"
    with open(new_file_name, 'w') as f:
        f.write(data)
        http_response = b"\nHTTP/1.1 200 ok\r\n\r\nupdated!!\n"

    return http_response

    # 调用cpp底层check是否可以运行

    # 假设检查完毕
    stdout = subprocess.call("g++ " + new_file_name + " -o " + username, shell=True)
    stdout = subprocess.getoutput("cat in.txt | ./" + username + " > out.txt")
    print("debug:::",stdout)
    
    # 编辑数据到数据库

    print("running done")

    

def handle_request(client_connection):
    """处理请求"""
    # 接受的第一次数据包含了http请求头部，分割出剩余的数据部分
    request = client_connection.recv(1024)
    http_type, http_params, http_protocol = request.decode().split('\n')[0].split(' ')
    data = request.decode().partition('\r\n\r\n')[2]
    
    # 循环接受剩余的数据
    while True:
        if len(request) < 1024:
            break
        request = client_connection.recv(1024)
        data += (request.decode())
    
    # 先设置为默认的请求失败字符串
    http_response = b"\nHTTP/1.1 400 false\n\nerror request\n"
    # 现在先处理了get与post请求
    if http_type.lower() == "get":
        http_response = response_get(http_params.split('?')[0])
    elif http_type.lower() == "post":
        http_response = response_post(http_params, data)
    # 返回数据    
    client_connection.sendall(http_response)


def server_loop():
    # 打开socket，绑定到指定端口，设置端口复用，并行开发
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_socket.bind(SERVER_ADDRESS)
    listen_socket.listen(REQUEST_QUEUE_SIZE)
    print("Serving HTTP on port {port} ...".format(port=PORT))

    # 监听子进程的退出的回调函数，并行开发
    signal.signal(signal.SIGCHLD, grim_reaper)

    while True:
        try:
            # 尝试连接，如果不成功，并且是因为EINTR则尝试重新连接，否则退出
            client_connection, client_address = listen_socket.accept()
        except IOError as e:
            code, msg = e.args
            if code == errno.EINTR:
                continue
            else:
                raise

        pid = os.fork()
        if pid == 0:  # child
            # 子进程只会运行一次，要关闭socket，否则会占用不必要的资源
            listen_socket.close()
            handle_request(client_connection)
            client_connection.close()
            os._exit(0)
        else:  # parent
            # 没吃运行的时候父进程要关闭连接，否则子进程关闭后，连接无法关闭，导致资源浪费
            client_connection.close()


if __name__ == "__main__":
    server_loop()
