import math
from tkinter import *
from tkinter import messagebox
from tkinter.filedialog import *
from tkinter.simpledialog import *
import os.path
import matplotlib.pyplot as plt

### 함수부

###################################################
## 공통 함수부
#################################################
def malloc2D(h, w):
    memory = [[0 for _ in range(w)] for _ in range(h)]
    return memory


def openImage():
    global window, canvas, paper, fullname
    global inImage, outImage, mopImage, inH, inW, outH, outW, mopH, mopW
    fullname = askopenfilename(parent=window, filetypes=(('RAW파일', '*.raw'), ('모든파일', '*.*')))
    # 중요! 입력 이미지 크기를 결정
    fsize = os.path.getsize(fullname)  # 파일 크기(Byte)
    inH = inW = int(math.sqrt(fsize))
    # 메모리 할당
    inImage = malloc2D(inH, inW)
    # 파일 --> 메모리
    rfp = open(fullname, 'rb')
    for i in range(inH):
        for k in range(inW):
            inImage[i][k] = ord(rfp.read(1))
    rfp.close()
    equalImage()


def mopingopen():
    global window, canvas, paper, fullname
    global inImage, outImage, mopImage, inH, inW, outH, outW, mopH, mopW
    mopname = askopenfilename(parent=window, filetypes=(('RAW파일', '*.raw'), ('모든파일', '*.*')))
    # 중요! 입력 이미지 크기를 결정
    fsize = os.path.getsize(mopname)  # 파일 크기(Byte)
    mopH = mopW = int(math.sqrt(fsize))
    # 메모리 할당
    mopImage = malloc2D(mopH, mopW)
    # 파일 --> 메모리
    rfp = open(mopname, 'rb')
    for i in range(mopH):
        for k in range(mopW):
            mopImage[i][k] = ord(rfp.read(1))
    rfp.close()


def saveImage() :
    global window, canvas, paper, fullname
    global inImage, outImage, inH, inW, outH, outW
    if (outImage == None or len(outImage)==0) :
        return
    wfp = asksaveasfile(parent=window, mode='wb', defaultextension='*.raw',filetypes=(('RAW파일', '*.raw'), ('모든파일', '*.*')))
    import struct
    for i in range(outH) :
        for k in range(outW) :
            wfp.write( struct.pack('B', outImage[i][k]) )
    wfp.close()
    messagebox.showinfo('성공', wfp.name + ' 저장완료!')

def displayImage():
    global window, canvas, paper, fullname
    global inImage, outImage, mopImage, inH, inW, outH, outW, mopH, mopW
    ## 기존에 이미지를 오픈한적이 있으면, 캔버스 뜯어내기
    if (canvas != None):
        canvas.destroy()
    ## 벽, 캔버스, 종이 설정
    window.geometry(str(outH) + 'x' + str(outW))  # "512x512"
    canvas = Canvas(window, height=outH, width=outW, bg='yellow')  # 칠판
    paper = PhotoImage(height=outH, width=outW)  # 종이
    canvas.create_image((outH / 2, outW / 2), image=paper, state='normal')
    ## 메모리 --> 화면
    # for i in range(outH):
    #     for k in range(outW):
    #         r = g = b = outImage[i][k]
    #         paper.put('#%02x%02x%02x' % (r, g, b), (k, i))
    # 더블 버퍼링... 비슷한 기법 (모두다 메모리상에 출력형태로 생성한 후에, 한방에 출력)
    rgbString = ""  # 전체에 대한 16진수 문자열
    for i in range(outH):
        oneString = ""  # 한줄에 대한 16진수 문자열
        for k in range(outW):
            r = g = b = outImage[i][k]
            oneString += '#%02x%02x%02x ' % (r, g, b)
        rgbString += '{' + oneString + '} '
    paper.put(rgbString)
    canvas.pack()
    plt.clf()
    plt.hist(outImage)
    plt.xlim([0, 256])
    plt.show()



## 퀵정렬 함수
def quickSort(arr1):
    if len(arr1) <= 1:
        return arr1
    pivot = arr1[int(len(arr1) / 2)]
    lesser_arr1, equal_arr1, greater_arr1 = [], [], []
    for num in arr1:
        if num < pivot:
            lesser_arr1.append(num)
        elif num > pivot:
            greater_arr1.append(num)
        else:
            equal_arr1.append(num)
    return quickSort(lesser_arr1) + equal_arr1 + quickSort(greater_arr1)


###################################################
## 영상처리 함수부
#################################################

def equalImage():  # 동일 이미지
    global window, canvas, paper, fullname
    global inImage, outImage, mopImage, inH, inW, outH, outW, mopH, mopW
    # 중요! 출력 영상 크기 결정 --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 메모리 할당
    outImage = malloc2D(outH, outW)
    #### 진짜 영상처리 알고리즘 ###
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = inImage[i][k]
    ############################
    displayImage()


## 이미지 덧셈
def addImage():  # 밝게/어둡게 이미지
    global window, canvas, paper, fullname
    global inImage, outImage, mopImage, inH, inW, outH, outW, mopH, mopW
    # 중요! 출력 영상 크기 결정 --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 메모리 할당
    outImage = malloc2D(outH, outW)
    #### 진짜 영상처리 알고리즘 ###
    value = askinteger('정수입력', '-255~255 입력', maxvalue=255, minvalue=-255)
    for i in range(inH):
        for k in range(inW):
            px = inImage[i][k] + value
            if (px > 255):
                px = 255
            if (px < 0):
                px = 0
            outImage[i][k] = px
    ############################
    displayImage()


## 영상 곱셈 처리(스트래치)
def mulImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    value = askinteger('정수입력', '-255~255 입력')
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] * value > 255:
                outImage[i][k] = 255
            elif inImage[i][k] * value < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = inImage[i][k] * value

    displayImage()


## 영상 나눗셈 처리
def divImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    value = askinteger('정수입력', '-255~255 입력')
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] / value > 255:
                outImage[i][k] = 255
            elif inImage[i][k] / value < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(inImage[i][k] / value)

    displayImage()


