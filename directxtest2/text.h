
#ifndef _TEXT_H
#define _TEXT_H
#define WIN32_LEAN_AND_MEAN

#include<string>
#include"constants.h"
#include"graphics.h"

class Text {
private:
	Graphics *graphics;
	COLOR_ARGB color;
	LP_DXFONT dxFont;
	RECT fontRect;

	D3DXMATRIX matrix;
	float angle;

public:
	//コンストラクタ
	Text();
	//デストラクタ
	virtual ~Text();

	//初期化
	virtual bool initialize(Graphics *g, int height, bool bold, bool italic,const std::string &fontName);
	
	//通常描画
	virtual int print(const std::string &str, int x, int y);

	//矩形描画
	virtual int print(const std::string &str, RECT &rect, UINT format);

	//文字列角度取得
	virtual float getDegrees() { return angle*(180.0f / (float)PI); }
	
	//文字列rad取得
	virtual float getRadians() { return angle; }

	//文字列色取得
	virtual COLOR_ARGB getFontColor() { return color; }

	//文字列角度設定
	virtual float setDegrees(float deg) { return angle = deg*((float)PI / 180.0f); }

	//文字列rad設定
	virtual float setRadians(float rad) { return angle = rad; }

	//文字列色設定
	virtual COLOR_ARGB setFontColor(COLOR_ARGB c) { return color = c; }

	//リソースの開放
	virtual void onLostDevice();

	//リソースの復元
	virtual void onResetDevice();

};

#endif

