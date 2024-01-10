#include "A.h"

//����ͼƬ����
IMAGE menubackground;     //�˵�����ͼ
IMAGE gamebackground;     //��Ϸ����ͼ
IMAGE setbackground;      //���ñ���ͼ
IMAGE overbackground;     //��Ϸ��������ͼ
IMAGE scorebackground;    //��߼�¼����ͼ
IMAGE radish[2];          //С�ܲ�������ͼΪ0�ͱ���ͼΪ1
IMAGE ji[2];              //���伦������ͼΪ0�ͱ���ͼΪ1


//��ĸ�ṹ��
struct letter
{
	int x, y;   //��������
	int target; //��ĸ
	int speed;  //�����ٶ�
};
struct letter array[RADISH_MAX];   //С�ܲ�����


struct letter1
{
	int x, y;   //��������
	int target; //��ĸ
	int speed;  //�����ٶ�
};
struct letter1 array1[JI_MAX];     //���伦����


//�ܲ����Ա���
int radish_num = 5;       //һ�����µ�С�ܲ�����Ĭ��Ϊ5
int radish_sum = 0;       //һ�ֻ��С�ܲ�����

//���伦���Ա���
int ji_num = 1;           //һ�����µĽ��伦��Ĭ��Ϊ1
int ji_flag = 0;          //����ģʽ����

//��ʱ����
int game_second = 120;     //һ����Ϸ��ʱ��  (��)��Ĭ��Ϊ120
clock_t  start, end;	   //��Ϸ�ܼ�ʱ����
clock_t p_start, p_end;    //��Ϸ��ͣ��ʱ����
int pause_time = 0;        //��Ϸ��ͣ����ʱ��
int p_flag = 0;            //��Ϸ��ͣ������   0Ϊ��  1Ϊ��

//��߼�¼
struct high_record
{
	int game_second;     //��Ϸ��ʱ���ģʽ
	int radish_sum;      //����ܲ�����
};
struct high_record record[4] = { {120,0},{240,0},{360,0},{0,0} }; //�ֱ� 120s 240s 360s ���� ��߼�¼



int main()
{
	LoadResource();   //������Դ
	MenuMouse();      //�˵�
	return 0;
}

//��Ϸ����(��Ϸ����)
void PlayGame()
{

	initgraph(WINDOW_LONG, WINDOW_WIDE);
	mciSendString(L"close game", 0, 0, 0);
	mciSendString(L"open ����\\��Ϸ��������.mp3 alias game", 0, 0, 0);
	mciSendString(L"play game repeat", 0, 0, 0);

	//��ʼ�����ɴ������ĸ��array�����䴦
	for (int i = 0; i < radish_num; i++)
	{
		InitArray(i);
	}
	//��ʼ�����ɴ������ĸ��array1�����䴦
	if (ji_flag == 1)
	{
		ji_num = 6;
		radish_sum = 100;
	}
	else
	{
		ji_num = 1;
		radish_sum = 0;
	}
	for (int i = 0; i < ji_num; i++)
	{
		InitArray1(i);
	}


	start = clock();       //��Ϸʱ�俪ʼ

	while (1)
	{

		end = clock();     //��Ϸʱ���ʱ��ʱ

		if (p_flag == 1)   //�����ͣ��������ͣ��ʱ��
		{
			pause_time += (int)(p_end - p_start) / 1000;
		}
		if ((int)(end - start) / 1000 - pause_time == game_second && ji_flag == 0)    //��Ϸʱ������ж�
		{
			EndBatchDraw();     //�����ڵùر�
			OverMouse();
		}
		if (radish_sum < 0)    //��ϷС�ܲ��������ж�
		{
			EndBatchDraw();
			OverMouse();
		}
		p_flag = 0;   //��ͣ���ĸ���


		BeginBatchDraw();//˫����
		DrawGameRun();   //������Ϸ����
		for (int i = 0; i < radish_num; i++)    //ÿ��array�������ٶ�����
		{
			array[i].y += array[i].speed;

		}
		for (int i = 0; i < radish_num; i++)    //��ÿ��array���Ϊ�ܲ�ͼ+�м�Ϊ��ĸ�����ͼ�ε�С�ܲ�
		{
			PrintLetterRadish(array[i].x, array[i].y, array[i].target);
		}
		for (int i = 0; i < radish_num; i++)   	//С�ܲ������±߿򣬸�С�ܲ����³�ʼ��
		{
			if (array[i].y >= WINDOW_WIDE)
			{
				InitArray(i);
			}
		}

		for (int i = 0; i < ji_num; i++)    //ÿ��array1�������ٶ�����
		{
			array1[i].y += array1[i].speed;

		}
		for (int i = 0; i < ji_num; i++)    //��ÿ��array1���Ϊ���伦ͼ+�м�Ϊ��ĸ�����ͼ�εĽ��伦
		{
			PrintLetterJi(array1[i].x, array1[i].y, array1[i].target);
		}

		for (int i = 0; i < ji_num; i++)   	//���伦�����±߿�С�ܲ�-1���ý��伦���³�ʼ��
		{
			if (array1[i].y >= WINDOW_WIDE)
			{
				CreateThread(nullptr, 0, PlayJiMusic, nullptr, 0, nullptr);
				radish_sum--;
				InitArray1(i);

			}
		}


		Sleep(50);   //��ʱ50ms
		FlushBatchDraw();


		//���̲���

		if (_kbhit())
		{
			EndBatchDraw();       //�����ڵùر�
			RunKeyBoard();        //������Ϸ���н��� ���̽���

		}

	}
}






//������Ϸ�˵�����
void DrawMenu()
{

	initgraph(WINDOW_LONG, WINDOW_WIDE);
	//�˵�����ͼƬ
	putimage(0, 0, &menubackground);

	//���ֲ���
	settextcolor(LIGHTBLUE);                            //����������ɫ
	settextstyle(WINDOW_WIDE / 25, 0, L"���Ŀ���"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��
	outtextxy(0 * WINDOW_LONG / 2 + 20, WINDOW_WIDE / 7 + 8 * WINDOW_WIDE / 10, L"������Ϸ1.1");

	settextcolor(WHITE);                            //����������ɫ
	settextstyle(WINDOW_WIDE / 20, 0, L"������κ"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��
	//�м�λ��ʱ�������ֳ�2�ݣ���ֳ�7��
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"��ʼ��Ϸ");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"��Ϸ����");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"��߼�¼");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"�ر�����");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"�����ϰ");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"�˳���Ϸ");

}


//������Ϸ���н���
void DrawGameRun()
{
	//��Ϸ����ͼƬ
	putimage(0, 0, &gamebackground);


	settextcolor(LIGHTBLUE);
	settextstyle(20, 0, L"�ֻ�24��-�������");
	setbkmode(TRANSPARENT);
	//�����ұ�ʱ�������ֳ�6�ݣ���ֳ�15��
	if (ji_flag == 1)
	{
		outtextxy(5 * WINDOW_LONG / 6, 1 * WINDOW_WIDE / 15, L"����ģʽ");
		WCHAR str_sum[20];
		wsprintf(str_sum, L"�ܲ���:%d��", radish_sum);
		outtextxy(5 * WINDOW_LONG / 6, 2 * WINDOW_WIDE / 15, str_sum);
	}
	else
	{
		outtextxy(5 * WINDOW_LONG / 6, 1 * WINDOW_WIDE / 15, L"��ʱģʽ");
		WCHAR str_second[20];//���ַ���unicode                               ����  ת������
		wsprintf(str_second, L"����ʱ:%d��", game_second - (int)(end - start) / CLOCKS_PER_SEC + pause_time);//����������ת���ɿ��ַ�����
		outtextxy(5 * WINDOW_LONG / 6, 2 * WINDOW_WIDE / 15, str_second);//���ַ�λ�ø�ʽ����ӡ
		WCHAR str_sum[20];
		wsprintf(str_sum, L"�ܲ���:%d��", radish_sum);
		outtextxy(5 * WINDOW_LONG / 6, 3 * WINDOW_WIDE / 15, str_sum);
	}
	outtextxy(5 * WINDOW_LONG / 6, 4 * WINDOW_WIDE / 15, L"��Ϸ��ʾ��");
	outtextxy(5 * WINDOW_LONG / 6, 5 * WINDOW_WIDE / 15, L"�밴�ո��");
}

//������Ϸ��ͣ����
void DrawGamePause()
{
	//��ʾ����
	settextcolor(LIGHTBLUE);
	settextstyle(20, 0, L"�ֻ�24��-�������");
	setbkmode(TRANSPARENT);                       //���ֱ���͸��
	line(4 * WINDOW_LONG / 6, 6 * WINDOW_WIDE / 15, 4 * WINDOW_LONG / 6, WINDOW_WIDE);
	line(4 * WINDOW_LONG / 6, 6 * WINDOW_WIDE / 15, WINDOW_LONG - 5, 6 * WINDOW_WIDE / 15);
	line(6 * WINDOW_LONG / 6 - 5, 6 * WINDOW_WIDE / 15, 6 * WINDOW_LONG / 6 - 5, WINDOW_WIDE);
	outtextxy(4 * WINDOW_LONG / 6 + 5, 6 * WINDOW_WIDE / 15 + 5, L"��Ϸ��ʾ:");
	settextstyle(15, 0, L"�ֻ�24��-�������");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 7 * WINDOW_WIDE / 15, L"1.������ڹ涨ʱ�����ռ�С�ܲ����ڼ���н�");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 7 * WINDOW_WIDE / 15 + 25, L"  �伦���䣬�������͵���Ѿ��ռ���С�ܲ���");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 7 * WINDOW_WIDE / 15 + 50, L"  ��ʱ��ľ���С�ܲ���ȫ��͵��ʱ��Ϸ������");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 9 * WINDOW_WIDE / 15, L"2.���ռ����ܲ�Խ��Խ��ʱ���ܲ����ֵø��죬");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 9 * WINDOW_WIDE / 15 + 25, L"  ���伦�����ٶȸ��졣");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 10 * WINDOW_WIDE / 15 + 15, L"3.	����������˵�������Ϸ�����ܲ�����������");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 10 * WINDOW_WIDE / 15 + 40, L"  ʱ�䳤�̻�����ʱ��ģʽ����ʼ��100��С�ܲ�");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 10 * WINDOW_WIDE / 15 + 65, L"  �����������Ľ��伦���������ܲ�!��");




	//��������ʱ��ģʽ����ʼ��100��С�ܲ��������������Ľ��伦���������ܲ���

	//���ֲ���
	settextcolor(WHITE);                            //����������ɫ
	settextstyle(WINDOW_WIDE / 20, 0, L"������κ"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"������Ϸ");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"���¿�ʼ");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"���ز˵�");
}

//������Ϸ���ý���
void DrawGameSet()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	//����ͼƬ
	putimage(0, 0, &setbackground);
	//������������
	settextcolor(WHITE);                            //����������ɫ
	settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"��ѡ��һ�����µ�    ����");
	//��һ���ܲ�
	putimage(WINDOW_LONG / 7 + 245, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 15, &radish[0], SRCAND);//����ͼ
	putimage(WINDOW_LONG / 7 + 245, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 15, &radish[1], SRCPAINT);//����ͼ
	//�ܲ�����
	for (int i = 0; i <= 6; i++)//��ӡ3��9����ѡ��
	{
		WCHAR str_num[2];
		wsprintf(str_num, L"%d", i + 3);
		if (i + 3 == radish_num)
		{
			settextcolor(YELLOW);
		}
		else
		{
			settextcolor(WHITE);
		}
		outtextxy(WINDOW_LONG / 7 + i * 51, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, str_num);
	}
	//��Ϸʱ��ѡ��
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"��ѡ����Ϸ��ʱ��");
	for (int i = 0; i <= 2; i++)//��ӡ2 4 6ѡ��
	{
		WCHAR str_minute[7];
		wsprintf(str_minute, L"%d����", 2 * (i + 1));
		if (2 * (i + 1) == game_second / 60)
		{
			settextcolor(YELLOW);
		}
		else
		{
			settextcolor(WHITE);
		}
		outtextxy(WINDOW_LONG / 7 + i * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, str_minute);
	}
	outtextxy(WINDOW_LONG / 7 + 3 * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"����ģʽ");

	//���ز˵���ť
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
}

//������Ϸ��������
void DrawGameOver()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	putimage(0, 0, &overbackground);
	//������������
	settextcolor(WHITE);                            //����������ɫ
	settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��   

	if (radish_sum < 0)
	{
		settextcolor(RED);
		outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"���ź�����Ϸ��սʧ�ܣ�");
	}
	else
	{
		settextcolor(WHITE);
		WCHAR str_num[20];
		wsprintf(str_num, L"�����ռ���      ����Ϊ:%d��", radish_sum);
		outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, str_num);
		//��һ���ܲ�
		putimage(WINDOW_LONG / 7 + 150, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 20, &radish[0], SRCAND);
		putimage(WINDOW_LONG / 7 + 150, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 20, &radish[1], SRCPAINT);

	}
	settextcolor(WHITE);
	WCHAR str_second[10];
	wsprintf(str_second, L"������ʱ:%d��", (int)(end - start) / 1000 - pause_time);
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, str_second);


	//����һ�ְ�ť
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"����һ��");
	//���ز˵���ť
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");

}

//������߼�¼����
void DrawHighScore()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	putimage(0, 0, &scorebackground);
	//������������
	settextcolor(WHITE);                            //����������ɫ
	settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸�� 

	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"�ܲ�������߼�¼��");
	//��ʱ
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"2����");
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"4����");
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"6����");
	//outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"����ģʽ         ������߼�¼");
	//��߷�
	WCHAR str_record[5];
	for (int i = 0; i <= 2; i++)
	{
		wsprintf(str_record, L"%d��", record[i].radish_sum);
		outtextxy(WINDOW_LONG / 2, WINDOW_WIDE / 7 + i * WINDOW_WIDE / 10 + 2 * WINDOW_WIDE / 10, str_record);
		putimage(WINDOW_LONG / 2 + 80, WINDOW_WIDE / 7 + i * WINDOW_WIDE / 10 - 20 + 2 * WINDOW_WIDE / 10, &radish[0], SRCAND);
		putimage(WINDOW_LONG / 2 + 80, WINDOW_WIDE / 7 + i * WINDOW_WIDE / 10 - 20 + 2 * WINDOW_WIDE / 10, &radish[1], SRCPAINT);
	}
	//���ز˵���ť
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
}


//��Ϸ�˵�������꽻��  ������������
void MenuMouse()
{

	DrawMenu();      //���Ʋ˵�����
	//���Ų˵���������
	mciSendString(L"open ����\\�˵���������.mp3 alias menu", 0, 0, 0);
	mciSendString(L"play menu repeat", 0, 0, 0);
	MOUSEMSG m;
	while (1)
	{
		//��ȡ���λ��
		m = GetMouseMsg();

		//�ж����λ��
		//��ʼ��Ϸ
		settextcolor(YELLOW);			//����������ɫ
		if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{

			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"��ʼ��Ϸ");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				//�رղ˵���������
				mciSendString(L"close menu", 0, 0, 0);
				//������Ϸ���б�������
				mciSendString(L"open ����\\��Ϸ��������.mp3 alias game", 0, 0, 0);
				mciSendString(L"play game repeat", 0, 0, 0);
				PlayGame();
			}
		}
		//��Ϸ����
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"��Ϸ����");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				SetMouse();
			}
		}
		//��߼�¼
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"��߼�¼");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				RecordMouse();
			}
		}
		//�ر�����
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"�ر�����");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				//�رղ˵���������
				mciSendString(L"close menu", 0, 0, 0);
			}
		}
		//�����ϰ
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"�����ϰ");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlayGame1();  //ǰ������ģ��
			}
		}
		//�˳���Ϸ
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"�˳���Ϸ");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				exit(0);
			}
		}
		else
		{
			settextcolor(WHITE);	   //����ɫ��ԭ					
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"��ʼ��Ϸ");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"��Ϸ����");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"��߼�¼");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"�ر�����");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"�����ϰ");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"�˳���Ϸ");
		}
	}
	_getch();//����
	closegraph();
}

//��Ϸ���н�����̽���
void RunKeyBoard()
{

	char userkey = _getch();  //���ܰ���

	for (int i = 0; i < radish_num; i++)
	{
		for (int i = 0; i < ji_num; i++)
		{
			if (userkey == array1[i].target || userkey == array1[i].target + 'a' - 'A')
			{
				InitArray1(i);

				CreateThread(nullptr, 0, PlayBombMusic, nullptr, 0, nullptr);

				return;   //�˴μ��̽����������
			}
		}
		if (userkey == array[i].target || userkey == array[i].target + 'a' - 'A')
		{
			InitArray(i);
			CreateThread(nullptr, 0, PlayBombMusic, nullptr, 0, nullptr);
			radish_sum++;
			break;   //�˴μ��̽����������
		}
		else if (userkey == 27)   //�û����esc��
		{
			mciSendString(L"close game", 0, 0, 0);
			radish_sum = 0;
			MenuMouse();   //ֱ�ӷ������˵�
		}
		else if (userkey == 32)   //�û����space��  ������ͣ����
		{
			p_start = clock();    //��ʼ��Ϸ��ͣ
			mciSendString(L"pause game", 0, 0, 0);
			PauseMouse();  //��Ϸ��ͣ���� ��꽻��
			mciSendString(L"resume game", 0, 0, 0);
			p_end = clock();      //������Ϸ��ͣ
			return;  //���������˼������ص���Ϸ����
		}
	}


}

//��Ϸ��ͣ������꽻��
void PauseMouse()
{
	MOUSEMSG m;

	while (1)//��ͣ
	{
		DrawGamePause();      //������ͣ����
		//��ȡ���λ��
		m = GetMouseMsg();
		//������������
		settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
		setbkmode(TRANSPARENT);                         //���ֱ���͸��
		settextcolor(YELLOW);	   //�����ɫ
		//������Ϸ
		if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"������Ϸ");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				p_flag = 1;  //��ͣ����������������ͣ����ʱ
				return;
			}
		}

		//���¿�ʼ
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"���¿�ʼ");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlayGame();
			}
		}

		//���ز˵�
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"���ز˵�");
			//������
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(L"close game", 0, 0, 0);
				MenuMouse();
			}
		}
		else
		{
			settextcolor(WHITE);	   //������ɫ��ԭ					
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"������Ϸ");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"���¿�ʼ");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"���ز˵�");
		}
	}
}
//��Ϸ����������꽻��  ������������
void OverMouse()
{
	mciSendString(L"close game", 0, 0, 0);

	ReadHighRecordFile();//��ȡ��߼�¼�ļ�
	SaveHighRecordFile();//������߼�¼�ļ�

	DrawGameOver();
	MOUSEMSG m;
	while (1)
	{
		//��ȡ���λ��
		m = GetMouseMsg();
		//������������
		settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
		setbkmode(TRANSPARENT);                         //���ֱ���͸��
		//����һ�ְ�������
		if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"����һ��");
			if (m.uMsg == WM_LBUTTONDOWN)
			{

				PlayGame();  //����һ��
			}

		}
		//���ز˵���������
		else if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				MenuMouse();  //���ز˵�
			}
		}
		else
		{
			settextcolor(WHITE);    //��겻��ʱ�������ɫ
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"����һ��");
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
		}

	}
}

//��Ϸ���ý�����꽻��
void SetMouse()
{
	DrawGameSet();
	MOUSEMSG m;
	while (1)
	{
		//��ȡ���λ��
		m = GetMouseMsg();
		//������������
		settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
		setbkmode(TRANSPARENT);                         //���ֱ���͸��
		//�ܲ�������������
		for (int i = 0; i <= 6; i++)
		{
			if (m.x >= WINDOW_LONG / 7 + i * 51 && m.y >= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 &&
				m.x <= WINDOW_LONG / 7 + i * 51 + 20 && m.y <= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 + 24)
			{
				settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
				//�����  �ı����
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					radish_num = i + 3;
				}
			}
			else if (i + 3 == radish_num)
			{
				settextcolor(YELLOW);   //�����ڵĸ���������Ϊ��ɫ
			}
			else
			{
				settextcolor(WHITE);    //��겻��ʱ�������ɫ
			}
			WCHAR str_num[2];
			wsprintf(str_num, L"%d", i + 3);
			outtextxy(WINDOW_LONG / 7 + i * 51, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, str_num);
		}

		//��Ϸʱ����������
		for (int i = 0; i <= 2; i++)
		{
			if (m.x >= WINDOW_LONG / 7 + i * 110 && m.y >= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 &&
				m.x <= WINDOW_LONG / 7 + i * 110 + 80 && m.y <= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 + 26)
			{
				settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
				//�����  �ı�ʱ��
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					game_second = 60 * 2 * (i + 1);
					ji_flag = 0;
				}
			}
			else if (2 * (i + 1) == game_second / 60)
			{
				settextcolor(YELLOW);   //�����ڵ�ʱ��������Ϊ��ɫ
			}
			else
			{
				settextcolor(WHITE);    //��겻��ʱ�������ɫ
			}
			WCHAR str_minute[7];
			wsprintf(str_minute, L"%d����", 2 * (i + 1));
			outtextxy(WINDOW_LONG / 7 + i * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, str_minute);
		}

		//����Ϊ����ģʽ
		if (m.x >= WINDOW_LONG / 7 + 3 * 110 && m.y >= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 3 * 110 + 80 && m.y <= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
			//�����  �ı�ʱ��
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				ji_flag = 1;
				game_second = 0;//
			}
		}
		else if (1 == ji_flag)
		{
			settextcolor(YELLOW);   //�����ڵ�ʱ��������Ϊ��ɫ
		}
		else
		{
			settextcolor(WHITE);    //��겻��ʱ�������ɫ
		}
		outtextxy(WINDOW_LONG / 7 + 3 * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"����ģʽ");

		//���ز˵���������
		if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				MenuMouse();  //���ز˵�
			}
		}
		else
		{
			settextcolor(WHITE);    //��겻��ʱ�������ɫ
		}
		outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
	}
}


//��Ϸ��¼������꽻��  
void RecordMouse()
{
	//�ļ�����
	ReadHighRecordFile();
	DrawHighScore();
	MOUSEMSG m;
	while (1)
	{
		//��ȡ���λ��
		m = GetMouseMsg();
		//������������
		settextstyle(WINDOW_WIDE / 20, 0, L"��������"); //����������ʽ
		setbkmode(TRANSPARENT);                         //���ֱ���͸��
		//���ز˵���������
		if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//����ڵ�ʱ���������ɫ
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				MenuMouse();  //���ز˵�
			}
		}
		else
		{
			settextcolor(WHITE);    //��겻��ʱ�������ɫ
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
		}

	}
}





//��ȡ��߼�¼�ļ�
void ReadHighRecordFile()
{
	FILE* fp;
	if ((fopen_s(&fp, "��߼�¼.txt", "r")) != 0)
	{
		if ((fopen_s(&fp, "��߼�¼.txt", "w")) != 0)//�޴�
		{
			printf("������߼�¼�ļ�ʧ��\n");
			exit(-1);
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			fread(&record[i], sizeof(struct high_record), 1, fp);
		}
	}
	fclose(fp);
}

//������߼�¼�ļ�
void SaveHighRecordFile()
{
	FILE* fp;
	if ((fopen_s(&fp, "��߼�¼.txt", "r+")) != 0)
	{
		printf("����߼�¼�ļ�ʧ��\n");
		exit(-1);
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (game_second == record[i].game_second && radish_sum > record[i].radish_sum)
			{
				record[i].radish_sum = radish_sum;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			fwrite(&record[i], sizeof(struct high_record), 1, fp);
		}
	}
	fclose(fp);
}



//������ĸ��С�ܲ����
void PrintLetterRadish(int x, int y, char target)
{

	settextcolor(BLACK);
	settextstyle(18, 0, L"MS Reference Sans Serif");  //��������
	setbkmode(TRANSPARENT);
	putimage(x, y, &radish[0], SRCAND);
	putimage(x, y, &radish[1], SRCPAINT);
	outtextxy(x + RADISH_LONG / 2 - RADISH_LONG / 17, y + RADISH_WIDE / 2 + RADISH_WIDE / 10, target);
}

//������ʼ��С�ܲ�
void InitArray(int i)
{

	srand((unsigned int)time(NULL));       //�����������
	array[i].target = rand() % 26 + 'A';   //�������26��Ӣ�Ĵ�д��ĸ

	//С�ܲ��ռ�Խ�࣬�ܲ��ٶ�Խ��
	if (radish_sum <= 50)
	{
		array[i].speed = rand() % 2 + WINDOW_WIDE / 250;
	}
	if (radish_sum > 50 && radish_sum <= 100)
	{
		array[i].speed = rand() % 3 + WINDOW_WIDE / 230;
	}
	if (radish_sum > 100 && radish_sum <= 200)
	{
		array[i].speed = rand() % 4 + WINDOW_WIDE / 210;
	}
	if (radish_sum > 200 && radish_sum <= 300)
	{
		array[i].speed = rand() % 4 + WINDOW_WIDE / 190;
	}
	if (radish_sum > 300 && radish_sum <= 400)
	{
		array[i].speed = rand() % 5 + WINDOW_WIDE / 190;
	}
	if (radish_sum > 400 && radish_sum <= 500)
	{
		array[i].speed = rand() % 5 + WINDOW_WIDE / 170;
	}
	if (radish_sum > 500)
	{
		array[i].speed = rand() % 6 + WINDOW_WIDE / 150;
	}
	//��ĸ�ظ� ��������
	while (1)
	{
		int flag = 0;
		int j = 1;
		int k = 0;
		//ȡ�෨�ҳ��������Ƚ�
		while (j < radish_num)
		{
			if (array[i].target == array[(i + j) % radish_num].target)
			{
				flag = 1;
				break;
			}
			while (k < JI_MAX)
			{
				if (array[i].target == array1[k].target)
				{
					flag = 1;
					break;
				}
				k++;
			}

			j++;
		}
		if (flag == 1)
		{
			array[i].target = rand() % 26 + 'A';   //�������26��Ӣ�Ĵ�д��ĸ
		}
		else
		{
			break;
		}

	}


	//�ܲ��������λ��
	array[i].x = rand() % 15 * WINDOW_LONG / 20 + 2 * WINDOW_LONG / 20;
	array[i].y = rand() % 100 - 150;
	//�ܲ�����λ���غ����� ��������
	while (1)
	{
		int flag = 0;
		int j = 1;

		//6���ܲ�����ʱ
		if (radish_num <= 6)
		{
			while (j < radish_num)
			{
				if (array[i].x >= array[(i + j) % radish_num].x - RADISH_LONG && array[i].x <= array[(i + j) % radish_num].x + 2 * RADISH_LONG &&
					array[i].y >= array[(i + j) % radish_num].y - 2 * RADISH_WIDE && array[i].y <= array[(i + j) % radish_num].y + 2 * RADISH_WIDE)
				{
					flag = 1;
				}
				j++;
			}
		}
		//6���ܲ�����ʱ
		else
		{
			while (j < radish_num)
			{
				if (array[i].x >= array[(i + j) % radish_num].x - RADISH_LONG / 1.3 && array[i].x <= array[(i + j) % radish_num].x + 2 * RADISH_LONG / 1.3 &&
					array[i].y >= array[(i + j) % radish_num].y - 2 * RADISH_WIDE / 1.3 && array[i].y <= array[(i + j) % radish_num].y + 2 * RADISH_WIDE / 1.3)
				{
					flag = 1;
				}
				j++;
			}
		}
		if (flag == 1)
		{
			array[i].x = rand() % 15 * WINDOW_LONG / 20 + 2 * WINDOW_LONG / 20;
			array[i].y = rand() % 100 - 150;
		}
		else
		{
			break;
		}
	}
}

//������ĸ�ͽ��伦���
void PrintLetterJi(int x, int y, char target)
{
	settextcolor(BLACK);
	settextstyle(18, 0, L"MS Reference Sans Serif");  //��������
	setbkmode(TRANSPARENT);
	putimage(x, y, &ji[0], SRCAND);
	putimage(x, y, &ji[1], SRCPAINT);
	outtextxy(x + JI_LONG / 2 - 6, y + JI_WIDE / 2 + 8, target);
}
//������ʼ�����伦
void InitArray1(int i)
{
	srand((unsigned int)time(NULL));       //�����������
	array1[i].target = rand() % 26 + 'A';   //�������26��Ӣ�Ĵ�д��ĸ
	//С�ܲ��ռ�Խ�࣬���伦�ٶ�Խ��
	if (radish_sum <= 50)
	{
		array1[i].speed = rand() % 4 + WINDOW_WIDE / 250;
	}
	if (radish_sum > 50 && radish_sum <= 100)
	{
		array1[i].speed = rand() % 6 + WINDOW_WIDE / 250;
	}
	if (radish_sum > 100 && radish_sum <= 200)
	{
		array1[i].speed = rand() % 4 + WINDOW_WIDE / 200;
	}
	if (radish_sum > 200 && radish_sum <= 300)
	{
		array1[i].speed = rand() % 4 + WINDOW_WIDE / 150;
	}
	if (radish_sum > 300 && radish_sum <= 400)
	{
		array1[i].speed = rand() % 5 + WINDOW_WIDE / 100;
	}
	if (radish_sum > 400 && radish_sum <= 500)
	{
		array1[i].speed = rand() % 6 + WINDOW_WIDE / 100;
	}
	if (radish_sum > 500)
	{
		array1[i].speed = rand() % 6 + WINDOW_WIDE / 100;
	}
	//���伦�������λ��
	array1[i].x = rand() % 15 * WINDOW_LONG / 20 + 2 * WINDOW_LONG / 20;
	array1[i].y = rand() % 1000 - 1050;
	//��ĸ�ظ� ��������
	while (1)
	{
		int flag = 0;
		int j = 1;
		int k = 0;
		while (j < ji_num)
		{
			if (array1[i].target == array1[(i + j) % ji_num].target)
			{
				flag = 1;
				break;
			}
			while (k < RADISH_MAX)
			{
				if (array1[i].target == array[k].target)
				{
					flag = 1;
					break;
				}
				k++;
			}
			j++;
		}
		if (flag == 1)
		{
			array1[i].target = rand() % 26 + 'A';   //�������26��Ӣ�Ĵ�д��ĸ
		}
		else
		{
			break;
		}

	}

	//���伦����λ���غ����� ��������
	while (1)
	{
		int flag = 0;
		int j = 1;


		while (j < ji_num)
		{
			if (array1[i].x >= array1[(i + j) % ji_num].x - JI_LONG && array1[i].x <= array1[(i + j) % ji_num].x + 2 * JI_LONG &&
				array1[i].y >= array1[(i + j) % ji_num].y - 2 * JI_WIDE && array1[i].y <= array1[(i + j) % ji_num].y + 2 * JI_WIDE)
			{
				flag = 1;
			}
			j++;
		}
		if (flag == 1)
		{
			array1[i].x = rand() % 15 * WINDOW_LONG / 20 + 2 * WINDOW_LONG / 20;
			array1[i].y = rand() % 1000 - 1050;
		}
		else
		{
			break;
		}
	}
}



//���̲߳��ű�ը��Ч1
DWORD WINAPI PlayBombMusic(LPVOID LpParale)
{
	mciSendString(L"open ����\\����.mp3 alias bomb", nullptr, 0, nullptr);
	mciSendString(L"play bomb wait", nullptr, 0, nullptr);
	mciSendString(L"close bomb", nullptr, 0, nullptr);
	return 0;
}
//���̲߳��ű�ը��Ч2
DWORD WINAPI PlayJiMusic(LPVOID LpParale)
{
	mciSendString(L"open ����\\���伦.mp3 alias bomb1", nullptr, 0, nullptr);
	mciSendString(L"play bomb1 wait", nullptr, 0, nullptr);
	mciSendString(L"close bomb1", nullptr, 0, nullptr);
	return 0;
}

//������Դ
void LoadResource()
{
	loadimage(&menubackground, L"ͼƬ\\�˵�����ͼ.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&gamebackground, L"ͼƬ\\��Ϸ����ͼ.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&setbackground, L"ͼƬ\\���ñ���ͼ.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&overbackground, L"ͼƬ\\��Ϸ��������ͼ.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&scorebackground, L"ͼƬ\\��߼�¼����ͼ.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&radish[0], L"ͼƬ\\�ܲ�����ͼ.jpg", RADISH_LONG, RADISH_WIDE);
	loadimage(&radish[1], L"ͼƬ\\�ܲ�����ͼ.jpg", RADISH_LONG, RADISH_WIDE);
	loadimage(&ji[0], L"ͼƬ\\������ͼ.jpg", JI_LONG, JI_WIDE);
	loadimage(&ji[1], L"ͼƬ\\������ͼ.jpg", JI_LONG, JI_WIDE);
}
