#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

	enum gamestate{
		GS_START,
		GS_INIT,
		GS_BOUNCE,
		GS_WIN,
		GS_LOSE,

	};

struct rect {
	int row;
	int col; 
	int horizontalVelocity;
	int verticalVelocity;
	unsigned short color; 
};

struct state {
	enum gamestate gamestate;
	int size;
	struct rect rectangleOne;
	struct rect obstacle;
};





#endif
