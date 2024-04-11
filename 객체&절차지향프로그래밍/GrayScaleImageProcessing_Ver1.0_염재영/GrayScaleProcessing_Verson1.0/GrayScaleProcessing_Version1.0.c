#define  _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h> // GUI 지원 기능
#include <math.h>
#include <io.h> // 파일 입력 확인을 위한 access명령어 사용	
//#include <winuser.h>
//#include<vfw.h>
/////////////////////
// 전역 변수부
/////////////////////
HWND hwnd; // 윈도 화면(외부,물리적)
HDC hdc; // 윈도 화면(내부, 논리적)
// Test 추가 코딩
//PAINTSTRUCT ps;
// 파일 입출력을 위한 파일 포인터 변수
FILE* rfp, * wfp;

// 파일 이름
char fileName[200], mopfileName[200];

// 영상의 크기(H : 열 길이 W : 헹 길이)
int inH, inW, outH, outW, mopH, mopW, nowH, nowW;

// 영상의 히스토그램 출력
int histo_in[256], histo_out[256], histo_mop[256];

// 메모리 확보
unsigned char** inImage, ** outImage, ** mopImage, ** tmpImage;

/////////////////////
// 함수 선언부
/////////////////////
// 공통 함수 선언부9
void loadImage(); void quickSort(unsigned char* arr, int L, int R); int medvalueImage();

int getIntValue(); void printImage(); void saveImage(); void printMenu(); void printMenu1();  void printMenu2(); void printMenu3(); void printMenu4(); void printMenu5();

void freeInputMemory(); void freeOutputMemory(); void mallocInputMemory(); void mallocOutputMemory(); void mallocTmpMemory(); void mallocMopMemory();  void freeMopMemory(); void freeTmpMemory();

double** mallocDoubleMemory(int, int); void freeDoubleMemory(double**, int); void checkworkimage(); void effectpile();

// 화소 처리 함수 선언부
void equalImage(); void equalmopImage(); void addImage();

void subImage(); void reverseImage(); void black_whiteImage();

void gamaImage(); void para_cupImage(); void black_whiteImage_med(); void black_whiteImage_middlle(); void para_capImage();

void strechImage(); void pressImage(); void posterImage(); void andImage(); void orImage(); void xorImage(); void impressImage();

// 화소 영역 처리 함수 선언부
void emboss(); void blur(); void sharp(); void blur9(); void sharp_high(); void unsharp(); void gauss(); void blurmask();

// 기하학 처리 함수 선언부
void zumout(); void zumin(); void zumin2(); void zumIn3(); void rotate(); void rotate2(); void rotate3(); void rotate4(); void zumout2(); void zumout3();

void moveImage(); void switchImage(); void moping(); void moping_load();

// 히스토그램 처리 함수 선언부
void histoStrech(); void endin(); void histoequal(); void histoSpecification();

// 엣지 추출 처리 함수 선언부
void edgeVertical(); void edgeHorizontal(); void edgeSame(); void edgesub(); void edgeFirstderivative(); void edgeSecondLap(); void edgeLoG(); void edgeDoG();

// 메인 함수
void main() {
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL); // Windows 10 : hwnd

	char inKey = 0, inKey1 = 0, inKey2 = 0, inKey3 = 0, inKey4 = 0, inKey5 = 0;

	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		// 버튼 값 입력 받기
		system("cls");
		// cmd 화면 초기화
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '9': break;
			// 화소 점 처리 영역
		case 'a': case 'A':
			inKey1 = 0; // 적용할 알고리즘 입력 버튼값 초기화
			while (inKey1 != '9') {
				printMenu1();
				inKey1 = _getch();
				switch (inKey1) {
				case '9': inKey = '9'; break; // 완전 종료버튼
				case '1': equalImage(); break; // 동일 이미지 
				case '2': addImage(); break; // 영상 화소점 덧셈
				case '3': subImage(); break; // 영상 화소점 뺄셀
				case '4': reverseImage(); break; // 영상 반전
				case '5': black_whiteImage(); break; // 영상 이진화(128)
				case '6': gamaImage(); break; // 영상 감마 보정
				case '7': para_cupImage(); break; // 영상 CUP 보정
				case '8': para_capImage(); break; // 영상 CAP 보정
				case 'a':case 'A': black_whiteImage_med(); break; // 영상 이진화(평균값)
				case 'b':case 'B': black_whiteImage_middlle(); break; // 영상 이진화(중앙값)
				case 'c':case 'C': strechImage(); break; // 영상 스트래칭
				case 'd':case 'D': pressImage(); break; // 영상 압축(나눗셈)
				case 'e':case 'E': posterImage(); break; // 영상 포스터라이징
				case 'f': case 'F': andImage(); break; // 영상 AND
				case 'g': case 'G': orImage(); break; // 영상 OR
				case 'h': case 'H': xorImage(); break; // 영상 XOR
				case 'i': case 'I': impressImage(); break; // 영상 범위 강조
				case '+': equalImage(); break; // 영상 동일 이미지
				case '*': effectpile(); break; // 효과 중첩
				case '-': inKey1 = '9'; break; // 뒤로 이동 버튼
				}
			}
			break;
			// 화소 영역 처리 영역
		case 'b': case 'B':
			inKey2 = 0; // 적용할 알고리즘 입력 버튼값 초기화
			while (inKey2 != '9') {
				printMenu2();
				inKey2 = _getch();
				switch (inKey2) {
				case '9': inKey1 = '9'; inKey2 = '9'; break; // 완전 종료버튼
				case '1': emboss(); break; // 영상 엠보싱
				case '2': blur(); break; // 영상 블러링(3X3)
				case '3': blur9(); break; // 영상 블러링 (9X9)
				case '4': sharp(); break; // 영상 샤프닝
				case '5': unsharp(); break; // 영상 언샤프 마스킹
				case '6': gauss(); break; // 영상 가우시안 스무딩
				case '7': blurmask(); break; // 영상 블러링(사용자 마스크 크기 입력)
				case '+': equalImage(); break; // 동일 이미지
				case '*': effectpile(); break; // 효과 중첩
				case '-': inKey2 = '9'; break; // 뒤로 이동 버튼
				}
			}
			break;
			// 기하학 처리 영역
		case 'c': case 'C':
			inKey3 = 0; // 적용할 알고리즘 입력 버튼값 초기화
			while (inKey3 != '9') {
				printMenu3();
				inKey3 = _getch();
				switch (inKey3) {
				case '9': inKey = '9'; break;
				case '1': zumout(); break;
				case '2': zumout2(); break;
				case '3': zumout3(); break;
				case '4': zumin(); break;
				case '5': zumin2(); break;
				case '6': zumIn3(); break;
				case '7': rotate(); break;
				case '8': rotate2(); break;
				case 'a':case 'A': rotate3(); break;
				case 'b':case 'B': rotate4(); break;
				case 'c': case 'C': moveImage(); break;
				case 'd': case 'D': switchImage(); break;
				case 'e': case 'E': moping(); break;
				case '+': equalImage(); break;
				case '*': effectpile(); break;
				case '-': inKey3 = '9'; break;
				}
			}
			break;
			// 히스토그램 처리 영역
		case 'd': case 'D':
			inKey4 = 0; // 적용할 알고리즘 입력 버튼값 초기화// 적용할 알고리즘 입력 버튼값 초기화
			while (inKey4 != '9') {
				printMenu4();
				inKey4 = _getch();
				switch (inKey4) {
				case '9': inKey = '9'; break;
				case '1': endin(); break;
				case '2': histoequal(); break;
				case '3': histoStrech(); break;
				case '4': histoSpecification(); break;
				case '+': equalImage(); break;
				case '*': effectpile(); break;
				case '-': inKey4 = '9'; break;
				}
			}
			break;
			// 엣지 검출 처리 영역
		case 'e': case 'E':
			inKey5 = 0; // 적용할 알고리즘 입력 버튼값 초기화// 적용할 알고리즘 입력 버튼값 초기화
			while (inKey5 != '9') {
				printMenu5();
				inKey5 = _getch();
				switch (inKey5) {
				case '9': inKey = '9'; break;
				case '1': edgeVertical(); break;
				case '2': edgeHorizontal(); break;
				case '3': edgeSame(); break;
				case '4': edgesub(); break;
				case '5': edgeFirstderivative(); break;
				case '6': edgeSecondLap(); break;
				case '7': edgeLoG(); break;
				case '8': edgeDoG(); break;
				case '+': equalImage(); break;
				case '*': effectpile(); break;
				case '-': inKey5 = '9'; break;
				}
			}
			break;
		default:
			puts("잘못된 키를 입력하였습니다!!!");
			break;
		}


	}
	freeInputMemory();
	freeOutputMemory();
	freeMopMemory();
}

