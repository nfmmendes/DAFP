//ECCO I NOSTRI DESTROY
#include <set>
#include <unordered_map>


#include "Evaluate.h"
#include "Route.h"
#include "Util.h"



bool relateness_passenger2(int trheshold, vector<vector<double>>& from_to, int idP, Passenger& r, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequenza, string pointer, int y, map<int, Passenger>& map_id_passenger) {
	// Poi questo for si potra togliere
	Passenger pass = map_id_passenger[idP];
	double Relateness = from_to[r.arrival_location][pass.arrival_location] + from_to[r.departure_location][pass.departure_location] + abs(r.arrival_time - pass.arrival_time) + abs(r.departure_time - pass.departure_time);

	if (Relateness <= trheshold) {

		for (int j : agr_pass[pointer]) passengers_removed.push_back(map_id_passenger[j]);
		sequenza.erase(sequenza.begin() + y);
		agr_pass.erase(agr_pass.find(pointer));

		return true;
	}
	else {
		return false;
	}

}


vector<Route> destroy_thanos(double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {


	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		if (f <= destroy_coef_route && r.index > 1) {

			//genero il numero random di nodi da distruggere da 2 a place.size()-1;
			double numero_random = (double)rand() / RAND_MAX;
			if (r.places.size() == 2) {
				numero_random = round(1 + (numero_random * (r.places.size() - 2)));
			}
			else {
				numero_random = round(2 + (numero_random * (r.places.size() - 3)));
			}

			if (numero_random == r.places.size() - 1) {
				// qua devo distruggere tutta la route e lasciare solo il depot
				for (int i = (int)(r.places.size()) - 1; i >= 1; i--) {
					r.places.erase(r.places.begin() + i);
					r.refueling.erase(r.refueling.begin() + i);
					r.quantity_fuel.erase(r.quantity_fuel.begin() + i);
					r.time_arr.erase(r.time_arr.begin() + i);
					r.time_dep.erase(r.time_dep.begin() + i);
					r.capacity.erase(r.capacity.begin() + i);
					r.weight.erase(r.weight.begin() + i);
					r.index--;
				}
				//qua ci dovrebbe essere solo il deposito
				//fisso il peso, capacita e fuel ai valori di default e la variabile passeggeri dentro a false
				r.quantity_fuel[0] = map_airplane[r.aircraft_code].max_fuel;
				r.weight[0] = map_airplane[r.aircraft_code].weight_fuel_people - r.quantity_fuel[0];
				r.capacity[0] = 0;
				r.primo_pass = false;

				//ora devo togliere tutti i passeggeri
				for (int p = r.passengers_in_route.size() - 1; p >= 0; p--) {
					r.passengers_in_route[p].route_before = index;
					passenger_removed.push_back(r.passengers_in_route[p]);
					r.passengers_in_route.erase(r.passengers_in_route.begin() + p);
				}

			}
			else {
				//qua devo pescare un numero di nodi da distruggere pari a numero_random
				int nodi_rimossi = 0; //finch? non ho rimosso 
				do {
					bool check = true;
					do {
						//if i'm here i can destroy the route
						//at the moment i destroy only a node
						int node_destroy = (int)(round(1 + ((double)rand() / RAND_MAX) * (r.index - 2)));

						if (node_destroy == r.index - 1) { //this in the case of that node is at the end
							check = false;

							vector<int> int_removed;

							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t] -= r.passengers_in_route[p].capacity;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								r.passengers_in_route[int_removed[i]].route_before = index;
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}

							int index_min_from = 0;
							for (int i = Min_From_Pass; i >= 0; i--) {
								if (r.refueling[i] && i != node_destroy) {
									index_min_from = i;
									break;
								}
							}

							for (int k = index_min_from; k < Max_To_Pass; k++) {
								if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
									int Node_min = k;
									double min_weight = r.weight[k];
									int index_updating_from = k;
									int index_updating_to = r.index;  //qua prima c'era -1
									for (int i = k + 1; i <= Max_To_Pass; i++) {
										if (r.refueling[i]) break;
										if (r.weight[i] < min_weight && i != node_destroy) {
											min_weight = r.weight[i];
											Node_min = i;
										}
										//}
									}
									if (Node_min >= 0) {
										for (int i = k + 1; i < r.index; i++) {
											if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
												index_updating_to = i;
												break;
											}
										}

										double Fuel_before = r.quantity_fuel[index_updating_from];
										r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
										r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
										for (int i = index_updating_from + 1; i < index_updating_to; i++) {
											if (i != node_destroy) {
												r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
												r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
											}
										}
									}
								}

							}

							int nodi_mancanti = (int)(r.places.size());
							r.removePlace(node_destroy, map_airplane);
							nodi_mancanti -= (int)r.places.size();
							nodi_rimossi += nodi_mancanti;
						}
						else {

							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];

							if (fuel_consumed <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
								check = false;
								vector<int> int_removed;
								int Min_From_Pass = node_destroy;
								int Max_To_Pass = node_destroy;
								for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
									if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
										if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
										if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
										int_removed.push_back(p);
										for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
											r.capacity[t] -= r.passengers_in_route[p].capacity;
											r.weight[t] += r.passengers_in_route[p].weight;
										}

									}
								}

								for (int i = int_removed.size() - 1; i >= 0; i--) {
									r.passengers_in_route[int_removed[i]].route_before = index;
									passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
									vector<Passenger>::iterator it;
									it = r.passengers_in_route.begin();
									r.passengers_in_route.erase(it + int_removed[i]);
								}

								int nodi_mancanti = (int)r.places.size();
								r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time

								int index_before = node_destroy - 1;
								double diff = 0;

								for (int i = node_destroy + 1; i < r.index; i++) {

									if (r.refueling[i]) break;
									if (index_before == (node_destroy - 1)) {

										diff = r.quantity_fuel[i];
										r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
										diff = diff - r.quantity_fuel[i];
									}
									else {
										r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
									}

									if (r.refueling[node_destroy]) {
										r.weight[i] = r.weight[i] + diff;
									}
									else {
										r.weight[i] = r.weight[i] + diff;
									}

									index_before = i + 1;
								}
								//*****************************************************************************************************************************************************************
								r.removePlace(node_destroy, map_airplane);
								nodi_mancanti -= (int)(r.places.size());
								nodi_rimossi += (int)(nodi_mancanti);

								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weight[j] < 0) {
										add_fuel = r.weight[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.refueling[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}

										for (int t = index_refueling; t < r.index; t++) {
											if (r.refueling[t] && t != index_refueling) break;
											r.quantity_fuel[t] += add_fuel;
											r.weight[t] -= add_fuel;
										}
									}
								}
							}
						}
					} while (check);
				} while (nodi_rimossi < numero_random);
			}

		}
		index++;
	}
	return solution;
}

