//Wave.cpp
#include"wave.h"

Wave::Wave() {
	size = 0;
	format = { 0 };
	hMmio = NULL;
}

Wave::~Wave() {
	this->Close();
}

bool Wave::LoadWave(const char* fp) {
	
	//ファイルパス未定義
	filepath = (LPSTR)fp;
	if (filepath == 0) {
		throw("filepath");
		return false;
	}
	
	MMIOINFO mmioInfo;

	//Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio) {
		throw("open");
		return false;
	}

	//RIFFチャンクサーチ
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		throw("RIFF");
		return false;
	}

	//フォーマットチャンクサーチ
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_ERROR) {
		mmioClose(hMmio, 0);
		throw("fmt");
		return false;
	}

	//WAVEFORMATEEX構造体格納
	DWORD fmsize = formatChunk.cksize;
	size = mmioRead(hMmio, (HPSTR)&format, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		throw("size");
		return false;
	}
	mmioAscend(hMmio, &formatChunk, 0);

	//データチャンクサーチ
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		throw("data");
		return false;
	}

	//音データを格納
	wData.resize(dataChunk.cksize);
	size = mmioRead(hMmio, reinterpret_cast<HPSTR>(&wData[0]), dataChunk.cksize);
	if (size != dataChunk.cksize) {
		throw("wData");
		return false;
	}

	this->Close();
	return true;
}

bool Wave::Close() {
	//ファイルを閉じる
	if (mmioClose(hMmio, 0) == MMIOERR_CANNOTWRITE) {
		OutputDebugString("WAVEファイルを閉じることはできませんでした。");
		return false;
	}
	return true;
}

const WAVEFORMATEX& Wave::GetFormatEX()const{
	return format;
}

const BYTE* Wave::GetWaveData() const{
	return &wData[0];
}

const size_t Wave::GetWaveSize() const {
	return wData.size();
}
