#include "diary.h"

int main(int argc, char** argv)
{
	Diary diary;
	std::string text;
	std::string temp;
	std::getline(std::cin, temp);
	while (temp != "." && !std::cin.eof()) {
		text += temp;
		text += '\n';
		std::getline(std::cin, temp);
	}
	text = text.substr(0, text.length() - 1);
	diary.pdadd(Diary::getTime(), text);

	return 0;
}
