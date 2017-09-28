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

	//Graphics�N���X�̃|�C���^���擾
	graphics = g;

	//�����̑���
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

	//�ϊ��s��̏�����
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);

	return true;
}

//�ʏ�̃e�L�X�g�`��
int Text::print(const std::string &str,int x,int y) {
	
	if (dxFont ==NULL) {
		return 0;
	}

	//�t�H���g�ʒu�̐ݒ�
	fontRect.top = y;
	fontRect.left = x;

	//��]���S
	D3DXVECTOR2 rCenter = D3DXVECTOR2((float)x, (float)y);
	//�e�L�X�g�����̊p�x�ŉ�]����s�������
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);
	//�s����X�v���C�g�ɓK�p
	graphics->getSprite()->SetTransform(&matrix);
	//�e�L�X�g���������ŕ`��
	return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &fontRect, DT_LEFT, color);
}

//��`���Ƀe�L�X�g�`��
int Text::print(const std::string &str,RECT &rect,UINT format) {
	
	if (dxFont == NULL) {
		return 0;
	}

	//�e�L�X�g�����̊p�x�ŉ�]�����Ȃ��s�������
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	//�s����X�v���C�g�ɓK�p
	graphics->getSprite()->SetTransform(&matrix);
	//�e�L�X�g���������ŕ`��
	return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &rect, DT_LEFT, color);
}

//�O���t�B�b�N�f�o�C�X�̏���
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

