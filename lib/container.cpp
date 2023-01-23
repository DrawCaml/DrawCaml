#include <X11/Xlib.h>
#include <string>
#include "utils.h"
#include "element.h"
#include "window.h"
#include "container.h"

SContainer::SContainer(SLayout lay, int width, int height){
	mLayout = lay;
	if(lay == SLayout::GridLayout){
		if (width > 1) _gridX = width;
		else ERROR("Invalid width for GridLayout: " + to_string(width)); 
		if(height > 1) _gridY = height;
		else ERROR("Invalid height for GridLayout: " + to_string(height));
	}
}

void SContainer::setPos(int posX, int posY){
	mPosX = posX;
	mPosY = posY;
}

void SContainer::setSize(int sizeX, int sizeY){
	mSizeX = sizeX;
	mSizeY = sizeY;
	if(mLayout == SLayout::GridLayout){
		_wSpace = sizeX/_gridX;
		_hSpace = sizeY/_gridY;
	}
}

void SContainer::addElem(SElement* elt, int posX, int posY){
	mElements.push_back(elt);

	switch(mLayout){
		case SLayout::FloatLayout:
			if(posX == -1 || posY == -1)
				ERROR("Cannot add element to FloatLayout without specifying coords.");
			elt->mPosX = posX;
			elt->mPosY = posY;
			elt->setSize(mSizeX-posX, mSizeY-posY);
			break;
		
		case SLayout::GridLayout:
			if(posX == -1 && posY == -1){
				if(_curGridPos < _gridX*_gridY){
					int curX = _curGridPos % _gridX;
					int curY = _curGridPos / _gridX;
					elt->mPosX = curX*_wSpace;
					elt->mPosY = curY*_hSpace;
					elt->setSize(_wSpace, _hSpace);
					_curGridPos++;
				}
			} else if (posX == -1 || posY == -1){
				ERROR("Cannot add element to GridLayout without specifying both coords.");
			} else {
				elt->mPosX = posX*_wSpace;
				elt->mPosY = posY*_hSpace;
				elt->setSize(_wSpace, _hSpace);
			}
			break;

		default:
			elt->mPosX = posX;
			elt->mPosY = posY;
			elt->setSize(mSizeX, mSizeY);
			break;
	}
}

void SContainer::_updateBgColor(){
	if(mBgColorStr == "") return;
	Status rc = XAllocNamedColor(mWin->mDisplay, mWin->mColormap, mBgColorStr.c_str(), &mBgColor, &mBgColor);
	if (rc == 0) {
		fprintf(stderr, "XAllocNamedColor - failed to allocated %s color.\n", mBgColorStr.c_str());
		exit(1);
	}
}

void SContainer::setBgColor(string bgColor){
	mBgColorStr = bgColor;
}

void SContainer::updateWin(SWindow* win){
	if(mWin != win){
		mWin = win;
		_updateBgColor();
	}
}

// add a parameter to access XLib stuff for drawing
void SContainer::draw(SWindow* win, int drawX, int drawY){
	updateWin(win);
	_updateBgColor();

	// draw the background
	XSetForeground(mWin->mDisplay, mWin->mGC, mBgColor.pixel);
	XFillRectangle(mWin->mDisplay, mWin->mWindow, mWin->mGC, drawX+mPosX, drawY+mPosY, mSizeX, mSizeY);
	// printf("Container drawn in: %d %d of size %d %d\n", mPosX, mPosY, mSizeX, mSizeY);
	for (SElement* elt : mElements){
		elt->draw(mWin, drawX+mPosX, drawY+mPosY);
	}
}