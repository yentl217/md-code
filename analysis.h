#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "atom.h"

//This header file contains the definitions of the analysis techniques used to work stuff out about the system

//***************************************************************************//
//			Function protypes:				     //
//***************************************************************************//

//Name:		total_energy
//
//Description:	Calculates the total energy of the system of atoms passed as
//		arguments using the Lennard Jones potential.
//
//TODO:		TESTING!!!! This only works with Lennard Jones, ought to be
//		easy to change potential... :S.

double total_energy(atom *atoms, const int num_atoms);

#endif

