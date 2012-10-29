#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include "vectron.h"

using namespace std;

//This file contains the settings class used to control program execution, along with a function that processes command line arguments.

//***************************************************************************//
//			set constants and variables:			     //
//***************************************************************************//

class prog_control
{
	//Files
	string default_input_file;		//Location of default input file
	string input_file_location;		//Location of input file to use
	bool input_file_exist;			//Input file verified to exist
	bool print_initial_atom_data;		//Print the initial atom data?
	string default_output_file;		//As above
	string output_file_location;		//...
	bool output_file_exist;			//...
	int steps_per_output;			//Number of steps per output to output file
	
	//Type of run
	bool damped_md;				//Use damped MD?
	bool dummy_run;				//Is dummy run? (for checking settings are read correctly)
	
	//Temperature
	bool thermo_switch;		//Is the thermostat on?
	double expected_temp;		//expected temperature
	bool monitor_actual_temp;	//display actual temperature?
	
	//Periodic boundary conditions settings
	bool pbc;				//Use periodic boundariy conditions?
	double cellsize;			//Size of the cell used for periodic boundaries
	vector cellcentre;			//Location of periodic boundary cell centre

	//Integration method
	public:
	enum integration_methods
	{
		euler,
		velocity_verlet,
		runge_kutta_4
	};
	private:
	integration_methods integration_method;	//Which integration method should be used?
	unsigned int num_steps;			//Number of timesteps
	double stepsize;			//Length of timesteps
	
	//Potential
//	int potential_to_use;			//TODO: Implement potentials and setting to control them here!
	// - Lennard Jones potential stuff:
	double epsilon;				//Size of epsilon (related to depth of Lennard-Jones potential well)
	double sigma;				//Size of sigma (related to distance from equilibrium point of two atoms)
	double cutoff;				//Cutoff for Lennard-Jones potential, beyond which force = 0	
	
	public:
	//Default constructor
	prog_control()
	{
		//Files
		default_input_file = "./input";
		input_file_location = default_input_file;
		input_file_exist = 0;
		print_initial_atom_data = 0;
		default_output_file = "./output";
		output_file_location = default_output_file;
		output_file_exist = 0;
		steps_per_output = 1;
		
		//Type of run
		damped_md = 0;
		dummy_run = 0;

		//Temperature
		thermo_switch = 0;
		expected_temp = 298; //Kelvins
		monitor_actual_temp = 0;
		
		//Periodic boundary conditions settings
		pbc = 0;
		cellsize = 100;
		cellcentre.set_vector(0.0,0.0,0.0);
		
		//Integration method
		integration_method = velocity_verlet;
		num_steps = 1000;
		stepsize = 0.005;

		//Potential
		epsilon = 124.524843;
		sigma = 3.7;
		cutoff = 50;
	}
	
	//Accessor function prototypes
	//Files
	string get_input_file_location();
	bool input_file_exists();
	bool is_print_initial_atom_data();
	string get_output_file_location();
	bool output_file_exists();
	int get_steps_per_output();

	//Type of run
	bool is_damped_md();
	bool is_dummy_run();

	//Temperature
	bool thermo_status();
	double get_expected_temp();
	bool start_monitor_actual_temp();
	double actual_temp;
	
	//Periodic boundary conditions settings
	bool get_pbc();
	double get_cellsize();
	vector get_cellcentre();
	
	//Integration method
	integration_methods get_integration_method();
	unsigned int get_num_steps();
	double get_stepsize();
	
	//Potential
//	int get_potential_to_use();
	double get_epsilon();
	double get_sigma();
	double get_cutoff();

	//Other function prototypes
	//Function to output values to terminal
	void output();
	
	//Command line argument function needs access to this class' private members
	friend bool command_line_argh(const int num_args, string *argh);
};

//***************************************************************************//
//			Function protypes:				     //
//***************************************************************************//

//Name: 	command_line_argh
//
//Description:	Detects command line arguments input by the user and handles
//		them appropriately. Allows user to change the way the program
//		behaves.
//
//Inputs:	Number of command line arguments (num_args), pointer to the
//		first string (**argh)
//
//Outputs:	File locations (*&output_file_location, *&input_file_location)
//
//Returns:	0 if successful, 1 on errors.
//
//TODO:		Handle other inputs, deal with input errors more effectively,
//		check types are correct TESTING.

bool command_line_argh(const int num_args, string *argh);	//TODO: How do we make argh const?

#endif

