#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<string>
#include<windows.h>
#include "mmsystem.h"//��������ͷ�ļ�
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ��� 
void playUtil(const char *fileName);
void pre_play_audio(int argc, char **argv);
void play_audio(std::string s);