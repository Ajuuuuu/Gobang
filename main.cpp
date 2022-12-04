#include "windows.h"
#include "conio.h"             
#include "graphics.h"

#pragma comment(lib,"Winmm.lib")

//ȫ�ֶ���
constexpr auto Width = 800;;
constexpr auto High = 564;;

//ȫ�ֱ���
int canvas[16][16];  //���ӵ�ÿһ����� �õ���36*36�ķ���
//���鸳ֵΪ1ʱΪ���� ��ֵΪ2ʱΪ����
int order = 1;
int colorWhite_x = 215, colorWhite_y = 235, colorWhite_z = 241;   //����˳������
int colorBlack_x = 13, colorBlack_y = 137, colorBlack_z = 25;
int gameStatus = 1;   //�����жϿ�ʼ�������״̬
int isStop = 1;       //�����ж�һ��ʤ���Ժ�
int x, y;           //�����˻����ʺ�ѡ��
int modeChoose = 1;      //ģʽѡ�� Ϊ1ʱΪ���˶�ս Ϊ2ʱΪ�˻���ս(����) Ϊ3ʱΪ�˻���ս(����) 
int number[16][16];   //�����洢ÿ����Ĺ�ֵ

struct Record
{
	int x;
	int y;
};

struct Record black;
struct Record white;


//����
void startup();         //��ʼ������
void show();            //��ʾ����
void dateWithoutInput();  //���û������޹صĸ���
void dateWithInput();     //���û������йصĸ���
void isWin();            //�ж���Ϸʤ��              
void assessment();        //��������


int calculate(int ai, int person) {
	if (ai != 0 && person != 0)
		return 0;
	if (ai == 0 && person == 0)
		return 7;
	if (ai == 1)
		return 35;
	if (ai == 2)
		return 800;
	if (ai == 3)
		return 15000;
	if (ai == 4)
		return 800000;
	if (person == 1)
		return 15;
	if (person == 2)
		return 400;
	if (person == 3)
		return 1800;
	if (person == 4)
		return 100000;
}

//����ϵͳ
void assessment()
{

	int i, j;
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {
			number[i][j] = 0;
		}
	}

	//���ű���������
	for (i = 1; i <= 11; i++) {
		for (j = 1; j <= 15; j++) {
			int black = 0, white = 0;
			int k;
			for (k = 1; k <= 4; k++) {
				if (canvas[i + k][j] == 1) {
					black++;
				}
				else if (canvas[i + k][j] == 2) {
					white++;
				}
			}
			int t = calculate(black, white);

			for (k = 1; k <= 4; k++) {
				number[i + k][j] += t;
			}
		}
	}

	//���ű���������
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 11; j++) {
			int black = 0, white = 0;
			int k;
			for (k = 1; k <= 4; k++) {
				if (canvas[i][j + k] == 1) {
					black++;
				}
				else if (canvas[i][j + k] == 2) {
					white++;
				}
			}
			int t = calculate(black, white);

			for (k = 1; k <= 4; k++) {
				number[i][j + k] += t;
			}
		}
	}

	//���±���������
	for (i = 1; i <= 11; i++) {
		for (j = 1; j <= 11; j++) {
			int black = 0, white = 0;
			int k;
			for (k = 1; k <= 4; k++) {
				if (canvas[i + k][j + k] == 1) {
					black++;
				}
				else if (canvas[i + k][j + k] == 2) {
					white++;
				}
			}
			int t = calculate(black, white);

			for (k = 1; k <= 4; k++) {
				number[i + k][j + k] += t;
			}
		}
	}

	//���ϱ���������
	for (i = 1; i <= 11; i++) {
		for (j = 5; j <= 15; j++) {
			int black = 0, white = 0;
			int k;
			for (k = 1; k <= 4; k++) {
				if (canvas[i + k][j - k] == 1) {
					black++;
				}
				else if (canvas[i + k][j - k] == 2) {
					white++;
				}
			}
			int t = calculate(black, white);

			for (k = 1; k <= 4; k++) {
				number[i + k][j - k] += t;
			}
		}
	}

	int max = 0;
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {
			if (number[i][j] >= max && canvas[i][j] == 0) {
				max = number[i][j];
				x = i;
				y = j;
			}
		}
	}

}

//������ʼ��
void startup()
{

	int i, j;
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {
			canvas[i][j] = 0;
		}
	}

	initgraph(Width, High);   //������
	setbkcolor(RGB(236, 222, 173));
	cleardevice();
	//��20��20����524��524�����ɵ���������

	//���ô�������
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Gobang 1.0");

	//��������
	if (isStop == 1) {
		mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
		mciSendString("play bkmusic  repeat", NULL, 0, NULL);
	}

	BeginBatchDraw();
}

