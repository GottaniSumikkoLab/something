//spacewar.cpp

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine Part 1
// Chapter 4 spacewar.cpp v1.0
// Spacewar is the class we create.

#include "spaceWar.h"
#include<stdio.h>
#include<time.h>
#include<xaudio2.h>

//�w�i�F�ϐ�
 static int red = 0, green = 0, blue = 0;

//�V�[�h�l������
inline void InitRand() {
	srand((unsigned int)time(NULL));
}

//�J���[����
inline int Rand_RGB() {
	return rand() % 255;
}

//�R���X�g���N�^
Spacewar::Spacewar()
{}

//�f�X�g���N�^
Spacewar::~Spacewar()
{
	releaseAll();//���\�[�X���
}

//������
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError

	//�摜�̃��[�h(�t�@�C���ւ̃p�X��constants�ɋL��)
	if (!backscreenTexture.initialize(graphics, BACKSCREEN_IMAGE)) {
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing backscreen texture"));
	}

	if (!foxtyannTexture.initialize(graphics, FOXTYANN_IMAGE)) {
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing foxtyann"));
	}

	if (!backscreen.initialize(graphics, 0, 0, 0, &backscreenTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing "));
	}

	if (!foxtyann.initialize(graphics, 0, 0, 0, &foxtyannTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing foxtyannTexture"));
	}
	//�w�i
	backscreen.setX(0.0f);
	backscreen.setY(0.0f);
	backscreen.setScale((float)(GAME_WIDTH / backscreen.getWidth()));
	//���˂����ʒu
	foxtyann.setX(GAME_WIDTH * 0.5f - foxtyann.getCenterX() * 0.5f);
	foxtyann.setY(0.0f);
	//���˂����X�P�[��
	foxtyann.setScale(0.5f);
	//���˂���񓧉ߗ�
	return;
}

//�X�V
void Spacewar::update()
{

	//�G�X�P�[�v�L�[�ŃA�v���P�[�V�������I��
	if (input->isKeyDown(ESC_KEY)==true) {
		PostQuitMessage(0);
	}//if

	//gamepad
	if (input->getGamepadA(0)) {
		MessageBox(NULL, "A", "input", MB_OK);
	}
	if (input->getGamepadB(0)) {
		MessageBox(NULL, "B", "input", MB_OK);
	}
	input->vibrateControllers(frameTime);
	input->gamePadVibrateLeft(0, 65535, 1.0f);
	input->gamePadVibrateRight(0, 65535, 1.0f);
}

//ai
void Spacewar::ai()
{}

//�Փ�
void Spacewar::collisions()
{}

//�`��
void Spacewar::render()
{
	graphics->spriteBegin();
	backscreen.draw();
	foxtyann.fadeIn(frameTime,FADEOUT_TIME);
	if (foxtyann.getfadejudge() == true) {
		foxtyann.draw();
	}
	graphics->spriteEnd();

}

//���
void Spacewar::releaseAll()
{

	backscreenTexture.onLostDevice();
	foxtyannTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//���Z�b�g
void Spacewar::resetAll()
{
	backscreenTexture.onResetDevice();
	foxtyannTexture.onResetDevice();
	Game::resetAll();
	return;
}
