#include <cmath>
#include <iostream>

#include "thermostat.h"
#include "atom.h"
#include "vectron.h"

using namespace std;

void gaussian_thermostat(double actual_temp, double expected_temp, vector &velocity)
{
	double scaling_factor = sqrt(expected_temp/actual_temp);
	velocity.x *= scaling_factor;
	velocity.y *= scaling_factor;
	velocity.z *= scaling_factor;
}		

	