vector<Route> destroy_casual(double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		if (f <= destroy_coef_route && r.index > 1) {
			bool check = true;
			do {
				//if i'm here i can destroy the route
				//at the moment i destroy only a node
				double n_destroy = (double)rand() / RAND_MAX;
				int node_destroy = (int)(round(1 + n_destroy * (r.index - 2)));
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					vector<int> int_removed;

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
							}
						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
					
						r.passengers_in_route[int_removed[i]].route_before = index;

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}

					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					
					// Codice che serve per cercare il minimo nel range
					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								if (r.refueling[i]) break;

								if (r.weight[i] < min_weight && i != node_destroy) {
									min_weight = r.weight[i];
									Node_min = i;
								}
							}

							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								double Fuel_before = r.quantity_fuel[index_updating_from];
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);

								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);

					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {

							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
									index_refueling = i;
									break;
								}
							}

							for (int t = index_refueling; t < r.index; t++) {

								if (r.refueling[t] && t != index_refueling) break;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;

							}
						}
					}
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
					
					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
						check = false;
						vector<int> int_removed;
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
							if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
								if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
								if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
								int_removed.push_back(p);
								for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
									r.capacity[t] -= r.passengers_in_route[p].capacity;
									r.weight[t] += r.passengers_in_route[p].weight;
								}

							}
						}

						for (int i = int_removed.size() - 1; i >= 0; i--) {
							// code for repair forbidden***********************************************************
							r.passengers_in_route[int_removed[i]].route_before = index;
							//*************************************************************************************
							passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
							vector<Passenger>::iterator it;
							it = r.passengers_in_route.begin();
							r.passengers_in_route.erase(it + int_removed[i]);
						}
						r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time

						int index_before = node_destroy - 1;
						double diff = 0;

						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i])
								break;
							if (index_before == (node_destroy - 1)) {
								diff = r.quantity_fuel[i];
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];

							}
							else {
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
							}

							r.weight[i] = r.weight[i] + diff;
							index_before = i + 1;

						}

						r.removePlace(node_destroy, map_airplane);
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {

								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}

								for (int t = index_refueling; t < r.index; t++) {
									if (r.refueling[t] && t != index_refueling) break;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
								}
							}
						}

					}
				}

			} while (check);
		}
		index++;
	}
	return solution;
}

