#ifndef ATOM_H
#define ATOM_H

#include "vectron.h"

//This header file contains the definition of the atom class and associated functions

class atom
{
	public:
	//Variables:
	int atom_id;					//Used to uniquely identify each atom in the system
	double atomic_mass;				//Mass of atom in number of proton masses
	vector position, velocity, acceleration;	//Lengths in Angstroms, time in picoseconds
	int charge;
	
	//Constructors:
	//Neutral atom
	atom(int a, double b, double pa, double pb, double pc, double va, double vb, double vc);
	//Ion (misnomer)
	atom(int a, double b, double pa, double pb, double pc, double va, double vb, double vc, double c);
	//Null constructor
	atom(); 
};

#endif

