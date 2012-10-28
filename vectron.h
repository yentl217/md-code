#ifndef VECTRON_H
#define VECTRON_H

//This header file contains the 3D vector class definition and associated functions
//
//Operations defined:
// - Assignment and array-style assignment		( setvector(x,y,z), = , vector[i] )
// - Vector addition and subtraction 			( +, -, +=, -= )
// - Multiplication or division by scalar 		( *, / )
// - Magnitude of a vector 				( magnitude() )
// - Create unit vector 				( unit_vector() )
// - Vector multiplication - dot and cross products	( dotproduct(a,b), crossproduct(a,b) )

//***************************************************************************//
//			Class definitions:				     //
//***************************************************************************//

class vector
{
	public:
	
	double x, y, z;
	vector() { x=y=z=0; }
	vector(double i, double j, double k) { x=i; y=j; z=k; }
	
	void set_vector(double x, double y, double z);
	void set_vector();
		
	//Define vector operations:
	double &operator[](int i);				//Array-style assignment
	vector operator+(vector op2);				//Vector addition
	vector operator-(vector op2);				//Vector subtraction
	void operator+=(vector op2);				//Vector +=
	void operator-=(vector op2); 				//Vector -=
	void operator*=(double op2);				//Vector *=
	friend vector operator*(vector op1, double op2);	//Multiplication by scalar
	friend vector operator*(double op1, vector op2);	//Multiplication by scalar
	vector operator/(double op2);				//Division by scalar
	
	friend vector crossproduct(vector vec1, vector vec2);	//Vector cross product
	friend double dotproduct(vector vec1, vector vec2);	//Vector dot product
	
	double magnitude();					//Magnitude of vector
	vector unit_vector();					//Direction of vector
};

#endif
