#pragma once
#include <iostream>
class Rational {
private:
	int chis;
	int znam;
	static int abs(int);
	static int __gcd(int, int);
	Rational& optimize();
	void Copy(const Rational& other);
public:
	Rational(int chisNew = 0, int znamNew = 1);
	Rational(const Rational& other);

	Rational& operator = (const Rational& other);
	Rational operator - () const;
	Rational operator ~ () const; /// a/b -> b/a
	//operator double() const;
	double getDouble() const;

	friend std::ostream& operator<<(std::ostream& out, const Rational& other);

	int getChis() const;
	int getZnam() const;
	class zeroDivision{};
};

Rational operator + (const Rational& a, const Rational& b);
Rational operator * (const Rational& a, const Rational& b);

Rational& operator += (Rational& a, const Rational& b);
Rational operator - (const Rational& a, const Rational& b);
Rational& operator -= (Rational& a, const Rational& b);
Rational& operator *= (Rational& a, const Rational& b);
Rational operator / (const Rational& a, const Rational& b);
Rational& operator /= (Rational& a, const Rational& b);

bool operator ==(const Rational& a, const Rational& b);
bool operator >(const Rational& a, const Rational& b);

bool operator !=(const Rational& a, const Rational& b);
bool operator >=(const Rational& a, const Rational& b);
bool operator <(const Rational& a, const Rational& b);
bool operator <=(const Rational& a, const Rational& b);