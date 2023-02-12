#include "Route.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "ProcessedInput.h"
using namespace std;


Route::Route()
{
	aircraft_code_company_comparison = "";
	aircraft_code = 0;
	index = 0;
	primo_pass = false;
	cost = 0.0;
}


Route::Route(int aircraft_code, const vector<Passenger>& pass)
{
	this->aircraft_code = aircraft_code;
	index = 0;
	aircraft_code_company_comparison = "";

	//weights;
	//capacities;
	primo_pass = false;

	passengers_in_route = pass;
	cost = 0.0;
}

Passenger& Route::get_passenger(int position)
{
	return passengers_in_route[position];
}

void Route::add_passenger(const Passenger& p)
{
	passengers_in_route.push_back(p);
}

void Route::erase_passenger(int position)
{
	passengers_in_route.erase(passengers_in_route.begin() + position);
}

void Route::print() {
	cout << "-" << to_string(aircraft_code) << "ha passeggeri: " << primo_pass << endl;
	cout << "\t";
	for (int t = 0; t < index; t++) {
		cout << " ->" << to_string(airstrips[t]) << "***\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " t_a(" << arrival[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " t_d(" << departure[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " q_f(" << fuel[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " c_p(" << capacities[z] << "***)\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " w_p(" << weights[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " s_f(" << refueling[z] << "***)\t";
	}
	cout << endl;
	cout << "Ha i seguenti passeggeri: " << endl;
	for (auto p : passengers_in_route) {
		p.print();
	}

	cout << endl;
}

void Route::printCompany() {
	cout << "-" << aircraft_code_company_comparison << endl;
	cout << "\t";
	for (int t = 0; t < index; t++) {
		cout << " ->" << places_company[t] << "***\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " t_a(" << arrival[z] << ")\t";
	}
	cout << endl;
}

void Route::addPlace_companySolution(string place, int minute) {
	places_company.push_back(place);
	arrival.push_back(minute);
	index++;
}

vector<string> Route::set_places_company_at(int position, string value)
{
	if (position >= 0 && position < static_cast<int>(places_company.size())) places_company[position] = value;
}

void Route::addPlace(int place, bool has_refueling, AirplaneStatus airplane_status, double minute_arr, double minute_dep) {
	airstrips.push_back(place);
	arrival.push_back(minute_arr);
	departure.push_back(minute_dep);
	refueling.push_back(has_refueling);
	fuel.push_back(airplane_status.fuel);
	weights.push_back(airplane_status.weight);
	capacities.push_back(airplane_status.capacity);
	
	index++;
}

void Route::eraseAt(int i)
{
	airstrips.erase(airstrips.begin() + i);
	arrival.erase(arrival.begin() + i);
	departure.erase(departure.begin() + i);
	refueling.erase(refueling.begin() + i);
	fuel.erase(fuel.begin() + i);
	weights.erase(weights.begin() + i);
	capacities.erase(capacities.begin() + i);
	index = index - 1;
}

void Route::removePlace(int position, map<int, Airplane>& map_airplane) {

	eraseAt(position);

	int cont = 0;
	for (int i = 0; i < index; i++) {
		if (capacities[i] == 0) 
			cont++;
	}
	
	if (cont == index) {
		primo_pass = false;
	}

	int index_sup = index;
	for (int i = index_sup - 1; i > 1; i--) {
		if (capacities[i - 1] != 0)
			break;

		if (capacities[i - 1] == 0) 
			eraseAt(i);
	}

	if (index == 2 && capacities[0] == 0) 
		eraseAt(1);

	Airplane* airplane = &map_airplane[aircraft_code]; 
	if (airstrips.size() == 1 && capacities[0] == 0) {
		fuel[0] = airplane->max_fuel;
		weights[0] = airplane->load_weight - airplane->max_fuel;
	}

}

void Route::update_from_to(int node_destroy, int sense)
{
	for (Passenger& p : passengers_in_route) {
		if (p.solution_from > node_destroy) 
			p.solution_from+= sense;
		if (p.solution_to > node_destroy) 
			p.solution_to += sense;
	}
}


void Route::update_solution_from_to(int& node_add_from)
{
	for (Passenger& p : passengers_in_route) {
		if (p.solution_from > node_add_from)
			p.solution_from++;
		if (p.solution_to > node_add_from)
			p.solution_to++;
	}
}

void Route::update_route_destroy(ProcessedInput* input, int node_destroy, int min_from_pass, int max_to_pass) {

	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	Airplane* airplane = & map<int, Airplane>(input->get_map_airplane())[aircraft_code];
	
	arrival[node_destroy + 1] = departure[node_destroy - 1] + airplane->travelTime(from_to[airstrips[node_destroy - 1]][airstrips[node_destroy + 1]]);
	departure[node_destroy + 1] = arrival[node_destroy + 1] + map_airstrip[airstrips[node_destroy + 1]].ground_time;

	for (int i = node_destroy + 1; i < index - 1; i++) {
		arrival[i + 1] = departure[i] + airplane->travelTime(from_to[airstrips[i]][airstrips[i + 1]]);
		departure[i + 1] = arrival[i + 1] + map_airstrip[airstrips[i + 1]].ground_time;
	}

	update_from_to(node_destroy, -1);
	
	int index_min_from = 0;
	for (int i = min_from_pass; i >= 0; i--) {
		if (refueling[i] && i != node_destroy) {
			index_min_from = i;
			break;
		}
	}

	for (int k = index_min_from; k < max_to_pass; k++) {
		if (refueling[k] && fuel[k] < airplane->max_fuel && k != node_destroy) { //&& k!= node_destroy
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weights[k];
			for (int i = k + 1; i <= max_to_pass; i++) {
				if (refueling[i]) 
					break;
				if (weights[i] < min_weight && i != node_destroy) {
					min_weight = weights[i];
					Node_min = i;
				}
			}
			if (Node_min >= 0) {
				for (int i = k + 1; i < index; i++) {
					if (refueling[i]) { 
						index_updating_to = i;
						break;
					}
				}

				double Fuel_before = fuel[index_updating_from];

				fuel[index_updating_from] = min(airplane->max_fuel, fuel[index_updating_from] + min_weight);
				weights[index_updating_from] -= (fuel[index_updating_from] - Fuel_before);
				for (int i = index_updating_from + 1; i < index_updating_to; i++) {
					if (i != node_destroy) {
						fuel[i] += (fuel[index_updating_from] - Fuel_before);
						weights[i] -= (fuel[index_updating_from] - Fuel_before);
					}
				}
			}
		}
	}
}

void Route::do_work_3(int node_add_to)
{	
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = node_add_to; j < index; j++) {
		if (weights[j] < 0) {
			add_fuel = weights[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (refueling[i]) {
					index_refueling = i;
					break;
				}
			}
			for (int t = index_refueling; t < index; t++) {
				if (refueling[t] && t != index_refueling) 
					break;
				
				fuel[t] += add_fuel;
				weights[t] -= add_fuel;
			}
		}
	}
}

void Route::update_fuel_and_weight(const int& node_add_to, const bool& non_to, double add_fuel, int index_weight_neg)
{
	int index_refueling = index_weight_neg;
	for (int i = index_weight_neg; i >= 0; i--) {
		if (i != node_add_to || !non_to) {
			if (refueling[i]) {
				index_refueling = i;
				break;
			}
		}
	}

	for (int t = index_refueling; t < index; t++) {
		if (t != node_add_to || !non_to) {
			if (refueling[t] && t != index_refueling)
				break;
			fuel[t] += add_fuel;
			weights[t] -= add_fuel;
		}
	}
}

void Route::update_route_rebuilt_one(ProcessedInput* input, int node_add_from, int node_add_to, int location_from, int location_to, Passenger& p) {
	Airplane* airplane = & input->get_map_airplane()[aircraft_code];
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();

	bool non_to = false;
	bool non_to_final = false;
	bool num_equals = false;

	num_equals |= node_add_from == node_add_to;
	non_to_final |= (node_add_to == index - 1);

	if (airstrips[node_add_from] == location_from) {
		//il nodo prima al from � uguale devo quindi aggiornare le capacit� e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != airstrips[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (airstrips[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			
			if (location_to != airstrips[node_support_to + 1] && (location_to != airstrips[node_support_to] || num_equals)) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		do_work_3(node_add_from);
		
	}
	else if (airstrips[node_add_from + 1] == location_from) {
		//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != airstrips[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (airstrips[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) 
					non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) 
					non_to_final = true;
			}
			if (num_equals) { //&& location_to == airstrips[node_support_to] // ho cambiato qua
				node_add_to = node_add_to + 2;
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					non_to = true;
				}
				else if (location_to != airstrips[node_support_to + 2]) {
					non_to = true;
				}
			}
		}

		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		do_work_3(node_add_from + 1);
	}
	else {
		airstrips.insert(airstrips.begin() + node_add_from + 1, location_from);
		node_add_to++;
		p.solution_from = node_add_from + 1;

		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != airstrips[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (airstrips[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			//TODO: Reduce it to if(!A && (!B || C)) ??
			if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != airstrips[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}

		double time_add = departure[node_add_from] + airplane->travelTime(from_to[airstrips[node_add_from]][location_from]);
		arrival.insert(arrival.begin() + node_add_from + 1, time_add);
		departure.insert(departure.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = airplane->travelTime(from_to[airstrips[node_add_from]][location_from]);
			+ airplane->travelTime(from_to[location_from][airstrips[node_add_from + 2]])
			+ map_airstrip[location_from].ground_time
			- airplane->travelTime(from_to[airstrips[node_add_from]][airstrips[node_add_from + 2]]);

		index++;

		for (int i = node_add_from + 2; i < index; i++) {
			arrival[i] += time;
			departure[i] += time;
		}
		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);

		//update passengers in route
		update_from_to(node_add_to);

		fuel.insert(fuel.begin() + node_add_from + 1, 0.0);

		weights.insert(weights.begin() + node_add_from + 1, 0.0);
		capacities.insert(capacities.begin() + node_add_from + 1, 0);

		//update of fuel, capacities and weights
		capacities[node_add_from + 1] = capacities[node_add_from];
		weights[node_add_from + 1] = weights[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		//fuel consumed
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[node_add_from]][airstrips[node_add_from + 1]];

		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[node_add_from + 1]) {
			fuel[node_add_from + 1] = airplane->max_fuel;
			weights[node_add_from + 1] = weights[node_add_from + 1] - (airplane->max_fuel - fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[node_add_from + 1]) {
			fuel[node_add_from + 1] = fuel[node_add_from] - fuel_consumed;
			weights[node_add_from + 1] = weights[node_add_from + 1] + fuel_consumed;
		}

		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_from + 1]][airstrips[node_add_from + 2]];
		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_from]][airstrips[node_add_from + 2]];

		if (!refueling[node_add_from + 1]) {
			for (int i = node_add_from + 2; i < index; i++) {
				if (refueling[i])
					break; 
				
				fuel[i] = fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
				weights[i] = weights[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
			}
		}
		else {
			//rebuilt_case bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = node_add_from + 2; i < index; i++) {
				if (refueling[i])
					break;
				
				fuel[i] = fuel[i] - fuel_consumed1 + fuel_consumed2 + (airplane->max_fuel - fuel[node_add_from]);
				weights[i] = weights[i] + fuel_consumed1 - fuel_consumed2 - (airplane->max_fuel - fuel[node_add_from]);
			}
		}

		do_work_3(node_add_from + 1);
	}

	if (non_to) {

		airstrips.insert(airstrips.begin() + node_add_to, location_to);
		p.solution_to = node_add_to;
		double time_add1 = departure[node_add_to - 1] + airplane->travelTime(from_to[airstrips[node_add_to - 1]][location_to]);
		arrival.insert(arrival.begin() + node_add_to, time_add1);
		departure.insert(departure.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);
		double time1 = 0.0;
		if (!non_to_final) {
			time1 = airplane->travelTime(from_to[airstrips[node_add_to - 1]][location_to])
				+ airplane->travelTime(from_to[location_to][airstrips[node_add_to + 1]])
				+ map_airstrip[location_to].ground_time
				- airplane->travelTime(from_to[airstrips[node_add_to - 1]][airstrips[node_add_to + 1]]);
		}

		index++;
		for (int i = node_add_to + 1; i < index; i++) {
			arrival[i] += time1;
			departure[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		//update passengers in route
		update_from_to(node_add_to);

		fuel.insert(fuel.begin() + node_add_to, 0.0);

		double weight_add = 0.0;

		weights.insert(weights.begin() + node_add_to, weight_add);
		capacities.insert(capacities.begin() + node_add_to, 0);

		/////////////////////////////////////////
		//fuel consumed
		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_to - 1]][airstrips[node_add_to]];
		if (refueling[node_add_to])
			fuel[node_add_to] = (refueling[node_add_to]) ? airplane->max_fuel : fuel[node_add_to - 1] - fuel_consumed3;

		//******************************************************************controlla bene*************
		if (!non_to_final) {
			capacities[node_add_to] = capacities[node_add_to - 1] - p.capacity;
			weights[node_add_to] = weights[node_add_to - 1] + p.weight;
			weights[node_add_to] += refueling[node_add_to] ? (fuel[node_add_to - 1] - airplane->max_fuel) : fuel_consumed3;
		}

		double fuel_consumed4 = 0.0;
		double fuel_consumed5 = 0.0;

		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_to]][airstrips[node_add_to + 1]];
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_to - 1]][airstrips[node_add_to + 1]];
		}

		//qua devo considerare il fottuto rebuilt_case bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (refueling[i])
					break; 
				
				fuel[i] = fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
				weights[i] = weights[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (refueling[i])
					break;
				
				fuel[i] = fuel[i] - fuel_consumed4 + fuel_consumed5 + (airplane->max_fuel - fuel[node_add_to - 1]);
				weights[i] = weights[i] + fuel_consumed4 - fuel_consumed5 - (airplane->max_fuel - fuel[node_add_to - 1]);
			}
		}

		if (non_to_final) {
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[node_add_to - 1]][airstrips[node_add_to]];
			double reduction = (map_airstrip[location_to].fuel) ? airplane->max_fuel : (fuel[node_add_to - 1] - fuel_consumed);
			weights[node_add_to] += airplane->load_weight - reduction;
		}

		do_work_3(node_add_to);
	}
	else {
		p.solution_to = node_add_to;
	}

	for (int k = 0; k < index; k++) {
		if (refueling[k] && fuel[k] < airplane->max_fuel) {
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1

			double min_weight = weights[k];
			for (int i = k + 1; i < index; i++) {
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weights[i] < min_weight) {
					min_weight = weights[i];
				}
			}
			
			double Fuel_before = fuel[index_updating_from];
			fuel[index_updating_from] = min(airplane->max_fuel, fuel[index_updating_from] + min_weight);
			weights[index_updating_from] -= (fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				fuel[i] += (fuel[index_updating_from] - Fuel_before);
				weights[i] -= (fuel[index_updating_from] - Fuel_before);
			}
		}
	}
}

