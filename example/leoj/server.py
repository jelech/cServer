import os
import re
import time
import errno
import socket
import signal
import subprocess
from le_resp_get import response_get
import redis

HOST, PORT = '', 8888
SERVER_ADDRESS = (HOST, PORT)
REQUEST_QUEUE_SIZE = 4
SITE_PATH = 'site/'
BAD_REQUEST = b"\nHTTP/1.1 400 false\n\nerror request\n"
PROBLEM_ROOT_PATH = "checkProblem/"


def grim_reaper(signum, frame):
    pid, status = os.wait()

def handle_request(client_connection):
    """处理请求"""
    # 接受的第一次数据包含了http请求头部，分割出剩余的数据部分
    request = client_connection.recv(1024)
    http_type, http_params, http_protocol = request.decode().split('\n')[0].split(' ')
    data = request.decode().partition('\r\n\r\n')[2]

    while len(request) >= 1024:
        request = client_connection.recv(1024)
        data += request.decode()

    # 现在先处理了get与post请求
    if http_type.lower() == "get":
        response_get(client_connection, http_params.split('?')[0])
    elif http_type.lower() == "post":
        response_post(client_connection, http_params, data)

def response_post(client_connection, http_params, data):
    http_response = BAD_REQUEST
    problemNum = '00001'
    username = http_params.partition("username=")[2].partition("&")[0]
    print(username)
    checkFileDir = PROBLEM_ROOT_PATH + problemNum + "/"
    compiledFileName = username + str(os.getpid())
    sourceFileName = compiledFileName + ".cpp"

    with open(sourceFileName, 'w') as f:
        f.write(data)

    http_response = b"\nHTTP/1.1 200 ok\n\nupdated!\n"

    # ====================================
    # check the cpp whether can run MODE
    # ====================================
    client_connection.sendall(http_response)
    client_connection.close()

    redisDB = redis.Redis(host='127.0.0.1', port=6379)
    redisDB.lpush("running_queue", "%s+%s+%s+%f"%(checkFileDir, compiledFileName, username, time.time()))


def server_loop():
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_socket.bind(SERVER_ADDRESS)
    listen_socket.listen(REQUEST_QUEUE_SIZE)
    print("Serving HTTP on port {port} ...".format(port=PORT))

    signal.signal(signal.SIGCHLD, grim_reaper)
    while True:
        try:
            client_connection, client_address = listen_socket.accept()
        except IOError as e:
            code, msg = e.args
            if code != errno.EINTR:
                continue
            else:
                raise

        pid = os.fork()
        if pid == 0:  # child
            listen_socket.close()
            handle_request(client_connection) # 内部关闭
            os._exit(0)
        else:  # parent
            client_connection.close()

if __name__ == "__main__":
    server_loop()
