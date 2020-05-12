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

Hungry_Para   = 0
Tired_Para    = 0
Touch_Sensor  = 0
Force_Sensor  = 0
Lift_Sensor   = 0
Oled_State    = 0
Fin_State     = 0
Tail_State    = 0
Face_Detect   = 0
Reward        = 0
Mode_Select   = 0

def recvMsg(sock,cv_recv):
    global  recv_packet
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
    global mode, recv_packet
    global Hungry_Para, Tired_Para,Touch_Sensor,Force_Sensor,Lift_Sensor,Oled_State,Fin_State,Tail_State,Face_Detect,Reward,Mode_Select
    while True:
        # Receive Mutex
        cv_recv.acquire()
        cv_recv.wait()
        cv_recv.release()
        Hungry_Para     = recv_packet[1]
        Tired_Para      = recv_packet[2]
        Touch_Sensor    = recv_packet[3]
        Force_Sensor    = recv_packet[4]
        Lift_Sensor     = recv_packet[5]
        Oled_State      = recv_packet[6]
        Fin_State       = recv_packet[7]
        Tail_State      = recv_packet[8]
        Face_Detect     = recv_packet[10]
        Reward          = recv_packet[11]
        Mode_Select     = recv_packet[12]
        # Input
        # Mode Control
        if mode < 19:
            print("mode:",mode)
            mode = mode + 1
        else:
            mode = 0
        # Receive Mutex
        cv_send.acquire()
        cv_send.notifyAll()
        cv_send.release()

def TcpThread():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((HOST, PORT))
        # Thread Init
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

if __name__ == "__main__":
    TcpThread()
