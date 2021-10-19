#ifndef MOVE_H
#define MOVE_H

#include <algorithm>


#include "Destroyers.h"
#include "Evaluate.h"
#include "Feasibility.h"
#include "Repair.h"
#include "Utils.h"

struct MoveParamSet
{
	double peso_TW;
	double inter_stop_weight;
	double end_day;
};

int sequential_same_node(const Route& r) {
	int node = -1;
	for (int i = 0; i < ((int)r.places.size()) - 1; i++) {
		if (r.places[i] == r.places[i + 1]) {
			node = i;
			break;
		}
	}
	return node;
}

void aggregate_same_nodes(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node))	p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		//cout << " Nodo doppio che devo eliminare risiede nell'ultima posizione" << endl;
		r.time_dep[node + 1] = r.time_dep[node];
		r.time_arr[node + 1] = r.time_arr[node];
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
	else {
		//cout << " Nodo doppio che devo eliminare NON risiede nell'ultima posizione" << endl;
		// I remove the extra time that was considered
		double time_to_remove = r.time_dep[node + 1] - r.time_dep[node];
		for (int i = node + 1; i < r.index; i++) {
			r.time_dep[i] -= time_to_remove;
			r.time_arr[i] -= time_to_remove;
		}
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
}

void aggregate_same_nodes_inter_ls(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node))	p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		//cout << " Nodo doppio che devo eliminare risiede nell'ultima posizione" << endl;
		r.time_dep[node + 1] = r.time_dep[node];
		r.time_arr[node + 1] = r.time_arr[node];
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
	else {
		//cout << " Nodo doppio che devo eliminare NON risiede nell'ultima posizione" << endl;
		// I remove the extra time that was considered
		double time_to_remove = r.time_dep[node + 1] - r.time_dep[node];
		for (int i = node + 1; i < r.index; i++) {
			if (i == node + 1) {
				r.time_dep[i] = r.time_dep[node];
				r.time_arr[i] = r.time_arr[node];
			}
			else {
				r.time_dep[i] -= time_to_remove;
				r.time_arr[i] -= time_to_remove;
			}

		}
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}



}

int swap_from(int node, const Route& r) {

	int swap_from = 0;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}
	return swap_from;
}

int swap_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}
	}
	
	return swap_to;
}


bool swap_is_allowed(int A, int B, const Route& r) {
	bool swap = false;
	if (B < swap_to(A, r) && swap_from(B, r) < A) swap = true;
	return swap;
}

//qua purtroppo non posso mettere usare i puntatori
Route update_route_after_swap(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double2DVector& from_to, double3DVector& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;

	//cout << "sto inserendo i vari posti nella nuova route" << endl;
	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	for (int i = 1; i < r.index; i++) {
		if (i == A) {
			//in questo posto ci devo mettere B
			r_new.addPlace(r.places[B], r.refueling[B], r.quantity_fuel[B], 0.0, 0, r.time_arr[B], r.time_dep[B]);
		}
		else if (i == B) {
			//in questo posto ci devo mettere A
			r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
		}
		else {
			//in questo posto ci devo mettere normalmente i
			r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
		}
	}
	
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			if (r_new.refueling[i]) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
			}
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}

	for (Passenger p : r.passengers_in_route) {
		if (p.solution_from == A) p.solution_from = B;
		else if (p.solution_to == A) p.solution_to = B;
		else if (p.solution_from == B) p.solution_from = A;
		else if (p.solution_to == B) p.solution_to = A;
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
	}
	
	//aggiorno fuel se il peso � negatico
	for (int i = 0; i < r_new.index; i++) {
		if (r_new.weight[i] < 0) {
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			r_new.weight[index_refueling] -= r_new.weight[i];
			
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					r_new.weight[j] -= Update_value;
				}
			}
		}
	}

	return r_new;
}

vector <Route> swap(MoveParamSet param_set, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double2DVector& from_to, double2DVector& location_fuel, double3DVector& from_to_FuelConsumed) {
	const auto peso_TW = param_set.peso_TW;
	const auto inter_stop_weight = param_set.inter_stop_weight;
	const auto end_day = param_set.end_day;
	
	vector<Route> routes_after_swap;

	for (const Route& r : routes) {
		Route r_support = r;
		for (int A = 1; A < r_support.index - 1; A++) {
			for (int B = A + 1; B < r_support.index; B++) {

				if (swap_is_allowed(A, B, r_support)) {
					Route r_new = update_route_after_swap(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
					double cost_route_support = cost_single_route(peso_TW, inter_stop_weight, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					double cost_route_new = cost_single_route(peso_TW, inter_stop_weight, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					
					if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
						int node = sequential_same_node(r_new);
						bool fatto = false;
						while (node != -1) {
							aggregate_same_nodes(r_new, node);
							cost_route_new = cost_single_route(peso_TW, inter_stop_weight, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							fatto = true;
							node = sequential_same_node(r_new);
						}

						if (fatto == false) {
							r_support = r_new;
							A = 1;
							B = A + 1;
						}
						else {
							if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
								r_support = r_new;
								A = 1;
								B = A + 1;
								cost_route_support = cost_route_new;
							}
						}
					}

				}

			}
		}
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
				int Node_min = k;
				double min_weight = r_support.weight[k];
				const int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
				}
				
				if (Node_min >= 0) {
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}

					const double Fuel_before = r_support.quantity_fuel[index_updating_from];
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);

					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_swap.push_back(r_support);
	}

	return routes_after_swap;
}

