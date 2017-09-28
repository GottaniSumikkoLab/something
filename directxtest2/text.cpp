//text.cpp

#include"text.h"

Text::Text() {

	color = graphicsNS::WHITE;

	fontRect.top = 0;
	fontRect.left = 0;
	fontRect.right = GAME_WIDTH;
	fontRect.bottom = GAME_HEIGHT;
	dxFont = NULL;
	angle = 0;
}

Text::~Text() {
	SAFE_RELEASE(dxFont);
}

bool Text::initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName) {

	//Graphicsクラスのポインタを取得
	graphics = g;

	//文字の太さ
	UINT weight = FW_NORMAL;
	if (bold == true) {
		weight = FW_BOLD;
	}

	// create DirectX font
	if (FAILED(D3DXCreateFont(graphics->get3Ddevice(), height, 0, weight, 1, italic,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
		FF_DONTCARE, fontName.c_str(), &dxFont))) {
		return false;
	}

	//変換行列の初期化
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);

	return true;
}

//通常のテキスト描画
int Text::print(const std::string &str,int x,int y) {
	
	if (dxFont ==NULL) {
		return 0;
	}

	//フォント位置の設定
	fontRect.top = y;
	fontRect.left = x;

	//回転中心
	D3DXVECTOR2 rCenter = D3DXVECTOR2((float)x, (float)y);
	//テキストを特定の角度で回転する行列を準備
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);
	//行列をスプライトに適用
	graphics->getSprite()->SetTransform(&matrix);
	//テキストを左揃えで描画
	return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &fontRect, DT_LEFT, color);
}

//矩形内にテキスト描画
int Text::print(const std::string &str,RECT &rect,UINT format) {
	
	if (dxFont == NULL) {
		return 0;
	}

	//テキストを特定の角度で回転させない行列を準備
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	//行列をスプライトに適用
	graphics->getSprite()->SetTransform(&matrix);
	//テキストを左揃えで描画
	return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &rect, DT_LEFT, color);
}

//グラフィックデバイスの消失
void Text::onLostDevice() {
	if (dxFont == NULL) {
		return;
	}
	dxFont->OnLostDevice();
}

void Text::onResetDevice() {

	if (dxFont == NULL) {
		return;
	}

	dxFont->OnResetDevice();
}

