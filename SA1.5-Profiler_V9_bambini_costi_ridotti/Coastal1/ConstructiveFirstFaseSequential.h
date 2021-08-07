#pragma once

#ifndef  CONSTRUCTIVE_FIRST_FASE_SEQUENTIAL_H
#define CONSTRUCTIVE_FIRST_FASE_SEQUENTIAL_H

#include "Util.h"
#include "ConstructiveHeuristic.h"



struct Penalty_Weights;

class ConstructiveFirstFaseSequential : public ConstructiveHeuristic 
{
public:
	ConstructiveFirstFaseSequential(const Penalty_Weights& penalties, Fleet& airplanes, const Input_maps& maps, double end_day, vector<Passenger> passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed);
	virtual vector<Route> Execute() override;
private:
	const Penalty_Weights penalties;
	const Fleet airplanes;
	const Input_maps maps;
	const double end_day;
	vector<Passenger> passengers;
	const int number_of_aircraft;
	const vector<vector<double>> from_to;
	const vector<vector<double>> location_fuel;
	Double3DVector from_to_FuelConsumed;

	double get_travel_time(Passenger passenger, Airplane& aircraft);
	void evaluate_weight(Route& r, int best_passenger);
	void do_work_1(Route& r, int best_passenger);
};


#endif