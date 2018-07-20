//#include<audio.h>
//#include<amp.h>
#include<thread>
#include<windows.h>
#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库 


/*void playUtil(const char *fileName)
{
	ALuint buffer;
	ALuint source;
	ALenum error;
	ALint status;
	Sleep(2);
	//Create an AL buffer from the given sound file. 
	buffer = alutCreateBufferFromFile(fileName);
	if (buffer == AL_NONE)
	{
		error = alutGetError();
		fprintf(stderr, "Error loading file: '%s'\n", alutGetErrorString(error));
		alutExit();
		exit(EXIT_FAILURE);
	}

	//Generate a single source, attach the buffer to it and start playing. 
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	// Normally nothing should go wrong above, but one never knows... 
	error = alGetError();
	if (error != ALUT_ERROR_NO_ERROR)
	{
		fprintf(stderr, "%s\n", alGetString(error));
		alutExit();
		exit(EXIT_FAILURE);
	}

	do
	{
		alutSleep(0.1f);
		alGetSourcei(source, AL_SOURCE_STATE, &status);
	} while (status == AL_PLAYING);
}

void pre_play_audio(int argc, char **argv)
{
	alutInit(&argc, argv);
	playUtil(argv[1]);
	alutExit();
}


void play_audio(std::string s)
{
	 
	unsigned int audioId_1;
	alGenSources(1, &audioId_1);
	alSource3f(audioId_1, AL_POSITION, 45.0f, 5.0f, -18.0f);
	alSourcef(audioId_1, AL_GAIN, 0.8f);
	char** plt = new char*[2];
	plt[1] = const_cast<char*> (s.c_str());
	//PlaySound("starwars.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	PlaySound(TEXT("D:\\code\\happy-bird\\audio\\sounds\\bubble.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	pre_play_audio(2, plt);
	delete[] plt;
}


*/

