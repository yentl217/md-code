#include "atom.h"
#include "vectron.h"
#include "acceleration.h"
#include "potentials.h"
#include "settings.h"
#include "integration_methods.h"
#include "analysis.h"
#include "thermostat.h"

#include <cmath>
#include <iostream>

using namespace std;

extern prog_control settings;
extern double actual_temp;

//***************************************************************************//
//			Function definitions:				     //
//***************************************************************************//

void pbc_relocate_atoms(vector &position)
{
	//Check if each coordinate of the atom is bigger than it's allowed to be, and if so shift it to the allowable region:
	if (abs(position.x - settings.get_cellcentre().x) > (settings.get_cellsize()/2)) position.x -= settings.get_cellsize() * floor(position.x / settings.get_cellsize());
	if (abs(position.y - settings.get_cellcentre().y) > (settings.get_cellsize()/2)) position.y -= settings.get_cellsize() * floor(position.y / settings.get_cellsize());
	if (abs(position.z - settings.get_cellcentre().z) > (settings.get_cellsize()/2)) position.z -= settings.get_cellsize() * floor(position.z / settings.get_cellsize());
}

void euler(atom *atoms, const int num_atoms)
{
	//Calculate acceleration for this timestep:
	acceleration(atoms,num_atoms);
	
	//Update velocity and acceleration based on this
	for(int i=0; i < num_atoms; i++)
	{
		atoms[i].position += atoms[i].velocity * settings.get_stepsize();
		if (settings.get_pbc() == 1) pbc_relocate_atoms(atoms[i].position);
		atoms[i].velocity += atoms[i].acceleration * settings.get_stepsize();	
	}
	if(settings.thermo_status() ==1)
	{
		calculate_actual_temperature(atoms, num_atoms);
		for(int i = 0; i < num_atoms; i++)
		{
			gaussian_thermostat(actual_temp, settings.get_expected_temp(), atoms[i].velocity);
		}
	}
}

void velocity_verlet(atom *atoms, const int num_atoms)
{
	for(int i = 0; i < num_atoms; i++)
	{
		atoms[i].position += settings.get_stepsize()*((atoms[i].velocity) + 0.5*(atoms[i].acceleration)*settings.get_stepsize());
		if (settings.get_pbc() == 1) pbc_relocate_atoms(atoms[i].position);
		atoms[i].velocity += 0.5*atoms[i].acceleration*settings.get_stepsize();
	}
	
	//calculate new acceleration halfway in integration step
	acceleration(atoms,num_atoms);
	
	for(int i = 0; i < num_atoms; i++)
	{
		atoms[i].velocity += 0.5*atoms[i].acceleration*settings.get_stepsize();
	}
	if(settings.thermo_status() ==1)
	{
		calculate_actual_temperature(atoms, num_atoms);
		for(int i = 0; i < num_atoms; i++)
		{
			gaussian_thermostat(actual_temp, settings.get_expected_temp(), atoms[i].velocity);
		}
	}
}

void runge_kutta(atom *atoms, const int num_atoms, atom* temp_atoms, atom *original_atoms)
{
//need to store velocity and acceleration at 4 different times in timestep
	for (int i = 0; i < num_atoms; i++)
	{
		temp_atoms[i]= atoms[i];
		original_atoms[i] = atoms[i];
	}
	for (int i = 0; i < num_atoms; i++)
	{
		temp_atoms[i].position = original_atoms[i].position + 0.5*settings.get_stepsize()*temp_atoms[i].velocity;
		if (settings.get_pbc() == 1) pbc_relocate_atoms(temp_atoms[i].position);
		
		atoms[i].position += settings.get_stepsize()/6*temp_atoms[i].velocity;
		
		temp_atoms[i].velocity = original_atoms[i].velocity + 0.5*settings.get_stepsize()*temp_atoms[i].acceleration;
		
		atoms[i].velocity += settings.get_stepsize()/6*temp_atoms[i].acceleration;
	}
	acceleration(temp_atoms,num_atoms);
	for (int i = 0; i < num_atoms; i++)
	{
		temp_atoms[i].position = original_atoms[i].position + 0.5*settings.get_stepsize()*temp_atoms[i].velocity;
		if (settings.get_pbc() == 1) pbc_relocate_atoms(temp_atoms[i].position);
		
		atoms[i].position += settings.get_stepsize()/3*temp_atoms[i].velocity;
		
		temp_atoms[i].velocity = original_atoms[i].velocity + 0.5*settings.get_stepsize()*temp_atoms[i].acceleration;
		
		atoms[i].velocity += settings.get_stepsize()/3*temp_atoms[i].acceleration;
	}
	acceleration(temp_atoms,num_atoms);
	for (int i = 0; i < num_atoms; i++)
	{
		temp_atoms[i].position = original_atoms[i].position + settings.get_stepsize()*temp_atoms[i].velocity;
		if (settings.get_pbc() == 1) pbc_relocate_atoms(temp_atoms[i].position);
		
		atoms[i].position += settings.get_stepsize()/3*temp_atoms[i].velocity;
		
		temp_atoms[i].velocity = original_atoms[i].velocity + settings.get_stepsize()*temp_atoms[i].acceleration;
		
		atoms[i].velocity += settings.get_stepsize()/3*temp_atoms[i].acceleration;
	}
	acceleration(temp_atoms,num_atoms);
	for (int i = 0; i < num_atoms; i++)
	{
		atoms[i].position += settings.get_stepsize()/6*temp_atoms[i].velocity;
		if (settings.get_pbc() == 1) pbc_relocate_atoms(atoms[i].position);
		atoms[i].velocity += settings.get_stepsize()/6*temp_atoms[i].acceleration;
	}
	acceleration(atoms,num_atoms);
	if(settings.thermo_status() ==1)
	{
		calculate_actual_temperature(atoms, num_atoms);
		for(int i = 0; i < num_atoms; i++)
		{
			gaussian_thermostat(actual_temp, settings.get_expected_temp(), atoms[i].velocity);
		}
	}
}