## 영상 반전 처리
def reverseImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if 255 - inImage[i][k] > 255:
                outImage[i][k] = 255
            elif 255 - inImage[i][k] < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = 255 - inImage[i][k]

    displayImage()


## 영상 이진화 처리(128)
def bwImage128():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] >= 128:
                outImage[i][k] = 255
            else:
                outImage[i][k] = 0

    displayImage()


## 영상 이진화 처리(평균값)
def bwImagemed():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    sumVal = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            sumVal += inImage[i][k]
    med = sumVal / (inH * inW)
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] >= med:
                outImage[i][k] = 255
            else:
                outImage[i][k] = 0

    displayImage()


## 영상 이진화 처리(중앙값)
def bwImagemiddle():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    sumVal = 0
    arr1 = []
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            arr1.append(inImage[i][k])
    quickSort(arr1)
    med = arr1[int(inH * inW / 2)]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] >= med:
                outImage[i][k] = 255
            else:
                outImage[i][k] = 0

    displayImage()


## 영상 포스터라이징 처리
def posterImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    level = askinteger("경계 갯수","경계 갯수를 입력하세요(최대:255)",maxvalue=255)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            for m in range(level):
                if (inImage[i][k] - ((255 / level) * m) <= 0):
                    if (m == 1):
                        outImage[i][k] = 0
                    else:
                        if ((255 / level) * (m)) > 255:
                            outImage[i][k] = 255
                        elif ((255 / level) * (m)) < 0:
                            outImage[i][k] = 0
                        else:
                            outImage[i][k] = int((255 / level) * (m))
                        break
    displayImage()


## 영상 AND 처리
def andImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    value = askinteger('정수입력', '-255~255 입력', maxvalue=255, minvalue=-255)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] & value > 255:
                outImage[i][k] = 255
            elif inImage[i][k] & value < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = inImage[i][k] & value

    displayImage()


## 영상 곱셈 처리(스트래치)
def orImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    value = askinteger('정수입력', '-255~255 입력', maxvalue=255, minvalue=-255)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] | value > 255:
                outImage[i][k] = 255
            elif inImage[i][k] | value < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = inImage[i][k] | value

    displayImage()


## 영상 xor 처리(스트래치)
def xorImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    value = askinteger('정수입력', '-255~255 입력', maxvalue=255, minvalue=-255)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if inImage[i][k] ^ value > 255:
                outImage[i][k] = 255
            elif inImage[i][k] ^ value < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = inImage[i][k] ^ value

    displayImage()


## 영상 범위강조 처리
def impressImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    value_st = askinteger('정수입력', '0~255 입력', maxvalue=255, minvalue=0)
    value_ed = askinteger('정수입력', '0~255 입력', maxvalue=255, minvalue=0)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if (inImage[i][k] >= value_st and inImage[i][k] <= value_ed):
                outImage[i][k] = 255
            else:
                outImage[i][k] = inImage[i][k]
    displayImage()


## 영상 범위강조 처리
def gamImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    gam = askfloat("감마값","감마값을 입력해주십시오(0.2~1.8사이)", maxvalue=1.8, minvalue=0.2)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if 255.0 * (inImage[i][k] / 255.0) ** gam > 255:
                outImage[i][k] = 255
            elif 255.0 * (inImage[i][k] / 255.0) ** gam < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(255.0 * (inImage[i][k] / 255.0) ** gam)
    displayImage()


## 영상 파라볼라cup 처리
def paracupImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    val = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            val = 255 - 255 * (inImage[i][k] / 127 - 1) ** 2
            if (val > 255.0):
                val = 255
            elif (val < 0):
                val = 0
            outImage[i][k] = int(val)
    displayImage()

    ## 영상 파라볼라cap 처리


def paracapImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    val = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            val = 255 * (inImage[i][k] / 127 - 1) ** 2
            if (val > 255.0):
                val = 255
            elif (val < 0.0):
                val = 0
            outImage[i][k] = int(val)
    displayImage()


## 엠보싱 처리
def emboss():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    mask = [[-1, 0, 0],
            [0, 0, 0],
            [0, 0, 1]]  ##2차원 배열(3x3)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 2):  # for(int i =0i<inHi++)
        for k in range(inW + 2):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpOutImage[i][k] += 127

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


## 샤프닝 처리
def sharp():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    mask1 = [[-1, -1, -1],
             [-1, 9, -1],
             [-1, -1, -1]]  ##2차원 배열(3x3)

    mask2 = [[0, -1, 0],
             [-1, 5, -1],
             [0, -1, 0]]  ##2차원 배열(3x3)
    checkmask = askinteger('정수입력', '1이나2 입력', maxvalue=2, minvalue=1)
    mask = malloc2D(3, 3)
    if (checkmask != 1) and (checkmask != 2):
        print("잘못된 입력입니다!")
        return

    if (checkmask == 1):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = mask1[i][k]


    elif (checkmask == 2):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = mask2[i][k]

    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 2):  # for(int i =0i<inHi++)
        for k in range(inW + 2):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


## 언샤프닝 처리
def unsharp():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    alpha = askfloat("", "출력 영상에 알파값을 입력해주십시오.(1이면 언샤프 마스킹, 1보다 크면 하이부스트 필터링)", maxvalue=10)
    mask = [[1 / 9, 1 / 9, 1 / 9],
            [1 / 9, 1 / 9, 1 / 9],
            [1 / 9, 1 / 9, 1 / 9]]  ##2차원 배열(3x3)

    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 2):  # for(int i =0i<inHi++)
        for k in range(inW + 2):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = inImage[i][k] + alpha * inImage[i][k] - S

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


## 가우시안 스무딩 처리
def gauss():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    mask = [[1 / 16, 1 / 8, 1 / 16],
            [1 / 8, 1 / 4, 1 / 8],
            [1 / 16, 1 / 8, 1 / 16]]  ##2차원 배열(3x3)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 2):  # for(int i =0i<inHi++)
        for k in range(inW + 2):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S
    #
    # for i in range(inH):  # for(int i =0i<inHi++)
    #     for k in range(inW):
    #         tmpOutImage[i][k] += 127

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