/////////////////////
// 함수 정의부
/////////////////////
/// 공통 함수
//////////////
int medvalueImage() {
	// 이미지 픽셀 평균값 출력
	int sum = 0;
	double med = 0;
	printf("\n평균값 구하기\n\n");
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			sum += inImage[i][k];
		}
	}
	med = sum / (inH * inW);

	printf("\n영상 grayscale 평균값 : %lf\n\n", med);

	return med;
}

void quickSort(unsigned char* arr, int L, int R) {
	// 중앙값추출을 위한 정렬 함수
	// 퀵정렬
	int left = L, right = R;
	unsigned char pivot = arr[(L + R) / 2];
	unsigned char temp = 0;
	do
	{
		while (arr[left] < pivot)
			left++;
		while (arr[right] > pivot)
			right--;
		if (left <= right)
		{
			temp = arr[left];
			arr[left] = arr[right];
			arr[right] = temp;
			left++;
			right--;
		}
	} while (left <= right);
	if (L < right)
		quickSort(arr, L, right);

	if (left < R)
		quickSort(arr, left, R);
}

// 어플리케이션 메인 GUI출력 함수
void printMenu() {
	puts("\n## GrayScale Image Processing ##");
	puts("0.열기  1.저장  9.종료");
	puts("A.화소 점 처리  B.화소 영역 처리  C.기하학 처리  D.히스토그램 처리 E.엣지 검출 처리 ");
	puts("+.현재 작업중인 영상확인 *.효과누적하기");
}

// 화소 점 처리 메뉴 GUI출력 함수
void printMenu1() {
	puts("\n##   화소 점 처리   ##");
	puts("-.이전페이지 9.종료");
	puts("1.동일 2.밝게 3.어둡게 4.반전 5.흑백(128값) 6.감마보정 7.파라볼라보정CUP 8.파라볼라CAP보정");
	puts("A.흑백(평균값)  B.흑백(중앙값) C.스트래치 D.압축 E.포스터라이징 F.AND G.OR H.XOR I.픽셀 값 범위 강조");
	puts("+.현재 작업중인 영상확인 *.효과누적하기");
}

// 화소 영역 처리 메뉴 GUI출력 함수
void printMenu2() {
	puts("\n##  화소 영역 처리   ##");
	puts("-.이전페이지 9.종료");
	puts("1.엠보싱 2.블러링 3.블러링(9x9) 4.샤프닝(고주파필터) 5.언샤프 마스크 6.가우시안 필터링 7.블러링(마스크크기 입력)");
	puts("+.현재 작업중인 영상확인 *.효과누적하기");
}

// 기하학 처리 메뉴GUI출력 함수
void printMenu3() {
	puts("\n##  기하학 처리  ##");
	puts("-.이전페이지 9.종료");
	puts("1.축소 2.축소(평균값) 3.축소(중앙값) 4.확대(포워딩사상) 5.확대(백워딩사상) 6.확대(양선형 보간법 + 백워딩 사상적용 함수) ");
	puts("7.회전(포워딩사상) 8.회전(중앙+백워딩사상) A. 회전(확대) B.회전(양선형 보간법 + 확대) C.이미지 이동 D.이미지 대칭 E. 이미지 모핑 ");
	puts("+.현재 작업중인 영상확인 *.효과누적하기");

}

// 히스토그램 메뉴 GUI출력 함수
void printMenu4() {
	puts("\n## 히스토그램 처리  ##");
	puts("-.이전페이지 9.종료");
	puts("1.엔드 인 히스토그램 처리 2.히스토그램 평활화 3.히스토그램 스트래칭 4.히스토그램 명세화");
	puts("+.현재 작업중인 영상확인 *.효과누적하기");
}

// 엣지 검출 처리 메뉴 GUI출력 함수
void printMenu5() {
	puts("\n## 엣지 검출 처리  ##");
	puts("-.이전페이지 9.종료");
	puts("1.엣지 수직검출 2.엣지 수평검출 3.엣지 유사연산자검출 4.엣지 차 연산자 검출 5.1차 미분 검출 6.2차 미분 라플라시안 검출  ");
	puts("7.LoG연산자 검출 8.DoG연산자 검출 ");
	puts("+.현재 작업중인 영상확인 *.효과누적하기 ");
}

//이미지 출력 함수
void printImage() {
	freeTmpMemory();
	nowH = inH;
	nowW = inW;
	mallocTmpMemory();
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpImage[i][k] = inImage[i][k];
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 50, i + 250, RGB(px, px, px));
		}
	}
}

// 이미지 로드 함수
void loadImage() {
	char fullName[200] = "C:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("파일명-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);
	int nResult = _access(fileName, 0);
	if (nResult == 0)
	{
		printf("지정한 경로에 파일⁄폴더가 존재 합니다.");
	}
	else if (nResult == -1)
	{
		printf("지정한 경로에 파일⁄폴더가 존재하지 않습니다.");
		return;
	}
	// 입력 영상 폭과 높이 구하기
	rfp = fopen(fileName, "rb");
	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);
	// 메모리 해제
	freeInputMemory();
	// 입력 영상 폭과 높이 구하기
	inH = inW = (int)sqrt(fsize);
	// 메모리 할당
	mallocInputMemory();

	// 파일 --> 메모리
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	// printf("%d ", inImage[100][100]);

	equalImage();
}

// 모핑 이미지 로드 함수
void moping_load() {
	char fullName[200] = "C:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("파일명-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(mopfileName, fullName);
	int nResult = _access(fileName, 0);
	if (nResult == 0)
	{
		printf("지정한 경로에 파일⁄폴더가 존재 합니다.");
	}
	else if (nResult == -1)
	{
		printf("지정한 경로에 파일⁄폴더가 존재하지 않습니다.");
		return;
	}
	// 입력 영상 폭과 높이 구하기
	rfp = fopen(mopfileName, "rb");
	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);
	// 메모리 해제
	freeMopMemory();
	// 입력 영상 폭과 높이 구하기
	mopH = mopW = (int)sqrt(fsize);
	// 메모리 할당
	mallocMopMemory();

	// 파일 --> 메모리
	rfp = fopen(mopfileName, "rb");
	for (int i = 0; i < mopH; i++)
		fread(mopImage[i], sizeof(unsigned char), mopW, rfp);
	fclose(rfp);

	// printf("%d ", mopImage[100][100]);

	equalmopImage();

}
void saveImage() {
	// 이미지 저장
	if (outImage == NULL)
		return;
	char fullName[200] = "C:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("파일명-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");
	// 메모리 --> 파일 (한행씩)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
	// MessageBox(hwnd, L"저장 완료", L"저장 창", NULL);
	printf("저장 완료");
}

// 정수값 입력 함수
int getIntValue() {
	int retValue;
	printf("정수값-->"); scanf("%d", &retValue);
	return retValue;
}

