#pragma once
#ifndef CONSTRUCTIVE_HEURISTIC
#define CONSTRUCTIVE_HEURISTIC

#include <vector>
#include "Route.h"

class ConstructiveHeuristic
{
public:
	virtual ~ConstructiveHeuristic() = default;
	virtual std::vector<Route> Execute() = 0;
	static bool is_fuel_ok(map<int, Airplane>& map_airplane, Route& r, Passenger current_passenger, double fuel_consumed);
	static double evaluate_tw_departure(const Passenger& p, double time_arrival);
	static double evaluate_tw_arrival(const Passenger& p, const double& time_arrival);
};

#endif
