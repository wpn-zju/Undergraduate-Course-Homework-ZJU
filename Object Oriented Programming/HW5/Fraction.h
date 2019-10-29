#ifndef _FRACTION_H
#define _FRACTION_H
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Fraction
{
	private:
		int numerator;
		int denominator;
		void Reduction();
	public:
		Fraction();
		Fraction(int a, int b);
		Fraction(const Fraction& that); 
		const Fraction operator+ (const Fraction& that) const;
		const Fraction operator- (const Fraction& that) const;
		const Fraction operator* (const Fraction& that) const;
		const Fraction operator/ (const Fraction& that) const;
		bool operator<  (const Fraction& that) const;
		bool operator<= (const Fraction& that) const;
		bool operator== (const Fraction& that) const;
		bool operator!= (const Fraction& that) const;
		bool operator>= (const Fraction& that) const;
		bool operator>  (const Fraction& that) const;
		operator double();
		string ToString();
		friend istream& operator>> (istream& is, Fraction& obj);
		friend ostream& operator<< (ostream& is, Fraction& obj);
		const Fraction operator+= (const Fraction& that);
		const Fraction operator-= (const Fraction& that);
		const Fraction operator*= (const Fraction& that);
		const Fraction operator/= (const Fraction& that); 
};
#endif