// ��ʾ
void show()
{
	int i, j;
	setlinestyle(PS_SOLID, 2);
	setcolor(RGB(129, 129, 129));

	for (i = 0; i <= 14; i++) {
		line(20 + i * 36, 20, 20 + i * 36, 524);
	}
	for (j = 0; j <= 14; j++) {
		line(20, 20 + j * 36, 524, 20 + j * 36);
	}

	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {
			if (canvas[i][j] == 1) {
				//���ú�ɫ
				setfillcolor(BLACK);
				solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
			}
			if (canvas[i][j] == 2) {
				//���ð�ɫ
				setfillcolor(WHITE);
				solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
			}
		}
	}

	//������������
	setcolor(RGB(colorWhite_x, colorWhite_y, colorWhite_z));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(574, 100, 644, 150);

	setbkmode(TRANSPARENT);   //���屳��͸��

	settextcolor(BLACK);
	settextstyle(30, 0, _T("����"));
	outtextxy(578, 110, _T("����"));


	//������������
	setcolor(RGB(colorBlack_x, colorBlack_y, colorBlack_z));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(694, 100, 764, 150);

	setbkmode(TRANSPARENT);  //���屳��͸��

	settextcolor(BLACK);
	settextstyle(30, 0, _T("����"));
	outtextxy(698, 110, _T("����"));

	//��ʼ�����水ť
	setcolor(RGB(215, 236, 241));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(634, 200, 704, 250);

	setbkmode(TRANSPARENT);  //���屳��͸��

	settextcolor(BLACK);
	settextstyle(30, 0, _T("����"));
	if (gameStatus == 1) {
		outtextxy(638, 210, _T("��ʼ"));
	}
	else if (gameStatus == 2) {
		outtextxy(638, 210, _T("����"));
	}

	//�����λ
	setcolor(RGB(215, 236, 241));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(574, 300, 644, 350);

	setbkmode(TRANSPARENT);  //���屳��͸��

	settextcolor(BLACK);
	settextstyle(30, 0, _T("����"));
	outtextxy(578, 310, _T("����"));

	//�˳���λ
	setcolor(RGB(215, 236, 241));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(694, 300, 764, 350);

	setbkmode(TRANSPARENT);  //���屳��͸��

	settextcolor(BLACK);
	settextstyle(30, 0, _T("����"));
	outtextxy(698, 310, _T("�˳�"));

	//����ѡ��ϵͳ
	setcolor(WHITE);
	setfillcolor(WHITE);
	solidcircle(578, 410, 10);

	settextcolor(BLACK);
	settextstyle(20, 0, _T("����"));
	outtextxy(598, 400, _T("���˶�ս"));

	if (modeChoose == 1) {
		setcolor(WHITE);
		setfillcolor(BLACK);
		solidcircle(578, 410, 5);
	}

	setcolor(WHITE);
	setfillcolor(WHITE);
	solidcircle(578, 440, 10);

	if (modeChoose == 2) {
		setcolor(WHITE);
		setfillcolor(BLACK);
		solidcircle(578, 440, 5);
	}

	settextcolor(BLACK);
	settextstyle(20, 0, _T("����"));
	outtextxy(598, 430, _T("�˻���ս"));



	FlushBatchDraw();
	Sleep(2);
}