vector<Route> destroy_worst(double peso_TW, double peso_intermediate_stop, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		if (f <= destroy_coef_route && r.index > 1) {
			// Primo elemanto la posizione il secondo il numero del nodo
			map<int, int> Node;
			bool check = true;
			int first = 0;
			do {
				// Qui va messa una funzione ch individua il nodo peggiore
				Node = Compute_WorstNode(peso_TW, peso_intermediate_stop, r, map_airstrip, from_to);
				int node_destroy = Node[first];

				if (node_destroy == 0 || first >= (int)Node.size()) break;
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					vector<int> int_removed;

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
							}
						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						r.passengers_in_route[int_removed[i]].route_before = index;

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}

					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}

					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								if (r.refueling[i])
									break;
								if (r.weight[i] < min_weight && i != node_destroy) {
									min_weight = r.weight[i];
									Node_min = i;
								}
							}

							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {  
										index_updating_to = i;
										break;
									}
								}
								double Fuel_before = r.quantity_fuel[index_updating_from];
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);

								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];

					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
						check = false;
						vector<int> int_removed;
						
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
							if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
								if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
								if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
								int_removed.push_back(p);
								for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
									r.capacity[t] -= r.passengers_in_route[p].capacity;
									r.weight[t] += r.passengers_in_route[p].weight;
								}

							}
						}

						for (int i = int_removed.size() - 1; i >= 0; i--) {
							r.passengers_in_route[int_removed[i]].route_before = index;

							passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
							vector<Passenger>::iterator it;
							it = r.passengers_in_route.begin();
							r.passengers_in_route.erase(it + int_removed[i]);
						}
						r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time


						int index_before = node_destroy - 1;
						double diff = 0;
						for (int i = node_destroy + 1; i < r.index; i++) {
							//cout << " sono al i = " << i << endl;
							if (r.refueling[i]) break;
							//cout << "weight before: " << r.weight[i] << endl;
							if (index_before == (node_destroy - 1)) {
								diff = r.quantity_fuel[i];

								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];
							}
							else {
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
							}

							if (r.refueling[node_destroy]) {
								r.weight[i] = r.weight[i] + diff;
							}
							else {
								r.weight[i] = r.weight[i] + diff;
							}

							index_before = i + 1;
						}

						r.removePlace(node_destroy, map_airplane);
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {
								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}

								for (int t = index_refueling; t < r.index; t++) {
									if (r.refueling[t] && t != index_refueling) break;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
								}
							}
						}
					}
				}

				first++;
			} while (check);
		}
		index++;
	}
	return solution;
}

