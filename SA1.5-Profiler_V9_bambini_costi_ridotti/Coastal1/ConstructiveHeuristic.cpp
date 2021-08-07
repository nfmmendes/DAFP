#include "ConstructiveHeuristic.h"


bool ConstructiveHeuristic::is_fuel_ok(map<int, Airplane>& map_airplane, Route& r, Passenger current_passenger, double fuel_consumed)
{
	bool fuel_ok = true;
	if (!r.refueling[r.index - 1]) {
		double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
		if (r.weight[r.index - 1] - current_passenger.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0)
			fuel_ok = false;
	}

	return fuel_ok;
}


double ConstructiveHeuristic::evaluate_tw_departure(const Passenger& p, double time_arrival)
{

	if (time_arrival < p.early_departure)
		return p.early_departure - time_arrival;

	if (time_arrival > p.late_departure)
		return  time_arrival - p.late_departure;

	return 0;
}


double ConstructiveHeuristic::evaluate_tw_arrival(const Passenger& p, const double& time_arrival)
{
	if (time_arrival < p.early_arrival)
		return p.early_arrival - time_arrival;
	else if (time_arrival > p.late_arrival)
		return time_arrival - p.late_arrival;

	return 0;
}