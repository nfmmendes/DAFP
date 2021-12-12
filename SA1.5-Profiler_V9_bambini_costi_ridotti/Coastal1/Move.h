#ifndef MOVE_H
#define MOVE_H

#include "Aggregate.h"
#include "Evaluate.h"
#include "Repair.h"



int sequential_same_node(const Route& r) {
	int node = -1;
	for (int i = 0; i < ((int)r.airstrips.size()) - 1; i++) {
		if (r.airstrips[i] == r.airstrips[i + 1]) {
			node = i;
			break;
		}
	}
	return node;
}

int move_from(int node, const Route& r) {

	int swap_from = 0;

	for ( auto& p : r.get_passengers()) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}


	return swap_from;

}

int move_to(int node, const Route& r) {
	int swap_to = 1000;

	for ( auto & p : r.get_passengers()) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) 
				swap_to = p.solution_to;
		}
	}

	return swap_to;
}

bool move_is_allowed(int A, int B, Route& r) {
	bool move = false;
	if (B > A && B < move_to(A, r))move = true;
	if (B < A && move_from(A, r) < B) move = true;
	return move;
}

bool move_flightleg_is_allowed(int A, Route& r) {
	bool move = true;
	
	for (auto& p : r.get_passengers()) {

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

void update_passenger_solution_to_from(const int &A, const int &B, Passenger& p)
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

void add_new_place(int A, int B, const Route& r, Route &r_new)
{
	if (B > A) {
		for (int i = 1; i < r.index; i++) {
			if ((i < A) || (i > B)) {
				// Caso in cui rimane come prima
				r_new.addPlace(r.airstrips[i], r.get_refueling()[i], r.fuel[i], 0.0, 0, r.get_arrivals()[i], r.get_departures()[i]);
			}
			else if ((i >= A) && (i < B)) {
				r_new.addPlace(r.airstrips[i + 1], r.get_refueling()[i + 1], r.fuel[i + 1], 0.0, 0, r.get_arrivals()[i + 1], r.get_departures()[i + 1]);
			}
			else if (i == B) {
				r_new.addPlace(r.airstrips[A], r.get_refueling()[A], r.fuel[A], 0.0, 0, r.get_arrivals()[A], r.get_departures()[A]);
			}
		}
	}
	else {
		for (int i = 1; i < r.index; i++) {
			if (i < (B + 1)) {

				//in questo posto ci devo mettere normalmente i
				r_new.addPlace(r.airstrips[i], r.get_refueling()[i], r.fuel[i], 0.0, 0, r.get_arrivals()[i], r.get_departures()[i]);
			}
			else if (i == (B + 1)) {
				//in questo posto ci devo mettere A
				r_new.addPlace(r.airstrips[A], r.get_refueling()[A], r.fuel[A], 0.0, 0, r.get_arrivals()[A], r.get_departures()[A]);
			}
			else if (i >= A + 1) {
				//in questo posto ci devo mettere normalmente i-1
				r_new.addPlace(r.airstrips[i], r.get_refueling()[i], r.fuel[i], 0.0, 0, r.get_arrivals()[i], r.get_departures()[i]);
			}
			else {
				r_new.addPlace(r.airstrips[i - 1], r.get_refueling()[i - 1], r.fuel[i - 1], 0.0, 0, r.get_arrivals()[i - 1], r.get_departures()[i - 1]);
			}
		}
	}
}

Route update_route_after_move(ProcessedInput*input, int A, int B, Route& r) {
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;

	r_new.addPlace(r.airstrips[0], r.get_refueling()[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.get_arrivals()[0], r.get_departures()[0]);
	add_new_place(A, B, r, r_new);

	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua ? come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {

			r_new.get_arrival_at(i) = r_new.get_departures()[i - 1] + (((from_to[r_new.airstrips[i - 1]][r_new.airstrips[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.get_departure_at(i) = r_new.get_arrivals()[i] + map_airstrip[r_new.airstrips[i]].ground_time;

			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.airstrips[i - 1]][r_new.airstrips[i]];

			if (r_new.get_refueling()[i]) {
				r_new.fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.fuel[i] = r_new.fuel[i - 1] - fuel_consumed;
			}
			r_new.weights[i] = map_airplane[r_new.aircraft_code].load_weight - r_new.fuel[i];
		}
		else {
			r_new.fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weights[i] = map_airplane[r_new.aircraft_code].load_weight - r_new.fuel[i];
		}
	}

	for(int i=0; i < r.get_passengers().size(); i++){
		Passenger& p = r.get_passenger(i);
		update_passenger_solution_to_from(A, B, p);
		r_new.add_passenger(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.add_capacity_at(t, p.capacity);
			r_new.weights[t] -= p.weight;
		}
	}

	//aggiorno fuel se il peso ? negatico */
	for (int i = 0; i < r_new.index; i++) {
		if (r_new.weights[i] < 0) {
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.get_refueling()[t]) {
					index_refueling = t;
					break;
				}
			}
			double Update_value = r_new.weights[i];
			r_new.fuel[index_refueling] += r_new.weights[i];
			r_new.weights[index_refueling] -= r_new.weights[i];
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.get_refueling()[j]) break;
				else {
					r_new.fuel[j] += Update_value;
					r_new.weights[j] -= Update_value;
				}
			}
		}
	}

	return r_new;
}

void modify_fuel_when_non_max(map<int, Airplane>& map_airplane, Route &r_support)
{
	for (int k = 0; k < r_support.index; k++) {
		if (r_support.get_refueling()[k] && r_support.fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy

			int Node_min = k;
			double min_weight = r_support.weights[k];
			int index_updating_from = k;
			int index_updating_to = r_support.index;  //qua prima c'era -1
			for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
				if (r_support.get_refueling()[i]) break;
				if (r_support.weights[i] < min_weight) {
					min_weight = r_support.weights[i];
					Node_min = i;
				}
			}

			if (Node_min >= 0) {

				for (int i = k + 1; i < r_support.index; i++) {
					if (r_support.get_refueling()[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
						index_updating_to = i;
						break;
					}
				}

				double Fuel_before = r_support.fuel[index_updating_from];
				r_support.fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.fuel[index_updating_from] + min_weight);
				r_support.weights[index_updating_from] -= (r_support.fuel[index_updating_from] - Fuel_before);

				for (int i = index_updating_from + 1; i < index_updating_to; i++) {
					r_support.fuel[i] += (r_support.fuel[index_updating_from] - Fuel_before);
					r_support.weights[i] -= (r_support.fuel[index_updating_from] - Fuel_before);
				}
			}
		}

	}
}

vector <Route> move(ProcessedInput* input, const PenaltyWeights& penalty_weights, vector<Route>& routes, double end_day){
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	vector<Route> routes_after_move;

	for (const Route& r : routes) {
		//r.print();
		Route r_support = r;
		for (int A = 1; A < r_support.index; A++) {
			// we have to move the Node A form its position to th enext after B
			for (int B = 1; B < r_support.index; B++) {
				if (A != B && B != (A - 1) && B != (A - 2)) {    //Il caso B != (A-1) || B!= (A-2) sono casi che valutiamo gia quando sposriamo avanti
					if (move_is_allowed(A, B, r_support)) {
						Route r_new = update_route_after_move(input, A, B, r_support);

						double cost_route_support = cost_single_route(input, penalty_weights, r_support);
						double cost_route_new = cost_single_route(input, penalty_weights, r_new);

						if (cost_route_support > cost_route_new && route_feasible(input, r_new, end_day)) {

							int node = sequential_same_node(r_new);

							bool fatto = false;
							int num_aggregazioni = 0;
							while (node != -1) {

								aggregate_same_nodes(r_new, node);
								if (A > node) num_aggregazioni++;

								cost_route_new = cost_single_route(input, penalty_weights, r_new);
								node = sequential_same_node(r_new);
								fatto = true;
							}

							if (fatto == false) r_support = r_new;
							else {
								if (cost_route_support > cost_route_new && route_feasible(input, r_new, end_day)) {
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
		modify_fuel_when_non_max(map_airplane, r_support);
		routes_after_move.push_back(r_support);
	}

	return routes_after_move;
}

void update_fuel_when_no_max_2(map<int, Airplane>& map_airplane, Route r_support)
{
	for (int k = 0; k < r_support.index; k++) {
		if (r_support.get_refueling()[k] && r_support.fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			int Node_min = k;
			double min_weight = r_support.weights[k];
			int index_updating_from = k;
			int index_updating_to = r_support.index;  //qua prima c'era -1
			for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
				if (r_support.get_refueling()[i])
					break;
					
				if (r_support.weights[i] < min_weight) {
					min_weight = r_support.weights[i];
					Node_min = i;
				}
			}
				
			if (Node_min >= 0) {
				for (int i = k + 1; i < r_support.index; i++) {
					if (r_support.get_refueling()[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
						index_updating_to = i;
						break;
					}
				}
					
				double Fuel_before = r_support.fuel[index_updating_from];					
				r_support.fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.fuel[index_updating_from] + min_weight);
					
				r_support.weights[index_updating_from] -= (r_support.fuel[index_updating_from] - Fuel_before);
					
				for (int i = index_updating_from + 1; i < index_updating_to; i++) {
					r_support.fuel[i] += (r_support.fuel[index_updating_from] - Fuel_before);
					r_support.weights[i] -= (r_support.fuel[index_updating_from] - Fuel_before);
				}
			}
		}

	}
}

vector <Route> inter_move(ProcessedInput* input, const PenaltyWeights& penalty_weights, vector<Route> routes, double end_day) {
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

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
			// Solution Rebuilt ? come se fosse il nostro best improvement
			vector<Route> solution_rebuilt;
			vector<Passenger> passenger_removed;
			
			// Ora devo eliminare i nodi solo se sono allowed
			if (move_flightleg_is_allowed(A, r_support)) {
				Route r_new = r_support;
				int NomeA1 = r_new.airstrips[A + 1];

				destroy_ls(input, n_route, A, passenger_removed, r_new);
				if (r_new.index != -1) {
					if (r_new.index != -1) {
						solution_rebuilt = repair_one_inter_move(input, penalty_weights, end_day, routes_destroyed, passenger_removed);

						if (solution_rebuilt.size() != 0) {
							solution_rebuilt.push_back(r_new);
							double before = calculate_ObjectiveFunction(input, penalty_weights, routes); // Qui non va bene devi considerare che dopo un primo miglioramneto cambi la route
							double after = calculate_ObjectiveFunction(input, penalty_weights, solution_rebuilt);
							if (before > after) {
								// Qui sto usando tutto solution_rebuilt.back() ma in realta potrei usare r_new e poi un volta che la ho istemanta switcharla con solution_rebuilt.back()
								int node = sequential_same_node(solution_rebuilt.back());
								bool fatto = false;
								int num_aggregazioni = 0;
								while (node != -1) {
									aggregate_same_nodes_inter_ls(solution_rebuilt.back(), node);
									if (A > node) 
										num_aggregazioni++;

									after = calculate_ObjectiveFunction(input, penalty_weights, solution_rebuilt);
									
									node = sequential_same_node(solution_rebuilt.back());
									fatto = true;
								}

								if (fatto == false) {
									r_support = r_new;
									routes_after_move = solution_rebuilt;
									routes = solution_rebuilt;
									// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
									routes_destroyed.clear();
									//routes_destroyed.shrink_to_fit();
									for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
									A = 1;
									r--;
								}
								else {
									if (before > after) {
										r_support = r_new;
										routes_after_move = solution_rebuilt;
										routes = solution_rebuilt;
										// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
										routes_destroyed.clear();
										for (int s = 0; s < solution_rebuilt.size() - 1; s++) 
											routes_destroyed.push_back(solution_rebuilt[s]);
										
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
		//Modifico Fuel quando non massimo
		update_fuel_when_no_max_2(map_airplane, r_support);
	}
	if (routes_after_move.empty()) 
		return routes;
	else {
		double before = calculate_ObjectiveFunction(input, penalty_weights, routes);
		double after = calculate_ObjectiveFunction(input,penalty_weights, routes_after_move); 
		if (before != after) {
			cout << " Costo Routes: " << before << endl;
			cout << " Costo routes_after_move: " << after << endl;
		}
		return routes_after_move;
	}
}

#endif