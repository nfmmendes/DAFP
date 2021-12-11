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
	
	//check and day
	if (r.arrival[r.index - 1] > end_day) {
		return false;
		feasible = false;
	}
	//for the last-trip
	if (r.fuel[r.index - 1] < (location_fuel[r.aircraft_code][r.airstrips[r.index - 1]] + map_airplane[r.aircraft_code].min_fuel)) {
		return false;
		feasible = false;
	}

	if (feasible) {
		for (int i = 0; i < r.index; i++) {
			if (i >= 1) {
				//if (r.fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
				if (r.fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					return false;
					feasible = false;
				}
				if (r.get_refueling()[i] && !r.get_refueling()[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.airstrips[i - 1]][r.airstrips[i]];
					if (r.fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						return false;
						feasible = false;
					}
				}
			}
			if (i < r.index - 1) {
				if (r.get_capacities()[i] > map_airplane[r.aircraft_code].capacity) {
					return false;
					feasible = false;
				}
			}

		}
	}

	if (feasible) {
		//check of the intermediate stop
		for (auto& p : r.get_passengers()) {
			if (p.solution_to - p.solution_from > p.stop) {
				return false;
				feasible = false;
			}
		}
	}

	return feasible;
}

#endif