// 효과누적하기 구현 함수
void effectpile() {
	if (inImage == NULL || outImage == NULL) {
		printf("\n처리된 영상이 없습니다. 다시 확인해주십시오.");
		return;
	}
	freeInputMemory();
	inH = outH;
	inW = outW;
	mallocInputMemory();
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			inImage[i][k] = outImage[i][k];
}

// 입력 영상 이미지 해제 함수
void freeInputMemory() {
	if (inImage == NULL) {
		printf("\n입력된 영상이 없습니다. 다시 확인해주십시오.");
		return;
	}
	for (int i = 0; i < inH; i++)
		free(inImage[i]);
	free(inImage);
	inImage = NULL;
}

// 출력 영상 이미지 해제 함수
void freeOutputMemory() {
	freeTmpMemory();
	nowH = outH;
	nowW = outW;
	mallocTmpMemory();
	//현재 작업 이미지 업데이트
	if (outImage == NULL) {
		printf("\n처리된 영상이 없습니다. 다시 확인해주십시오.");
		return;
	}
	for (int i = 0; i < outH; i++)
		free(outImage[i]);
	free(outImage);
	outImage = NULL;
}

// 모핑 영상 이미지 해제 함수
void freeMopMemory() {
	if (mopImage == NULL) {
		printf("\n입력된 영상이 없습니다. 다시 확인해주십시오.");
		return;
	}
	for (int i = 0; i < mopH; i++)
		free(mopImage[i]);
	free(mopImage);
	mopImage = NULL;
}

// 작업 영상 이미지 해제 함수
void freeTmpMemory() {
	if (tmpImage == NULL) {
		printf("\n입력된 영상이 없습니다. 다시 확인해주십시오.");
		return;
	}
	for (int i = 0; i < nowH; i++)
		free(tmpImage[i]);
	free(tmpImage);
	tmpImage = NULL;
}

// 임시 메모리 할당 해제 함수
void freeDoubleMemory(double** Memorry, int h) {
	if (Memorry == NULL) {
		printf("\n입력된 영상이 없습니다. 다시 확인해주십시오.");
		return;
	}
	for (int i = 0; i < h; i++)
		free(Memorry[i]);
	free(Memorry);
	Memorry = NULL;
}

// 입력 영상 이미지 동적 할당(로드) 함수
void mallocInputMemory() {
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
	for (int i = 0; i < inH; i++)
		inImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
}

// 출력 영상 이미지 동적 할당(로드) 함수
void mallocOutputMemory() {
	outImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);
	for (int i = 0; i < outH; i++)
		outImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);
}

// 모핑 영상 이미지 동적 할당(로드) 함수
void mallocMopMemory() {
	mopImage = (unsigned char**)malloc(sizeof(unsigned char*) * mopH);
	for (int i = 0; i < mopH; i++)
		mopImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * mopW);
}

// 누적된 이미지 동적 할당 함수
void mallocTmpMemory() {
	tmpImage = (unsigned char**)malloc(sizeof(unsigned char*) * nowH);
	for (int i = 0; i < nowH; i++)
		tmpImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * nowW);
}

// 임시 메모리 동적 할당 함수
double** mallocDoubleMemory(int h, int w) {
	double** retMemorry;
	retMemorry = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		retMemorry[i] = (double*)malloc(sizeof(double) * w);
	return retMemorry;
}

//////////////////
//  영상처리 함수
////////////////////

// 동일 이미지 출력 함수
void equalImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

// 모핑 이미지 출력 함수
void equalmopImage() {
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = mopH;
	outW = mopW;
	// 메모리 할당
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = mopImage[i][k];
		}
	}
	printImage();
}

// 영상 밝게하기 알고리즘 함수
void addImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	int val = getIntValue();
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
		}
	}

	printImage();
}

// 영상 어둡게하기 알고리즘 함수
void subImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	int val = getIntValue();
	// 이미지 입력된 값만큼 어둡게
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] - val >= 0)
				outImage[i][k] = inImage[i][k] - val;
			else
				outImage[i][k] = 0;
		}
	}

	printImage();
}

// 영상 반전처리 함수
void reverseImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			outImage[i][k] = 255 - inImage[i][k];
	printImage();
}

// 흑백 알고리즘(128기준) 함수
void black_whiteImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	int med = 128;
	// 이미지 흑백처리(128값기준)
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (med <= inImage[i][k])
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}

	printImage();
}

// 감마 알고리즘 함수
void gamaImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	double gam = 0;
	printf("감마값을 입력해주십시오(0.2~1.8사이)");
	scanf("%lf", &gam);
	// 이미지 감마보정
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (unsigned char)255.0 * pow(((double)inImage[i][k] / 255.0), gam);
		}
	}
	printImage();
}
// 파라볼라CUP 알고리즘 함수
void para_cupImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 이미지 파라볼라 처리
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			double val = 255.0 - 255.0 * pow(inImage[i][k] / 127.0 - 1.0, 2.0);
			if (val > 255.0) val = 255.0;
			else if (val < 0.0) val = 0.0;
			outImage[i][k] = (unsigned char)val;
		}
	}
	printImage();
}

// 파라볼라CAP 알고리즘 함수
void para_capImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 이미지 파라볼라 처리
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			double val = 255.f * pow(inImage[i][k] / 127.0 - 1.0, 2.0);
			if (val > 255.0) val = 255.0;
			else if (val < 0.0) val = 0.0;
			outImage[i][k] = (unsigned char)val;
		}
	}
	printImage();
}

// 흑백 알고리즘(평균값 기준) 함수
void black_whiteImage_med() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 이미지 흑백처리(평균값 기준)
	int med;
	med = medvalueImage();

	// (3-4) 이미지 흑백처리
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (med <= inImage[i][k])
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}

// 흑백 알고리즘(중앙값 기준) 함수
void black_whiteImage_middlle() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 이미지 흑백처리(중앙값기준)
	unsigned char med = 0; // 필터에서 나온 중앙값
	unsigned char* onescale; // 2차원 배열을 오름차순 정렬을 위해 1차원 배열에 할당. 이후 이를 정렬
	int count = 0;
	onescale = (unsigned char*)malloc(sizeof(unsigned char) * (inH * inW)); // 1차원 동적 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			onescale[count] = inImage[i][k];
			count++;
		}

	}
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			onescale[inImage[i][k]]++;
	// 오름차순 정렬을 위해 퀵정렬
	quickSort(onescale, 0, (inH * inW) - 1);
	med = onescale[inH * inW / 2];
	printf("중앙값 : %d \n", med);
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (med <= inImage[i][k])
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}
	free(onescale);
	printImage();

}

// 스트래치 알고리즘 함수
void strechImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	double strach = 0;
	printf("스트래치값을 입력해주십시오");
	scanf("%lf", &strach);
	// 이미지 스트래지(입력된 계수만큼 스트래치)
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (strach * inImage[i][k] < 255)
				outImage[i][k] = (unsigned char)(strach * inImage[i][k]);
			else
				outImage[i][k] = 255;
		}
	}

	printImage();
}

// 압축 알고리즘 함수
void pressImage() {
	// 메모리 할당
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	double press = 0;
	printf("압축값을 입력해주십시오");
	scanf("%lf", &press);
	// 이미지 압축처리(입력된 계수만큼 압축)
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (press * inImage[i][k] > 0)
				outImage[i][k] = (unsigned char)(inImage[i][k] / press);
			else
				outImage[i][k] = 0;
		}
	}

	printImage();
}

// 포스터라이징 처리 함수
void posterImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	int level = getIntValue();
	// 이미지 포스터라이징(입력된 값을 라이징 값으로 입력받음)
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			for (int m = 1; m <= level; m++)
			{
				if (inImage[i][k] - ((255 / level) * m) <= 0)
				{
					if (m == 1)
					{
						outImage[i][k] = 0;
					}
					else
					{
						outImage[i][k] = ((255 / level) * (m));
						break;
					}
				}
			}
		}
	}
	printImage();
}

