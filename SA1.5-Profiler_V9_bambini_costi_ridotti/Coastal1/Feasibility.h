#ifndef FEASIBILITY
#define FEASIBILITY

#include "Passenger.h"
#include "Route.h"
#include "ProcessedInput.h"



bool route_feasible(ProcessedInput* input, Route& r, double end_day) {
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double2DVector location_fuel = input->get_location_fuel();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	bool feasible = true;

	// A index lower than 1 represents an error in some part of the code, thus the solution can not be 
	// feasible.
	if (r.index < 1)
		return false;
	
	//check and day
	if (r.get_arrivals()[r.index - 1] > end_day) {
		return false;
	}
	//for the last-trip
	if (r.fuel[r.index - 1] < (location_fuel[r.aircraft_code][r.get_airstrips()[r.index - 1]] + map_airplane[r.aircraft_code].min_fuel)) {
		return false;
	}

	if (feasible) {
		for (int i = 0; i < r.index; i++) {
			if (i >= 1) {
				if (r.fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					return false;
					
				}
				if (r.get_refueling()[i] && !r.get_refueling()[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.get_airstrips()[i - 1]][r.get_airstrips()[i]];
					if (r.fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						return false;
					}
				}
			}
			if (i < r.index - 1) {
				if (r.get_capacities()[i] > map_airplane[r.aircraft_code].capacity) {
					return false;
				}
			}

		}
	}

	//check of the intermediate stop
	for (auto& p : r.get_passengers()) {
		if (p.solution_to - p.solution_from > p.stop) {
			return false;
		}
	}

	return true;
}

#endif