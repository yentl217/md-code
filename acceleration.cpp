#include <cmath>
#include <iostream>

#include "acceleration.h"
#include "atom.h"
#include "vectron.h"
#include "potentials.h"
#include "settings.h"

using namespace std;

extern prog_control settings;

//***************************************************************************//
//			Function definitions:				     //
//***************************************************************************//

void acceleration(atom *atoms, const int num_atoms)
{
	//set previous acceleration values to zero for fresh calculation
	for (int i = 0; i < num_atoms; i++)
	{
		(atoms[i].acceleration).set_vector();
	}
	
	//operate exclusively on separation vector
	for (int i = 0; i < num_atoms; i++)
	{
		for (int j = i+1; j < num_atoms; j++)
		{
			//Calculate separation of atoms
			vector separation = atoms[i].position - atoms[j].position; 
			double force_magnitude = 0.0;
			vector force_direction;	
			if (settings.get_pbc() == 1) 
			{
				//check if either copy of atom j within cellsize of atom i along an axis is within cutoff of atom i along that axis
				//if not, move to next atom
				//otherwise, check whether the chosen copy of atom j has separation within cutoff of atom i already
				//if so, interact and move to next atom
				//otherwise, check next axis or if all three axes checked, move to next atom
				
				for (int k = 0; k < 3; k++)
				{
					if (abs(separation[k]) > settings.get_cutoff())
					{
						if ((settings.get_cellsize() - abs(separation[k])) > settings.get_cutoff()) break;
						separation[k] = abs(separation[k])-settings.get_cellsize();
					}
					
					if (separation.magnitude() <= settings.get_cutoff()) 
					{
						//Calculate forces on atoms
						double separation_magnitude = separation.magnitude();
						lennard_jones_force(force_direction, force_magnitude, separation, separation_magnitude);
						if (settings.get_use_coulomb() == 1)
						{
							coulomb_force(force_direction, force_magnitude, separation, separation_magnitude, atoms[i].charge, atoms[j].charge);
						}
						
						//Update acceleration of atoms
						atoms[i].acceleration += force_magnitude*force_direction/atoms[i].atomic_mass;
						atoms[j].acceleration -= force_magnitude*force_direction/atoms[j].atomic_mass;
						break;
					}
				}
			}
			else
			{	
				double separation_magnitude = separation.magnitude();
				if (separation_magnitude <= settings.get_cutoff()) 
				{
					//Calculate forces on atoms
					lennard_jones_force(force_direction,force_magnitude,separation,separation_magnitude);
					if (settings.get_use_coulomb() == 1)
					{
						coulomb_force(force_direction, force_magnitude, separation, separation_magnitude, atoms[i].charge, atoms[j].charge);
					}
					//Calculate acceleration of atoms
					atoms[i].acceleration += force_magnitude*force_direction/atoms[i].atomic_mass;
					atoms[j].acceleration -= force_magnitude*force_direction/atoms[j].atomic_mass;
				}
			}
		}
	}
}
