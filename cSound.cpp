#include "cSound.h"

cSound::cSound(void)
{
}

cSound::~cSound(void)
{
}

void cSound::Init() {
	
	// Create FMOD interface object
	FMOD::System_Create(&system);

	// Initialise FMOD
	system->init(20, FMOD_INIT_NORMAL, 0);
	system->createChannelGroup(NULL, &musicChannelGroup);
	system->createChannelGroup(NULL, &effectsChannelGroup);
	musicChannel1->setChannelGroup(musicChannelGroup);
	effectsChannel1->setChannelGroup(effectsChannelGroup);
	effectsChannel2->setChannelGroup(effectsChannelGroup);
	musicVolume = 0.5f;
	effectsVolume = 0.5f;

}

void cSound::LoadSound(int id, const char* filename, int type) {
	switch (type) {
	case (BACKGROUND) :
		system->createStream(filename, FMOD_2D | FMOD_LOOP_NORMAL, 0, &sounds[id]);
		break;
	case (EFFECT):
		system->createSound(filename, FMOD_2D, 0, &sounds[id]);
	}
}

void cSound::Play(int id, int channel) {
	if (channel == MUSIC_CHANNEL1) {
		musicChannelGroup->setVolume(musicVolume);
		system->playSound(sounds[id], musicChannelGroup, false, &musicChannel1);
		//musicChannel->setPaused(false);
	}
	else if (channel == EFFECTS_CHANNEL1) {
		effectsChannelGroup->setVolume(effectsVolume);
		system->playSound(sounds[id], effectsChannelGroup, false, &effectsChannel1);
		//effectsChannel->setPaused(false);
	}
	else if (channel == EFFECTS_CHANNEL2) {
		effectsChannelGroup->setVolume(effectsVolume);
		system->playSound(sounds[id], effectsChannelGroup, false, &effectsChannel2);
		//effectsChannel->setPaused(false);
	}
}

void cSound::PauseChannel(int channel) {
	if (channel == MUSIC_CHANNEL1) {
		bool isPaused;
		musicChannel1->getPaused(&isPaused);
		musicChannel1->setPaused(!isPaused);
	}
	else if (channel == EFFECTS_CHANNEL1) {
		bool isPaused;
		effectsChannel1->getPaused(&isPaused);
		effectsChannel1->setPaused(!isPaused);
	}
	else if (channel == EFFECTS_CHANNEL2) {
		bool isPaused;
		effectsChannel2->getPaused(&isPaused);
		effectsChannel2->setPaused(!isPaused);
	}
}

void cSound::SetVolume(int channelgroup, float volume) {
	if (channelgroup == MUSIC_CG) {
		musicChannelGroup->setVolume(volume);
	}
	else if (channelgroup == EFFECTS_CG) {
		effectsChannelGroup->setVolume(volume);
	}
}

void cSound::ReleaseSound(int id) {
	sounds[id]->release();
}

void cSound::UpdateSound()
{
	system->update();
}