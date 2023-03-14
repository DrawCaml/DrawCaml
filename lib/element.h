#pragma once

#include "utils.h"

class SWindow;

/**
 * Abstract class for the graphical elements of the frame. Code for Drawcaml.delement
 * */
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

/**
 * Graphical element that prints text. Code for Drawcaml.dLabel
 * */
class SLabel : public SElement {
private:
	void _update();
public:

	/**
	 * Text of the Label.
	 * */
	const char* mText;
	
	/**
	 * Font for the label, using X logical font format.
	 * */
	string mFontStr;
	XFontStruct* mFontStruct;

	/**
	 * Color of the text.
	 * */
	const char* mColorStr;
	XColor mColor;

	/**
	 * Set the text of the Label.
	 * */
	void setText(const char* text);
	/**
	 * Set the font of the label.
	 * */
	void setFont(const char* font);
	/**
	 * Set the color of the text.
	 * */
	void setColor(const char* col);

	virtual void setSize(int sizeX, int sizeY);
	virtual void draw(SWindow* win, int drawX, int drawY);
};