## 입력된 마스크 스케일만큼 블러링 처리
def blurmask():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    maskscale = askinteger("블러링 마스크","블러링 마스크 스케일 크기를 입력하세요",maxvalue=100)
    if maskscale % 2 == 0:
        maskscale += 1
    print(maskscale)
    mask = malloc2D(maskscale, maskscale)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + maskscale - 1, inW + maskscale - 1)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(maskscale):  # for(int i =0i<inHi++)
        for k in range(maskscale):
            mask[i][k] = 1.0 / (maskscale ** 2)
    for i in range(inH + maskscale - 1):  # for(int i =0i<inHi++)
        for k in range(inW + maskscale - 1):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpInImage[int(i + (maskscale / 2))][int(k + (maskscale / 2))] = inImage[i][k]

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0
            for m in range(maskscale):  # for(int i =0i<inHi++)
                for n in range(maskscale):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]
            tmpOutImage[i][k] = int(S)

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


## 영상 축소
def zumout():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scale = askinteger("축소할 배수값을 입력해주십시오.", "최대값: 20", maxvalue=20)
    outH = int(inH / scale)
    outW = int(inW / scale)
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            outImage[int(i / scale)][int(k / scale)] = inImage[i][k]
    displayImage()


## 영상 축소(평균값보간)
def zumout2():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scale = askinteger("축소할 배수값을 입력해주십시오.", "최대값: 20", maxvalue=20)
    outH = int(inH / scale)
    outW = int(inW / scale)
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            sum = 0
            for a in range(i, i + scale):  # for(int i =0i<inHi++)
                for b in range(k, k + scale):  # for(int k =0k<inWi++)
                    if ((0 <= a and a < inH) and (0 <= b and b < inW)):
                        sum += inImage[a][b]
            med = int(sum / (scale * scale))
            if med > 255:
                outImage[int(i / scale)][int(k / scale)] = 255
            elif med < 0:
                outImage[int(i / scale)][int(k / scale)] = 0
            else:
                outImage[int(i / scale)][int(k / scale)] = int(med)
    displayImage()

## 영상 축소(중앙값보간)
def zumout3():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scale = askinteger("축소할 배수값을 입력해주십시오.", "최대값: 20", maxvalue=20)
    outH = int(inH / scale)
    outW = int(inW / scale)
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    sum = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            temp = [0 for _ in range(scale ** 2)]
            temp_index = 0
            for a in range(i, i + scale):  # for(int i =0i<inHi++)
                for b in range(k, k + scale):  # for(int k =0k<inWi++)
                    if ((0 <= a and a < inH) and (0 <= b and b < inW)):
                        temp[temp_index] = inImage[a][b]
                        temp_index += 1
            quickSort(temp)
            if temp[int(scale ** 2 / 2)] > 255:
                outImage[(i / scale)][int(k / scale)] = 255
            elif temp[int(scale ** 2 / 2)] < 0:
                outImage[int(i / scale)][int(k / scale)] = 0
            else:
                outImage[int(i / scale)][int(k / scale)] = temp[int(scale ** 2 / 2)]

    displayImage()


## 영상 확대(포워딩사상)
def zumin():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scale = askinteger("확대할 배수값을 입력해주십시오.", "최대값: 20", maxvalue=20)
    outH = int(inH * scale)
    outW = int(inW * scale)
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    sum = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            outImage[int(i * scale)][int(k * scale)] = inImage[i][k]
    displayImage()


## 영상 확대(백워딩사상)
def zumin2():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scale = askinteger("확대할 배수값을 입력해주십시오.", "최대값: 20", maxvalue=20)
    outH = int(inH * scale)
    outW = int(inW * scale)
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    sum = 0
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)
            outImage[i][k] = inImage[int(i / scale)][int(k / scale)]
    displayImage()


## 영상 확대(백워딩 + 양선형 보간법)
def zumin3():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scale = askinteger("확대할 배수값을 입력해주십시오.", "최대값: 20", maxvalue=20)
    outH = int(inH * scale)
    outW = int(inW * scale)
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    sum, newValue = 0, 0
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)

            r_H = i / scale
            r_W = k / scale
            i_H = int(math.floor(r_H))
            i_W = int(math.floor(r_W))
            s_H = r_H - i_H
            s_W = r_W - i_W
            if (i_H < 0 or i_H >= (inH - 1) or i_W < 0 or i_W >= (inW - 1)):
                outImage[i][k] = 255
            else:
                C1 = inImage[i_H][i_W]
                C2 = inImage[i_H][i_W + 1]
                C3 = inImage[i_H + 1][i_W + 1]
                C4 = inImage[i_H + 1][i_W]
                newValue = C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H
                if newValue > 255:
                    outImage[i][k] = 255
                elif newValue < 0:
                    outImage[i][k] = 0
                else:
                    outImage[i][k] = int(newValue)
    displayImage()


## 영상 회전(포워딩)
def rotate():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    degree = askinteger("각도값을 입력해주십시오.", "최대값: 360", maxvalue=360, minvalue=0)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    radian = degree * 3.141592 / 180.0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            xs = i
            ys = k
            xd = int(math.cos(radian) * xs - math.sin(radian) * ys)
            yd = int(math.sin(radian) * xs + math.cos(radian) * ys)

            if ((0 <= xd and xd < outH) and (0 <= yd and yd < outW)):
                outImage[xd][yd] = inImage[xs][ys]
    displayImage()


## 영상 회전(백워딩)
def rotate2():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    degree = askinteger("각도값을 입력해주십시오.", "최대값: 360", maxvalue=360)
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    radian = degree * 3.141592 / 180.0
    cx = inH / 2
    cy = inW / 2

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            xd = i
            yd = k

            xs = math.cos(radian) * (xd - cx) + math.sin(radian) * (yd - cy)
            ys = -math.sin(radian) * (xd - cx) + math.cos(radian) * (yd - cy)
            xs += cx
            ys += cy
            if ((0 <= xs and xs < outH) and (0 <= ys and ys < outW)):
                outImage[xd][yd] = inImage[xd][yd]

    displayImage()


