//sound.h
#ifndef _SOUND_H
#define _SOUND_H
#include<xaudio2.h>

class Sound {

private:
	IXAudio2* xaudio;//XAudio2インターフェース
	IXAudio2MasteringVoice* mastering_voice;//マスターボイス


public:
	Sound();
	virtual ~Sound();
	void initialize();
	void CreateSVoice();
	void cleanup();
};
#endif
