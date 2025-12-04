#include "rational.h"
int Rational::abs(int a) {
	return (a < 0 ? -a : a);
}
int Rational::__gcd(int a, int b){
	if (a < b) {
		a ^= b;
		b ^= a;
		a ^= b;
	}
	if (b == 0) {
		return a;
	}
	a %= b;
	return __gcd(b,a);
} 
Rational& Rational::optimize() {
	if (znam < 0) {
		znam *= -1;
		chis *= -1;
	}
	int gc = __gcd(abs(chis), znam);
	chis /= gc;
	znam /= gc;
	return *this;
}
void Rational::Copy(const Rational& other) {
	chis = other.chis;
	znam = other.znam;
}
int Rational::getChis() const {
	return chis;
}
int Rational::getZnam() const {
	return znam;
}
Rational::Rational(int chisNew, int znamNew) {
	if (znamNew == 0) {
		throw zeroDivision();
	}
	chis = chisNew;
	znam = znamNew;
	optimize();
}
Rational::Rational(const Rational& other):Rational(other.chis,other.znam) { }
Rational& Rational::operator = (const Rational& other) {
	if (&other != this) {
		Copy(other);
	}
	return *this;
}
Rational Rational::operator-() const{
	return Rational(-chis, znam);
}
Rational Rational::operator~() const{
	return Rational(znam, chis);
}
/*
Rational::operator double() const {
	return (double)chis / znam;
}*/
double Rational::getDouble() const {
	return (double)chis / znam;
}
Rational operator + (const Rational& a, const Rational& b) {
	int ch1 = a.getChis();
	int ch2 = b.getChis();
	int zn1 = a.getZnam();
	int zn2 = b.getZnam();
	return Rational(ch1 * zn2 + ch2 * zn1, zn1 * zn2);
}
Rational operator * (const Rational& a, const Rational& b) {
	return Rational(a.getChis() * b.getChis(), a.getZnam() * b.getZnam());
}
Rational& operator += (Rational& a, const Rational& b) {
	a = a + b;
	return a;
}
Rational operator - (const Rational& a, const Rational& b) {
	return a + (-b);
}
Rational& operator -= (Rational& a, const Rational& b) {
	a += (-b);
	return a;
}
Rational& operator *= (Rational& a, const Rational& b) {
	a = a * b;
	return a;
}
Rational operator / (const Rational& a, const Rational& b) {
	return a * (~b);
}
Rational& operator /= (Rational& a, const Rational& b) {
	a *= (~b);
	return a;
}
std::ostream& operator <<(std::ostream& out, const Rational& other) {
	out << other.chis;
	if (other.znam != 1) {
		out << "/" << other.znam;
	}
	return out;
}
bool operator == (const Rational& a, const Rational& b) {
	return (a.getChis() == b.getChis() && a.getZnam() == b.getZnam());
}
bool operator > (const Rational& a, const Rational& b) {
	return (a.getChis() * b.getZnam() > b.getChis() * a.getZnam());
}
bool operator !=(const Rational& a, const Rational& b) {
	return !(a == b);
}
bool operator >=(const Rational& a, const Rational& b) {
	return (a == b || a > b);
}
bool operator <(const Rational& a, const Rational& b) {
	return !(a >= b);
}
bool operator <=(const Rational& a, const Rational& b) {
	return !(a > b);
}