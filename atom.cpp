#include "atom.h"
#include "vectron.h"
using namespace std;

atom::atom(int a, double b, double pa, double pb, double pc, double va, double vb, double vc) //initialise public variables of atom object 
{
	atom_id = a;
	atomic_mass = b;
	charge = 0;
	position.set_vector(pa,pb,pc);
	velocity.set_vector(va,vb,vc); 
}

atom::atom(int a, double b, double pa, double pb, double pc, double va, double vb, double vc, int c) //initialise public variables of atom object 
{
	atom_id = a;
	atomic_mass = b;
	charge = c;
	position.set_vector(pa,pb,pc);
	velocity.set_vector(va,vb,vc); 
}

atom::atom()
{
	atom_id = 0;
	atomic_mass = 0.0;
	charge = 0;
	position.set_vector();
	velocity.set_vector(); 
}
	
