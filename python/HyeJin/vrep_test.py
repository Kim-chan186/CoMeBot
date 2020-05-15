import random

O_LED = 0       # OLED 표정
#LED = 0         # LED 색상
TAIL = 0        # 꼬리 움직임 정도
FIN = 0         # 지느러미 움직임 정도
HUNGER = 0      # 배고픔 정도
FATIGUE = 0     # 피로함 정도

TOUCH = 0       # 터치센서 감지부위 및 유무
FORCE = 0    # 가속도센서 변화로 인한 움직임 종류
FACE = 0        # 얼굴감지 유무
WORD = 0        # 지정단어 입력 유무 및 단어 종류

cnt = 0
pre_state = [0, 0, 0, 0]
x = 6

def Vrep_state(action):
    global pre_state

    pre_hung = pre_state[0]
    pre_fati = pre_state[1]
    pre_touch = pre_state[2]
    pre_move = pre_state[3]

    if pre_touch == 0:
        if pre_move == 0:
            if (pre_hung < 3 & pre_fati < 3) or pre_hung < 1 or pre_fati < 1:
                if(action>=13 & action <17):    # ㅠㅠ
                    reward = 10
                elif(action > 3 & action < 9):  # ><,^^
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung > 3 & pre_fati > 3) or pre_hung > 4 or pre_fati > 4:
                if(action<4):   # ..
                    reward = 10
                elif(action>16):    #\/(2)
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung < 4 or pre_fati < 4):
                if action>=13 & action <17:   # ㅠㅠ
                    reward = 5
                else:
                    reward = 0
            else:
                reward = 0
        else:
            if (pre_hung < 3 & pre_fati < 3) or pre_hung < 1 or pre_fati < 1:
                if(action>16):  # \/(1)
                    reward = 10
                elif(action <9): # ..,><,^^
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung > 3 & pre_fati > 3) or pre_hung > 4 or pre_fati > 4:
                if(action>8 & action<13):   # --(1)
                    reward = 10
                elif(action>16):    # \/
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung < 4 or pre_fati < 4):
                if action>16: #\/(2)
                    reward = 5
                else:
                    reward = 0
            else:
                reward = 0
    else:
        if pre_move == 0:
            if (pre_hung < 3 & pre_fati < 3) or pre_hung < 1 or pre_fati < 1:
                if(action>8 & action<13):  # --(2)
                    reward = 10
                elif(action<9): # ..,><,^^
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung > 3 & pre_fati > 3) or pre_hung > 4 or pre_fati > 4:
                if(action>5 & action <9):   # ^^
                    reward = 10
                elif(action>10 & action <18):    # --,ㅠㅠ,\/
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung < 4 or pre_fati < 4):
                if action>8 & action <13:   # --
                    reward = 5
                else:
                    reward = 0
            else:
                reward = 0
        else:
            if (pre_hung < 3 & pre_fati < 3) or pre_hung < 1 or pre_fati < 1:
                if(action>16):  # \/
                    reward = 10
                elif(action<9): # ..,><,^^
                    reward = -10
                else:
                    reward = 0
            elif (pre_hung > 3 & pre_fati > 3) or pre_hung > 4 or pre_fati > 4:
                if(action>3 & action<6):    # ><
                    reward = 10
                elif(action>18 or action<4):    #\/,..
                    reward = -3
                else:
                    reward = 0
            elif (pre_hung < 4 or pre_fati < 4):
                if action>16:   #\/(1)
                    reward = 5
                else:
                    reward = 0
            else:
                reward = 0

    if action == 19:
        O_LED = 4
        # LED = random.randrange(0, 3)
        TAIL = 2
        FIN = 1
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 18:
        O_LED = 4
        # LED = random.randrange(0, 3)
        TAIL = 2
        FIN = 2
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 17:
        O_LED = 4
        # LED = random.randrange(0, 3)
        TAIL = 3
        FIN = 1
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 16:
        O_LED = 3
        # LED = random.randrange(0, 3)
        TAIL = 0
        FIN = 0
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 15:
        O_LED = 3
        # LED = random.randrange(0, 3)
        TAIL = 0
        FIN = 3
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 14:
        O_LED = 3
        # LED = random.randrange(0, 3)
        TAIL = 1
        FIN = 3
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 13:
        O_LED = 3
        # LED = random.randrange(0, 3)
        TAIL = 2
        FIN = 3
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 12:
        O_LED = 2
        # LED = random.randrange(0, 3)
        TAIL = 1
        FIN = 2
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 11:
        O_LED = 2
        # LED = random.randrange(0, 3)
        TAIL = 2
        FIN = 1
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 10:
        O_LED = 2
        # LED = random.randrange(0, 3)
        TAIL = 2
        FIN = 2
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 9:
        O_LED = 2
        # LED = random.randrange(0, 3)
        TAIL = 3
        FIN = 2
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 8:
        O_LED = 1
        # LED = random.randrange(0, 3)
        TAIL = 0
        FIN = 0
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 7:
        O_LED = 1
        # LED = random.randrange(0, 3)
        TAIL = 0
        FIN = 1
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 6:
        O_LED = 1
        # LED = random.randrange(0, 3)
        TAIL = 3
        FIN = 1
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 5:
        O_LED = 5
        # LED = random.randrange(0, 3)
        TAIL = 1
        FIN = 0
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 4:
        O_LED = 5
        # LED = random.randrange(0, 3)
        TAIL = 1
        FIN = 3
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 3:
        O_LED = 0
        # LED = random.randrange(0, 3)
        TAIL = 0
        FIN = 2
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 2:
        O_LED = 0
        # LED = random.randrange(0, 3)
        TAIL = 2
        FIN = 0
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    elif action == 1:
        O_LED = 0
        # LED = random.randrange(0, 3)
        TAIL = 3
        FIN = 0
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    else:
        O_LED = 0
        # LED = random.randrange(0, 3)
        TAIL = 1
        FIN = 1
        HUNGER = random.randrange(0, 6)
        FATIGUE = random.randrange(0, 6)
        TOUCH = random.randrange(0, 2)
        FORCE = random.randrange(0, 2)
        FACE = 0
        WORD = 0
        #reward = 1
    #reward = random.randrange(-1,1)
    #reward = input('reward:')
    # global x
    # HUNGER = x
    # FATIGUE = x
    print('pre:', pre_state, 'action:', action, 'reward:', reward, 'state:',[HUNGER,FATIGUE,TOUCH,FORCE])
    global cnt
    if cnt == 19:
        pre_state=[0, 0, 0, 0]
        # if x == 0:
        #     x = 6
        # else:
        #     x -= 1
        cnt = 0
    else:
        pre_state=[HUNGER,FATIGUE,TOUCH,FORCE]
    # print(cnt)
    cnt += 1
    #pre_state = [HUNGER, FATIGUE, TOUCH, FORCE]

    #print('state, reward 전송완료')
    #return [HUNGER, FATIGUE, TOUCH, FORCE, O_LED, FIN, TAIL, FACE, WORD, reward]
    return [HUNGER, FATIGUE, TOUCH, FORCE, 0, O_LED, 0, 0, FACE, WORD, reward]
