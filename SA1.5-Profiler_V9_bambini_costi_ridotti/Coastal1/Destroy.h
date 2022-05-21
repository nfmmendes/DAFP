//ECCO I NOSTRI DESTROY
#include <set>
#include <unordered_map>

#include "Evaluate.h"
#include "Route.h"
#include "Util.h"

bool relateness_passenger2(int trheshold, double relateness, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequence, int y, map<int, Passenger>& map_id_passenger) {
	// Poi questo for si potra togliere
	auto pointer = sequence[y];
	
	if (relateness <= trheshold) {
		for (int j : agr_pass[pointer]) 
			passengers_removed.push_back(map_id_passenger[j]);
		sequence.erase(sequence.begin() + y);
		agr_pass.erase(agr_pass.find(pointer));

		return true;
	}
	
	return false;
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

			if (r.get_airstrips().size() == 2) 
				numero_random = round(1 + (numero_random * (r.get_airstrips().size() - 2)));
			else
				numero_random = round(2 + (numero_random * (r.get_airstrips().size() - 3)));

			if (numero_random == r.get_airstrips().size() - 1) {
				// qua devo distruggere tutta la route e lasciare solo il depot
				for (int i = (int)(r.get_airstrips().size()) - 1; i >= 1; i--)
					r.remove_at(i);
					
				//qua ci dovrebbe essere solo il deposito
				//fisso il peso, capacita e fuel ai valori di default e la variabile passeggeri dentro a false
				r.fuel[0] = map_airplane[r.aircraft_code].max_fuel;
				r.get_weight_at(0) = map_airplane[r.aircraft_code].load_weight - r.fuel[0];
				r.set_capacity_at(0, 0);
				r.primo_pass = false;

				//ora devo togliere tutti i passeggeri
				for (int p = r.get_passengers().size() - 1; p >= 0; p--) {
					r.get_passenger(p).route_before = index;
					passenger_removed.push_back(r.get_passengers()[p]);
					r.erase_passenger(p);
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
							for (int p = 0; p < (int)r.get_passengers().size(); p++) {
								// TODO: replace r.get_passengers()[p] by a Passenger object
								if (r.get_passengers()[p].solution_to == node_destroy) {  // ho cambiato questa condizione
									if (r.get_passengers()[p].solution_from < Min_From_Pass) Min_From_Pass = r.get_passengers()[p].solution_from;
									int_removed.push_back(p);
									for (int t = r.get_passengers()[p].solution_from; t < r.get_passengers()[p].solution_to; t++) {
										r.add_capacity_at(t, -1*r.get_passengers()[p].capacity);
										r.get_weight_at(t) += r.get_passengers()[p].weight;
									}
								}
							}
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								r.get_passenger(int_removed[i]).route_before = index;
								passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
								r.erase_passenger(int_removed[i]);
							}

							int index_min_from = 0;
							for (int i = Min_From_Pass; i >= 0; i--) {
								if (r.get_refueling()[i] && i != node_destroy) {
									index_min_from = i;
									break;
								}
							}

							for (int k = index_min_from; k < Max_To_Pass; k++) {
								if (r.get_refueling()[k] && r.fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
									int Node_min = k;
									double min_weight = r.get_weights()[k];
									int index_updating_from = k;
									int index_updating_to = r.index;  //qua prima c'era -1
									for (int i = k + 1; i <= Max_To_Pass; i++) {
										if (r.get_refueling()[i]) 
											break;
										
										if (r.get_weights()[i] < min_weight && i != node_destroy) {
											min_weight = r.get_weights()[i];
											Node_min = i;
										}
									}
									if (Node_min >= 0) {
										for (int i = k + 1; i < r.index; i++) {
											if (r.get_refueling()[i]) {  
												index_updating_to = i;
												break;
											}
										}

										double Fuel_before = r.fuel[index_updating_from];
										r.fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.fuel[index_updating_from] + min_weight);
										r.get_weight_at(index_updating_from) -= (r.fuel[index_updating_from] - Fuel_before);
										for (int i = index_updating_from + 1; i < index_updating_to; i++) {
											if (i != node_destroy) {
												r.fuel[i] += (r.fuel[index_updating_from] - Fuel_before);
												r.get_weight_at(i) -= (r.fuel[index_updating_from] - Fuel_before);
											}
										}
									}
								}
							}

							int nodi_mancanti = (int)(r.get_airstrips().size());
							r.removePlace(node_destroy, map_airplane);
							nodi_mancanti -= (int)r.get_airstrips().size();
							nodi_rimossi += nodi_mancanti;
						}
						else {
							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.get_airstrips()[node_destroy - 1]][r.get_airstrips()[node_destroy + 1]];

							if (fuel_consumed <= r.fuel[node_destroy - 1] && r.get_airstrips()[node_destroy - 1] != r.get_airstrips()[node_destroy + 1]) {
								check = false;
								vector<int> int_removed;
								int Min_From_Pass = node_destroy;
								int Max_To_Pass = node_destroy;
								for (int p = 0; p < (int)r.get_passengers().size(); p++) {
									if (r.get_passengers()[p].solution_to == node_destroy || r.get_passengers()[p].solution_from == node_destroy) { // ho cambiato questa condizione
										if (r.get_passengers()[p].solution_from < Min_From_Pass) 
											Min_From_Pass = r.get_passengers()[p].solution_from;
										if (r.get_passengers()[p].solution_to > Max_To_Pass) 
											Max_To_Pass = r.get_passengers()[p].solution_to;
										
										int_removed.push_back(p);
										for (int t = r.get_passengers()[p].solution_from; t < r.get_passengers()[p].solution_to; t++) {
											r.add_capacity_at(t, -1.0*r.get_passengers()[p].capacity);
											r.get_weight_at(t) += r.get_passengers()[p].weight;
										}
									}
								}

								for (int i = int_removed.size() - 1; i >= 0; i--) {
									r.get_passenger(int_removed[i]).route_before = index;
									passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
									r.erase_passenger(int_removed[i]);
								}

								int nodi_mancanti = (int)r.get_airstrips().size();
								r.update_route_destroy(input, node_destroy, Min_From_Pass, Max_To_Pass); //update of the time

								int index_before = node_destroy - 1;
								double diff = 0;

								for (int i = node_destroy + 1; i < r.index; i++) {

									if (r.get_refueling()[i]) break;
									if (index_before == (node_destroy - 1)) {
										diff = r.fuel[i];
										r.fuel[i] = r.fuel[index_before] - fuel_consumed;
										diff = diff - r.fuel[i];
									}
									else {
										r.fuel[i] = r.fuel[i] - diff;
									}

									if (r.get_refueling()[node_destroy])
										r.get_weight_at(i) = r.get_weights()[i] + diff;
									else
										r.get_weight_at(i) = r.get_weights()[i] + diff;

									index_before = i + 1;
								}
								//******************************************************************
								r.removePlace(node_destroy, map_airplane);
								nodi_mancanti -= (int)(r.get_airstrips().size());
								nodi_rimossi += (int)(nodi_mancanti);

								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.get_weights()[j] < 0) {
										add_fuel = r.get_weights()[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.get_refueling()[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}

										for (int t = index_refueling; t < r.index; t++) {
											if (r.get_refueling()[t] && t != index_refueling) break;
											r.fuel[t] += add_fuel;
											r.get_weight_at(t) -= add_fuel;
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

void do_work11(Route& r, Airplane* airplane, const int node_destroy, const int Max_To_Pass, const int k)
{
	int Node_min = k;
	double min_weight = r.get_weights()[k];
	int index_updating_from = k;
	int index_updating_to = r.index;  //qua prima c'era -1
	for (int i = k + 1; i <= Max_To_Pass; i++) {
		if (r.get_refueling()[i]) 
			break;

		if (r.get_weights()[i] < min_weight && i != node_destroy) {
			min_weight = r.get_weights()[i];
			Node_min = i;
		}
	}

	if (Node_min >= 0) {
		for (int i = k + 1; i < r.index; i++) {
			if (r.get_refueling()[i]) {  
				index_updating_to = i;
				break;
			}
		}
		double Fuel_before = r.fuel[index_updating_from];
		r.fuel[index_updating_from] = min(airplane->max_fuel, r.fuel[index_updating_from] + min_weight);
		r.get_weight_at(index_updating_from) -= (r.fuel[index_updating_from] - Fuel_before);

		for (int i = index_updating_from + 1; i < index_updating_to; i++) {
			if (i != node_destroy) {
				r.fuel[i] += (r.fuel[index_updating_from] - Fuel_before);
				r.get_weight_at(i) -= (r.fuel[index_updating_from] - Fuel_before);
			}
		}
	}
}


void do_work13(Route& r, const int node_destroy, vector<int>& int_removed, int& Min_From_Pass, int& Max_To_Pass)
{
	for (int p = 0; p < (int)r.get_passengers().size(); p++) {
		const Passenger passenger = r.get_passengers()[p];
		if (passenger.solution_to == node_destroy || passenger.solution_from == node_destroy) {								if (r.get_passengers()[p].solution_from < Min_From_Pass) 
			Min_From_Pass = passenger.solution_from;
			Max_To_Pass = max(Max_To_Pass, passenger.solution_to);
								
			int_removed.push_back(p);
			for (int t = passenger.solution_from; t < passenger.solution_to; t++) {
				r.add_capacity_at(t, -1*passenger.capacity);
				r.get_weight_at(t) += passenger.weight;
			}
		}
	}
}

void do_work14(Route& r, int node_destroy, double fuel_consumed)
{
	int index_before = node_destroy - 1;
	double diff = 0;
	
	for (int i = node_destroy + 1; i < r.index; i++) {
		if (r.get_refueling()[i])
			break;
		if (index_before == (node_destroy - 1)) {
			diff = r.fuel[i];
			r.fuel[i] = r.fuel[index_before] - fuel_consumed;
			diff = diff - r.fuel[i];
		}
		else {
			r.fuel[i] = r.fuel[i] - diff;
		}

		r.get_weight_at(i) = r.get_weights()[i] + diff;
		index_before = i + 1;
	}
}

void do_work12(Route& r)
{
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = 0; j < r.index; j++) {
		if (r.get_weights()[j] < 0) {

			add_fuel = r.get_weights()[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (r.get_refueling()[i]) { //&& i != node_destroy
					index_refueling = i;
					break;
				}
			}

			for (int t = index_refueling; t < r.index; t++) {
				if (r.get_refueling()[t] && t != index_refueling) 
					break;
				r.fuel[t] += add_fuel;
				r.get_weight_at(t) -= add_fuel;
			}
		}
	}
}

void do_work15(Route& r, int node_destroy, vector<int>& int_removed, int& Min_From_Pass, int& Max_To_Pass)
{
	Min_From_Pass = node_destroy;
	Max_To_Pass = node_destroy;
	for (int p = 0; p < (int)r.get_passengers().size(); p++) {
		Passenger passenger = r.get_passengers()[p];
						
		if (passenger.solution_to == node_destroy) {  // ho cambiato questa condizione
			Min_From_Pass = min(Min_From_Pass, passenger.solution_from);
			int_removed.push_back(p);

			for (int t = passenger.solution_from; t < passenger.solution_to; t++) {
				r.add_capacity_at(t, -1*passenger.capacity);
				r.get_weight_at(t) += passenger.weight;
			}
		}
	}
}

vector<Route> destroy_casual(ProcessedInput* input, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	int index = 0;
	for (Route& r : solution) {

		Airplane* airplane = &map_airplane[r.aircraft_code];
		auto airplane_consumptions = from_to_FuelConsumed[r.aircraft_code];
		
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

					int Min_From_Pass;
					int Max_To_Pass;
					do_work15(r, node_destroy, int_removed, Min_From_Pass, Max_To_Pass);
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						r.get_passenger(int_removed[i]).route_before = index;

						passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
						vector<Passenger>::iterator it;
						r.erase_passenger(int_removed[i]);
					}

					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.get_refueling()[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					
					// Codice che serve per cercare il minimo nel range
					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.get_refueling()[k] && r.fuel[k] < airplane->max_fuel) { //&& k!= node_destroy
							do_work11(r, airplane, node_destroy, Max_To_Pass, k);
						}
					}
					
					r.removePlace(node_destroy, map_airplane);
					do_work12(r);
				}
				else {
					double fuel_consumed = airplane_consumptions[r.get_airstrips()[node_destroy - 1]][r.get_airstrips()[node_destroy + 1]];
					
					if ((fuel_consumed + airplane->min_fuel) <= r.fuel[node_destroy - 1] && r.get_airstrips()[node_destroy - 1] != r.get_airstrips()[node_destroy + 1]) {
						check = false;
						vector<int> int_removed;
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						do_work13(r, node_destroy, int_removed, Min_From_Pass, Max_To_Pass);

						for (int i = int_removed.size() - 1; i >= 0; i--) {
							// code for repair forbidden***********************************************************
							r.get_passenger(int_removed[i]).route_before = index;
							//*************************************************************************************
							passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
							r.erase_passenger(int_removed[i]);
						}
						r.update_route_destroy(input, node_destroy, Min_From_Pass, Max_To_Pass); //update of the time

						do_work14(r, node_destroy, fuel_consumed);

						r.removePlace(node_destroy, map_airplane);
						do_work12(r);
					}
				}
			} while (check);
		}
		index++;
	}
	return solution;
}

