#ifndef _A_H_
#define _A_H_


#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>  //easyx图形库
#include <easyx.h>
#include <string.h>
#include <time.h>      //随机函数
#include <conio.h>     //输入函数,检验按键
#include <mmsystem.h>  //多媒体库

#pragma comment(lib,"winmm.lib")//加载静态资源库 音频

void DrawMenu();			//绘制 游戏 菜单界面
void DrawGameRun();			//绘制 游戏 进行界面
void DrawGamePause();		//绘制 游戏 暂停界面
void DrawGameSet();			//绘制 游戏 设置界面
void DrawGameOver();        //绘制 游戏 结束界面
void DrawHighScore();       //绘制 游戏 最高纪录界面

void MenuMouse();			//游戏 菜单界面鼠标交互 
void RunKeyBoard(); 		//游戏 进行界面键盘交互
void PauseMouse();			//游戏 暂停界面鼠标交互
void SetMouse();			//游戏 设置界面鼠标交互
void OverMouse();           //游戏 结束界面鼠标交互 
void RecordMouse();			//游戏 纪录界面鼠标交互 

void ReadHighRecordFile();	//读取最高纪录文件
void SaveHighRecordFile();	//保存最高纪录文件


void PlayGame();		                             //游戏进行(游戏核心)
void PlayGame1();                                    //游戏打单词练习
void LoadResource();                                 //加载资源(各背景图加载)

void PrintLetterRadish(int x, int y, char target);   //绘制字母和小萝卜结合
void InitArray(int i);                               //单个初始化小萝卜
void PrintLetterJi(int x, int y, char target);
void InitArray1(int i);
DWORD WINAPI PlayBombMusic(LPVOID LpParale);         //多线程播放爆炸音效1
DWORD WINAPI PlayJiMusic(LPVOID LpParale);           //多线程播放爆炸音效2


#define WINDOW_LONG 1000   //窗口长x
#define WINDOW_WIDE 600    //窗口宽y
#define RADISH_LONG 60     //萝卜图片的长
#define RADISH_WIDE 60     //萝卜图片的宽
#define JI_LONG 65         //降落鸡图片的长
#define JI_WIDE 90         //降落鸡图片的宽
#define RADISH_MAX  10	   //一次落下的萝卜最大的数量
#define JI_MAX  10	       //一次落下的降落鸡最大的数量
//全局变量


#endif