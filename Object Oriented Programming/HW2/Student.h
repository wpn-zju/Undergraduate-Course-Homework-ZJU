#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class Student{
	public:
		Student(string str)
		{
			name = str;
		};
		void addCourse(string courseName,int score)
		{
			Course course;
			course.name = courseName;
			course.score = score;
			courses.push_back(course);
		}
		void print()
		{
			cout << "Name: " << name << endl;
			cout << "Courses: ";
			for(int i = 0; i < courses.size(); i++)
				cout << left << setw(6) <<courses[i].name << left << setw(6) << courses[i].score << '\t';
			cout << endl;
		}
	private:
		string name;
		struct Course{
			string name;
			int score;
		};
		vector<Course> courses;
}; 
