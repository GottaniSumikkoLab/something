//sound.cpp
#include"sounds.h"

IXAudio2 *Sound::xaudio = NULL;
IXAudio2MasteringVoice *Sound::mastering_voice = NULL;

//コンストラクタ
Sound::Sound():source_voice(NULL) {
	ZeroMemory(&wave_format, sizeof(wave_format));
	select = false;
}

//デストラクタ
Sound::~Sound() {
	this->cleanup();
}

Sound* Sound::GetInst() {
	static Sound Inst;
	return &Inst;
}

//初期化
bool Sound::initialize() {

	HRESULT hr;//インスタンスハンドル
	UINT32 flags = 0;//動作フラグ
	UINT32 deviceCount = 0;//オーディオデバイスの数
	XAUDIO2_DEVICE_DETAILS deviceDetails;//デバイスのプロパティの詳細
	int preferredDevice = 0;//選択したデバイス

	//COMの初期化
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		throw("CoinitializeEx");
		return false;
	}

#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	//XAudio2エンジンのインスタンスの生成
	if (FAILED(hr = XAudio2Create(&xaudio, flags,XAUDIO2_DEFAULT_PROCESSOR))) {
		throw("XAudio2Create");
		return false;
	}
	if (select == true) {
		//オーディオデバイスの数取得
		xaudio->GetDeviceCount(&deviceCount);

		//最適なオーディオデバイスのインデックスを指定
		for (unsigned int i = 0; i < deviceCount; i++) {
			xaudio->GetDeviceDetails(i, &deviceDetails);
			if (deviceDetails.OutputFormat.Format.nChannels > 2) {
				preferredDevice = i;
			}
		}
	}
	else {
		preferredDevice = 0;
	}

	//マスターボイスの生成
	if (FAILED(hr = xaudio->CreateMasteringVoice(&mastering_voice,XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,0,preferredDevice,NULL))) {
		throw("CreateMasteringVOice");
		SAFE_RELEASE(xaudio);
		return false;
	}
	OutputDebugString("サウンドデバイスの初期化成功\n");
	return true;
}

bool Sound::CreateSVoice() {
	
	HRESULT hr;//インスタンスハンドル
	if (FAILED(hr = xaudio->CreateSourceVoice(&source_voice,&wave_sound.GetFormatEX() ))) {
		throw("CreateSourceVoice");
		return false;
	}
	return true;
}

bool Sound::Submit() {
	XAUDIO2_BUFFER wSubmit = { 0 };
	wSubmit.AudioBytes = wave_sound.GetWaveSize();
	wSubmit.pAudioData = wave_sound.GetWaveData();
	wSubmit.Flags = XAUDIO2_END_OF_STREAM;
	if (source_voice->SubmitSourceBuffer(&wSubmit) != S_OK) {
		throw("音楽データ送信失敗");
		return false;
	}
	return true;
}

bool Sound::LoadWaveFile(const char *fp) {

	//Waveクラスを用いてロード
	if (wave_sound.LoadWave(fp) == NULL) {
		return false;
	}
	//ソースボイスの生成
	if (this->CreateSVoice() == false){
		return false;
	}
	//キューに送信
	if (this->Submit() == false) {
		return false;
	}
	OutputDebugString("Waveファイルロード成功");
	return true;
}

//エンジンの解放
void Sound::cleanup() {
	//ソースボイスの解放
	if (source_voice != NULL) {
		source_voice->Stop(0);
		source_voice->DestroyVoice();
		source_voice = NULL;
	}
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

void Sound::Play() {
	if (source_voice != NULL) {
		source_voice->Start(0);
	}
}

void Sound::Stop() {
	if (source_voice != NULL) {
		source_voice->Stop(0);
	}
}
