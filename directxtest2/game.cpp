//game.cpp

#include"game.h"

//�O���[�o���ϐ�
static int id = 0;

//�R���X�g���N�^
Game::Game() {
	
	input = new Input();
	paused = false;
	graphics = NULL;
	sound = NULL;
	fps = 60.0f;
	fpsOn = false;
	initialized = false;
}
//�f�X�g���N�^
Game::~Game() {

	deleteAll();
	ShowCursor(true);
}

//�E�B���h�E���b�Z�[�W
LRESULT Game::messageHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

	if (initialized) {
		switch (msg) {

		case WM_KEYDOWN:case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return(0);
		case WM_KEYUP:case WM_SYSKEYUP:
			input->keyUp(wParam);
			return(0);
		case WM_CHAR:
			input->keyIn(wParam);
			return(0);
		case WM_MOUSEMOVE:
			input->mouseIn(lParam);
			return(0);
		case WM_INPUT:
			input->mouseRawIn(lParam);
			return(0);
		case WM_LBUTTONDOWN:
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return(0);
		case WM_LBUTTONUP:
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return(0);
		case WM_MBUTTONDOWN:
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return(0);
		case WM_MBUTTONUP:
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return(0);
		case WM_RBUTTONDOWN:
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return(0);
		case WM_RBUTTONUP:
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return(0);
		case WM_XBUTTONDOWN:case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return(0);
		case WM_DEVICECHANGE:
			input->checkControllers();
			return(0);
		case WM_CLOSE:
			id = MessageBox(hwnd, (LPCSTR)"�I�������`", (LPCSTR)"�I���m�F", MB_YESNO | MB_ICONQUESTION);
			if (id == IDYES) {
				DestroyWindow(hwnd);
			}
			return (0);
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);

		}//switch
	}//if
	return DefWindowProc(hwnd, msg, wParam, lParam);
}//hadler

//�Q�[���̏�����
void Game::initialize(HWND hw) {
	
	hwnd = hw;//windows�n���h���̃Z�[�u

	graphics = new Graphics();

	sound = new Sound();

	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	sound->initialize();

	input->initialize(hwnd, false);

	if (QueryPerformanceFrequency(&timerFreq) == false) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	}//if

	QueryPerformanceCounter(&timeStart);
	initialized = true;
}

//�Q�[���O���t�B�N�X�����_�����O
void Game::renderGame() {

	if (SUCCEEDED(graphics->beginScean())) {

		render();
		graphics->endSceen();
	}//if

	handleLostGraphicsDevice();
	graphics->showBackbuffer();
}

//���������O���t�B�b�N�f�o�C�X�̃n���h��
void Game::handleLostGraphicsDevice() {

	hr = graphics->getDeviceState();
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST) {
			Sleep(100);
			return;
		}//if
		else if(hr == D3DERR_DEVICENOTRESET){
			releaseAll();
			hr = graphics->reset();
			if (FAILED(hr)) {
				return;
				resetAll();
			}//if
		}//else if
		else {
			return;
		}//else
	}//if
}

//�^�C�}�[
void Game::run(HWND hwnd) {
	
	if (graphics == NULL) {
		return;
	}

	QueryPerformanceCounter(&timeEnd);
	//�t���[�����v�Z
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	if (frameTime < MIN_FRAME_TIME) {
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return;
	}//if

	if (frameTime > 0.0) {
		fps = (fps * 0.99f) + (0.01f / frameTime);
	}

	if (frameTime > MAX_FRAME_TIME) {
		frameTime = MAX_FRAME_TIME;
	}

	timeStart = timeEnd;
	input->readControllers();

	if (!paused) {
		update();
		ai();
		collisions();
		input->vibrateControllers(frameTime);
	}
	renderGame();
	input->clear(inputNS::KEYS_PRESSED);
}

void Game::releaseAll() {

}

void Game::resetAll() {

}

void Game::deleteAll() {
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(sound);
	SAFE_DELETE(input);
	initialized = false;
}