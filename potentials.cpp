#include <cmath>
#include <iostream>

#include "potentials.h"
#include "settings.h"
#include "vectron.h"

using namespace std;

extern prog_control settings;

//***************************************************************************//
//			Function definitions:				     //
//***************************************************************************//	

void lennard_jones_force(vector &force, vector separation, const double separation_magnitude)
{
	if(separation_magnitude == 0)
	{
		cout << "Error: atom separation is 0  (inside lennard_jones_force)" << endl;
		exit(0);
	}
	//calculate vector from one atom to the other, and extract associated direction (i.e. the separation of the atoms):		
	vector force_direction = separation.unit_vector();
			
	//Calculate the magnitude of the force between the atoms
	double force_magnitude = 4 * settings.get_epsilon() * (12*pow(settings.get_sigma(),12)/pow(separation_magnitude,13) - 6*pow(settings.get_sigma(),6)/pow(separation_magnitude,7));
	force += force_direction*force_magnitude;
}

void lennard_jones_potential(double &potential, const double separation)
{
	//Calculate potential:
	potential = lennard_jones_potential(separation);
}

double lennard_jones_potential(const double separation)
{
	if(separation == 0)
	{
		cout << "Error: atom separation is 0  (inside lennard_jones_potential)" << endl;
		exit(0);
	}
	//Calculate potential:
	double potential = 4 * settings.get_epsilon() * ( pow( (settings.get_sigma()/separation), 12 ) - pow( (settings.get_sigma()/separation),6) );
	
	return potential;
}

void coulomb_force(vector &force, vector separation, const double separation_magnitude, const int charge_a, const int charge_b)
{
	if(separation_magnitude == 0)
	{
		cout << "Error: ion separation is 0 (inside coulomb_force)" << endl;
		exit(0);
	}
	vector force_direction = charge_a*charge_b*separation.unit_vector();
	double force_magnitude = abs(charge_a*charge_b)/(separation_magnitude*separation_magnitude);
	force += force_direction*force_magnitude;
}

void coulomb_potential(double &potential, const double separation, const int charge_a, const int charge_b)
{
	potential = coulomb_potential(separation,charge_a,charge_b);
}

double coulomb_potential(const double separation, const int charge_a, const int charge_b)
{
	if(separation == 0)
	{
		cout << "Error: ion separation is 0 (inside Coulomb potential)" << endl;
		exit(0);
	}
	double potential = charge_a*charge_b/separation;
	return potential;
}
