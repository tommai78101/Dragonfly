#include "../include/LogManager.h"
#include "../header/Common.h"

void test(){
	LogManager& Output = LogManager::getInstance();
	if (Output.isStarted()){
		
	}
	else {
		int Result = Output.startUp();
		if (Result >= 0){
			Output.writeLog("Hello world.");
		}
	}
	return;
}