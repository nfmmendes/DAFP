﻿#ifndef PROCESSED_INPUT_H
#define PROCESSED_INPUT_H
#include <map>


#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Util.h"

struct PenaltyWeights
{
	double intermediate_stop;  //valore che si da ad una sosta intermedia 
	double transhipment;  //valore che si da ad un transhipment
	double time_window; //peso dato ad un minuto di Time Windows
};


class ProcessedInput
{
public:
	ProcessedInput(std::map <int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double2DVector& from_to, double2DVector& location_fuel, double3DVector& fuel_consumption);

	map<int, Airplane>& get_map_airplane();
	const map<int, Airstrip>& get_map_airstrip() const;
	const double2DVector& get_from_to() const; 
	const double2DVector& get_location_fuel() const;
	const double3DVector& get_from_to_fuel_consumed() const;

	void set_map_airplane(const map<int, Airplane>& p_map_airplane);
	void set_map_airstrip(const map<int, Airstrip>& p_map_airstrip);
	void set_from_to(const double2DVector& p_from_to);
	void set_location_fuel(const double2DVector& p_location_fuel);
	void set_from_to_fuel_consumed(const double3DVector& p_from_to_fuel_consumed);
	
private:
	map<int, Airplane> map_airplane;
	map<int, Airstrip> map_airstrip;
	double2DVector from_to;
	double2DVector location_fuel;
	double3DVector from_to_fuel_consumed;
};

#endif
