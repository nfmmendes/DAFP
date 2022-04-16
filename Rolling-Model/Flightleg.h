#ifndef FLIGHTLEG_H
#define FLIGHTLEG_H

#include "Passenger.h"
#include "Route.h"
#include <vector>
using namespace std;
class Flightleg
{
public:
	Flightleg();
	~Flightleg();

	int cod_airplane;
	int from;
	int to;
	double time_arr_from;
	double time_arr_to;
	double time_dep_from;
	double time_dep_to;
	bool refuel_from;
	bool refuel_to;
	double quantity_fuel_from;
	double quantity_fuel_to;
	double cost;
	bool flight_leg_fittizzio;
	
	double fuel_necessario_per_essere_fatta;
	double fuel_pescabile;
	vector<Passenger> passengers_in_flightleg;
	Route route_padre;
	//Flightleg(Route r);
	void print();
	void print_route_padre();

};

#endif // Route_h