//�ж�ʤ���ĺ���
void isWin()
{
	int typeDerection[4][2] = { {1,1},{1,0},{1,-1},{0,1} };

	int k;
	int i, j;
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {

			for (k = 0; k < 4; k++) {
				int x = typeDerection[k][0];
				int y = typeDerection[k][1];

				int black = 0;
				int white = 0;

				//�жϺ�������
				if (canvas[i][j] == 1) {
					int p;

					//����
					for (p = 1; p <= 4; p++) {
						if (1 <= i + x * p <= 15 && 1 <= j + y * p <= 15) {
							if (canvas[i + x * p][j + y * p] == 1)
							{
								black++;
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}

					//����
					for (p = 1; p <= 4; p++) {
						if (1 <= i - x * p <= 15 && 1 <= j - y * p <= 15) {
							if (canvas[i - x * p][j - y * p] == 1)
							{
								black++;
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}
				}

				//�жϰ�������
				if (canvas[i][j] == 2) {
					int p;
					//����
					for (p = 1; p <= 4; p++) {
						if (1 <= i + x * p <= 15 && 1 <= j + y * p <= 15) {
							if (canvas[i + x * p][j + y * p] == 2)
							{
								white++;
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}

					//����
					for (p = 1; p <= 4; p++) {
						if (1 <= i - x * p <= 15 && 1 <= j - y * p <= 15) {
							if (canvas[i - x * p][j - y * p] == 2)
							{
								white++;
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}
				}

				if (black == 4) {
					settextcolor(RED);
					settextstyle(30, 0, _T("����"));
					outtextxy(598, 50, "����ʤ��");
					isStop = 2;
					//�رձ�������
					mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
					mciSendString("close bkmusic", NULL, 0, NULL);
				}
				else if (white == 4) {
					settextcolor(RED);
					settextstyle(30, 0, _T("����"));
					outtextxy(598, 50, "����ʤ��");
					isStop = 2;
					//�رձ�������
					mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
					mciSendString("close bkmusic ", NULL, 0, NULL);
				}
			}

		}
	}


}

//���û��޹صĸ���
void dateWithoutInput()
{
	isWin();

	if (modeChoose == 2 && isStop == 1) {
		gameStatus = 2;
		if (order == 1) {
			canvas[8][8] = 1;
			order++;

			//����
			mciSendString("close downmusic", NULL, 0, NULL);
			mciSendString("open ./resources/down.mp3 alias downmusic", NULL, 0, NULL);
			mciSendString("play downmusic", NULL, 0, NULL);

			colorWhite_x = 13;
			colorWhite_y = 137;
			colorWhite_z = 25;
			colorBlack_x = 215;
			colorBlack_y = 235;
			colorBlack_z = 241;

		}
		if (order % 2 == 1) {
			assessment();
			canvas[x][y] = 1;
			order++;

			//����
			mciSendString("close downmusic", NULL, 0, NULL);
			mciSendString("open ./resources/down.mp3 alias downmusic", NULL, 0, NULL);
			mciSendString("play downmusic", NULL, 0, NULL);

			colorWhite_x = 13;
			colorWhite_y = 137;
			colorWhite_z = 25;
			colorBlack_x = 215;
			colorBlack_y = 235;
			colorBlack_z = 241;
		}
	}

}

//���û��йصĸ���
void dateWithInput()
{

	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		int i, j;
		if (isStop == 1) {
			for (i = 1; i <= 15; i++) {
				for (j = 1; j <= 15; j++) {

					setcolor(RGB(236, 222, 173));
					setlinestyle(PS_SOLID, 1.5);
					//�����ϴ����µĺ�ɫ����
					line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 - 10 + 4, 20 + (j - 1) * 36 - 10);
					line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 - 10 + 4);
					line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 - 10 + 4, 20 + (j - 1) * 36 + 10);
					line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 + 10 - 4);
					line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 + 10 - 4);
					line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 + 10 - 4, 20 + (j - 1) * 36 + 10);
					line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 - 10 + 4);
					line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 + 10 - 4, 20 + (j - 1) * 36 - 10);

				}
			}

			for (i = 1; i <= 15; i++) {
				for (j = 1; j <= 15; j++) {

					if ((msg.x >= 20 + (i - 1) * 36 - 10) && (msg.x <= 20 + (i - 1) * 36 + 10) && (msg.y >= 20 + (j - 1) * 36 - 10) && (msg.y <= 20 + (j - 1) * 36 + 10)) {

						setcolor(RED);
						setlinestyle(PS_SOLID, 1.5);
						line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 - 10 + 4, 20 + (j - 1) * 36 - 10);
						line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 - 10 + 4);
						line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 - 10 + 4, 20 + (j - 1) * 36 + 10);
						line(20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 - 10, 20 + (j - 1) * 36 + 10 - 4);
						line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 + 10 - 4);
						line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 + 10, 20 + (i - 1) * 36 + 10 - 4, 20 + (j - 1) * 36 + 10);
						line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 - 10 + 4);
						line(20 + (i - 1) * 36 + 10, 20 + (j - 1) * 36 - 10, 20 + (i - 1) * 36 + 10 - 4, 20 + (j - 1) * 36 - 10);


						if (msg.uMsg == WM_LBUTTONDOWN) {    //�������

							gameStatus = 2;

							if ((order % 2 == 0) && (canvas[i][j] == 0)) {

								//����
								mciSendString("close downmusic", NULL, 0, NULL);
								mciSendString("open ./resources/down.mp3 alias downmusic", NULL, 0, NULL);
								mciSendString("play downmusic", NULL, 0, NULL);


								canvas[i][j] = 2;  //����

								white.x = i;
								white.y = j;

								colorWhite_x = 215;
								colorWhite_y = 235;
								colorWhite_z = 241;
								colorBlack_x = 13;
								colorBlack_y = 137;
								colorBlack_z = 25;
								order++;
								break;


							}
							else if ((order % 2 == 1) && (canvas[i][j] == 0) && modeChoose == 1) {
								//����
								mciSendString("close downmusic", NULL, 0, NULL);
								mciSendString("open ./resources/down.mp3 alias downmusic", NULL, 0, NULL);
								mciSendString("play downmusic", NULL, 0, NULL);


								canvas[i][j] = 1;  //����

								black.x = i;
								black.y = j;


								colorWhite_x = 13;
								colorWhite_y = 137;
								colorWhite_z = 25;
								colorBlack_x = 215;
								colorBlack_y = 235;
								colorBlack_z = 241;

								order++;
								break;
							}
							else {
								break;
							}
						}

					}


				}
			}
		}

		//�����ģʽ
		if ((msg.x >= 568) && (msg.x <= 588) && (msg.y >= 400) && (msg.y <= 420)) {
			if (msg.uMsg == WM_LBUTTONDOWN) {
				modeChoose = 1;
			}
		}
		if ((msg.x >= 568) && (msg.x <= 588) && (msg.y >= 430) && (msg.y <= 450)) {
			if (msg.uMsg == WM_LBUTTONDOWN) {
				modeChoose = 2;
			}
		}


		//�������
		if ((msg.x >= 634) && (msg.x <= 704) && (msg.y >= 200) && (msg.y <= 250)) {
			if (msg.uMsg == WM_LBUTTONDOWN) {
				gameStatus = 1;
				isStop = 1;
				int i, j;
				for (i = 1; i <= 15; i++) {
					for (j = 1; j <= 15; j++) {
						canvas[i][j] = 0;
					}
				}

				if (modeChoose == 1) {
					colorWhite_x = 215; colorWhite_y = 236; colorWhite_z = 241;
					colorBlack_x = 13; colorBlack_y = 137; colorBlack_z = 25;
				}
				if (modeChoose == 2) {
					colorWhite_x = 13; colorWhite_y = 137; colorWhite_z = 25;
					colorBlack_x = 215; colorBlack_y = 236; colorBlack_z = 241;
				}
				order = 1;

				//��������
				if (isStop == 1) {
					mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
					mciSendString("play bkmusic  repeat", NULL, 0, NULL);
				}

				cleardevice();
			}
		}

		//����˳�
		if ((msg.x >= 694) && (msg.x <= 764) && (msg.y >= 300) && (msg.y <= 350)) {
			if (msg.uMsg == WM_LBUTTONDOWN) {
				exit(0);
			}
		}

		//�������
		if (modeChoose == 1 && msg.x >= 574 && msg.x <= 644 && msg.y >= 300 && msg.y <= 350) {
			if (msg.uMsg == WM_LBUTTONDOWN) {
				if (order % 2 == 0 && order >= 2) {
					canvas[black.x][black.y] = 0;

					cleardevice();

					int i, j;
					setlinestyle(PS_SOLID, 2);
					setcolor(RGB(129, 129, 129));

					for (i = 0; i <= 14; i++) {
						line(20 + i * 36, 20, 20 + i * 36, 524);
					}
					for (j = 0; j <= 14; j++) {
						line(20, 20 + j * 36, 524, 20 + j * 36);
					}

					for (i = 1; i <= 15; i++) {
						for (j = 1; j <= 15; j++) {
							if (canvas[i][j] == 1) {
								//���ð�ɫ
								setfillcolor(BLACK);
								solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
							}
							if (canvas[i][j] == 2) {
								//���ú�ɫ
								setfillcolor(WHITE);
								solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
							}
						}
					}

					colorWhite_x = 215;
					colorWhite_y = 235;
					colorWhite_z = 241;
					colorBlack_x = 13;
					colorBlack_y = 137;
					colorBlack_z = 25;

					order--;
				}
				else if (order % 2 == 1 && order >= 2) {
					canvas[white.x][white.y] = 0;
					int i, j;

					cleardevice();

					setlinestyle(PS_SOLID, 2);
					setcolor(RGB(129, 129, 129));

					for (i = 0; i <= 14; i++) {
						line(20 + i * 36, 20, 20 + i * 36, 524);
					}
					for (j = 0; j <= 14; j++) {
						line(20, 20 + j * 36, 524, 20 + j * 36);
					}

					for (i = 1; i <= 15; i++) {
						for (j = 1; j <= 15; j++) {
							if (canvas[i][j] == 1) {
								//���ð�ɫ
								setfillcolor(BLACK);
								solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
							}
							if (canvas[i][j] == 2) {
								//���ú�ɫ
								setfillcolor(WHITE);
								solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
							}
						}
					}

					colorWhite_x = 13;
					colorWhite_y = 137;
					colorWhite_z = 25;
					colorBlack_x = 215;
					colorBlack_y = 235;
					colorBlack_z = 241;

					order--;
				}
			}
		}


	}

	FlushMouseMsgBuffer();    //��������Ϣ������

	FlushBatchDraw();
}

//������
int main()
{
	startup();
	while (1)
	{
		show();
		dateWithoutInput();
		dateWithInput();
	}

	return 0;

}