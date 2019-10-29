#include <iostream>  
#include <iomanip>
#include <fstream> 
using namespace std;
class Students{
	public:
			int no;
			string name;
			int score1;
			int score2;
			int score3;
			float average;
};
int main(){
	ifstream data("data.txt");
	Students* stu = new Students[10];
	int i;
	int min1=6,min2=6,min3=6;
	int max1=0,max2=0,max3=0;
	int sum1=0,sum2=0,sum3=0;
	for(i=0;i<10;i++){
		//Input
		data>>stu[i].no>>stu[i].name>>stu[i].score1>>stu[i].score2>>stu[i].score3;
		stu[i].average=((float)stu[i].score1+(float)stu[i].score2+(float)stu[i].score3)/3;
		sum1+=stu[i].score1;
		sum2+=stu[i].score2;
		sum3+=stu[i].score3;
		if(stu[i].score1<min1)
		min1=stu[i].score1;
		if(stu[i].score1>max1)
		max1=stu[i].score1;
		if(stu[i].score2<min2)
		min2=stu[i].score2;
		if(stu[i].score2>max2)
		max2=stu[i].score2;
		if(stu[i].score3<min3)
		min3=stu[i].score3;
		if(stu[i].score3>max3)
		max3=stu[i].score3;
	}
	
	//Print
	cout<<"no      name    score1  score2  score3  average"<<endl;
	for(i=0;i<10;i++){
		cout<<left<<setw(8)<<stu[i].no;
		cout<<left<<setw(8)<<stu[i].name;
		cout<<left<<setw(8)<<stu[i].score1;
		cout<<left<<setw(8)<<stu[i].score2;
		cout<<left<<setw(8)<<stu[i].score3;
		cout<<left<<setw(8)<<stu[i].average;
		cout<<endl;
	}
	cout<<left<<setw(8)<<" ";
	cout<<left<<setw(8)<<"average ";
	cout<<left<<setw(8)<<(float)sum1/10;
	cout<<left<<setw(8)<<(float)sum2/10;
	cout<<left<<setw(8)<<(float)sum3/10;
	cout<<endl;
	cout<<left<<setw(8)<<" ";
	cout<<left<<setw(8)<<"min ";
	cout<<left<<setw(8)<<min1;
	cout<<left<<setw(8)<<min2;
	cout<<left<<setw(8)<<min3;
	cout<<endl;
	cout<<left<<setw(8)<<" ";
	cout<<left<<setw(8)<<"max ";
	cout<<left<<setw(8)<<max1;
	cout<<left<<setw(8)<<max2;
	cout<<left<<setw(8)<<max3;
	cout<<endl;
	delete[] stu;
	getchar(); 
	return 0;
}
