#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<string>
#include<cstdint>
#include<thread>
#include<future>
#include<chrono>
#include<random>
#include<windows.h>

#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库 
//void playUtil(const char *fileName);
//void pre_play_audio(int argc, char **argv);
#define play_audio(s) PlaySound(TEXT(s), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
//void test_audio_2();
//int test_audio_3();

class Audiofile
{
	std::string filename_;
	std::string abspath_;	// only VS2017 may use this member
	const uint32_t index;
public:

	Audiofile(std::string filename) :filename_(filename), abspath_("D:\\code\\happy-bird\\audio\\sounds\\" + filename),
		index(Timer::New()){}


	void play()
	{
		std::cout << abspath_ << std::endl;
		mciSendString((LPCSTR)(("play " +abspath_).c_str()), 0, 0, 0);
	}
	void play(float duration)
	{
		auto how_long= Timer::Query(index);
		if(how_long<=duration)
		mciSendString((LPCSTR)(("play " + abspath_).c_str()), 0, 0, 0);
	}
};


