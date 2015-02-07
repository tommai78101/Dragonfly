#ifndef EXIT_H
#define EXIT_H

#include "Common.h"

class Exit : public Object {
private:
public:
	Exit();
	~Exit();
	int eventHandler(Event* e);
	void draw();
};

#endif