// AND 처리 함수
void andImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	printf("0~255사이의 정수값을 입력해주십시오.\n");
	int val = getIntValue();

	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (unsigned char)(inImage[i][k] & val);

		}
	}

	printImage();

}

// OR 처리 함수
void orImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	printf("0~255사이의 정수값을 입력해주십시오.\n");
	int val = getIntValue();

	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			outImage[i][k] = (unsigned char)(inImage[i][k] | val);
	printImage();
}

// XOR 처리 함수
void xorImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	printf("0~255사이의 정수값을 입력해주십시오.\n");
	int val = getIntValue();

	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			outImage[i][k] = (unsigned char)(inImage[i][k] ^ val);

	printImage();

}

// 범위 강조 함수
void impressImage()
{
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	int impress_st = 0, impress_ed = 255;
	printf("\n강조할 범위의 시작값을 입력해주십시오");
	scanf("%d", &impress_st);
	printf("\n강조할 범위의 끝값을 입력해주십시오");
	scanf("%d", &impress_ed);
	// 이미지 압축처리(입력된 계수만큼 압축)
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] >= impress_st && inImage[i][k] <= impress_ed)
				outImage[i][k] = 255;
			else
				outImage[i][k] = inImage[i][k];
		}
	}

	printImage();

}

// 축소 알고리즘(포워딩사상) 함수
void zumout() {
	// 스케일 입력
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}
void zumin() {
	// 확대 알고리즘(포워딩 사상)
	// 스케일 입력
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k];
		}
	}
	printImage();
}

// 확대 알고리즘(백워딩 사상) 함수
void zumin2() {
	// 스케일 입력
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = inImage[(int)(i / scale)][(int)(k / scale)];
		}
	}
	printImage();
}

// 확대 + 양선형 보간법 + 백워딩 사상적용 함수
void zumIn3()
{
	double scale = (double)getIntValue();
	// 메모리 해제

	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당
	mallocOutputMemory();
	int i_H, i_W;
	unsigned char newValue;
	double r_H, r_W, s_H, s_W;
	double C1, C2, C3, C4;
	// 입력 배열 --> 출력 배열

	for (int i = 0; i < outH; i++) {
		for (int j = 0; j < outW; j++) {
			r_H = i / scale;
			r_W = j / scale;
			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);
			s_H = r_H - i_H;
			s_W = r_W - i_W;
			if (i_H < 0 || i_H >= (inH - 1) || i_W < 0
				|| i_W >= (inW - 1))
			{
				outImage[i][j] = 255;
			}
			else
			{
				C1 = (double)inImage[i_H][i_W];
				C2 = (double)inImage[i_H][i_W + 1];
				C3 = (double)inImage[i_H + 1][i_W + 1];
				C4 = (double)inImage[i_H + 1][i_W];
				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[i][j] = newValue;
			}
		}
	}
	printImage();
}

// 회전 알고리즘 (포워딩  사상) 함수`
void rotate() { // 회전 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	int degree = getIntValue();
	double radian = degree * 3.141592 / 180.0;
	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			int xs = i;
			int ys = k;
			int xd = (int)(cos(radian) * xs - sin(radian) * ys);
			int yd = (int)(sin(radian) * xs + cos(radian) * ys);

			if ((0 <= xd && xd < outH) && (0 <= yd && yd < outW))
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

// 회전 알고리즘 + 중앙/백워딩 함수
void rotate2() {
	// 메모리 해제
	int degree = getIntValue();
	double radian = degree * M_PI / 180;
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// xd = cos*(xs) - sin*(ys)
	// yd = sin*(xs) + cos*(ys)
	// 입력 배열 --> 출력 배열

	// 입력 영상의 각 픽셀을 중앙으로 시킨후 회전을 시키기 위한 중앙 위치 값
	int cx = inH / 2;
	int cy = inW / 2;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cy));
			int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cy));
			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < outH) && (0 <= ys && ys < outW))
				outImage[xd][yd] = inImage[xd][yd];

		}
	}
	printImage();
}

// 회전 알고리즘 + 확대 함수
void rotate3() {
	// 메모리 해제
	// 메모리 해제
	int degree = getIntValue();
	double radian = degree * M_PI / 180;

	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outW = (int)(inH * cos((double)((double)(90 - degree) * M_PI / 180.0)) + inW * cos(radian));
	outH = (int)(inW * cos((double)((double)(90 - degree) * M_PI / 180.0)) + inH * cos(radian));
	// 메모리 할당
	mallocOutputMemory();
	// xd = cos*(xs) - sin*(ys)
	// yd = sin*(xs) + cos*(ys)
	// 입력 배열 --> 출력 배열

	// 입력 영상을 중앙으로 이동시키기 위한 값
	int cx = inH / 2;
	int cy = inW / 2;

	// 중앙을 기준으로 실제 출력 영상에서 위치를 맞춰주기 위한 값
	int cx2 = outH / 2;
	int cy2 = outW / 2;
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - cx2) + sin(radian) * (yd - cy2));
			int ys = (int)(-sin(radian) * (xd - cx2) + cos(radian) * (yd - cy2));

			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW))
				outImage[xd][yd] = inImage[xs][ys];
			else
				outImage[xd][yd] = 255;
		}
	}
	printImage();
}
// 회전 + 양선형 보간법 + 확대
void rotate4() {
	// 메모리 해제
		// 메모리 해제
	int degree = getIntValue();
	double radian = degree * M_PI / 180;

	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outW = (int)(inH * cos((double)((double)(90 - degree) * M_PI / 180.0)) + inW * cos(radian));
	outH = (int)(inW * cos((double)((double)(90 - degree) * M_PI / 180.0)) + inH * cos(radian));
	double scale_W = (double)(outW / inW);
	double scale_H = (double)(outH / inH);
	// 메모리 할당
	mallocOutputMemory();
	// xd = cos*(xs) - sin*(ys)
	// yd = sin*(xs) + cos*(ys)
	// 입력 배열 --> 출력 배열

	// 입력 영상을 중앙으로 이동시키기 위한 값
	int cx = inH / 2;
	int cy = inW / 2;

	// 중앙을 기준으로 실제 출력 영상에서 위치를 맞춰주기 위한 값
	int cx2 = outH / 2;
	int cy2 = outW / 2;

	int i_H, i_W;
	unsigned char newValue;
	double r_H, r_W, s_H, s_W;
	double C1, C2, C3, C4;

	int xs1 = 0, xs2 = 0, xs3 = 0, xs4 = 0, ys1 = 0, ys2 = 0, ys3 = 0, ys4 = 0;
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {

			r_H = i / scale_H;
			r_W = k / scale_W;
			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);
			s_H = r_H - i_H;
			s_W = r_W - i_W;
			if (i_H < 0 || i_H >= (inH - 1) || i_W < 0
				|| i_W >= (inW - 1))
			{
				outImage[i][k] = 255;
			}
			int xd = i;
			int yd = k;

			xs1 = (int)(cos(radian) * (xd - cx2) + sin(radian) * (yd - cy2));
			ys1 = (int)(-sin(radian) * (xd - cx2) + cos(radian) * (yd - cy2));

			xs2 = (int)(cos(radian) * (xd + 1 - cx2) + sin(radian) * (yd - cy2));
			ys2 = (int)(-sin(radian) * (xd + 1 - cx2) + cos(radian) * (yd - cy2));

			xs3 = (int)(cos(radian) * (xd - cx2) + sin(radian) * (yd + 1 - cy2));
			ys3 = (int)(-sin(radian) * (xd - cx2) + cos(radian) * (yd + 1 - cy2));

			xs4 = (int)(cos(radian) * (xd + 1 - cx2) + sin(radian) * (yd + 1 - cy2));
			ys4 = (int)(-sin(radian) * (xd + 1 - cx2) + cos(radian) * (yd + 1 - cy2));

			xs1 += cx;
			ys1 += cy;

			xs2 += cx;
			ys2 += cy;

			xs3 += cx;
			ys3 += cy;

			xs4 += cx;
			ys4 += cy;

			if ((0 <= xs1 && xs1 < inH) && (0 <= ys1 && ys1 < inW))
				C1 = (double)inImage[xs1][ys1];
			else
				C1 = 255;
			if ((0 <= xs2 && xs2 < inH) && (0 <= ys2 && ys2 < inW))
				C2 = (double)inImage[xs2][ys2];
			else
				C2 = 255;
			if ((0 <= xs3 && xs3 < inH) && (0 <= ys3 && ys3 < inW))
				C3 = (double)inImage[xs3][ys3];
			else
				C3 = 255;
			if ((0 <= xs4 && xs4 < inH) && (0 <= ys4 && ys4 < inW))
				C4 = (double)inImage[xs4][ys4];
			else
				C4 = 255;
			newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)
				+ C2 * s_W * (1 - s_H) + C4 * s_W * s_H + C3 * (1 - s_W) * s_H);
			outImage[i][k] = newValue;
		}
	}
	printImage();

}
// 축소 알고리즘 => 평균값 함수
void zumout2() {
	// 스케일 입력
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당
	mallocOutputMemory();
	int sum;
	int med;
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			sum = 0;
			med = 0;
			for (int a = i; a < i + scale; a++) {
				for (int b = k; b < k + scale; b++) {
					if ((0 <= a && a < inH) && (0 <= b && b < inW))
						sum += inImage[a][b];
				}
			}
			med = (int)(sum / (scale * scale));
			outImage[(int)(i / scale)][(int)(k / scale)] = (unsigned char)med;

			//printf("실패 \n");
		}
		//outImage[(int)(i / scale)][(int)(k / scale)] = 255;
	}

	printImage();
}