vector<Route> destroy_cluster_aggr2(double peso_TW, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, double2DVector& from_to, vector<Passenger> all_passenegr, map<int, Passenger>& map_id_passenger, double& peso_itermediate_stop) {
	int soglia_relateness = 300; // Prima era 100
	vector<int> int_removed;
	vector<Route> route_destroyed;
	vector<Passenger> passengers;

	unordered_map<int, double> CostTWPass;
	set<double, MyCOMP<double>> Myset;

	route_destroyed.insert(route_destroyed.end(), solution.begin(), solution.end());
	passengers.insert(passengers.end(), all_passenegr.begin(), all_passenegr.end());

	for (Passenger& p : passengers) CostTWPass.insert(make_pair(p.pnr, 0));
	for (Route& s : route_destroyed) {
		for (Passenger& pass : s.passengers_in_route) {
			int Codpass = pass.pnr;
			CostTWPass[Codpass] += cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW);
			Myset.insert(cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW));
		}
	}
	vector<string> OrderVectore;
	for (auto x : Myset) {
		for (auto y : CostTWPass) {
			if (y.second == x) {
				OrderVectore.push_back(to_string(y.first) + "|" + to_string(x));
			}
		}
	}

	map<string, vector<int>> agr_pass;
	vector<string> sequenza;
	for (string x : OrderVectore) {

		int codice = stoi(split(x, '|')[0]);
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[codice].departure_location) + "|" + to_string(map_id_passenger[codice].arrival_location);
		if (agr_pass.find(code) != agr_pass.end())
		{
			agr_pass[code].push_back(map_id_passenger[codice].pnr);
		}
		else {
			sequenza.push_back(code);
			vector<int> id_passeggieri;
			id_passeggieri.push_back(map_id_passenger[codice].pnr);
			agr_pass.insert(make_pair(code, id_passeggieri));
		}

	}

	for (int x = 0; x < (int)sequenza.size(); x++) {
		int Codpass = agr_pass[sequenza[x]][0];

		// tolgo tutti di questa aggregazione
		for (int p : agr_pass[sequenza[x]]) passenger_removed.push_back(map_id_passenger[p]);
		agr_pass.erase(agr_pass.find(sequenza[x]));
		sequenza.erase(sequenza.begin() + x);

		for (int y = 0; y < (int)sequenza.size(); y++) {

			// Qui adesso chimao la funzione Relateness !! 
			bool retrocedi = relateness_passenger2(soglia_relateness, from_to, Codpass, map_id_passenger[agr_pass[sequenza[y]][0]], passenger_removed, agr_pass, sequenza, sequenza[y], y, map_id_passenger);
			if (retrocedi) y--;
		}

		x--;
		if ((int)passenger_removed.size() >= num_passenger) break;
	}

	int Npass = 0;
	int Nroute = -1;

	// Devo toglierli dalla route 
	for (Route& s : route_destroyed) {
		Nroute++;
		vector<int> int_removed;
		bool Update = false;
		int Min_From_Pass = s.index;
		int Max_To_Pass = -1;
		for (int p = 0; p < (int)s.passengers_in_route.size(); p++) {
			for (Passenger& pass : passenger_removed) {
				if (s.passengers_in_route[p].pnr == pass.pnr) {
					Update = true;
					pass.route_before = Nroute;
					if (s.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = s.passengers_in_route[p].solution_from;
					if (s.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = s.passengers_in_route[p].solution_to;

					int_removed.push_back(p);
					for (int t = s.passengers_in_route[p].solution_from; t < s.passengers_in_route[p].solution_to; t++) {
						s.capacity[t] -= s.passengers_in_route[p].capacity;
						s.weight[t] += s.passengers_in_route[p].weight;
					}

				}
			}
		}

		for (int i = int_removed.size() - 1; i >= 0; i--) {
			vector<Passenger>::iterator it;
			it = s.passengers_in_route.begin();
			s.passengers_in_route.erase(it + int_removed[i]);
		}

		if (Update) {
			int index_min_from = Min_From_Pass;
			for (int i = Min_From_Pass; i >= 0; i--) {
				if (s.refueling[i]) {
					index_min_from = i;
					break;
				}
			}
			for (int k = index_min_from; k < Max_To_Pass; k++) {
				if (s.refueling[k] && s.quantity_fuel[k] < map_airplane[s.aircraft_code].max_fuel) { //&& k!= node_destroy
					int index_updating_from = k;
					int index_updating_to = s.index;
					int Node_min = k;
					double min_weight = s.weight[k];
					//appena aggiunto
					for (int i = Max_To_Pass; i < s.index; i++) {
						if (s.refueling[i]) break;
						Max_To_Pass = i;
					}

					for (int i = k + 1; i <= Max_To_Pass; i++) {
						if (s.refueling[i]) break;

						if (s.weight[i] < min_weight) {
							min_weight = s.weight[i];
							Node_min = i;
						}
					}
					if (Node_min >= 0) {
						for (int i = k + 1; i < s.index; i++) {
							if (s.refueling[i]) {
								index_updating_to = i;
								break;
							}
						}

						double Fuel_before = s.quantity_fuel[index_updating_from];

						s.quantity_fuel[index_updating_from] = min(map_airplane[s.aircraft_code].max_fuel, s.quantity_fuel[index_updating_from] + min_weight);
						s.weight[index_updating_from] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						for (int i = index_updating_from + 1; i < index_updating_to; i++) {
							s.quantity_fuel[i] += (s.quantity_fuel[index_updating_from] - Fuel_before);
							s.weight[i] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						}
					}
				}
			}


			int index_sup = s.index;
			for (int i = index_sup - 1; i > 1; i--) {

				if (s.capacity[i - 1] != 0) break;

				if (s.capacity[i - 1] == 0) {
					s.places.erase(s.places.begin() + i);
					s.time_arr.erase(s.time_arr.begin() + i);
					s.time_dep.erase(s.time_dep.begin() + i);
					s.refueling.erase(s.refueling.begin() + i);
					s.quantity_fuel.erase(s.quantity_fuel.begin() + i);
					s.weight.erase(s.weight.begin() + i);
					s.capacity.erase(s.capacity.begin() + i);
					s.index = s.index - 1;

				}
			}


			if (s.index == 2 && s.capacity[0] == 0) {
				s.places.erase(s.places.begin() + 1);
				s.time_arr.erase(s.time_arr.begin() + 1);
				s.time_dep.erase(s.time_dep.begin() + 1);
				s.refueling.erase(s.refueling.begin() + 1);
				s.quantity_fuel.erase(s.quantity_fuel.begin() + 1);
				s.weight.erase(s.weight.begin() + 1);
				s.capacity.erase(s.capacity.begin() + 1);
				s.index = s.index - 1;
			}

			// Questa parte qua forse si puo togliere
			//qua ? la parte che ho aggiunto io (NELLI) per il problema del nodo al deposito che non si aggiorna
			if ((int)s.places.size() == 1 && s.capacity[0] == 0) {
				s.quantity_fuel[0] = map_airplane[s.aircraft_code].max_fuel;
				s.weight[0] = map_airplane[s.aircraft_code].weight_fuel_people - map_airplane[s.aircraft_code].max_fuel;
			}
		}
	}

	Npass = 0;
	for (Route& s : route_destroyed) {
		Npass += (int)(s.passengers_in_route.size());
		if (s.passengers_in_route.size() == 0) s.primo_pass = false;
	}

	return route_destroyed;

}

void destroy_ls(int index, int node_destroy, vector<Passenger>& passenger_removed, Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {

	if (r.index > 1) {
		bool check = true;
		double fuel_consumed_check = 0.0;
		if ((node_destroy + 2) < r.index) {
			//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; 
			double time_travel = from_to[r.places[node_destroy - 1]][r.places[node_destroy + 2]] / map_airplane[r.aircraft_code].speed;
			if (time_travel <= 1) fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
			else {
				fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
			}
		}
		if ((fuel_consumed_check + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
			check = false;
			int number_initial_node = r.index;
			for (int i = 0; i < 2; i++) {
				if (i == 1 && (number_initial_node - r.index) > 1) break;
				double fuel_consumed = 0.0;
				double time_travel = from_to[r.places[node_destroy - 1]][r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed;
				if (time_travel <= 1) fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
				else {
					fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
				}
				vector<int> int_removed;
				int Min_From_Pass = node_destroy;
				int Max_To_Pass = node_destroy;
				for (int p = 0; p < r.passengers_in_route.size(); p++) {
					if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
						if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
						if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;

						int_removed.push_back(p);
						for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
							r.capacity[t] -= r.passengers_in_route[p].capacity;
							r.weight[t] += r.passengers_in_route[p].weight;
						}

					}
				}

				for (int i = int_removed.size() - 1; i >= 0; i--) {
					r.passengers_in_route[int_removed[i]].route_before = index;
					passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
					vector<Passenger>::iterator it;
					it = r.passengers_in_route.begin();
					r.passengers_in_route.erase(it + int_removed[i]);
				}

				r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
				int index_before = node_destroy - 1;
				double diff = 0;

				for (int i = node_destroy + 1; i < r.index; i++) {

					if (r.refueling[i]) break;

					if (index_before == (node_destroy - 1)) {
						diff = r.quantity_fuel[i];
						r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
						diff = diff - r.quantity_fuel[i];

					}
					else {
						r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
					}

					r.weight[i] = r.weight[i] + diff;
					index_before = i + 1;

				}

				r.removePlace(node_destroy, map_airplane);
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weight[j] < 0) {
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						for (int t = index_refueling; t < r.index; t++) {

							if (r.refueling[t] && t != index_refueling) break;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
						}
					}
				}

			}
		}

		if (check) {
			r.index = -1;
		}
	}
}
