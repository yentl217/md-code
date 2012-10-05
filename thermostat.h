#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "atom.h"

//TODO: Choice of thermostats
void gaussian_thermostat(double actual_temp, double expected_temp, vector &velocity);

#endif
