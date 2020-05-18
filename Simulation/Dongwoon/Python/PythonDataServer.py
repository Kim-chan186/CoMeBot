import socketserver
import threading
from time import struct_time

HOST = '192.168.0.33'
PORT = 8585
lock = threading.Lock()  # syncronized 동기화 진행하는 스레드 생성
sync_lock = threading.Lock()
cpp_Packet = []
py_Packet = []
stt_data_num = 0


class DataManager:

    def __init__(self):
        self.IDs = {}  # 사용자의 등록 정보를 담을 사전 {사용자 이름:(소켓,주소),...}

    def addID(self, Id, conn, addr):  # 사용자 ID를 self.IDs에 추가하는 함수
        if Id in self.IDs:  # 이미 등록된 사용자라면
            conn.send('이미 등록된 ID입니다.\n'.encode())
            return None

        lock.acquire()  # 스레드 동기화를 막기위한 락
        self.IDs[Id] = (conn, addr)
        lock.release()  # 업데이트 후 락 해제

        print('+++ Client 수 [%d]' % len(self.IDs))
        return Id

    def removeID(self, Id):  # 사용자를 제거하는 함수
        if Id not in self.IDs:
            return

        lock.acquire()
        del self.IDs[Id]
        lock.release()

        print('--- Client 수 [%d]' % len(self.IDs))
    
    def messageHandler(self, Id, msg, port):
        global stt_data_num
        if msg[0] != '/':  # 보낸 메세지의 첫문자가 '/'가 아니면
            if Id == "cpp":
                self.sendMessageToOne("rei", port, msg)
            if Id == "rei":
                py_Packet.append(msg.split(','))
                self.sendMessageToOne("cpp", port, msg)
            if Id == "stt":
                stt_data_num = int(msg)
                print("stt:",stt_data_num)
            return
        if msg.strip() == '/quit':  # 보낸 메세지가 'quit'이면
            self.removeID(Id)
            return -1

    def sendMessageToAll(self, msg):
        for conn, addr in self.IDs.values():
            conn.send(msg.encode())
            
    def sendMessageToOne(self,sendId,port,msg):
        global stt_data_num
        print(sendId,":",msg,"\n")
        buffer = msg
        for Id, info in self.IDs.items():
            print(Id, " * ",info[1][1],"*", port)
            if Id == sendId :
                conn = info[0] #IDs = {Id : (conn, addr)}
                if sendId == "cpp":
                    buffer = ''.join(msg)
                    conn.send(buffer.encode())
                elif sendId == "rei":
                    buffer = ''.join(msg) + ","  + str(stt_data_num)
                    stt_data_num = 0
                    conn.send(buffer.encode())
                elif sendId == "stt":
                    buffer = "stt data" + msg
                    conn.send(msg.encode())
                else:
                    print("해당되는 ID가 없습니다")

class MyTcpHandler(socketserver.BaseRequestHandler):
    userman = DataManager()

    def handle(self):  # 클라이언트가 접속시 클라이언트 주소 출력
        print('[%s] 연결됨' % self.client_address[0])
        try:
            Id = self.registerId()
            msg = 1
            while msg:
                msg = self.request.recv(1024)
                if self.userman.messageHandler(Id, msg.decode(),self.client_address[1]) == -1:
                    self.request.close()
                    break
        except Exception as e:
            print(e)
        print('[%s] 접속종료' % self.client_address[0])
        self.userman.removeID(Id)

    def registerId(self):
        while True:
            self.request.send('ID:'.encode())
            Id = self.request.recv(1024)
            Id = Id.decode().strip()
            if self.userman.addID(Id, self.request, self.client_address):
                return Id


class ChatingServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


def runServer():
    print('+++ 서버를 시작합니다.')
    try:
        server = ChatingServer((HOST, PORT), MyTcpHandler)
        server.serve_forever()
    except KeyboardInterrupt:
        print('--- 채팅 서버를 종료합니다.')
        server.shutdown()
        server.server_close()

runServer()
