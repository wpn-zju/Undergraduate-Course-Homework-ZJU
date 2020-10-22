#include "diary.h"

int main(int argc, char** argv)
{
	Diary diary;
	if (argc == 2) {
		int date = atoi(argv[1]);
		diary.pdshow(date);
	}
	else {
		diary.pdshow(Diary::getTime());
	}

	return 0;
}