void do_work1(Route& r, const int node_destroy, vector<int>& int_removed, int& min_from_pass)
{
	for (int p = 0; p < (int)r.get_passengers().size(); p++) {
		const Passenger passenger = r.get_passengers()[p];
						
		if (passenger.solution_to == node_destroy) {  // ho cambiato questa condizione
			if (passenger.solution_from < min_from_pass) 
				min_from_pass = passenger.solution_from;
			int_removed.push_back(p);

			for (int t = passenger.solution_from; t < passenger.solution_to; t++) {
				r.add_capacity_at(t, -1*passenger.capacity);
				r.get_weight_at(t) += passenger.weight;
			}
		}
	}
}

void do_work6(Route& r, const Airplane* airplane, const int node_destroy, const int max_to_pass, const int k)
{
	int Node_min = k;
	double min_weight = r.get_weights()[k];
	int index_updating_to = r.index;  //qua prima c'era -1
	for (int i = k + 1; i <= max_to_pass; i++) {
		if (r.get_refueling()[i])
			break;
		if (r.get_weights()[i] < min_weight && i != node_destroy) {
			min_weight = r.get_weights()[i];
			Node_min = i;
		}
	}

	if (Node_min >= 0) {
		for (int i = k + 1; i < r.index; i++) {
			if (r.get_refueling()[i]) {
				index_updating_to = i;
				break;
			}
		}
								
		double fuel_before = r.fuel[k];
		r.fuel[k] = min(airplane->max_fuel, r.fuel[k] + min_weight);
		r.get_weight_at(k) -= (r.fuel[k] - fuel_before);

		for (int i = k + 1; i < index_updating_to; i++) {
			if (i != node_destroy) {
				r.fuel[i] += (r.fuel[k] - fuel_before);
				r.get_weight_at(i) -= (r.fuel[k] - fuel_before);
			}
		}
	}
}

