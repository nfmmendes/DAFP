#include "Route.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;


Route::Route()
{
	aircraft_code_company_comparison = "";
	aircraft_code = 0;
	//places;
	//places_company;
	//time_arr;
	//time_dep;
	index = 0;
	//refueling;
	//quantity_fuel;
	//weight;
	//capacity;
	primo_pass = false;
	airplane_day_before = false;

	//passengers_in_route;
	cost = 0.0;

}


Route::Route(int aircraft_code, vector<Passenger>& pass)
{
	this->aircraft_code = aircraft_code;
	//places_company;
	//places;
	index = 0;
	//refueling;
	//quantity_fuel;
	//time_arr;
	//time_dep;
	aircraft_code_company_comparison = "";
	airplane_day_before = false;

	//weight;
	//capacity;
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
		cout << " t_a(" << time_arr[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " t_d(" << time_dep[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " q_f(" << quantity_fuel[z] << ")\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " c_p(" << capacity[z] << "***)\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " w_p(" << weight[z] << ")\t";
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
		cout << " t_a(" << time_arr[z] << ")\t";
	}

	cout << endl;




}

void Route::addPlace_companySolution(string place, int minute) {
	places_company.push_back(place);
	time_arr.push_back(minute);
	index++;
	return;
}

void Route::addPlace(int place, bool refueling, double quantity_fuel, double weight, int capacity, double minute_arr, double minute_dep) {
	places.push_back(place);
	time_arr.push_back(minute_arr);
	time_dep.push_back(minute_dep);


	this->refueling.push_back(refueling);
	this->quantity_fuel.push_back(quantity_fuel);
	index++;

	this->weight.push_back(weight);
	this->capacity.push_back(capacity);
	//index_cap++;



	return;
}

void Route::removePlace(int position, map<int, Airplane>& map_airplane) {
	index = index - 1;
	//index_cap = index_cap - 1;


	places.erase(places.begin() + position);
	time_arr.erase(time_arr.begin() + position);
	time_dep.erase(time_dep.begin() + position);
	refueling.erase(refueling.begin() + position);
	quantity_fuel.erase(quantity_fuel.begin() + position);
	weight.erase(weight.begin() + position);
	capacity.erase(capacity.begin() + position);

	int cont = 0;
	for (int i = 0; i < index; i++) {
		if (capacity[i] == 0) cont++;
	}
	if (cont == index) {
		//print();
		primo_pass = false;
		//cout << "verificato" << endl;
	}


	int index_sup = index;
	for (int i = index_sup - 1; i > 1; i--) {

		if (capacity[i - 1] != 0) break;

		if (capacity[i - 1] == 0) {
			places.erase(places.begin() + i);
			time_arr.erase(time_arr.begin() + i);
			time_dep.erase(time_dep.begin() + i);
			refueling.erase(refueling.begin() + i);
			quantity_fuel.erase(quantity_fuel.begin() + i);
			weight.erase(weight.begin() + i);
			capacity.erase(capacity.begin() + i);
			index = index - 1;
			//index_cap = index_cap - 1;

		}
	}


	if (index == 2 && capacity[0] == 0) {
		places.erase(places.begin() + 1);
		time_arr.erase(time_arr.begin() + 1);
		time_dep.erase(time_dep.begin() + 1);
		refueling.erase(refueling.begin() + 1);
		quantity_fuel.erase(quantity_fuel.begin() + 1);
		weight.erase(weight.begin() + 1);
		capacity.erase(capacity.begin() + 1);
		index = index - 1;
		//index_cap = index_cap - 1;
	}



	if (places.size() == 1 && capacity[0] == 0) {
		quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
		weight[0] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
	}





}


void Route::update_route_destroy(int node_destroy, int min_from_pass, int max_to_pass, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip) {

	time_arr[node_destroy + 1] = time_dep[node_destroy - 1] + from_to[places[node_destroy - 1]][places[node_destroy + 1]] / map_airplane[aircraft_code].speed * 60;
	time_dep[node_destroy + 1] = time_arr[node_destroy + 1] + map_airstrip[places[node_destroy + 1]].ground_time;

	for (int i = node_destroy + 1; i < index - 1; i++) {
		time_arr[i + 1] = time_dep[i] + from_to[places[i]][places[i + 1]] / map_airplane[aircraft_code].speed * 60;
		time_dep[i + 1] = time_arr[i + 1] + map_airstrip[places[i + 1]].ground_time;
	}

	for (Passenger& p : passengers_in_route) {
		if (p.solution_from > node_destroy) p.solution_from--;
		if (p.solution_to > node_destroy) p.solution_to--;
	}


	//cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
	//cout << "******************* Sono in Route.cpp voglio ridare fuel  *******************" << endl;
	//cout << " CODE ------> " << aircraft_code << endl;
	//cout << " Minimo from passeggeri ----> " << min_from_pass << endl;
	//cout << " Massimo to passeggeri ----> " << max_to_pass << endl;
	//print();
	//cout << " Node destroyed ------> " << node_destroy << endl;
	int index_min_from = 0;
	for (int i = min_from_pass; i >= 0; i--) {
		if (refueling[i] && i != node_destroy) {
			index_min_from = i;
			break;
		}
	}
	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	// Codice che da reduel ai nodi con refuel non massimo 
	for (int k = index_min_from; k < max_to_pass; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel && k != node_destroy) { //&& k!= node_destroy
		//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i <= max_to_pass; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) break;
				if (weight[i] < min_weight && i != node_destroy) {
					min_weight = weight[i];
					Node_min = i;
				}
				//}
			}
			if (Node_min >= 0) {
				for (int i = k + 1; i < index; i++) {
					if (refueling[i]) {  // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
						index_updating_to = i;
						break;
					}
				}
				//cout << " Minimo trovato " << min_weight << endl;
				//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
				//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
				double Fuel_before = quantity_fuel[index_updating_from];
				//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[aircraft_code].max_fuel << " e " << quantity_fuel[index_updating_from] + min_weight << endl;
				quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
				weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
				for (int i = index_updating_from + 1; i < index_updating_to; i++) {
					if (i != node_destroy) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}

	}
}


void Route::update_route_rebuilt_one(int node_add_from, int node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed) {

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
			capacity[i] += p.capacity;
			weight[i] -= p.weight;
		}
		//cout << "pippo1" << endl;





		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j < index; j++) {
			if (weight[j] < 0) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 467 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				//ccout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//ccout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//ccout << " Weight dopo " << weight[t] << endl;
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
			capacity[i] += p.capacity;
			weight[i] -= p.weight;
		}




		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j < index; j++) {
			if (weight[j] < 0) {
				//ccout << " Sono nel rapeir devo aggiunger fuel linea 581 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
				//ccout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//ccout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//ccout << " Weight dopo " << weight[t] << endl;
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

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
			//node_add_to = node_add_to + 1;
			//non_to = true;
			//}

		}

		double time_add = time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_from + 1, time_add);
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}
		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}


		quantity_fuel.insert(quantity_fuel.begin() + node_add_from + 1, 0.0);

		weight.insert(weight.begin() + node_add_from + 1, 0.0);
		capacity.insert(capacity.begin() + node_add_from + 1, 0);

		//update of fuel, capacity and weight
		capacity[node_add_from + 1] = capacity[node_add_from];
		weight[node_add_from + 1] = weight[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i] += p.capacity;
			weight[i] -= p.weight;
		}

		//fuel consumed
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[node_add_from + 1]];
		//double travel_time = (from_to[places[node_add_from] + ";" + places[node_add_from + 1]] / map_airplane[aircraft_code].speed) * 60;

		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[node_add_from + 1]) {
			quantity_fuel[node_add_from + 1] = map_airplane[aircraft_code].max_fuel;
			weight[node_add_from + 1] = weight[node_add_from + 1] - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[node_add_from + 1]) {
			quantity_fuel[node_add_from + 1] = quantity_fuel[node_add_from] - fuel_consumed;
			weight[node_add_from + 1] = weight[node_add_from + 1] + fuel_consumed;
		}



		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[node_add_from + 1]][places[node_add_from + 2]];
		//double travel_time1 = (from_to[places[node_add_from + 1] + ";" + places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60;



		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[node_add_from + 2]];

		if (!refueling[node_add_from + 1]) {
			for (int i = node_add_from + 2; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
					weight[i] = weight[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
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
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
					weight[i] = weight[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
				}
				else {
					break;
				}
			}
		}


		/*
		int index_refueling = node_add_from + 1;
		for (int i = node_add_from + 1; i >= 0; i--) {
		if (refueling[i]) {
		index_refueling = i;
		break;
		}
		}
		double add_fuel = 0;
		for (int j = node_add_from + 1; j < node_add_to; j++) {
		if (weight[j] < add_fuel) add_fuel = weight[j];
		}

		for (int t = index_refueling; t < index; t++) {
		if (refueling[t] && t != index_refueling) break;
		quantity_fuel[t] += add_fuel;
		weight[t] -= add_fuel;
		}
		*/

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from + 1; j < index; j++) { //secondo me qua non � NODE_ADD_TO ma deve essere INDEX
			if (weight[j] < 0) {
				//ccout << " Sono nel rapeir devo aggiunger fuel linea 789 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
				//ccout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//ccout << " Weight dopo " << weight[t] << endl;
				}
			}
		}


	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		//cout << "places.size()=" << places.size() << endl;
		//cout << "node_add_to" <<node_add_to << endl;
		p.solution_to = node_add_to;
		double time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_to, time_add1);
		time_dep.insert(time_dep.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);
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
			time_arr[i] += time1;
			time_dep[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from >= node_add_to) p.solution_from++;
			if (p.solution_to >= node_add_to) p.solution_to++;
		}

		quantity_fuel.insert(quantity_fuel.begin() + node_add_to, 0.0);

		double weight_add = 0.0;

		weight.insert(weight.begin() + node_add_to, weight_add);
		capacity.insert(capacity.begin() + node_add_to, 0);




		/////////////////////////////////////////
		//fuel consumed
		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];
		//double travel_time3 = (from_to[places[node_add_to - 1] + ";" + places[node_add_to]] / map_airplane[aircraft_code].speed) * 60;





		if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}

		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacity[node_add_to] = capacity[node_add_to - 1] - p.capacity;
			if (refueling[node_add_to]) {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
			}
			else {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}
		//********************************************************************************************************************************************************************************





		double fuel_consumed4 = 0.0;
		double fuel_consumed5 = 0.0;
		//double travel_time4 = 0.0;

		if (!non_to_final) {

			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
			//travel_time4 = (from_to[places[node_add_to] + ";" + places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60;


			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}


		//double travel_time5 = 0.0;
		//if (!non_to_final) {


			/*travel_time5 = (from_to[places[node_add_to - 1] + ";" + places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60;
			if (travel_time5 <= 60) {
				fuel_consumed5 += (travel_time5 / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed5 += map_airplane[aircraft_code].fuel_burn_first + ((travel_time5 - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/
			//cout << "fuel 5: " << fuel_consumed5 << endl;
		//}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weight[i] = weight[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
					weight[i] = weight[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}
















		if (non_to_final) {
			//double time = (from_to[places[node_add_to - 1] + ";" + places[node_add_to]] / map_airplane[aircraft_code].speed) * 60;
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];





			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[node_add_to - 1] - fuel_consumed);
			}
			weight[node_add_to] += weight_add;
		}


		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO



		//cout << "sono davanti all'aggiornamento del caso in cui ho inserito il to" << endl;
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_to; j < index; j++) {
			//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
			if (weight[j] < 0) {
				//cout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//cout << " Weight dopo " << weight[t] << endl;
				}
			}
		}



	}
	else {
		p.solution_to = node_add_to;
	}


	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	//cout << "caso prima dell'aggiornamento in fondo del repair" << endl;
	//print();
	for (int k = 0; k < index; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			//int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					//Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}


}




void Route::add_update_only_one_node_first_passanger(Passenger& p, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {

	if (places[0] == p.departure_location) {
		time_arr[0] = p.early_departure;
		time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
		capacity[0] = p.capacity;
		quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
		weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


		double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


		addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
		if (refueling[1] == true) {
			quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
			weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
		}
		else {
			quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
			weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
		}

		p.solution_from = 0;
		p.solution_to = 1;


	}
	else {

		time_arr[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
			map_airstrip[places[index - 1]].ground_time;
		time_dep[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

		//for the weight
		weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
			quantity_fuel[index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
		//end for weight

		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
		if (map_airstrip[p.departure_location].fuel) {
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
				map_airplane[aircraft_code].max_fuel - p.weight;
		}
		else {
			double fuel_before = quantity_fuel[index - 1];

			//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;



			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
			weight[index - 1] = weight[index - 2] - p.weight + fuel_consumed;

		}
		p.solution_from = index - 1;
		double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;




		if (map_airstrip[p.arrival_location].fuel) {
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
				map_airplane[aircraft_code].max_fuel;
		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;

			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = weight[index - 2] + p.weight + fuel_consumed;

		}
		p.solution_to = index - 1;

	}

}


void Route::move_c(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {


	if (places[index - 1] == location_from) {

		capacity[index - 1] += p.capacity;
		weight[index - 1] -= p.weight;

		double time_arri = time_dep[index - 1] + (from_to[location_from][location_to] / map_airplane[aircraft_code].speed) * 60;
		double time_depa = time_arri + map_airstrip[location_to].ground_time;
		p.solution_from = index - 1;

		//double time_fly = (from_to[location_from + ";" + location_to] / map_airplane[aircraft_code].speed) * 60;


		double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_from][location_to];


		if (map_airstrip[location_to].fuel) {
			addPlace(location_to, map_airstrip[location_to].fuel, map_airplane[aircraft_code].max_fuel, (map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel), 0, time_arri, time_depa);
		}
		else {
			addPlace(location_to, map_airstrip[location_to].fuel, quantity_fuel[index - 1] - fuel_consumed, (map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[index - 1] - fuel_consumed)), 0, time_arri, time_depa);
		}

		p.solution_to = index - 1;


	}
	else {

		if (map_airstrip[p.departure_location].fuel) {
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, p.capacity,
				time_dep[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				time_dep[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
				map_airplane[aircraft_code].max_fuel - p.weight;


		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;



			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, time_dep[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				time_dep[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1] - p.weight;

		}
		p.solution_from = index - 1;
		double time = time_dep[index - 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;





		if (map_airstrip[p.arrival_location].fuel) {
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;

		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = weight[index - 2] + fuel_consumed + p.weight;

		}


		p.solution_to = index - 1;





	}







}

void Route::update_time_for_check_repair(int node_add_from, int node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip) {

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

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
				//node_add_to = node_add_to + 1;
				//non_to = true;
			//}

		}

		time_arr.insert(time_arr.begin() + node_add_from + 1, time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60));
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}

	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		//cout << "places.size()=" << places.size() << endl;
		//cout << "node_add_to" <<node_add_to << endl;
		time_arr.insert(time_arr.begin() + node_add_to, time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60));
		time_dep.insert(time_dep.begin() + node_add_to, time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[location_to].ground_time);
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
			time_arr[i] += time1;
			time_dep[i] += time1;
		}


	}




}

//for updating in order to improve the computational time
void Route::update_route_rebuilt_one_first_fase(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

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

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
			//node_add_to = node_add_to + 1;
			//non_to = true;
			//}

		}

		double time_add = time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_from + 1, time_add);
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}

		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_from + 1, 0.0);
		weight.insert(weight.begin() + node_add_from + 1, 0.0);
		capacity.insert(capacity.begin() + node_add_from + 1, 0);



	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		double time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_to, time_add1);
		time_dep.insert(time_dep.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);

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
			time_arr[i] += time1;
			time_dep[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_to, 0.0);
		weight.insert(weight.begin() + node_add_to, 0.0);
		capacity.insert(capacity.begin() + node_add_to, 0);




	}



}



void Route::update_route_rebuilt_one_second_fase(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	//in questo commento qua sotto ci sono i vari casi con l'if al posto dello switch

	if (caso == -1) {
		cout << "problema in route.cpp con la variabile caso" << endl;
		cin.get();
	}
	else if (caso == 1) {

		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacity[i] += p.capacity;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j < index; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 467 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
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
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
							//ccout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
			}

		}





	}
	else if (caso == 2) {
		//il nodo dopo il from � uguale quindi devo aggiornare le capacit� e basta, non i tempi
		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i] += p.capacity;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j < index; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 581 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
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
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}

						//ccout << " Weight dopo " << weight[t] << endl;
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

		//update of fuel, capacity and weight
		capacity[node_add_from + 1] = capacity[node_add_from];
		weight[node_add_from + 1] = weight[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i] += p.capacity;
			weight[i] -= p.weight;
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
			quantity_fuel[A] = map_airplane[aircraft_code].max_fuel;
			weight[A] = weight[A] - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[A]) {
			quantity_fuel[A] = quantity_fuel[node_add_from] - fuel_consumed;
			weight[A] = weight[A] + fuel_consumed;
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
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
						weight[i] = weight[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
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
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
						weight[i] = weight[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
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
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 789 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
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
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (non_to && t == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}
						//ccout << " Weight dopo " << weight[t] << endl;
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
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}




		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacity[node_add_to] = capacity[node_add_to - 1] - p.capacity;
			if (refueling[node_add_to]) {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
			}
			else {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}
		//********************************************************************************************************************************************************************************






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
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weight[i] = weight[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
					weight[i] = weight[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}


		if (non_to_final) {
			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[node_add_to - 1] - from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]]);
			}
			weight[node_add_to] += weight_add;
		}


		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO


		//cout << "sono davanti all'aggiornamento del caso in cui ho inserito il to" << endl;
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_to; j < index; j++) {
			//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
			if (weight[j] < 0) {
				//cout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//cout << " Weight dopo " << weight[t] << endl;
				}
			}
		}



	}
	else {
		p.solution_to = node_add_to;
	}


	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	//cout << "caso prima dell'aggiornamento in fondo del repair" << endl;
	//print();
	for (int k = 0; k < index; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}


}





Route::~Route()
{
}
