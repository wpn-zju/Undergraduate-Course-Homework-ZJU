#include "Fraction.h"

using namespace std;

int GetGcd(int a, int b)
{
	if (a < 0)
		return GetGcd(-a, b);
	else if (b < 0)
		return GetGcd(a, -b);
	else if (a > b)
		return GetGcd(b, a);
	else if (a == b || a == 0)
		return b;
	else
		return GetGcd(a, b%a);
}

void Fraction::Reduction()
{
	int gcd = GetGcd(numerator, denominator);
	if (numerator < 0 && denominator < 0)
		gcd *= -1;
	numerator /= gcd;
	denominator /= gcd;
	return;
}

Fraction::Fraction()
{
	numerator = 1;
	denominator = 1;
	cout << "!!! Using Default ctor 1/1 !!!" << endl;
}

Fraction::Fraction(int a, int b)
{
	numerator = a;
	denominator = b;
	Reduction();
}

Fraction::Fraction(const Fraction& that)
{
	numerator = that.numerator;
	denominator = that.denominator;
	Reduction();
}

const Fraction Fraction::operator+ (const Fraction& that) const
{
	return Fraction(numerator*that.denominator + denominator * that.numerator, denominator*that.denominator);
}

const Fraction Fraction::operator- (const Fraction& that) const
{
	return Fraction(numerator*that.denominator - denominator * that.numerator, denominator*that.denominator);
}

const Fraction Fraction::operator* (const Fraction& that) const
{
	return Fraction(numerator*that.numerator, denominator*that.denominator);
}

const Fraction Fraction::operator/ (const Fraction& that) const
{
	return Fraction(numerator*that.denominator, denominator*that.numerator);
}

bool Fraction::operator<  (const Fraction& that) const
{
	return numerator * that.denominator > denominator*that.numerator;
}

bool Fraction::operator<= (const Fraction& that) const
{
	return !(that < *this);
}

bool Fraction::operator== (const Fraction& that) const
{
	return (numerator == that.numerator && denominator == that.denominator);
}

bool Fraction::operator!= (const Fraction& that) const
{
	return !(*this == that);
}

bool Fraction::operator>= (const Fraction& that) const
{
	return !(*this < that);
}

bool Fraction::operator>  (const Fraction& that) const
{
	return that < *this;
}

Fraction::operator double()
{
	return numerator / (double)denominator;
}

string Fraction::ToString()
{
	string output;
	stringstream temp;
	temp << numerator << "/" << denominator;
	temp >> output;
	return output;
}

istream& operator>> (istream& is, Fraction& obj)
{
	is >> obj.numerator;
	is.get();
	is >> obj.denominator;
	obj.Reduction();
	return is;
}

ostream& operator<< (ostream& os, Fraction& obj)
{
	os << obj.ToString();
	return os;
}

const Fraction Fraction::operator+= (const Fraction& that)
{
	*this = *this + that;
	return *this;
}

const Fraction Fraction::operator-= (const Fraction& that)
{
	*this = *this - that;
	return *this;
}

const Fraction Fraction::operator*= (const Fraction& that)
{
	*this = *this * that;
	return *this;
}

const Fraction Fraction::operator/= (const Fraction& that)
{
	*this = *this / that;
	return *this;
}
