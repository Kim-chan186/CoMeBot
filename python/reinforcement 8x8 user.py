# Q러닝 문제 사용자가 풀수 있도록

import sys
import tensorflow as tf
import os
import gym
import numpy as np
import matplotlib.pyplot as plt
import msvcrt  # winsow os
import cv2 as cv
from gym.envs.registration import register
from gym.envs.toy_text import frozen_lake as frozen

import readchar

# https://c548adc0c815.gitbooks.io/javacafe-ml/open-ai-gym-ex.html
# 체육관 패키지를 가져오고 OpenAI GYM에서 제공하는 환경 중점 FrozenLake-v0 환경을 생성했습니다.

env = gym.make("FrozenLake-v0")
observation = env.reset()


'''
try:
    # Windows용 코드
    import msvcrt
    def getkey():
        """단일키 누르는 것을 받아옴"""
        return msvcrt.getch()

    def getkey():
        """단일키 누르는 것을 받아옴"""
        return msvcrt.getch()

except ImportError:
    # Linux & Mac 용 코드
    import sys
    import tty
    import termios


    def getkey():
        """단일키 누르는 것을 받아옴"""
        fd = sys.stdin.fileno()
        original_attributes = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, original_attributes)
        return ch
'''


class _Getch:
    def __call__(self):
        """단일키 누르는 것을 받아옴"""
        print("bb")
        return msvcrt.getch()

# 현재 환경을 스크린으로 출력합니다.
# 환경에 따라 행동이 정해야합니다. 이 부분에 알고리즘이 있습니다.
# 적절한 정한 조치를 취합니다. 배상, 보상, 완료, 정보를 얻습니다.


# Python에서 입력 된 키를 입력하십시오.
inkey = _Getch()

# MACROS
LEFT = 0
DOWN = 1
RIGHT = 2
UP = 3

# Key mapping
arrow_keys = {
    2490368 :  UP,
    2621440 :  DOWN,
    2555904 :  RIGHT,
    2424832 :  LEFT,
    '\x1b[A': UP,
    '\x1b[B': DOWN,
    '\x1b[C': RIGHT,
    '\x1b[D': LEFT
}

frozen.MAPS = dict(mymap=frozen.generate_random_map(size=16, p=0.8))

register(
    id='FrozenLake-v3',
    entry_point='gym.envs.toy_text:FrozenLakeEnv',
    kwargs={'map_name' : 'mymap', 'is_slippery': False}
)

env = gym.make('FrozenLake-v3')
env.render() # Show the initial board
cv.imshow("button", np.ones((250, 250), np.uint8))

while True:
    '''
    key = inkey()
    if key not in arrow_keys.keys():
        print("Game aborted!")
        break
    '''

    # choese an action from keyboard
    key = cv.waitKeyEx(1)

    if key == -1:
        continue

    elif key not in arrow_keys.keys():
        print("Game aborted!")
        break

    # key = readchar.readkey()  # 키보드 입력을 받는다
    #
    # if key not in arrow_keys.keys():
    #     print("Game aborted!")
    #     break

    action = arrow_keys[key]
    state, reward, done, info = env.step(action)
    env.render() # show the board after action
    print("State :", state, "Action: ", action, "Reward: ", reward, "info: ", info)

    if done:
        print("Finished with reward", reward)
        break