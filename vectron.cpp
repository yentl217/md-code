#include <cmath>
#include <iostream>
#include <cstdlib>

#include "vectron.h"

using namespace std;

double &vector::operator[](int i)
{
	switch(i)
	{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			cout << "Error: " << i << "th member of vector doesn't exist!" << endl;
			exit(0);
			break;
	}
}

vector vector::operator+(vector op2)
{
	vector temp;
	temp.x = x + op2.x;
	temp.y = y + op2.y;
	temp.z = z + op2.z;
	
	return temp;
}

vector vector::operator-(vector op2)
{
	vector temp;
	temp.x = x - op2.x;
	temp.y = y - op2.y;
	temp.z = z - op2.z;
	
	return temp;
}

void vector::operator+=(vector op2)
{
	x += op2.x;
	y += op2.y;
	z += op2.z;
}


void vector::operator-=(vector op2)
{
	x -= op2.x;
	y -= op2.y;
	z -= op2.z;
}

void vector::operator*=(double op2)
{
	x *= op2;
	y *= op2;
	z *= op2;
}

void vector::operator/=(double op2)
{
	if(op2 == 0)
	{
		cout << endl << "Error: Oops, someone's trying to divide by zero :S." << endl;
		exit(0);
	}
	x /= op2;
	y /= op2;
	z /= op2;
}

vector operator*(vector op1, double op2)
{
	vector temp;
	temp.x = op1.x * op2;
	temp.y = op1.y * op2;
	temp.z = op1.z * op2;
	
	return temp;
}

vector operator*(double op1, vector op2)
{
	vector temp;
	temp.x = op2.x * op1;
	temp.y = op2.y * op1;
	temp.z = op2.z * op1;
	
	return temp;
}
	
vector vector::operator/(double op2)
{
	if(op2 == 0)
	{
		cout << endl << "Error: Oops, someone's trying to divide by zero :S." << endl;
		exit(0);
	}
	vector temp;
	temp.x = x / op2;
	temp.y = y / op2;
	temp.z = z / op2;
	
	return temp;
}

void vector::set_vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void vector::set_vector()
{
	x=y=z=0;
}

double dotproduct(vector vec1, vector vec2)
{
	double dotproduct = vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
	
	return dotproduct;
}

vector crossproduct(vector vec1, vector vec2)
{
	vector temp;
	temp.x = vec1.y*vec2.z - vec1.z*vec2.y;
	temp.y = vec1.z*vec2.x - vec1.x*vec2.z;
	temp.z = vec1.x*vec2.y - vec1.y*vec2.x;
	
	return temp;
}

double vector::magnitude()
{
	double magnitude = sqrt(dotproduct(*this,*this));
	
	return magnitude;
}

vector vector::unit_vector()
{
	vector unit_vector = *this/(this->magnitude());
	
	return unit_vector;
}