void do_work7(Route& r, int node_destroy, double fuel_consumed, int index_before, double diff)
{
	for (int i = node_destroy + 1; i < r.index; i++) {
		if (r.get_refueling()[i]) 
			break;
							
		if (index_before == (node_destroy - 1)) {
			diff = r.fuel[i];

			r.fuel[i] = r.fuel[index_before] - fuel_consumed;
			diff = diff - r.fuel[i];
		}
		else {
			r.fuel[i] = r.fuel[i] - diff;
		}

		if (r.get_refueling()[node_destroy]) 
			r.get_weight_at(i) = r.get_weights()[i] + diff;
		else
			r.get_weight_at(i) = r.get_weights()[i] + diff;

		index_before = i + 1;
	}
}

void do_work8(Route& r, double& add_fuel, int& index_weight_neg)
{
	for (int j = 0; j < r.index; j++) {
		if (r.get_weights()[j] < 0) {
			add_fuel = r.get_weights()[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (r.get_refueling()[i]) { //&& i != node_destroy
					index_refueling = i;
					break;
				}
			}

			for (int t = index_refueling; t < r.index; t++) {
				if (r.get_refueling()[t] && t != index_refueling) 
					break;
				
				r.fuel[t] += add_fuel;
				r.get_weight_at(t) -= add_fuel;
			}
		}
	}
}

