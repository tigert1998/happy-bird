#include "world.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char** argv)
{

	World demo;
	//std::string xxx("play D:\\code\\happy-bird\\audio\\sounds\\bubble.wav");
//#if defined(ENABLE_AUDIO) && (defined (__WIN32) || defined (__WIN64) || defined (_MSC_VER))
	//mciSendString(TEXT("play D:\\code\\happy-bird\\audio\\sounds\\bubble.wav"), 0, 0, 0);
	//mciSendString((LPCSTR)("play D:\\code\\happy-bird\\audio\\sounds\\bubble.wav"), 0, 0, 0);
	
	//mciSendString((LPCSTR)(xxx.c_str()), 0, 0, 0);
	//mciSendString((LPCSTR)(xxx.c_str()), 0, 0, 0);
	//mciSendString((LPCSTR)(xxx.c_str()), 0, 0, 0);
	//mciSendString((LPCWSTR)("D:\\code\\happy-bird\\audio\\sounds\\bubble.wav"), 0, 0, 0);
//	PlaySound(TEXT("./audio/sounds/weird.wav"), NULL, SND_ASYNC | SND_FILENAME);
//#endif
	demo.Run();
	return 0;
}
