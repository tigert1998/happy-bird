#include "world.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char** argv)
{

	World demo;
#if defined(ENABLE_AUDIO) && (defined (__WIN32) || defined (__WIN64) || defined (_MSC_VER))
	PlaySound(TEXT("./audio/sounds/weird.wav"), NULL, SND_ASYNC | SND_FILENAME);
#endif
	demo.Run();

}
