
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
	//�R���X�g���N�^
	Text();
	//�f�X�g���N�^
	virtual ~Text();

	//������
	virtual bool initialize(Graphics *g, int height, bool bold, bool italic,const std::string &fontName);
	
	//�ʏ�`��
	virtual int print(const std::string &str, int x, int y);

	//��`�`��
	virtual int print(const std::string &str, RECT &rect, UINT format);

	//������p�x�擾
	virtual float getDegrees() { return angle*(180.0f / (float)PI); }
	
	//������rad�擾
	virtual float getRadians() { return angle; }

	//������F�擾
	virtual COLOR_ARGB getFontColor() { return color; }

	//������p�x�ݒ�
	virtual float setDegrees(float deg) { return angle = deg*((float)PI / 180.0f); }

	//������rad�ݒ�
	virtual float setRadians(float rad) { return angle = rad; }

	//������F�ݒ�
	virtual COLOR_ARGB setFontColor(COLOR_ARGB c) { return color = c; }

	//���\�[�X�̊J��
	virtual void onLostDevice();

	//���\�[�X�̕���
	virtual void onResetDevice();

};

#endif

