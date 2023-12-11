#from IPython.display import clear_output
from time import sleep
import random
# won_1 = 충전금액
# take_1 매장이용, 포장, 충전 구분
# take_2 충전 완료후, 포장, 결제 구분
# discount = 할인금액

#충전 금액 입력
won_1 = 0


# 적립금 초기화
save = 0

# 음료를 큰 카테고리(ctg) 8개로 딕셔너리로 해서 함수로 작성
menu_tea = {'복숭아 아이스티' : 3000, '유자 허니티' : 3500, '자몽 허니티' : 3500, '레몬 허니티' : 3500, '애플유자티' : 3500,
            '청글얼그레이' : 2500, '리치캐모마일' : 2500, '트리플민트' : 2500, '애플히비스커스' : 2500}
menu_normal = {'핫 아메리카노' :1500 , '아이스 아메리카노' : 1800 , '핫 카페라떼' : 2700, '아이스 카페라떼' : 2700, '핫 오트카페라떼' : 3500, '아이스 오트카페라떼' : 3500,
               '핫 믹스커피' : 2300, '아이스 믹스커피' : 2300, '아이스 바닐라딥라떼' : 3500, '핫 바닐라딥라떼' : 3500, '아이스 헤이즐넛딥라떼' : 3500, '핫 헤이즐넛딥라떼' : 3500,
               '핫 연유라떼' : 3500, '핫 카라멜마끼아또' : 3500, '아이스 카라멜마끼아또' : 3500, '핫 카페모카' : 3500, '아이스 카페모카' : 3500, '핫 토피넛라떼' : 3700, '아이스 토피넛라떼' : 3700,
               '핫 꿀다크리카노' : 2300, '아이스 꿀다크리카노' : 2300,'핫 바닐라다크리카노' : 2300, '아이스 바닐라다크리카노' : 2300, '핫 헤이즐넛다크리카노' : 2300, '아이스 헤이즐넛다크리카노' : 2300,
               '핫 아인슈페너' : 3500, '아이스 아인슈페너' : 3500}
menu_coldbrew = {'콜드브루' : 3300, '콜드브루 라떼' : 3800, '바닐라크림 콜드브루' : 4100, '헤이즐넛크림 콜드브루' : 4100}
menu_ventisize= {'아메리카노' : 2800, '카페라떼' : 3700, '믹스커피' : 3300, '복숭아 아이스티' : 4000}
menu_beverage = {'딸기라떼' : 3500, '미숫가루' : 2500, '핫 미숫가루 라떼' : 3000, '아이스 미숫가루 라떼' : 3000, '핫 로얄밀크티' : 3500,
                 '아이스 로얄밀크티' : 3500, '핫초코라떼' : 3000, '아이스초코라떼' : 3000, '핫 녹차라떼' : 3300,
                '아이스 녹차라떼' : 3300, '핫 고구마라떼' : 3500, '아이스 고구마라떼' : 3500}
menu_juice = {'포블아' : 4300,'망플단' : 4300, '사딸비' : 4300, '오사당' : 4300, '블루레몬 에이드' : 3900,
              '자몽 에이드' : 3900, '청포도 에이드' : 3900, '애플망고 에이드' : 3900, '크림소다' : 3900, '메론소다' : 3900, '체리콕' : 3900}
menu_blend = {'에스프레소 쉐이크' : 3900, '딸기 쉐이크' : 3900, '밀크 쉐이크' : 3900, '초코 쉐이크' : 3900,  '딸기 요거트스무디 ' : 3900, '플레인 요거트스무디 ' : 3900,
              '블루베리 요거트스무디 ' : 3900, '쿠키앤크림 프라페' : 3900, '코코초코 프라페' : 3900, '코코밀크 프라페' : 3900, '자바칩 프라페' : 3900,
              '초코칩 프라페' : 4200, '토피넛초코칩 프라페' : 4200, '민트초코칩 프라페' : 4200, '딸기초코칩 프라페' : 4200, '말차초코칩 프라페' : 4200}
