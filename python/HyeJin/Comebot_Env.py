import gym
from gym import spaces
from vrep_test import Vrep_state
import ReinforcePython

# state_feature = 10 ############################

Actions = 20    # action 가짓수
#Spaces = [4, 3, 3, 5, 5, 3, 2, 2, 10]
Spaces = [5, 5, 3, 2, 2, 4, 3, 3, 2, 10]    # state종류와 각 종류별 가짓수
# Hungry_Para   = 0
# Tired_Para    = 0
# Touch_Sensor  = 0
# Force_Sensor  = 0
# Lift_Sensor   = 0
# Oled_State    = 0
# Fin_State     = 0
# Tail_State    = 0
# Face_Detect   = 0
#####단어도 추가해야함

Good_Reward = 100   #  1
No_Reward   = -0.5  #  0
Bad_Reward  = -30   # -1

rewards= [100, -0.5, -30]

# 1step : -1,total(100) : 0~4, addtion : 30
HUNGER = 0      # 포만감 정도

# 1episode : -10,total(100) : 0~4, addtion : 30
FATIGUE = 0     # 체력 정도

# 머리, 꼬리
TOUCH = 0       # 터치센서 감지부위 및 유무

# 유무 0,1
FORCE = 0    # 충격 유무(충격 센서)

# 유무 0,1
LIFT = 0    # 코미봇이 들어올려졌는지 아닌지

# 기본, 행복, 신남, 슬픔, 화남, 지루
O_LED = 0       # OLED 표정

# 앞뒤, 날개짓, 내림, 천천히 뒤로
FIN = 0         # 지느러미 움직임 정도

# 신나게 흔들기, 천천히, 흔들기, 내림, 올림
TAIL = 0        # 꼬리 움직임 정도

# 유무 0,1
FACE = 0        # 얼굴감지 유무

# STT 20
WORD = 0        # 지정단어 입력 유무 및 단어 종류
# {
#     단어 미정
# }

################################################

class ComebotEnv(gym.Env):      #gym.Env 상속

    def __init__(self):
        self.action_space = spaces.Discrete(Actions)    # action 갯수만큼 불연속적인 값들로 이루어진 action_space 생성
        self.observation_space = spaces.MultiDiscrete(Spaces)   # 각 state별 상태갯수만큼 불연속적인 값들로 이루어진 observation_space 생성

    def reset(self):
        # 새로운 episode가 시작됨에 따라 환경 초기화하는 부분
        print('----reset----')
        self.done = False
        self.cnt = 0
        self.reward = 0
        return [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # 초기 observation(=state)반환

    def step(self, action):
        # action을 수행하고 변화된 state, reward, episode 종료여부 등을 반환하는 부분
        print('----step----')

        # print('v-rep에 action {} 보내기'.format(action))
        # 통신하는 동안 별다른일을 하지 않도록 구상 : 행동을 할수 있는 자료가 없음
        get_obs = Vrep_state(action)
        print(get_obs)
        print("포만감:",get_obs[0],"체력:",get_obs[1],"터치:",get_obs[2],"충격:",get_obs[3])
        obs = get_obs[:10]
        reward = get_obs[10]
        # reward = rewards[ get_obs[9] ] # -1, 0, 1
        # reward = self._get_reward(reward)

        # 20step이 1episode가 되도록 설정함, 추후에 변경가능
        self.cnt += 1
        self.done = self.cnt > 20

        return obs, reward, self.done, {}   # state, reward, episode종료여부 반환

    # def _get_reward(self, reward):
    #     if reward == 'o':
    #         #print('Good')
    #         return Good_Reward
    #     elif reward == 'x':
    #         #print('Bad')
    #         return Bad_Reward
    #     else:
    #         #print('None')
    #         return No_Reward
