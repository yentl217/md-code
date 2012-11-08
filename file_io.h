#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>
#include "atom.h"

using namespace std;

/*
File format for vmd:
START OF FILE! No empty lines can go here, the number of atoms must be first
[ N ] Number of atoms
[ molecule name ] name of molecule (can be blank)
atom1 x y z [optional data] atom name followed by xyz coords
atom2 x y z [ ...         ] and and (optionally) other data.
...                        
atomN x y z [ ...         ]
END OF FILE!
*/

//***************************************************************************//
//			Function protoypes:				     //
//***************************************************************************//

	//Input:

//Name:		read_input
//
//Description:	Reads the information about a group of atoms from a file.
//		If the file contains multiple states, the state required can
//		be specified.
//
//Inputs:	File location to check, entry from end of input file.
//
//Outputs:	array of atoms correctly initialised as specified in input
//		file and the length of that array.
//
//Returns:	1 on error, 0 on success.
//
//TODO:		TESTING!!! Make the state specification more elegant. Maybe
//		remove dependance on 'reader' function :P. Get the format
//		working with vmd.

bool read_input(atom *&atoms, int &num_atoms, const int entry_from_end, int &num_ions);
bool read_input(atom *&atoms, int &num_atoms, int &num_ions);	//Assumes last entry

	//Output:
//Name:		clear_file
//
//Description:	Deletes contents of the file specified.
//
//TODO:		Make this more reliable and able to detect more cases.

bool clear_file(const string file_location);

//Name:		output_state
//
//Description:	Outputs the state of the system to the file specified.
//
//TODO:		Get working with vmd.

bool output_state(atom *atoms, const int numatoms);	//Return status indicates error status: 1 = error, 0 = success.

	//Misc:
//Name:		file_exists
//
//Description:	Checks whether the specified file exists. At least, that's what
//		it's supposed to do. It actually checks whether it can open the
//		file. So obviously that needs solving.
//
//TODO:		See above :P.

bool file_exists(const string file_location);

//Name:		reader
//
//Description:	Used by the read_input function specified above. Takes an input
//		stream and reads until it reaches the specified delimiter. It
//		then converts the string into a numerical value.
//
//TODO:		Maybe this could simply be made part of the read_input
//		function? Seems a bit of a random/unnecessary thing to have.
//		Implement better type checking, although I'm not sure if that's
//		possible...

void reader(istream &input_stream, char delim, int &integer);
void reader(istream &input_stream, char delim, double &decimal);

#endif

