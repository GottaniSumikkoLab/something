//game.h

#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN
#pragma comment(lib,"winmm.lib")

#include<windows.h>
#include<Mmsystem.h>
#include"graphics.h"
#include"constants.h"
#include"gameError.h"
#include"input.h"
#include"text.h"

class Game {

protected:
	Graphics *graphics;
	Input *input;
	HWND hwnd;//�E�B���h�E�n���h��
	HRESULT hr;//�W���̖߂�^
	LARGE_INTEGER timeStart;//�p�t�H�[�}���X�J�E���^�̊J�n�l
	LARGE_INTEGER timeEnd;//�p�t�H�[�}���X�J�E���^�̏I���l
	LARGE_INTEGER timerFreq;//�p�t�H�[�}���X�J�E���^�̎��g��
	float frameTime;//�Ō�̃t���[���ɗv��������
	float fps;//�t���[�����[�g�i�P�b������̃t���[�����j
	bool fpsOn;
	DWORD sleepTime;//�t���[���ԂŃX���[�v���鎞��
	bool paused;//�Q�[�����ꎞ��~����Ă��鎞��
	bool initialized;

public:
	//�R���X�g���N�^
	Game();
	//�f�X�g���N�^
	virtual ~Game();
	//windows���b�Z�[�W�n���h���[
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//�Q�[���̏�����
	virtual void initialize(HWND);

	virtual void run(HWND);

	virtual void releaseAll();

	virtual void resetAll();

	virtual void deleteAll();
	
	virtual void renderGame();

	virtual void handleLostGraphicsDevice();

	Graphics* getGraphics() { return graphics; }

	Input* getInput() { return input; }

	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	virtual void update() = 0;

	virtual void ai() = 0;

	virtual void collisions() = 0;

	virtual void render() = 0;

};

#endif