void Route::add_update_only_one_node_first_passanger(ProcessedInput* input, Passenger& p) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();

	Airplane* airplane = &map_airplane[aircraft_code];
	
	if (airstrips[0] == p.origin) {
		arrival[0] = p.early_departure;
		departure[0] = p.early_departure + map_airstrip[p.origin].ground_time;
		capacities[0] = p.capacity;
		fuel[0] = airplane->max_fuel;
		weights[0] = airplane->load_weight - fuel[0] - p.weight;

		double time = map_airplane[aircraft_code].travelTime(from_to[p.origin][p.destination]);
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.origin][p.destination];

		AirplaneStatus airplane_status{ 0.0, 0.0 , 0 };
		double minute_arrival = departure[0] + time;
		double minute_departure = minute_arrival + map_airstrip[p.destination].ground_time;
		addPlace(p.destination, map_airstrip[p.destination].fuel, airplane_status, minute_arrival, minute_departure);
		if (refueling[1] == true) {
			fuel[1] = airplane->max_fuel;
			weights[1] = airplane->load_weight - fuel[1];
		}
		else {
			fuel[1] = fuel[0] - fuel_consumed;
			weights[1] = map_airplane[aircraft_code].load_weight - fuel[1];
		}

		p.solution_from = 0;
		p.solution_to = 1;
	}
	else {

		arrival[index - 1] = p.early_departure - map_airplane[aircraft_code].travelTime(from_to[airstrips[index - 1]][p.origin]) -
			map_airstrip[airstrips[index - 1]].ground_time;
		departure[index - 1] = p.early_departure - map_airplane[aircraft_code].travelTime(from_to[airstrips[index - 1]][p.origin]);

		weights[index - 1] = map_airplane[aircraft_code].load_weight - fuel[index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel

		double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[index - 1]][p.origin];
		if (map_airstrip[p.origin].fuel) {
			AirplaneStatus airplane_status{ map_airplane[aircraft_code].max_fuel, 0.0, p.capacity };
			addPlace(p.origin, map_airstrip[p.origin].fuel, airplane_status, p.early_departure, p.early_departure + map_airstrip[p.origin].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight -
				map_airplane[aircraft_code].max_fuel - p.weight;
		}
		else {
			double fuel_before = fuel[index - 1];
			AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, p.capacity  };
			addPlace(p.origin, map_airstrip[p.origin].fuel, airplane_status, p.early_departure, p.early_departure + map_airstrip[p.origin].ground_time);
			weights[index - 1] = weights[index - 2] - p.weight + fuel_consumed;
		}
		p.solution_from = index - 1;
		double time = p.early_departure + map_airstrip[p.origin].ground_time + map_airplane[aircraft_code].travelTime(from_to[p.origin][p.destination]);

		if (map_airstrip[p.destination].fuel) {
			AirplaneStatus airplane_status{ map_airplane[aircraft_code].max_fuel, 0.0, 0 };
			addPlace(p.destination, map_airstrip[p.destination].fuel, airplane_status, time, time + map_airstrip[p.destination].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight -
				map_airplane[aircraft_code].max_fuel;
		}
		else {
			double fuel_before = fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[index - 1]][p.destination];

			AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, 0 };
			addPlace(p.destination, map_airstrip[p.destination].fuel, airplane_status, time, time + map_airstrip[p.destination].ground_time);
			weights[index - 1] = weights[index - 2] + p.weight + fuel_consumed;
		}
		p.solution_to = index - 1;
	}
}

