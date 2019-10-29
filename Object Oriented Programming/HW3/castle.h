#ifndef CASTLE_H
#define CASTLE_H
#include "room.h"

class Castle{
	public:
		Castle();
		~Castle();
		void run();
	private:
		int scale;
		int x,y;
		int distinyx,distinyy;
		int roomNum;
		bool** castleMap;
		Room** rooms;
		Room currentRoom;
		bool gameOver;
		bool mapVisible;
		bool princessVisible;
		bool myPositionVisible;
		bool getPrincess;
		bool checkMonster();
		void gameover();
};
#endif
