#include "diary.cpp"

int main(int argc, char** argv)
{
	Diary diary;
	if(argc == 2){
		int date = atoi(argv[1]);
		return diary.pdremove(date);
	}
	else{
		return diary.pdremove(Diary::getTime());
	}
}