//축소 알고리즘 = > 중앙값 함수
void zumout3() {
	// 스케일 입력
	int sum = 0;

	int med = 0;
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			int temp_index = 0;
			unsigned char* temp;
			temp = (unsigned char*)malloc(sizeof(unsigned char) * (int)pow(scale, 2));
			temp_index = 0;
			for (int a = i; a < scale + i; a++) {
				for (int b = k; b < scale + k; b++) {
					if ((0 <= a && a < inH) && (0 <= b && b < inW)) {
						temp[temp_index] = inImage[a][b];
						temp_index++;
					}
				}
			}
			quickSort(temp, 0, (int)pow(scale, 2) - 1);
			outImage[(int)(i / scale)][(int)(k / scale)] = temp[(int)(pow(scale, 2) / 2)];
			free(temp);
		}
	}
	printImage();
}

// 이미지 이동 함수
void moveImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	printf("\n왼쪽으로 이동할 값을 입력해주십시오");
	int left = getIntValue();
	printf("\n오른쪽으로 이동할 값을 입력해주십시오");
	int right = getIntValue();
	printf("\n위로 이동할 값을 입력해주십시오");
	int up = getIntValue();
	printf("\n아래로 이동할 값을 입력해주십시오");
	int down = getIntValue();
	for (int i = 0; i < inH; i++) {
		int move_updown = i + up - down;
		for (int k = 0; k < inW; k++) {
			int move_side = k + left - right;
			if ((0 <= move_updown && move_updown < inH) && (0 <= move_side && move_side < inW))
				outImage[i][k] = inImage[move_updown][move_side];
		}
	}
	printImage();
}

// 이미지 대칭 함수
void switchImage() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	char inKey = 0;
	while (inKey != '9') {
		puts("\n어떤 방향으로 대칭을 할 것입니까?(1 : 좌우 2 : 위아래 3 : 좌우위아래 9 : 종료)");
		inKey = _getch();

		switch (inKey) {
		case '1':
			for (int i = 0; i < inH; i++)
				for (int k = 0; k < inW; k++)
					outImage[i][k] = inImage[i][inW - (k + 1)];
			printImage();
			break;
		case'2':
			for (int i = 0; i < inH; i++)
				for (int k = 0; k < inW; k++)
					outImage[i][k] = inImage[inH - (i + 1)][k];
			printImage();
			break;
		case'3':
			for (int i = 0; i < inH; i++)
				for (int k = 0; k < inW; k++)
					outImage[i][k] = inImage[inH - (i + 1)][inW - (k + 1)];
			printImage();
			break;
		case'9':
			break;
		}
	}
}

// 이미지 모핑
void moping() {
	// 모핑할 이미지 로드
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	moping_load();
	// 메모리 해제
	freeOutputMemory();
	int cx, cy, cx2, cy2;
	int s_H, s_W;
	int insmall = 1;
	if (inH >= mopH) {
		cx = cy = inH / 2;
		outH = outW = inH;
		cx2 = cy2 = mopH / 2;
		insmall = 0;
	}
	else {
		cx = cy = mopH / 2;
		outH = outW = mopH;
		cx2 = cy2 = inH / 2;
	}
	// 메모리 할당
	mallocOutputMemory();
	double mopimageper;
	printf("입력 영상에 모핑 영상을 넣을 비율을 입력해주십시오.");
	scanf("%lf", &mopimageper);
	// 입력 배열 --> 출력 배열
	int xs = 0, ys = 0;

	double value = 0;
	int outv = 0;
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			xs = (int)((double)cx - cx2);
			ys = (int)((double)cy - cx2);

			if (insmall == 1) {
				if ((xs <= xd && xd < xs + inH) && (ys <= yd && yd < ys + inW)) {
					value = fabs((double)(inImage[xd - xs][yd - ys] - mopImage[xd][yd]));
					if (inImage[xd - xs][yd - ys] <= mopImage[xd][yd])
						outv = (int)((double)mopImage[xd][yd] - value * (1.0 - (mopimageper / 100.0)));
					else
						outv = (int)((double)mopImage[xd][yd] + value * (1.0 - (mopimageper / 100.0)));
				}
				else {
					outv = mopImage[xd][yd];
				}
			}
			else {
				if ((xs <= xd && xd < xs + mopH) && (ys <= yd && yd < ys + mopW)) {
					value = fabs((double)(inImage[xd][yd] - mopImage[xd - xs][yd - ys]));
					if (inImage[xd][yd] <= mopImage[xd - xs][yd - ys])
						outv = (int)((double)mopImage[xd - xs][yd - ys] - value * (1.0 - (mopimageper / 100.0)));
					else
						outv = (int)((double)mopImage[xd - xs][yd - ys] + value * (1.0 - (mopimageper / 100.0)));
				}
				else {
					outv = inImage[xd][yd];
				}
			}
			if (outv > 255)
				outv = 255;
			if (outv < 0)
				outv = 0;
			outImage[xd][yd] = outv;
		}
	}
	printImage();

}

// 히스토그램 스트래칭 처리 함수
void histoStrech() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 진짜 영상처리 알고리즘
	// new = (old- low) / (high - low) * 255.0
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < low)
				low = inImage[i][k];
			if (inImage[i][k] > high)
				high = inImage[i][k];
		}
	}
	int old, new;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)((double)(old - low) / (double)(high - low) * 255.0);
			if (new > 255)
				new = 255;
			if (new < 0)
				new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

// 앤드 인 탐색 알고리즘
void endin() {
	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 진짜 영상처리 알고리즘
	// new = (old- low) / (high - low) * 255.0
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < low)
				low = inImage[i][k];
			if (inImage[i][k] > high)
				high = inImage[i][k];
		}
	}
	high -= 50;
	low += 50;
	int old, new;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)((double)(old - low) / (double)(high - low) * 255.0);
			if (new > 255)
				new = 255;
			if (new < 0)
				new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

