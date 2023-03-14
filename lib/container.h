#pragma once

#include <string>
#include <vector>
#include <X11/Xlib.h>

#include "element.h"
// #include "window.h"

using namespace std;

/**
 * Layout types: for now on, BorderLayout is unimplemented.
 * */
enum class SLayout{FloatLayout, GridLayout, BorderLayout};

/**
 * Class used to manage other elements. Code for Drawcaml.dcontainer
 * */
class SContainer : public SElement {
private:
	// Relevant for GridLayout
	int _gridX = 1, _gridY = 1;
	int _curGridPos = 0;
	int _wSpace, _hSpace;

	void _updateBgColor();

public:
	/**
	 * Layout of the container
	 * */
	SLayout mLayout;
	/**
	 * Elements managed by the container
	 * */
	vector<SElement*> mElements;

	string mBgColorStr = "white";
	XColor mBgColor;

	SContainer(SLayout lay, int width=1, int height=1);
	
	/**
	 * Re-generate the color according to the GC of the window.
	 * */
	void updateWin(SWindow* win);
	/**
	 * Draw recursively the elements of the container, by calling the draw method of any SElement in mElements.
	 * */
	virtual void draw(SWindow* win, int drawX, int drawY);
	/**
	 * Set the size of the container
	 * */
	virtual void setSize(int sizeX, int sizeY);
	/**
	 * Remove an element from the container. (Be careful to call draw after to see the changes)
	 * */
	void removeElem(SElement* elt);
	/**
	 * Add an element from the container. (Be careful to call draw after to see the changes)
	 * */
	void addElem(SElement* elt, int posX=-1, int posY=-1);

	/**
	 * Set the background color of the container.
	 * */
	void setBgColor(string bgColor);
};
