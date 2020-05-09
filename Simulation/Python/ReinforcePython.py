import socket
from threading import Thread
import threading
import time

HOST = '192.168.0.33'
PORT = 8585

recv_packet =[]
mode = 0
lock=threading.Lock()
condition_recv = threading.Condition()
condition_send = threading.Condition()

def recvMsg(sock,cv_recv):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                break
            recv_packet = data.decode().split(',')
            print(recv_packet)
            cv_recv.acquire()
            cv_recv.notifyAll()
            cv_recv.release()
        except:
            pass

def sendMsg(sock,cv_send):
    global mode
    sock.send("rei".encode())
    while True:
        with cv_send:
            cv_send.acquire()
            cv_send.wait()
            cv_send.release()

            sock.send(str(mode).encode())

def ReinForceThread(cv_send,cv_recv):
    global mode
    while True:
        print("what")
        cv_recv.acquire()
        cv_recv.wait()
        cv_recv.release()
        print("out")
        print(recv_packet)
        time.sleep(1)
        if mode < 20:
            print(mode)
            mode = mode + 1
        else:
            mode = 0
        print("recv")
        cv_send.acquire()
        cv_send.notifyAll()
        cv_send.release()

def TcpThread():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:

        sock.connect((HOST, PORT))
        thread_recv = Thread(target=recvMsg, args=(sock,condition_recv))
        thread_send = Thread(target=sendMsg, args=(sock,condition_send))
        thread_rein = Thread(target=ReinForceThread, args=(condition_send, condition_recv))
        thread_recv.daemon = True
        thread_send.daemon = True
        thread_rein.daemon = True
        thread_recv.start()
        thread_send.start()
        thread_rein.start()
        while(1):
            pass

TcpThread()
