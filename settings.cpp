#include <iostream>
#include <cstdlib>

#include "file_io.h"
#include "settings.h"

using namespace std;

extern prog_control settings;

//***************************************************************************//
//			Function definitions:				     //
//***************************************************************************//

//Accessor function definitions
//Files

char *prog_control::get_input_file_location() { return input_file_location; }

bool prog_control::input_file_exists()
{
	input_file_exist = file_exists(settings.input_file_location);
	return input_file_exist;
}

bool prog_control::is_print_initial_atom_data() { return print_initial_atom_data; }

char *prog_control::get_output_file_location() { return output_file_location; }

bool prog_control::output_file_exists()
{
	output_file_exist = file_exists(settings.output_file_location);
	return output_file_exist;
}

int prog_control::get_steps_per_output() { return steps_per_output; }

//Type of run

bool prog_control::is_damped_md() { return damped_md; }

bool prog_control::is_dummy_run() { return dummy_run; }

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
		exit;
	}
	cout << " - Stepsize:\t\t\t" << stepsize << " ps" << endl;
	cout << " - Number of steps:\t\t" << num_steps << endl;
	cout << "Input/Output settings:" << endl;
	cout << " - Input file:\t\t\t" << settings.input_file_location << endl;
	cout << " - Output file: \t\t" << settings.output_file_location << endl;
	cout << " - Steps per output:\t\t"<< settings.steps_per_output << endl;

	cout << endl;
}