void Route::move_c(ProcessedInput* input, Passenger& p, int location_from, int location_to) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();

	Airplane* airplane = &map_airplane[aircraft_code];
	
	if (airstrips[index - 1] == location_from) {
		capacities[index - 1] += p.capacity;
		weights[index - 1] -= p.weight;

		double time_arri = departure[index - 1] + airplane->travelTime(from_to[location_from][location_to]);
		double time_depa = time_arri + map_airstrip[location_to].ground_time;
		p.solution_from = index - 1;
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_from][location_to];

		if (map_airstrip[location_to].fuel) {
			AirplaneStatus airplane_status{ map_airplane[aircraft_code].max_fuel, (airplane->load_weight - airplane->max_fuel), 0 };
			addPlace(location_to, map_airstrip[location_to].fuel, airplane_status , time_arri, time_depa);
		}
		else {
			AirplaneStatus airplane_status{ fuel[index - 1] - fuel_consumed, (airplane->load_weight - (fuel[index - 1] - fuel_consumed)), 0 };
			addPlace(location_to, map_airstrip[location_to].fuel, airplane_status, time_arri, time_depa);
		}

		p.solution_to = index - 1;
	}
	else {

		if (map_airstrip[p.origin].fuel) {
			double arrival_time = departure[index - 1] + airplane->travelTime(from_to[airstrips[index - 1]][p.origin]);
			AirplaneStatus airplane_status{ airplane->max_fuel, 0.0, p.capacity };
			addPlace(p.origin, map_airstrip[p.origin].fuel, airplane_status,
				arrival_time,
				arrival_time + map_airstrip[p.origin].ground_time);

			weights[index - 1] = airplane->load_weight - airplane->max_fuel - p.weight;
		}
		else {
			double fuel_before = fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[index - 1]][p.origin];
			AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, p.capacity };
			addPlace(p.origin, map_airstrip[p.origin].fuel, airplane_status , departure[index - 1] +
				airplane->travelTime(from_to[airstrips[index - 1]][p.origin]),
				departure[index - 1] + airplane->travelTime(from_to[airstrips[index - 1]][p.origin]) + map_airstrip[p.origin].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight - fuel[index - 1] - p.weight;

		}
		p.solution_from = index - 1;
		double time = departure[index - 1] + airplane->travelTime(from_to[p.origin][p.destination]);

		if (map_airstrip[p.destination].fuel) {
			AirplaneStatus airplane_status{ airplane->max_fuel, 0.0, 0 }; 
			addPlace(p.destination, map_airstrip[p.destination].fuel, airplane_status, time, time + map_airstrip[p.destination].ground_time);
			weights[index - 1] = airplane->load_weight - airplane->max_fuel;

		}
		else {
			double fuel_before = fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[index - 1]][p.destination];

			AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, 0 };
			addPlace(p.destination, map_airstrip[p.destination].fuel,airplane_status , time, time + map_airstrip[p.destination].ground_time);
			weights[index - 1] = weights[index - 2] + fuel_consumed + p.weight;
		}

		p.solution_to = index - 1;
	}
}

