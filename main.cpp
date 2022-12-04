#include "windows.h"
#include "conio.h"             
#include "graphics.h"

#pragma comment(lib,"Winmm.lib")

//全局定义
constexpr auto Width = 800;;
constexpr auto High = 564;;

//全局变量
int canvas[16][16];  //棋子的每一个落点 用的是36*36的方块
//数组赋值为1时为黑棋 赋值为2时为白棋
int order = 1;
int colorWhite_x = 215, colorWhite_y = 235, colorWhite_z = 241;   //下棋顺序亮灯
int colorBlack_x = 13, colorBlack_y = 137, colorBlack_z = 25;
int gameStatus = 1;   //用于判断开始和重玩的状态
int isStop = 1;       //用来判断一方胜利以后
int x, y;           //储存人机的适合选项
int modeChoose = 1;      //模式选择 为1时为人人对战 为2时为人机对战(后手) 为3时为人机对战(先手) 
int number[16][16];   //用来存储每个点的估值

struct Record
{
	int x;
	int y;
};

struct Record black;
struct Record white;


//函数
void startup();         //初始化函数
void show();            //显示函数
void dateWithoutInput();  //与用户输入无关的更新
void dateWithInput();     //与用户输入有关的更新
void isWin();            //判断游戏胜负              
void assessment();        //分数评估


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

//评估系统
void assessment()
{

	int i, j;
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {
			number[i][j] = 0;
		}
	}

	//横着遍历五数组
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

	//竖着遍历五数组
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

	//右下遍历五数组
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

	//左上遍历五数组
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

//变量初始化
void startup()
{

	int i, j;
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= 15; j++) {
			canvas[i][j] = 0;
		}
	}

	initgraph(Width, High);   //画画布
	setbkcolor(RGB(236, 222, 173));
	cleardevice();
	//（20，20）（524，524）构成的整体棋盘

	//设置窗口名字
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Gobang 1.0");

	//背景音乐
	if (isStop == 1) {
		mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
		mciSendString("play bkmusic  repeat", NULL, 0, NULL);
	}

	BeginBatchDraw();
}

// 显示
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
				//设置黑色
				setfillcolor(BLACK);
				solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
			}
			if (canvas[i][j] == 2) {
				//设置白色
				setfillcolor(WHITE);
				solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
			}
		}
	}

	//画白棋操作棋框
	setcolor(RGB(colorWhite_x, colorWhite_y, colorWhite_z));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(574, 100, 644, 150);

	setbkmode(TRANSPARENT);   //字体背景透明

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(578, 110, _T("白棋"));


	//画黑棋操作棋框
	setcolor(RGB(colorBlack_x, colorBlack_y, colorBlack_z));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(694, 100, 764, 150);

	setbkmode(TRANSPARENT);  //字体背景透明

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(698, 110, _T("黑棋"));

	//开始和重玩按钮
	setcolor(RGB(215, 236, 241));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(634, 200, 704, 250);

	setbkmode(TRANSPARENT);  //字体背景透明

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	if (gameStatus == 1) {
		outtextxy(638, 210, _T("开始"));
	}
	else if (gameStatus == 2) {
		outtextxy(638, 210, _T("重玩"));
	}

	//悔棋键位
	setcolor(RGB(215, 236, 241));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(574, 300, 644, 350);

	setbkmode(TRANSPARENT);  //字体背景透明

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(578, 310, _T("悔棋"));

	//退出键位
	setcolor(RGB(215, 236, 241));
	setfillcolor(RGB(215, 236, 241));
	fillrectangle(694, 300, 764, 350);

	setbkmode(TRANSPARENT);  //字体背景透明

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(698, 310, _T("退出"));

	//绘制选择系统
	setcolor(WHITE);
	setfillcolor(WHITE);
	solidcircle(578, 410, 10);

	settextcolor(BLACK);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(598, 400, _T("人人对战"));

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
	settextstyle(20, 0, _T("黑体"));
	outtextxy(598, 430, _T("人机对战"));



	FlushBatchDraw();
	Sleep(2);
}

//判断胜负的函数
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

				//判断黑棋连子
				if (canvas[i][j] == 1) {
					int p;

					//正向
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

					//反向
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

				//判断白棋连子
				if (canvas[i][j] == 2) {
					int p;
					//正向
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

					//反向
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
					settextstyle(30, 0, _T("黑体"));
					outtextxy(598, 50, "黑棋胜利");
					isStop = 2;
					//关闭背景音乐
					mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
					mciSendString("close bkmusic", NULL, 0, NULL);
				}
				else if (white == 4) {
					settextcolor(RED);
					settextstyle(30, 0, _T("黑体"));
					outtextxy(598, 50, "白棋胜利");
					isStop = 2;
					//关闭背景音乐
					mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
					mciSendString("close bkmusic ", NULL, 0, NULL);
				}
			}

		}
	}


}

//与用户无关的更新
void dateWithoutInput()
{
	isWin();

	if (modeChoose == 2 && isStop == 1) {
		gameStatus = 2;
		if (order == 1) {
			canvas[8][8] = 1;
			order++;

			//音乐
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

			//音乐
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

//与用户有关的更新
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
					//清理上次留下的红色线条
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


						if (msg.uMsg == WM_LBUTTONDOWN) {    //按下左键

							gameStatus = 2;

							if ((order % 2 == 0) && (canvas[i][j] == 0)) {

								//音乐
								mciSendString("close downmusic", NULL, 0, NULL);
								mciSendString("open ./resources/down.mp3 alias downmusic", NULL, 0, NULL);
								mciSendString("play downmusic", NULL, 0, NULL);


								canvas[i][j] = 2;  //白棋

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
								//音乐
								mciSendString("close downmusic", NULL, 0, NULL);
								mciSendString("open ./resources/down.mp3 alias downmusic", NULL, 0, NULL);
								mciSendString("play downmusic", NULL, 0, NULL);


								canvas[i][j] = 1;  //黑棋

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

		//点击换模式
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


		//点击重玩
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

				//背景音乐
				if (isStop == 1) {
					mciSendString("open ./resources/bkmusic.mp3 alias bkmusic", NULL, 0, NULL);
					mciSendString("play bkmusic  repeat", NULL, 0, NULL);
				}

				cleardevice();
			}
		}

		//点击退出
		if ((msg.x >= 694) && (msg.x <= 764) && (msg.y >= 300) && (msg.y <= 350)) {
			if (msg.uMsg == WM_LBUTTONDOWN) {
				exit(0);
			}
		}

		//点击悔棋
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
								//设置白色
								setfillcolor(BLACK);
								solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
							}
							if (canvas[i][j] == 2) {
								//设置黑色
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
								//设置白色
								setfillcolor(BLACK);
								solidcircle(20 + (i - 1) * 36, 20 + (j - 1) * 36, 14);
							}
							if (canvas[i][j] == 2) {
								//设置黑色
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

	FlushMouseMsgBuffer();    //清空鼠标消息缓冲区

	FlushBatchDraw();
}

//主函数
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