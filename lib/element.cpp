#include <X11/Xlib.h>
#include <string>
#include <cstring>
#include "utils.h"
#include "element.h"
#include "window.h"

/*
SLABEL METHODS
*/

/**
 * the names speak for themselves
*/

void SLabel::_update(){
	if(!mColorStr) mColorStr = "black";

	XAllocNamedColor(mWin->mDisplay, mWin->mColormap, mColorStr, &mColor, &mColor);

	mFontStruct = XLoadQueryFont(mWin->mDisplay, mFontStr.c_str());
	if(!mFontStruct){
		ERROR("SLABEL: Cannot load font " + mFontStr + " !\n");
	}
}

// For now on, only xlib fonts are supported 
void SLabel::setFont(const char* font){
	string s(font);
	mFontStr = s;
}

void SLabel::setText(const char* text){
	mText = text;
}

void SLabel::setColor(const char* col){
	mColorStr = col;
}

void SLabel::setSize(int sizeX, int sizeY){
	// to resize a label, use the appropriate X font
}

void SLabel::draw(SWindow* win, int drawX, int drawY){
	if(mWin != win)
		mWin = win;
	_update();

	if(mFontStruct){
		XSetForeground(mWin->mDisplay, mWin->mGC, mColor.pixel);
		XSetFont(mWin->mDisplay, mWin->mGC, mFontStruct->fid);
		XDrawString(mWin->mDisplay, mWin->mBackBuffer, mWin->mGC, drawX+mPosX, drawY+mPosY, mText, strlen(mText));
	}
	else WARNING("Label not printed\n");
}