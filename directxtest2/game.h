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
	HWND hwnd;//ウィンドウハンドル
	HRESULT hr;//標準の戻り型
	LARGE_INTEGER timeStart;//パフォーマンスカウンタの開始値
	LARGE_INTEGER timeEnd;//パフォーマンスカウンタの終了値
	LARGE_INTEGER timerFreq;//パフォーマンスカウンタの周波数
	float frameTime;//最後のフレームに要した時間
	float fps;//フレームレート（１秒当たりのフレーム数）
	bool fpsOn;
	DWORD sleepTime;//フレーム間でスリープする時間
	bool paused;//ゲームが一時停止されている時間
	bool initialized;

public:
	//コンストラクタ
	Game();
	//デストラクタ
	virtual ~Game();
	//windowsメッセージハンドラー
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//ゲームの初期化
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

