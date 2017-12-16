/*
 * playm.h
 *
 *  Created on: 2017. 12. 6.
 *      Author: craftingmod
 */
#include "fmod.h"

#ifndef PLAYM_H_
#define PLAYM_H_

void playm_createSystem(FMOD_SYSTEM **);
void playm_playSound(FMOD_SYSTEM *,FMOD_SOUND **,char [200], int);
void playm_playSoundWC(FMOD_SYSTEM *,FMOD_SOUND **,FMOD_CHANNEL **,char [200], int);
void playm_pauseChannel(FMOD_CHANNEL *,int );
int playm_getPaused(FMOD_CHANNEL *);
void playm_stopSound(FMOD_SOUND *);
void playm_destorySystem(FMOD_SYSTEM *);

typedef FMOD_SYSTEM SndSystem;
typedef FMOD_SOUND SndSound;
typedef FMOD_CHANNEL SndChannel;

#endif /* PLAYM_H_ */
