#include "ConstructiveHeuristicFirstFase.h"
#include <iostream>


ConstructiveHeuristicFirstFase::ConstructiveHeuristicFirstFase(Penalty_Weights _pen,
																Fleet& _airplanes, 
																Input_maps _maps, 
																double _end_day,
																vector<Passenger>& _passengers,
																int _number_of_aircraft, 
																vector<vector<double>>& _from_to, 
																vector<vector<double>>& _location_fuel, 
																Double3DVector& _from_to_fuelConsumed):
																airplanes(_airplanes),
																end_day(_end_day),
																passengers(_passengers),
																number_of_aircraft(_number_of_aircraft),
																from_to(_from_to),
																location_fuel(_location_fuel),
																from_to_FuelConsumed(_from_to_fuelConsumed){

	map_airplane = _maps.map_airplane;
	map_airstrip = _maps.map_airstrip;
	peso_TW = penalties.time_window_weight;
	peso_intermediate_stop = penalties.stop_weight;
	
}

//TODO: Merge this function with insert_place_removing_passenger
void ConstructiveHeuristicFirstFase::insert_place_with_new_passenger(const Passenger &p, int route_index)
{
	Route* route = &solution[route_index];
	int index = route->index - 1;
	Airplane *aircraft = &map_airplane[route->aircraft_code];
	Airstrip *departure_location = &map_airstrip[p.departure_location];
	double timeToGo = ((from_to[route->places[index]][p.departure_location] / map_airplane[route->aircraft_code].speed) * 60);
	double previousDepartureTime = route->time_dep[index];
	double weight_fuel_people = aircraft->weight_fuel_people;
	double arrivalTime = previousDepartureTime + timeToGo;
	
	
	if (map_airstrip[p.departure_location].fuel) {
		route->addPlace(p.departure_location, departure_location->fuel, aircraft->max_fuel, 0.0, p.capacity,
						arrivalTime, previousDepartureTime + timeToGo + departure_location->ground_time);

		route->weight[index] = weight_fuel_people - aircraft->max_fuel - p.weight;
	}
	else {
		double fuel_before = route->quantity_fuel[index];
		double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][route->places[index]][p.departure_location];
		double departure_time = previousDepartureTime + timeToGo + departure_location->ground_time;
		
		route->addPlace(p.departure_location, departure_location->fuel, fuel_before - fuel_consumed, 0.0, p.capacity, 
						arrivalTime, departure_time);
		route->weight[index] = route->weight[index - 1] - p.weight + fuel_consumed;	
	}
}

void ConstructiveHeuristicFirstFase::insert_place_removing_passenger(const Passenger &p, int best_route, double time)
{
	Route* route = &solution[best_route];
	const int index = route->index - 1;
	const Airstrip* arrival_location = &map_airstrip[p.arrival_location];
	const Airplane* aircraft = &map_airplane[solution[best_route].aircraft_code];
	double departure_time = time + arrival_location->ground_time;
	
	if (map_airstrip[p.arrival_location].fuel) {
		route->addPlace(p.arrival_location, arrival_location->fuel, aircraft->max_fuel, 0.0, 0, time, departure_time);
		route->weight[index] = aircraft->weight_fuel_people - aircraft->max_fuel;
	}
	else {
		const double fuel_before = route->quantity_fuel[index];
		const double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][route->places[index]][p.arrival_location];
		route->addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, departure_time);
		route->weight[solution[best_route].index - 1] = route->weight[index - 1] + fuel_consumed + p.weight;
	}
}

void ConstructiveHeuristicFirstFase::insert_place_with_new_passenger_(const Passenger &p, int best_route)
{
	Route * route = &solution[best_route]; 
	Airplane *airplane = &map_airplane[solution[best_route].aircraft_code];
	Airstrip* departure_location = &map_airstrip[p.departure_location];
	auto index = solution[best_route].index - 1;
	double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][route->places[index]][p.departure_location];
	const double departure_time = p.early_departure + departure_location->ground_time;
	
	
	if (map_airstrip[p.departure_location].fuel) {
		route->addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, airplane->max_fuel, 0.0, 1, p.early_departure, departure_time);
		route->weight[index] = airplane->weight_fuel_people - airplane->max_fuel - p.weight;
	}
	else {
		const double fuel_before = route->quantity_fuel[index];
		route->addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, departure_time );
		route->weight[index] = route->weight[index - 1] - p.weight + fuel_consumed;
	}
}

