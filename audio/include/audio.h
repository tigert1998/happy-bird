#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<string>
#include<windows.h>
#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库 
void playUtil(const char *fileName);
void pre_play_audio(int argc, char **argv);
void play_audio(std::string s);