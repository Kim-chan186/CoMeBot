import socket

from threading import Thread

import threading

import time

from timeout import keyboard_timer
# 서버의 주소입니다. hostname 또는 ip address를 사용할 수 있습니다.

HOST = '192.168.0.25'

# 서버에서 지정해 놓은 포트 번호입니다.

PORT = 8585

condition_recv = threading.Condition()

condition_send = threading.Condition()

keyboard = 0

stt_mode = 0

stt_list = ['1','2','3','4','5']

flag = [0, 0, 0, 0, 0]


def keyboard_event(cv_recv):
    global keyboard, flag

    while True:
        keyboard = keyboard_timer()
        if keyboard in stt_list:
            if keyboard == '1':
                flag[0] = 1
            elif keyboard == '2':
                flag[1] = 1
            elif keyboard == '3':
                flag[2] = 1
            elif keyboard == '4':
                flag[3] = 1
            elif keyboard == '5':
                flag[4] = 1
            else:
                pass
        #keyboard = input('keyboard:')
        cv_recv.acquire()

        cv_recv.notifyAll()

        cv_recv.release()


def sendMsg(sock, cv_send):
    global stt_mode

    sock.send("stt".encode())

    while True:
        with cv_send:
            cv_send.acquire()

            cv_send.wait()

            cv_send.release()

            sock.send(str(stt_mode).encode())


def STTThread(cv_send, cv_recv):
    global flag

    global stt_mode

    while True:

        cv_recv.acquire()

        cv_recv.wait()

        cv_recv.release()

        if flag == [0, 0, 0, 0, 0]:
            stt_mode = 0
            cv_send.acquire()

            cv_send.notifyAll()

            cv_send.release()
        else:
            for i in range(5):
                if flag[i] == 1:
                    stt_mode = int(i)+1

                    cv_send.acquire()

                    cv_send.notifyAll()

                    cv_send.release()

                    flag = [0, 0, 0, 0, 0]


        # for stt_num in stt_list:
        #
        #     if stt_num == keyboard:
        #         #stt_num = int(stt_num)
        #         # if flag[stt_mode - 1] == 0:
        #         #     flag[stt_mode - 1] = 1
        #         stt_mode = int(stt_num)
        #
        #
        #         cv_send.acquire()
        #
        #         cv_send.notifyAll()
        #
        #         cv_send.release()
        #
        #         #stt_mode = 0


def TcpThread():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((HOST, PORT))

        thread_keyboard = Thread(target=keyboard_event, args=(condition_recv,))

        thread_send = Thread(target=sendMsg, args=(sock, condition_send,))

        thread_stt = Thread(target=STTThread, args=(condition_send, condition_recv))

        thread_keyboard.daemon = True

        thread_send.daemon = True

        thread_stt.daemon = True

        thread_keyboard.start()

        thread_send.start()

        thread_stt.start()

        while (1):
            pass


TcpThread()

