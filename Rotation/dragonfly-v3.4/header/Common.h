#ifndef COMMON_H
#define COMMON_H

#include "..\include\ResourceManager.h"
#include "..\include\WorldManager.h"
#include "..\include\LogManager.h"
#include "..\include\GameManager.h"
#include "..\include\Event.h"
#include "..\include\EventStep.h"
#include "..\include\EventKeyboard.h"
#include "..\include\Box.h"
#include "..\include\GraphicsManager.h"
#include "..\include\utility.h"
#include "..\include\ViewObject.h"
#include "..\include\ObjectList.h"
#include "..\include\ObjectListIterator.h"
#include "..\include\Pause.h"

#define ArrayLength(Array) sizeof((Array))/sizeof((Array)[0])

inline const char* BoolToString(bool value){
	return value ? "true" : "false";
}

#endif