// 히스토그램 평활화
void histoequal() {

	// 메모리 해제
	freeOutputMemory();
	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();
	// 1단계 빈도수 세기(=히스토그램)
	int histo[256] = { 0, };

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			histo[inImage[i][k]]++;
		}
	}

	// 2단계 누적히스토그램 생성
	int sumHisto[256] = { 0, };

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++) {
		sumHisto[i] = sumHisto[i - 1] + histo[i];
	}

	// 3단계 정규화된 히스토그램 생성 nomalHisto = sumHist * (1.0/(inH*inW)) * 255.0)
	double normalHisto[256] = { 1.0, };
	for (int i = 0; i < 256; i++) {
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0;

	}
	// 4단계 inImage를 정규화된 값으로 치환
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char)normalHisto[inImage[i][k]];
		}
	}
	printImage();
}

// 히스토그램 명세화 함수
void histoSpecification() {
	// 메모리 해제
	freeOutputMemory();

	// 명세화작업을 할 이미지 로드
	moping_load();

	// 중요! 출력 이미지의 크기를 결정 --> 알고리즘에 의존!
	outH = inH;
	outW = inW;

	// 메모리 할당
	mallocOutputMemory();

	// 1단계 빈도수 세기(=히스토그램)
	int histo[256] = { 0, }, histo2[256] = { 0, };

	// 명세화 작업을 하기 위한 역명세화 히스토그램 생성
	int reverse_histo[256] = { 0, }, reverse_histo[256] = { 0, };

	// 역 명세화 작업을 위한 변수 설정
	int top = 255, bottom = top - 1;

	//DMIN = m_Sum_Of_DHIST[0];
	//DMAX = m_Sum_Of_DHIST[255];
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			histo[inImage[i][k]]++;
		}
	}
	for (int i = 0; i < mopH; i++) {
		for (int k = 0; k < mopW; k++) {
			histo2[mopImage[i][k]]++;
		}
	}
	// 2단계 누적히스토그램 생성
	int sumHisto[256] = { 0, }, sumHisto2[256] = { 0, };
	sumHisto[0] = histo[0];
	sumHisto2[0] = histo2[0];
	for (int i = 1; i < 256; i++) {
		sumHisto[i] = sumHisto[i - 1] + histo[i];
		sumHisto2[i] = sumHisto2[i - 1] + histo2[i];
	}

	// 3단계 정규화된 히스토그램 생성 
	double** normalHisto = (double**)malloc(sizeof(double*) * inH);
	for (int i = 0; i < inH; i++)
		inImage[i] = (double*)malloc(sizeof(double) * inW);
	double normalHisto2[256] = { 1.0, };

	int DMIN = sumHisto[0];
	int DMAX = sumHisto[255];

	int DMIN2 = sumHisto2[0];
	int DMAX2 = sumHisto2[255];
	// 원본 영상 평활화
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			normalHisto[i][k] = (double)(sumHisto[inImage[i][k]] * inH / (inH * inW));
		}
	}
	// 원하는 영상을 평활화
	for (int i = 0; i < 256; i++) {
		normalHisto2[i] = (unsigned char)((sumHisto2[i] - DMIN2) * mopH / (DMAX2 - DMIN2));
	}
	// 룩업테이블을 이용한 명세화
	while (1) {
		for (int i = (int)normalHisto2[bottom]; i <= (int)normalHisto2[top]; i++) {
			reverse_histo[i] = top;
		}
		top = bottom;
		bottom = bottom - 1;
		if (bottom <= -1)
			break;
	}

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char)reverse_histo[(int)normalHisto[i][k]];
		}
	}

	printImage();
	for (int i = 0; i < inH; i++)
		free(normalHisto[i]);
	free(normalHisto);
	normalHisto = NULL;

}

// 엠보싱 함수
void emboss() { // 화소영역 처리 : 엠보싱 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {-1.0, 0.0, 0.0}, // 엠보싱 마스크
						  { 0.0, 0.0, 0.0},
						  { 0.0, 0.0, 1.0} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpoutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

//가우시안 필터링 함수
void gauss() { // 
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {1.0 / 16, 1.0 / 8, 1.0 / 16}, // 엠보싱 마스크
						  { 1.0 / 8, 1.0 / 4, 1.0 / 8},
						  { 1.0 / 16, 1.0 / 8, 1.0 / 16} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpoutImage[i][k] += 127.0 / 8;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

// 블러링 함수
void blur() { // 화소영역 처리 : 엠보싱 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {1.0 / 9, 1.0 / 9, 1.0 / 9}, // 엠보싱 마스크
						  { 1.0 / 9, 1.0 / 9, 1.0 / 9},
						  { 1.0 / 9, 1.0 / 9, 1.0 / 9} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += (tmpInImage[i + m][k + n] * mask[m][n]);

			tmpoutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

// 샤프닝 함수
void sharp() { // 화소영역 처리 : 엠보싱 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { 0, };
	double mask1[3][3] = { {-1.0, -1.0, -1.0}, // 샤프 마스크2
					  { -1.0, 9.0, -1.0},
					  { -1.0, -1.0, -1.0} };

	double mask2[3][3] = { {0.0, -1.0, 0.0}, // 샤프 마스크1 
						  { -1.0, 5.0, -1.0},
						  { 0.0, -1.0, 0.0} };
	int checkmask = 0;
	printf("샤프닝 마스크를 선택해주십시오. (1 or 2)");
	scanf(" %d", &checkmask);
	if (checkmask != 1 && checkmask != 2) {
		printf("잘못된 입력입니다!");
		return;
	}
	if (checkmask == 1) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++)
				mask[i][k] = mask1[i][k];
	}
	else if (checkmask == 2) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++)
				mask[i][k] = mask2[i][k];
	}
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}
void blur9() { // 화소영역 처리 : 엠보싱 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[9][9] = { {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81}, // 엠보싱 마스크
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81},
						  {1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,1.0 / 81, 1.0 / 81, 1.0 / 81} , };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 8, inW + 8);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 8; i++)
		for (int k = 0; k < inW + 8; k++) {
			tmpInImage[i][k] = 127;
		}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 4][k + 4] = inImage[i][k];
	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {


			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 9; m++)
				for (int n = 0; n < 9; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];
			tmpoutImage[i][k] = S;
		}
	}

	// 후처리 (마스크 값의 합계에 따라서...)
	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 8);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

// 입력된 마스크로 블러처리
void blurmask() { // 화소영역 처리 : 엠보싱 알고리즘
	// 메모리 해제
	int scale = 0;
	printf("마스크의 크기를 입력해주십시오.");
	scanf(" %d", &scale);
	if (scale % 2 == 0)
		scale++;
	double** mask = mallocDoubleMemory(scale, scale);

	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	for (int i = 0; i < scale; i++)
		for (int k = 0; k < scale; k++) {
			mask[i][k] = 1.0 / pow(scale, 2.0);
		}
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + (scale - 1), inW + (scale - 1));
	double** tmpoutImage = mallocDoubleMemory(outH, outW);


	for (int i = 0; i < inH + (scale - 1); i++)
		for (int k = 0; k < inW + (scale - 1); k++) {
			tmpInImage[i][k] = 127;
		}

	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + (int)(scale / 2)][k + (int)(scale / 2)] = inImage[i][k];

	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			S = 0.0; // 마스크와 입력값을 각각 곱해서 합한 값.
			for (int m = 0; m < scale; m++)
				for (int n = 0; n < scale; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];
			tmpoutImage[i][k] = S;
		}
	}

	// 후처리 (마스크 값의 합계에 따라서...)
	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + (scale - 1));
	freeDoubleMemory(tmpoutImage, outH);
	freeDoubleMemory(mask, scale);
	printImage();
}


void sharp_high()
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {-1.0, -1.0, -1.0 }, // 샤프 마스크2
					  { -1.0 , 8.0 , -1.0 },
					  { -1.0 , -1.0 , -1.0 } };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 127.0;

	 //임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}


