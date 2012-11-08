#ifndef ION_H
#ifndef ION_H 

#include "atom.h"
#include "vectron.h"
using namespace std;

class ion : public atom
{
	public:
		int charge; //in electron charge
		ion(int a, double b, double pa, double pb, double pc, double va, double vb, double vc, int c);
		ion();
}

#endif
	