int move_from(int node, const Route& r) {

	int swap_from = 0;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}
	
	return swap_from;
}

inline int move_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}
	}
	return swap_to;
}

inline bool move_is_allowed(int A, int B, Route& r) {
	bool move = false;
	if (B > A && B < move_to(A, r))move = true;
	if (B < A && move_from(A, r) < B) move = true;
	return move;
}

inline bool move_flightleg_is_allowed(int A, Route& r) {
	bool move = true;
	
	for (Passenger& p : r.passengers_in_route) {
		if ((p.solution_from == A) && (p.solution_to != (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_from != A) && (p.solution_to == (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_to == A) || (p.solution_from == (A + 1))) {
			move = false;
			break;
		}
	}
	return move;
}

inline void update_passenger_to(int A, int B, Passenger& p)
{
	if (p.solution_from == A && A > B) {
		p.solution_from = B + 1;
	}
	else if (p.solution_to == A && A > B) {
		p.solution_to = B + 1;
	}
	else if (p.solution_to == A && A < B) {
		p.solution_to = B;
	}
	else if (p.solution_from == A && A < B) {
		p.solution_from = B;
	}
	else if (p.solution_from <= B && B < p.solution_to && A > p.solution_to) {
		p.solution_to++;
	}
	else if (p.solution_from <= B && B < p.solution_to && A < p.solution_from) {
		p.solution_from--;
	}
	else if (p.solution_from > B && A > p.solution_to) {
		p.solution_from++;
		p.solution_to++;

	}
	else if (p.solution_from < A && A < p.solution_to && B < p.solution_from) {
		p.solution_from++;
	}
	else if (p.solution_from < A && A < p.solution_to && B >= p.solution_to && A < B) {
		p.solution_to--;
	}
	else if (p.solution_from > A && A < p.solution_to && B >= p.solution_to) {
		p.solution_to--;
		p.solution_from--;
	}
}

inline Route create_new_route(int A, int B, const Route& r, map<int, Airplane>& map_airplane)
{
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;

	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	if (B > A) {
		
		for (int i = 1; i < r.index; i++) {
			if ((i < A) || (i > B)) {
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if ((i >= A) && (i < B)) {
				r_new.addPlace(r.places[i + 1], r.refueling[i + 1], r.quantity_fuel[i + 1], 0.0, 0, r.time_arr[i + 1], r.time_dep[i + 1]);
			}
			else if (i == B) {
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
		}
	}
	else {
		for (int i = 1; i < r.index; i++) {
			if (i < (B + 1)) {
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if (i == (B + 1)) {
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
			else if (i >= A + 1) {
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else {
				r_new.addPlace(r.places[i - 1], r.refueling[i - 1], r.quantity_fuel[i - 1], 0.0, 0, r.time_arr[i - 1], r.time_dep[i - 1]);
			}
		}
	}

	return r_new;
}

inline Route update_route_after_move(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double2DVector& from_to, double3DVector& from_to_FuelConsumed) {
	Route r_new = create_new_route(A, B, r, map_airplane);

	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			if (r_new.refueling[i]) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
			}
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}

	for (Passenger p : r.passengers_in_route) {
		update_passenger_to(A, B, p);
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
	}
	
	for (int i = 0; i < r_new.index; i++) {
		if (r_new.weight[i] < 0) {
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					break;
				}
			}
			
			double Update_value = r_new.weight[i];
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			r_new.weight[index_refueling] -= r_new.weight[i];
			
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					r_new.weight[j] -= Update_value;
				}
			}
		}
	}
	
	return r_new;
}

void update_fuel_on_non_maximum(map<int, Airplane>& map_airplane, Route& r_support, int k)
{
	int Node_min = k;
	double min_weight = r_support.weight[k];
	int index_updating_from = k;
	int index_updating_to = r_support.index;  //qua prima c'era -1
	for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
		if (r_support.refueling[i]) break;
		if (r_support.weight[i] < min_weight) {
			min_weight = r_support.weight[i];
			Node_min = i;
		}
		//}
	}
				
	if (Node_min >= 0) {
		for (int i = k + 1; i < r_support.index; i++) {
			if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
				index_updating_to = i;
				break;
			}
		}
					
		double Fuel_before = r_support.quantity_fuel[index_updating_from];
		r_support.quantity_fuel[index_updating_from] = std::min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					
		r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
		for (int i = index_updating_from + 1; i < index_updating_to; i++) {
			r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
			r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
		}
	}
}

