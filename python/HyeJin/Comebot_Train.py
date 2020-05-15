'''
버전정보
python 3.6
tensorflow 1.13.1
keras 2.3.1
keras-rl
---------------------
pip install keras-rl
pip install wandb
'''

from Comebot_Env import ComebotEnv
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout
from keras.optimizers import Adam
from rl.agents.dqn import DQNAgent
from rl.policy import LinearAnnealedPolicy, EpsGreedyQPolicy
from rl.memory import SequentialMemory
from rl.callbacks import ModelIntervalCheckpoint, FileLogger

# 신경망 만드는 부분
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


def main():
    env = ComebotEnv()  # 코미봇환경 불러오기

    num_actions = env.action_space.n    # action 20가지
    state_space = env.observation_space.shape[0]    # state 10개 parameter
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

if __name__ == '__main__':
    main()
