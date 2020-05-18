import socket
from threading import Thread
import threading
import time
# from Scripts.Comebot_Train import reinforce

from Comebot_Env import ComebotEnv
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout
from keras.optimizers import Adam
from rl.agents.dqn import DQNAgent
from rl.policy import LinearAnnealedPolicy, EpsGreedyQPolicy
from rl.memory import SequentialMemory
from rl.callbacks import ModelIntervalCheckpoint, FileLogger

HOST = '192.168.0.33'
PORT = 8585

recv_packet =[]
mode = []
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
STT_Mode   = 0

def build_model(state_size, num_actions):
    # Sequential()은 층을 나열해서 쌓을 때 사용, 이전층의 출력이 다음층의 출력으로 이어짐 (Linear stack layer)
    model = Sequential()
    # Flatten()은 state_size가 (10,) 10차원이므로 (1,10)로 일렬화해주는(1차원으로 바꿔주는) 레이어
    model.add(Flatten(input_shape=(1, state_size)))
    # 각 은닉층의 역할 뭔지???일수있나??
    model.add(Dense(30, activation='relu'))  #
    model.add(Dense(10, activation='relu'))  #
    model.add(Dropout(0.3))
    # 출력노드의 개수는 num_actions=20개, linear활성화함수는 input을 그대
    # 로 return하는 함수
    # 각 action별 q값 그대로 출력
    model.add(Dense(num_actions, activation='linear'))
    model.summary()
    return model
def build_callbacks(env_name):
    checkpoint_weights_filename = 'dqn_' + env_name + '_weights_{step}.h5f'
    log_filename = 'dqn_{}_log.json'.format(env_name)
    callbacks = [ModelIntervalCheckpoint(checkpoint_weights_filename, interval=500)]
    callbacks += [FileLogger(log_filename, interval=100)]
    return callbacks


def recvMsg(sock,cv_recv):
    global  recv_packet

    while True:
        try:
            data = sock.recv(1024)
            if not data:
                break
            recv_packet=[]
            for data in data.decode().split(','):
                recv_packet.append(data)

            print("recv::",recv_packet)
            cv_recv.acquire()
            cv_recv.notifyAll()
            cv_recv.release()

            # for packet in recv_packet:
            #     # print (packet)
            #     if packet != 'ID:':
            #
            #     else:
            #         break
            #     cv_recv.acquire()
            #     cv_recv.notifyAll()
            #     cv_recv.release()
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
            print("send mode",mode[0])
            sock.send(str(mode[0]).encode())

def ReinForceThread(cv_send,cv_recv):
    global mode, recv_packet
    global Hungry_Para, Tired_Para,Touch_Sensor,Force_Sensor,Lift_Sensor,Oled_State,Fin_State,Tail_State,Face_Detect,Reward,STT_Mode
    while True:
        # Receive Mutex
        # cv_recv.acquire()
        # cv_recv.wait()
        # cv_recv.release()
        # Hungry_Para     = recv_packet[1]
        # Tired_Para      = recv_packet[2]
        # Touch_Sensor    = recv_packet[3]
        # Force_Sensor    = recv_packet[4]
        # Lift_Sensor     = recv_packet[5]
        # Oled_State      = recv_packet[6]
        # Fin_State       = recv_packet[7]
        # Tail_State      = recv_packet[8]
        # Face_Detect     = recv_packet[10]
        # Reward          = recv_packet[11]
        # STT_Mode     = recv_packet[12]
        # # Input
        # # Mode Control
        # if mode == 6:
        #     mode = 17
        # else :
        #     mode = 6
        # # Receive Mutex
        # cv_send.acquire()
        # cv_send.notifyAll()
        # cv_send.release()
        env = ComebotEnv(cv_send, cv_recv, mode, recv_packet)  # 코미봇환경 불러오기

        num_actions = env.action_space.n  # action 20가지
        state_space = env.observation_space.shape[0]  # state 10개 parameter
        print(num_actions, state_space)

        model = build_model(state_space, num_actions)

        memory = SequentialMemory(limit=10000, window_length=1)

        policy = LinearAnnealedPolicy(EpsGreedyQPolicy(), attr='eps',
                                      value_max=1., value_min=.1, value_test=.1,
                                      nb_steps=10000)

        dqn = DQNAgent(model=model, nb_actions=num_actions,
                       memory=memory, nb_steps_warmup=10,
                       target_model_update=1e-7, policy=policy)
        # target_model_update 1e-2 --> 1e-7로 수정
        dqn.compile(Adam(lr=0.1), metrics=['categorical_crossentropy'])
        # lr 0.01-->0.1로 수정 학습step수가 감소함에 따라
        callbacks = build_callbacks('comebot')
        # 학습step수 1000단위로 학습되가는지 확인해보고 늘리기
        hist = dqn.fit(env, nb_steps=10000,
                       visualize=False,
                       verbose=2,
                       callbacks=callbacks)

        dqn.save_weights('dqn_{}_weights.h5f'.format('comebot'), overwrite=True)

        dqn.test(env, nb_episodes=5, visualize=False)

        # print(hist.history['metrics'])
        import matplotlib.pyplot as plt

        plt.figure(1)
        plt.plot(hist.history['metrics'])
        plt.xlabel('episode')
        plt.ylabel('loss(blue)')
        # hist.history['metrics'] = [loss(파랑),categorical_crossentropy(노랑),mean_q(초록),mean_eps(빨강)]
        plt.figure(2)
        plt.plot(hist.history['episode_reward'])
        plt.xlabel('episode')
        plt.ylabel('episode reward')
        plt.show()

def TcpThread():
    # global recv_packet,mode
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((HOST, PORT))
        # Thread Init
        thread_recv = Thread(target=recvMsg, args=(sock,condition_recv))
        thread_send = Thread(target=sendMsg, args=(sock,condition_send))
        thread_rein = Thread(target=ReinForceThread, args=(condition_send, condition_recv))
        # thread_rein = Thread(target=reinforce, args=(condition_send, condition_recv,recv_packet,mode))
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
