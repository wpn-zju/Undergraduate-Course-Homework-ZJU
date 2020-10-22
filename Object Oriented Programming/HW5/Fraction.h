#ifndef _FRACTION_H
#define _FRACTION_H
#include <iostream>
#include <sstream>
#include <string>

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
		std::string ToString();
		friend std::istream& operator>> (std::istream& is, Fraction& obj);
		friend std::ostream& operator<< (std::ostream& is, Fraction& obj);
		const Fraction operator+= (const Fraction& that);
		const Fraction operator-= (const Fraction& that);
		const Fraction operator*= (const Fraction& that);
		const Fraction operator/= (const Fraction& that); 
};
#endif