void Route::update_time_for_check_repair(ProcessedInput* input, int node_add_from, int node_add_to, int location_from, int location_to) {

	Airplane* airplane = &map<int, Airplane>(input->get_map_airplane())[aircraft_code];
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	bool non_to = false;
	bool non_to_final = false;
	bool num_equals = false;

	num_equals |= (node_add_from == node_add_to);
	non_to_final |= (node_add_to == index - 1);

	if (airstrips[node_add_from] == location_from) {
		//il nodo prima al from � uguale devo quindi aggiornare le capacit� e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != airstrips[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (airstrips[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != airstrips[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
	}
	else if (airstrips[node_add_from + 1] == location_from) {
		//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != airstrips[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (airstrips[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				non_to_final |= (node_add_to >= index); // ho cambiato qua forse la condizione dopo la puoi commentare
			    non_to_final |= (num_equals && node_add_to >= index);
			}
			if (num_equals) { //&& location_to == airstrips[node_support_to] // ho cambiato qua
				node_add_to = node_add_to + 2;
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					non_to = true;
				}
				else if (location_to != airstrips[node_support_to + 2]) 
						node_add_to = node_add_to + 2;
			}
		}
	}
	else {
		airstrips.insert(airstrips.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != airstrips[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (airstrips[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != airstrips[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}

		const double travel_distance = airplane->travelTime(from_to[airstrips[node_add_from]][location_from]);
		arrival.insert(arrival.begin() + node_add_from + 1, departure[node_add_from] + travel_distance);
		departure.insert(departure.begin() + node_add_from + 1, departure[node_add_from] + travel_distance + map_airstrip[location_from].ground_time);

		double time = airplane->travelTime(from_to[airstrips[node_add_from]][location_from]);
			+ airplane->travelTime(from_to[location_from][airstrips[node_add_from + 2]])
			+ map_airstrip[location_from].ground_time
			- airplane->travelTime(from_to[airstrips[node_add_from]][airstrips[node_add_from + 2]]);

		index++;

		for (int i = node_add_from + 2; i < index; i++) {
			arrival[i] += time;
			departure[i] += time;
		}
	}

	if (non_to) {
		airstrips.insert(airstrips.begin() + node_add_to, location_to);
		double travel_time = airplane->travelTime(from_to[airstrips[node_add_to - 1]][location_to]);
		arrival.insert(arrival.begin() + node_add_to, departure[node_add_to - 1] + travel_time);
		departure.insert(departure.begin() + node_add_to, departure[node_add_to - 1] + travel_time + map_airstrip[location_to].ground_time);
		double time1 = 0.0;
		
		if (!non_to_final) {
			double travel_time_2 = airplane->travelTime(from_to[location_to][airstrips[node_add_to + 1]]);
			double travel_time_3 = airplane->travelTime(from_to[airstrips[node_add_to - 1]][airstrips[node_add_to + 1]]);
			time1 = travel_time	 + travel_time_2 + map_airstrip[location_to].ground_time - travel_time_3;
		}

		index++;

		for (int i = node_add_to + 1; i < index; i++) {
			arrival[i] += time1;
			departure[i] += time1;
		}
	}
}

void Route::do_work(int& node_add_to, int location_to, bool& non_to, bool& non_to_final, bool& num_equals)
{
	rebuilt_case = 1; //impongo che questo � il rebuilt_case 1
	//il nodo prima al from � uguale devo quindi aggiornare le capacit� e basta, non i tempi
	//li aggiorno fino a node_add_to
	int node_support_to = node_add_to;

	if (non_to_final) {
		if (location_to != airstrips[node_support_to]) {
			node_add_to = node_add_to + 1;
			non_to = true;
		}
	}
	else {
		if (airstrips[node_support_to + 1] == location_to) {
			node_add_to = node_add_to + 1;
		}
		if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
			node_add_to = node_add_to + 1;
			non_to = true;
		}
		if (num_equals && location_to != airstrips[node_support_to + 1]) {
			node_add_to = node_add_to + 1;
			non_to = true;
		}
	}
}

void Route::do_work2(int& node_add_to, int location_to, bool& non_to, bool& non_to_final, bool& num_equals)
{
	//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
	rebuilt_case = 2;
	int node_support_to = node_add_to;

	if (non_to_final) {
		if (location_to != airstrips[node_support_to]) {
			node_add_to = node_add_to + 1;
			non_to = true;
		}
	}
	else {
		if (airstrips[node_support_to + 1] == location_to) {
			node_add_to = node_add_to + 1;
		}
		if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
			node_add_to = node_add_to + 1;
			non_to = true;
			non_to_final |= (node_add_to >= index); // ho cambiato qua forse la condizione dopo la puoi commentare
			non_to_final |= (num_equals && node_add_to >= index);
		}
		if (num_equals) {
			node_add_to = node_add_to + 2;
			if (node_add_to + 2 >= index) {
				non_to_final = true;
				non_to = true;
			}
			else if (location_to != airstrips[node_support_to + 2]) {
				non_to = true;	
			}
		}
	}
}

void Route::do_work3(int& node_add_from, int& node_add_to, int location_from, int location_to, bool& non_to, bool& non_to_final, bool& num_equals)
{
	rebuilt_case = 3;
	airstrips.insert(airstrips.begin() + node_add_from + 1, location_from);
	node_add_to++;

	int node_support_to = node_add_to;

	if (non_to_final) {
		if (location_to != airstrips[node_support_to]) { // ho cambiato qua
			node_add_to = node_add_to + 1;
			non_to = true;
		}
	}
	else {

		if (airstrips[node_support_to + 1] == location_to) 
			node_add_to = node_add_to + 1;
			
		if (location_to != airstrips[node_support_to + 1] && location_to != airstrips[node_support_to] && !num_equals) {
			node_add_to = node_add_to + 1;
			non_to = true;
		}

		if (num_equals && location_to != airstrips[node_support_to + 1]) { // ho aggiunto questa
			node_add_to = node_add_to + 1;
			non_to = true;
		}
	}
}

void Route::do_work4(int& node_add_from, int location_from, map<unsigned int, Airstrip> map_airstrip, double2DVector from_to, Airplane* airplane)
{
	double time_add = departure[node_add_from] + airplane->travelTime(from_to[airstrips[node_add_from]][location_from]);

	arrival.insert(arrival.begin() + node_add_from + 1, time_add);
	departure.insert(departure.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

	double time = airplane->travelTime(from_to[airstrips[node_add_from]][location_from])
		+ airplane->travelTime(from_to[location_from][airstrips[node_add_from + 2]])
		+ map_airstrip[location_from].ground_time
		- airplane->travelTime(from_to[airstrips[node_add_from]][airstrips[node_add_from + 2]]);

	index++;

	for (int i = node_add_from + 2; i < index; i++) {
		arrival[i] += time;
		departure[i] += time;
	}

	refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);
	fuel.insert(fuel.begin() + node_add_from + 1, 0.0);
	weights.insert(weights.begin() + node_add_from + 1, 0.0);
	capacities.insert(capacities.begin() + node_add_from + 1, 0);
}

void Route::do_work5(int& node_add_to, int location_to, bool& non_to_final, map<int, Airplane> map_airplane, map<unsigned int, Airstrip> map_airstrip, double2DVector from_to)
{
	airstrips.insert(airstrips.begin() + node_add_to, location_to);
	double time_add1 = departure[node_add_to - 1] + map_airplane[aircraft_code].travelTime(from_to[airstrips[node_add_to - 1]][location_to]);
	arrival.insert(arrival.begin() + node_add_to, time_add1);
	departure.insert(departure.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);

	double time1 = 0.0;
	if (!non_to_final) {
		time1 = map_airplane[aircraft_code].travelTime(from_to[airstrips[node_add_to - 1]][location_to])
			+ map_airplane[aircraft_code].travelTime(from_to[location_to][airstrips[node_add_to + 1]])
			+ map_airstrip[location_to].ground_time
			- map_airplane[aircraft_code].travelTime(from_to[airstrips[node_add_to - 1]][airstrips[node_add_to + 1]]);
	}

	index++;

	for (int i = node_add_to + 1; i < index; i++) {
		arrival[i] += time1;
		departure[i] += time1;
	}
	refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
	fuel.insert(fuel.begin() + node_add_to, 0.0);
	weights.insert(weights.begin() + node_add_to, 0.0);
	capacities.insert(capacities.begin() + node_add_to, 0);
}

//for updating in order to improve the computational time
void Route::update_rebuilt_one_first_fase(ProcessedInput * input, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	num_equals |= (node_add_from == node_add_to);
	non_to_final |= (node_add_from == node_add_to);
	
	rebuilt_case = -1;
	if (airstrips[node_add_from] == location_from) {
		do_work(node_add_to, location_to, non_to, non_to_final, num_equals);
	}
	else if (airstrips[node_add_from + 1] == location_from) {
		do_work2(node_add_to, location_to, non_to, non_to_final, num_equals);
	}
	else {
		do_work3(node_add_from, node_add_to, location_from, location_to, non_to, non_to_final, num_equals);

		Airplane* airplane = &map_airplane[aircraft_code];
		do_work4(node_add_from, location_from, map_airstrip, from_to, airplane);
	}


	if (non_to) {

		do_work5(node_add_to, location_to, non_to_final, map_airplane, map_airstrip, from_to);
	}
}

void Route::remove_at(int position)
{
	airstrips.erase(airstrips.begin() + position);
	refueling.erase(refueling.begin() + position);
	fuel.erase(fuel.begin() + position);
	arrival.erase(arrival.begin() + position);
	departure.erase(departure.begin() + position);
	capacities.erase(capacities.begin() + position);
	weights.erase(weights.begin() + position);
	index--;
}

vector<string> Route::get_places_company()
{
	return places_company;
}

const vector<Passenger>& Route::get_passengers() const
{
	return passengers_in_route;
}


void Route::update_a_b(const int& node_add_from, const int& node_add_to, bool& non_to, int& A, int& B)
{
	if (node_add_to == node_add_from + 1 && non_to) {
		A = node_add_from + 2;
		B = node_add_from + 3;
	}
	else if (node_add_to == node_add_from + 2 && non_to) {
		A = node_add_from + 1;
		B = node_add_from + 3;
	}
	else {
		A = node_add_from + 1;
		B = node_add_from + 2;
	}
}

void Route::manage_rebuilt_case_1(int& node_add_from, int& node_add_to, Passenger& p, bool& non_to)
{
	p.solution_from = node_add_from;
	for (int i = node_add_from; i < node_add_to; i++) {
		capacities[i] += p.capacity;
		weights[i] -= p.weight;
	}

	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = node_add_from; j < index; j++) {
		if (j != node_add_to || !non_to){
			if (weights[j] < 0) {
				add_fuel = weights[j];
				index_weight_neg = j;
				update_fuel_and_weight(node_add_to, non_to, add_fuel, index_weight_neg);
			}
		}
	}
}

