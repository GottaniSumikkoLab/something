//wave.h
#ifndef _WAVE_H
#define _WAVE_H
#pragma comment(lib,"winmm.lib")

#include<windows.h>
#include<vector>
#include<math.h>
#include<mmsystem.h>

class Wave {

private:
	DWORD size;//データサイズ
	WAVEFORMATEX format;//WAVEFORMATEX構造体
	LPSTR filepath;//ファイルパス
	std::vector<BYTE> wData;//WAVEデータ
	HMMIO hMmio;

public:
	Wave();
	virtual ~Wave();
	bool LoadWave(const char *fp);
	const WAVEFORMATEX& GetFormatEX() const;
	const BYTE* GetWaveData() const;
	const size_t GetWaveSize() const;
	bool Close();
};

#endif

