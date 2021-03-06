#include "fmod.h"
#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include "playm.h"

#define NULL (void *)0

void playm_createSystem(FMOD_SYSTEM **);
void playm_playSound(FMOD_SYSTEM *,FMOD_SOUND **,char [200], int);
void playm_playSoundWC(FMOD_SYSTEM *,FMOD_SOUND **,FMOD_CHANNEL **,char [200], int);
void playm_pauseChannel(FMOD_CHANNEL *,int);
int playm_getPaused(FMOD_CHANNEL *);
void playm_stopSound(FMOD_SOUND *);
void playm_destorySystem(FMOD_SYSTEM *);

void playm_createSystem(FMOD_SYSTEM **sys){
    FMOD_System_Create(sys);
    FMOD_System_Init(*sys,32,FMOD_INIT_NORMAL, NULL);
}
void getSoundPath(char out[600],char fileName[200]){
    //char mPath[420];
    char nowDir[399];
    char c;
    #ifdef _MSC_VER
    /* Windows */
    GetModuleFileName(NULL, nowDir, 398);
    char *pExe = &nowDir[0];
    while(*(pExe++) != '\0');
    while(*(--pExe) != '\\'){
        *(pExe) = '\0';
    }
    #else
    /* *nix */
    if(getcwd(nowDir,398) == NULL){
        nowDir[0] = '.';
        nowDir[1] = '/';
        nowDir[2] = '\0';
    }
    #endif

    char *pPath = nowDir;
    int i = 0;
    while((c=*pPath++) != '\0'){
    	*(out+i) = c;
     	i += 1;
    }
    c = *out+i-1;
    if(c != '/' && c != '\\'){
    	*(out+i) = '/';
    	i += 1;
    }
    pPath = fileName;
    while((c=*pPath++) != '\0'){
    	*(out+i) = c;
    	i += 1;
    }
    *(out+i) = '\0';
}
void playm_playSoundWC(FMOD_SYSTEM *sys,FMOD_SOUND **sound,FMOD_CHANNEL **channel,char fileName[200], int loop){
	char mPath[600] = {};
	getSoundPath(mPath,fileName);
    FMOD_System_CreateSound(sys,mPath,FMOD_DEFAULT, 0, sound);
    FMOD_Sound_SetMode(*sound,loop?FMOD_LOOP_NORMAL:FMOD_LOOP_OFF);
    if(loop){
        FMOD_Sound_SetLoopCount(*sound,2147483647);
    }
    FMOD_System_PlaySound(sys,*sound,0,0,channel);
}
void playm_playSound(FMOD_SYSTEM *sys,FMOD_SOUND **sound,char fileName[200], int loop){
	playm_playSoundWC(sys,sound,NULL,fileName,loop);
}
void playm_pauseChannel(FMOD_CHANNEL *channel,int pause){
	FMOD_Channel_SetPaused(channel,pause);
}
int playm_getPaused(FMOD_CHANNEL *channel){
	FMOD_BOOL pause = 0;
	FMOD_Channel_GetPaused(channel,&pause);
	return pause != 0;
}
void playm_stopSound(FMOD_SOUND *sound){
	FMOD_Sound_Release(sound);
}
void playm_destorySystem(FMOD_SYSTEM *sys){
    FMOD_System_Close(sys);
    FMOD_System_Release(sys);
}
