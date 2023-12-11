#!/usr/bin/env python
# coding: utf-8

# In[5]:


import pandas as pd              # 데이터 조작을 위한 라이브러리
from konlpy.tag import Kkma     # 한국어 텍스트 분석을 위한 라이브러리
import random                   # 난수 생성을 위한 라이브러리  
from PIL import Image          # 이미지 처리를 위한 라이브러리
import PIL.Image          
import matplotlib.pyplot as plt # 그래프 작성을 위한 라이브러리
from collections import Counter # 항목의 개수를 세는데 사용되는 라이브러리
import socket                   # 네트워크 소켓 통신을 위한 라이브러리

def PosNeg(Pos, Neg):
    if Pos > Neg:
        return Pos
    elif Pos == Neg:
        return 0  # 알고리즘 3번
    else:
        return -Neg


# 이미지 데이터에 대한 가중치 설정
def Image_emotion(x):
    if x == 0:  # 1번 이미지
        return -0.275
    elif x == 1:  # 2번 이미지
        return 0.467
    elif x == 2:  # 3번 이미지
        return 0.491
    elif x == 3:  # 4번 이미지
        return 0.675
    elif x == 4:  # 5번 이미지
        return 0.708
    elif x == 5:  # 6번 이미지
        return 0.366
    elif x == 6:  # 7번 이미지
        return 0.458
    elif x == 7:  # 8번 이미지
        return 0.725
    elif x == 8:  # 9번 이미지
        return -0.733
    elif x == 9:  # 10번 이미지
        return -0.783
    elif x == 10:  # 11번 이미지
        return -0.55
    elif x == 11:  # 12번 이미지
        return -0.352
    else:  # 13번 이미지
        return -0.117

# 분석결과로 감정 추출
def Emotion(x, y):
    # 1사분면
    if x > 0 and y > 0:
        if x == y:
            return "기대감\즐거움"
        elif x > y:
            return "기대감"
        else:
            return "즐거움"
    # 4사분면
    if x > 0 and y < 0:
        if x == y:
            return "분노\혐오"
        elif x > abs(y):
            return "분노"
        else:
            return "혐오"
    # 2사분면
    if x < 0 and y > 0:
        if x == y:
            return "공포\신뢰"
        elif abs(x) > y:
            return "공포"
        else:
            return "신뢰"
    # 3사분면
    if x < 0 and y < 0:
        if x == y:
            return "놀라움\슬픔"
        elif abs(x) > abs(y):
            return "놀라움"
        else:
            return "슬픔"
    # y가 0일 때
    if y == 0:
        if x > 0:
            return "기대감\분노"
        else:
            return "공포\놀라움"


# 메인서버에서 받아온 데이터를 바탕으로 사용자 감정 분석
def ai(list_msg):
    df = pd.read_csv("C:/Users/SUNGRO/PycharmProjects/pythonProject/test/kkma.csv", encoding='cp949')

    res_x = []
    res_y = []
    res_emotion = []

    kkma = Kkma()
    for i in range(1, 5):
        image_emotion = Image_emotion(int(list_msg[2 * i]))

        Sentence = list_msg[2 * i + 1]

        kkma_S = kkma.morphs(Sentence)
        print(kkma_S)
        Answer = kkma.pos(Sentence)
        print(Answer)

        va_words = []
        for word, pos in Answer:
            if pos == 'VA':  # 형용사
                va_words.append(word)

        if va_words:
            print("형용사 : ", ', '.join(va_words))
        else:
            print("형용사 없음")

        rows = []

        if va_words == []:
            result_emotion = 0  # 형용사가 없다면, 최종감정을 중립으로 설정
        else:
            rows = df[df['형태소'].isin(va_words).copy()]  # va_words 안의 형용사를 df랑 비교해서 rows 생성
            if rows.empty == True:
                result_emotion = 0  # 예외처리, rows가 비어있다면 최종감정을 중립으로 설정
            else:
                counts = Counter(va_words)
                rows = rows.reset_index(drop=True)
                for j in range(len(rows)):
                    rows.loc[j, 'count'] = counts[rows.loc[j, "형태소"]]
                max_count = rows['count'].max()
                result_va = rows[rows['count'] == max_count]
                Pos = float(result_va['PosScore'].max())
                Neg = float(result_va['NegScore'].max())
                result_emotion = PosNeg(Pos, Neg)
        result = Emotion(image_emotion, result_emotion)
        print(f"x축의 값 : {image_emotion}, y축의 값 : {result_emotion}")
        print(f"당신의 현재 감정은 '{result}' 입니다.")
        res_x.append(image_emotion)
        res_y.append(result_emotion)
        res_emotion.append(result)
    return res_x, res_y, res_emotion


#서버 설정
server_ip = '0.0.0.0'  # 파이썬 서버의 IP 주소 또는 호스트 이름
server_port = 8080  # 클라이언트와 동일한 포트 번호
#소켓 생성
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#서버 소켓을 IP 주소와 포트에 바인딩
server_socket.bind((server_ip, server_port))

#연결 대기
server_socket.listen(1)

print(f"서버 대기 중 ({server_ip}:{server_port})")
while True:
    # 클라이언트 연결 수락
    client_socket, client_address = server_socket.accept()
    print(f"클라이언트 연결 수락: {client_address}")
    while True:
        # 텍스트 수신
        message = client_socket.recv(1024)
        recv_msg = message.decode()
        list_msg = recv_msg.split("●")


        user_id = list_msg[0]

        # client_socket.send("메세지 받음!!".encode())

        result_x, result_y, result_emotion = ai(list_msg)
        print(result_x)
        print(result_y)
        print(result_emotion)
        final_x = 0
        final_y = 0
        final_result_x = 0
        final_result_y = 0
        for i in range(4):
            final_x += result_x[i]
            final_y += result_y[i]
        final_result_x = final_x / 4                 # 최종감정 도출을 위해 x축 평균값계산
        rounded_result_x = round(final_result_x, 3)  # 최종 x축 소수점 3자리까지 반올림후 나타내기
        final_result_y = final_y / 4                 # 최종감정 도출을 위해 y축 평균값계산
        rounded_result_y = round(final_result_y, 3)  # 최종 y축 소수점 3자리까지 반올림후 나타내기
        final_emotion = Emotion(final_result_x, final_result_y)
        print(f"최종X: {final_result_x:.3f} 최종Y: {final_result_y:.3f} 최종감정: {final_emotion}")
        img_idx = list_msg[2]+","+list_msg[4]+","+list_msg[6]+","+list_msg[8]
        result_x1 = []
        result_y1 = [] 
        for i in result_x:
            result_x1.append(str(i))
        for i in result_y:
            result_y1.append(str(i))
        
        # 감정 분석 후 결과 전송
        message = "test_result"+"●"+list_msg[1]+"●"+img_idx+"●"+','.join(result_x1)+"●"+','.join(result_y1)+"●"+','.join(result_emotion)+"●"+str(rounded_result_x)+","+str(rounded_result_y)+","+final_emotion+"●"
        client_socket.send(message.encode())

    # 현재 클라이언트와의 연결 종료
    #client_socket.close()
    #print(f"클라이언트 연결 끊김: {client_address}")
    # 다음 클라이언트와의 연결을 대기하기 위해 서버 소켓 닫기
    #server_socket.close()


# In[ ]:




