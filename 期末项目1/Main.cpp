#include "A.h"

//定义图片变量
IMAGE menubackground;     //菜单背景图
IMAGE gamebackground;     //游戏背景图
IMAGE setbackground;      //设置背景图
IMAGE overbackground;     //游戏结束背景图
IMAGE scorebackground;    //最高记录背景图
IMAGE radish[2];          //小萝卜的掩码图为0和背景图为1
IMAGE ji[2];              //降落鸡的掩码图为0和背景图为1


//字母结构体
struct letter
{
	int x, y;   //横纵坐标
	int target; //字母
	int speed;  //下落速度
};
struct letter array[RADISH_MAX];   //小萝卜数组


struct letter1
{
	int x, y;   //横纵坐标
	int target; //字母
	int speed;  //下落速度
};
struct letter1 array1[JI_MAX];     //降落鸡数组


//萝卜属性变量
int radish_num = 5;       //一次落下的小萝卜数，默认为5
int radish_sum = 0;       //一局获得小萝卜个数

//降落鸡属性变量
int ji_num = 1;           //一次落下的降落鸡，默认为1
int ji_flag = 0;          //无限模式旗帜

//计时变量
int game_second = 120;     //一局游戏的时间  (秒)，默认为120
clock_t  start, end;	   //游戏总计时变量
clock_t p_start, p_end;    //游戏暂停计时变量
int pause_time = 0;        //游戏暂停的总时间
int p_flag = 0;            //游戏暂停的旗帜   0为假  1为真

//最高记录
struct high_record
{
	int game_second;     //游戏的时间的模式
	int radish_sum;      //获得萝卜个数
};
struct high_record record[4] = { {120,0},{240,0},{360,0},{0,0} }; //分别 120s 240s 360s 无限 最高纪录



int main()
{
	LoadResource();   //加载资源
	MenuMouse();      //菜单
	return 0;
}

//游戏进行(游戏核心)
void PlayGame()
{

	initgraph(WINDOW_LONG, WINDOW_WIDE);
	mciSendString(L"close game", 0, 0, 0);
	mciSendString(L"open 音乐\\游戏背景音乐.mp3 alias game", 0, 0, 0);
	mciSendString(L"play game repeat", 0, 0, 0);

	//初始化生成带随机字母的array在下落处
	for (int i = 0; i < radish_num; i++)
	{
		InitArray(i);
	}
	//初始化生成带随机字母的array1在下落处
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


	start = clock();       //游戏时间开始

	while (1)
	{

		end = clock();     //游戏时间此时计时

		if (p_flag == 1)   //如果暂停，计算暂停总时间
		{
			pause_time += (int)(p_end - p_start) / 1000;
		}
		if ((int)(end - start) / 1000 - pause_time == game_second && ji_flag == 0)    //游戏时间结束判断
		{
			EndBatchDraw();     //换窗口得关闭
			OverMouse();
		}
		if (radish_sum < 0)    //游戏小萝卜负结束判断
		{
			EndBatchDraw();
			OverMouse();
		}
		p_flag = 0;   //暂停旗帜更新


		BeginBatchDraw();//双缓冲
		DrawGameRun();   //绘制游戏界面
		for (int i = 0; i < radish_num; i++)    //每个array以他的速度下落
		{
			array[i].y += array[i].speed;

		}
		for (int i = 0; i < radish_num; i++)    //将每个array变成为萝卜图+中间为字母的组合图形的小萝卜
		{
			PrintLetterRadish(array[i].x, array[i].y, array[i].target);
		}
		for (int i = 0; i < radish_num; i++)   	//小萝卜超出下边框，该小萝卜重新初始化
		{
			if (array[i].y >= WINDOW_WIDE)
			{
				InitArray(i);
			}
		}

		for (int i = 0; i < ji_num; i++)    //每个array1以他的速度下落
		{
			array1[i].y += array1[i].speed;

		}
		for (int i = 0; i < ji_num; i++)    //将每个array1变成为降落鸡图+中间为字母的组合图形的降落鸡
		{
			PrintLetterJi(array1[i].x, array1[i].y, array1[i].target);
		}

		for (int i = 0; i < ji_num; i++)   	//降落鸡超出下边框，小萝卜-1，该降落鸡重新初始化
		{
			if (array1[i].y >= WINDOW_WIDE)
			{
				CreateThread(nullptr, 0, PlayJiMusic, nullptr, 0, nullptr);
				radish_sum--;
				InitArray1(i);

			}
		}


		Sleep(50);   //延时50ms
		FlushBatchDraw();


		//键盘操作

		if (_kbhit())
		{
			EndBatchDraw();       //换窗口得关闭
			RunKeyBoard();        //进行游戏进行界面 键盘交互

		}

	}
}






//绘制游戏菜单界面
void DrawMenu()
{

	initgraph(WINDOW_LONG, WINDOW_WIDE);
	//菜单背景图片
	putimage(0, 0, &menubackground);

	//文字布局
	settextcolor(LIGHTBLUE);                            //设置字体颜色
	settextstyle(WINDOW_WIDE / 25, 0, L"华文楷体"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明
	outtextxy(0 * WINDOW_LONG / 2 + 20, WINDOW_WIDE / 7 + 8 * WINDOW_WIDE / 10, L"打字游戏1.1");

	settextcolor(WHITE);                            //设置字体颜色
	settextstyle(WINDOW_WIDE / 20, 0, L"华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明
	//中间位置时，将长分成2份，宽分成7份
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"开始游戏");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"游戏设置");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"最高纪录");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"关闭音乐");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"打词练习");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"退出游戏");

}


//绘制游戏进行界面
void DrawGameRun()
{
	//游戏背景图片
	putimage(0, 0, &gamebackground);


	settextcolor(LIGHTBLUE);
	settextstyle(20, 0, L"字魂24号-镇魂手书");
	setbkmode(TRANSPARENT);
	//窗口右边时，将长分成6份，宽分成15份
	if (ji_flag == 1)
	{
		outtextxy(5 * WINDOW_LONG / 6, 1 * WINDOW_WIDE / 15, L"无限模式");
		WCHAR str_sum[20];
		wsprintf(str_sum, L"萝卜数:%d个", radish_sum);
		outtextxy(5 * WINDOW_LONG / 6, 2 * WINDOW_WIDE / 15, str_sum);
	}
	else
	{
		outtextxy(5 * WINDOW_LONG / 6, 1 * WINDOW_WIDE / 15, L"限时模式");
		WCHAR str_second[20];//宽字符串unicode                               除以  转化成秒
		wsprintf(str_second, L"倒计时:%d秒", game_second - (int)(end - start) / CLOCKS_PER_SEC + pause_time);//将整型秒数转化成宽字符中文
		outtextxy(5 * WINDOW_LONG / 6, 2 * WINDOW_WIDE / 15, str_second);//宽字符位置格式化打印
		WCHAR str_sum[20];
		wsprintf(str_sum, L"萝卜数:%d个", radish_sum);
		outtextxy(5 * WINDOW_LONG / 6, 3 * WINDOW_WIDE / 15, str_sum);
	}
	outtextxy(5 * WINDOW_LONG / 6, 4 * WINDOW_WIDE / 15, L"游戏提示：");
	outtextxy(5 * WINDOW_LONG / 6, 5 * WINDOW_WIDE / 15, L"请按空格键");
}

//绘制游戏暂停界面
void DrawGamePause()
{
	//提示布局
	settextcolor(LIGHTBLUE);
	settextstyle(20, 0, L"字魂24号-镇魂手书");
	setbkmode(TRANSPARENT);                       //文字背景透明
	line(4 * WINDOW_LONG / 6, 6 * WINDOW_WIDE / 15, 4 * WINDOW_LONG / 6, WINDOW_WIDE);
	line(4 * WINDOW_LONG / 6, 6 * WINDOW_WIDE / 15, WINDOW_LONG - 5, 6 * WINDOW_WIDE / 15);
	line(6 * WINDOW_LONG / 6 - 5, 6 * WINDOW_WIDE / 15, 6 * WINDOW_LONG / 6 - 5, WINDOW_WIDE);
	outtextxy(4 * WINDOW_LONG / 6 + 5, 6 * WINDOW_WIDE / 15 + 5, L"游戏提示:");
	settextstyle(15, 0, L"字魂24号-镇魂手书");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 7 * WINDOW_WIDE / 15, L"1.你可以在规定时间内收集小萝卜，期间会有降");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 7 * WINDOW_WIDE / 15 + 25, L"  落鸡降落，他降落会偷走已经收集的小萝卜，");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 7 * WINDOW_WIDE / 15 + 50, L"  当时间耗尽或小萝卜被全部偷走时游戏结束。");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 9 * WINDOW_WIDE / 15, L"2.当收集的萝卜越来越多时，萝卜出现得更快，");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 9 * WINDOW_WIDE / 15 + 25, L"  降落鸡降落速度更快。");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 10 * WINDOW_WIDE / 15 + 15, L"3.	你可以在主菜单设置游戏的中萝卜下落数量，");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 10 * WINDOW_WIDE / 15 + 40, L"  时间长短或无限时间模式（初始有100个小萝卜");
	outtextxy(4 * WINDOW_LONG / 6 + 5, 10 * WINDOW_WIDE / 15 + 65, L"  这会吸引更多的降落鸡，保护好萝卜!）");




	//，或无限时间模式（初始有100个小萝卜，这会吸引更多的降落鸡，保护好萝卜）

	//文字布局
	settextcolor(WHITE);                            //设置字体颜色
	settextstyle(WINDOW_WIDE / 20, 0, L"华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"继续游戏");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"重新开始");
	outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"返回菜单");
}

//绘制游戏设置界面
void DrawGameSet()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	//背景图片
	putimage(0, 0, &setbackground);
	//文字属性设置
	settextcolor(WHITE);                            //设置字体颜色
	settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"请选择一次落下的    个数");
	//贴一个萝卜
	putimage(WINDOW_LONG / 7 + 245, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 15, &radish[0], SRCAND);//掩码图
	putimage(WINDOW_LONG / 7 + 245, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 15, &radish[1], SRCPAINT);//背景图
	//萝卜个数
	for (int i = 0; i <= 6; i++)//打印3到9个的选项
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
	//游戏时间选择
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"请选择游戏的时长");
	for (int i = 0; i <= 2; i++)//打印2 4 6选项
	{
		WCHAR str_minute[7];
		wsprintf(str_minute, L"%d分钟", 2 * (i + 1));
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
	outtextxy(WINDOW_LONG / 7 + 3 * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"无限模式");

	//返回菜单按钮
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
}

//绘制游戏结束界面
void DrawGameOver()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	putimage(0, 0, &overbackground);
	//文字属性设置
	settextcolor(WHITE);                            //设置字体颜色
	settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明   

	if (radish_sum < 0)
	{
		settextcolor(RED);
		outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"很遗憾，游戏挑战失败！");
	}
	else
	{
		settextcolor(WHITE);
		WCHAR str_num[20];
		wsprintf(str_num, L"本局收集的      个数为:%d个", radish_sum);
		outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, str_num);
		//贴一个萝卜
		putimage(WINDOW_LONG / 7 + 150, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 20, &radish[0], SRCAND);
		putimage(WINDOW_LONG / 7 + 150, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 - 20, &radish[1], SRCPAINT);

	}
	settextcolor(WHITE);
	WCHAR str_second[10];
	wsprintf(str_second, L"本局用时:%d秒", (int)(end - start) / 1000 - pause_time);
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, str_second);


	//再来一局按钮
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"再来一局");
	//返回菜单按钮
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");

}

//绘制最高记录界面
void DrawHighScore()
{
	initgraph(WINDOW_LONG, WINDOW_WIDE);
	putimage(0, 0, &scorebackground);
	//文字属性设置
	settextcolor(WHITE);                            //设置字体颜色
	settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明 

	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"萝卜达人最高纪录：");
	//用时
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"2分钟");
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"4分钟");
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"6分钟");
	//outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"无限模式         暂无最高纪录");
	//最高分
	WCHAR str_record[5];
	for (int i = 0; i <= 2; i++)
	{
		wsprintf(str_record, L"%d个", record[i].radish_sum);
		outtextxy(WINDOW_LONG / 2, WINDOW_WIDE / 7 + i * WINDOW_WIDE / 10 + 2 * WINDOW_WIDE / 10, str_record);
		putimage(WINDOW_LONG / 2 + 80, WINDOW_WIDE / 7 + i * WINDOW_WIDE / 10 - 20 + 2 * WINDOW_WIDE / 10, &radish[0], SRCAND);
		putimage(WINDOW_LONG / 2 + 80, WINDOW_WIDE / 7 + i * WINDOW_WIDE / 10 - 20 + 2 * WINDOW_WIDE / 10, &radish[1], SRCPAINT);
	}
	//返回菜单按钮
	outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
}


//游戏菜单界面鼠标交互  背景音乐设置
void MenuMouse()
{

	DrawMenu();      //绘制菜单界面
	//播放菜单背景音乐
	mciSendString(L"open 音乐\\菜单背景音乐.mp3 alias menu", 0, 0, 0);
	mciSendString(L"play menu repeat", 0, 0, 0);
	MOUSEMSG m;
	while (1)
	{
		//获取鼠标位置
		m = GetMouseMsg();

		//判断鼠标位置
		//开始游戏
		settextcolor(YELLOW);			//设置字体颜色
		if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{

			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"开始游戏");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				//关闭菜单背景音乐
				mciSendString(L"close menu", 0, 0, 0);
				//播放游戏进行背景音乐
				mciSendString(L"open 音乐\\游戏背景音乐.mp3 alias game", 0, 0, 0);
				mciSendString(L"play game repeat", 0, 0, 0);
				PlayGame();
			}
		}
		//游戏设置
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"游戏设置");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				SetMouse();
			}
		}
		//最高记录
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"最高纪录");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				RecordMouse();
			}
		}
		//关闭音乐
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"关闭音乐");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				//关闭菜单背景音乐
				mciSendString(L"close menu", 0, 0, 0);
			}
		}
		//打词练习
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"打词练习");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlayGame1();  //前往打字模块
			}
		}
		//退出游戏
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"退出游戏");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				exit(0);
			}
		}
		else
		{
			settextcolor(WHITE);	   //字颜色还原					
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 0 * WINDOW_WIDE / 10, L"开始游戏");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"游戏设置");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"最高纪录");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"关闭音乐");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"打词练习");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"退出游戏");
		}
	}
	_getch();//阻塞
	closegraph();
}

//游戏进行界面键盘交互
void RunKeyBoard()
{

	char userkey = _getch();  //接受按键

	for (int i = 0; i < radish_num; i++)
	{
		for (int i = 0; i < ji_num; i++)
		{
			if (userkey == array1[i].target || userkey == array1[i].target + 'a' - 'A')
			{
				InitArray1(i);

				CreateThread(nullptr, 0, PlayBombMusic, nullptr, 0, nullptr);

				return;   //此次键盘交互进行完毕
			}
		}
		if (userkey == array[i].target || userkey == array[i].target + 'a' - 'A')
		{
			InitArray(i);
			CreateThread(nullptr, 0, PlayBombMusic, nullptr, 0, nullptr);
			radish_sum++;
			break;   //此次键盘交互进行完毕
		}
		else if (userkey == 27)   //用户点击esc键
		{
			mciSendString(L"close game", 0, 0, 0);
			radish_sum = 0;
			MenuMouse();   //直接返回主菜单
		}
		else if (userkey == 32)   //用户点击space键  弹出暂停界面
		{
			p_start = clock();    //开始游戏暂停
			mciSendString(L"pause game", 0, 0, 0);
			PauseMouse();  //游戏暂停界面 鼠标交互
			mciSendString(L"resume game", 0, 0, 0);
			p_end = clock();      //结束游戏暂停
			return;  //若返回至此继续返回到游戏进行
		}
	}


}

//游戏暂停界面鼠标交互
void PauseMouse()
{
	MOUSEMSG m;

	while (1)//暂停
	{
		DrawGamePause();      //绘制暂停界面
		//获取鼠标位置
		m = GetMouseMsg();
		//文字属性设置
		settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
		setbkmode(TRANSPARENT);                         //文字背景透明
		settextcolor(YELLOW);	   //字体变色
		//继续游戏
		if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"继续游戏");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				p_flag = 1;  //暂停旗帜启动，计算暂停总用时
				return;
			}
		}

		//重新开始
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"重新开始");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlayGame();
			}
		}

		//返回菜单
		else if (m.x >= WINDOW_LONG / 2 - WINDOW_LONG / 15 &&
			m.x <= WINDOW_LONG / 2 + WINDOW_LONG / 15 &&
			m.y >= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 &&
			m.y <= WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10 + WINDOW_WIDE / 20)
		{
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"返回菜单");
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(L"close game", 0, 0, 0);
				MenuMouse();
			}
		}
		else
		{
			settextcolor(WHITE);	   //字体颜色还原					
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, L"继续游戏");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 2 * WINDOW_WIDE / 10, L"重新开始");
			outtextxy(WINDOW_LONG / 2 - WINDOW_LONG / 15, WINDOW_WIDE / 7 + 3 * WINDOW_WIDE / 10, L"返回菜单");
		}
	}
}
//游戏结束界面鼠标交互  背景音乐设置
void OverMouse()
{
	mciSendString(L"close game", 0, 0, 0);

	ReadHighRecordFile();//读取最高纪录文件
	SaveHighRecordFile();//保存最高纪录文件

	DrawGameOver();
	MOUSEMSG m;
	while (1)
	{
		//获取鼠标位置
		m = GetMouseMsg();
		//文字属性设置
		settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
		setbkmode(TRANSPARENT);                         //文字背景透明
		//再来一局按键设置
		if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//鼠标在的时候字体的颜色
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"再来一局");
			if (m.uMsg == WM_LBUTTONDOWN)
			{

				PlayGame();  //再来一局
			}

		}
		//返回菜单按键设置
		else if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//鼠标在的时候字体的颜色
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				MenuMouse();  //返回菜单
			}
		}
		else
		{
			settextcolor(WHITE);    //鼠标不在时字体的颜色
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 5 * WINDOW_WIDE / 10, L"再来一局");
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
		}

	}
}

//游戏设置界面鼠标交互
void SetMouse()
{
	DrawGameSet();
	MOUSEMSG m;
	while (1)
	{
		//获取鼠标位置
		m = GetMouseMsg();
		//文字属性设置
		settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
		setbkmode(TRANSPARENT);                         //文字背景透明
		//萝卜个数按键设置
		for (int i = 0; i <= 6; i++)
		{
			if (m.x >= WINDOW_LONG / 7 + i * 51 && m.y >= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 &&
				m.x <= WINDOW_LONG / 7 + i * 51 + 20 && m.y <= WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10 + 24)
			{
				settextcolor(YELLOW);	//鼠标在的时候字体的颜色
				//鼠标点击  改变个数
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					radish_num = i + 3;
				}
			}
			else if (i + 3 == radish_num)
			{
				settextcolor(YELLOW);   //把现在的个数字体设为黄色
			}
			else
			{
				settextcolor(WHITE);    //鼠标不在时字体的颜色
			}
			WCHAR str_num[2];
			wsprintf(str_num, L"%d", i + 3);
			outtextxy(WINDOW_LONG / 7 + i * 51, WINDOW_WIDE / 7 + 1 * WINDOW_WIDE / 10, str_num);
		}

		//游戏时长按键设置
		for (int i = 0; i <= 2; i++)
		{
			if (m.x >= WINDOW_LONG / 7 + i * 110 && m.y >= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 &&
				m.x <= WINDOW_LONG / 7 + i * 110 + 80 && m.y <= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 + 26)
			{
				settextcolor(YELLOW);	//鼠标在的时候字体的颜色
				//鼠标点击  改变时长
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					game_second = 60 * 2 * (i + 1);
					ji_flag = 0;
				}
			}
			else if (2 * (i + 1) == game_second / 60)
			{
				settextcolor(YELLOW);   //把现在的时长字体设为黄色
			}
			else
			{
				settextcolor(WHITE);    //鼠标不在时字体的颜色
			}
			WCHAR str_minute[7];
			wsprintf(str_minute, L"%d分钟", 2 * (i + 1));
			outtextxy(WINDOW_LONG / 7 + i * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, str_minute);
		}

		//分钟为无限模式
		if (m.x >= WINDOW_LONG / 7 + 3 * 110 && m.y >= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 3 * 110 + 80 && m.y <= WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//鼠标在的时候字体的颜色
			//鼠标点击  改变时长
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				ji_flag = 1;
				game_second = 0;//
			}
		}
		else if (1 == ji_flag)
		{
			settextcolor(YELLOW);   //把现在的时长字体设为黄色
		}
		else
		{
			settextcolor(WHITE);    //鼠标不在时字体的颜色
		}
		outtextxy(WINDOW_LONG / 7 + 3 * 110, WINDOW_WIDE / 7 + 4 * WINDOW_WIDE / 10, L"无限模式");

		//返回菜单按键设置
		if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//鼠标在的时候字体的颜色
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				MenuMouse();  //返回菜单
			}
		}
		else
		{
			settextcolor(WHITE);    //鼠标不在时字体的颜色
		}
		outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
	}
}


//游戏纪录界面鼠标交互  
void RecordMouse()
{
	//文件操作
	ReadHighRecordFile();
	DrawHighScore();
	MOUSEMSG m;
	while (1)
	{
		//获取鼠标位置
		m = GetMouseMsg();
		//文字属性设置
		settextstyle(WINDOW_WIDE / 20, 0, L"华文琥珀"); //设置文字样式
		setbkmode(TRANSPARENT);                         //文字背景透明
		//返回菜单按键设置
		if (m.x >= WINDOW_LONG / 7 && m.y >= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 &&
			m.x <= WINDOW_LONG / 7 + 110 && m.y <= WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10 + 26)
		{
			settextcolor(YELLOW);	//鼠标在的时候字体的颜色
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				MenuMouse();  //返回菜单
			}
		}
		else
		{
			settextcolor(WHITE);    //鼠标不在时字体的颜色
			outtextxy(WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
		}

	}
}





