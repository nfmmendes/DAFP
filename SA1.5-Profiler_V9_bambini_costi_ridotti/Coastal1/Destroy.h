//ECCO I NOSTRI DESTROY
#include <set>
#include <unordered_map>


#include "Evaluate.h"
#include "Route.h"
#include "Util.h"



bool relateness_passenger2(int trheshold, vector<vector<double>>& from_to, int idP, Passenger& r, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequenza, string pointer, int y, map<int, Passenger>& map_id_passenger) {
	// Poi questo for si potra togliere
	Passenger pass = map_id_passenger[idP];
	double Relateness = from_to[r.destination][pass.destination] + from_to[r.origin][pass.origin] + abs(r.arrival_time - pass.arrival_time) + abs(r.departure_time - pass.departure_time);

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


vector<Route> destroy_thanos(ProcessedInput* input, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		if (f <= destroy_coef_route && r.index > 1) {

			//genero il numero random di nodi da distruggere da 2 a place.size()-1;
			double numero_random = (double)rand() / RAND_MAX;
			if (r.airstrips.size() == 2) {
				numero_random = round(1 + (numero_random * (r.airstrips.size() - 2)));
			}
			else {
				numero_random = round(2 + (numero_random * (r.airstrips.size() - 3)));
			}

			if (numero_random == r.airstrips.size() - 1) {
				// qua devo distruggere tutta la route e lasciare solo il depot
				for (int i = (int)(r.airstrips.size()) - 1; i >= 1; i--) {
					r.airstrips.erase(r.airstrips.begin() + i);
					r.refueling.erase(r.refueling.begin() + i);
					r.fuel.erase(r.fuel.begin() + i);
					r.arrival.erase(r.arrival.begin() + i);
					r.departure.erase(r.departure.begin() + i);
					r.capacities.erase(r.capacities.begin() + i);
					r.weights.erase(r.weights.begin() + i);
					r.index--;
				}
				//qua ci dovrebbe essere solo il deposito
				//fisso il peso, capacita e fuel ai valori di default e la variabile passeggeri dentro a false
				r.fuel[0] = map_airplane[r.aircraft_code].max_fuel;
				r.weights[0] = map_airplane[r.aircraft_code].load_weight - r.fuel[0];
				r.capacities[0] = 0;
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
										r.capacities[t] -= r.passengers_in_route[p].capacity;
										r.weights[t] += r.passengers_in_route[p].weight;
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
								if (r.refueling[k] && r.fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
									int Node_min = k;
									double min_weight = r.weights[k];
									int index_updating_from = k;
									int index_updating_to = r.index;  //qua prima c'era -1
									for (int i = k + 1; i <= Max_To_Pass; i++) {
										if (r.refueling[i]) break;
										if (r.weights[i] < min_weight && i != node_destroy) {
											min_weight = r.weights[i];
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

										double Fuel_before = r.fuel[index_updating_from];
										r.fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.fuel[index_updating_from] + min_weight);
										r.weights[index_updating_from] -= (r.fuel[index_updating_from] - Fuel_before);
										for (int i = index_updating_from + 1; i < index_updating_to; i++) {
											if (i != node_destroy) {
												r.fuel[i] += (r.fuel[index_updating_from] - Fuel_before);
												r.weights[i] -= (r.fuel[index_updating_from] - Fuel_before);
											}
										}
									}
								}

							}

							int nodi_mancanti = (int)(r.airstrips.size());
							r.removePlace(node_destroy, map_airplane);
							nodi_mancanti -= (int)r.airstrips.size();
							nodi_rimossi += nodi_mancanti;
						}
						else {

							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.airstrips[node_destroy - 1]][r.airstrips[node_destroy + 1]];

							if (fuel_consumed <= r.fuel[node_destroy - 1] && r.airstrips[node_destroy - 1] != r.airstrips[node_destroy + 1]) {
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
											r.capacities[t] -= r.passengers_in_route[p].capacity;
											r.weights[t] += r.passengers_in_route[p].weight;
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

								int nodi_mancanti = (int)r.airstrips.size();
								r.update_route_destroy(input, node_destroy, Min_From_Pass, Max_To_Pass); //update of the time

								int index_before = node_destroy - 1;
								double diff = 0;

								for (int i = node_destroy + 1; i < r.index; i++) {

									if (r.refueling[i]) break;
									if (index_before == (node_destroy - 1)) {

										diff = r.fuel[i];
										r.fuel[i] = r.fuel[index_before] - fuel_consumed;
										diff = diff - r.fuel[i];
									}
									else {
										r.fuel[i] = r.fuel[i] - diff;
									}

									if (r.refueling[node_destroy]) {
										r.weights[i] = r.weights[i] + diff;
									}
									else {
										r.weights[i] = r.weights[i] + diff;
									}

									index_before = i + 1;
								}
								//*****************************************************************************************************************************************************************
								r.removePlace(node_destroy, map_airplane);
								nodi_mancanti -= (int)(r.airstrips.size());
								nodi_rimossi += (int)(nodi_mancanti);

								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weights[j] < 0) {
										add_fuel = r.weights[j];
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
											r.fuel[t] += add_fuel;
											r.weights[t] -= add_fuel;
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

vector<Route> destroy_casual(ProcessedInput* input, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
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
								r.capacities[t] -= r.passengers_in_route[p].capacity;
								r.weights[t] += r.passengers_in_route[p].weight;
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
						if (r.refueling[k] && r.fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weights[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								if (r.refueling[i]) break;

								if (r.weights[i] < min_weight && i != node_destroy) {
									min_weight = r.weights[i];
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
								double Fuel_before = r.fuel[index_updating_from];
								r.fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.fuel[index_updating_from] + min_weight);
								r.weights[index_updating_from] -= (r.fuel[index_updating_from] - Fuel_before);

								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.fuel[i] += (r.fuel[index_updating_from] - Fuel_before);
										r.weights[i] -= (r.fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);

					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weights[j] < 0) {

							add_fuel = r.weights[j];
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
								r.fuel[t] += add_fuel;
								r.weights[t] -= add_fuel;

							}
						}
					}
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.airstrips[node_destroy - 1]][r.airstrips[node_destroy + 1]];
					
					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.fuel[node_destroy - 1] && r.airstrips[node_destroy - 1] != r.airstrips[node_destroy + 1]) {
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
									r.capacities[t] -= r.passengers_in_route[p].capacity;
									r.weights[t] += r.passengers_in_route[p].weight;
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
						r.update_route_destroy(input, node_destroy, Min_From_Pass, Max_To_Pass); //update of the time

						int index_before = node_destroy - 1;
						double diff = 0;

						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i])
								break;
							if (index_before == (node_destroy - 1)) {
								diff = r.fuel[i];
								r.fuel[i] = r.fuel[index_before] - fuel_consumed;
								diff = diff - r.fuel[i];

							}
							else {
								r.fuel[i] = r.fuel[i] - diff;
							}

							r.weights[i] = r.weights[i] + diff;
							index_before = i + 1;

						}

						r.removePlace(node_destroy, map_airplane);
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weights[j] < 0) {

								add_fuel = r.weights[j];
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
									r.fuel[t] += add_fuel;
									r.weights[t] -= add_fuel;
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

vector<Route> destroy_worst(ProcessedInput* input, const PenaltyWeights& penalty_weights, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
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
								r.capacities[t] -= r.passengers_in_route[p].capacity;
								r.weights[t] += r.passengers_in_route[p].weight;
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
						if (r.refueling[k] && r.fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
							int Node_min = k;
							double min_weight = r.weights[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								if (r.refueling[i])
									break;
								if (r.weights[i] < min_weight && i != node_destroy) {
									min_weight = r.weights[i];
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
								double Fuel_before = r.fuel[index_updating_from];
								r.fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.fuel[index_updating_from] + min_weight);
								r.weights[index_updating_from] -= (r.fuel[index_updating_from] - Fuel_before);

								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.fuel[i] += (r.fuel[index_updating_from] - Fuel_before);
										r.weights[i] -= (r.fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.airstrips[node_destroy - 1]][r.airstrips[node_destroy + 1]];

					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.fuel[node_destroy - 1] && r.airstrips[node_destroy - 1] != r.airstrips[node_destroy + 1]) {
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
									r.capacities[t] -= r.passengers_in_route[p].capacity;
									r.weights[t] += r.passengers_in_route[p].weight;
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
						r.update_route_destroy(input, node_destroy, Min_From_Pass, Max_To_Pass); //update of the time


						int index_before = node_destroy - 1;
						double diff = 0;
						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i]) break;
							
							if (index_before == (node_destroy - 1)) {
								diff = r.fuel[i];

								r.fuel[i] = r.fuel[index_before] - fuel_consumed;
								diff = diff - r.fuel[i];
							}
							else {
								r.fuel[i] = r.fuel[i] - diff;
							}

							if (r.refueling[node_destroy]) {
								r.weights[i] = r.weights[i] + diff;
							}
							else {
								r.weights[i] = r.weights[i] + diff;
							}

							index_before = i + 1;
						}

						r.removePlace(node_destroy, map_airplane);
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weights[j] < 0) {
								add_fuel = r.weights[j];
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
									r.fuel[t] += add_fuel;
									r.weights[t] -= add_fuel;
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

vector<Route> destroy_cluster_aggr2(ProcessedInput* input, const PenaltyWeights& penalty_weights, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution,  vector<Passenger> all_passenegr, map<int, Passenger>& map_id_passenger) {
	double peso_TW = penalty_weights.time_window;
	double peso_itermediate_stop = penalty_weights.intermediate_stop;
	
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();

	const int soglia_relateness = 300; // Prima era 100
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
			CostTWPass[Codpass] += cost_for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW);
			Myset.insert(cost_for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW));
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
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[codice].origin) + "|" + to_string(map_id_passenger[codice].destination);
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
						s.capacities[t] -= s.passengers_in_route[p].capacity;
						s.weights[t] += s.passengers_in_route[p].weight;
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
				if (s.refueling[k] && s.fuel[k] < map_airplane[s.aircraft_code].max_fuel) { //&& k!= node_destroy
					int index_updating_from = k;
					int index_updating_to = s.index;
					int Node_min = k;
					double min_weight = s.weights[k];
					//appena aggiunto
					for (int i = Max_To_Pass; i < s.index; i++) {
						if (s.refueling[i]) break;
						Max_To_Pass = i;
					}

					for (int i = k + 1; i <= Max_To_Pass; i++) {
						if (s.refueling[i]) break;

						if (s.weights[i] < min_weight) {
							min_weight = s.weights[i];
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

						double Fuel_before = s.fuel[index_updating_from];

						s.fuel[index_updating_from] = min(map_airplane[s.aircraft_code].max_fuel, s.fuel[index_updating_from] + min_weight);
						s.weights[index_updating_from] -= (s.fuel[index_updating_from] - Fuel_before);
						for (int i = index_updating_from + 1; i < index_updating_to; i++) {
							s.fuel[i] += (s.fuel[index_updating_from] - Fuel_before);
							s.weights[i] -= (s.fuel[index_updating_from] - Fuel_before);
						}
					}
				}
			}


			int index_sup = s.index;
			for (int i = index_sup - 1; i > 1; i--) {

				if (s.capacities[i - 1] != 0) break;

				if (s.capacities[i - 1] == 0) {
					s.airstrips.erase(s.airstrips.begin() + i);
					s.arrival.erase(s.arrival.begin() + i);
					s.departure.erase(s.departure.begin() + i);
					s.refueling.erase(s.refueling.begin() + i);
					s.fuel.erase(s.fuel.begin() + i);
					s.weights.erase(s.weights.begin() + i);
					s.capacities.erase(s.capacities.begin() + i);
					s.index = s.index - 1;

				}
			}


			if (s.index == 2 && s.capacities[0] == 0) {
				s.airstrips.erase(s.airstrips.begin() + 1);
				s.arrival.erase(s.arrival.begin() + 1);
				s.departure.erase(s.departure.begin() + 1);
				s.refueling.erase(s.refueling.begin() + 1);
				s.fuel.erase(s.fuel.begin() + 1);
				s.weights.erase(s.weights.begin() + 1);
				s.capacities.erase(s.capacities.begin() + 1);
				s.index = s.index - 1;
			}

			// Questa parte qua forse si puo togliere
			//qua ? la parte che ho aggiunto io (NELLI) per il problema del nodo al deposito che non si aggiorna
			if ((int)s.airstrips.size() == 1 && s.capacities[0] == 0) {
				s.fuel[0] = map_airplane[s.aircraft_code].max_fuel;
				s.weights[0] = map_airplane[s.aircraft_code].load_weight - map_airplane[s.aircraft_code].max_fuel;
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

void destroy_ls(ProcessedInput* input, int index, int node_destroy, vector<Passenger>& passenger_removed, Route& r) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	if (r.index > 1) {
		bool check = true;
		double fuel_consumed_check = 0.0;
		if ((node_destroy + 2) < r.index) {
			//double time_travel = from_to[r.airstrips[node_destroy - 1] + ";" + r.airstrips[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; 
			double time_travel = from_to[r.airstrips[node_destroy - 1]][r.airstrips[node_destroy + 2]] / map_airplane[r.aircraft_code].speed;
			if (time_travel <= 1) fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
			else {
				fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
			}
		}
		if ((fuel_consumed_check + map_airplane[r.aircraft_code].min_fuel) <= r.fuel[node_destroy - 1]) {
			check = false;
			int number_initial_node = r.index;
			for (int i = 0; i < 2; i++) {
				if (i == 1 && (number_initial_node - r.index) > 1) break;
				double fuel_consumed = 0.0;
				double time_travel = from_to[r.airstrips[node_destroy - 1]][r.airstrips[node_destroy + 1]] / map_airplane[r.aircraft_code].speed;
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
							r.capacities[t] -= r.passengers_in_route[p].capacity;
							r.weights[t] += r.passengers_in_route[p].weight;
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

				r.update_route_destroy(input, node_destroy, Min_From_Pass, Max_To_Pass); //update of the time
				int index_before = node_destroy - 1;
				double diff = 0;

				for (int i = node_destroy + 1; i < r.index; i++) {

					if (r.refueling[i]) break;

					if (index_before == (node_destroy - 1)) {
						diff = r.fuel[i];
						r.fuel[i] = r.fuel[index_before] - fuel_consumed;
						diff = diff - r.fuel[i];

					}
					else {
						r.fuel[i] = r.fuel[i] - diff;
					}

					r.weights[i] = r.weights[i] + diff;
					index_before = i + 1;

				}

				r.removePlace(node_destroy, map_airplane);
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weights[j] < 0) {
						add_fuel = r.weights[j];
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
							r.fuel[t] += add_fuel;
							r.weights[t] -= add_fuel;
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
