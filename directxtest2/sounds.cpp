//sound.cpp
#include"sounds.h"

//�R���X�g���N�^
Sound::Sound() {
	xaudio = NULL;
	mastering_voice = NULL;
}

//�f�X�g���N�^
Sound::~Sound() {
	this->cleanup();
}

//������
void Sound::initialize() {

	HRESULT hr;//�C���X�^���X�n���h��
	UINT32 flags = 0;//����t���O
	UINT32 deviceCount = 0;//�I�[�f�B�I�f�o�C�X�̐�
	XAUDIO2_DEVICE_DETAILS deviceDetails;//�f�o�C�X�̃v���p�e�B�̏ڍ�
	int preferredDevice = 0;//�I�������f�o�C�X

	//COM�̏�����
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		throw("CoinitializeEx");
	}

#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	//XAudio2�G���W���̃C���X�^���X�̐���
	if (FAILED(hr = XAudio2Create(&xaudio, flags,XAUDIO2_DEFAULT_PROCESSOR))) {
		throw("XAudio2Create");
	}

	//�I�[�f�B�I�f�o�C�X�̐��擾
	xaudio->GetDeviceCount(&deviceCount);

	//�œK�ȃI�[�f�B�I�f�o�C�X�̃C���f�b�N�X���w��
	for (unsigned int i = 0; i < deviceCount; i++) {
		xaudio->GetDeviceDetails(i, &deviceDetails);
		if (deviceDetails.OutputFormat.Format.nChannels > 2) {
			preferredDevice = i;
		}
	}

	//�}�X�^�[�{�C�X�̐���
	if (FAILED(hr = xaudio->CreateMasteringVoice(&mastering_voice,XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,0,preferredDevice,NULL))) {
		throw("CreateMasteringVOice");
		//SAFE_RELEASE(xaudio)
	}
}

//�\�[�X�{�C�X�̐���
void Sound::CreateSVoice() {
	
}

//�G���W���̉��
void Sound::cleanup() {
	//�\�[�X�{�C�X�̉��

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
