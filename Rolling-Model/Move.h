#ifndef MOVE_H
#define MOVE_H

#include <algorithm>
#include <stdlib.h>


#include "Destroyers.h"
#include "Evaluate.h"
#include "Feasibility.h"
#include "Repair.h"

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
Route update_route_after_swap(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
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
	//cout << "ho finito di inserire i vari posti nella nuova route" << endl;


	//cout << "aggiorno i tempi e il fuel" << endl;
	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua � come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			//cout << "from: " << r_new.places[i - 1] << endl;
			//cout << "to: " << r_new.places[i] << endl;
			//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			//cout << "fuel consumed: " << fuel_consumed << endl;

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
	//cout << "ho finito di aggiornare i tempi e il fuel" << endl;

	//cout << "aggiorno i i passeggeri" << endl;
	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri spostati
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
	//cout << "finito" << endl;

	//cout << "aggiorno il fuel se peso negativo" << endl;
	//aggiorno fuel se il peso � negatico
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			//	cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					//	cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			//cout << " r_new.quantity_fuel[index_refueling] : " << r_new.quantity_fuel[index_refueling] << " +=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			//cout << " r_new.weight[index_refueling] : " << r_new.weight[index_refueling] << " -=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.weight[index_refueling] -= r_new.weight[i];
			//cout << " Adesso tolgo ai nodi senza possibilit� di fare refuel successvi " << endl;
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					//cout << " Dopo Fuel era = " << r_new.quantity_fuel[j] << endl;
					//cout << " Prima Weight era = " << r_new.weight[j] << endl;
					r_new.weight[j] -= Update_value;
					//cout << " Dopo Weight era = " << r_new.weight[j] << endl;
				}
			}

			//r_new.weight[i] = 0;

		}
	}



	return r_new;
}