## 영상 회전(확대)
def rotate3():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    degree = askinteger("각도값을 입력해주십시오.", "최대값: 360", maxvalue=360)
    radian = degree * math.pi / 180.0
    outW = int(inH * math.cos((90 - degree) * math.pi / 180.0) + inW * math.cos(radian))
    outH = int(inW * math.cos((90 - degree) * math.pi / 180.0) + inH * math.cos(radian))
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    cx = int(inH / 2)
    cy = int(inW / 2)
    cx2 = int(outH / 2)
    cy2 = int(outW / 2)
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)
            xd = i
            yd = k
            xs = int(math.cos(radian) * (xd - cx2) + math.sin(radian) * (yd - cy2))
            ys = int(-math.sin(radian) * (xd - cx2) + math.cos(radian) * (yd - cy2))
            xs += cx
            ys += cy

            if ((0 <= xs < inH) and (0 <= ys < inW)):
                outImage[xd][yd] = inImage[xs][ys]
            else:
                outImage[xd][yd] = 255

    displayImage()


## 영상 회전(확대+양선형 보간법)
def rotate4():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    degree = askinteger("각도값을 입력해주십시오.", "최대값: 360", maxvalue=360)
    radian = degree * math.pi / 180
    outW = int(inH * math.cos((90 - degree) * math.pi / 180.0) + inW * math.cos(radian))
    outH = int(inW * math.cos((90 - degree) * math.pi / 180.0) + inH * math.cos(radian))
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    cx = int(inH / 2)
    cy = int(inW / 2)
    cx2 = int(outH / 2)
    cy2 = int(outW / 2)
    C1, C2, C3, C4 = [0] * 4
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)

            r_H = i / (outH / inH)
            r_W = k / (outH / inH)
            i_H = int(math.floor(r_H))
            i_W = int(math.floor(r_W))
            s_H = r_H - i_H
            s_W = r_W - i_W
            if ((inH - 1) <= i_H < 0 or 0 > i_W >= (inW - 1)):
                outImage[i][k] = 255
            xd = i
            yd = k
            xs1 = int(math.cos(radian) * (xd - cx2) + math.sin(radian) * (yd - cy2))
            ys1 = int(-math.sin(radian) * (xd - cx2) + math.cos(radian) * (yd - cy2))

            xs2 = int(math.cos(radian) * (xd + 1 - cx2) + math.sin(radian) * (yd - cy2))
            ys2 = int(-math.sin(radian) * (xd + 1 - cx2) + math.cos(radian) * (yd - cy2))

            xs3 = int(math.cos(radian) * (xd - cx2) + math.sin(radian) * (yd + 1 - cy2))
            ys3 = int(-math.sin(radian) * (xd - cx2) + math.cos(radian) * (yd + 1 - cy2))

            xs4 = int(math.cos(radian) * (xd + 1 - cx2) + math.sin(radian) * (yd + 1 - cy2))
            ys4 = int(-math.sin(radian) * (xd + 1 - cx2) + math.cos(radian) * (yd + 1 - cy2))

            xs1 += cx
            ys1 += cy

            xs2 += cx
            ys2 += cy

            xs3 += cx
            ys3 += cy

            xs4 += cx
            ys4 += cy

            if ((0 <= xs1 and xs1 < inH) and (0 <= ys1 and ys1 < inW)):
                C1 = inImage[int(xs1)][int(ys1)]
            else:
                C1 = 255
            if ((0 <= xs2 and xs2 < inH) and (0 <= ys2 and ys2 < inW)):
                C2 = inImage[int(xs2)][int(ys2)]
            else:
                C2 = 255
            if ((0 <= xs3 < inH) and (0 <= ys3 < inW)):
                C3 = inImage[int(xs3)][int(ys3)]
            else:
                C3 = 255
            if ((0 <= xs4 < inH) and (0 <= ys4 < inW)):
                C4 = inImage[int(xs4)][int(ys4)]
            else:
                C4 = 255
            newValue = int(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C4 * s_W * s_H + C3 * (1 - s_W) * s_H)
            if newValue > 255:
                outImage[i][k] = 255
            elif newValue < 0:
                outImage[i][k] = 0
            else:
                outImage[i][k] = newValue

    displayImage()


## 영상 이동
def moveImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    left = askinteger("왼쪽으로 이동할 값을 입력해주십시오.", "0~500", maxvalue=500, minvalue=0)
    right = askinteger("오른쪽으로 이동할 값을 입력해주십시오", "0~500", maxvalue=500, minvalue=0)
    up = askinteger("위쪽으로 이동할 값을 입력해주십시오", "0~500", maxvalue=500, minvalue=0)
    down = askinteger("아래쪽으로 이동할 값을 입력해주십시오", "0~500", maxvalue=500, minvalue=0)
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        move_updown = i + up - down
        for k in range(inW):  # for(int k =0k<inWi++)
            move_side = k + left - right
            if ((0 <= move_updown and move_updown < inH) and (0 <= move_side and move_side < inW)):
                outImage[i][k] = inImage[move_updown][move_side]

    displayImage()


##이미지 대칭

def switchImage():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존

    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    switchval = 0
    while (switchval != 9):
        switchval = askinteger("", "어떤 방향으로 대칭을 할 것입니까?(1 : 좌우 2 : 위아래 3 : 좌우위아래 9 : 종료)")
        # int(input("어떤 방향으로 대칭을 할 것입니까?(1 : 좌우 2 : 위아래 3 : 좌우위아래 9 : 종료)"))
        if switchval == 1:
            for i in range(inH):  # for(int i =0i<inHi++)
                for k in range(inW):  # for(int k =0k<inWi++)
                    outImage[i][k] = inImage[i][inW - (k + 1)]
            displayImage()
        if switchval == 2:
            for i in range(inH):  # for(int i =0i<inHi++)
                for k in range(inW):  # for(int k =0k<inWi++)
                    outImage[i][k] = inImage[inH - (i + 1)][k]
            displayImage()
        if switchval == 3:
            for i in range(inH):  # for(int i =0i<inHi++)
                for k in range(inW):  # for(int k =0k<inWi++)
                    outImage[i][k] = inImage[inH - (i + 1)][inW - (k + 1)]
            displayImage()
        if switchval == 9:
            messagebox.showinfo("종료", "프로그램 종료")
            break