//读取最高纪录文件
void ReadHighRecordFile()
{
	FILE* fp;
	if ((fopen_s(&fp, "最高纪录.txt", "r")) != 0)
	{
		if ((fopen_s(&fp, "最高纪录.txt", "w")) != 0)//无创
		{
			printf("创建最高纪录文件失败\n");
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

//保存最高纪录文件
void SaveHighRecordFile()
{
	FILE* fp;
	if ((fopen_s(&fp, "最高纪录.txt", "r+")) != 0)
	{
		printf("打开最高纪录文件失败\n");
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



//绘制字母和小萝卜结合
void PrintLetterRadish(int x, int y, char target)
{

	settextcolor(BLACK);
	settextstyle(18, 0, L"MS Reference Sans Serif");  //西文字体
	setbkmode(TRANSPARENT);
	putimage(x, y, &radish[0], SRCAND);
	putimage(x, y, &radish[1], SRCPAINT);
	outtextxy(x + RADISH_LONG / 2 - RADISH_LONG / 17, y + RADISH_WIDE / 2 + RADISH_WIDE / 10, target);
}

//单个初始化小萝卜
void InitArray(int i)
{

	srand((unsigned int)time(NULL));       //生成随机种子
	array[i].target = rand() % 26 + 'A';   //随机生成26个英文大写字母

	//小萝卜收集越多，萝卜速度越快
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
	//字母重复 重新生成
	while (1)
	{
		int flag = 0;
		int j = 1;
		int k = 0;
		//取余法找出所有来比较
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
			array[i].target = rand() % 26 + 'A';   //随机生成26个英文大写字母
		}
		else
		{
			break;
		}

	}


	//萝卜随机下落位置
	array[i].x = rand() % 15 * WINDOW_LONG / 20 + 2 * WINDOW_LONG / 20;
	array[i].y = rand() % 100 - 150;
	//萝卜下落位置重合严重 重新生成
	while (1)
	{
		int flag = 0;
		int j = 1;

		//6个萝卜以下时
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
		//6个萝卜以上时
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

//绘制字母和降落鸡结合
void PrintLetterJi(int x, int y, char target)
{
	settextcolor(BLACK);
	settextstyle(18, 0, L"MS Reference Sans Serif");  //西文字体
	setbkmode(TRANSPARENT);
	putimage(x, y, &ji[0], SRCAND);
	putimage(x, y, &ji[1], SRCPAINT);
	outtextxy(x + JI_LONG / 2 - 6, y + JI_WIDE / 2 + 8, target);
}
//单个初始化降落鸡
void InitArray1(int i)
{
	srand((unsigned int)time(NULL));       //生成随机种子
	array1[i].target = rand() % 26 + 'A';   //随机生成26个英文大写字母
	//小萝卜收集越多，降落鸡速度越快
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
	//降落鸡随机下落位置
	array1[i].x = rand() % 15 * WINDOW_LONG / 20 + 2 * WINDOW_LONG / 20;
	array1[i].y = rand() % 1000 - 1050;
	//字母重复 重新生成
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
			array1[i].target = rand() % 26 + 'A';   //随机生成26个英文大写字母
		}
		else
		{
			break;
		}

	}

	//降落鸡下落位置重合严重 重新生成
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



//多线程播放爆炸音效1
DWORD WINAPI PlayBombMusic(LPVOID LpParale)
{
	mciSendString(L"open 音乐\\击落.mp3 alias bomb", nullptr, 0, nullptr);
	mciSendString(L"play bomb wait", nullptr, 0, nullptr);
	mciSendString(L"close bomb", nullptr, 0, nullptr);
	return 0;
}
//多线程播放爆炸音效2
DWORD WINAPI PlayJiMusic(LPVOID LpParale)
{
	mciSendString(L"open 音乐\\降落鸡.mp3 alias bomb1", nullptr, 0, nullptr);
	mciSendString(L"play bomb1 wait", nullptr, 0, nullptr);
	mciSendString(L"close bomb1", nullptr, 0, nullptr);
	return 0;
}

//加载资源
void LoadResource()
{
	loadimage(&menubackground, L"图片\\菜单背景图.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&gamebackground, L"图片\\游戏背景图.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&setbackground, L"图片\\设置背景图.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&overbackground, L"图片\\游戏结束背景图.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&scorebackground, L"图片\\最高纪录背景图.jpg", WINDOW_LONG, WINDOW_WIDE);
	loadimage(&radish[0], L"图片\\萝卜掩码图.jpg", RADISH_LONG, RADISH_WIDE);
	loadimage(&radish[1], L"图片\\萝卜背景图.jpg", RADISH_LONG, RADISH_WIDE);
	loadimage(&ji[0], L"图片\\鸡掩码图.jpg", JI_LONG, JI_WIDE);
	loadimage(&ji[1], L"图片\\鸡背景图.jpg", JI_LONG, JI_WIDE);
}
