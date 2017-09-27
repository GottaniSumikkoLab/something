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

	graphics = g;

	UINT weight = FW_NORMAL;
	if (bold == true) {
		weight = FW_BOLD;
	}

	// create DirectX font
	if (FAILED(D3DXCreateFont(graphics->get3Ddevice(), height, 0, weight, 1, talic,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
		FF_DONTCARE, fontName.c_str(), &dxFont))) {

	}
}