#ifndef _A_H_
#define _A_H_


#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>  //easyxͼ�ο�
#include <easyx.h>
#include <string.h>
#include <time.h>      //�������
#include <conio.h>     //���뺯��,���鰴��
#include <mmsystem.h>  //��ý���

#pragma comment(lib,"winmm.lib")//���ؾ�̬��Դ�� ��Ƶ

void DrawMenu();			//���� ��Ϸ �˵�����
void DrawGameRun();			//���� ��Ϸ ���н���
void DrawGamePause();		//���� ��Ϸ ��ͣ����
void DrawGameSet();			//���� ��Ϸ ���ý���
void DrawGameOver();        //���� ��Ϸ ��������
void DrawHighScore();       //���� ��Ϸ ��߼�¼����

void MenuMouse();			//��Ϸ �˵�������꽻�� 
void RunKeyBoard(); 		//��Ϸ ���н�����̽���
void PauseMouse();			//��Ϸ ��ͣ������꽻��
void SetMouse();			//��Ϸ ���ý�����꽻��
void OverMouse();           //��Ϸ ����������꽻�� 
void RecordMouse();			//��Ϸ ��¼������꽻�� 

void ReadHighRecordFile();	//��ȡ��߼�¼�ļ�
void SaveHighRecordFile();	//������߼�¼�ļ�


void PlayGame();		                             //��Ϸ����(��Ϸ����)
void PlayGame1();                                    //��Ϸ�򵥴���ϰ
void LoadResource();                                 //������Դ(������ͼ����)

void PrintLetterRadish(int x, int y, char target);   //������ĸ��С�ܲ����
void InitArray(int i);                               //������ʼ��С�ܲ�
void PrintLetterJi(int x, int y, char target);
void InitArray1(int i);
DWORD WINAPI PlayBombMusic(LPVOID LpParale);         //���̲߳��ű�ը��Ч1
DWORD WINAPI PlayJiMusic(LPVOID LpParale);           //���̲߳��ű�ը��Ч2


#define WINDOW_LONG 1000   //���ڳ�x
#define WINDOW_WIDE 600    //���ڿ�y
#define RADISH_LONG 60     //�ܲ�ͼƬ�ĳ�
#define RADISH_WIDE 60     //�ܲ�ͼƬ�Ŀ�
#define JI_LONG 65         //���伦ͼƬ�ĳ�
#define JI_WIDE 90         //���伦ͼƬ�Ŀ�
#define RADISH_MAX  10	   //һ�����µ��ܲ���������
#define JI_MAX  10	       //һ�����µĽ��伦��������
//ȫ�ֱ���


#endif