void ConstructiveHeuristicFirstFase::do_work_4(Passenger p, vector<Route>::value_type r, int from, int to, bool& capacity_satisfy)
{
	for (int c = from; c < to; c++) {
		capacity_satisfy = (r.capacity[c] + p.capacity <= map_airplane[r.aircraft_code].capacity);

		//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
		double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
		double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

		//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAA
		if (c == to - 1)
			fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

		if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0)
			capacity_satisfy = false;

	}
}

void ConstructiveHeuristicFirstFase::do_work_5(Passenger& p, int best_route, int& index)
{
	Route* route = &solution[best_route];
	Airplane* airplane = &map_airplane[route->aircraft_code];
	route->primo_pass = true;
	index = route->index - 1;
	//time
	route->time_dep[index] = p.early_departure + map_airstrip[p.departure_location].ground_time;
	route->time_arr[index] = p.early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

	p.solution_from = index;
	route->capacity[index] += p.capacity;

	//for the weight
	//here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
	route->weight[index] = airplane->weight_fuel_people - route->quantity_fuel[index] - p.weight;  
	if (route->weight[index] < 0) {
		route->quantity_fuel[index] += route->weight[index];
		route->weight[index] = airplane->weight_fuel_people - route->quantity_fuel[index] - p.weight;
	}
}

void ConstructiveHeuristicFirstFase::do_work_6(Passenger& p, int best_route, int index)
{
	double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
	double time_from = solution[best_route].time_dep[index];
	double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

	if (map_airstrip[p.arrival_location].fuel) {
		solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
		solution[best_route].weight[index] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
	}
	else {
		double fuel_before = solution[best_route].quantity_fuel[index];
		solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
		//for the weight
		solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[index - 1] + fuel_consumed + p.weight;
		//end for weight

	}
	p.solution_to = index;
	solution[best_route].passengers_in_route.push_back(p);
}

void ConstructiveHeuristicFirstFase::update_solution_considering_fuel(Passenger& p, int best_route, double time)
{
	if (map_airstrip[p.arrival_location].fuel) {
		solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

		solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
			map_airplane[solution[best_route].aircraft_code].max_fuel;
	}
	else {
		double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
		double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
		//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

		solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
		solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

	}
	p.solution_to = solution[best_route].index - 1;
	solution[best_route].passengers_in_route.push_back(p);
}

void ConstructiveHeuristicFirstFase::create_connection_on_solution(Passenger & p, int best_route)
{
	
	double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60);
	p.solution_from = solution[best_route].index - 1;
	insert_place_removing_passenger(p, best_route, time);


	p.solution_to = solution[best_route].index - 1;
	solution[best_route].passengers_in_route.push_back(p);
}

void ConstructiveHeuristicFirstFase::deal_with_situation_4(Passenger& p, int best_route, int best_from, int best_to)
{
	for (int h = best_from; h < best_to; h++) {
		solution[best_route].capacity[h] += p.capacity;
		solution[best_route].weight[h] -= p.weight;
	}

	//**************************************************************************
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = best_from; j < solution[best_route].index; j++) {
		if (solution[best_route].weight[j] < 0) {

			add_fuel = solution[best_route].weight[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (solution[best_route].refueling[i]) {
					index_refueling = i;
					break;
				}
			}

			for (int t = index_refueling; t < solution[best_route].index; t++) {
				if (solution[best_route].refueling[t] && t != index_refueling)
					break;
				solution[best_route].quantity_fuel[t] += add_fuel;
				solution[best_route].weight[t] -= add_fuel;
			}
		}
	}

	p.solution_from = best_from;
	p.solution_to = best_to;
	solution[best_route].passengers_in_route.push_back(p);
}

