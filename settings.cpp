#include <iostream>
#include <string>

#include "file_io.h"
#include "settings.h"

using namespace std;

extern prog_control settings;

//***************************************************************************//
//			Function definitions:				     //
//***************************************************************************//

//Accessor function definitions
//Files

string prog_control::get_input_file_location() { return input_file_location; }

bool prog_control::input_file_exists()
{
	input_file_exist = file_exists(settings.input_file_location);
	return input_file_exist;
}

bool prog_control::is_print_initial_atom_data() { return print_initial_atom_data; }

string prog_control::get_output_file_location() { return output_file_location; }

bool prog_control::output_file_exists()
{
	output_file_exist = file_exists(settings.output_file_location);
	return output_file_exist;
}

int prog_control::get_steps_per_output() { return steps_per_output; }

//Type of run

bool prog_control::is_damped_md() { return damped_md; }

bool prog_control::is_dummy_run() { return dummy_run; }

//Temperature
bool prog_control::thermo_status() {return thermo_switch; } 
double prog_control::get_expected_temp() {return expected_temp; }
bool prog_control::start_monitor_actual_temp() {return monitor_actual_temp; }


//Periodic boundary conditions settings

bool prog_control::get_pbc() { return pbc; }

double prog_control::get_cellsize() { return cellsize; }

vector prog_control::get_cellcentre() { return cellcentre; }

//Integration method

prog_control::integration_methods prog_control::get_integration_method() { return integration_method;}

unsigned int prog_control::get_num_steps() { return num_steps; }

double prog_control::get_stepsize() { return stepsize; }

//Potential

double prog_control::get_epsilon() { return epsilon; }

double prog_control::get_sigma() { return sigma; }

double prog_control::get_cutoff() { return cutoff; }

//Other function definitions
void prog_control::output()
{
	cout << "Settings:" << endl << endl;
	cout << "Run type:\t\t\t";
	if(settings.is_damped_md() == 1) cout << "Damped molecular dynamics" << endl;
	else cout << "Molecular dynamics" << endl;
	//if(run_type == MD)
	//{
	cout << " - Periodic boundaries:\t\t";
	if(pbc == 1)
	{
		cout << "Yes" << endl;
		cout << " -  - Period of boundary:\t" << cellsize << " Angstroms" << endl;
	}
	else cout << "No" << endl;
	//}
	cout << "Potential:\t\t\t" << "Lennard-Jones" << endl;
	cout << " - Epsilon:\t\t\t" << epsilon * 0.000103642695 << " eV" << endl;
	cout << " - Sigma:\t\t\t" << sigma << " Angstroms" << endl;
	cout << " - Cutoff:\t\t\t" << cutoff << " Angstroms" << endl;

	if(settings.integration_method == prog_control::euler) cout << "Integration method:\t\t" << "Euler" << endl;
	else if (settings.integration_method == prog_control::velocity_verlet) cout << "Integration method:\t\t" << "Velocity Verlet" << endl;
	else if (settings.integration_method == prog_control::runge_kutta_4) cout << "Integration method:\t\t" << "Runge-Kutta 4" << endl;
	else
	{
		cout << "Integration method:\t\t" << "Not recognised" << endl;
		exit(0);
	}
	cout << " - Stepsize:\t\t\t" << stepsize << " ps" << endl;
	cout << " - Number of steps:\t\t" << num_steps << endl;
	cout << "Input/Output settings:" << endl;
	cout << " - Input file:\t\t\t" << settings.input_file_location << endl;
	cout << " - Output file: \t\t" << settings.output_file_location << endl;
	cout << " - Steps per output:\t\t"<< settings.steps_per_output << endl;

	if(settings.thermo_status() == 1) 
	{
		cout << "Thermostat used" << endl; //TODO: allow for choice of thermostats
		cout << "Desired temperature: " << settings.expected_temp << endl;
		if (settings.start_monitor_actual_temp() == 1) 
		{ 
			cout << "Temperature is being monitored." << endl;
		}
		else
		{
			cout << "Temperature is not being monitored." << endl;
		}
	}
	

	cout << endl;
}

