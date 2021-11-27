
#include "Evaluate.h"
#include "Feasibility.h"
#include "Move.h"
#include "Route.h"



int swap_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}

	}


	return swap_to;
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


bool swap_is_allowed(int A, int B, const Route& r) {
	bool swap = false;
	if (B < swap_to(A, r) && swap_from(B, r) < A) swap = true;
	return swap;
}

//qua purtroppo non posso mettere usare i puntatori
Route update_route_after_swap(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;

	//cout << "sto inserendo i vari posti nella nuova route" << endl;
	r_new.addPlace(r.airstrips[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.arrival[0], r.departure[0]);
	for (int i = 1; i < r.index; i++) {
		if (i == A) {
			//in questo posto ci devo mettere B
			r_new.addPlace(r.airstrips[B], r.refueling[B], r.fuel[B], 0.0, 0, r.arrival[B], r.departure[B]);
		}
		else if (i == B) {
			//in questo posto ci devo mettere A
			r_new.addPlace(r.airstrips[A], r.refueling[A], r.fuel[A], 0.0, 0, r.arrival[A], r.departure[A]);
		}
		else {
			//in questo posto ci devo mettere normalmente i
			r_new.addPlace(r.airstrips[i], r.refueling[i], r.fuel[i], 0.0, 0, r.arrival[i], r.departure[i]);
		}
	}

	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua ? come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			r_new.arrival[i] = r_new.departure[i - 1] + (((from_to[r_new.airstrips[i - 1]][r_new.airstrips[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.departure[i] = r_new.arrival[i] + map_airstrip[r_new.airstrips[i]].ground_time;

			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.airstrips[i - 1]][r_new.airstrips[i]];

			if (r_new.refueling[i]) {
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

	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri spostati
	for (Passenger p : r.passengers_in_route) {
		if (p.solution_from == A) p.solution_from = B;
		else if (p.solution_to == A) p.solution_to = B;
		else if (p.solution_from == B) p.solution_from = A;
		else if (p.solution_to == B) p.solution_to = A;
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacities[t] += p.capacity;
			r_new.weights[t] -= p.weight;
		}
	}

	//aggiorno fuel se il peso ? negatico
	for (int i = 0; i < r_new.index; i++) {
		if (r_new.weights[i] < 0) {
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					break;
				}
			}
			double Update_value = r_new.weights[i];
			r_new.fuel[index_refueling] += r_new.weights[i];
			r_new.weights[index_refueling] -= r_new.weights[i];

			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.fuel[j] += Update_value;
					r_new.weights[j] -= Update_value;
				}
			}
		}
	}

	return r_new;
}

vector <Route> swap(ProcessedInput* input, const PenaltyWeights& penalty_weights, vector<Route>& routes, double end_day) {
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();


	vector<Route> routes_after_swap;

	for (const Route& r : routes) {
		Route r_support = r;
		for (int A = 1; A < r_support.index - 1; A++) {
			for (int B = A + 1; B < r_support.index; B++) {

				if (swap_is_allowed(A, B, r_support)) {
					Route r_new = update_route_after_swap(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
					double cost_route_support = cost_single_route(input, penalty_weights, r_support);
					double cost_route_new = cost_single_route(input, penalty_weights, r_new);
					
					if (cost_route_support > cost_route_new && route_feasible(input, r_new, end_day)) {
						int node = sequential_same_node(r_new);
						bool fatto = false;
						while (node != -1) {

							aggregate_same_nodes(r_new, node);
							cost_route_new = cost_single_route(input, penalty_weights, r_new);

							fatto = true;
							node = sequential_same_node(r_new);
						}

						if (fatto == false) {
							r_support = r_new;
							A = 1;
							B = A + 1;
						}
						else {
							if (cost_route_support > cost_route_new && route_feasible(input, r_new, end_day)) {
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
			if (r_support.refueling[k] && r_support.fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weights[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {

					if (r_support.refueling[i]) break;
					if (r_support.weights[i] < min_weight) {
						min_weight = r_support.weights[i];
						Node_min = i;
					}
					//}
				}

				if (Node_min >= 0) {

					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
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
		routes_after_swap.push_back(r_support);
	}

	return routes_after_swap;
}





