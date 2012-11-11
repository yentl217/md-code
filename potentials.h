#ifndef POTENTIALS_H
#define POTENTIALS_H

#include "vectron.h"

//This header file contains the prototypes of potential and force functions

//***************************************************************************//
//			Function protypes:				     //
//***************************************************************************//

//Lennard Jones stuff:
	//Force
void lennard_jones_force(vector &force, vector separation, const double separation_magnitude);
	//Potential
void lennard_jones_potential(double &potential, const double separation);
double lennard_jones_potential(const double separation);

//Coulomb potential:
	//Force
void coulomb_force(vector &force, vector separation, const double separation_magnitude, const int charge_a, const int charge_b);
	//Potential
void coulomb_potential(double &potential, const double separation, const int charge_a, const int charge_b);
double coulomb_potential(const double separation, const int charge_a, const int charge_b);

//Other potentials:
//TODO: eat chocolate.

#endif
