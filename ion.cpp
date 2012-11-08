#include "ion.h"
#include "atom.h"
#include "vectron.h"
using namespace std;

ion::ion(int a, double b, double pa, double pb, double pc, double va, double vb, double vc, int c) //initialise public variables of atom object 
{
	atom_id = a;
	atomic_mass = b;
	position.set_vector(pa,pb,pc);
	velocity.set_vector(va,vb,vc); 
	charge = c;
}

ion::ion()
{
	atom_id = 0;
	atomic_mass = 0.0;
	position.set_vector();
	velocity.set_vector();
	charge = 0; 
}


