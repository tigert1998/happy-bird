#include<audio.h>
ALCcontext *context;
ALCdevice *device;
static void list_audio_devices(const ALCchar *devices);
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

int test_audio_3()
{
	// device opening
	ALCdevice *device;
	device = alcOpenDevice(NULL);
	if (!device)
	{
		return -1;
	}
	// device enumeration
	ALboolean enumeration;

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
	{
		;
	}
	else
	{
		// enumeration supported
	}
	// Retrieving the device list
	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	// Error stack initialisation and usage
	ALCenum error;
	error = alGetError();
	if (error != AL_NO_ERROR){}
	// Context creation and initialization
	ALCcontext *context;
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
	{
		// failed to make context current
		// test for errors here using alGetError();
	}

	// Definining and configuring the listener
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
	// check for errors

	// Source generation
	ALuint source;
	alGenSources((ALuint)1, &source);
	// check for errors
	alSourcef(source, AL_PITCH, 1);
	// check for errors
	alSourcef(source, AL_GAIN, 1);
	// check for errors
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(source, AL_LOOPING, AL_FALSE);
	// check for errros
	
	// Buffer generation
	ALuint buffer;
	alGenBuffers((ALuint)1, &buffer);
	// check for errors
	
	// Loading an audio stream to a buffer
	/*method 1:*/
	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;
	alutLoadWAVFile("D:\\code\\happy-bird\\audio\\sounds\\bubble.wav", &format, &data, &size, &freq, &loop);
	// check for errors
	

	/*method 2:
	WaveInfo *wave;
	char *bufferData;
	int ret;

	wave = WaveOpenFileForReading("test.wav");
	if (!wave) {
		fprintf(stderr, "failed to read wave file\n");
		return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
		fprintf(stderr, "failed to seek wave file\n");
		return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
		perror("malloc");
		return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
		return -1;
	}
	
	
	alBufferData(buffer, format, data, size, freq);
	// check for errors*/

	// Binding a source to a buffer
	alSourcei(source, AL_BUFFER, buffer);

	// Playing the source

	/*method 1:
	alSourcePlay(source);

	// cleanup context
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	*/

	alSourcePlay(source);
	// check for errors
	ALint source_state =0;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	// check for errors
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		// check for errors
	}
	// Cleaning up context and resources
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	
	return 0;
}



static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}