#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "student.h"
using namespace std;

int main(){
	vector<Student> stuVec;
	string stuName;
	string name;
	int score;
	
	ifstream data("data.txt");
	while(1){
		if(data >> stuName){
			Student Stu(stuName);
			data >> name;
			while(name != "end"){
				data >> score;
				Stu.addCourse(name, score);
				data >> name;
			}
			stuVec.push_back(Stu);
		}
		else
			break;
	}
	for(int i = 0; i < stuVec.size(); i++)
		stuVec[i].print();
	stuVec.clear();
	getchar(); 
	return 0;
}

