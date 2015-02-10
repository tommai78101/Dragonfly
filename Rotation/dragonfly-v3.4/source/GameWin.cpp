#include "..\header\GameWin.h"
#include "..\header\Game.h"

GameWin::GameWin(game_state* GameState, Game* game){
	LogManager& l = LogManager::getInstance();
	ResourceManager& r = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	Sprite* sprite = r.getSprite("game-win");
	if (sprite){
		setSprite(sprite);
		setTransparency();
		setSpriteSlowdown(5);

		int width = g.getHorizontal();
		int height = g.getVertical();

		Position pos(width / 2, height / 2);
		setPosition(pos);

		registerInterest(DF_KEYBOARD_EVENT);

		setVisible(true);
		this->GameState = GameState;
		this->game = game;
		l.writeLog("[GameWin] Successfully initialized winning text.");
		return;
	}
	l.writeLog("[GameWin] Something is wrong. Cannot initialize correctly.");
}

GameWin::~GameWin(){
	unregisterInterest(DF_KEYBOARD_EVENT);
}

int GameWin::eventHandler(Event* e){
	if (e->getType().compare(DF_KEYBOARD_EVENT) == 0){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		this->GameState->Stage1.win.isGameWinCreated = false;
		this->GameState->Stage1.win.win = false;
		this->game->reset();
		this->game->getMenu()->reset();
			
		WorldManager& w = WorldManager::getInstance();
		w.markForDelete(this);
	}
	return 1;
}

void GameWin::draw(){
	Object::draw();
}