void Route::manage_rebuilt_case_2(int& node_add_from, int& node_add_to, Passenger& p, bool& non_to)
{
	p.solution_from = node_add_from + 1;
	for (int i = node_add_from + 1; i < node_add_to; i++) {
		capacities[i] += p.capacity;
		weights[i] -= p.weight;
	}

	double add_fuel = 0;
	int index_weight_neg = -1;

	for (int j = node_add_from + 1; j < index; j++) {
		if (j != node_add_to || !non_to) {
			if (weights[j] < 0) {
				add_fuel = weights[j];
				index_weight_neg = j;
				update_fuel_and_weight(node_add_to, non_to, add_fuel, index_weight_neg);
			}
		}
	}
}

void Route::manage_rebuilt_case_3(int& node_add_from, int& node_add_to, Passenger& p, bool& non_to, Airplane* airplane, double3DVector from_to_fuelConsumed)
{
	p.solution_from = node_add_from + 1;

	//update passengers in route
	update_solution_from_to(node_add_from);

	//update of fuel, capacities and weights
	capacities[node_add_from + 1] = capacities[node_add_from];
	weights[node_add_from + 1] = weights[node_add_from];

	for (int i = node_add_from + 1; i < node_add_to; i++) {
		capacities[i] += p.capacity;
		weights[i] -= p.weight;
	}

	//fuel consumed
	int A = 0;
	int B = 0;
	update_a_b(node_add_from, node_add_to, non_to, A, B);

	double fuel_consumed = from_to_fuelConsumed[aircraft_code][airstrips[node_add_from]][airstrips[A]];
	//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
	if (refueling[A]) {
		fuel[A] = airplane->max_fuel;
		weights[A] = weights[A] - (airplane->max_fuel - fuel[node_add_from]);  //ho aggiunto questa riga
	}
	else if (!refueling[A]) {
		fuel[A] = fuel[node_add_from] - fuel_consumed;
		weights[A] = weights[A] + fuel_consumed;
	}

	double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][airstrips[A]][airstrips[B]];
	double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_from]][airstrips[B]];
	if (!refueling[A]) {
		for (int i = B; i < index; i++) {
			if (!non_to || i != node_add_to) {
				if (refueling[i])
					break;
					
				fuel[i] -= fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
				weights[i] += fuel_consumed + fuel_consumed1 - fuel_consumed2;
			}
		}
	}
	else {
		for (int i = B; i < index; i++) {
			if (!non_to || i != node_add_to) {
				if (refueling[i])
					break;

				fuel[i] -= fuel_consumed1 + fuel_consumed2 + (airplane->max_fuel - fuel[node_add_from]);
				weights[i] += fuel_consumed1 - fuel_consumed2 - (airplane->max_fuel - fuel[node_add_from]);
			}
		}
	}

	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = A; j < index; j++) { //secondo me qua non � NODE_ADD_TO ma deve essere INDEX
		if (!non_to || j != node_add_to) {
			if (weights[j] < 0) {
				add_fuel = weights[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (!non_to || i != node_add_to) {
						if (refueling[i]) {
							index_refueling = i;
							break;
						}
					}
				}
					
				for (int t = index_refueling; t < index; t++) {
					if (!non_to || t != node_add_to) 
					{
						if (refueling[t] && t != index_refueling) 
							break;
						
						fuel[t] += add_fuel;
						weights[t] -= add_fuel;
					}
				}
			}
		}
	}
}

