//sounds.h
#ifndef _SOUNDS_H
#define _SOUNDS_H
#define WIN_DCOM //CoInitializeEx関数使用を宣言

#include<xaudio2.h>
#include<vector>
#include"constants.h"
#include"wave.h"

//シングルトン
class Sound {

private:
	static IXAudio2* xaudio;//XAudio2インターフェース
	static IXAudio2MasteringVoice* mastering_voice;//マスターボイス
	IXAudio2SourceVoice* source_voice;//ソースボイス
	WAVEFORMATEX wave_format;
	Wave wave_sound;
	bool select;

private:
	Sound();//コンストラクタ
	Sound(const Sound&);
	~Sound();
	Sound& operator=(const Sound&);
	bool CreateSVoice();//ソースボイスの生成

public:
	static Sound* GetInst();//インスタンスの生成
	bool initialize();//初期化
	bool LoadWaveFile(const char *fp);
	bool Submit();//データをキューに送る
	void Play();//再生
	void Stop();//停止
	void cleanup();//解放
};
#endif