void unsharp()
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	double alpha;
	printf("출력 영상에 알파값을 입력해주십시오.(1이면 언샤프 마스킹, 1보다 크면 하이부스트 필터링)");
	scanf("%lf", &alpha);
	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {1.0 / 9 , 1.0 / 9, 1.0 / 9 }, // 샤프 마스크2
					  { 1.0 / 9, 1.0 / 9, 1.0 / 9},
					  { 1.0 / 9, 1.0 / 9 , 1.0 / 9} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = inImage[i][k] + alpha * ((double)inImage[i][k] - S);
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 127.0;

	//임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

// 엣지 검출 처리 함수 정의부

// 수직 엣지 검출 함수
void edgeVertical()
{	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {0.0, 0.0, 0.0}, // 수직 에지 마스크
					  { -1.0, 1.0, 0.0},
					  { 0.0, 0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpinImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double s;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			s = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					s += tmpinImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = s;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpoutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

// 수평 엣지 검출 함수
void edgeHorizontal() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {0.0, -1.0, 0.0}, // 수평 에지 마스크
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpinImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double s;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			s = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					s += tmpinImage[i + m][k + n] * mask[m][n];

			tmpoutImage[i][k] = s;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpoutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}


// 엣지 검출 유사연산자 함수
void edgeSame() {
	// 마스크 스케일 입력
	printf("마스크 크기를 입력해주십시오.");
	int scale = getIntValue();
	if (scale % 2 == 0)
		scale++;
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	//double mask[3][3] = { {0.0, -1.0, 0.0}, // 수평 에지 마스크
	//					  { 0.0, 1.0, 0.0},
	//					  { 0.0, 0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + scale - 1, inW + scale - 1);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);
	// 임시 입력 메모리를 초기화(127) : 필요시 평균값

	for (int i = 0; i < inH + scale - 1; i++)
		for (int k = 0; k < inW + scale - 1; k++)
			tmpinImage[i][k] = 127.0;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + (int)((int)(scale / 2))][k + (int)((int)(scale / 2))] = (double)inImage[i][k];
	// *** 회선 연산 ***
	double s;
	int outValueCount = 0;
	double maxout = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			maxout = 0.0;
			// 마스크 와 한점을 중심으로한 3x3을 곱하기
			for (int m = 0; m < (int)((int)(scale / 2)); m++) {
				for (int n = 0; n < (int)((int)(scale / 2)); n++) {
					//printf("%lf ", maxout);
					if (maxout <= fabs(tmpinImage[i + (int)(scale / 2)][k + (int)(scale / 2)] - tmpinImage[i + m][k + n]))
						maxout = fabs(tmpinImage[i + (int)(scale / 2)][k + (int)(scale / 2)] - tmpinImage[i + m][k + n]);
				}
			}
			tmpoutImage[i][k] = maxout;
		}
	}
	//// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}

	freeDoubleMemory(tmpinImage, inH + scale - 1);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();


}

// 엣지 차 연산자 검출 함수
void edgesub() {
	// 마스크 스케일 입력
	printf("마스크 크기를 입력해주십시오.");
	int scale = getIntValue();
	if (scale % 2 == 0)
		scale++;
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	////////////////
	// 화소 영역 처리
	/////////////////
	//double mask[3][3] = { {0.0, -1.0, 0.0}, // 수평 에지 마스크
	//					  { 0.0, 1.0, 0.0},
	//					  { 0.0, 0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + scale - 1, inW + scale - 1);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);
	// 임시 입력 메모리를 초기화(127) : 필요시 평균값

	for (int i = 0; i < inH + scale - 1; i++)
		for (int k = 0; k < inW + scale - 1; k++)
			tmpinImage[i][k] = 127.0;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + (int)((int)(scale / 2))][k + (int)((int)(scale / 2))] = (double)inImage[i][k];
	// *** 회선 연산 ***
	double s;
	int outValueCount = 0;
	double maxout = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			maxout = 0.0;
			// 마스크 와 한점을 중심으로한 3x3을 곱하기
			for (int m = 0; m < (int)((int)((int)(scale / 2))); m++) {
				for (int n = 0; n < (int)((int)(scale / 2)); n++) {
					//printf("%lf ", maxout);
					if (maxout <= fabs(tmpinImage[i + m][k + n] - tmpinImage[i + (scale - 1) - m][k + (scale - 1) - n]))
						maxout = fabs(tmpinImage[i + m][k + n] - tmpinImage[i + (scale - 1) - m][k + (scale - 1) - n]);
				}
			}
			tmpoutImage[i][k] = maxout;
		}
	}
	//// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + scale - 1);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}


// 1차 미분 회선 마스크 함수
void edgeFirstderivative() {
	// 마스크 스케일 입력
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	////////////////
	// 화소 영역 처리
	/////////////////
	double maskRobert_1[3][3] = { {-1.0, 0.0, 0.0}, // 로버츠 행 검출 마스크
					  { 0.0, 1.0, 0.0},
					  { 0.0, 0.0, 0.0} };

	double maskRobert_2[3][3] = { {0.0, 0.0, -1.0},  // 로버츠 열 검출 마스크 
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	double maskPrewitt_1[3][3] = { {-1.0, -1.0, -1.0},  // 프리윗 행 검출 마스크
					  { 0.0, 0.0, 0.0},
					  { 1.0, 1.0, 1.0} };

	double maskPrewitt_2[3][3] = { {1.0, 0.0, -1.0}, // 프리윗 열 검출 마스크
					  { 1.0, 0.0, -1.0},
					  {1.0, 0.0, -1.0} };
	double maskSobel_1[3][3] = { {-1.0, -2.0, -1.0}, // 소벨 행 검출 마스크 
						  { 0.0, 0.0, 0.0},
						  {1.0, 2.0, 1.0} };

	double maskSobel_2[3][3] = { {1.0, 0.0, -1.0}, // 소벨 열 검출 마스크 
						  { 2.0, 0.0, -2.0},
						  {1.0, 0.0, -1.0} };
	int checkmask = 0;
	int checkmask2 = 0;
	int checkmask3 = 0;
	double mask[3][3], mask2[3][3];

	printf("1차 미분 회선 마스크 종류중 선택해주십시오. (1 : 로버츠  2 : 프리윗  3 : 소벨 )");
	scanf(" %d", &checkmask);
	printf("1차 미분 회선 마스크의 행과 열중 선택해주십시오. (1 : 행 or 2 :열)");
	scanf(" %d", &checkmask2);
	printf("합성 출력을 하시겠습니까? (1 : 예 or 2 :아니요)");
	scanf(" %d", &checkmask3);

	if (checkmask == 1 && checkmask2 == 1) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++) {
				mask[i][k] = maskRobert_1[i][k];
				mask2[i][k] = maskRobert_2[i][k];
			}
	}
	if (checkmask == 1 && checkmask2 == 2) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++) {
				mask[i][k] = maskRobert_2[i][k];
				mask2[i][k] = maskRobert_1[i][k];
			}
	}
	if (checkmask == 2 && checkmask2 == 1) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++) {
				mask[i][k] = maskPrewitt_1[i][k];
				mask2[i][k] = maskPrewitt_2[i][k];
			}
	}
	if (checkmask == 2 && checkmask2 == 2) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++) {
				mask[i][k] = maskPrewitt_2[i][k];
				mask2[i][k] = maskPrewitt_1[i][k];
			}
	}
	if (checkmask == 3 && checkmask2 == 1) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++) {
				mask[i][k] = maskSobel_1[i][k];
				mask2[i][k] = maskSobel_2[i][k];
			}

	}
	if (checkmask == 3 && checkmask2 == 2) {
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				mask[i][k] = maskSobel_2[i][k];
				mask2[i][k] = maskSobel_1[i][k];
				printf("%lf ", mask[i][k]);
			}
			printf("\n");
		}
	}

	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);
	// 임시 입력 메모리를 초기화(127) : 필요시 평균값

	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpinImage[i][k] = 127.0;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + 1][k + 1] = (double)inImage[i][k];
	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++) {
					S += tmpinImage[i + m][k + n] * mask[m][n];
					if (checkmask3 == 1)
						S += tmpinImage[i + m][k + n] * mask2[m][n];
				}
			tmpoutImage[i][k] = S;
		}
	}
	//// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpoutImage[i][k] += 50.0;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}