void ConstructiveHeuristicFirstFase::do_work_10(Passenger& p, int best_route, int best_from)
{
	p.solution_from = best_from;
	int place_1 = solution[best_route].places[solution[best_route].index - 1];
	double aircraft_speed = map_airplane[solution[best_route].aircraft_code].speed;
	double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

	if (map_airstrip[p.arrival_location].fuel) {
		solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
		solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
			map_airplane[solution[best_route].aircraft_code].max_fuel;

	}
	else {
		double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
		double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];

		solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
		solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

	}

	p.solution_to = solution[best_route].index - 1;
	solution[best_route].passengers_in_route.push_back(p);
}

void ConstructiveHeuristicFirstFase::do_work_11(int best_route, double add_fuel, int index_weight_neg, int j)
{
	add_fuel = solution[best_route].weight[j];
	index_weight_neg = j;
	int index_refueling = index_weight_neg;
	for (int i = index_weight_neg; i >= 0; i--) {
		if (solution[best_route].refueling[i]) {
			index_refueling = i;
			break;
		}
	}
	//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
	for (int t = index_refueling; t < solution[best_route].index; t++) {
		if (solution[best_route].refueling[t] && t != index_refueling)
			break;

		solution[best_route].quantity_fuel[t] += add_fuel;
		solution[best_route].weight[t] -= add_fuel;

	}
}

void ConstructiveHeuristicFirstFase::do_work_12(Passenger& p, int best_route)
{
	Airplane airplane = map_airplane[solution[best_route].aircraft_code];
	auto index = solution[best_route].index - 1;
	double time_to_go = (from_to[solution[best_route].places[index]][p.departure_location] / airplane.speed) * 60;
		solution[best_route].primo_pass = true;
	solution[best_route].time_arr[index] = p.early_departure - time_to_go - map_airstrip[solution[best_route].places[index]].ground_time;
	solution[best_route].time_dep[index] = p.early_departure - time_to_go;
	solution[best_route].weight[index] = airplane.weight_fuel_people - solution[best_route].quantity_fuel[index];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
}

void ConstructiveHeuristicFirstFase::do_work_13(Passenger p, vector<Route>::value_type r, double& cost, double& t_arr_arrival)
{
	//guardo il costo di aggiungere un pezzo alla fine
	//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
	double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
	cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];


	double time_1 = (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
	double time_2 = (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
	double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + time_1;
	t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + time_2;
	double TW_departure = ConstructiveHeuristic::evaluate_tw_departure(p, t_arr_departure);

	double TW_arrival = ConstructiveHeuristic::evaluate_tw_arrival(p, t_arr_arrival);
	cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
}

void ConstructiveHeuristicFirstFase::do_work_14(vector<Passenger>::value_type p, vector<Route>::value_type r, double& f_after_primo_tratto, double& f_after_secondo_tratto)
{
	//per il check sul fuel:
	//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
	double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

	//****
	//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
	double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

	f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
	f_after_secondo_tratto = (map_airstrip[p.departure_location].fuel ? map_airplane[r.aircraft_code].max_fuel :
								  r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto) - fuel_consumed_secondo_tratto;
}

void ConstructiveHeuristicFirstFase::do_work_15(Passenger p, vector<Route>::value_type r, vector<int>::value_type from, double& cost, double& fuel_consumed, bool& fuel_ok)
{
	double TW_departure = ConstructiveHeuristic::evaluate_tw_departure(p, r.time_arr[from]);

	cost = (TW_departure * peso_TW);
	// ora ci metto chilometri e fuel dell'ultimo pezzo;
	cost += from_to[r.places[r.index - 1]][p.arrival_location];
	fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];

	cost += fuel_consumed;

	fuel_ok = true;
	if (!r.refueling[r.index - 1]) {
		double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
		if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
	}
}

void ConstructiveHeuristicFirstFase::do_work_16(vector<Passenger>::value_type p, int& situation, int& best_route, double& best_cost, int i, vector<Route>::value_type r)
{
	//in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
	double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
	//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
	double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
	cost += fuel_consumed;
	//per il check sul fuel:
	double fuel_after_trip = 0.0;
	fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
	if (best_cost > cost) {
		if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
			best_cost = cost;
			best_route = i;
			situation = 1;
		}
	}
}

