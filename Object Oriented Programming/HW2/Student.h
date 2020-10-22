#include <string>
#include <vector>
#include <iomanip>

class Student {
public:
	Student(std::string str)
	{
		name = str;
	};
	void addCourse(std::string courseName, int score)
	{
		Course course;
		course.name = courseName;
		course.score = score;
		courses.push_back(course);
	}
	void print()
	{
		std::cout << "Name: " << name << std::endl;
		std::cout << "Courses: ";
		for (int i = 0; i < courses.size(); i++)
			std::cout << std::left << std::setw(6) << courses[i].name << std::left << std::setw(6) << courses[i].score << '\t';
		std::cout << std::endl;
	}
private:
	std::string name;
	struct Course {
		std::string name;
		int score;
	};
	std::vector<Course> courses;
};
