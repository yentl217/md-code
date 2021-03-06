#include <fstream>
#include "settings.h"
#include "vectron.h"

class prog_control;
extern prog_control settings;
extern double current_energy;

using namespace std;

//***************************************************************************//
//
#include <iostream>
#include <string>
#include <cstdlib>

#include "analysis.h"
#include "atom.h"
#include "file_io.h"			

//Function definitions:				     //
//***************************************************************************//

	//Input:
bool read_input(atom *&atoms, int &num_atoms, const int entry_from_end)
{
	ifstream input_stream;
	string str;
	string file_location = settings.get_input_file_location();
	
	//Check input file correctly opened
	input_stream.open(file_location.c_str(), ios::in | ios::binary);
	if(input_stream.is_open() == false)
	{
		cout << "Error: input file \"" << file_location << "\" could not be opened" << endl;
		return 1;
	}
	
	//Read number of atoms:
	reader(input_stream,'\n',num_atoms);
	cout << num_atoms << endl;

	//Skip title line
	char ch = 'h'; //better initialisation
	while (ch != '\n')
	{
		input_stream.get(ch);
	}

	//Create memory for the required number of atoms.
	atoms = new atom[num_atoms];
	
	//go to end of file
	input_stream.seekg(-1,ios::end);
	
	//Go to the last numerical entry (i.e. the end of one of the atom description lines)
	while( (ch < 48) || (ch > 57) )
	{
		input_stream.get(ch);
		input_stream.seekg(-2,ios::cur);
	}
	
	//Go back to the beginning of the atom description lines
	for(int i=0; i < (entry_from_end*(num_atoms+2)-2); i++)
	{
		while(ch != '\n')
		{
			input_stream.seekg(-2,ios::cur);
			input_stream.get(ch);
		}
		input_stream.seekg(-2,ios::cur);
		input_stream.get(ch);
	}
	for (int i = 0; i < num_atoms; i++)
	{
		//read initial values for atom i 
		getline(input_stream,str,' ');
		reader(input_stream,' ',(atoms[i].position).x);
		reader(input_stream,' ',(atoms[i].position).y);
		reader(input_stream,' ',(atoms[i].position).z);
		reader(input_stream,' ',(atoms[i].velocity).x);
		reader(input_stream,' ',(atoms[i].velocity).y);
		reader(input_stream,' ',(atoms[i].velocity).z);
		input_stream.get(ch);
		string str2 = (string)&ch;
		while((ch != '\n' && ch != ' '))
		{
			input_stream.get(ch);
			str2 += (string)&ch;
		}
		if (ch == '\n')
		{
			atoms[i].atomic_mass = stof(str2);
		}
		else
		{
			reader(input_stream,'\n',atoms[i].atomic_mass);
		}
		atoms[i].atom_id = i;
		//Output them to screen for verification if command line option enabled
		if(settings.is_print_initial_atom_data() == 1)
		{
			cout << "Atom " << i << ":" << endl;
			cout << "Position: (" << (atoms[i].position).x << "," << (atoms[i].position).y << "," << (atoms[i].position).z << ")" << endl;
			cout << "Velocity: (" << (atoms[i].velocity).x << "," << (atoms[i].velocity).y << "," << (atoms[i].velocity).z << ")" << endl;
			cout << "Charge:" << atoms[i].charge << endl;
			cout << "Mass: " << atoms[i].atomic_mass << endl;
			cout << "ID: " << atoms[i].atom_id << "\n\n";
		}
	}
	return 0;
}