void do_work10(Route& r, int node_destroy, vector<int> int_removed, int& min_from_pass, int& max_to_pass)
{
	for (int p = 0; p < (int)r.get_passengers().size(); p++) {
		const Passenger passenger = r.get_passengers()[p];
		if (passenger.solution_to == node_destroy || passenger.solution_from == node_destroy) { 
			min_from_pass = min(min_from_pass, passenger.solution_from);
			max_to_pass = max(max_to_pass, passenger.solution_to);

			int_removed.push_back(p);
			for (int t = r.get_passengers()[p].solution_from; t < passenger.solution_to; t++) {
				r.add_capacity_at(t, -1*passenger.capacity);
				r.get_weight_at(t) += passenger.weight;
			}
		}
	}
}

vector<Route> destroy_worst(ProcessedInput* input, const PenaltyWeights& penalty_weights, double destroy_coef, vector<Passenger>& passenger_removed, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	int index = 0;
	for (Route& r : solution) {
		const Airplane* airplane = &map_airplane[r.aircraft_code];
		auto airplane_consume = from_to_FuelConsumed[r.aircraft_code];
		
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		if (f <= destroy_coef && r.index > 1) {
			// Primo elemanto la posizione il secondo il numero del nodo
			map<int, int> Node;
			bool check = true;
			int first = 0;
			do {
				// Qui va messa una funzione ch individua il nodo peggiore
				Node = Compute_WorstNode(peso_TW, peso_intermediate_stop, r, map_airstrip, from_to);
				int node_destroy = Node[first];

				if (node_destroy == 0 || first >= (int)Node.size()) 
					break;
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					vector<int> int_removed;

					int min_from_pass = node_destroy;
					int max_to_pass = node_destroy;
					do_work1(r, node_destroy, int_removed, min_from_pass);
					
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						r.get_passenger(int_removed[i]).route_before = index;
						passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
						r.erase_passenger(int_removed[i]);
					}

					int index_min_from = 0;
					for (int i = min_from_pass; i >= 0; i--) {
						if (r.get_refueling()[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}

					for (int k = index_min_from; k < max_to_pass; k++) {
						if (r.get_refueling()[k] && r.fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
							do_work6(r, airplane, node_destroy, max_to_pass, k);
						}
					}
					r.removePlace(node_destroy, map_airplane);
				}
				else {
					double fuel_consumed = airplane_consume[r.get_airstrips()[node_destroy - 1]][r.get_airstrips()[node_destroy + 1]];

					if ((fuel_consumed + airplane->min_fuel) <= r.fuel[node_destroy - 1] && 
						r.get_airstrips()[node_destroy - 1] != r.get_airstrips()[node_destroy + 1]) {
						check = false;
						vector<int> int_removed;
						
						int min_from_pass = node_destroy;
						int max_to_pass = node_destroy;
						do_work10(r, node_destroy, int_removed, min_from_pass, max_to_pass);

						for (int i = int_removed.size() - 1; i >= 0; i--) {
							r.get_passenger(int_removed[i]).route_before = index;

							passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
							r.erase_passenger(int_removed[i]);
						}
						r.update_route_destroy(input, node_destroy, min_from_pass, max_to_pass); //update of the time

						int index_before = node_destroy - 1;
						double diff = 0;
						do_work7(r, node_destroy, fuel_consumed, index_before, diff);

						r.removePlace(node_destroy, map_airplane);
						double add_fuel = 0;
						int index_weight_neg = -1;
						do_work8(r, add_fuel, index_weight_neg);
					}
				}
				
				first++;
			} while (check);
		}
		index++;
	}
	return solution;
}