# 영상 모핑
def moping():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    mopingopen()
    mopimageper = askfloat("모핑할 비율 입력", "0 ~ 100", maxvalue=100, minvalue=0)
    # 출력 이미지 메모리 확보
    insmall = 1
    if (inH >= mopH):
        cx = cy = int(inH / 2)
        outH = outW = inH
        cx2 = cy2 = int(mopH / 2)
        insmall = 0
    else:
        cx = cy = int(mopH / 2)
        outH = outW = mopH
        cx2 = cy2 = int(inH / 2)
    outImage = malloc2D(outH, outW)
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)
            xd = i
            yd = k
            xs = cx - cx2
            ys = cy - cx2
            if (insmall == 1):
                if ((xs <= xd and xd < xs + inH) and (ys <= yd and yd < ys + inW)):
                    value = abs((inImage[xd - xs][yd - ys] - mopImage[xd][yd]))
                    if (inImage[xd - xs][yd - ys] <= mopImage[xd][yd]):
                        outv = int(mopImage[xd][yd] - value * (1 - (mopimageper / 100)))
                    else:
                        outv = int(mopImage[xd][yd] + value * (1 - (mopimageper / 100)))
                else:
                    outv = mopImage[xd][yd]
            else:
                if ((xs <= xd and xd < xs + mopH) and (ys <= yd and yd < ys + mopW)):
                    value = abs((inImage[xd][yd] - mopImage[xd - xs][yd - ys]))
                    if (inImage[xd][yd] <= mopImage[xd - xs][yd - ys]):
                        outv = int((mopImage[xd - xs][yd - ys] - value * (1 - (mopimageper / 100))))
                    else:
                        outv = int((mopImage[xd - xs][yd - ys] + value * (1 - (mopimageper / 100))))
                else:
                    outv = inImage[xd][yd]

            if (outv > 255):
                outv = 255
            if (outv < 0):
                outv = 0
            outImage[xd][yd] = int(outv)
    displayImage()


## 앤드 인 탐색
def endin():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if (inImage[i][k] < low):
                low = inImage[i][k]
            if (inImage[i][k] > high):
                high = inImage[i][k]
    high -= 50
    low += 50
    ## 진짜 영상 처리 알고리즘
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            old = inImage[i][k]
            new = int(((old - low) / (high - low) * 255))
            if (new > 255):
                new = 255
            if (new < 0):
                new = 0
            outImage[i][k] = new
    displayImage()


## 히스토그램 평활화
def histoequal():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    histo = [0 for i in range(256)]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            histo[inImage[i][k]] += 1
    sumHisto = [0 for i in range(256)]
    sumHisto[0] = histo[0]

    for i in range(1, 256):  # for(int i =0i<inHi++)
        sumHisto[i] = int(sumHisto[i - 1] + histo[i])

    normalHisto = [0 for i in range(256)]
    for i in range(256):  # for(int i =0i<inHi++)
        normalHisto[i] = int(sumHisto[i] * (1 / (inH * inW)) * 255)

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if (int(normalHisto[inImage[i][k]]) > 255):
                outImage[i][k] = 255
            if (int(normalHisto[inImage[i][k]]) < 0):
                outImage[i][k] = 0
            outImage[i][k] = int(normalHisto[inImage[i][k]])
    displayImage()


# 히스토그램 명세화
def histospecific():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    mopingopen()
    histo = [0 for i in range(256)]
    histo2 = [0 for i in range(256)]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            histo[inImage[i][k]] += 1

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            histo2[mopImage[i][k]] += 1
    sumHisto = [0 for i in range(256)]
    sumHisto2 = [0 for i in range(256)]
    sumHisto[0] = histo[0]


    for i in range(1, 256):  # for(int i =0i<inHi++)
        sumHisto[i] = int(sumHisto[i - 1] + histo[i])


    for i in range(1, 256):  # for(int i =0i<inHi++)
        sumHisto2[i] = int(sumHisto2[i - 1] + histo2[i])


    normalHisto = [[0 for _ in range(inW)]for _ in range(inH)]
    normalHisto2 = [0 for _ in range(256)]
    reverse_histo = [0 for i in range(outH * outW)]
    for i in range(inH):
        for k in range(inW):  # for(int i =0i<inHi++)
            normalHisto[i][k] = int(sumHisto[inImage[i][k]] * (1 / (inH * inW)) * 255)

    DMIN = sumHisto2[0]
    DMAX = sumHisto2[255]
    for i in range(256):  # for(int i =0i<inHi++)
        normalHisto2[i] = int((sumHisto2[i]-DMIN) * (1/ (DMAX-DMIN)) *255)

    top = 255
    bottom = top - 1
    while (True):
        for i in range(normalHisto2[bottom], normalHisto2[top] + 1):
            reverse_histo[i] = top

        top = bottom
        bottom = bottom - 1
        if (bottom < -1):
            break
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)
            if (int(reverse_histo[normalHisto[i][k]]) > 255):
                outImage[i][k] = 255
            if (int(reverse_histo[normalHisto[i][k]]) < 0):
                outImage[i][k] = 0
            outImage[i][k] = int(reverse_histo[normalHisto[i][k]])


    displayImage()


## 히스토그램 기본 명암 대비
def histostrech():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            if (inImage[i][k] < low):
                low = inImage[i][k]
            if (inImage[i][k] > high):
                high = inImage[i][k]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            old = inImage[i][k]
            new = int((old - low) / (high - low) * 255)
            if (new > 255):
                new = 255
            if (new < 0):
                new = 0
            outImage[i][k] = new
    displayImage()


## 엣지 수직 검출
def edgeVertical():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    mask = [[0, 0, 0],
            [-1, 1, 0],
            [0, 0, 0]]  ##2차원 배열(3x3)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 2):  # for(int i =0i<inHi++)
        for k in range(inW + 2):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpOutImage[i][k] += 60

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


