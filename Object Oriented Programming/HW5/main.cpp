#include "Fraction.h"

using namespace std;

int Test()
{
	Fraction a(40, 40), b(5, 5), c(-10, -30), d(-10, -40), e(-10, -50), f(1, 1);
	f = (a + b - c) * d / e;
	Fraction two(2, 1);
	cout << "f = " << f << " f + 2 = ";
	f += two;
	cout << f << endl << "f = " << f << " f - 2 = ";
	f -= two;
	cout << f << endl << "f = " << f << " f * 2 = ";
	f *= two;
	cout << f << endl << "f = " << f << " f / 2 = ";
	f /= two;
	cout << f << endl;
	double value = (double)f;
	cout << "f = " << f << " = " << value << endl;

	cout << "a = 40/40, b = 5/5." << endl;
	bool val[6];
	val[0] = (a == b);
	val[1] = (a != b);
	val[2] = (a < b);
	val[3] = (a > b);
	val[4] = (a <= b);
	val[5] = (a >= b);
	cout << val[0] << " a==b" << endl << val[1] << " a!=b" << endl << val[2] << " a<b" << endl << val[3] << " a>b" << endl << val[4] << " a<=b" << endl << val[5] << " a>=b" << endl;
	cout << "Input a Fraction like a/b" << endl;
	cin >> f;
	cout << f << " = " << (double)f << endl;
}

int main()
{
	Test();
	return 0;
}
