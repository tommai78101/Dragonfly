#include "..\include\ViewObject.h"
#include "..\include\Object.h"
#include "..\include\Event.h"

#define DF_TYPE_MENU "Menu"

class Menu : public ViewObject {
private:
	bool initalSpin;
public:
	Menu();
	int eventHandler(Event* e);
	void draw();
};