vector <Route> swap(double peso_TW, double peso_intermediate_stop, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_swap;

	for (const Route& r : routes) {
		//cout << " sto provando a fare lo swap in questa route: " << endl;
		//r.print();
		Route r_support = r;
		for (int A = 1; A < r_support.index - 1; A++) {
			for (int B = A + 1; B < r_support.index; B++) {

				if (swap_is_allowed(A, B, r_support)) {
					//cout << "******************************************************Sto provando a fare lo swap fra " << A << " e " << B << endl;
					Route r_new = update_route_after_swap(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
					//cout << "ho finito l'update" << endl;
					//r_new.print();
					double cost_route_support = cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					double cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					//cout << cost_route_support << " c_supp > c_new " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to) << endl;
					if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
						//cout << " OK ! dopo move Migliora ------- Ma prima devo controllare che non sia con nodi doppi " << endl;
						int node = sequential_same_node(r_new);
						bool fatto = false;
						while (node != -1) {
							/*
							cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
							r_new.print();
							// i have two airports close to each other
							cout << " Nodo dove inizia coppia --> " << node << endl;
							system("pause");
							*/
							aggregate_same_nodes(r_new, node);
							//r_new.print();
							cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to) << endl;

							fatto = true;
							node = sequential_same_node(r_new);
						}
						//cout << " Ok non aveva nodi doppi ! " << endl;
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
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
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

int move_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
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
	//cout << " Sono dentro al controllo per sapere se � consentito " << endl;
	//cout << " sto guardando la route" << r.aircraft_code << " con il nodo " << A << endl;
	for (Passenger& p : r.passengers_in_route) {
		//cout << " sto facendo il passeggiero: ";
		//p.print();
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
		//cout << " Move = " << move << endl;
	}
	return move;
}

Route update_route_after_move(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;
	/*cout << " Questa � la vecchia routes: " << endl;
	//r.print();
	*/
	//cout << "dentro a route after move -- A -> " << A << " B --> " << B << endl;
	//cout << "sto inserendo i vari posti nella nuova route" << endl;

	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	if (B > A) {
		//cout << " Sono nel caso in cui B > A " << endl;
		for (int i = 1; i < r.index; i++) {
			//cout << " Sono nel indice i --> " << i << endl;
			if ((i < A) || (i > B)) {
				//cout << " caso (i < A) || (i > B) in questo casorimane come prima devo mettere normalmente quello che c'� in i " << endl;
				//cout << r.places[i] << endl;
				// Caso in cui rimane come prima
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if ((i >= A) && (i < B)) {
				//cout << " caso (i >= A) && (i < B) in questo caso devo metterquello che c'� in i+1 " << endl;
				//cout << r.places[i+1] << endl;
				//cout << "stampo nel caso indicato dal bed allocator i+1" << endl;
				//cout << "i+1: " << i+1 << " r.places[i+1]: " << r.places[i+1] << endl;
				r_new.addPlace(r.places[i + 1], r.refueling[i + 1], r.quantity_fuel[i + 1], 0.0, 0, r.time_arr[i + 1], r.time_dep[i + 1]);
			}
			else if (i == B) {
				//cout << " caso i = (B) in questo caso devo mettere normalmente A " << endl;
				//cout << r.places[A] << endl;
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
		}
	}
	else {
		//cout << " Sono nel caso in cui B < A " << endl;
		for (int i = 1; i < r.index; i++) {
			//cout << " Sono nel indice i --> " << i << endl;
			if (i < (B + 1)) {
				//cout << " caso i < (B + 1) in questo caso devo mettere normalmente quello che c'� in i " << endl;
				//cout << r.places[i] << endl;
				//in questo posto ci devo mettere normalmente i
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if (i == (B + 1)) {
				//cout << " caso i = (B + 1) in questo caso devo mettere normalmente A " << endl;
				//cout << r.places[A] << endl;
				//in questo posto ci devo mettere A
				//cout << "stampo nel caso indicato dal bed allocator A" << endl;
				//cout << "ecco l'index: " << r.index << endl;
				//cout << "indice i: " << i << endl;
				//cout << "A: " << A << " r.places[A]: " << r.places[A] << endl;
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
			else if (i >= A + 1) {
				//in questo posto ci devo mettere normalmente i-1
				//cout << " caso (i >= A +1) in questo caso devo mettere normalmente quello che c'� in i" << endl;
				//cout << r.places[i-1] << endl;
				//cout << "stampo nel caso indicato dal bed allocator" << endl;
				//cout << "i: " << i << " r.places[i]: " << r.places[i] << endl;
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else {
				//cout << " caso i > (B + 1) in questo caso devo mettere normalmente quello che c'� in i - 1" << endl;
				//cout << r.places[i - 1] << endl;
				r_new.addPlace(r.places[i - 1], r.refueling[i - 1], r.quantity_fuel[i - 1], 0.0, 0, r.time_arr[i - 1], r.time_dep[i - 1]);
			}
		}
	}
	//cout << "ho finito di inserire i vari posti nella nuova route" << endl;
	//r_new.print();
	//cout << "aggiorno i tempi e il fuel" << endl;
	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua � come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			//cout << "from: " << r_new.places[i - 1] << endl;
			//cout << "to: " << r_new.places[i] << endl;
			//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];


			//cout << "fuel consumed: " << fuel_consumed << endl;

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
	//cout << "ho finito di aggiornare i tempi e il fuel" << endl;
	//r_new.print();
	//cout << "aggiorno i i passeggeri" << endl;
	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri 
	//cout << " A -> " << A << " B --> " << B << endl;
	for (Passenger p : r.passengers_in_route) {
		//cout << " Ho preso il passeggiero: " << endl;
		//cout << " Nome : " << p.name << " che ha from: " << p.solution_to << " e to: " << p.solution_to << endl;
		if (p.solution_from == A && A > B) {
			//cout << " Modifico from = B+1 pperche A > B " << endl;
			p.solution_from = B + 1;
		}
		else if (p.solution_to == A && A > B) {
			//cout << " Modifico to = B+1 pperche A > B " << endl;
			p.solution_to = B + 1;
		}
		else if (p.solution_to == A && A < B) {
			//cout << " Modifico to = B perch� A < B " << endl;
			p.solution_to = B;
		}
		else if (p.solution_from == A && A < B) {
			//cout << " Modifico from = B perch� A < B  " << endl;
			p.solution_from = B;
		}
		else if (p.solution_from <= B && B < p.solution_to && A > p.solution_to) {
			//cout << " Incremento il to di 1  caso A > B" << endl;
			p.solution_to++;
		}
		else if (p.solution_from <= B && B < p.solution_to && A < p.solution_from) {
			//cout << " Decremento il from di 1  caso A < B" << endl;
			p.solution_from--;
		}
		else if (p.solution_from > B && A > p.solution_to) {
			//cout << " Terza casistica A era dopo richiesta adesso va orima" << endl;
			p.solution_from++;
			p.solution_to++;

		}
		else if (p.solution_from < A && A < p.solution_to && B < p.solution_from) {
			//cout << " Quarta casistica A" << endl;
			p.solution_from++;
		}
		else if (p.solution_from < A && A < p.solution_to && B >= p.solution_to && A < B) {
			//cout << " Quarta casistica B" << endl;
			p.solution_to--;
		}
		else if (p.solution_from > A && A < p.solution_to && B >= p.solution_to) {
			//cout << " Quinta casistica Nodo va da prima della richiesta a dopo" << endl;
			p.solution_to--;
			p.solution_from--;
		}
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
		//cout << " Nome : " << p.name << " che ha from AGGIORNATO: " << p.solution_from << " e to AGGIORNATO: " << p.solution_to << endl;
	}
	/*cout << "finito" << endl;
	r_new.print();
	cout << "aggiorno il fuel se peso negativo" << endl;
	//aggiorno fuel se il peso � negatico */
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			//	cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					//	cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			//cout << " r_new.quantity_fuel[index_refueling] : " << r_new.quantity_fuel[index_refueling] << " +=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			//cout << " r_new.weight[index_refueling] : " << r_new.weight[index_refueling] << " -=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.weight[index_refueling] -= r_new.weight[i];
			//cout << " Adesso tolgo ai nodi senza possibilit� di fare refuel successvi " << endl;
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					//cout << " Dopo Fuel era = " << r_new.quantity_fuel[j] << endl;
					//cout << " Prima Weight era = " << r_new.weight[j] << endl;
					r_new.weight[j] -= Update_value;
					//cout << " Dopo Weight era = " << r_new.weight[j] << endl;
				}
			}

			//r_new.weight[i] = 0;

		}
	}
	//cout << "finito" << endl;

	//stampo_caso_strano_single(r, from_to, map_airplane);

	return r_new;
}

vector <Route> move(double peso_TW, double peso_intermediate_stop, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;

	for (const Route& r : routes) {
		//cout << " sto provando a fare il move in questa route: " <<r.aircraft_code <<  endl;
		//r.print();
		Route r_support = r;
		for (int A = 1; A < r_support.index; A++) {
			//cout << " Considero di spostare nodo -> " << A << endl;
			// we have to move the Node A form its position to th enext after B
			for (int B = 1; B < r_support.index; B++) {
				if (A != B && B != (A - 1) && B != (A - 2)) {    //Il caso B != (A-1) || B!= (A-2) sono casi che valutiamo gia quando sposriamo avanti
				//cout << " Nella posizione -> " << B << endl;
					if (move_is_allowed(A, B, r_support)) {
						//cout << " CONSENTITO sono nel main -----> provo a fare move da " << A << " e " << B << endl;
						Route r_new = update_route_after_move(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
						//cout << "ho finito l'update" << endl;
						//r_new.print();
						double cost_route_support = cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						double cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						//cout << " Deve valere che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to,location_fuel) << endl;
						if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
							//cout << " OK ! dopo move Migliora ------- Ma prima devo controllare che non sia con nodi doppi " << endl;
							int node = sequential_same_node(r_new);
							//cout << "node fuori dal while: " << node << endl;
							//r_new.print();
							bool fatto = false;
							int num_aggregazioni = 0;
							while (node != -1) {
								/* cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
								//r_new.print();
								// i have two airports close to each other
								cout << " Nodo dove inizia coppia --> " << node << endl;
								system("pause");
								*/
								//cout << "******* Aggrego nodo in posizione **********" << node << endl;
								aggregate_same_nodes(r_new, node);
								if (A > node) num_aggregazioni++;
								//cout << " Ho finito codice di aggregazione " << endl;
								cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to, location_fuel) << endl;
								//system("pause");
								node = sequential_same_node(r_new);
								//cout << "node dentro al while: " << node << endl;
								//r_new.print();
								fatto = true;
							}

							//cout << " Ok non aveva nodi doppi ! " << endl;
							if (fatto == false) r_support = r_new;
							else {
								if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
									r_support = r_new;
									cost_route_support = cost_route_new;
									//cout << " Ho finito while di aggregazione ma devo aggiornare A tolgo " << num_aggregazioni << endl;
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
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = std::min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_move.push_back(r_support);
	}

	return routes_after_move;
}



vector <Route> inter_move(double peso_TW, double peso_intermediate_stop, vector<Route> routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;
	int n_route = -1;
	//cout << " Stampo le route prima di cominciare " << endl;
	//for (Route r : routes) r.print();
	//cout << "************************************************** INCOMINCIO ********************************************************************************************* " << endl;
	//for (Route r : routes) {
	/*int t = 0;
	for (auto x : routes) t += x.passengers_in_route.size();
	cout << "ecco i passeggeri che arrivano in input dalle routes: " << t << endl;
	*/

	for (int r = 0; r < routes.size(); r++) {
		n_route += 1;
		vector<Route> routes_destroyed;



		//cout << " Adesso anallizzo la route numero " << r << " dell'aereo " << routes[r].aircraft_code << endl;
		//Route distrutte sono uguali tranne quella a cui sto togliendo la coppia
		//for (Route s : routes) if (s.aircraft_code != r.aircraft_code) routes_destroyed.push_back(s);
		for (const Route& s : routes) {
			//cout << "if: " << to_string(s.aircraft_code) << "  ----  " << to_string(routes[r].aircraft_code) << endl;
			if (s.aircraft_code != routes[r].aircraft_code) routes_destroyed.push_back(s);
		}
		Route r_support = routes[r];
		//cout << "r_support ha questi passeggeri: " << r_support.passengers_in_route.size() << endl;


		for (int A = 1; A < (r_support.index - 1); A++) {
			//cout << " Sto guardando Nodo A: " << A << " di " << r_support.aircraft_code << " size: " << r_support.index <<  endl;
			// Solution Rebuilt � come se fosse il nostro best improvement
			vector<Route> solution_rebuilt;
			vector<Passenger> passenger_removed;
			//cout << " Considero di spostare nodo -> " << A << " e nodo " << A+1 << endl;
			// Ora devo eliminare i nodi solo se sono allowed
			if (move_flightleg_is_allowed(A, r_support)) {
				//cout << " CONSENTITO sono nel main -----> provo a fare move di " << A << " e " << A+1 << " da " << r_support.aircraft_code << " un altra route " << endl;
				Route r_new = r_support;
				//cout << " La route adesso ha numero di nodi: " << r_new.index << endl;
				int NomeA1 = r_new.places[A + 1];


				destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);

				//cout << " La route adesso ha numero di nodi: " << r_new.index << " con numero di passeggieri rimossi " << passenger_removed.size() << endl;
				//cout << " Stampo la  route distrutta " << endl;
				//r_new.print();
				if (r_new.index != -1) {
					/*if (A != (r_new.index - 1) && r_new.places[A] == NomeA1 && r_new.index > 1) {
						//cout << " Ok vado a togliere anche il nodo A " << endl;
						destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);
						//for (Passenger p : passenger_removed) p.print();
						//r_new.print();
					}*/
					if (r_new.index != -1) {


						solution_rebuilt = repair_one_inter_move(peso_TW, peso_intermediate_stop, end_day, routes_destroyed, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);



						if (solution_rebuilt.size() != 0) {
							//int Npass = 0;
							//for (Route p : solution_rebuilt) Npass += p.passengers_in_route.size();
							//cout << " Numero Passeggeri dopo  repair dentro alla ricerca locale riga 6799 : " << Npass << " Sono i passeggieri senza route distrutta " << endl;
							//cout << " Numero Passegger nella route distrutta: " << r_new.passengers_in_route.size() << endl;
							solution_rebuilt.push_back(r_new);





							double before = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed); // Qui non va bene devi considerare che dopo un primo miglioramneto cambi la route
							double after = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (before > after) {
								//cout << " OK ! dopo mossa MIGLIORAAAAAAAAAAAAAAA di " << before <<" - "<<after << "------ Ma prima devo controllare che non sia con nodi doppi " << endl;
								// Qui sto usando tutto solution_rebuilt.back() ma in realta potrei usare r_new e poi un volta che la ho istemanta switcharla con solution_rebuilt.back()
								int node = sequential_same_node(solution_rebuilt.back());
								//cout << "node fuori dal while: " << node << endl;
								//r_new.print();
								bool fatto = false;
								int num_aggregazioni = 0;
								while (node != -1) {
									//cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << solution_rebuilt.back().aircraft_code<< endl;
									//solution_rebuilt.back().print();
									// i have two airports close to each other
									//cout << " Nodo dove inizia coppia --> " << node << endl;
									//if (node == 1) solution_rebuilt.back().print();
									//system("pause");
									//cout << "******* Aggrego nodo in posizione **********" << node << endl;
									//cout << " Numero nodi prima:  " << solution_rebuilt.back().index << endl;
									aggregate_same_nodes_inter_ls(solution_rebuilt.back(), node);
									//cout << " Numero nodi dopo:  " << solution_rebuilt.back().index << endl;
									if (A > node) num_aggregazioni++;
									//cout << " Ho finito codice di aggregazione " << endl;
									after = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to, location_fuel) << endl;
									//system("pause");
									node = sequential_same_node(solution_rebuilt.back());
									//solution_rebuilt.back().print();
									fatto = true;
								}

								if (fatto == false) {
									//cout << " Ok non aveva nodi doppi ! " << endl;
									//cout << " Il for degli A prima andava fino a " << r_support.index  << endl;
									r_support = r_new;
									//cout << " Il for degli A adesso va fino a " << r_support.index  << endl;
									routes_after_move = solution_rebuilt;
									routes = solution_rebuilt;
									// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
									routes_destroyed.clear();
									//routes_destroyed.shrink_to_fit();
									for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
									A = 1;
									r--;
									if (A >= (r_support.index - 1)) {
										//cout << " caso Loop " << endl;
									}
								}
								else {
									if (before > after) {
										// bisognerebbe mettere l'aggiornamento una cosa del tipo
										//cout << " Ok MIGLIORAAAAAAAAAAAAAAA aveva nodi doppi ! " << endl;
										//system("pause");
										//cout << " Il for degli A prima andava fino a " << r_support.index  << endl;
										r_support = r_new;
										//cout << " Il for degli A adesso va fino a " << r_support.index  << endl;
										routes_after_move = solution_rebuilt;
										routes = solution_rebuilt;
										//cout << " Ho finito while di aggregazione ma devo aggiornare A tolgo " << num_aggregazioni << endl;
										// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
										routes_destroyed.clear();
										//routes_destroyed.shrink_to_fit();
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
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
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
		if (calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) != calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
			cout << " Costo Routes: " << calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
			cout << " Costo routes_after_move: " << calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
			//cin.get();
		}
		return routes_after_move;
	}
}


#endif
