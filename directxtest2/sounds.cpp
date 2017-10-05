//sound.cpp
#include"sounds.h"

//�R���X�g���N�^
Sound::Sound() {
	xaudio = NULL;
	mastering_voice = NULL;
	source_voice = NULL;
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
		SAFE_RELEASE(xaudio);
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

void Sound::soundtest() {

	HRESULT hr;

	WAVEFORMATEX format = { 0 };

	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 1;//���m����
	format.wBitsPerSample = 16;//�T���v��������̗ʎq��16bit
	format.nSamplesPerSec = 44100;//�T���v�����O���g��
	format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
	format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;

	if (FAILED(hr = xaudio->CreateSourceVoice(&source_voice, &format))) {
		throw("CreateSourceVoice");
	}

	std::vector<BYTE>data(format.nAvgBytesPerSec * 1);//�o�b�t�@

	short* p = (short*)&data[0];
	for (size_t i = 0; i < data.size() / 2; i++) {
		float length = format.nSamplesPerSec / 440.0f;
		*p = (short)(32767 * sinf(i*PI / (length / 2)));
		p++;
	}

	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = data.size();//�o�b�t�@�̃o�C�g��
	buffer.pAudioData = &data[0];//�o�b�t�@�̐擪�A�h���X
	buffer.Flags = XAUDIO2_END_OF_STREAM;//�o�b�t�@�̌��ɗ\�����Ȃ��f�[�^������ꍇ������
	source_voice->SubmitSourceBuffer(&buffer);

	source_voice->Start();
	MessageBox(NULL, "���t���I�����܂�", "", MB_OK);

	source_voice->Stop();
	source_voice->DestroyVoice();

	return;
}
