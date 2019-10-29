#include "diary.cpp"

int main(int argc, char** argv)
{
	Diary diary;
	string text;
	string temp;
	getline(cin, temp);
	while(temp!="." && !cin.eof()){
		text += temp;
		text += '\n';
		getline(cin, temp);
	}
	text = text.substr(0,text.length()-1);
	diary.pdadd(Diary::getTime(),text);
	
	return 0; 
}
