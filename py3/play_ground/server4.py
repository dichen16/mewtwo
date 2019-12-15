# server.py

from fib import fib
from socket import *
from collections import deque

tasks = deque()
recv_wait = { }     # mapping sockets -> tasks
send_wait = { }

def run():
    while tasks:
        task = tasks.popleft()
        try:
            why, what = next(task)      #run to the yield
            if why == 'recv':
                # must go wait somewhere
                recv_wait[what] = task
                pass
            elif why == 'send':
                send_wait[what] = task
                pass
            else:
                raise RuntimeError("ARG!")
        except StopIteration:
            print("task done")


def fib_server(address):
    sock = socket(AF_INET, SOCK_STREAM)
    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    sock.bind(address)
    sock.listen(5)
    while True:
        yield 'recv', sock 
        client, addr = sock.accept()    #blocking
        print("Connection", addr)
        fib_handler(client)


def fib_handler(client):
    while True:
        yield 'recv', client
        req = client.recv(100)          #blocking
        if not req:
            break
        n = int(req)
        result = fib(n)
        resp = str(result).encode('ascii') + b'\n'
        yield 'send', client
        client.send(resp)               #blocking
    print("Closed")


tasks.append(fib_server(("", 25000)))
