#pragma once

#include "utils.h"

// #include "window.h"
class SWindow;

class SElement {
public:
	int mPosX, mPosY;
	int mSizeX = 0, mSizeY = 0;

	SWindow* mWin; // to keep access to XLib stuff

	virtual void setSize(int sizeX, int sizeY){}
	virtual void draw(SWindow* win, int drawX, int drawY){
		LOG("parent draw\n");
	}
};

class SLabel : public SElement {
private:
	void _update();
public:

	const char* mText;
	
	string mFontStr;
	XFontStruct* mFontStruct;

	const char* mColorStr;
	XColor mColor;

	void setText(const char* text);
	void setFont(const char* font);
	void setColor(const char* col);

	virtual void setSize(int sizeX, int sizeY);
	virtual void draw(SWindow* win, int drawX, int drawY);
};

// class SImage : public SElement {

// };