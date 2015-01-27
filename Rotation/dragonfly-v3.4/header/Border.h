#ifndef BORDER_H

#define TYPE_BORDER "Border"

#include "Common.h"

class Border : public Object {
private:
	int width;
	int height;
public:
	Border();
	int eventHandler(Event* e);
	void draw();
};

#define BORDER_H
#endif