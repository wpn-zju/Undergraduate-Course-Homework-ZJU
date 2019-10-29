#ifndef _DIARY_H
#define _DIARY_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <stdlib.h>
using namespace std;

class Diary{
	public:
		Diary();
		~Diary();
		static int getTime();
		void pdadd(int date, string data);
		void pdlist(int begin = 0, int end = 99999999);
		void pdshow(int date);
		int pdremove(int date);
	private:
		map<int,string> entities;
}; 
#endif
