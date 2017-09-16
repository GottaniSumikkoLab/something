//main.cpp

#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<stdlib.h>
#include<crtdbg.h>
#include"spaceWar.h"

//プロトタイプ宣言
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();

//グローバル変数
HINSTANCE hinst = NULL;

//gameポインタ
Spacewar *game = NULL;
HWND hwnd = NULL;

//windows main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//メモリリークチェック
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	//windowsメッセージ
	MSG msg;

	//ゲームインスタンスの生成
	game = new Spacewar;
	
	//ミューテックス
	if (AnotherInstance()) {
		return false;
	}

	//ウィンドウを作成
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow)) {
		return false;
	}
	
	try {
		
		//Gameクラス継承クラスの初期化
		game->initialize(hwnd);

		//メインのメッセージループ
		bool done = false;
		//メインループ
		while (!done) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					done = true;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}//if
			else {
				game->run(hwnd);//ゲームループ管理(fps等)
			}//else
		}//while

		SAFE_DELETE(game);//メモリ解放
		return (msg.wParam);
	}//try
	catch (const GameError &err){
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}//catch
	catch (...) {
		MessageBox(NULL, "Unknown error occurred in game.", "Error", MB_OK);
	}//catch
	SAFE_DELETE(game);//メモリ解放
	return (0);
}

//コールバック関数
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	return(game->messageHandler(hWnd, msg, wParam, lParam));
}

//ウィンドウの生成メソッド
bool CreateMainWindow(HWND &hWnd,HINSTANCE hInstance, int nCmdShow) {
	
	//ウィンドウ構造体
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = "MENUBAR";
	wc.lpszClassName = CLASS_NAME;

	if (RegisterClass(&wc) == 0) 
		return (false);

	//ウィンドウスタイル
	DWORD style;
	if (FULLSCREEN) {
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP |WS_CAPTION;
	}//if
	else {
		style = WS_OVERLAPPEDWINDOW;
	}//else

	//ウィンドウの生成
	hWnd = CreateWindow(
		CLASS_NAME,//タイトルバーテキスト
		CLASS_NAME,//クラス名
		style,//ウィンドウスタイル
		CW_USEDEFAULT,//ウィンドウ左上ｘ
		CW_USEDEFAULT,//ウィンドウ左上ｙ
		GAME_WIDTH,//ウィンドウ幅
		GAME_HEIGHT,//ウィンドウ高さ
		(HWND)NULL,//親ウィンドウのハンドル
		(HMENU)NULL,//メニューなし
		hInstance,//アプリケーションインスタンスへのハンドル
		(LPVOID)NULL//ウィンドウパラメータなし
	);

	if (hWnd == NULL) {
		return false;//エラーで終了
	}

	if (!FULLSCREEN)             // if window
	{
		//GAME_WIDTH x GAME_HEIGHTをクライアント座標
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);   //現在のクライアント座標を取得
		MoveWindow(hWnd,								 //ウィンドウハンドル
			0,                                           //左
			0,                                           //上
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    //右
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), //下
			TRUE);                                       //ウィンドウを再描画
	}

	ShowWindow(hWnd, nCmdShow);//ウィンドウを表示
	UpdateWindow(hWnd);//ウィンドウプロージャにWM_PAINTメッセージを送る

	return true;
}

//ミューテックス(実行ファイル複数インスタンスの生成防止)
bool AnotherInstance() {
	HANDLE ourMutex;
	ourMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161_XYZZY");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return true;//別のインスタンスが見つかった場合
	}//if
	return false;//自身が唯一のインスタンスである場合
}



