//sound.h
#ifndef _SOUND_H
#define _SOUND_H
#include<xaudio2.h>

class Sound {

private:
	IXAudio2* xaudio;//XAudio2�C���^�[�t�F�[�X
	IXAudio2MasteringVoice* mastering_voice;//�}�X�^�[�{�C�X


public:
	Sound();
	virtual ~Sound();
	void initialize();
	void CreateSVoice();
	void cleanup();
};
#endif