bool command_line_argh(const int num_args, string *argh)
{
	//Usage text:
	const string usage_text = "Usage: mmm [-d|--damped-md] [--dummy] [-e|--epsilon <epsilon> [<unit>]]\n\t   [-h|--help [<command>]] [-i|--input-file <input file>]\n\t   [-n|--num-steps <number of steps>] [-o|--output-file <output file>]\n\t   [-p|--periodic-boundaries] [--print-initial-atom-data]\n\t   [-s|--integration-method] [--sigma <sigma> [<units>]]\n\t   [-z|--stepsize <stepsize> [<units>]] [--steps-per-output]\n\t    [-t|--temperature <desired temperature> [<monitor>]]";
	
	if(num_args < 2)
	{
		cout << "Error - no simulation settings specified.\n\n" << usage_text << endl << endl;
		return 1;
	}
	
	//Loop runs through command line arguments and sets various parameters
	for(int i=1; i < num_args; i++)
	{
		if(argh[i][0] != '-') { cout << "Syntax error - option \"" << argh[i] << "\" not recognised.\n\n" << usage_text << endl; return 1; }
		else
		{
			//Check for damped MD
			if((argh[i] == "-d") || (argh[i] == "--damped-md"))
			{
				settings.damped_md = 1;
				continue;
			}
			//Check for dummy option (print settings and terminate)
			if(argh[i] == "--dummy")
			{
				settings.dummy_run = 1;
				continue;
			}

			//Check for thermostat settings
			else if((argh[i] == "-t") || (argh[i] == "--temperature"))
			{
				if((i+2) > num_args)
				{
					cout << "Syntax error - to specify a temperature for the system, use format: -t <temperature>. Note that this is in Kelvins." << endl;
					return 1;
				}
				/*else if (isdigit(argh[i+1][0]))
				{
					cout << "Syntax error - the temperature of the system must be entered as a number. Note that the unit is Kelvins." << endl;
					return 1;
				}*/
				else
				{
					settings.thermo_switch = 1;
					settings.expected_temp = atof(argh[i+1].c_str());
					if(argh[i+2] == "monitor")
					{
						settings.monitor_actual_temp = 1;
						i+=3;
					}
					else 
					{
						i+=2;
					}
				}
				continue;
			}
					
			//Check for epsilon value
			else if((argh[i] == "-e") || (argh[i] == "--epsilon"))
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify epsilon, use format: -e <epsilon> [<unit>]" << endl;
					return 1;
				}
				else
				{
					settings.epsilon = atof(argh[i+1].c_str());
					if(((i+3) <= num_args) && (argh[i+2][0] != '-'))
					{
						if((argh[i+2] == "MeV") || (argh[i+2] == "Mev"))
						{
							settings.epsilon *= 9648533360;
						}
						else if((argh[i+2] == "keV") || (argh[i+2] == "kev"))
						{
							settings.epsilon *= 9648533.36;
						}
						else if((argh[i+2] == "eV") || (argh[i+2] == "ev"))
						{
							settings.epsilon *= 9648.53336;
						}
						else if((argh[i+2] == "meV") || (argh[i+2] == "mev"))
						{
							settings.epsilon *= 9.64853336;
						}
						else if((argh[i+2] == "J") || (argh[i+2] == "j") || (argh[i+2] == "Joules") || (argh[i+2] == "joules"))
						{
							settings.epsilon *= 60221415000000000000000.0;
						}
						else if((argh[i+2] == "mJ") || (argh[i+2] == "mj") || (argh[i+2] == "milliJoules") || (argh[i+2] == "millijoules"))
						{
							settings.epsilon *= 60221415000000000000.0;
						}
						else if((argh[i+2] == "uJ") || (argh[i+2] == "uj") || (argh[i+2] == "microJoules") || (argh[i+2] == "microjoules"))
						{
							settings.epsilon *= 60221415000000000.0;
						}
						else if((argh[i+2] == "nJ") || (argh[i+2] == "nj") || (argh[i+2] == "nanoJoules") || (argh[i+2] == "nanojoules"))
						{
							settings.epsilon *= 60221415000000.0;
						}
						else if((argh[i+2] == "pJ") || (argh[i+2] == "pj") || (argh[i+2] == "picoJoules") || (argh[i+2] == "picojoules"))
						{
							settings.epsilon *= 60221415000.0;
						}
						else if((argh[i+2] == "fJ") || (argh[i+2] == "fj") || (argh[i+2] == "femtoJoules") || (argh[i+2] == "femtojoules"))
						{
							settings.epsilon *= 60221415;
						}
						else if((argh[i+2] == "aJ") || (argh[i+2] == "aj") || (argh[i+2] == "attoJoules") || (argh[i+2] == "attojoules"))
						{
							settings.epsilon *= 60221.415;
						}
						else if((argh[i+2] == "zJ") || (argh[i+2] == "zj") || (argh[i+2] == "zeptoJoules") || (argh[i+2] == "zeptojoules"))
						{
							settings.epsilon *= 60.221415;
						}
						else if((argh[i+2] == "yJ") || (argh[i+2] == "yj") || (argh[i+2] == "yoctoJoules") || (argh[i+2] == "yoctojoules"))
						{
							settings.epsilon *= 0.060221415;
						}
						else { cout << "Syntax error - to specify epsilon, use format: -e <epsilon> [<unit>]" << endl; return 1; }
						i++;
					}
				}
				i++;
				continue;
			}
			//Check for help request
			else if((argh[i] == "-h") || (argh[i] == "--help"))
			{
				//Explain commands in more detail
				cout << usage_text << endl << endl;
				//TODO: Check for additional commands - e.g. "--help --epsilon"
				return 1;
			}
			//Check for input file location
			else if((argh[i] == "-i") || (argh[i] == "--input-file"))
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify input file, use format: -i <input file>" << endl;
					return 1;
				}
				else
				{
					if(argh[i+1][0] == '-')
					{
						cout << "Syntax error - to specify input file, use format: -i <input file>" << endl;
						return 1;
					}
					else
					{
						settings.input_file_location=argh[i+1];
						if(settings.input_file_exists() == 1)
						{
							cout << "Error: input file \"" << argh[i+1] << "\" could not be opened" << endl;
							return 1;
						}
					}
				}
				i++;
				continue;
			}
			//Check for number of steps
			else if((argh[i] == "-n") || (argh[i] == "--num-steps"))
			{
				if((i+2) > num_args)
				{
					cout << "Syntax error - to specify number of steps, use format: -n <number of steps>" << endl;
					return 1;
				}
				else
				{
					if(argh[i+1][0] == '-')
					{
						cout << "Syntax error - to specify number of steps, use format: -n <number of steps>" << endl;
						return 1;
					}
					else
					{
						if(atoi(argh[i+1].c_str()) < 1) { cout << "I'm sorry, Dave. I'm afraid I can't do that." << endl; return 1; }
						settings.num_steps = atoi(argh[i+1].c_str());
						if(settings.num_steps < 3) cout << "Use a calculator, lazy!" << endl;
					
					}
				}
				i++;
				continue;
			}
			//Check for output file location
			else if((argh[i] == "-o") || (argh[i] == "--output-file"))
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify output file, use format: -o <output file>" << endl;
					return 1;
				}
				else
				{
					if(argh[i+1][0] == '-')
					{
						cout << "Syntax error - to specify output file, use format: -o <output file>" << endl;
						return 1;
					}
					else
					{
						settings.output_file_location=argh[i+1];
						if(settings.output_file_exists() == 1)
						{
							cout << "Error: output file \"" << argh[i+1] << "\" could not be opened" << endl;
							return 1;
						}
					}
				}
				i++;
				continue;
			}			
			//Check for periodic boundaries
			else if((argh[i] == "-p") || (argh[i] == "--periodic-boundaries"))
			{
				settings.pbc = 1;
				continue;
			}
			//check whether to output atom input information
			else if(argh[i] == "--print-initial-atom-data")
			{
				settings.print_initial_atom_data = 1;
				continue;
			}
			//check for integration method
			else if((argh[i] == "-s") || (argh[i] == "--integration-method"))
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify integration method, use format: --integration-method <method>" << endl;
					return 1;
				}
				else
				{
					string temp_string = argh[i+1];
					for(int j=1; j < temp_string.length(); j++) temp_string[j] = tolower(temp_string[j]);
					cout << temp_string << endl;

					if(temp_string == "-euler")
					{
						settings.integration_method = prog_control::euler;
					}
					else if(temp_string == "-velocity-verlet")
					{
						settings.integration_method = prog_control::velocity_verlet;
					}
					else if((temp_string == "-runge-kutta-4") || (temp_string == "-rk4") || (temp_string == "-rk-4") || (temp_string == "-r-k-4"))
					{
						settings.integration_method = prog_control::runge_kutta_4;
					}
					else
					{
						cout << "Syntax error - No valid integration method specified. Available integration\nmethods are velocity verlet (type 'verlet'), Runge-Kutta 4 (type 'rk4') and\nEuler integration (type 'euler').\nDo not use Euler unless risk of death is involved." << endl;
						return 1;
					}
				}
				i++;
				continue;
			}
			//Check for sigma
			else if(argh[i] == "--sigma")
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify sigma, use format: --sigma <sigma> [<unit>]" << endl;
					return 1;
				}
				else
				{
					settings.sigma = atof(argh[i+1].c_str());
					if(settings.sigma <= 0)
					{
						cout << "Error: sigma cannot be less than or equal to 0." << endl;
						return 1;
					}
					if(((i+3) <= num_args) && (argh[i+2][0] != '-'))
					{
						if((argh[i+2] == "A") || (argh[i+2] == "Angstroms") || (argh[i+2] == "angstroms"))
						{
							settings.sigma *= 1;
						}
						else if((argh[i+2] == "nm") || (argh[i+2] == "nanometers"))
						{
							settings.sigma *= 10;
						}
						else if((argh[i+2] == "um") || (argh[i+2] == "micrometers"))
						{
							settings.sigma *= 10000;
						}
						else if((argh[i+2] == "mm") || (argh[i+2] == "millimeters"))
						{
							settings.sigma *= 10000000;
						}
						else if((argh[i+2] == "cm") || (argh[i+2] == "centimeters"))
						{
							settings.sigma *= 100000000;
						}
						else if((argh[i+2] == "m") || (argh[i+2] == "meters"))
						{
							settings.sigma *= 10000000000;
						}
						else if((argh[i+2] == "pm") || (argh[i+2] == "picometers"))
						{
							settings.sigma *= 0.01;
						}
						else if((argh[i+2] == "fm") || (argh[i+2] == "femtometers"))
						{
							settings.sigma *= 0.00001;
						}
						else if((argh[i+2] == "am") || (argh[i+2] == "attometers"))
						{
							settings.sigma *= 0.00000001;
						}
						else if((argh[i+2] == "zm") || (argh[i+2] == "zeptometers"))
						{
							settings.sigma *= 0.00000000001;
						}
						else if((argh[i+2] == "ym") || (argh[i+2] == "yoctometers"))
						{
							settings.sigma *= 0.00000000000001;
						}
						else { cout << "Syntax error - to specify sigma, use format: -e <sigma> [<unit>]" << endl; return 1; }
						i++;
					}
				}
				i++;
			}
			//Check for stepsize
			else if((argh[i] == "-z") || (argh[i] == "--stepsize"))
			{
				//Check parameter exists
				if((i+2) > num_args)
				{
					cout << "Syntax error - to specify stepsize, use format: -z <stepsize> [<unit>]" << endl;
					return 1;
				}
				else
				{
					//Check for next option (i.e. parameter missed out)
					if(argh[i+1][0] == '-')
					{
						cout << "Syntax error - to specify stepsize, use format: -z <stepsize> [<unit>]" << endl;
						return 1;
					}
					else
					{
						//Read stepsize
						settings.stepsize = atof(argh[i+1].c_str());
						//Check for input error
						if(settings.stepsize == 0) { cout << "If you put nothing in, you get nothing out... (check the stepsize :P)" << endl; return 1; }
						//Check for units!
						if(((i+3) <= num_args) && (argh[i+2][0] != '-'))
						{
							if((argh[i+2] == "ps") || (argh[i+2] == "picoseconds"))
							{
								settings.stepsize *= 1;
							}
							else if((argh[i+2] == "ns") || (argh[i+2] == "nanoseconds"))
							{
								settings.stepsize *= 1000;
							}
							else if((argh[i+2] == "us") || (argh[i+2] == "microseconds"))
							{
								settings.stepsize *= 1000000;
							}
							else if((argh[i+2] == "ms") || (argh[i+2] == "milliseconds"))
							{
								settings.stepsize *= 1000000000;
							}
							else if((argh[i+2] == "s") || (argh[i+2] == "seconds"))
							{
								settings.stepsize *= 1000000000000;
							}
							else if((argh[i+2] == "fs") || (argh[i+2] == "femtoseconds"))
							{
								settings.stepsize *= 0.001;
							}
							else if((argh[i+2] == "as") || (argh[i+2] == "attoseconds"))
							{
								settings.stepsize *= 0.000001;
							}
							else if((argh[i+2] == "zs") || (argh[i+2] == "zeptoseconds"))
							{
								settings.stepsize *= 0.000000001;
							}
							else if((argh[i+2] == "ys") || (argh[i+2] == "yoctoseconds"))
							{
								settings.stepsize *= 0.000000000001;
							}
							else { cout << "Syntax error - to specify stepsize, use format: -z <stepsize> [<unit>]" << endl; return 1; }
							i++;
						}
					}
				}
				i++;
			}
			else if(argh[i] == "--steps-per-output")
			{
				if((i+2) > num_args)
				{
					cout << "Syntax error - to specify number of steps per output to file,\n use format: -n <number of steps>" << endl;
					return 1;
				}
				else
				{
					if(argh[i+1][0] == '-')
					{
						cout << "Syntax error - to specify number of steps per output to file,\n use format: -n <number of steps>" << endl;
						return 1;
					}
					else
					{
						if(atoi(argh[i+1].c_str()) < 0) { cout << "Uhh, less than 0? That's impossible..." << endl; return 1; }
						settings.steps_per_output = atoi(argh[i+1].c_str());					
					}
				}
				i++;
				continue;
			}
			//[Patronising voice]If an item does not appear in our records, it does not exist. [/Patronising voice]
			else { cout << "Syntax error - option \"" << argh[i] << "\" not recognised.\n\n" << usage_text << endl; return 1; }
		}
	}
	
	//Do checking to see if program can run with current parameters:
	
	//First check input/output files (defaults may have been used)	
	if(settings.input_file_exists() == 1)
	{
		cout << "Error: input file \"" << settings.input_file_location << "\" could not be opened" << endl;
		return 1;
	}
	if(settings.output_file_exists() == 1)
	{
		cout << "Error: output file \"" << settings.output_file_location << "\" could not be opened" << endl;
		return 1;
	}
	
	return 0;
}