## 엣지 수평 검출
def edgeHorizontal():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    mask = [[0, -1, 0],
            [0, 1, 0],
            [0, 0, 0]]  ##2차원 배열(3x3)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 2):  # for(int i =0i<inHi++)
        for k in range(inW + 2):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

                tmpOutImage[i][k] = S

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpOutImage[i][k] += 60

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


## 엣지 유사연산자 검출
def edgeSame():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scalemask = askinteger("마스크 크기를 입력해주십시오", "마스크 크기를 입력해주십시오. 최대값 : 20", maxvalue=20, minvalue=2)
    if (scalemask % 2 == 0):
        scalemask += 1
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + scalemask - 1, inW + scalemask - 1)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + scalemask - 1):  # for(int i =0i<inHi++)
        for k in range(inW + scalemask - 1):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[int(i + scalemask / 2)][int(k + scalemask / 2)] = inImage[i][k]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            maxout = 0
            for m in range(int(scalemask / 2)):  # for(int i =0i<inHi++)
                for n in range(int(scalemask / 2)):  # for(int k =0k<inWi++)
                    if (maxout <= abs(
                            tmpInImage[int(i + (scalemask / 2))][int(k + (scalemask / 2))] - tmpInImage[i + m][k + n])):
                        maxout = abs(
                            tmpInImage[int(i + (scalemask / 2))][int(k + (scalemask / 2))] - tmpInImage[i + m][k + n])
            tmpOutImage[i][k] = maxout
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)
            if (tmpOutImage[i][k] < 0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


## 엣지 차연산자 검출
def edgesub():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    scalemask = askinteger("마스크 크기를 입력해주십시오", "마스크 크기를 입력해주십시오 .최대값 : 20", maxvalue=20, minvalue=2)
    if (scalemask % 2 == 0):
        scalemask += 1
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + scalemask - 1, inW + scalemask - 1)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + scalemask - 1):  # for(int i =0i<inHi++)
        for k in range(inW + scalemask - 1):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[int(i + scalemask / 2)][int(k + scalemask / 2)] = inImage[i][k]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            maxout = 0
            for m in range(int(scalemask / 2)):  # for(int i =0i<inHi++)
                for n in range(int(scalemask / 2)):  # for(int k =0k<inWi++)
                    if (maxout <= abs(
                            tmpInImage[i + m][k + n] - tmpInImage[i + (scalemask - 1) - m][k + (scalemask - 1) - n])):
                        maxout = abs(
                            tmpInImage[i + m][k + n] - tmpInImage[i + (scalemask - 1) - m][k + (scalemask - 1) - n])
            tmpOutImage[i][k] = maxout
    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):  # for(int k =0k<inWi++)
            if (tmpOutImage[i][k] < 0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]

    displayImage()


## 1차 미분 회선 마스크 검출
def edgeFirstderivative():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    maskRobert_1 = [[-1, 0, 0],
                    [0, 1, 0],
                    [0, 0, 0]]  ##2차원 배열(3x3)
    maskRobert_2 = [[0, 0, -1],
                    [0, 1, 0],
                    [0, 0, 0]]  ##2차원 배열(3x3)
    maskPrewitt_1 = [[-1, -1, -1],
                     [0, 0, 0],
                     [1, 1, 1]]  ##2차원 배열(3x3)
    maskPrewitt_2 = [[1, 0, -1],
                     [1, 0, -1],
                     [1, 0, -1]]  ##2차원 배열(3x3)
    maskSobel_1 = [[-1, -2, -1],
                   [0, 0, 0],
                   [1, 2, 1]]  ##2차원 배열(3x3)
    maskSobel_2 = [[1, 0, -1],
                   [2, 0, -2],
                   [1, 0, -1]]  ##2차원 배열(3x3)
    mask = malloc2D(3, 3)
    mask2 = malloc2D(3, 3)
    checkmask = askinteger("1차 미분 회선 마스크 종류중 선택해주십시오. ", "(1 : 로버츠  2 : 프리윗  3 : 소벨 )", maxvalue=3, minvalue=1)
    # int(input("1차 미분 회선 마스크 종류중 선택해주십시오. (1 : 로버츠  2 : 프리윗  3 : 소벨 )")))
    checkmask2 = askinteger("1차 미분 회선 마스크의 행과 열중 선택해주십시오. ", "(1 : 행 or 2 :열)", maxvalue=2, minvalue=1)
    checkmask3 = askinteger("합성 출력을 하시겠습니까?  ", "(1 : 예 or 2 :아니요)", maxvalue=2, minvalue=1)

    if (checkmask == 1 and checkmask2 == 1):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = maskRobert_1[i][k]
                mask2[i][k] = maskRobert_2[i][k]
    if (checkmask == 1 and checkmask2 == 2):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = maskRobert_2[i][k]
                mask2[i][k] = maskRobert_1[i][k]
    if (checkmask == 2 and checkmask2 == 1):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = maskPrewitt_1[i][k]
                mask2[i][k] = maskPrewitt_2[i][k]
    if (checkmask == 2 and checkmask2 == 2):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = maskPrewitt_2[i][k]
                mask2[i][k] = maskPrewitt_1[i][k]
    if (checkmask == 3 and checkmask2 == 1):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = maskSobel_1[i][k]
                mask2[i][k] = maskSobel_2[i][k]
    if (checkmask == 3 and checkmask2 == 2):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = maskSobel_2[i][k]
                mask2[i][k] = maskSobel_1[i][k]

    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0
            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]
                    if (checkmask3 == 1):
                        S += tmpInImage[i + m][k + n] * mask2[m][n]
            tmpOutImage[i][k] = S

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpOutImage[i][k] += 60

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


## LoG 검출
def edgeLoG():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    mask = [[0, 0, -1, 0, 0],
            [0, -1, -2, -1, 0],
            [-1, -2, 16, -2, -1],
            [0, -1, -2, -1, 0],
            [0, 0, -1, 0, 0]]
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 4, inW + 4)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + 4):  # for(int i =0i<inHi++)
        for k in range(inW + 4):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i + 2][k + 2] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(5):  # for(int i =0i<inHi++)
                for n in range(5):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpOutImage[i][k] += 60

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


