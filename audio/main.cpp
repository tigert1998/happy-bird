
#include<windows.h>
#include<AL/alut.h>
#include<al.h>
#include<alc.h>
#include<efx.h>
#include<efx-creative.h>
#include<EFX-Util.h>
#include<xram.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char** plt;




/*
* This program loads and plays a variety of files.
*/

static void playFile(const char *fileName)
{
	ALuint buffer;
	ALuint source;
	ALenum error;
	ALint status;
	Sleep(2);
	/* Create an AL buffer from the given sound file. */
	buffer = alutCreateBufferFromFile(fileName);
	if (buffer == AL_NONE)
	{
		error = alutGetError();
		fprintf(stderr, "Error loading file: '%s'\n", alutGetErrorString(error));
		alutExit();
		exit(EXIT_FAILURE);
	}

	/* Generate a single source, attach the buffer to it and start playing. */
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	/* Normally nothing should go wrong above, but one never knows... */
	error = alGetError();
	if (error != ALUT_ERROR_NO_ERROR)
	{
		fprintf(stderr, "%s\n", alGetString(error));
		alutExit();
		exit(EXIT_FAILURE);
	}

	/* Check every 0.1 seconds if the sound is still playing. */
	do
	{
		alutSleep(0.1f);
		alGetSourcei(source, AL_SOURCE_STATE, &status);
	} while (status == AL_PLAYING);
}

int play(int argc, char **argv)
{
	/* Initialise ALUT and eat any ALUT-specific commandline flags. */
	if (!alutInit(&argc, argv))
	{
		ALenum error = alutGetError();

		fprintf(stderr, "%s\n", alutGetErrorString(error));
		exit(EXIT_FAILURE);
	}

	/* Check for correct usage. */
	if (argc != 2)
	{
		fprintf(stderr, "usage: playfile <fileName>\n");
		alutExit();
		exit(EXIT_FAILURE);
	}

	/* If everything is OK, play the sound file and exit when finished. */
	playFile(argv[1]);

	if (!alutExit())
	{
		ALenum error = alutGetError();

		fprintf(stderr, "%s\n", alutGetErrorString(error));
		exit(EXIT_FAILURE);
	}

	alutInit(&argc, argv);
	ALuint helloBuffer, helloSource;
	helloBuffer = alutCreateBufferHelloWorld();
	alGenSources(1, &helloSource);
	alSourcei(helloSource, AL_BUFFER, helloBuffer);
	alSourcePlay(helloSource);
	alutSleep(1);
	alutExit();
	return EXIT_SUCCESS;
}


int main()
{
	unsigned int audioId_1;
	alGenSources(1, &audioId_1);
	alSource3f(audioId_1, AL_POSITION, 45.0f, 5.0f, -18.0f);
	alSourcef(audioId_1, AL_GAIN, 0.8f);
	plt = (char**)malloc(2 * sizeof(char*));
	plt[1] = "D:\\ProgramsD\\matlab\\toolbox\\sl3d\\vrealm\\program\\sounds\\bubble.wav";
	play(2, plt);
	return 0;
}



