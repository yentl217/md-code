#include <cmath>

#include "analysis.h"
#include "atom.h"
#include "potentials.h"
#include "vectron.h"
#include "settings.h"

using namespace std;

extern prog_control settings;

//***************************************************************************//
//			Function definitions:				     //
//***************************************************************************//

double kinetic_energy(atom *atoms, const int num_atoms)
{
	double kinetic_energy = 0;
	for (int i = 0; i < num_atoms; i++)
	{
		kinetic_energy += 0.5*(atoms[i].atomic_mass)*dotproduct(atoms[i].velocity,atoms[i].velocity);
	}
	return kinetic_energy*0.000103642695;
}

double potential_energy(atom *atoms, const int num_atoms)
{
	double potential_energy = 0;
	
	//Loop over all of the atoms	
	for (int i = 0; i < num_atoms; i++)
	{
		for (int j = i+1; j < num_atoms; j++)
		{
			//Calculate separation of atoms
			vector separation = atoms[i].position - atoms[j].position; 
			
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
						//Calculate potential between atoms
						potential_energy += lennard_jones_potential(separation.magnitude());
						break;
					}
				}
			}
			else
			{	
				double separation_magnitude = separation.magnitude();	
				if (separation_magnitude <= settings.get_cutoff()) 
				{
					//Calculate potential between atoms
					potential_energy += lennard_jones_potential(separation_magnitude);
				}
			}
		}
	}
	return potential_energy*0.000103642695;
	//Convert to eV by: E * atomic_mass_unit * Angstroms^2 * ps^-2 / electron charge
}

double total_energy(atom *atoms, const int num_atoms)
{
	return (kinetic_energy(atoms, num_atoms) + potential_energy(atoms, num_atoms));
}

void calculate_actual_temperature(atom *atoms, const int num_atoms)
{
	double temperature = 2*kinetic_energy(atoms, num_atoms)/(num_atoms*3*0.000086173324);
	settings.actual_temp = temperature;
}	

