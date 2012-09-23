#ifndef POTENTIALS_H
#define POTENTIALS_H

#include "vectron.h"

//This header file contains the prototypes of potential and force functions

//***************************************************************************//
//			Function protypes:				     //
//***************************************************************************//

//Lennard Jones stuff:
	//Force
void lennard_jones_force(vector &force_direction, double &force_magnitude, vector separation, const double separation_magnitude);
	//Potential
void lennard_jones_potential(double &potential, const double separation);
double lennard_jones_potential(const double separation);

//Other potentials:
//TODO: eat chocolate.

#endif
