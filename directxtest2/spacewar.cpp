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

//背景色変数
 static int red = 0, green = 0, blue = 0;

 std::string buf = "fps:";
 std::string fp;

//シード値初期化
inline void InitRand() {
	srand((unsigned int)time(NULL));
}

//カラー乱数
inline int Rand_RGB() {
	return rand() % 255;
}

//コンストラクタ
Spacewar::Spacewar()
{}

//デストラクタ
Spacewar::~Spacewar()
{
	releaseAll();//リソース解放
}

//初期化
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError
	const int BUF_SIZE = 20;
	fpsOn = true;

	//画像のロード(ファイルへのパスはconstantsに記載)
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

	//フォントの初期化
	if (text.initialize(graphics, POINT_SIZE, false, false, FONT) == false) {
		throw(GameError(gameErrorNS::FATAL_ERROR,"text"));
	}
	text.setFontColor(graphicsNS::WHITE);

	//背景
	backscreen.setX(0.0f);
	backscreen.setY(0.0f);
	backscreen.setScale((float)(GAME_WIDTH / backscreen.getWidth()));
	//きつねちゃん位置
	foxtyann.setX(GAME_WIDTH * 0.5f - foxtyann.getCenterX() * 0.5f);
	foxtyann.setY(0.0f);
	//きつねちゃんスケール
	foxtyann.setScale(0.5f);
	//きつねちゃん透過率
	return;
}

//更新
void Spacewar::update()
{

	//エスケープキーでアプリケーションを終了
	if (input->isKeyDown(ESC_KEY)==true) {
		PostQuitMessage(0);
	}

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

	//sioundtest
	sound->soundtest();
}

//ai
void Spacewar::ai()
{}

//衝突
void Spacewar::collisions()
{}

//描画
void Spacewar::render()
{
	graphics->spriteBegin();
	backscreen.draw();
	foxtyann.fadeIn(frameTime,FADEOUT_TIME);
	if (foxtyann.getfadejudge() == true) {
		foxtyann.draw();
	}
	if (fpsOn == true) {
		
		fp = buf + std::to_string((int)fps);

		text.print(fp, 0, 0);
	}
	graphics->spriteEnd();

}

//解放
void Spacewar::releaseAll()
{

	backscreenTexture.onLostDevice();
	foxtyannTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//リセット
void Spacewar::resetAll()
{
	backscreenTexture.onResetDevice();
	foxtyannTexture.onResetDevice();
	Game::resetAll();
	return;
}
