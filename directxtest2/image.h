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
		int cols;//アニメーションする画像の個数（フレーム数）
		int startFrame;
		int endFrame;
		int currentFrame;
		float frameDelay;
		float animTimer;//アニメーション経過時間判定
		float fadeTimer;//フェード経過時間判定
		int fadealpha;//フェード時のアルファ値変数
		HRESULT hr;
		bool loop;//アニメーションループ判定
		bool visible;//画像の可視判定
		bool initialized;//初期化の判定
		bool animComplete;//アニメーション終了判定
		bool fadejudge;//フェード判定

	public:
		Image();
		virtual ~Image();
		//パラメータ取得メソッド
		const virtual SpriteData& getSpriteInfo() { return spriteData; }//SpriteDataのアドレス
		virtual bool getVisible() { return visible; }//画像の描画を許可の判定
		virtual float getX() { return spriteData.x; }//画像位置.x
		virtual float getY() { return spriteData.y; }//.y
		virtual float getScale() { return spriteData.scale; }//画像の拡大縮小（拡大縮小率は幅高さ同様）
		virtual int getWidth() { return spriteData.width; }//画像の幅
		virtual int getHight() { return spriteData.height; }//画像の高さ
		virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }//画像の中心.x
		virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }//.y
		virtual float getFrameDegrees() { return spriteData.angle*(180.0f / (float)PI); }//画像の角度度数
		virtual float getRadians() { return spriteData.angle; }//画像の角度rad
		virtual float getFrameDelay() { return frameDelay; }//アニメーション間隔
		virtual int getStartFrame() { return startFrame; }//アニメーションの最初のフレーム
		virtual int getEndFrame() { return endFrame; }//アニメーションの終わりのフレーム
		virtual int getCurrentFrame() { return currentFrame; }//アニメーションの現在のフレーム
		virtual RECT getSpritaDataRect() { return spriteData.rect; }//SpriteDataの四角
		virtual bool getAnimationComplete() { return animComplete; }//アニメーションの終了の判定
		virtual COLOR_ARGB getColorFilter() { return colorFilter; }//画像をフィルタする色
		virtual bool getfadejudge() { return fadejudge; }//フェード判定可能の判定

		//パラメータ設定メソッド
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
		//その他の機能
		virtual bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);
		virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }
		virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }
		virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);
		virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE);
		virtual void update(float frameTime);
		//テスト中
		virtual void fadeOut(float frameTime, float fadeoutTime);
		virtual void fadeIn(float frameTime, float fadeinTime);
};
#endif
