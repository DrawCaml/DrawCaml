#pragma once

#include <string>
#include <vector>
#include <X11/Xlib.h>

#include "element.h"
// #include "window.h"

using namespace std;

enum class SLayout{FloatLayout, GridLayout, BorderLayout};

class SContainer : public SElement {
private:
	// Relevant for GridLayout
	int _gridX = 1, _gridY = 1;
	int _curGridPos = 0;
	int _wSpace, _hSpace;

	void _updateBgColor();

public:
	SLayout mLayout;
	vector<SElement*> mElements;

	string mBgColorStr;
	XColor mBgColor;

	SContainer(SLayout lay, int width=1, int height=1);

	virtual void draw(SWindow* win, int drawX, int drawY);
	virtual void setSize(int sizeX, int sizeY);
	void addElem(SElement* elt, int posX=-1, int posY=-1);
	void setPos(int posX, int posY);

	void setBgColor(string bgColor);
};