## 2차 미분 라플라시안 검출
def edgeSecondLap():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    mask1 = [[0, -1, 0],
             [-1, 4, -1],
             [0, -1, 0]]  ##2차원 배열(3x3)
    mask2 = [[1, 1, 1],
             [1, -8, 1],
             [1, 1, 1]]  ##2차원 배열(3x3)
    mask3 = [[-1, -1, -1],
             [-1, 8, -1],
             [-1, -1, -1]]  ##2차원 배열(3x3)
    checkmask = 0
    mask = [[0 for _ in range(3)] for _ in range(3)]
    messagebox.showinfo("",'2차 미분 라플라시안 마스크 종류중 선택해주십시오.')
    messagebox.showinfo(
        "","1 :  [ [0.0, -1.0, -1.0],\n[ -1.0, 4.0, -1.0],\n[ 0.0, -1.0, 0.0 ] ] \n2 : [ [1.0, 1.0, 1.0],\n[ 1.0, -8.0, 1.0],\n[ 1.0, 1.0, 1.0 ]]\n3 : [ [-1.0, -1.0, -1.0],\n[ -1.0, 8.0, -1.0],\n[ -1.0, -1.0, -1.0 ] ]")

    checkmask = askinteger("2차 미분 라플라시안 마스크 종류 선택 ", "1,2,3중 하나", maxvalue=3, minvalue=1)
    if (checkmask != 1 and checkmask != 2 and checkmask != 3):
        messagebox.showinfo("잘못된 입력입니다!!")
        return
    if (checkmask == 1):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = mask1[i][k]

    if (checkmask == 2):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = mask2[i][k]

    if (checkmask == 3):
        for i in range(3):  # for(int i =0i<inHi++)
            for k in range(3):  # for(int k =0k<inWi++)
                mask[i][k] = mask3[i][k]

    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + 2, inW + 2)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = inImage[i][k]
    S = 0
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(3):  # for(int i =0i<inHi++)
                for n in range(3):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            tmpOutImage[i][k] += 127

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = tmpOutImage[i][k]
    displayImage()


