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


#define CI_MAX  4	 //һ�����µĵ��ʵ���������
#define CI_SPEED  2  //���µ��ʵ��ٶ�

//���䵥�ʵĽṹ��
struct TARGET
{
	int x;
	int y;
	char* str;
}words[CI_MAX];

//�û�����Ĵ�
struct USEKEY
{
	int x;
	int y;
	char str[20];
}userkey = { 3 * WINDOW_LONG / 6 - 35,14 * WINDOW_WIDE / 15 - 10 ,"" };

//��Ϸ���� ��ȷ ����
int right = 0;
int error = 0;

//������Ϸ���в���
void divWindow()
{
	IMAGE gamebackground1;     //��Ϸ����ͼ1
	loadimage(&gamebackground1, L"ͼƬ\\��Ϸ����ͼ1.jpg", WINDOW_LONG, WINDOW_WIDE);
	putimage(0, 0, &gamebackground1);
	//setbkcolor(RGB(205,186,150));
	line(5 * WINDOW_LONG / 6, 0, 5 * WINDOW_LONG / 6, 13 * WINDOW_WIDE / 15);
	line(0, 13 * WINDOW_WIDE / 15, WINDOW_LONG, 13 * WINDOW_WIDE / 15);
	line(5 * WINDOW_LONG / 6, 5 * WINDOW_WIDE / 15, WINDOW_LONG, 5 * WINDOW_WIDE / 15);
}

//������Ϸ����ҳ������
void drawScore()
{
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);                         //���ֱ���͸�� 
	settextstyle(25, 0, L"������κ");
	outtextxy(5 * WINDOW_LONG / 6 + 10, 20, L"�����ϰ");
	settextstyle(18, 0, L"���Ŀ���");
	outtextxy(5 * WINDOW_LONG / 6 + 5, 70, L"��C���Թؼ��ʡ�");
	settextstyle(25, 0, L"���Ŀ���");
	WCHAR str_r[20];
	wsprintf(str_r, L"��ȷ��:%d��", right);
	outtextxy(5 * WINDOW_LONG / 6 + 10, 225, str_r);

	WCHAR str_e[20];
	wsprintf(str_e, L"������:%d��", error);
	outtextxy(5 * WINDOW_LONG / 6 + 10, 285, str_e);

	outtextxy(5 * WINDOW_LONG / 6 + 10, 355, L"���س�����");
	outtextxy(5 * WINDOW_LONG / 6 + 10, 395, L"�ո���˳�");

}
//��ʼ������
void iniTarget(struct TARGET words[], int n)
{
	srand((unsigned int)time(NULL));

	static char str[33][10] = { "main","include","void","while","for",
							   "true","false","break","int","char",
							   "float","double","switch","case","static",
							   "if","else","short","long","signed",
							   "unsigned","sizeof","continue","struct","union",
								"enum","default","return","do","goto",
							   "auto","extern","const" };//32�ؼ���
	static char str1[999][10] = {


	};

	words[n].str = str[rand() % 33];
	//ȡ�෨��ֹ�����ظ�
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

	//�������Ҫ����ǰ��λ��
	words[n].x = rand() % (4 * WINDOW_LONG / 6) + 5;
	words[n].y = -25;

}

//��Ϸ����
void PlayGame1()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	int k = 0;    //�����û����ʽ���
	//��ʼ��ÿ������
	for (int n = 0; n < CI_MAX; n++)
	{
		iniTarget(words, n);
		words[n].y = -50 - n * 50;//�γɲ��ȸߣ���ֹ��̫��
	}

	BeginBatchDraw();
	while (1)
	{
		cleardevice(); //��ֹͣ��
		divWindow();
		drawScore();

		//��ӡ������������ߴ���
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

		if (_kbhit())//��ⰴ��
		{
			char target = _getch();

			if (target != '\r')  //����û���س�
			{
				if (target == 32 || target == 27)   //�û�����ո������Esc
				{
					break;   //	�������˵�		
				}
				userkey.str[k++] = target;
			}
			else   //���»س���
			{
				int flagError = 0;  //�������ģ����»س����������0��error++
				//����
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

				//�û����ʳ�ʼ��Ϊ0
				k = 0;
				userkey.x = 3 * WINDOW_LONG / 6 - 35;
				memset(userkey.str, 0, 20);
			}
		}

		//û�а��س��� ��ӡ�û�����
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
	MenuMouse();   //ֱ�ӷ������˵�		



}