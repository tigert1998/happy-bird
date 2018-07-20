#include<audio.h>
ALCcontext *context;
ALCdevice *device;

void test_audio_2()
{
	//auto device = alcOpenDevice(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	ALCdevice *device;
	const ALCchar *initstr =
		(const ALCchar *) "'( ( devices '( native null ) ) )";

	device = alcOpenDevice(initstr);
	if (device == NULL)
	{
		// Handle Exception
	}
	//Create a context
	context = alcCreateContext(device, NULL);
	//Set active context
	alcMakeContextCurrent(context);
	// Clear Error Code
	alGetError();



	char*     alBuffer;             //data for the buffer
	ALenum alFormatBuffer;    //buffer format
	ALsizei   alFreqBuffer;       //frequency
	long       alBufferLen;        //bit depth
	ALboolean    alLoop;         //loop
	unsigned int alSource;      //source
	unsigned int alSampleSet;

	//load the wave file
	alutLoadWAVFile("D:\\code\\happy-bird\\audio\\sounds\\bubble.wav",
				&alFormatBuffer, (void **)&alBuffer, (ALsizei*)&alBufferLen, &alFreqBuffer, &alLoop);
	//create a source
	alGenSources(1, &alSource);

	//create  buffer
	alGenBuffers(1, &alSampleSet);
	//alutCreateBufferFromFile("D:\\code\\happy-bird\\audio\\sounds\\bubble.wav");
	//put the data into our sampleset buffer
	alBufferData(alSampleSet, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
	

	//assign the buffer to this source
	alSourcei(alSource, AL_BUFFER, alSampleSet);

	//release the data
	alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

	alSourcei(alSource, AL_LOOPING, AL_TRUE);

	//play
	alSourcePlay(alSource);

	//to stop
	alSourceStop(alSource);


	alDeleteSources(1, &alSource);

	//delete our buffer
	alDeleteBuffers(1, &alSampleSet);

	context = alcGetCurrentContext();

	//Get device for active context
	device = alcGetContextsDevice(context);

	//Disable context
	alcMakeContextCurrent(NULL);

	//Release context(s)
	alcDestroyContext(context);

	//Close device
	alcCloseDevice(device);
}