void ConstructiveHeuristicFirstFase::do_work_17(vector<Passenger>::value_type p, vector<Route>::value_type r, double& cost, double& f_after_primo_tratto, double& f_after_secondo_tratto)
{
	cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
	//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
	double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

	cost += fuel_consumed;

	//per il check sul fuel
	//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
	double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

	//****
	//double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
	double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

	f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
	f_after_secondo_tratto = (map_airstrip[p.departure_location].fuel ?
								  map_airplane[r.aircraft_code].max_fuel : r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto)
		- fuel_consumed_secondo_tratto;
}

void ConstructiveHeuristicFirstFase::deal_with_situation_2(Passenger& p, int best_route)
{
	do_work_12(p, best_route);
	insert_place_with_new_passenger_(p, best_route);
	p.solution_from = solution[best_route].index - 1;
	double time_to_go= (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60; 
	double time = p.early_departure + map_airstrip[p.departure_location].ground_time + time_to_go ;
	update_solution_considering_fuel(p, best_route, time);
}

void ConstructiveHeuristicFirstFase::deal_with_situation_3(vector<Passenger>::value_type p, int best_route)
{
	insert_place_with_new_passenger(p, best_route);
	create_connection_on_solution(p, best_route);
}

void ConstructiveHeuristicFirstFase::deal_with_situation_5(vector<Route> solution, vector<Passenger>::value_type p, int best_route, int best_from)
{
	for (int h = best_from; h < solution[best_route].index; h++) {
		solution[best_route].capacity[h] += p.capacity;
		solution[best_route].weight[h] -= p.weight;
	}

	//******************************************************************************
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = best_from; j < solution[best_route].index; j++) {
		if (solution[best_route].weight[j] < 0) 
			do_work_11(best_route, add_fuel, index_weight_neg, j);
	}
	//******************************************************************************

	do_work_10(p, best_route, best_from);
}

void ConstructiveHeuristicFirstFase::deal_with_situation_1(vector<Passenger>::value_type p, int best_route)
{
	int index;
	do_work_5(p, best_route, index);
	do_work_6(p, best_route, index);
}

void ConstructiveHeuristicFirstFase::do_work_1(vector<Passenger>::value_type p, int& situation, int& best_route, double& best_cost, int i, vector<Route>::value_type r)
{
	if (r.places[r.index - 1] == p.departure_location) {
		do_work_16(p, situation, best_route, best_cost, i, r);
	}
	else {
		double cost;
		double f_after_primo_tratto;
		double f_after_secondo_tratto;
		do_work_17(p, r, cost, f_after_primo_tratto, f_after_secondo_tratto);

		if (best_cost > cost) {
			if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
				if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
					best_cost = cost;
					best_route = i;
					situation = 2;
				}
			}
		}
	}
}

void ConstructiveHeuristicFirstFase::do_work_2(vector<Passenger>::value_type p, int& situation, int& best_route, double& best_cost, int i, vector<Route>::value_type r)
{
	double cost;
	double t_arr_arrival;
	do_work_13(p, r, cost, t_arr_arrival);

	double f_after_primo_tratto;
	double f_after_secondo_tratto;
	do_work_14(p, r, f_after_primo_tratto, f_after_secondo_tratto);

	if (best_cost > cost) {
		if (t_arr_arrival <= end_day) {
			if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
				if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
					best_cost = cost;
					best_route = i;
					situation = 3;
				}
			}
		}
	}
}

