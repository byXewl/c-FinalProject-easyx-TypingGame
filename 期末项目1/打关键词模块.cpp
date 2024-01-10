#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<windows.h>
#include <graphics.h>		//easyx
#include <time.h>
#include <conio.h>  
#include<string.h>
#include "A.h"


#define CI_MAX  4	 //一次落下的单词的最大的数量
#define CI_SPEED  2  //落下单词的速度

//下落单词的结构体
struct TARGET
{
	int x;
	int y;
	char* str;
}words[CI_MAX];

//用户输入的词
struct USEKEY
{
	int x;
	int y;
	char str[20];
}userkey = { 3 * WINDOW_LONG / 6 - 35,14 * WINDOW_WIDE / 15 - 10 ,"" };

//游戏属性 正确 错误
int right = 0;
int error = 0;

//绘制游戏进行布局
void divWindow()
{
	IMAGE gamebackground1;     //游戏背景图1
	loadimage(&gamebackground1, L"图片\\游戏背景图1.jpg", WINDOW_LONG, WINDOW_WIDE);
	putimage(0, 0, &gamebackground1);
	//setbkcolor(RGB(205,186,150));
	line(5 * WINDOW_LONG / 6, 0, 5 * WINDOW_LONG / 6, 13 * WINDOW_WIDE / 15);
	line(0, 13 * WINDOW_WIDE / 15, WINDOW_LONG, 13 * WINDOW_WIDE / 15);
	line(5 * WINDOW_LONG / 6, 5 * WINDOW_WIDE / 15, WINDOW_LONG, 5 * WINDOW_WIDE / 15);
}

//绘制游戏进行页面文字
void drawScore()
{
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);                         //文字背景透明 
	settextstyle(25, 0, L"华文新魏");
	outtextxy(5 * WINDOW_LONG / 6 + 10, 20, L"打词练习");
	settextstyle(18, 0, L"华文楷体");
	outtextxy(5 * WINDOW_LONG / 6 + 5, 70, L"《C语言关键词》");
	settextstyle(25, 0, L"华文楷体");
	WCHAR str_r[20];
	wsprintf(str_r, L"正确数:%d个", right);
	outtextxy(5 * WINDOW_LONG / 6 + 10, 225, str_r);

	WCHAR str_e[20];
	wsprintf(str_e, L"错误数:%d个", error);
	outtextxy(5 * WINDOW_LONG / 6 + 10, 285, str_e);

	outtextxy(5 * WINDOW_LONG / 6 + 10, 355, L"按回车检验");
	outtextxy(5 * WINDOW_LONG / 6 + 10, 395, L"空格键退出");

}
//初始化单词
void iniTarget(struct TARGET words[], int n)
{
	srand((unsigned int)time(NULL));

	static char str[33][10] = { "main","include","void","while","for",
							   "true","false","break","int","char",
							   "float","double","switch","case","static",
							   "if","else","short","long","signed",
							   "unsigned","sizeof","continue","struct","union",
								"enum","default","return","do","goto",
							   "auto","extern","const" };//32关键词
	static char str1[999][10] = {


	};

	words[n].str = str[rand() % 33];
	//取余法防止单词重复
	while (1)
	{
		int flag = 0;
		int j = 1;
		while (j < CI_MAX)
		{
			if (words[n].str == words[(n + j) % CI_MAX].str)
			{
				flag = 1;
				break;
			}
			j++;
		}
		if (flag == 1)
		{
			words[n].str = str[rand() % 33];
		}
		else
		{
			break;
		}

	}

	//生成随机要下落前的位置
	words[n].x = rand() % (4 * WINDOW_LONG / 6) + 5;
	words[n].y = -25;

}

//游戏进行
void PlayGame1()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	int k = 0;    //用于用户单词接收
	//初始化每个单词
	for (int n = 0; n < CI_MAX; n++)
	{
		iniTarget(words, n);
		words[n].y = -50 - n * 50;//形成不等高，防止挨太近
	}

	BeginBatchDraw();
	while (1)
	{
		cleardevice(); //防止停留
		divWindow();
		drawScore();

		//打印单词下落和碰线处理
		WCHAR str_words[CI_MAX][20];
		for (int i = 0; i < CI_MAX; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				str_words[i][j] = words[i].str[j];
			}
		}
		for (int n = 0; n < CI_MAX; n++)
		{
			settextcolor(RGB(255, 215, 0));
			settextstyle(30, 0, L"MS Reference Sans Serif");
			outtextxy(words[n].x, words[n].y, str_words[n]);
			words[n].y += CI_SPEED;

			if (words[n].y >= 13 * WINDOW_WIDE / 15 - textheight(str_words[n]))
			{
				iniTarget(words, n);
			}
		}

		if (_kbhit())//检测按键
		{
			char target = _getch();

			if (target != '\r')  //输入没按回车
			{
				if (target == 32 || target == 27)   //用户点击空格键或者Esc
				{
					break;   //	返回主菜单		
				}
				userkey.str[k++] = target;
			}
			else   //按下回车后
			{
				int flagError = 0;  //错误旗帜，按下回车，如果还是0则error++
				//消除
				for (int i = 0; i < CI_MAX; i++)
				{
					if (strcmp(userkey.str, words[i].str) == 0)
					{
						iniTarget(words, i);
						right++;
						flagError = 1;
					}
				}
				if (flagError == 0)
				{
					error++;
					//CreateThread(nullptr, 0, PlayBombMusic, nullptr, 0, nullptr);
				}

				//用户单词初始化为0
				k = 0;
				userkey.x = 3 * WINDOW_LONG / 6 - 35;
				memset(userkey.str, 0, 20);
			}
		}

		//没有按回车键 打印用户输入
		WCHAR str_user[20];
		for (int i = 0; i < 20; i++)
		{
			str_user[i] = userkey.str[i];
		}
		outtextxy(userkey.x, userkey.y, str_user);



		FlushBatchDraw();
		Sleep(100);
	}
	EndBatchDraw();
	MenuMouse();   //直接返回主菜单		



}