#pragma once

class SElement {
public:
	int mPosX, mPosY;
	int mSizeX, mSizeY;

	virtual void setSize(int sizeX, int sizeY);
	virtual void draw(int drawX, int drawY);
}