#include "Flightleg.h"
#include <iostream>
using namespace std;


Flightleg::Flightleg()
{
	cod_airplane = 0;
	from = 0;
	to = 0;
	time_arr_from = 0.0;
	time_arr_to = 0.0;
	time_dep_from = 0.0;
	time_dep_to = 0.0;
	refuel_from = false;
	refuel_to = false;
	quantity_fuel_from = 0.0;
	quantity_fuel_to = 0.0;
	cost = 0.0;
	flight_leg_fittizzio = false;

	/*
	NB:
	il fuel necessario per essere fatta è il fuel che la route prcedente gli dovrebbe garantire
	il fuel pescabile è il massimo fuel che la route può dare alla route successiva
	*/
	fuel_necessario_per_essere_fatta = 0.0;
	fuel_pescabile;
}


/*Flightleg::Flightleg(Route r)
{
	cod_airplane = r.aircraft_code;
	from = r.places[0];
	to = r.places[r.index - 1];
	time_arr_from = r.time_arr[0];
	time_arr_to = r.time_arr[r.index - 1];
	time_dep_from = r.time_dep[0];
	time_dep_to = r.time_dep[r.index - 1];
	refuel_from = r.refueling[0];
	refuel_to = r.refueling[r.index - 1];
	quantity_fuel_from = r.quantity_fuel[0];
	quantity_fuel_to = r.quantity_fuel[r.index - 1];
	cost = r.cost;
}
*/
void Flightleg::print() {
	cout << " il costo del fligth leg ------- > " << cost << endl;
	cout << "l'aereo di riferimento e': " << cod_airplane << endl;
	cout << " p: " << from << " ---------->    " << to << endl;
	//cout << " t_a: " << time_arr_from << " ---------->    " << time_arr_to << endl;
	//cout << " t_d: " << time_dep_from << " ---------->    " << time_dep_to << endl;
	//cout << " r: " << refuel_from << " ---------->    " << refuel_to << endl;
	//cout << " q_f: " << quantity_fuel_from << " ---------->    " << quantity_fuel_to << endl;
	cout << "ecco i passeggeri nel flight leg----> " << passengers_in_flightleg.size() << endl;
	/*for (Passenger& p : passengers_in_flightleg) {
		p.print();
	}*/
}


void Flightleg::print_route_padre() {
	cout << "la route da cui deriva questo flightleg e': " << endl;
	route_padre.print();

}



Flightleg::~Flightleg()
{
}
