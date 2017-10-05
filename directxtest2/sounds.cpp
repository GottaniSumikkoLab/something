//sound.cpp
#include"sounds.h"

//コンストラクタ
Sound::Sound() {
	xaudio = NULL;
	mastering_voice = NULL;
	source_voice = NULL;
}

//デストラクタ
Sound::~Sound() {
	this->cleanup();
}

//初期化
void Sound::initialize() {

	HRESULT hr;//インスタンスハンドル
	UINT32 flags = 0;//動作フラグ
	UINT32 deviceCount = 0;//オーディオデバイスの数
	XAUDIO2_DEVICE_DETAILS deviceDetails;//デバイスのプロパティの詳細
	int preferredDevice = 0;//選択したデバイス

	//COMの初期化
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		throw("CoinitializeEx");
	}

#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	//XAudio2エンジンのインスタンスの生成
	if (FAILED(hr = XAudio2Create(&xaudio, flags,XAUDIO2_DEFAULT_PROCESSOR))) {
		throw("XAudio2Create");
	}

	//オーディオデバイスの数取得
	xaudio->GetDeviceCount(&deviceCount);

	//最適なオーディオデバイスのインデックスを指定
	for (unsigned int i = 0; i < deviceCount; i++) {
		xaudio->GetDeviceDetails(i, &deviceDetails);
		if (deviceDetails.OutputFormat.Format.nChannels > 2) {
			preferredDevice = i;
		}
	}

	//マスターボイスの生成
	if (FAILED(hr = xaudio->CreateMasteringVoice(&mastering_voice,XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,0,preferredDevice,NULL))) {
		throw("CreateMasteringVOice");
		SAFE_RELEASE(xaudio);
	}
}

//ソースボイスの生成
void Sound::CreateSVoice() {
	
}

//エンジンの解放
void Sound::cleanup() {
	//ソースボイスの解放

	//マスターボイスの解放
	if (mastering_voice != NULL) {
		mastering_voice->DestroyVoice();
		mastering_voice = NULL;
	}
	//XAudio2インターフェースの解放
	if (xaudio != NULL) {
		xaudio->Release();
		xaudio = NULL;
	}
	CoUninitialize();
}

void Sound::soundtest() {

	HRESULT hr;

	WAVEFORMATEX format = { 0 };

	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 1;//モノラル
	format.wBitsPerSample = 16;//サンプル当たりの量子化16bit
	format.nSamplesPerSec = 44100;//サンプリング周波数
	format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
	format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;

	if (FAILED(hr = xaudio->CreateSourceVoice(&source_voice, &format))) {
		throw("CreateSourceVoice");
	}

	std::vector<BYTE>data(format.nAvgBytesPerSec * 1);//バッファ

	short* p = (short*)&data[0];
	for (size_t i = 0; i < data.size() / 2; i++) {
		float length = format.nSamplesPerSec / 440.0f;
		*p = (short)(32767 * sinf(i*PI / (length / 2)));
		p++;
	}

	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = data.size();//バッファのバイト数
	buffer.pAudioData = &data[0];//バッファの先頭アドレス
	buffer.Flags = XAUDIO2_END_OF_STREAM;//バッファの後ろに予期しないデータがある場合教える
	source_voice->SubmitSourceBuffer(&buffer);

	source_voice->Start();
	MessageBox(NULL, "演奏を終了します", "", MB_OK);

	source_voice->Stop();
	source_voice->DestroyVoice();

	return;
}