// 2차 미분 라플라시안 회선 마스크 함수
void edgeSecondLap() {
	// 마스크 스케일 입력
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	////////////////
	// 화소 영역 처리
	/////////////////
	double mask1[3][3] = { {0.0, -1.0, 0.0},  // 라플라시안 마스크 1
						  { -1.0, 4.0, -1.0},
						  { 0.0, -1.0, 0.0} };

	double mask2[3][3] = { {1.0, 1.0, 1.0},  // 라플라시안 마스크 2
						  { 1.0, -8.0, 1.0},
						  { 1.0, 1.0, 1.0} };
	double mask3[3][3] = { {-1.0, -1.0, -1.0},  // 라플라시안 마스크 3 
						  { -1.0, 8.0, -1.0},
						  { -1.0, -1.0, -1.0} };
	int checkmask = 0;
	double mask[3][3];
	printf("2차 미분 라플라시안 마스크 종류중 선택해주십시오. \n\n1 :  { {0.0, -1.0, -1.0},\n{ -1.0, 4.0, -1.0},\n{ 0.0, -1.0, 0.0 } }\n\n2 : { {1.0, 1.0, 1.0},\n{ 1.0, -8.0, 1.0},\n{ 1.0, 1.0, 1.0 }}");
	printf("\n\n3 : { {-1.0, -1.0, -1.0},\n{ -1.0, 8.0, -1.0},\n{ -1.0, -1.0, -1.0 } }\n");

	scanf(" %d", &checkmask);
	if (checkmask != 1 && checkmask != 2 && checkmask != 3) {
		printf("잘못된 입력입니다!!");
		return;
	}
	if (checkmask == 1) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++)
				mask[i][k] = mask1[i][k];
	}
	if (checkmask == 2) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++)
				mask[i][k] = mask2[i][k];
	}
	if (checkmask == 3) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++)
				mask[i][k] = mask3[i][k];
	}

	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);
	// 임시 입력 메모리를 초기화(127) : 필요시 평균값

	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpinImage[i][k] = 127.0;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + 1][k + 1] = (double)inImage[i][k];
	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpinImage[i + m][k + n] * mask[m][n];
			tmpoutImage[i][k] = S;
		}
	}
	//// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 67.0;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + 2);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

// LoG 연산자 마스크 함수
void edgeLoG() {
	// 마스크 스케일 입력
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[5][5] = { {0.0, 0.0,-1.0, 0.0,0.0},  // LoG 검출 마스크 
							{0.0, -1.0,-2.0, -1.0,0.0},
							{-1.0, -2.0,16.0, -2.0,-1.0} ,
							{0.0, -1.0,-2.0,-1.0,0.0},
							{0.0,0.0,-1.0, 0.0,	0.0} };

	int checkmask = 0;
	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + 4, inW + 4);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);
	// 임시 입력 메모리를 초기화(127) : 필요시 평균값

	for (int i = 0; i < inH + 4; i++)
		for (int k = 0; k < inW + 4; k++)
			tmpinImage[i][k] = 127.0;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + 2][k + 2] = (double)inImage[i][k];
	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 5; m++)
				for (int n = 0; n < 5; n++)
					S += tmpinImage[i + m][k + n] * mask[m][n];
			tmpoutImage[i][k] = S;
		}
	}
	//// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 67.0;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + 4);
	freeDoubleMemory(tmpoutImage, outH);
	printImage();
}

void edgeDoG() {
	// 마스크 스케일 입력
// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	////////////////
	// 화소 영역 처리
	/////////////////
	double mask1[7][7] = { {0.0, 0.0,-1.0,-1.0,-1.0, 0.0,0.0},  // 로버츠 열 검출 마스크 
							{0.0, -2.0,-3.0,-3.0,-3.0, -2.0,0.0},
							{-1.0, -3.0,5.0,5.0,5.0, -3.0,-1.0},
							{-1.0, -3.0,5.0,16.0,5.0, -3.0,-1.0},
							{-1.0, -3.0,5.0,5.0,5.0, -3.0,-1.0},
							{0.0, -2.0,-3.0,-3.0,-3.0, -2.0,0.0},
							{0.0, 0.0,-1.0,-1.0,-1.0, 0.0,0.0} };
	double mask2[9][9] = { {0.0, 0.0,0.0,-1.0,-1.0,-1.0, 0.0, 0.0,0.0},  // 로버츠 열 검출 마스크 
							{0.0, -2.0,-3.0,-3.0,-3.0,-3.0, -3.0, -2.0,0.0},
							{0.0, -3.0,-2.0,-1.0,-1.0,-1.0, -2.0, -3.0,0.0},
							{-1.0, -3.0,-1.0,9.0,9.0,9.0, -1.0, -3.0,-1.0},
							{-1.0, -3.0,-1.0,9.0,19.0,9.0, -1.0, -3.0,-1.0},
							{-1.0, -3.0,-1.0,9.0,9.0,9.0, -1.0, -3.0,-1.0},
							{0.0, -3.0,-2.0,-1.0,-1.0,-1.0, -2.0, -3.0,0.0},
							{0.0, -2.0,-3.0,-3.0,-3.0,-3.0, -3.0, -2.0,0.0},
							{0.0, 0.0,0.0,-1.0,-1.0,-1.0, 0.0, 0.0,0.0} };


	int checkmask = 0;
	printf("\n\n사용할 DoG 엣지 검출의 마스크 크기를 결정해주세요 ( 7: 7x7 9: 9x9)\n\n");
	scanf(" %d", &checkmask);
	int maskH = 7, maskW = 7;
	double** mask = mallocDoubleMemory(maskH, maskW);
	for (int i = 0; i < maskH; i++)
		for (int k = 0; k < maskW; k++)
			mask[i][k] = 0.0;
	if (checkmask != 7 && checkmask != 9) {
		printf("잘못된 입력입니다!!");
		return;
	}
	if (checkmask == 7) {
		maskH = maskW = 7;
		freeDoubleMemory(mask, 7);
		mask = mallocDoubleMemory(maskH, maskW);
		for (int i = 0; i < maskH; i++)
			for (int k = 0; k < maskW; k++)
				mask[i][k] = mask1[i][k];
	}
	if (checkmask == 9) {
		maskH = maskW = 9;
		freeDoubleMemory(mask, 7);
		mask = mallocDoubleMemory(maskH, maskW);
		for (int i = 0; i < maskH; i++)
			for (int k = 0; k < maskW; k++)
				mask[i][k] = mask2[i][k];
	}

	// 임시 메모리 할당(실수형)
	double** tmpinImage = mallocDoubleMemory(inH + maskH - 1, inW + maskW - 1);
	double** tmpoutImage = mallocDoubleMemory(outH, outW);
	// 임시 입력 메모리를 초기화(127) : 필요시 평균값

	for (int i = 0; i < inH + maskH - 1; i++)
		for (int k = 0; k < inW + maskW - 1; k++)
			tmpinImage[i][k] = 127.0;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpinImage[i + (int)(maskH / 2)][k + (int)(maskW / 2)] = (double)inImage[i][k];
	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < maskH; m++)
				for (int n = 0; n < maskW; n++)
					S += tmpinImage[i + m][k + n] * mask[m][n];
			tmpoutImage[i][k] = S;
		}
	}
	//// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpoutImage[i][k] += 67.0;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpoutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpoutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpoutImage[i][k];
		}
	}
	freeDoubleMemory(tmpinImage, inH + maskW - 1);
	freeDoubleMemory(tmpoutImage, outH);
	freeDoubleMemory(mask, maskH);
	printImage();
}