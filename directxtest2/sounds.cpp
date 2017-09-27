//sound.cpp
#include"sounds.h"

//コンストラクタ
Sound::Sound() {
	xaudio = NULL;
	mastering_voice = NULL;
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
		//SAFE_RELEASE(xaudio)
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
