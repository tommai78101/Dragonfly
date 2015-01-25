#include "..\include\ViewObject.h"
#include "..\include\Object.h"
#include "..\include\Event.h"

#define DF_TYPE_MENU "Menu"

class Menu : public ViewObject {
public:
	Menu();
	int eventHandler(Event* e);
	void draw();
};