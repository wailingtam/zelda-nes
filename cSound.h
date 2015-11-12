#pragma once
#pragma comment (lib, "fmod_vc.lib")

#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"
#include <iostream>

#define NS		20
#define BACKGROUND	0
#define EFFECT		1

#define	DEFAULT_VOLUME	0.1f

#define MUSIC_CG			0
#define EFFECTS_CG			1

#define MUSIC_CHANNEL1		0
#define EFFECTS_CHANNEL1	1
#define EFFECTS_CHANNEL2	2
#define EFFECTS_CHANNEL3	3

#define SWORD			0
#define SWORD_SHOOT		1
#define	HURT			2
#define HIT				3
#define	KILL			4
#define	LOW_HEALTH		5
#define	DIE				6
#define BOOMERANG		7
#define OVERWORLD_BGM	10

class cSound {

public:

	cSound(void);
	~cSound(void);

	void Init();
	void LoadSound(int id, const char* filename, int type);
	void Play(int id, int channel);
	void PauseChannel(int channel);
	void SetVolume(int channelgroup, float volume);
	void ReleaseSound(int id);
	void UpdateSound();
	bool GetChannel3Paused();
	void SetChannel3Paused(bool p);

private:
	FMOD::System *system;
	FMOD::Sound* sounds[NS];
	FMOD::ChannelGroup *musicChannelGroup;
	FMOD::ChannelGroup *effectsChannelGroup;
	FMOD::Channel *musicChannel1;
	FMOD::Channel *effectsChannel1;
	FMOD::Channel *effectsChannel2;
	FMOD::Channel *effectsChannel3;
	float musicVolume;
	float effectsVolume;
	bool ch3Paused;
};