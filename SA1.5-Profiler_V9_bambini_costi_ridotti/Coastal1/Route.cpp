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


Route::Route(int aircraft_code, vector<Passenger>& pass)
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

void Route::print() {
	cout << "-" << to_string(aircraft_code) << "ha passeggeri: " << primo_pass << endl;
	cout << "\t";
	for (int t = 0; t < index; t++) {
		cout << " ->" << to_string(places[t]) << "***\t";
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
	return;
}

void Route::addPlace(int place, bool has_refueling, double quantity_fuel, double weight, int capacity, double minute_arr, double minute_dep) {
	places.push_back(place);
	arrival.push_back(minute_arr);
	departure.push_back(minute_dep);
	refueling.push_back(has_refueling);
	fuel.push_back(quantity_fuel);
	weights.push_back(weight);
	capacities.push_back(capacity);
	
	index++;
}

void Route::removePlace(int position, map<int, Airplane>& map_airplane) {
	index = index - 1;

	places.erase(places.begin() + position);
	arrival.erase(arrival.begin() + position);
	departure.erase(departure.begin() + position);
	refueling.erase(refueling.begin() + position);
	fuel.erase(fuel.begin() + position);
	weights.erase(weights.begin() + position);
	capacities.erase(capacities.begin() + position);

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

		if (capacities[i - 1] != 0) break;

		if (capacities[i - 1] == 0) {
			places.erase(places.begin() + i);
			arrival.erase(arrival.begin() + i);
			departure.erase(departure.begin() + i);
			refueling.erase(refueling.begin() + i);
			fuel.erase(fuel.begin() + i);
			weights.erase(weights.begin() + i);
			capacities.erase(capacities.begin() + i);
			index = index - 1;
		}
	}


	if (index == 2 && capacities[0] == 0) {
		places.erase(places.begin() + 1);
		arrival.erase(arrival.begin() + 1);
		departure.erase(departure.begin() + 1);
		refueling.erase(refueling.begin() + 1);
		fuel.erase(fuel.begin() + 1);
		weights.erase(weights.begin() + 1);
		capacities.erase(capacities.begin() + 1);
		index = index - 1;
	}

	Airplane* airplane = &map_airplane[aircraft_code]; 
	if (places.size() == 1 && capacities[0] == 0) {
		fuel[0] = airplane->max_fuel;
		weights[0] = airplane->load_weight - airplane->max_fuel;
	}

}

