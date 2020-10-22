#include "diary.h"

using namespace std;

// constructor, read diary from diary.txt
Diary::Diary()
{
	ifstream file("diary.txt");
	int date = 0;
	string temp;
	string text;
	while (file >> date) {
		getline(file, temp);
		getline(file, temp);
		while (temp != "." && !file.eof()) {
			text += temp;
			text += '\n';
			getline(file, temp);
		}
		text = text.substr(0, text.length() - 1);
		entities[date] = text;
		text = "";
		date = 0;
	}
}

//destructor, write diary into diary.txt
Diary::~Diary()
{
	ofstream file("diary.txt");
	map<int, string>::iterator i;
	for (i = entities.begin(); i != entities.end(); i++)
		file << i->first << endl << i->second << endl << "." << endl;
}

void Diary::pdadd(int date, string data)
{
	entities[date] = data;
}

void Diary::pdlist(int begin, int end)
{
	bool found = false;
	map<int, string>::iterator i;
	for (i = entities.begin(); i != entities.end(); i++) {
		if (i->first >= begin && i->first <= end) {
			cout << i->first << endl;
			cout << i->second << endl;
			cout << endl;
			found = true;
		}
	}
	if (!found)
		cout << "Not Found!" << endl;
}

void Diary::pdshow(int date)
{
	map<int, string>::iterator i = entities.find(date);
	if (i != entities.end())
		cout << i->second << endl;
	else
		cout << "Not Found!" << endl;
}

int Diary::pdremove(int date)
{
	map<int, string>::iterator i = entities.find(date);
	if (i != entities.end()) {
		entities.erase(date);
		return 0;
	}
	else
		return -1;
}

int Diary::getTime()
{
	char temp[10];
	time_t timer = time(NULL);
	strftime(temp, 10, "%Y%m%d", localtime(&timer));
	return atoi(temp);
}
