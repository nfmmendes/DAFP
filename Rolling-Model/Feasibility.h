#ifndef FEASIBILITY_H
#define FEASIBILITY_H

#include "Passenger.h"
#include "Route.h"

bool route_feasible(Route& r, map<int, Airplane>& map_airplane, double end_day, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool feasible = true;


	//check and day
	if (r.time_arr[r.index - 1] > end_day) {
		return false;
		feasible = false;
	}
	//for the last-trip
	if (r.quantity_fuel[r.index - 1] < (location_fuel[r.aircraft_code][r.places[r.index - 1]] + map_airplane[r.aircraft_code].min_fuel)) {
		return false;
		feasible = false;
	}

	if (feasible) {
		for (int i = 0; i < r.index; i++) {
			if (i >= 1) {
				//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					return false;
					feasible = false;
				}
				if (r.refueling[i] && !r.refueling[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
					if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						return false;
						feasible = false;
					}
				}
			}
			if (i < r.index - 1) {
				if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
					return false;
					feasible = false;
				}
			}

		}
	}

	if (feasible) {
		//check of the intermediate stop
		for (Passenger& p : r.passengers_in_route) {
			if (p.solution_to - p.solution_from > p.stop) {
				return false;
				feasible = false;
			}
		}
	}







	return feasible;
}

#endif