void Route::update_route_destroy(ProcessedInput* input, int node_destroy, int min_from_pass, int max_to_pass) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	
	arrival[node_destroy + 1] = departure[node_destroy - 1] + from_to[places[node_destroy - 1]][places[node_destroy + 1]] / map_airplane[aircraft_code].speed * 60;
	departure[node_destroy + 1] = arrival[node_destroy + 1] + map_airstrip[places[node_destroy + 1]].ground_time;

	for (int i = node_destroy + 1; i < index - 1; i++) {
		arrival[i + 1] = departure[i] + from_to[places[i]][places[i + 1]] / map_airplane[aircraft_code].speed * 60;
		departure[i + 1] = arrival[i + 1] + map_airstrip[places[i + 1]].ground_time;
	}

	for (Passenger& p : passengers_in_route) {
		if (p.solution_from > node_destroy) p.solution_from--;
		if (p.solution_to > node_destroy) p.solution_to--;
	}
	
	int index_min_from = 0;
	for (int i = min_from_pass; i >= 0; i--) {
		if (refueling[i] && i != node_destroy) {
			index_min_from = i;
			break;
		}
	}

	for (int k = index_min_from; k < max_to_pass; k++) {
		if (refueling[k] && fuel[k] < map_airplane[aircraft_code].max_fuel && k != node_destroy) { //&& k!= node_destroy
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weights[k];
			for (int i = k + 1; i <= max_to_pass; i++) {
				if (refueling[i]) break;
				if (weights[i] < min_weight && i != node_destroy) {
					min_weight = weights[i];
					Node_min = i;
				}
			}
			if (Node_min >= 0) {
				for (int i = k + 1; i < index; i++) {
					if (refueling[i]) {  // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
						index_updating_to = i;
						break;
					}
				}

				double Fuel_before = fuel[index_updating_from];

				fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, fuel[index_updating_from] + min_weight);
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

void Route::update_route_rebuilt_one(ProcessedInput* input, int node_add_from, int node_add_to, int location_from, int location_to, Passenger& p) {
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();

	bool non_to = false;
	bool non_to_final = false;
	bool num_equals = false;

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == index - 1) non_to_final = true;

	if (places[node_add_from] == location_from) {
		//il nodo prima al from � uguale devo quindi aggiornare le capacit� e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j < index; j++) {
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
					if (refueling[t] && t != index_refueling) break;
					fuel[t] += add_fuel;
					weights[t] -= add_fuel;
				}
			}
		}
	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
		int node_support_to = node_add_to;


		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}


		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j < index; j++) {
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
					if (refueling[t] && t != index_refueling) break;
					fuel[t] += add_fuel;
					weights[t] -= add_fuel;
				}
			}
		}

	}
	else {
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;
		p.solution_from = node_add_from + 1;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}

		double time_add = departure[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		arrival.insert(arrival.begin() + node_add_from + 1, time_add);
		departure.insert(departure.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			arrival[i] += time;
			departure[i] += time;
		}
		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}

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
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[node_add_from + 1]];
		//double travel_time = (from_to[places[node_add_from] + ";" + places[node_add_from + 1]] / map_airplane[aircraft_code].speed) * 60;

		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[node_add_from + 1]) {
			fuel[node_add_from + 1] = map_airplane[aircraft_code].max_fuel;
			weights[node_add_from + 1] = weights[node_add_from + 1] - (map_airplane[aircraft_code].max_fuel - fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[node_add_from + 1]) {
			fuel[node_add_from + 1] = fuel[node_add_from] - fuel_consumed;
			weights[node_add_from + 1] = weights[node_add_from + 1] + fuel_consumed;
		}

		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[node_add_from + 1]][places[node_add_from + 2]];
		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[node_add_from + 2]];

		if (!refueling[node_add_from + 1]) {
			for (int i = node_add_from + 2; i < index; i++) {
				if (!refueling[i]) {
					fuel[i] = fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
					weights[i] = weights[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
				}
				else {
					break;
				}
			}
		}
		else {
			//caso bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = node_add_from + 2; i < index; i++) {
				if (!refueling[i]) {
					fuel[i] = fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - fuel[node_add_from]);
					weights[i] = weights[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - fuel[node_add_from]);
				}
				else {
					break;
				}
			}
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from + 1; j < index; j++) { //secondo me qua non � NODE_ADD_TO ma deve essere INDEX
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
					if (refueling[t] && t != index_refueling) break;
					fuel[t] += add_fuel;
					weights[t] -= add_fuel;
				}
			}
		}
	}

	if (non_to) {

		places.insert(places.begin() + node_add_to, location_to);
		p.solution_to = node_add_to;
		double time_add1 = departure[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		arrival.insert(arrival.begin() + node_add_to, time_add1);
		departure.insert(departure.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);
		double time1 = 0.0;
		if (!non_to_final) {
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		for (int i = node_add_to + 1; i < index; i++) {
			arrival[i] += time1;
			departure[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from >= node_add_to) p.solution_from++;
			if (p.solution_to >= node_add_to) p.solution_to++;
		}

		fuel.insert(fuel.begin() + node_add_to, 0.0);

		double weight_add = 0.0;

		weights.insert(weights.begin() + node_add_to, weight_add);
		capacities.insert(capacities.begin() + node_add_to, 0);

		/////////////////////////////////////////
		//fuel consumed
		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];
		if (refueling[node_add_to]) {
			fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			fuel[node_add_to] = fuel[node_add_to - 1] - fuel_consumed3;
		}

		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacities[node_add_to] = capacities[node_add_to - 1] - p.capacity;
			if (refueling[node_add_to]) {
				weights[node_add_to] = weights[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - fuel[node_add_to - 1]);
			}
			else {
				weights[node_add_to] = weights[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}

		double fuel_consumed4 = 0.0;
		double fuel_consumed5 = 0.0;

		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					fuel[i] = fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weights[i] = weights[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					fuel[i] = fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - fuel[node_add_to - 1]);
					weights[i] = weights[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}

		if (non_to_final) {
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];

			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].load_weight - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].load_weight - (fuel[node_add_to - 1] - fuel_consumed);
			}
			weights[node_add_to] += weight_add;
		}


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
					if (refueling[t] && t != index_refueling) break;
					fuel[t] += add_fuel;
					weights[t] -= add_fuel;
				}
			}
		}
	}
	else {
		p.solution_to = node_add_to;
	}

	for (int k = 0; k < index; k++) {
		if (refueling[k] && fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			//int Node_min = k;
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
			fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, fuel[index_updating_from] + min_weight);
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
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	if (places[0] == p.departure_location) {
		arrival[0] = p.early_departure;
		departure[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
		capacities[0] = p.capacity;
		fuel[0] = map_airplane[aircraft_code].max_fuel;
		weights[0] = map_airplane[aircraft_code].load_weight - fuel[0] - p.weight;

		double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];

		addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, departure[0] + time, departure[0] + time + map_airstrip[p.arrival_location].ground_time);
		if (refueling[1] == true) {
			fuel[1] = map_airplane[aircraft_code].max_fuel;
			weights[1] = map_airplane[aircraft_code].load_weight - fuel[1];
		}
		else {
			fuel[1] = fuel[0] - fuel_consumed;
			weights[1] = map_airplane[aircraft_code].load_weight - fuel[1];
		}

		p.solution_from = 0;
		p.solution_to = 1;
	}
	else {

		arrival[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
			map_airstrip[places[index - 1]].ground_time;
		departure[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

		//for the weights
		weights[index - 1] = map_airplane[aircraft_code].load_weight -
			fuel[index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
		//end for weights

		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
		if (map_airstrip[p.departure_location].fuel) {
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight -
				map_airplane[aircraft_code].max_fuel - p.weight;
		}
		else {
			double fuel_before = fuel[index - 1];

			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
			weights[index - 1] = weights[index - 2] - p.weight + fuel_consumed;
		}
		p.solution_from = index - 1;
		double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;

		if (map_airstrip[p.arrival_location].fuel) {
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight -
				map_airplane[aircraft_code].max_fuel;
		}
		else {
			double fuel_before = fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];

			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weights[index - 1] = weights[index - 2] + p.weight + fuel_consumed;

		}
		p.solution_to = index - 1;
	}
}

void Route::move_c(ProcessedInput* input, Passenger& p, int location_from, int location_to) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();

	if (places[index - 1] == location_from) {

		capacities[index - 1] += p.capacity;
		weights[index - 1] -= p.weight;

		double time_arri = departure[index - 1] + (from_to[location_from][location_to] / map_airplane[aircraft_code].speed) * 60;
		double time_depa = time_arri + map_airstrip[location_to].ground_time;
		p.solution_from = index - 1;
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_from][location_to];

		if (map_airstrip[location_to].fuel) {
			addPlace(location_to, map_airstrip[location_to].fuel, map_airplane[aircraft_code].max_fuel, (map_airplane[aircraft_code].load_weight - map_airplane[aircraft_code].max_fuel), 0, time_arri, time_depa);
		}
		else {
			addPlace(location_to, map_airstrip[location_to].fuel, fuel[index - 1] - fuel_consumed, (map_airplane[aircraft_code].load_weight - (fuel[index - 1] - fuel_consumed)), 0, time_arri, time_depa);
		}

		p.solution_to = index - 1;
	}
	else {

		if (map_airstrip[p.departure_location].fuel) {
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, p.capacity,
				departure[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				departure[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight -
				map_airplane[aircraft_code].max_fuel - p.weight;
		}
		else {
			double fuel_before = fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
			
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, departure[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				departure[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weights[index - 1] = map_airplane[aircraft_code].load_weight - fuel[index - 1] - p.weight;

		}
		p.solution_from = index - 1;
		double time = departure[index - 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;

		if (map_airstrip[p.arrival_location].fuel) {
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weights[index - 1] = map_airplane[aircraft_code].load_weight - map_airplane[aircraft_code].max_fuel;

		}
		else {
			double fuel_before = fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
			
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weights[index - 1] = weights[index - 2] + fuel_consumed + p.weight;
		}

		p.solution_to = index - 1;
	}
}

void Route::update_time_for_check_repair(ProcessedInput* input, int node_add_from, int node_add_to, int location_from, int location_to) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	bool non_to = false;
	bool non_to_final = false;
	bool num_equals = false;

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == index - 1) non_to_final = true;

	if (places[node_add_from] == location_from) {
		//il nodo prima al from � uguale devo quindi aggiornare le capacit� e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}
	}
	else {
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}

		arrival.insert(arrival.begin() + node_add_from + 1, departure[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60));
		departure.insert(departure.begin() + node_add_from + 1, departure[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			arrival[i] += time;
			departure[i] += time;
		}
	}


	if (non_to) {
		places.insert(places.begin() + node_add_to, location_to);
		//cout << "places.size()=" << places.size() << endl;
		arrival.insert(arrival.begin() + node_add_to, departure[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60));
		departure.insert(departure.begin() + node_add_to, departure[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[location_to].ground_time);
		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		//index_cap++;

		for (int i = node_add_to + 1; i < index; i++) {
			arrival[i] += time1;
			departure[i] += time1;
		}
	}
}

//for updating in order to improve the computational time
void Route::update_route_rebuilt_one_first_fase(ProcessedInput * input, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == index - 1) non_to_final = true;

	if (places[node_add_from] == location_from) {
		caso = 1; //impongo che questo � il caso 1
		//il nodo prima al from � uguale devo quindi aggiornare le capacit� e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
		caso = 2;
		int node_support_to = node_add_to;

		if (non_to_final) {
			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {
			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) non_to_final = true;
			}
			if (num_equals) { 
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}
	}
	else {
		caso = 3;
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
			}
		}

		double time_add = departure[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		arrival.insert(arrival.begin() + node_add_from + 1, time_add);
		departure.insert(departure.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			arrival[i] += time;
			departure[i] += time;
		}

		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);
		fuel.insert(fuel.begin() + node_add_from + 1, 0.0);
		weights.insert(weights.begin() + node_add_from + 1, 0.0);
		capacities.insert(capacities.begin() + node_add_from + 1, 0);
	}


	if (non_to) {

		places.insert(places.begin() + node_add_to, location_to);
		double time_add1 = departure[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		arrival.insert(arrival.begin() + node_add_to, time_add1);
		departure.insert(departure.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);

		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		//index_cap++;


		for (int i = node_add_to + 1; i < index; i++) {
			arrival[i] += time1;
			departure[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		fuel.insert(fuel.begin() + node_add_to, 0.0);
		weights.insert(weights.begin() + node_add_to, 0.0);
		capacities.insert(capacities.begin() + node_add_to, 0);
	}
}

void Route::update_route_rebuilt_one_second_fase(ProcessedInput* input, int& caso, int& node_add_from, int& node_add_to, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals) {

	//in questo commento qua sotto ci sono i vari casi con l'if al posto dello switch
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double3DVector from_to_fuelConsumed = input->get_from_to_fuel_consumed();
	double2DVector from_to = input->get_from_to();
	
	if (caso == -1) {
		cout << "problema in route.cpp con la variabile caso" << endl;
		cin.get();
	}
	else if (caso == 1) {

		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j < index; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weights[j] < 0) {
					add_fuel = weights[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}

					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							fuel[t] += add_fuel;
							weights[t] -= add_fuel;
						}
					}
				}
			}

		}
	}
	else if (caso == 2) {
		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacities[i] += p.capacity;
			weights[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j < index; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weights[j] < 0) {

					add_fuel = weights[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							fuel[t] += add_fuel;
							weights[t] -= add_fuel;
						}
					}
				}
			}

		}

	}
	else if (caso == 3) {

		p.solution_from = node_add_from + 1;

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}

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

		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[A]];
		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[A]) {
			fuel[A] = map_airplane[aircraft_code].max_fuel;
			weights[A] = weights[A] - (map_airplane[aircraft_code].max_fuel - fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[A]) {
			fuel[A] = fuel[node_add_from] - fuel_consumed;
			weights[A] = weights[A] + fuel_consumed;
		}

		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[A]][places[B]];
		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[B]];
		if (!refueling[A]) {
			for (int i = B; i < index; i++) {
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						fuel[i] = fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
						weights[i] = weights[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
					}
					else {
						break;
					}
				}

			}
		}
		else {
			//caso bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = B; i < index; i++) {
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						fuel[i] = fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - fuel[node_add_from]);
						weights[i] = weights[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - fuel[node_add_from]);
					}
					else {
						break;
					}
				}
			}
		}


		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = A; j < index; j++) { //secondo me qua non � NODE_ADD_TO ma deve essere INDEX
			if (non_to && j == node_add_to) {
				//non devo fare niente
			}
			else {
				if (weights[j] < 0) {
					add_fuel = weights[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (non_to && i == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					
					for (int t = index_refueling; t < index; t++) {
						if (non_to && t == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
						
							fuel[t] += add_fuel;
							weights[t] -= add_fuel;
						}
					}
				}
			}
		}
	}


	if (non_to) {

		p.solution_to = node_add_to;
		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from >= node_add_to) p.solution_from++;
			if (p.solution_to >= node_add_to) p.solution_to++;
		}

		double weight_add = 0.0;
		/////////////////////////////////////////
		//fuel consumed
		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];

		if (refueling[node_add_to]) {
			fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			fuel[node_add_to] = fuel[node_add_to - 1] - fuel_consumed3;
		}

		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacities[node_add_to] = capacities[node_add_to - 1] - p.capacity;
			if (refueling[node_add_to]) {
				weights[node_add_to] = weights[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - fuel[node_add_to - 1]);
			}
			else {
				weights[node_add_to] = weights[node_add_to - 1] + p.weight + fuel_consumed3;
			}
		}
		
		double fuel_consumed4 = 0.0;
		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
		}

		double fuel_consumed5 = 0.0;
		if (!non_to_final) {
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					fuel[i] = fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weights[i] = weights[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					fuel[i] = fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - fuel[node_add_to - 1]);
					weights[i] = weights[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}


		if (non_to_final) {
			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].load_weight - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].load_weight - (fuel[node_add_to - 1] - from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]]);
			}
			weights[node_add_to] += weight_add;
		}

		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO

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
					if (refueling[t] && t != index_refueling) break;
					fuel[t] += add_fuel;
					weights[t] -= add_fuel;
				}
			}
		}
	}
	else {
		p.solution_to = node_add_to;
	}

	for (int k = 0; k < index; k++) {
		if (refueling[k] && fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
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
			fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, fuel[index_updating_from] + min_weight);
			weights[index_updating_from] -= (fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				fuel[i] += (fuel[index_updating_from] - Fuel_before);
				weights[i] -= (fuel[index_updating_from] - Fuel_before);
			}

		}
	}
}

Route::~Route()
{
}