void Route::update_rebuilt_one_second_fase(ProcessedInput* input, int& node_add_from, int& node_add_to, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals) {

	Airplane* airplane = & map<int,Airplane>(input->get_map_airplane())[aircraft_code];
	
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	if (rebuilt_case == -1) {
		cout << "problema in route.cpp con la variabile rebuilt_case" << endl;
		cin.get();
	}
	else if (rebuilt_case == 1) {
		manage_rebuilt_case_1(node_add_from, node_add_to, p, non_to);
	}
	else if (rebuilt_case == 2) {
		manage_rebuilt_case_2(node_add_from, node_add_to, p, non_to);
	}
	else if (rebuilt_case == 3) {
		manage_rebuilt_case_3(node_add_from, node_add_to, p, non_to, airplane, from_to_fuelConsumed);
	}


	if (non_to) {

		p.solution_to = node_add_to;
		do_work_3(node_add_to);

		double weight_add = 0.0;
		/////////////////////////////////////////
		//fuel consumed
		auto fuel_consumed_array = from_to_fuelConsumed[aircraft_code];
		double fuel_consumed3 = fuel_consumed_array[airstrips[node_add_to - 1]][airstrips[node_add_to]];
		fuel[node_add_to] = refueling[node_add_to] ? airplane->max_fuel : (fuel[node_add_to - 1] - fuel_consumed3);

		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacities[node_add_to] = capacities[node_add_to - 1] - p.capacity;
			if (refueling[node_add_to]) {
				weights[node_add_to] = weights[node_add_to - 1] + p.weight - (airplane->max_fuel - fuel[node_add_to - 1]);
			}
			else {
				weights[node_add_to] = weights[node_add_to - 1] + p.weight + fuel_consumed3;
			}
		}
		
		double fuel_consumed4 = 0.0;
		double fuel_consumed5 = 0.0;
		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_to]][airstrips[node_add_to + 1]];
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][airstrips[node_add_to - 1]][airstrips[node_add_to + 1]];
		}

		//qua devo considerare il fottuto rebuilt_case bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (refueling[i])
					break;

				fuel[i] -= fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
				weights[i] = fuel_consumed3 + fuel_consumed4 - fuel_consumed5;				
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (refueling[i])
					break; 
				
				fuel[i] -= fuel_consumed4 + fuel_consumed5 + (airplane->max_fuel - fuel[node_add_to - 1]);
				weights[i] += fuel_consumed4 - fuel_consumed5 - (airplane->max_fuel - fuel[node_add_to - 1]);
			}
		}

		if (non_to_final) {
			if (map_airstrip[location_to].fuel) 
				weight_add = airplane->load_weight - airplane->max_fuel;
			else {
				const auto& airstrip_a = airstrips[node_add_to - 1];
				const auto& airstrip_b = airstrips[node_add_to];
				weight_add = airplane->load_weight - (fuel[node_add_to - 1] - from_to_fuelConsumed[aircraft_code][airstrip_a][airstrip_b]);
			}
			weights[node_add_to] += weight_add;
		}

		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO
		do_work_3(node_add_to); 
	}
	else {
		p.solution_to = node_add_to;
	}

	for (int k = 0; k < index; k++) {
		if (refueling[k] && fuel[k] < airplane->max_fuel) {
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weights[k];
			for (int i = k + 1; i < index; i++) {
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weights[i] < min_weight) {
					min_weight = weights[i];
					Node_min = i;
				}

			}
			double Fuel_before = fuel[index_updating_from];
			fuel[index_updating_from] = min(airplane->max_fuel, fuel[index_updating_from] + min_weight);
			weights[index_updating_from] -= (fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				fuel[i] += fuel[index_updating_from] - Fuel_before;
				weights[i] -= fuel[index_updating_from] - Fuel_before;
			}
		}
	}
}

Route::~Route()
{
}

const vector<int> Route::get_capacities()
{
	return capacities;
}

void Route::set_capacity_at(int position, int value)
{
	if (position >= 0 && position < static_cast<int>(capacities.size())) capacities[position] = value;
}

void Route::add_capacity_at(int position, int value)
{
	if (position >= 0 && position < static_cast<int>(capacities.size())) capacities[position] += value;
}
