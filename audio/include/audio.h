#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<string>
#include<thread>
#include<future>
#include<chrono>
#include<random>
#include<windows.h>

#include "mmsystem.h"//��������ͷ�ļ�
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ��� 
//void playUtil(const char *fileName);
//void pre_play_audio(int argc, char **argv);
#define play_audio(s) PlaySound(TEXT(s), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
//void test_audio_2();
//int test_audio_3();


