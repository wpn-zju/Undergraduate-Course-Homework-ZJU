#ifndef ROOM_H
#define ROOM_H
#include <string>

class Room {
public:
	std::string name;
	bool exits[4];
};
#endif
