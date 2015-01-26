/*
 * Empty main.  Ready to fill with code!
 */

#include "header\Common.h"
#include "header\Game.h"
#include "header\Menu.h"

bool PopulateWorld(){
	LogManager& log = LogManager::getInstance();
	log.writeLog("Populating game world.");
	new Menu;
	return true;
}

bool LoadResources(){
	ResourceManager& resource = ResourceManager::getInstance();
	LogManager& log = LogManager::getInstance();
	resource.startUp();
	if (!resource.isStarted()){
		log.writeLog("Error starting up resource manager.");
		return false;
	}

	int Result = 0;
	if ((Result = resource.loadSprite("assets/square_spinning.txt", "square_spinning")) != 0){
		log.writeLog("There's something wrong. Return value: %d", Result);
	}
	if ((Result = resource.loadSprite("assets/logo.txt", "logo")) != 0){
		log.writeLog("There's something wrong. Return value: %d", Result);
	}
	return true;
}

int main() {
	LogManager& log = LogManager::getInstance();
	if (!log.isStarted()){
		log.startUp();
		log.setFlush(true);
	}

	GameManager& game = GameManager::getInstance();
	game.startUp();
	if (!game.isStarted()){
		log.writeLog("Error starting up game manager.");
		game.shutDown();
		log.shutDown();
		return -1;
	}

	WorldManager& world = WorldManager::getInstance();
	log.writeLog("Starting up world manager.");
	world.startUp();
	if (!world.isStarted()){
		log.writeLog("Error starting up world manager.");
		world.shutDown();
		game.shutDown();
		log.shutDown();
		return -2;
	}

	if (LoadResources() && PopulateWorld()) {
		game.run();
		game.shutDown();
		return 0;
	}
	else {
		log.writeLog("Something terrible has happened. This message should not appear.");
		if (world.isStarted()){
			world.shutDown();
		}
		if (game.isStarted()){
			game.shutDown();
		}
		if (log.isStarted()){
			log.shutDown();
		}
		return -99;
	}
} 
