#pragma once

class SElement {
public:
	int mPosX, mPosY;
	int mSizeX, mSizeY;

	SWindow mWin; // to keep access to XLib stuff

	virtual void setSize(int sizeX, int sizeY);
	virtual void draw(SWindow* win, int drawX, int drawY);
}
