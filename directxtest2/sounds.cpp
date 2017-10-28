//sound.cpp
#include"sounds.h"

IXAudio2 *Sound::xaudio = NULL;
IXAudio2MasteringVoice *Sound::mastering_voice = NULL;

//�R���X�g���N�^
Sound::Sound():source_voice(NULL) {
	ZeroMemory(&wave_format, sizeof(wave_format));
	select = false;
}

//�f�X�g���N�^
Sound::~Sound() {
	this->cleanup();
}

Sound* Sound::GetInst() {
	static Sound Inst;
	return &Inst;
}

//������
bool Sound::initialize() {

	HRESULT hr;//�C���X�^���X�n���h��
	UINT32 flags = 0;//����t���O
	UINT32 deviceCount = 0;//�I�[�f�B�I�f�o�C�X�̐�
	XAUDIO2_DEVICE_DETAILS deviceDetails;//�f�o�C�X�̃v���p�e�B�̏ڍ�
	int preferredDevice = 0;//�I�������f�o�C�X

	//COM�̏�����
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		throw("CoinitializeEx");
		return false;
	}

#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	//XAudio2�G���W���̃C���X�^���X�̐���
	if (FAILED(hr = XAudio2Create(&xaudio, flags,XAUDIO2_DEFAULT_PROCESSOR))) {
		throw("XAudio2Create");
		return false;
	}
	if (select == true) {
		//�I�[�f�B�I�f�o�C�X�̐��擾
		xaudio->GetDeviceCount(&deviceCount);

		//�œK�ȃI�[�f�B�I�f�o�C�X�̃C���f�b�N�X���w��
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

	//�}�X�^�[�{�C�X�̐���
	if (FAILED(hr = xaudio->CreateMasteringVoice(&mastering_voice,XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,0,preferredDevice,NULL))) {
		throw("CreateMasteringVOice");
		SAFE_RELEASE(xaudio);
		return false;
	}
	OutputDebugString("�T�E���h�f�o�C�X�̏���������\n");
	return true;
}

bool Sound::CreateSVoice() {
	
	HRESULT hr;//�C���X�^���X�n���h��
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
		throw("���y�f�[�^���M���s");
		return false;
	}
	return true;
}

bool Sound::LoadWaveFile(const char *fp) {

	//Wave�N���X��p���ă��[�h
	if (wave_sound.LoadWave(fp) == NULL) {
		return false;
	}
	//�\�[�X�{�C�X�̐���
	if (this->CreateSVoice() == false){
		return false;
	}
	//�L���[�ɑ��M
	if (this->Submit() == false) {
		return false;
	}
	OutputDebugString("Wave�t�@�C�����[�h����");
	return true;
}

//�G���W���̉��
void Sound::cleanup() {
	//�\�[�X�{�C�X�̉��
	if (source_voice != NULL) {
		source_voice->Stop(0);
		source_voice->DestroyVoice();
		source_voice = NULL;
	}
	//�}�X�^�[�{�C�X�̉��
	if (mastering_voice != NULL) {
		mastering_voice->DestroyVoice();
		mastering_voice = NULL;
	}
	//XAudio2�C���^�[�t�F�[�X�̉��
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
