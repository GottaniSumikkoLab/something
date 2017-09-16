//main.cpp

#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<stdlib.h>
#include<crtdbg.h>
#include"spaceWar.h"

//�v���g�^�C�v�錾
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();

//�O���[�o���ϐ�
HINSTANCE hinst = NULL;

//game�|�C���^
Spacewar *game = NULL;
HWND hwnd = NULL;

//windows main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//���������[�N�`�F�b�N
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	//windows���b�Z�[�W
	MSG msg;

	//�Q�[���C���X�^���X�̐���
	game = new Spacewar;
	
	//�~���[�e�b�N�X
	if (AnotherInstance()) {
		return false;
	}

	//�E�B���h�E���쐬
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow)) {
		return false;
	}
	
	try {
		
		//Game�N���X�p���N���X�̏�����
		game->initialize(hwnd);

		//���C���̃��b�Z�[�W���[�v
		bool done = false;
		//���C�����[�v
		while (!done) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					done = true;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}//if
			else {
				game->run(hwnd);//�Q�[�����[�v�Ǘ�(fps��)
			}//else
		}//while

		SAFE_DELETE(game);//���������
		return (msg.wParam);
	}//try
	catch (const GameError &err){
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}//catch
	catch (...) {
		MessageBox(NULL, "Unknown error occurred in game.", "Error", MB_OK);
	}//catch
	SAFE_DELETE(game);//���������
	return (0);
}

//�R�[���o�b�N�֐�
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	return(game->messageHandler(hWnd, msg, wParam, lParam));
}

//�E�B���h�E�̐������\�b�h
bool CreateMainWindow(HWND &hWnd,HINSTANCE hInstance, int nCmdShow) {
	
	//�E�B���h�E�\����
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

	//�E�B���h�E�X�^�C��
	DWORD style;
	if (FULLSCREEN) {
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP |WS_CAPTION;
	}//if
	else {
		style = WS_OVERLAPPEDWINDOW;
	}//else

	//�E�B���h�E�̐���
	hWnd = CreateWindow(
		CLASS_NAME,//�^�C�g���o�[�e�L�X�g
		CLASS_NAME,//�N���X��
		style,//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,//�E�B���h�E���゘
		CW_USEDEFAULT,//�E�B���h�E���゙
		GAME_WIDTH,//�E�B���h�E��
		GAME_HEIGHT,//�E�B���h�E����
		(HWND)NULL,//�e�E�B���h�E�̃n���h��
		(HMENU)NULL,//���j���[�Ȃ�
		hInstance,//�A�v���P�[�V�����C���X�^���X�ւ̃n���h��
		(LPVOID)NULL//�E�B���h�E�p�����[�^�Ȃ�
	);

	if (hWnd == NULL) {
		return false;//�G���[�ŏI��
	}

	if (!FULLSCREEN)             // if window
	{
		//GAME_WIDTH x GAME_HEIGHT���N���C�A���g���W
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);   //���݂̃N���C�A���g���W���擾
		MoveWindow(hWnd,								 //�E�B���h�E�n���h��
			0,                                           //��
			0,                                           //��
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    //�E
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), //��
			TRUE);                                       //�E�B���h�E���ĕ`��
	}

	ShowWindow(hWnd, nCmdShow);//�E�B���h�E��\��
	UpdateWindow(hWnd);//�E�B���h�E�v���[�W����WM_PAINT���b�Z�[�W�𑗂�

	return true;
}

//�~���[�e�b�N�X(���s�t�@�C�������C���X�^���X�̐����h�~)
bool AnotherInstance() {
	HANDLE ourMutex;
	ourMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161_XYZZY");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return true;//�ʂ̃C���X�^���X�����������ꍇ
	}//if
	return false;//���g���B��̃C���X�^���X�ł���ꍇ
}



