#include "diary.cpp"

int main(int argc, char** argv)
{
	Diary diary;
	if(argc == 3){
		int begin = atoi(argv[1]);
		int end = atoi(argv[2]); 
		diary.pdlist(begin,end);
	}
	else{
		diary.pdlist();
	}
	
	return 0;
}