void ConstructiveHeuristicFirstFase::choose_situation(vector<Passenger>::value_type p, int& situation, int& best_route, int& best_from, int& best_to)
{
	situation = -1;
	best_route = -1;
	best_from = -1;
	best_to = -1;
	double best_cost = DBL_MAX;
	int i = 0;

	//qui calcolo solo dove mettere il passeggero
	for (auto r : solution) {
		if (r.primo_pass == false) {
			do_work_1(p, situation, best_route, best_cost, i, r);
		}
		else {
			//nella route c'? almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit? della route
			//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'? il FROM, non cerco il TO
			vector<int> FROM;
			vector<int> TO;
			for (int t = 0; t < r.index; t++) {
				if (r.places[t] == p.departure_location)
					FROM.push_back(t);
			}

			if (FROM.size() != 0) {
				//dentro questo abbiamo trovato dei FROM 
				for (int t = FROM[0]; t < r.index; t++)
					if (r.places[t] == p.arrival_location)
						TO.push_back(t);


				if (TO.size() != 0) {
					for (auto from : FROM) {
						for (auto to : TO) {
							if (from < to && to - from <= p.stop) {

								//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
								//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
								bool capacity_satisfy = true;
								do_work_4(p, r, from, to, capacity_satisfy);
								//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
								if (!capacity_satisfy) break; //check also for the fuel;

								//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider? per l'aggiunta del passeggero qui
								//? la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

								double TW_departure = 0.0;
								//double t_arr_departure = r.time_arr[from];
								if (r.time_arr[from] < p.early_departure) 
									TW_departure = p.early_departure - r.time_arr[from];
								else if (r.time_arr[from] > p.late_departure) 
									TW_departure = r.time_arr[from] - p.late_departure;

								double TW_arrival = 0.0;
								//double t_arr_arrival = r.time_arr[to];
								if (r.time_arr[to] < p.early_arrival)
									TW_arrival = p.early_arrival - r.time_arr[to];
								else if (r.time_arr[to] > p.late_arrival)
									TW_arrival = r.time_arr[to] - p.late_arrival;

								double cost = ((TW_departure + TW_arrival) * peso_TW + ((to - from - 1) * peso_intermediate_stop)) * p.capacity;
								if (best_cost > cost) {
									best_cost = cost;
									best_route = i;
									situation = 4;
									best_from = from;
									best_to = to;
								}
							}
						}
					}
				}
				//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
				//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
				if (r.places[r.index - 1] != p.arrival_location) {
					for (auto from : FROM) {
						bool capacity_satisfy = true;
						for (int c = from; c < r.index; c++) {
							if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

							if (c < r.index - 1) {
								//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
								double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

								double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

								if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
							}
						}
						//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
						if (capacity_satisfy && (r.index - from) <= p.stop) {

							double cost;
							double fuel_consumed;
							bool fuel_ok;
							do_work_15(p, r, from, cost, fuel_consumed, fuel_ok);


							double travel_time = (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
							double t_arr_arrival = r.time_arr[r.index - 1] + travel_time + map_airstrip[r.places[r.index - 1]].ground_time;
							double TW_arrival = ConstructiveHeuristic::evaluate_tw_arrival(p, t_arr_arrival);
							cost += (peso_intermediate_stop * (r.index - from - 1)) * p.capacity;

							if (fuel_ok) {
								if (best_cost > cost) {
									if (t_arr_arrival <= end_day) {
										if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
											best_cost = cost;
											best_route = i;
											best_from = from;
											situation = 5;
										}

									}
								}
							}
						}
					}
				}
			}
			if (r.places[r.index - 1] != p.departure_location) {
				do_work_2(p, situation, best_route, best_cost, i, r);

			}
		}
		i++;
	}
}

void ConstructiveHeuristicFirstFase::ugly_do_while()
{
	int cont = 0;
	//TODO: Replace it by a for
	do {
		Route r(airplanes[cont].code);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
}

vector<Route> ConstructiveHeuristicFirstFase::Execute()
{
	//creo i punti di partenza
	ugly_do_while();

	for (auto p : passengers) {

		int situation;
		int best_route;
		int best_from;
		int best_to;
		choose_situation(p, situation, best_route, best_from, best_to);

		switch(situation){
			case 1:
				deal_with_situation_1(p, best_route);
				break;
			case 2:
				deal_with_situation_2(p, best_route);
				break; 
			case 3:
				deal_with_situation_3(p, best_route);
				break; 
			case 4:
				deal_with_situation_4(p, best_route, best_from, best_to);
				break; 
			case 5:
				deal_with_situation_5(solution, p, best_route, best_from);
				break; 
			default:
				std::cout << "**************il passeggero: " << p.name << " non trova sistemazione" << endl;
		}
		
	}


	vector<Route> solution_clean;
	for (auto s : solution) {
		if (s.index != 1) solution_clean.push_back(s);
	}

	return solution_clean;
}

