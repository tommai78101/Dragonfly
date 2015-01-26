#include "..\include\ViewObject.h"
#include "..\include\Object.h"
#include "..\include\Event.h"

#define TYPE_MENU "Menu"
#define TYPE_LOGO "Logo"

class Logo : public ViewObject {
private:
	int goUp;
public:
	Logo();
	int eventHandler(Event* e);
	void draw();
};

class Menu : public ViewObject {
private:
	bool initalSpin;
	Position cursorPosition;
	bool StartGame;
public:
	Menu();
	~Menu();
	int eventHandler(Event* e);
	void draw();
};