vector<Route> destroy_cluster_aggr2(ProcessedInput* input, const PenaltyWeights& penalty_weights, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution,  vector<Passenger> all_passengers) {

	double peso_TW = penalty_weights.time_window;
	double stop_weight = penalty_weights.intermediate_stop;

	map<int, Passenger> map_id_passenger; 
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();

	const int soglia_relateness = 300; // Prima era 100
	vector<int> int_removed;
	vector<Route> route_destroyed;
	vector<Passenger> passengers;

	unordered_map<int, double> cost_tw_pass;
	set<double, MyCOMP<double>> my_set;

	route_destroyed.insert(route_destroyed.end(), solution.begin(), solution.end());
	passengers.insert(passengers.end(), all_passengers.begin(), all_passengers.end());

	for (Passenger& p : passengers) {
		map_id_passenger[p.pnr] = p;
		cost_tw_pass.insert(make_pair(p.pnr, 0));
	}
	
	for (Route& s : route_destroyed) {
		for (const Passenger& pass : s.get_passengers()) {
			int codpass = pass.pnr;
			cost_tw_pass[codpass] += cost_for_route_passenger_destroyCluster(s, pass, stop_weight, peso_TW);
			my_set.insert(cost_for_route_passenger_destroyCluster(s, pass, stop_weight, peso_TW));
		}
	}
	vector<string> OrderVectore;
	for (auto x : my_set) {
		for (auto y : cost_tw_pass) {
			if (y.second == x) {
				OrderVectore.push_back(to_string(y.first) + "|" + to_string(x));
			}
		}
	}

	map<string, vector<int>> agr_pass;
	vector<string> sequence;
	for (string x : OrderVectore) {

		int codice = stoi(split(x, '|')[0]);
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[codice].origin) + "|" + to_string(map_id_passenger[codice].destination);
		if (agr_pass.find(code) != agr_pass.end()){
			agr_pass[code].push_back(map_id_passenger[codice].pnr);
		}
		else {
			sequence.push_back(code);
			vector<int> id_passeggieri;
			id_passeggieri.push_back(map_id_passenger[codice].pnr);
			agr_pass.insert(make_pair(code, id_passeggieri));
		}
	}

	for (int x = 0; x < (int)sequence.size(); x++) {
		int Codpass = agr_pass[sequence[x]][0];

		// tolgo tutti di questa aggregazione
		for (int p : agr_pass[sequence[x]]) 
			passenger_removed.push_back(map_id_passenger[p]);
		agr_pass.erase(agr_pass.find(sequence[x]));
		sequence.erase(sequence.begin() + x);

		for (int y = 0; y < (int)sequence.size(); y++) {
			// Qui adesso chimao la funzione Relateness !!
			Passenger pass = map_id_passenger[Codpass];
			auto p_r = map_id_passenger[agr_pass[sequence[y]][0]];
			double distances = from_to[p_r.destination][pass.destination] + from_to[p_r.origin][pass.origin];
			double relateness = distances + abs(p_r.arrival_time - pass.arrival_time) + abs(p_r.departure_time - pass.departure_time);

			if (relateness_passenger2(soglia_relateness, relateness, passenger_removed, agr_pass, sequence, y, map_id_passenger))
				y--;
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
		bool update = false;
		int min_from_pass = s.index;
		int max_to_pass = -1;
		Airplane *airplane = &map_airplane[s.aircraft_code];
		
		for (int p = 0; p < (int)s.get_passengers().size(); p++) {
			Passenger passenger = s.get_passengers()[p];
			for (Passenger& pass : passenger_removed) {
				if (s.get_passengers()[p].pnr == pass.pnr) {
					update = true;
					pass.route_before = Nroute;
					min_from_pass = min(min_from_pass, passenger.solution_from);
					max_to_pass = max(max_to_pass, passenger.solution_to);

					int_removed.push_back(p);
					for (int t = s.get_passengers()[p].solution_from; t < passenger.solution_to; t++) {
						s.add_capacity_at(t, -1*passenger.capacity);
						s.get_weight_at(t) += passenger.weight;
					}
				}
			}
		}

		for (int i = int_removed.size() - 1; i >= 0; i--)
			s.erase_passenger(int_removed[i]);

		if (update) {
			int index_min_from = min_from_pass;
			for (int i = min_from_pass; i >= 0; i--) {
				if (s.get_refueling()[i]) {
					index_min_from = i;
					break;
				}
			}
			for (int k = index_min_from; k < max_to_pass; k++) {
				if (s.get_refueling()[k] && s.fuel[k] < airplane->max_fuel) { //&& k!= node_destroy
					int index_updating_from = k;
					int index_updating_to = s.index;
					int node_min = k;
					double min_weight = s.get_weights()[k];
					//appena aggiunto
					for (int i = max_to_pass; i < s.index; i++) {
						if (s.get_refueling()[i]) 
							break;
						max_to_pass = i;
					}

					for (int i = k + 1; i <= max_to_pass; i++) {
						if (s.get_refueling()[i]) 
							break;

						if (s.get_weights()[i] < min_weight) {
							min_weight = s.get_weights()[i];
							node_min = i;
						}
					}
					
					if (node_min >= 0) {
						for (int i = k + 1; i < s.index; i++) {
							if (s.get_refueling()[i]) {
								index_updating_to = i;
								break;
							}
						}

						double fuel_before = s.fuel[index_updating_from];

						s.fuel[index_updating_from] = min(airplane->max_fuel, s.fuel[index_updating_from] + min_weight);
						s.get_weight_at(index_updating_from) -= (s.fuel[index_updating_from] - fuel_before);
						
						for (int i = index_updating_from + 1; i < index_updating_to; i++) {
							s.fuel[i] += (s.fuel[index_updating_from] - fuel_before);
							s.get_weight_at(i) -= (s.fuel[index_updating_from] - fuel_before);
						}
					}
				}
			}

			int index_sup = s.index;
			for (int i = index_sup - 1; i > 1; i--) {
				if (s.get_capacities()[i - 1] != 0)
					break;

				s.remove_at(i);
			}

			if (s.index == 2 && s.get_capacities()[0] == 0) {
				s.remove_at(1);
			}

			// Questa parte qua forse si puo togliere
			//qua ? la parte che ho aggiunto io (NELLI) per il problema del nodo al deposito che non si aggiorna
			if ((int)s.get_airstrips().size() == 1 && s.get_capacities()[0] == 0) {
				s.fuel[0] = airplane->max_fuel;
				s.get_weight_at(0) = airplane->load_weight - airplane->max_fuel;
			}
		}
	}

	Npass = 0;
	for (Route& s : route_destroyed) {
		Npass += (int)(s.get_passengers().size());
		if (s.get_passengers().size() == 0) 
			s.primo_pass = false;
	}

	return route_destroyed;
}

