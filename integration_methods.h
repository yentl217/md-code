#ifndef INTEGRATION_METHODS_H
#define INTEGRATION_METHODS_H

#include "atom.h"

//This header file contains the definitions of the integration methods that may be used

//***************************************************************************//
//			Function protypes:				     //
//***************************************************************************//

//Name: 	euler
//
//Description:	Numerical integration using the Euler method
//
//TODO:		TESTING - check implementation is correct!

void euler(atom *atoms, const int num_atoms);

//Name:		velocity_verlet
//
//Description:	Numerical integration using the velocity verlet method
//
//TODO:		Check algorithm is implemented correctly.

void velocity_verlet(atom *atoms, const int num_atoms);

//Name:		runge_kutta
//
//Description	Does what it says on the tin
//
//TODO:		

void runge_kutta(atom *atoms, const int num_atoms, atom *temp_atoms, atom *original_atoms);

void pebc(vector &position);

#endif