bool read_input(atom *&atoms, int &num_atoms, const int entry_from_end, int &num_ions)
{
	ifstream input_stream;
	string str;
	num_ions = 0;
	string file_location = settings.get_input_file_location();
	
	//Check input file correctly opened
	input_stream.open(file_location.c_str(), ios::in | ios::binary);
	if(input_stream.is_open() == false)
	{
		cout << "Error: input file \"" << file_location << "\" could not be opened" << endl;
		return 1;
	}

	//Read number of atoms:
	reader(input_stream,'\n',num_atoms);

	//Skip title line
	char ch = 'h'; //better initialisation
	while (ch != '\n')
	{
		input_stream.get(ch);
	}

	//Create memory for the required number of atoms.
	atoms = new atom[num_atoms];
	
	//go to end of file
	input_stream.seekg(-1,ios::end);
	
	//Go to the last numerical entry (i.e. the end of one of the atom description lines)
	while( (ch < 48) || (ch > 57) )
	{
		input_stream.get(ch);
		input_stream.seekg(-2,ios::cur);
	}
	
	//Go back to the beginning of the atom description lines
	for(int i=0; i < (entry_from_end*(num_atoms+2)-2); i++)
	{
		while(ch != '\n')
		{
			input_stream.seekg(-2,ios::cur);
			input_stream.get(ch);
		}
		input_stream.seekg(-2,ios::cur);
		input_stream.get(ch);
	}
	
	for (int i = 0; i < num_atoms; i++)
	{
		//read initial values for atom i 
		getline(input_stream,str,' ');
		reader(input_stream,' ',(atoms[i].position).x);
		reader(input_stream,' ',(atoms[i].position).y);
		reader(input_stream,' ',(atoms[i].position).z);
		reader(input_stream,' ',(atoms[i].velocity).x);
		reader(input_stream,' ',(atoms[i].velocity).y);
		reader(input_stream,' ',(atoms[i].velocity).z);
		reader(input_stream,' ',atoms[i].charge);
		reader(input_stream,' ',atoms[i].atomic_mass);
		
		//Output them to screen for verification if command line option enabled
		if(settings.is_print_initial_atom_data() == 1)
		{
			cout << "Atom " << i << ":" << endl;
			cout << "Position: (" << (atoms[i].position).x << "," << (atoms[i].position).y << "," << (atoms[i].position).z << ")" << endl;
			cout << "Velocity: (" << (atoms[i].velocity).x << "," << (atoms[i].velocity).y << "," << (atoms[i].velocity).z << ")" << endl;
			cout << "Charge:" << atoms[i].charge << endl;
			cout << "Mass: " << atoms[i].atomic_mass << endl;
			cout << "ID: " << atoms[i].atom_id << "\n\n";
		}
	}
	
	//Count number of ions present
	for(int i=0; i < num_atoms; i++) if(atoms[i].charge != 0) num_ions++;
	
	return 0;
}	

bool read_input(atom *&atoms, int &num_atoms)
{
	read_input(atoms, num_atoms, 1);
	return 0;
}

bool read_input(atom *&atoms, int &num_atoms, int &num_ions)
{
	read_input(atoms, num_atoms, 1, num_ions);
	return 0;
}

	//Output:
bool clear_file(const string file_location)
{
	//Clears file by opening it in the default way since the default way is to truncate it.
	ofstream killfile(file_location.c_str());
	if(killfile.is_open() == false)
	{
		cout << "Error: the file \"" << file_location << "\" could not be cleared." << endl;	//Does this work??
		return 1;
	}
	else
	{
		killfile.close();
		return 0;
	}
}

/*READ BEFORE MODIFYING THE FOLLOWING FUNCTION!!!

So as to keep a record of the format in use, please update the file format 
comment at the beginning of file_io.h whenever editing this function.
*/

bool output_state(atom *atoms, const int numatoms)
{
	string output_file_location = settings.get_output_file_location();
	//Open file etc.
	ofstream mystream;
	mystream.open(output_file_location.c_str(), ios::app | ios::ate);
	
	if(mystream.is_open() == false)
	{
		cout << "Error: output file \"" << output_file_location << "\" could not be opened." << endl;
		return 1;
	}
	
	//Add title line and specify the number of atoms
	mystream << numatoms << endl;
	mystream << "Total energy: " << current_energy << " eV" << endl;
	
	//Output information about each atom - SEE ABOVE BEFORE MODIFYING!!!
	for(int i=0; i < numatoms; i++)
	{
		mystream << "atom" << i << " "
			 << (atoms[i].position).x << " "
			 << (atoms[i].position).y << " "
			 << (atoms[i].position).z << " "
			 << (atoms[i].velocity).x << " "
			 << (atoms[i].velocity).y << " "
			 << (atoms[i].velocity).z << " "
			 << atoms[i].charge << " "
			 << atoms[i].atomic_mass
			 << endl;
	}
	
	return 0;
}

	//Misc:
bool file_exists(const string file_location)
{
	//Pretty self-explanatory :P.
	ifstream mystream(file_location.c_str());
	if(mystream.is_open() == false)	return 1;
	else return 0;
}

void reader(istream &input_stream, char delim, int &integer)
{
	string str;
	//Get a string from the input stream - stop at delimiter specified.
	getline(input_stream,str,delim);
	integer = stoi(str);
	//apparent loss of accuracy due to printing error with cout; you'll realise this doesn't exist if you use the outputted values to do 
	//some arithmetic
}

void reader(istream &input_stream, char delim, double &decimal)
{
	string str;
	
	//Floating point version of above function.
	getline(input_stream,str,delim);
	decimal = stof(str); //apparent loss of accuracy due to printing error with cout; you'll realise this doesn't exist if you use the outputted values to do 
	//some arithmetic
}
