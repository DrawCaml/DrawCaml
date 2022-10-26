#pragma once

#include <vector>
#include "element.h"

using namespace std;

enum class SLayout{FloatLayout, GridLayout, BorderLayout};

class SContainer : public SElement {
private:
	// Relevant for GridLayout
	int _gridX = 1, _gridY = 1;
	int _curGridPos = 0;
	int _wSpace, _hSpace;

public:
	SLayout mLayout;
	vector<SElement> mElements;

	SContainer();

	void draw(int drawX, int drawY);
	void addElem(SElement elt);
}
