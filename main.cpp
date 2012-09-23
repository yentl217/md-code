#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "acceleration.h"
#include "analysis.h"
#include "atom.h"
#include "file_io.h"
#include "integration_methods.h"
#include "settings.h"
#include "vectron.h"

using namespace std;

//***************************************************************************//
//			set constants and variables:			     //
//***************************************************************************//

prog_control settings;
double initial_energy;
double current_energy;

//***************************************************************************//
//			Main function:					     //
//***************************************************************************//

int main(int argc, char **argv)
{
	int num_atoms;
	atom *atoms;

	//Deal with command line arguments:
	if(command_line_argh(argc, argv) == 1) return 1;
	
	//Output recorded settings for debugging:
	settings.output();
	if(settings.is_dummy_run() == 1)
	{
		cout << "Dummy run: terminating now." << endl;
		return 0;
	}
	
	cout << "Reading atoms in from file..." << endl;
	//Read input file from location specified in arguments.
	if(read_input(atoms, num_atoms, 1) == 1) return 1;
	cout << "Found " << num_atoms << " atoms in input file.\n" << endl;

	//Clear the output file. TODO: Check if output file exists, if not then create one.
	if(clear_file(settings.get_output_file_location()) == 1) return 1;
	
//***************************************************************************//
//			      Start of simulation			     //
//***************************************************************************//
	
	cout << "Starting simulation!" << endl;
	
	//Output initial state:
	output_state(atoms, num_atoms);
	
	//Start simulation by calculating the initial forces
	acceleration(atoms,num_atoms);
	
	cout << "Initial acceleration is: (" << (atoms[0].acceleration).x << "," << (atoms[0].acceleration).y << "," << (atoms[0].acceleration).z << ")" << endl;
	
	initial_energy = total_energy(atoms,num_atoms);
	cout << "Initial energy is: " << initial_energy << " eV" << endl;
	
	//If using RK4 allocate memory for it. TODO: Use a less hacky method :(.
	atom *temp_atoms;
	atom *original_atoms;
	
	if(settings.get_integration_method() == prog_control::runge_kutta_4)
	{
		temp_atoms = new atom [num_atoms];
		original_atoms = new atom [num_atoms];
	}

	//Main loop!
	for (int i = 0; i < settings.get_num_steps(); i++) 
	{
		//Choose which integration method to use and then progress system by one integration step.
		if(settings.get_integration_method() == prog_control::velocity_verlet) velocity_verlet(atoms, num_atoms);
		else if(settings.get_integration_method() == prog_control::runge_kutta_4) runge_kutta(atoms,num_atoms,temp_atoms,original_atoms);
		else if(settings.get_integration_method() == prog_control::euler) euler(atoms,num_atoms);
		
		//Damp system if using damped MD.
		if(settings.is_damped_md() == 1) for(int i=0; i < num_atoms; i++) (atoms[i].velocity).set_vector();
		
		//Calculate current energy
		current_energy = total_energy(atoms,num_atoms);
	
		//Output state of the system to location specified.
		if(settings.get_steps_per_output() != 0)
		{
			if((i % settings.get_steps_per_output()) == 0) output_state(atoms, num_atoms);
		}
		
		//Output progress
		if( (100*i) % settings.get_num_steps() == 0 )
		{
			cout << "Progress:" << setw(4) << ((100 *i)/settings.get_num_steps()) << "% ";
			cout << "Total energy: " << setprecision(6) << current_energy << " eV\t(";
			cout << setprecision(4) << (((current_energy - initial_energy)/initial_energy) * 100) << "% deviation from original)" << endl;	
		}
	}
	
	//If there are no file outputs during the run, output to file now.
	if(settings.get_steps_per_output() == 0) output_state(atoms,num_atoms);
	
	//Output final energy to console.
	cout << "Final energy is: " << total_energy(atoms, num_atoms) << " eV" << endl;
	
	//If using RK4, deallocate its memory.
	if(settings.get_integration_method() == prog_control::runge_kutta_4)
	{
		delete [] temp_atoms;
		delete [] original_atoms;
	}
	
	//Clean up atom array
	delete [] atoms;
	return 0;
}

