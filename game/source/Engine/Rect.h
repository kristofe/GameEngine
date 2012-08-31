#ifndef _RECT_H
#define _RECT_H

class Rect {
public:
	Rect():x(0),y(0),width(0),height(0){};
	Rect(float px, float py, float pwidth, float pheight);
	
	float x;
	float y;
	float width;
	float height;
};

#endif