bool command_line_argh(const int num_args, char **argh)
{
	//Usage text:
	char usage_text[] = "Usage: mmm [-d|--damped-md] [--dummy] [-e|--epsilon <epsilon> [<unit>]]\n\t   [-h|--help [<command>]] [-i|--input-file <input file>]\n\t   [-n|--num-steps <number of steps>] [-o|--output-file <output file>]\n\t   [-p|--periodic-boundaries] [--print-initial-atom-data]\n\t   [-s|--integration-method] [--sigma <sigma> [<units>]]\n\t   [-z|--stepsize <stepsize> [<units>]] [--steps-per-output]";
	
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
			if((strcmp(argh[i],"-d") == 0) || (strcmp(argh[i],"--damped-md") == 0))
			{
				settings.damped_md = 1;
				continue;
			}
			//Check for dummy option (print settings and terminate)
			if(strcmp(argh[i],"--dummy") == 0)
			{
				settings.dummy_run = 1;
				continue;
			}
			//Check for epsilon value
			else if((strcmp(argh[i],"-e") == 0) || (strcmp(argh[i],"--epsilon") == 0))
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify epsilon, use format: -e <epsilon> [<unit>]" << endl;
					return 1;
				}
				else
				{
					settings.epsilon = atof(argh[i+1]);
					if(((i+3) <= num_args) && (argh[i+2][0] != '-'))
					{
						if((strcmp(argh[i+2],"MeV") == 0) || (strcmp(argh[i+2],"Mev") == 0))
						{
							settings.epsilon *= 9648533360;
						}
						else if((strcmp(argh[i+2],"keV") == 0) || (strcmp(argh[i+2],"kev") == 0))
						{
							settings.epsilon *= 9648533.36;
						}
						else if((strcmp(argh[i+2],"eV") == 0) || (strcmp(argh[i+2],"ev") == 0))
						{
							settings.epsilon *= 9648.53336;
						}
						else if((strcmp(argh[i+2],"meV") == 0) || (strcmp(argh[i+2],"mev") == 0))
						{
							settings.epsilon *= 9.64853336;
						}
						else if((strcmp(argh[i+2],"J") == 0) || (strcmp(argh[i+2],"j") == 0) || (strcmp(argh[i+2],"Joules") == 0) || (strcmp(argh[i+2],"joules") == 0))
						{
							settings.epsilon *= 60221415000000000000000.0;
						}
						else if((strcmp(argh[i+2],"mJ") == 0) || (strcmp(argh[i+2],"mj") == 0) || (strcmp(argh[i+2],"milliJoules") == 0) || (strcmp(argh[i+2],"millijoules") == 0))
						{
							settings.epsilon *= 60221415000000000000.0;
						}
						else if((strcmp(argh[i+2],"uJ") == 0) || (strcmp(argh[i+2],"uj") == 0) || (strcmp(argh[i+2],"microJoules") == 0) || (strcmp(argh[i+2],"microjoules") == 0))
						{
							settings.epsilon *= 60221415000000000.0;
						}
						else if((strcmp(argh[i+2],"nJ") == 0) || (strcmp(argh[i+2],"nj") == 0) || (strcmp(argh[i+2],"nanoJoules") == 0) || (strcmp(argh[i+2],"nanojoules") == 0))
						{
							settings.epsilon *= 60221415000000.0;
						}
						else if((strcmp(argh[i+2],"pJ") == 0) || (strcmp(argh[i+2],"pj") == 0) || (strcmp(argh[i+2],"picoJoules") == 0) || (strcmp(argh[i+2],"picojoules") == 0))
						{
							settings.epsilon *= 60221415000.0;
						}
						else if((strcmp(argh[i+2],"fJ") == 0) || (strcmp(argh[i+2],"fj") == 0) || (strcmp(argh[i+2],"femtoJoules") == 0) || (strcmp(argh[i+2],"femtojoules") == 0))
						{
							settings.epsilon *= 60221415;
						}
						else if((strcmp(argh[i+2],"aJ") == 0) || (strcmp(argh[i+2],"aj") == 0) || (strcmp(argh[i+2],"attoJoules") == 0) || (strcmp(argh[i+2],"attojoules") == 0))
						{
							settings.epsilon *= 60221.415;
						}
						else if((strcmp(argh[i+2],"zJ") == 0) || (strcmp(argh[i+2],"zj") == 0) || (strcmp(argh[i+2],"zeptoJoules") == 0) || (strcmp(argh[i+2],"zeptojoules") == 0))
						{
							settings.epsilon *= 60.221415;
						}
						else if((strcmp(argh[i+2],"yJ") == 0) || (strcmp(argh[i+2],"yj") == 0) || (strcmp(argh[i+2],"yoctoJoules") == 0) || (strcmp(argh[i+2],"yoctojoules") == 0))
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
			else if((strcmp(argh[i],"-h") == 0) || (strcmp(argh[i],"--help") == 0))
			{
				//Explain commands in more detail
				cout << usage_text << endl << endl;
				//TODO: Check for additional commands - e.g. "--help --epsilon"
				return 1;
			}
			//Check for input file location
			else if((strcmp(argh[i],"-i") == 0) || (strcmp(argh[i],"--input-file") == 0))
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
			else if((strcmp(argh[i],"-n") == 0) || (strcmp(argh[i],"--num-steps") == 0))
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
						if(atoi(argh[i+1]) < 1) { cout << "I'm sorry, Dave. I'm afraid I can't do that." << endl; return 1; }
						settings.num_steps = atoi(argh[i+1]);
						if(settings.num_steps < 3) cout << "Use a calculator, lazy!" << endl;
					
					}
				}
				i++;
				continue;
			}
			//Check for output file location
			else if((strcmp(argh[i],"-o") == 0) || (strcmp(argh[i],"--output-file") == 0))
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
			else if((strcmp(argh[i],"-p") == 0) || (strcmp(argh[i],"--periodic-boundaries") == 0))
			{
				settings.pbc = 1;
				continue;
			}
			//check whether to output atom input information
			else if(strcmp(argh[i],"--print-initial-atom-data") == 0)
			{
				settings.print_initial_atom_data = 1;
				continue;
			}
			//check for integration method
			else if((strcmp(argh[i],"-s") == 0) || (strcmp(argh[i],"--integration-method") == 0))
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify integration method, use format: --integration-method <method>" << endl;
					return 1;
				}
				else
				{
					char *temp_string = new char [strlen(argh[i+1])+1];
					strcpy(temp_string,argh[i+1]);
					for(int i=0; i < strlen(temp_string); i++) temp_string[i] = tolower(temp_string[i]);
					cout << temp_string << endl;
					
					if(strcmp(temp_string,"euler") == 0)
					{
						settings.integration_method = prog_control::euler;
					}
					else if(strcmp(temp_string,"velocity-verlet") == 0)
					{
						settings.integration_method = prog_control::velocity_verlet;
					}
					else if((strcmp(temp_string,"runge-kutta-4") == 0) || (strcmp(temp_string,"rk4") == 0) || (strcmp(temp_string,"rk-4") == 0) || (strcmp(temp_string,"r-k-4") == 0))
					{
						settings.integration_method = prog_control::runge_kutta_4;
					}
					else
					{
						cout << "Syntax error - No valid integration method specified. Available integration\nmethods are velocity verlet (type 'verlet'), Runge-Kutta 4 (type 'rk4') and\nEuler integration (type 'euler').\nDo not use Euler unless risk of death is involved." << endl;
						return 1;
					}
					
					delete [] temp_string;
				}
				i++;
				continue;
			}
			//Check for sigma
			else if(strcmp(argh[i],"--sigma") == 0)
			{
				if((i+2) > num_args)
				{ 
					cout << "Syntax error - to specify sigma, use format: --sigma <sigma> [<unit>]" << endl;
					return 1;
				}
				else
				{
					settings.sigma = atof(argh[i+1]);
					if(settings.sigma <= 0)
					{
						cout << "Error: sigma cannot be less than or equal to 0." << endl;
						return 1;
					}
					if(((i+3) <= num_args) && (argh[i+2][0] != '-'))
					{
						if((strcmp(argh[i+2],"A") == 0) || (strcmp(argh[i+2],"Angstroms") == 0) || (strcmp(argh[i+2],"angstroms") == 0))
						{
							settings.sigma *= 1;
						}
						else if((strcmp(argh[i+2],"nm") == 0) || (strcmp(argh[i+2],"nanometers") == 0))
						{
							settings.sigma *= 10;
						}
						else if((strcmp(argh[i+2],"um") == 0) || (strcmp(argh[i+2],"micrometers") == 0))
						{
							settings.sigma *= 10000;
						}
						else if((strcmp(argh[i+2],"mm") == 0) || (strcmp(argh[i+2],"millimeters") == 0))
						{
							settings.sigma *= 10000000;
						}
						else if((strcmp(argh[i+2],"cm") == 0) || (strcmp(argh[i+2],"centimeters") == 0))
						{
							settings.sigma *= 100000000;
						}
						else if((strcmp(argh[i+2],"m") == 0) || (strcmp(argh[i+2],"meters") == 0))
						{
							settings.sigma *= 10000000000;
						}
						else if((strcmp(argh[i+2],"pm") == 0) || (strcmp(argh[i+2],"picometers") == 0))
						{
							settings.sigma *= 0.01;
						}
						else if((strcmp(argh[i+2],"fm") == 0) || (strcmp(argh[i+2],"femtometers") == 0))
						{
							settings.sigma *= 0.00001;
						}
						else if((strcmp(argh[i+2],"am") == 0) || (strcmp(argh[i+2],"attometers") == 0))
						{
							settings.sigma *= 0.00000001;
						}
						else if((strcmp(argh[i+2],"zm") == 0) || (strcmp(argh[i+2],"zeptometers") == 0))
						{
							settings.sigma *= 0.00000000001;
						}
						else if((strcmp(argh[i+2],"ym") == 0) || (strcmp(argh[i+2],"yoctometers") == 0))
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
			else if((strcmp(argh[i],"-z") == 0) || (strcmp(argh[i],"--stepsize") == 0))
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
						settings.stepsize = atof(argh[i+1]);
						//Check for input error
						if(settings.stepsize == 0) { cout << "If you put nothing in, you get nothing out... (check the stepsize :P)" << endl; return 1; }
						//Check for units!
						if(((i+3) <= num_args) && (argh[i+2][0] != '-'))
						{
							if((strcmp(argh[i+2],"ps") == 0) || (strcmp(argh[i+2],"picoseconds") == 0))
							{
								settings.stepsize *= 1;
							}
							else if((strcmp(argh[i+2],"ns") == 0) || (strcmp(argh[i+2],"nanoseconds") == 0))
							{
								settings.stepsize *= 1000;
							}
							else if((strcmp(argh[i+2],"us") == 0) || (strcmp(argh[i+2],"microseconds") == 0))
							{
								settings.stepsize *= 1000000;
							}
							else if((strcmp(argh[i+2],"ms") == 0) || (strcmp(argh[i+2],"milliseconds") == 0))
							{
								settings.stepsize *= 1000000000;
							}
							else if((strcmp(argh[i+2],"s") == 0) || (strcmp(argh[i+2],"seconds") == 0))
							{
								settings.stepsize *= 1000000000000;
							}
							else if((strcmp(argh[i+2],"fs") == 0) || (strcmp(argh[i+2],"femtoseconds") == 0))
							{
								settings.stepsize *= 0.001;
							}
							else if((strcmp(argh[i+2],"as") == 0) || (strcmp(argh[i+2],"attoseconds") == 0))
							{
								settings.stepsize *= 0.000001;
							}
							else if((strcmp(argh[i+2],"zs") == 0) || (strcmp(argh[i+2],"zeptoseconds") == 0))
							{
								settings.stepsize *= 0.000000001;
							}
							else if((strcmp(argh[i+2],"ys") == 0) || (strcmp(argh[i+2],"yoctoseconds") == 0))
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
			else if(strcmp(argh[i],"--steps-per-output") == 0)
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
						if(atoi(argh[i+1]) < 0) { cout << "Uhh, less than 0? That's impossible..." << endl; return 1; }
						settings.steps_per_output = atoi(argh[i+1]);					
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

