/*
 * Empty main.  Ready to fill with code!
 */

#include "include\ResourceManager.h"
#include "include\WorldManager.h"
#include "include\LogManager.h"
#include "include\GameManager.h"

#include "header\Common.h"

static bool PopulateWorld(){
	LogManager& log = LogManager::getInstance();
	log.writeLog("Populating game world.");

	/*
		Start the game.

		syntax:

		new GameStart();
	*/

	return true;
}

static bool LoadResources(){
	ResourceManager& resource = ResourceManager::getInstance();
	resource.startUp();
	if (!resource.isStarted()){
		LogManager& log = LogManager::getInstance();
		log.writeLog("Error starting up resource manager.");
		return false;
	}

	/*
		Load sprites here:

		syntax: 

		if (result = rm.loadSprite("assets/gameStart.txt", "GameStart") != 0){
			l.writeLog("There's something wrong. Return value: %d", result);
		}
	*/

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
		return -1;
	}

	WorldManager& world = WorldManager::getInstance();
	world.startUp();
	if (!world.isStarted()){
		log.writeLog("Error starting up world manager.");
		world.shutDown();
		game.shutDown();
		return -2;
	}

	if (LoadResources() && PopulateWorld()) {
	{
		game.run();
		game.shutDown();
		return 0;
	}

	log.writeLog("Something terrible has happened. This message should not appear.");
	if (world.isStarted()){
		world.shutDown();
	}
	if (game.isStarted()){
		game.shutDown();
	}
	return -99;
}