menu_bub = {'흑설탕버블티' : 3900, '타로버블티' : 3900, '로얄밀크버블티' : 4300, '청포도 요구르탱' : 3900, '애플망고 요구르탱' : 3900, '딸기 요구르탱' : 3900}

# 옵션 3개
# option(wondu_dic)
wondu_dic = {'마일드 원두(2샷)': 0, '다크 원두(2샷)': 0, '디카페인 원두(2샷)': 500,
             '마일드 원두(2샷)+샷추가(2샷)': 500, '다크 원두(2샷)+샷추가(2샷)': 500,
             '디카페인 원두(2샷)+샷추가(2샷)': 1300}
# option(sirub_dic)
sirub_dic = {'달게': 0, '바닐라시럽 추가': 500, '헤이즐넛시럽 추가': 500,
             '흑설탕시럽 추가': 500, '꿀 추가': 500, '연유 추가': 500}
# option(topping_dic)
topping_dic = {'아이스크림 추가': 500, '버블(타피오카펄) 추가': 800, '화이트펄 추가': 800}

# select_ctg() : 입력한 카테고리에 대한 변수로 지정한 값 반환하는 함수
def select_ctg():
    while True:
        ctg = input("""
              ┌──────────────────────────────────┐
              │☕ 음료 카테고리를 선택해주세요  ◀│
          ┌───┴──────────────────────────────────┴───┐
          │   커피, 콜드브루, 벤티사이즈, 베버리지   │
          │ 티, 주스&에이드. 아이스 블렌디드, 버블티 │
          └──────────────────────────────────────────┘
                               """)
        print()
        print()
        print()
        print()
        if ctg == '커피':
            return menu_normal
        elif ctg == '콜드브루':
            return menu_coldbrew
        elif ctg == '벤티사이즈':
            return menu_ventisize
        elif ctg == '베버리지':
            return menu_beverage
        elif ctg == '티':
            return menu_tea
        elif ctg == '주스&에이드':
            return menu_juice
        elif ctg == '아이스 블렌디드':
            return menu_blend
        elif ctg == '버블티':
            return menu_bub
        elif ctg == '0':
            return '0'
        else:
            print("카테고리가 존재하지 않습니다.다시 입력 해주세요")



# 파라미터로 받은 menu 값을 한 줄에 3개씩 출력하는 함수
def show_menu(menu):
    cnt = 0
    print("="*100)
    for drink, price in menu.items():
        cnt += 1
        print(f'{"":<5}{drink} - {price}원', end='\t')
        if (cnt%3==0):
            print()
    print("="*100)

# 입력한 옵션에 대한 딕셔너리를 출력하고
# 선택한 옵션에 대한 가격을 total에 더해서 반환하는 함수
# '0'을 선택하면 option()함수 종료
def option(wondu_dic,total):
    while True :
        show_menu(wondu_dic)
        wondu = input("""
                             ┌────────────────────────────────┐
                             │        옵션을 골라주세요       │
                             │ 0을 누르면 다음으로 이동합니다.│
                             └────────────────────────────────┘
                                      """)
        if wondu in wondu_dic.keys() :
            total += wondu_dic[wondu]
            print("[ 선택하신 메뉴의 가격은 ]", total)
            # list4.append(wondu)
            # list5.append(wondu_dic[wondu])
        elif wondu == '0' :
            print("""
                                     ┌────────────────┐
                                     │ 옵션 추가 완료 │
                                     └────────────────┘
                                          """)
            break
        else :
            print('옵션에 없습니다.')
    return total


