//sounds.h
#ifndef _SOUNDS_H
#define _SOUNDS_H
#define WIN_DCOM //CoInitializeEx�֐��g�p��錾

#include<xaudio2.h>
#include<vector>
#include"constants.h"
#include"wave.h"

//�V���O���g��
class Sound {

private:
	static IXAudio2* xaudio;//XAudio2�C���^�[�t�F�[�X
	static IXAudio2MasteringVoice* mastering_voice;//�}�X�^�[�{�C�X
	IXAudio2SourceVoice* source_voice;//�\�[�X�{�C�X
	WAVEFORMATEX wave_format;
	Wave wave_sound;
	bool select;

private:
	Sound();//�R���X�g���N�^
	Sound(const Sound&);
	~Sound();
	Sound& operator=(const Sound&);
	bool CreateSVoice();//�\�[�X�{�C�X�̐���

public:
	static Sound* GetInst();//�C���X�^���X�̐���
	bool initialize();//������
	bool LoadWaveFile(const char *fp);
	bool Submit();//�f�[�^���L���[�ɑ���
	void Play();//�Đ�
	void Stop();//��~
	void cleanup();//���
};
#endif
