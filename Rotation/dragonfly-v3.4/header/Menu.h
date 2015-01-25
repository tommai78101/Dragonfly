#include "..\include\Object.h"
#include "..\include\Event.h"

#define DF_TYPE_MENU "Menu"

class Menu : public Object {
private:
	void step();
	void start();
public:
	Menu();
	~Menu();
	int eventHandler(Event* e);
};