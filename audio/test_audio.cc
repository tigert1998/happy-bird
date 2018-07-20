/*
Play Compressed Formats With OpenAL
This tutorial will demonstrate how you can stream any mplayer-supported format with OpenAL, on almost any POSIX-compliant system.

The source code (downloadable here) is licensed under the LGPL (not that I'm doing anything special that I could claim copyright infringement over..).


Notice
It would be a good idea to first learn how OpenAL streaming works, if you don't already know. This tutorial is more focused on an OpenAL program using mplayer to decode the data and how to read from it.


First, some standard includes. Nothing special.
*/
#include <al.h>
#include <alc.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <io.h>  
#include <process.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    ALCdevice *dev;
    ALCcontext *ctx;
    struct stat statbuf;

    

    return 0;
}
 