# '0'이 아닌 선택한 카테고리를 출력(0이면 choice()함수 종료)
# 선택한 카테고리에 대한 모든 메뉴를 출력 후 입력받는다
# 입력받은 음료에 대한 가격을 초기화된 total값에 더해주고
# 옵션으로 선택한 것의 가격을 total에 더해줘서 total로 반환한다.
# 다 주문받으면 '0'을 입력하여 종료
# 선택한 음료의 개수를 선택하고
# 주문한 음료의 이름과 음료의 가격, 음 개수를 list1,list2,list3에 저장 후
# list1,list2,list3를 하나의 리스트에 담아서 반환
def choice(list1,list2,list3):
    while True :
        menu = select_ctg()
        if menu=='0':
            break
        total = 0
        show_menu(menu)
        order = input("""
                               ┌──────────────────────────────┐
                               │      메뉴를 골라주세요       │
                               │ 0을 누르면 종료할수 있습니다.│
                               └──────────────────────────────┘
                                        """)
        if order in menu.keys() :
            print("메뉴의 가격은 : ",menu[order], "원 입니다.")
            total += menu[order]
            total = option(wondu_dic,total)
            total = option(sirub_dic,total)
            total = option(topping_dic,total)
            list2.append(total)
            count = int(input("""
                              ┌─────────────────────────────┐
                              │ 주문할 개수를 입력해 주세요 │
                              └─────────────────────────────┘
                                             """))
            total += total*(count-1)
            print(f"[ 선택하신 메뉴의 가격은 총 {total}원 입니다 ]")
            list1.append(order)
            list3.append(count)
            print("="*50)
            print("커피명\t\t커피가격\t\t커피개수")
            print("="*50)
            for i in range(len(list1)):
                print(f'{list1[i]}\t{list2[i]}\t\t\t{list3[i]}')
            print("="*20)
            total1=0
            for i in range(len(list1)):
                total1 += list2[i]*list3[i]
            print(f"[ 선택하신 메뉴의 가격은 총 {total1}원 입니다 ]")
        elif order == '0' :
            print("주문이 완료되었습니다.")
            break
        else :
            print('메뉴에 없습니다.')
    return [list1,list2,list3]

# total값을 초기화하고
# list_total 변수에 choice함수 반환값을 대입한다.
# total에 주문한 총 음료의 가격을 더한다.
def show_choice(list1,list2,list3):
    total = 0
    list_total = choice(list1,list2,list3)
    for i in range(len(list_total[0])):
        total += list_total[1][i]*list_total[2][i]
    return list_total,total

# 충전된 잔액이 계산할 금액보다 적을 시 충전하도록 유도하는 함수
def prt2(won_1, total):
    boolean = True
    while boolean:
        if (won_1 >= total):
            boolean = False
        else:
            # bright red로 출력
            print('\033[31m' + '잔액이 부족합니다!!' + '\033[0m')
            print('\033[31m' + f'{abs(won_1 - total)}원을 충전해주세요!!' + '\033[0m')
            won_2 = int(input("충전할 금액을 입력해주세요!! : "))
            won_1 += won_2
            print(f"{won_2}원이 입금되었습니다. 현재 잔액은 {won_1}입니다.")
    return won_1

