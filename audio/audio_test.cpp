#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include<cstdint>
#include<future>
#include<iostream>
#include<windows.h>
#include<mmreg.h>
#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库 
#define FOURCC_WAVE mmioFOURCC('W', 'A', 'V', 'E')
#define FOURCC_FMT mmioFOURCC('f', 'm', 't', ' ')
#define FOURCC_DATA mmioFOURCC('d', 'a', 't', 'a')

DWORD CalculateWaveLength(LPTSTR FileName)
{
	MMIOINFO mmioinfo = { 0 };
	MMCKINFO mmckinfoRIFF = { 0 };
	MMCKINFO mmckinfoFMT = { 0 };
	MMCKINFO mmckinfoDATA = { 0 };
	MMRESULT mmr;
	WAVEFORMATEXTENSIBLE waveFormat = { 0 };
	HMMIO mmh = mmioOpen(FileName, &mmioinfo, MMIO_DENYNONE | MMIO_READ);
	if (mmh == NULL)
	{
		printf("Unable to open %s: %x\n", FileName, mmioinfo.wErrorRet);
		exit(1);
	}

	mmr = mmioDescend(mmh, &mmckinfoRIFF, NULL, 0);
	if (mmr != MMSYSERR_NOERROR && mmckinfoRIFF.ckid != FOURCC_RIFF)
	{
		printf("Unable to find RIFF section in .WAV file, possible file format error: %x\n", mmr);
		exit(1);
	}
	if (mmckinfoRIFF.fccType != FOURCC_WAVE)
	{
		printf("RIFF file %s is not a WAVE file, possible file format error\n", FileName);
		exit(1);
	}

	// It's a wave file, read the format tag.
	mmckinfoFMT.ckid = FOURCC_FMT;
	mmr = mmioDescend(mmh, &mmckinfoFMT, &mmckinfoRIFF, MMIO_FINDCHUNK);
	if (mmr != MMSYSERR_NOERROR)
	{
		printf("Unable to find FMT section in RIFF file, possible file format error: %x\n", mmr);
		exit(1);
	}
	// The format tag fits into a WAVEFORMAT, so read it in.
	if (mmckinfoFMT.cksize >= sizeof(WAVEFORMAT))
	{
		// Read the requested size (limit the read to the existing buffer though).
		LONG readLength = mmckinfoFMT.cksize;
		if (mmckinfoFMT.cksize >= sizeof(waveFormat))
		{
			readLength = sizeof(waveFormat);
		}
		if (readLength != mmioRead(mmh, (char *)&waveFormat, readLength))
		{
			printf("Read error reading WAVE format from file\n");
			exit(1);
		}
	}
	if (waveFormat.Format.wFormatTag != WAVE_FORMAT_PCM)
	{
		printf("WAVE file %s is not a PCM format file, it's a %d format file\n", FileName, waveFormat.Format.wFormatTag);
		exit(1);
	}
	// Pop back up a level
	mmr = mmioAscend(mmh, &mmckinfoFMT, 0);
	if (mmr != MMSYSERR_NOERROR)
	{
		printf("Unable to pop up in RIFF file, possible file format error: %x\n", mmr);
		exit(1);
	}

	// Now read the data section.
	mmckinfoDATA.ckid = FOURCC_DATA;
	mmr = mmioDescend(mmh, &mmckinfoDATA, &mmckinfoRIFF, MMIO_FINDCHUNK);
	if (mmr != MMSYSERR_NOERROR)
	{
		printf("Unable to find FMT section in RIFF file, possible file format error: %x\n", mmr);
		exit(1);
	}
	// Close the handle, we're done.
	mmr = mmioClose(mmh, 0);
	//
	// We now have all the info we need to calculate the file size. Use 64bit math 
	// to avoid potential rounding issues.
	//
	LONGLONG fileLengthinMS = mmckinfoDATA.cksize * 1000;
	fileLengthinMS /= waveFormat.Format.nAvgBytesPerSec;
	return fileLengthinMS;
}





