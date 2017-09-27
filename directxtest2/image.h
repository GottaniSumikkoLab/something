//image.h

#ifndef _IMAGE_H
#define _IMAGE_H
#define WIN32_LEAN_AND_MEAN

#include"textureManager.h"
#include"constants.h"

class Image {
	
	protected:
		Graphics *graphics;
		TextureManager *textureManager;
		SpriteData spriteData;
		COLOR_ARGB colorFilter;
		int cols;//�A�j���[�V��������摜�̌��i�t���[�����j
		int startFrame;
		int endFrame;
		int currentFrame;
		float frameDelay;
		float animTimer;//�A�j���[�V�����o�ߎ��Ԕ���
		float fadeTimer;//�t�F�[�h�o�ߎ��Ԕ���
		int fadealpha;//�t�F�[�h���̃A���t�@�l�ϐ�
		HRESULT hr;
		bool loop;//�A�j���[�V�������[�v����
		bool visible;//�摜�̉�����
		bool initialized;//�������̔���
		bool animComplete;//�A�j���[�V�����I������
		bool fadejudge;//�t�F�[�h����

	public:
		Image();
		virtual ~Image();
		//�p�����[�^�擾���\�b�h
		const virtual SpriteData& getSpriteInfo() { return spriteData; }//SpriteData�̃A�h���X
		virtual bool getVisible() { return visible; }//�摜�̕`������̔���
		virtual float getX() { return spriteData.x; }//�摜�ʒu.x
		virtual float getY() { return spriteData.y; }//.y
		virtual float getScale() { return spriteData.scale; }//�摜�̊g��k���i�g��k�����͕��������l�j
		virtual int getWidth() { return spriteData.width; }//�摜�̕�
		virtual int getHight() { return spriteData.height; }//�摜�̍���
		virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }//�摜�̒��S.x
		virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }//.y
		virtual float getFrameDegrees() { return spriteData.angle*(180.0f / (float)PI); }//�摜�̊p�x�x��
		virtual float getRadians() { return spriteData.angle; }//�摜�̊p�xrad
		virtual float getFrameDelay() { return frameDelay; }//�A�j���[�V�����Ԋu
		virtual int getStartFrame() { return startFrame; }//�A�j���[�V�����̍ŏ��̃t���[��
		virtual int getEndFrame() { return endFrame; }//�A�j���[�V�����̏I���̃t���[��
		virtual int getCurrentFrame() { return currentFrame; }//�A�j���[�V�����̌��݂̃t���[��
		virtual RECT getSpritaDataRect() { return spriteData.rect; }//SpriteData�̎l�p
		virtual bool getAnimationComplete() { return animComplete; }//�A�j���[�V�����̏I���̔���
		virtual COLOR_ARGB getColorFilter() { return colorFilter; }//�摜���t�B���^����F
		virtual bool getfadejudge() { return fadejudge; }//�t�F�[�h����\�̔���

		//�p�����[�^�ݒ胁�\�b�h
		virtual void setX(float newX) { spriteData.x = newX; }
		virtual void setY(float newY) { spriteData.y = newY; }
		virtual void setScale(float s) { spriteData.scale = s; }
		virtual void setDegrees(float deg) { spriteData.angle = deg*((float)PI / 180.0f); }
		virtual void setRadians(float rad) { spriteData.angle = rad; }
		virtual void setVisible(bool v) { visible = v; }
		virtual void setFrameDelay(float d) { frameDelay = d; }
		virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }
		virtual void setCurrentFrame(int c);
		virtual void setRect();
		virtual void setSpliteDataRect(RECT r) { spriteData.rect = r; }
		virtual void setLoop(bool lp) { loop = lp; }
		virtual void setAnimationComplete(bool a) { animComplete = a; }
		virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }
		virtual void setTextureManager(TextureManager *textureM) { textureManager = textureM; }
		//���̑��̋@�\
		virtual bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);
		virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }
		virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }
		virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);
		virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE);
		virtual void update(float frameTime);
		//�e�X�g��
		virtual void fadeOut(float frameTime, float fadeoutTime);
		virtual void fadeIn(float frameTime, float fadeinTime);
};
#endif