# 최종 시작하는 함수
def starts(won_1, save):
    while True:
        # 영수증에 출력하기 위한 리스트 3개
        # list1 : 커피 이름, list2 : 커피가격, list3 : 커피가격
        take_2 = 0
        list1=[]
        list2=[]
        list3=[]
        print("""
                    ┌─────────────────────────────────┐
                    │  T   H   E   V   E   N   T   I  │
                    └───────┬─────────────────┬───────┘
                            │COFFEE & BEVERAGE│
                            └─────────────────┘
                """)

        take_1 = int(input("""
            ┌─────────────┐   ┌──────────────────┐   ┌─────────────┐
            │ 매 장 이 용 │   │  테 이 크 아 웃  │   │ 요 금 충 전 │
            └─────────────┘   └──────────────────┘   └─────────────┘

            [ 매장이용시 [1] , 테이크아웃시 [2] 요금 충전 [3] ]
                                    """))
        while take_1 > 3 or take_1 <= 0:
            take_1 = int(input("""
            ┌─────────────┐   ┌──────────────────┐   ┌─────────────┐
            │ 매 장 이 용 │   │  테 이 크 아 웃  │   │ 요 금 충 전 │
            └─────────────┘   └──────────────────┘   └─────────────┘
                    [선택 번호 오류 다시 입력해주세요]
                [ (MIN)1 ~ (MAX)3 까지의 숫자를 입력해주세요]
            [ 매장이용시 [1] , 테이크아웃시 [2] 요금 충전 [3] ]
                                    """))

        if take_1 == 3:
            won_1 += int(input("""
                        ┌──────────────────────────┐
                        │ 충전 금액을 입력해주세요 │
                        └──────────────────────────┘
                                    """))
            while won_1 < 5000:
                print("최소 금액 5000원이상을 충전해주세요")
                won_1 += int(input("[ 충전 금액을 입력해주세요 ] : "))
            print("""
                        [ 잠시만 기다려주세요 ] """)
            print()
            sleep(1.5) # 추가됨 1.5초 딜레이
            print(f"""
            ┌──────────────────────────────────────────────────┐
            │ 충전이 완료되었습니다 현재 잔액은 {won_1}원 입니다. │
            └──────────────────────────────────────────────────┘
                """)

            take_2 = int(input("""
                ┌──────────────────┐      ┌──────────────────┐
                │   매 장  이 용   │      │  테 이 크 아 웃  │
                └──────────────────┘      └──────────────────┘
                    [ 매장이용시 [1] , 테이크아웃시 [2] ]
                                        """))
            while take_2 > 2 or take_2 <= 0:
                    take_2 = int(input("""
                ┌──────────────────┐      ┌──────────────────┐
                │   매 장  이 용   │      │  테 이 크 아 웃  │
                └──────────────────┘      └──────────────────┘
                    [선택 번호 오류 다시 입력해주세요]
                [ (MIN)1 ~ (MAX)2 까지의 숫자를 입력해주세요]
                    [ 매장이용시 [1] , 테이크아웃시 [2] ]
                                        """))



        list_total, total = show_choice(list1,list2,list3)

        # 정가 결제금액 계산
        discount = 0
        if take_1 == 1 or take_2 == 1: # 매장이용
            pass
        # 할인된 결제금액 계산
        elif take_1 == 2 or take_2 == 2: # 테이크아웃
            discount = int(total*0.15) # 수정필요
            total -= discount
        final = prt2(won_1, total)
        print(f"[ 결제금액은 {total}원 입니다.]")


        # 최종 잔액
        final -= total

        # 적립할 금액 save
        save += total * 0.02

        print(f'잔액은 {final}원 이며, 적립 금액은 {save}원 입니다.')
        print()

        # 영수증 출력
        num_1 = random.sample(list(range(1, 90)), 1)

        print("┌────────────────────────────────────────────────────────┐")
        print("│                        영 수 증                        │")
        print("│ 점포명[더 벤티 광주 인력개발원 점]                     │")
        print(f"│ 영수증 번호{num_1}                                        │")
        print("├────────────────────────────────────────────────────────┤")
        for i in range(len(list1)):
            print(f'│{"":<5}{list1[i]}{"":<5}{list2[i]}{"":<5}{list3[i]}                        │')
        print("│                                                        │")
        print("│                                                        │")
        print("│                                                        │")
        print("│                                                        │")
        print("│                                                        │")
        print("│                                                        │")
        print("├────────────────────────────────────────────────────────┤")
        print(f"                                  할인금액 : {discount}원")
        print(f"                                  결제금액 :    {total}원")
        print(f"                                  남은잔액 :    {final}원")
        print(f"                                  적립금액 :     {int(save)}원")
        sleep(3)

starts(won_1, save)