def edgeDoG():
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW, mopH, mopW, mopImage  ## 파이썬에서는 그로벌변수를 다음과 같이 정의해줘야한다.
    # ( 중요!) 출력 이미지 크기 결정 ! --> 알고리즘에 의존
    mask1 = [[0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0],
             [0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0],
             [-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0],
             [-1.0, -3.0, 5.0, 16.0, 5.0, -3.0, -1.0],
             [-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0],
             [0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0],
             [0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0]]
    mask2 = [[0.0, 0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0],
             [0.0, -2.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.0, 0.0],
             [0.0, -3.0, -2.0, -1.0, -1.0, -1.0, -2.0, -3.0, 0.0],
             [-1.0, -3.0, -1.0, 9.0, 9.0, 9.0, -1.0, -3.0, -1.0],
             [-1.0, -3.0, -1.0, 9.0, 19.0, 9.0, -1.0, -3.0, -1.0],
             [-1.0, -3.0, -1.0, 9.0, 9.0, 9.0, -1.0, -3.0, -1.0],
             [0.0, -3.0, -2.0, -1.0, -1.0, -1.0, -2.0, -3.0, 0.0],
             [0.0, -2.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.0, 0.0],
             [0.0, 0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0]]
    checkmask = askinteger("사용할 DoG 엣지 검출의 마스크 크기를 결정해주세요 ( 7: 7x7 9: 9x9)",
                           "사용할 DoG 엣지 검출의 마스크 크기를 결정해주세요 ( 7: 7x7 9: 9x9)")
    mask = malloc2D(7, 7)
    for i in range(7):  # for(int i =0i<inHi++)
        for k in range(7):
            mask[i][k] = 0
    if (checkmask != 7 and checkmask != 9):
        messagebox.showinfo("잘못된 입력입니다!!")
        return

    if (checkmask == 7):
        maskH = maskW = 7
        for i in range(7):  # for(int i =0i<inHi++)
            for k in range(7):
                mask[i][k] = mask1[i][k]
    if (checkmask == 9):
        maskH = maskW = 9
        mask = malloc2D(9, 9)
        for i in range(9):  # for(int i =0i<inHi++)
            for k in range(9):
                mask[i][k] = mask2[i][k]
    outH = inH
    outW = inW
    # 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)
    ## 진짜 영상 처리 알고리즘
    tmpInImage = malloc2D(inH + maskH - 1, inW + maskW - 1)
    tmpOutImage = malloc2D(outH, outW)
    for i in range(inH + maskH - 1):  # for(int i =0i<inHi++)
        for k in range(inW + maskW - 1):  # for(int k =0k<inWi++)
            tmpInImage[i][k] = 127

    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):  # for(int k =0k<inWi++)
            tmpInImage[int(i + maskH / 2)][int(k + maskW / 2)] = inImage[i][k]
    for i in range(inH):  # for(int i =0i<inHi++)
        for k in range(inW):
            S = 0

            for m in range(maskH):  # for(int i =0i<inHi++)
                for n in range(maskW):  # for(int k =0k<inWi++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    for i in range(outH):  # for(int i =0i<inHi++)
        for k in range(outW):
            if (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            elif (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


def copyFile():
    messagebox.showinfo('요기제목', '복사 클릭');


## 변수부
window, canvas, paper = None, None, None
inImage, outImage, mopImage = [], [], []
inH, inW, outH, outW, mopH, mopW = [0] * 6
fullname = ""
mopname = ""
## 메인부
window = Tk()  # 벽
window.geometry("500x500")
window.resizable(width=False, height=False)
window.title("영상처리 (RC 1)")

#########################################
# 메뉴 만들기
mainMenu = Menu(window)  # 메뉴의 틀
window.config(menu=mainMenu)

fileMenu = Menu(mainMenu, tearoff=0)  # 상위 메뉴 (파일)
mainMenu.add_cascade(label='파일', menu=fileMenu)
fileMenu.add_command(label='열기', command=openImage)
fileMenu.add_command(label='저장',command=saveImage)
fileMenu.add_separator()
fileMenu.add_command(label='종료')

##############################################
####화소점 처리 메뉴
##############################################
pixcomaMenu = Menu(mainMenu, tearoff=0)  # 상위 메뉴 (파일)

mainMenu.add_cascade(label='화소점처리', menu=pixcomaMenu)

pixcomaMenu.add_command(label='밝게/어둡게하기', command=addImage)
pixcomaMenu.add_command(label='스트래칭(곱셈하기)', command=mulImage)
pixcomaMenu.add_command(label='압축(나눗셈하기)', command=divImage)
bwmenu = Menu(pixcomaMenu)  # 상위 메뉴 (파일)
pixcomaMenu.add_cascade(label='이진화하기', menu=bwmenu)
bwmenu.add_command(label='128이진화', command=bwImage128)
bwmenu.add_command(label='중앙값이진화', command=bwImagemiddle)
bwmenu.add_command(label='평균값이진화', command=bwImagemed)
pixcomaMenu.add_command(label='반전하기', command=reverseImage)
pixcomaMenu.add_command(label='포스터라이징', command=posterImage)
pixcomaMenu.add_command(label='범위 강조', command=impressImage)
pixcomaMenu.add_command(label='감마보정', command=gamImage)
paraMenu = Menu(pixcomaMenu)  # 추가상위 메뉴 (파라볼라보정 cap&cup)
pixcomaMenu.add_cascade(label='파라볼라보정', menu=paraMenu)
paraMenu.add_command(label='파라볼라CAP보정', command=paracapImage)
paraMenu.add_command(label='파라볼라CUP보정', command=paracupImage)
bitMenu = Menu(pixcomaMenu)  # 추가상위 메뉴 (AND&OR&XOR)
pixcomaMenu.add_cascade(label='비트연산', menu=bitMenu)
bitMenu.add_command(label='AND', command=andImage)
bitMenu.add_command(label='OR', command=orImage)
bitMenu.add_command(label='XOR', command=xorImage)

##############################################
####화소 영역 처리 메뉴
##############################################
pixscaleMenu = Menu(mainMenu, tearoff=0)  # 상위 메뉴 (파일)

mainMenu.add_cascade(label='화소영역처리', menu=pixscaleMenu)

pixscaleMenu.add_command(label='엠보싱', command=emboss)
pixscaleMenu.add_command(label='블러링', command=blurmask)
pixscaleMenu.add_command(label='샤프닝', command=sharp)
pixscaleMenu.add_command(label='언샤프닝', command=unsharp)
pixscaleMenu.add_command(label='가우스', command=gauss)

##############################################
####기하학 처리 메뉴
##############################################
moveMenu = Menu(mainMenu, tearoff=0)  # 상위 메뉴 (파일)

mainMenu.add_cascade(label='기하학처리', menu=moveMenu)

zumoutMenu = Menu(moveMenu, tearoff=0)  # 추가상위 메뉴 (축소)
moveMenu.add_cascade(label='축소', menu=zumoutMenu)
zumoutMenu.add_command(label='포워딩사상', command=zumout)
zumoutMenu.add_command(label='에일리어싱해결(평균값)', command=zumout2)
zumoutMenu.add_command(label='에일리어싱해결(중간값)', command=zumout3)

zuminMenu = Menu(moveMenu, tearoff=0)  # 추가상위 메뉴 (확대)
moveMenu.add_cascade(label='확대', menu=zuminMenu)
zuminMenu.add_command(label='포워딩사상', command=zumin)
zuminMenu.add_command(label='백워딩사상', command=zumin2)
zuminMenu.add_command(label='백워딩+양선형 보간법', command=zumin3)

rotateMenu = Menu(moveMenu,tearoff=0)  # 추가상위 메뉴 (회전)
moveMenu.add_cascade(label='회전', menu=rotateMenu)
rotateMenu.add_command(label='포워딩사상', command=rotate)
rotateMenu.add_command(label='백워딩사상', command=rotate2)
rotateMenu.add_command(label='확대', command=rotate3)
rotateMenu.add_command(label='확대 + 양선형 보간법', command=rotate4)

moveMenu.add_command(label='이동', command=moveImage)
moveMenu.add_command(label='대칭', command=switchImage)
moveMenu.add_command(label='모핑', command=moping)

##############################################
####엣지 검출 처리
#############################################
EdgeMenu = Menu(mainMenu,tearoff=0)  # 상위 메뉴 (파일)

mainMenu.add_cascade(label='엣지검출', menu=EdgeMenu)
EdgeMenu.add_command(label='엣지수직검출', command=edgeVertical)
EdgeMenu.add_command(label='엣지수평검출', command=edgeHorizontal)
EdgeMenu.add_command(label='1차미분검출', command=edgeFirstderivative)
EdgeMenu.add_command(label='2차라플라시안검출', command=edgeSecondLap)
EdgeMenu.add_command(label='유사연산자 검출', command=edgeSame)
EdgeMenu.add_command(label='차 연산자 검출', command=edgesub)
EdgeMenu.add_command(label='LoG', command=edgeLoG)
EdgeMenu.add_command(label='DoG', command=edgeDoG)

##############################################
####히스토그램 처리
#############################################
histoMenu = Menu(mainMenu,tearoff=0)  # 상위 메뉴 (파일)

mainMenu.add_cascade(label='히스토그램처리', menu=histoMenu)
histoMenu.add_command(label='엔드인', command=endin)
histoMenu.add_command(label='히스토그램평활화', command=histoequal)
histoMenu.add_command(label='기본명암대비', command=histostrech)
histoMenu.add_command(label='명세화', command=histospecific)

# 컨트롤 == 위젯
# label1 = Label(window, text='나는 글자다', font=('궁서체',20), fg='red', bg='yellow' )
# button1 = Button(window, text='나를 클릭해줘~')
#
#
# label1.pack()
# button1.pack(side=BOTTOM)

window.mainloop()
# 퀴즈 : [영상처리]>>[동일영상],[밝게하기]equalImage()