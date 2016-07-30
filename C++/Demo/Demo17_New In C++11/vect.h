#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

namespace VECTOR
{

class Vector
{
public:
	enum Mode {RECT, POL}; //RECT for rectangular, POL for Polar modes
private:
	double x;
	double y;
	double mag;
	double ang;
	Mode mode;
private:
	void set_x();
	void set_y();
	void set_mag();
	void set_ang();
public:
	Vector();
	Vector(double n1, double n2, Mode form = RECT);
	~Vector();
	double xval() const { return x; }
	double yval() const { return y; }
	double magval() const { return mag; }
	double angval() const { return ang; }
	void polar_mode();
	void rect_mode();
	//operator overloading
	Vector operator+ (const Vector & b) const;
	Vector operator- (const Vector & b) const;
	Vector operator- () const;
	Vector operator* (double n) const;
	//friends function
	friend Vector operator* (double n, const Vector & a);
	friend std::ostream & operator<< (std::ostream & os, const Vector & v);
};

} //end namespace VECTOR

#endif //_VECTOR_H_