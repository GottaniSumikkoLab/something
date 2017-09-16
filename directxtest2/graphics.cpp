//graphics.cpp

// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 3 graphics.cpp v1.0

#include "graphics.h"

//コンストラクタ
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;    // width & height are replaced in initialize()
	height = GAME_HEIGHT;
	backcolor = SETCOLOR_ARGB(255, 0, 0, 0);
}

//デストラクタ
Graphics::~Graphics()
{
	releaseAll();
}

//解放
void Graphics::releaseAll()
{
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//初期化
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	//initialize Direct3D
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));
	}

	initD3Dpp();        // init D3D presentation parameters
	if (fullscreen)      // if full-screen mode
	{
		if (isAdapterCompatible()) {   // is the adapter compatible
									 // set the refresh rate with a compatible one
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		}//if
		else {
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"The graphics device does not support the specified resolution and/or format."));
		}//else
	}//if
						// determine if graphics card supports harware texturing and lighting and vertex shaders
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// If device doesn't support HW T&L or doesn't support 1.1 vertex 
	// shaders in hardware, then switch to software vertex processing.
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1)) {
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}// use software only processing
	else {
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}// use hardware only processing

														 //create Direct3D device
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d);
	
	if (FAILED(result)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));
	}

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result)) {
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error creating Direct3D sprite"));
	}

}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&d3dpp, sizeof(d3dpp));              // fill the structure with 0
														// fill in the parameters we need
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen) {                                  // if fullscreen
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		}// 24 bit color
		else{
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // use desktop setting
		}
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));

	}
}

//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // default to fail, replace on success
						// (this function will be moved in later versions)
						// Clear the backbuffer to lime green 
	// Display backbuffer to screen
	result = device3d->Present(NULL, NULL, NULL, NULL);
	return result;
}

bool Graphics::isAdapterCompatible() {
	
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,d3dpp.BackBufferFormat);

	for (UINT i = 0; i < modes; i++) {
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);

		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz) {

			return true;
		}//if
	}//for
	return false;
}

//消失デバイスのテスト
HRESULT Graphics::getDeviceState() {

	result = E_FAIL;
	if (device3d == NULL) {
		return result;
	}
	result = device3d->TestCooperativeLevel();
	return result;
}

//グラフィックデバイスのリセット
HRESULT Graphics::reset() {

	result = E_FAIL;
	initD3Dpp();
	result = device3d->Reset(&d3dpp);
	return result;
}

//シーンの開始
HRESULT Graphics::beginScean() {
	result = E_FAIL;
	if (device3d == NULL) {
		return result;
	}

	device3d->Clear(0, NULL, D3DCLEAR_TARGET, backcolor, 1.0F, 0);
	result = device3d->BeginScene();
	return result;
}

HRESULT Graphics::endSceen() {
	
	result = E_FAIL;
	if (device3d) {
		result = device3d->EndScene();
	}
	return result;
}

HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture) {

	//ファイル情報を読み込むための構造体
	D3DXIMAGE_INFO info;
	//失敗をデフォルトで成功時に置き換え
	result = E_FAIL;

	try {
		if (filename == NULL) {
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		//幅と高さをファイルから取得
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK) {
			return result;
		}

		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(
			device3d,//3Dデバイス
			filename,//画像ファイルの名前
			info.Width,//テクスチャの幅
			info.Height,//テクスチャの高さ
			1,//ミップマップのレベル
			0,//使用方法(Usage)
			D3DFMT_UNKNOWN,//サーフェイスフォーマット
			D3DPOOL_DEFAULT,//テクスチャ用のメモリの種類
			D3DX_DEFAULT,//画像フィルタ
			D3DX_DEFAULT,//ミップフィルタ
			transcolor,//透明色の色キー
			&info,//ビットマップファイル情報
			NULL,//カラーパレット
			&texture);//目的のテクスチャ
	}
	catch (...) {
		//テクスチャの読み込みのエラーをスロー
		throw(gameErrorNS::FATAL_ERROR,
			"Error in Graphics::loadTexture");
	}

	return result;
}

void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color) {

	//テクスチャのポインタ判定
	if (spriteData.texture == NULL) {
		return;
	}
	//スプライトの中心ベクトル
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2 ((float)spriteData.width/2 * spriteData.scale, 
		(float)spriteData.height/2 * spriteData.scale);
	//スプライトの位置ベクトル
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	//スプライトの拡大縮小のベクトル
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);
	
	if(spriteData.flipHorizontal){//y軸対象で反転
		scaling.x *= -1;
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);
		translate.x += (float)(spriteData.width * spriteData.scale);
	}

	if (spriteData.flipVertical) {//x軸対象で反転
		scaling.y *= -1;
		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);
		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	D3DXMATRIX matrix;//行列の定義
	D3DXMatrixTransformation2D(//変換行列
		&matrix,//行列
		NULL,//拡大縮小を行うとき原点は左上のまま
		0.0f,//拡大縮小辞の回転なし
		&scaling,//拡大縮小の倍率
		&spriteCenter,//回転の中心
		(float)(spriteData.angle),//回転の角度
		&translate);//x,y位置

	//行列をスプライトに適用
	sprite->SetTransform(&matrix);
	//スプライトの描画
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}