vector <Route> move(MoveParamSet param_set, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double2DVector& from_to, double2DVector& location_fuel, double3DVector& from_to_FuelConsumed) {

	const auto peso_TW = param_set.peso_TW;
	const auto inter_stop_weight = param_set.inter_stop_weight;
	const auto end_day = param_set.end_day;
	vector<Route> routes_after_move;

	for (const Route& r : routes) {
		Route r_support = r;
		for (int A = 1; A < r_support.index; A++) {
			for (int B = 1; B < r_support.index; B++) {
				if (A != B && B != (A - 1) && B != (A - 2)) {    //Il caso B != (A-1) || B!= (A-2) sono casi che valutiamo gia quando sposriamo avanti
					if (move_is_allowed(A, B, r_support)) {
						Route r_new = update_route_after_move(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
						
						double cost_route_support = cost_single_route(peso_TW, inter_stop_weight, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						double cost_route_new = cost_single_route(peso_TW, inter_stop_weight, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
							int node = sequential_same_node(r_new);
							
							bool fatto = false;
							int num_aggregazioni = 0;
							while (node != -1) {
								aggregate_same_nodes(r_new, node);
								if (A > node) num_aggregazioni++;
								
								cost_route_new = cost_single_route(peso_TW, inter_stop_weight, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								node = sequential_same_node(r_new);
								fatto = true;
							}

							if (fatto == false) r_support = r_new;
							else {
								if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
									r_support = r_new;
									cost_route_support = cost_route_new;
									A -= num_aggregazioni;
									if (A <= 0) A = 1;
								}
							}
						}
					}
				}

			}
		}
		
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
				update_fuel_on_non_maximum(map_airplane, r_support, k);
			}

		}
		routes_after_move.push_back(r_support);
	}

	return routes_after_move;
}



vector <Route> inter_move(MoveParamSet param_set, vector<Route> routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double2DVector& from_to, double2DVector& location_fuel, double3DVector& from_to_FuelConsumed) {

	const auto peso_TW = param_set.peso_TW;
	const auto inter_stop_weight = param_set.inter_stop_weight;
	const auto end_day = param_set.end_day;
	
	vector<Route> routes_after_move;
	int n_route = -1;

	for (int r = 0; r < routes.size(); r++) {
		n_route += 1;
		vector<Route> routes_destroyed;

		for (const Route& s : routes) {
			if (s.aircraft_code != routes[r].aircraft_code) routes_destroyed.push_back(s);
		}
		Route r_support = routes[r];

		for (int A = 1; A < (r_support.index - 1); A++) {
			vector<Route> solution_rebuilt;
			vector<Passenger> passenger_removed;
			
			if (move_flightleg_is_allowed(A, r_support)) {
				Route r_new = r_support;
				int NomeA1 = r_new.places[A + 1];

				destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);

				if (r_new.index != -1) {
					
					if (r_new.index != -1) {
						solution_rebuilt = repair_one_inter_move(peso_TW, inter_stop_weight, end_day, routes_destroyed, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);

						if (!solution_rebuilt.empty()) {
							
							solution_rebuilt.push_back(r_new);

							double before = calculate_ObjectiveFunction(peso_TW, inter_stop_weight, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed); // Qui non va bene devi considerare che dopo un primo miglioramneto cambi la route
							double after = calculate_ObjectiveFunction(peso_TW, inter_stop_weight, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (before > after) {
								
								int node = sequential_same_node(solution_rebuilt.back());
								
								bool fatto = false;
								int num_aggregazioni = 0;
								while (node != -1) {
									aggregate_same_nodes_inter_ls(solution_rebuilt.back(), node);
									if (A > node) num_aggregazioni++;
									after = calculate_ObjectiveFunction(peso_TW, inter_stop_weight, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									node = sequential_same_node(solution_rebuilt.back());
									fatto = true;
								}

								if (fatto == false) {
									r_support = r_new;
									routes_after_move = solution_rebuilt;
									routes = solution_rebuilt;

									routes_destroyed.clear();

									for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
									A = 1;
									r--;
									if (A >= (r_support.index - 1)) {
										//cout << " caso Loop " << endl;
									}
								}
								else {
									if (before > after) {
										r_support = r_new;
										routes_after_move = solution_rebuilt;
										routes = solution_rebuilt;
										routes_destroyed.clear();
										for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
										A = 1;
										r--;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for (Route r_support : routes_after_move) {
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
				}
				
				if (Node_min >= 0) {
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
			
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
	}
	if (routes_after_move.size() == 0) return routes;
	else {
		double cost_before_move = calculate_ObjectiveFunction(peso_TW, inter_stop_weight, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		double cost_after_move = calculate_ObjectiveFunction(peso_TW, inter_stop_weight, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed); 
		if ( cost_before_move != cost_after_move) {
			cout << " Costo Routes: " << cost_before_move << endl;
			cout << " Costo routes_after_move: " << cost_after_move << endl;
		}
		return routes_after_move;
	}
}


#endif
