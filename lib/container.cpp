#include <X11/Xlib.h>
#include "utils.h"

using namespace std;

SContainer::SContainer(SLayout lay, int width=1, int height=1){
	mLayout = lay;
	if(lay == GridLayout){
		if (width > 1) _gridX = width;
		else ERROR("Invalid width for GridLayout: " + (string)width); 
		if(height > 1) _gridY = height;
		else ERROR("Invalid height for GridLayout: " + (string)height);
	}
}

SContainer::setSize(int sizeX, int sizeY){
	mSizeX = sizeX;
	mSizeY = sizeY;
	if(mLayout == GridLayout){
		_wSpace = sizeX/_gridX;
		_hSpace = sizeY/_gridY;
	}
}

SContainer::addElem(SElement elt, int posX=-1, int posY=-1){
	mElements.push_back(elt);

	switch(mLayout){
		case FloatLayout:
			if(posX == -1 || posY == -1)
				ERROR("Cannot add element to FloatLayout without specifying coords.");
			elt.mPosX = posX;
			elt.mPosY = posY;
			elt.setSize(mSizeX, mSizeY);
			break;
		
		case GridLayout:
			if(posX == -1 && posY == -1){
				if(_curGridPos < _gridX*_gridY){
					int curX = _curGridPos % _gridX;
					int curY = (_curGridPos / _gridX);
					elt.mPosX = curX*_wSpace;
					elt.mPosY = curY*_hSpace;
					elt.setSize(_wSpace, _hSpace);
					_curGridPos++;
				}
			} else if (posX == -1 || posY == -1){
				ERROR("Cannot add element to GridLayout without specifying both coords.");
			} else {
				elt.mPosX = posX*_wSpace;
				elt.mPosY = posY*_hSpace;
				elt.setSize(_wSpace, _hSpace);
			}

		default:
			elt.mPosX = posX;
			elt.mPosY = posY;
			elt.setSize(mSizeX, mSizeY);
			break;
	}
}

void setBgColor(string bgColor){
	Status rc = XAllocNamedColor(display, screen_colormap, bgColor, mBgColor, mBgColor);
	if (rc == 0) {
		fprintf(stderr, "XAllocNamedColor - failed to allocated %s color.\n", bgColor);
		exit(1);
	}
}

// add a parameter to access XLib stuff for drawing
SContainer::draw(int drawX, int drawY){
	for(SElement e : mElements){
		// draw the background
		if(mBgColor != NULL){
			XSetForeground(mWin->mDisplay, mWin->mGC, mBgColor->pixel);
			XFillRectangle(mWin->mDisplay, mWin->mWindow, mWin->mGC, mPosX, mPosY, mSizeX, mSizeY);
		}
		e.draw(drawX+mPosX, drawY+mPosY);
	}
}