void do_work9(int node_destroy, Route& r, vector<int>& int_removed, int& min_from_pass, int& max_to_pass)
{
	min_from_pass = node_destroy;
	max_to_pass = node_destroy;
	for (int p = 0; p < r.get_passengers().size(); p++) {
		const Passenger& passenger = r.get_passengers()[p];
		if (r.get_passengers()[p].solution_to == node_destroy || passenger. solution_from == node_destroy) { // ho cambiato questa condizione
			min_from_pass = min(min_from_pass, passenger.solution_from);
			max_to_pass = max(max_to_pass, passenger.solution_to);

			int_removed.push_back(p);
			for (int t = r.get_passengers()[p].solution_from; t < passenger.solution_to; t++) {
				r.add_capacity_at(t, -1*r.get_passengers()[p].capacity);
				r.get_weight_at(t) += r.get_passengers()[p].weight;
			}
		}
	}
}

void destroy_ls(ProcessedInput* input, int index, int node_destroy, vector<Passenger>& passenger_removed, Route& r) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	Airplane* airplane = &map_airplane[r.aircraft_code];
	
	if (r.index > 1) {
		bool check = true;
		double fuel_consumed_check = 0.0;
		if ((node_destroy + 2) < r.index) {
			double time_travel = airplane->travelTime(from_to[r.get_airstrips()[node_destroy - 1]][r.get_airstrips()[node_destroy + 2]]);
			fuel_consumed_check = airplane->fuel_burn_first * min(1.0, time_travel/60) + max(0.0, time_travel/60-1)* airplane->fuel_burn_second;
		}
		
		if ((fuel_consumed_check + airplane->min_fuel) <= r.fuel[node_destroy - 1]) {
			check = false;
			int number_initial_node = r.index;
			for (int i = 0; i < 2; i++) {
				if (i == 1 && (number_initial_node - r.index) > 1) 
					break;
				double fuel_consumed = 0.0;
				double time_travel = airplane->travelTime(from_to[r.get_airstrips()[node_destroy - 1]][r.get_airstrips()[node_destroy + 1]]);
				
				fuel_consumed = airplane->fuel_burn_first * min(1.0, time_travel/60) + max(0.0, time_travel/60 - 1)* airplane->fuel_burn_second;

				vector<int> int_removed;
				int min_from_pass;
				int max_to_pass;
				do_work9(node_destroy, r, int_removed, min_from_pass, max_to_pass);

				for (int i = int_removed.size() - 1; i >= 0; i--) {
					r.get_passenger(int_removed[i]).route_before = index;
					passenger_removed.push_back(r.get_passengers()[int_removed[i]]);
					r.erase_passenger(int_removed[i]);
				}

				r.update_route_destroy(input, node_destroy, min_from_pass, max_to_pass); //update of the time
				int index_before = node_destroy - 1;
				double diff = 0;

				for (int i = node_destroy + 1; i < r.index; i++) {

					if (r.get_refueling()[i]) 
						break;

					if (index_before == (node_destroy - 1)) {
						diff = r.fuel[i];
						r.fuel[i] = r.fuel[index_before] - fuel_consumed;
						diff -= r.fuel[i];
					}
					else {
						r.fuel[i] = r.fuel[i] - diff;
					}

					r.get_weight_at(i) = r.get_weights()[i] + diff;
					index_before = i + 1;
				}

				r.removePlace(node_destroy, map_airplane);
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.get_weights()[j] < 0) {
						add_fuel = r.get_weights()[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.get_refueling()[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						for (int t = index_refueling; t < r.index; t++) {

							if (r.get_refueling()[t] && t != index_refueling) break;
							r.fuel[t] += add_fuel;
							r.get_weight_at(t) -= add_fuel;
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
