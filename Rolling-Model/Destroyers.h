#ifndef DESTROYERS_H
#define DESTROYERS_H

//ECCO I NOSTRI DESTROY
#include <unordered_map>

#include "Route.h"


bool relateness_passenger2(int trheshold, vector<vector<double>>& from_to, int idP, Passenger& r, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequenza, string pointer, int y, map<int, Passenger>& map_id_passenger) {
	//cout << " Sono dentro la funzione Relateness " << endl;
	//vector<string> Pass1 = split(idP, '/');
	// Poi questo for si potra togliere
	Passenger pass = map_id_passenger[idP];

	// Questo poi va cambiato
	if (pass.name.size() == 0) {
		cout << " ******************************* ERRORE ***********************************" << endl;
		cout << " Problema con il passeggiero con Pnr: " << idP << endl;
		cin.get();
	}
	double Relateness = from_to[r.arrival_location][pass.arrival_location] + from_to[r.departure_location][pass.departure_location] + abs(r.arrival_time - pass.arrival_time) + abs(r.departure_time - pass.departure_time);
	//cout <<" Relateness ---> " << Relateness << "  Soglia " << trheshold <<endl;
	if (Relateness <= trheshold) {
		/*cout << " Sono dentro Releatensess---> Valore : " << Relateness <<  endl;
		cout << " Sono dentro Releatensess---> Dato il passeggiero : " << endl;
		pass.print();
		cout << " Sono dentro Releatensess---> Devo togliere il passeggiero " << endl;
		r.print();
		cout << " e con lui: ";
		for (string j : agr_pass[pointer]) cout << j << " / " ;
		cout << endl;
		*/
		for (int j : agr_pass[pointer]) passengers_removed.push_back(map_id_passenger[j]);
		//cout << " Numero di passeggieri che sono stati messi nei rimossi RIGA 839 Main " << passengers_removed.size() << endl;
		//cout << " Devo eliminare sia da sequenza sia dalla mappa " << pointer << endl;
		sequenza.erase(sequenza.begin() + y);
		agr_pass.erase(agr_pass.find(pointer));
		//cout << " Sequenza dopo ha: " << sequenza.size() << endl;
		//cout << " Gruppi dopo ha: " << agr_pass.size() << endl;
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
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {

			//genero il numero random di nodi da distruggere da 2 a place.size()-1;
			double numero_random = (double)rand() / RAND_MAX;
			if (r.places.size() == 2) {
				numero_random = round(1 + (numero_random * (r.places.size() - 2)));
				//numero_random = 0;
			}
			else {
				numero_random = round(2 + (numero_random * (r.places.size() - 3)));
			}


			//cout << "sono la route numero: " << index << " il numero random generato �: " << numero_random << " la route ha places.size-1 pari a: " << (r.places.size() - 1) << endl;

			if (numero_random == r.places.size() - 1) {
				//cout << "sono dentro all'if del numero random uguale a places()-1" << endl;
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
					//r.index_cap--;
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
				int nodi_rimossi = 0; //finch� non ho rimosso 
				do {
					//cout << endl;
					//cout << "nodi rimossi: " << nodi_rimossi << endl;
					bool check = true;
					do {
						//if i'm here i can destroy the route
						//at the moment i destroy only a node
						int node_destroy = (int)(round(1 + ((double)rand() / RAND_MAX) * (r.index - 2)));
						//cout << "node destroy: " << node_destroy << endl;
						//cout << "r.index - 1= " << (r.index - 1) << endl;
						if (node_destroy == r.index - 1) { //this in the case of that node is at the end
							check = false;
							//cout << "nodo distrutto sono nel caso della rimozione dell'ultimo nodo: " << node_destroy <<endl;
							//r.print();
							vector<int> int_removed;
							//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
							//cout << " Questa route ha un index = " << r.index << endl; 
							//cout << "route prima della distruzione: " << endl;
							//r.print();

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
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							// Qua devo mettere codice che � quello dell'Update
							/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
							cout << "******************* Stampiamo la Route: *******************" << endl;
							cout << " CODE ------> " << r.aircraft_code << endl;
							cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
							cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
							cout << " Node destroyed ------> " << node_destroy << endl;*/
							int index_min_from = 0;
							for (int i = Min_From_Pass; i >= 0; i--) {
								if (r.refueling[i] && i != node_destroy) {
									index_min_from = i;
									break;
								}
							}
							//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
							//Codice che serve per cercare il minimo nel range
							//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;
							for (int k = index_min_from; k < Max_To_Pass; k++) {
								if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
								//cout << " Sto valutando il caso del nodo " << k << endl;
									int Node_min = k;
									double min_weight = r.weight[k];
									int index_updating_from = k;
									int index_updating_to = r.index;  //qua prima c'era -1
									for (int i = k + 1; i <= Max_To_Pass; i++) {
										//cout << " Sto guardando il nodo " << i << endl;
										//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
										//cout << " Ho passato IF di controllo " << endl;
										if (r.refueling[i]) break;
										if (r.weight[i] < min_weight && i != node_destroy) {
											min_weight = r.weight[i];
											Node_min = i;
										}
										//}
									}
									//cout << " Nodo di minimo ---> " << Node_min << endl;
									//cout << " Valore di minimi --> " << min_weight << endl;
									if (Node_min >= 0) {
										for (int i = k + 1; i < r.index; i++) {
											if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
												index_updating_to = i;
												break;
											}
										}
										//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
										//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
										double Fuel_before = r.quantity_fuel[index_updating_from];
										//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
										//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
										r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
										//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
										//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
										r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
										//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
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
							//cout << "route dopo la distruzione: " << endl;
							//r.print();
							// FORSE QUI CI VUOLE IL CODICE DEI CASI CON PESO NEGATIVO
						}
						else {
							//cout << "sono dentro l'else " << endl;
							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
							//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed;



							if (fuel_consumed <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
								check = false;
								vector<int> int_removed;
								//cout << " route prima di togliere il Nodo: " << node_destroy << endl;
								//r.print();
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

								//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
								//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
								//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
								for (int i = int_removed.size() - 1; i >= 0; i--) {
									// code for repair forbidden***********************************************************
									r.passengers_in_route[int_removed[i]].route_before = index;
									//*************************************************************************************
									passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
									vector<Passenger>::iterator it;
									it = r.passengers_in_route.begin();
									r.passengers_in_route.erase(it + int_removed[i]);
								}

								int nodi_mancanti = (int)r.places.size();
								r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
								//QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA***************************************************************************************

								//double fuel_consumed1 = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy]];

								//double fuel_consumed2 = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy]][r.places[node_destroy + 1]];



								int index_before = node_destroy - 1;
								//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
								//r.print();
								//cout << " Adessos sto iniziando a sistemare i pedi perche togliero nodo " << endl;
								double diff = 0;
								//cout << " Index before: " << index_before << endl;
								for (int i = node_destroy + 1; i < r.index; i++) {
									//cout << " sono al i = " << i << endl;
									if (r.refueling[i]) break;
									//cout << "weight before: " << r.weight[i] << endl;
									if (index_before == (node_destroy - 1)) {
										//cout << " sono al primo giro faccio " << endl;
										diff = r.quantity_fuel[i];
										//cout << " diff: " << diff << endl;
										r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
										diff = diff - r.quantity_fuel[i];
										//cout << " diff aggiornata: " << diff << endl;

									}
									else {
										//cout << " non sono al primo giro " << endl;
										r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
										//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
										//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									}

									if (r.refueling[node_destroy]) {
										//cout << " caso in cui facevo refuiling nel nodo distrutto " << endl;
										//cout << "a r.weight[i]=  " << r.weight[i] << " tolgo questo valore  " << diff << endl;
										r.weight[i] = r.weight[i] + diff;

									}
									else {
										//	cout << " caso odo che ho tolto non era un fuel " << endl;
										//	cout << " weight prima " << r.weight[i] << endl;
										r.weight[i] = r.weight[i] + diff;
										//cout << " weight dopo " << r.weight[i] << endl;
									}


									//cout << "weight after: " << r.weight[i] << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

									//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
									//cout << " inidce before = " << index_before << endl;
									index_before = i + 1;
									//cout << " inidce before = " << index_before << " riparto con un altro giro " << endl;
								}
								//*****************************************************************************************************************************************************************
								r.removePlace(node_destroy, map_airplane);
								nodi_mancanti -= (int)(r.places.size());
								nodi_rimossi += (int)(nodi_mancanti);
								//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weight[j] < 0) {
										//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
										add_fuel = r.weight[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.refueling[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}
										//cout << " Aggiorno da " << index_refueling << endl;
										for (int t = index_refueling; t < r.index; t++) {
											//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
											if (r.refueling[t] && t != index_refueling) break;
											//cout << " Ho passato if aggiorno nodo " << t << endl;
											//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
											r.quantity_fuel[t] += add_fuel;
											r.weight[t] -= add_fuel;
											//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
										}
									}
								}
								//cout << "route dopo la distruzione: " << endl;
								//r.print();
							}
						}
					} while (check);


				} while (nodi_rimossi < numero_random);




			}





		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
	}
	return solution;



}

vector<Route> destroy_casual(double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			bool check = true;
			do {
				//if i'm here i can destroy the route
				//at the moment i destroy only a node
				double n_destroy = (double)rand() / RAND_MAX;
				int node_destroy = (int)(round(1 + n_destroy * (r.index - 2)));
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					//cout << "nodo distrutto: " << node_destroy <<endl;
					//r.print();
					vector<int> int_removed;
					//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
					//cout << " Questa route ha un index = " << r.index << endl; 
					//cout << " siamo nel caso che il nodo distrutto sia l'ultimo " << endl;
					//r.print();

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
								/*
								if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
								r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
								r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
								}
								*/
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
					// Qua devo mettere codice che � quello dell'Update
					/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
					cout << "******************* Stampiamo la Route: *******************" << endl;
					cout << " CODE ------> " << r.aircraft_code << endl;
					cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
					cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
					//r.print();
					cout << " Node destroyed ------> " << node_destroy << endl;
					*/

					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
					// Codice che serve per cercare il minimo nel range
					//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


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
								//cout << " Ho passato il break " << endl;
								if (r.weight[i] < min_weight && i != node_destroy) {
									//	cout << " Ho passato IF di controllo " << endl;
									min_weight = r.weight[i];
									Node_min = i;
								}
							}
							//cout << " Nodo di minimo ---> " << Node_min << endl;
							//cout << " Valore di minimi --> " << min_weight << endl;
							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
								//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
								double Fuel_before = r.quantity_fuel[index_updating_from];
								//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
								//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
								//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
								//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
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
					//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi quando tolgo ultimo nodo " << endl;
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {
							//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
									index_refueling = i;
									break;
								}
							}
							//cout << " Aggiorno da " << index_refueling << endl;
							for (int t = index_refueling; t < r.index; t++) {
								//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
								if (r.refueling[t] && t != index_refueling) break;
								//cout << " Ho passato if aggiorno nodo " << t << endl;
								//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
								//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
							}
						}
					}
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {   //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
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
									/*
									if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
									}
									*/
								}

							}
						}

						/*
						cout << "Nodo: " << node_destroy << endl;
						cout << " Sto tohliendo caso del nodo non ultimo riga 3278 " << endl;
						cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
						cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
						cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
						*/
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
						//r.print();





						int index_before = node_destroy - 1;
						//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
						//r.print();
						//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
						double diff = 0;
						//cout << " Index before: " << index_before << endl;
						for (int i = node_destroy + 1; i < r.index; i++) {
							//cout << " sono al i = " << i << endl;
							if (r.refueling[i]) break;
							//cout << "weight before: " << r.weight[i] << endl;
							if (index_before == (node_destroy - 1)) {
								//cout << " sono al primo giro faccio " << endl;
								diff = r.quantity_fuel[i];
								//cout << " diff: " << diff << endl;
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];
								//cout << " diff aggiornata: " << diff << endl;

							}
							else {
								//cout << " non sono al primo giro " << endl;
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
								//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							}

							//cout << " weight prima " << r.weight[i] << endl;
							r.weight[i] = r.weight[i] + diff;
							//cout << " weight dopo " << r.weight[i] << endl;



							//cout << "weight after: " << r.weight[i] << endl;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

							//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
							//cout << " inidce before = " << index_before << endl;
							index_before = i + 1;
							//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
						}
						//*****************************************************************************************************************************************************************


						r.removePlace(node_destroy, map_airplane);
						//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {
								//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}
								//cout << " Aggiorno da " << index_refueling << endl;
								for (int t = index_refueling; t < r.index; t++) {
									//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
									if (r.refueling[t] && t != index_refueling) break;
									//cout << " Ho passato if aggiorno nodo " << t << endl;
									//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
									//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
								}
							}
						}

						//r.print();
					}
				}

			} while (check);
		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
	}
	return solution;
}

vector<Route> destroy_worst(double peso_TW, double peso_intermediate_stop, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			// Primo elemanto la posizione il secondo il numero del nodo
			map<int, int> Node;
			bool check = true;
			int first = 0;
			do {
				// Qui va messa una funzione ch individua il nodo peggiore
				//cout << " ************** Chiamo funzione che mi calcoli il nodo peggiore **************" << endl;
				Node = Compute_WorstNode(peso_TW, peso_intermediate_stop, r, map_airstrip, from_to);
				int node_destroy = Node[first];
				//cout << " La funzione ha finito togli il nodo: " << node_destroy << endl;
				//cout << " Route " << r.aircraft_code << endl;
				//cout << " Node ha una size di " << Node.size() << endl;
				//cout << " Voglio accedere a first = " << first << endl;
				if (node_destroy == 0 || first >= (int)Node.size()) break;
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					//cout << "nodo distrutto: " << node_destroy <<endl;
					//r.print();
					vector<int> int_removed;
					//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
					//cout << " Questa route ha un index = " << r.index << endl; 
					//r.print();

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
								/*
								if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
								r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
								r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
								}
								*/
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
					// Qua devo mettere codice che � quello dell'Update
					/*
					cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
					cout << "******************* Stampiamo la Route: *******************" << endl;
					cout << " CODE ------> " << r.aircraft_code << endl;
					cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
					cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
					r.print();
					cout << " Node destroyed ------> " << node_destroy << endl;
					*/
					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
					// Codice che serve per cercare il minimo nel range
					//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
								//cout << " Ho passato IF di controllo " << endl;
								if (r.refueling[i]) break;
								if (r.weight[i] < min_weight && i != node_destroy) {
									min_weight = r.weight[i];
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
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
								//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
								double Fuel_before = r.quantity_fuel[index_updating_from];
								//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
								//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
								//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
								//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
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
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {   //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
						check = false;
						vector<int> int_removed;
						//cout << "Nodo: " << node_destroy << endl;
						//r.print();
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
									/*
									if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
									}
									*/
								}

							}
						}




						//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
						//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
						//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
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



						//QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA***************************************************************************************





						int index_before = node_destroy - 1;
						//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
						//r.print();
						//cout << " Adessos sto iniziando a sistemare i pedi perche togliero nodo " << endl;
						double diff = 0;
						//cout << " Index before: " << index_before << endl;
						for (int i = node_destroy + 1; i < r.index; i++) {
							//cout << " sono al i = " << i << endl;
							if (r.refueling[i]) break;
							//cout << "weight before: " << r.weight[i] << endl;
							if (index_before == (node_destroy - 1)) {
								//cout << " sono al primo giro faccio " << endl;
								//cout << "r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed " << r.quantity_fuel[i] <<"="<< r.quantity_fuel[index_before] <<"-"<< fuel_consumed;
								diff = r.quantity_fuel[i];
								//cout << " diff: " << diff << endl;
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];
								//cout << " diff aggiornata: " << diff << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1 + (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);

							}
							else {
								//cout << " non sono al primo giro " << endl;
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
								//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							}

							if (r.refueling[node_destroy]) {
								//cout << " caso in cui facevo refuiling nel nodo distrutto " << endl;
								//cout << " al peso del nodo " << i << " aggiorno con fuel_consumed - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]); " << fuel_consumed << "- " << fuel_consumed2 << " - " << (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]) << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
								r.weight[i] = r.weight[i] + diff;
								//cout << "a r.weight[i]=  " << r.weight[i]<<" tolgo questo valore  " <<diff << endl;
							}
							else {
								//cout << " caso odo che ho tolto non era un fuel " << endl;
								//cout << " al peso del nodo " << i << " aggiorno l'attuale " << r.weight[i]<<" con  + "<< fuel_consumed<<" - "<<fuel_consumed2<<" -"<< fuel_consumed1;
								//cout << " weight prima " << r.weight[i] << endl;
								r.weight[i] = r.weight[i] + diff;
								//cout << " weight dopo " << r.weight[i] << endl;
							}


							//cout << "weight after: " << r.weight[i] << endl;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

							//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
							//cout << " inidce before = " << index_before << endl;
							index_before = i + 1;
							//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
						}
						//*****************************************************************************************************************************************************************


						r.removePlace(node_destroy, map_airplane);
						//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {
								//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}
								//cout << " Aggiorno da " << index_refueling << endl;
								for (int t = index_refueling; t < r.index; t++) {
									//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
									if (r.refueling[t] && t != index_refueling) break;
									//cout << " Ho passato if aggiorno nodo " << t << endl;
									//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
									//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
								}
							}
						}

						//r.print();
					}
				}
				//cout << "  Si Esce quando Check = False in questo caso: " << check << endl;
				//cout << " la posizione " << first << " non va bene mi sposto.. " << endl;
				first++;
			} while (check);
		}
		index++;
	}
	return solution;
}

vector<Route> destroy_cluster_aggr2(double peso_TW, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<Passenger> all_passenegr, map<int, Passenger>& map_id_passenger, double& peso_itermediate_stop) {
	int soglia_relateness = 300; // Prima era 100
	vector<int> int_removed;
	vector<Route> route_destroyed;
	vector<Passenger> passengers;

	unordered_map<int, double> CostTWPass;
	set<double, MyCOMP<double>> Myset;


	//srand(time(NULL));
	route_destroyed.insert(route_destroyed.end(), solution.begin(), solution.end());
	passengers.insert(passengers.end(), all_passenegr.begin(), all_passenegr.end());
	//for (Passenger& p : all_passenegr) passengers.push_back(p);

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
	/*for (auto x : OrderVectore) {
		cout << " ID: " << split(x, '|')[0] << " con costo: " << split(x, '|')[1] << endl;
		//cout << x << endl;
	}
	system("pause");*/
	map<string, vector<int>> agr_pass;
	vector<string> sequenza;
	for (string x : OrderVectore) {
		//cout << " Devo inserire dentro la mappa che non dovrebbe essere ordinata " << x << endl;
		//cout << "sono split(x, '|')[0]: " << split(x, '|')[0] << endl;
		//system("pause");
		int codice = stoi(split(x, '|')[0]);
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[codice].departure_location) + "|" + to_string(map_id_passenger[codice].arrival_location);
		if (agr_pass.find(code) != agr_pass.end())
		{
			// c'� gia
			agr_pass[code].push_back(map_id_passenger[codice].pnr);
		}
		else {
			// non c'� gia
			sequenza.push_back(code);
			vector<int> id_passeggieri;
			id_passeggieri.push_back(map_id_passenger[codice].pnr);
			agr_pass.insert(make_pair(code, id_passeggieri));
		}

	}

	/*for (string s : sequenza) cout << s << endl;
	for (auto x : agr_pass) {
		cout << " Con queste caratteristiche: " << x.first << " abbiamo : ";
		for (string k : x.second) {
			map_id_passenger[k].print();
			cout << endl;
		}
	}


	//cout << " Aggregati numero di passeggieri pari a ";
	int ndisa = 0;
	for (auto x : agr_pass) ndisa += (int)x.second.size();
	cout << ndisa << endl;
	//cout << " Aggregati numero di passeggieri pari a " << agr_pass.size() << endl;

	*/


	for (int x = 0; x < (int)sequenza.size(); x++) {
		int Codpass = agr_pass[sequenza[x]][0];
		//cout << " Adesso considero String CODPASS da passare alla relateness: " << Codpass << endl;
		//cout << " Tolgo tutti i passeggieri di: " << sequenza[x] << endl;
		// tolgo tutti di questa aggregazione
		for (int p : agr_pass[sequenza[x]]) passenger_removed.push_back(map_id_passenger[p]);
		//cout << " Numero passeggieri rimossi = " << passenger_removed.size() << endl;
		agr_pass.erase(agr_pass.find(sequenza[x]));
		sequenza.erase(sequenza.begin() + x);
		//cout << " Numero di sequenze e grurroi  = " << sequenza.size() << " e " << agr_pass.size()<<  endl;
		for (int y = 0; y < (int)sequenza.size(); y++) {
			//cout << " Sto per chimare funzione relateness per il passeggiero " << i << endl;
			//passengers[i].print();
			// Qui adesso chimao la funzione Relateness !! 
			//cout << " Adesso devo fare il conftonto con  "<< sequenza[y] << endl;
			bool retrocedi = relateness_passenger2(soglia_relateness, from_to, Codpass, map_id_passenger[agr_pass[sequenza[y]][0]], passenger_removed, agr_pass, sequenza, sequenza[y], y, map_id_passenger);
			if (retrocedi) y--;
			//cout << " Abbiamo Valore indice x: " << x << " e Y: " << y << " su: " << sequenza.size() << endl;
		}
		//cout << " Fin'ora ho tolto numero odi passeggieri ----> " << passenger_removed.size() << " su " << num_passenger << endl;
		x--;
		if ((int)passenger_removed.size() >= num_passenger) break;
	}

	//int NRimossi = 0;
	int Npass = 0;
	int Nroute = -1;
	//for (Route r : route_destroyed) Npass += r.passengers_in_route.size();
	// Devo toglierli dalla route 
	for (Route& s : route_destroyed) {
		Nroute++;
		vector<int> int_removed;
		bool Update = false;
		int Min_From_Pass = s.index;
		int Max_To_Pass = -1;
		for (int p = 0; p < (int)s.passengers_in_route.size(); p++) {
			for (Passenger& pass : passenger_removed) {
				//if (s.passengers_in_route[p].name == pass.name && s.passengers_in_route[p].surname == pass.surname && s.passengers_in_route[p].pnr == pass.pnr && s.passengers_in_route[p].code_flight == pass.code_flight && s.passengers_in_route[p].gender == pass.gender  && s.passengers_in_route[p].arrival_location == pass.arrival_location  && s.passengers_in_route[p].departure_location == pass.departure_location && s.passengers_in_route[p].weight == pass.weight) {
				if (s.passengers_in_route[p].pnr == pass.pnr) {
					Update = true;
					pass.route_before = Nroute;
					if (s.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = s.passengers_in_route[p].solution_from;
					if (s.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = s.passengers_in_route[p].solution_to;
					//cout << " devo rimuovere passeggiero dalla route dell'aereo " << s.aircraft_code << endl;
					//s.passengers_in_route[p].print();
					int_removed.push_back(p);
					for (int t = s.passengers_in_route[p].solution_from; t < s.passengers_in_route[p].solution_to; t++) {
						s.capacity[t] -= s.passengers_in_route[p].capacity;
						s.weight[t] += s.passengers_in_route[p].weight;
					}

				}
			}


		}
		for (int i = int_removed.size() - 1; i >= 0; i--) {
			//cout << int_removed[i] << endl;
			vector<Passenger>::iterator it;
			it = s.passengers_in_route.begin();
			s.passengers_in_route.erase(it + int_removed[i]);
			//cout << " Route adesso ha ---> " << s.passengers_in_route.size() << " passeggieri" << endl;
			//cout << " Stampo passeggieri dopo rimozione: " << endl;
			//for (Passenger passeggiero : s.passengers_in_route) passeggiero.print(); 
		}

		if (Update) {


			// Codice che da reduel ai nodi con refuel non massimo 
			int index_min_from = Min_From_Pass;
			for (int i = Min_From_Pass; i >= 0; i--) {
				if (s.refueling[i]) {
					index_min_from = i;
					break;
				}
			}
			for (int k = index_min_from; k < Max_To_Pass; k++) {
				if (s.refueling[k] && s.quantity_fuel[k] < map_airplane[s.aircraft_code].max_fuel) { //&& k!= node_destroy
				//cout << " Sto valutando il caso del nodo " << k << endl;
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
						//cout << " Sto guardando il nodo " << i << endl;
						//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
						//cout << " Ho passato IF di controllo " << endl;
						if (s.refueling[i]) break;
						if (s.weight[i] < min_weight) {
							min_weight = s.weight[i];
							Node_min = i;
						}
						//}
					}
					if (Node_min >= 0) {
						for (int i = k + 1; i < s.index; i++) {
							if (s.refueling[i]) {  // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
								index_updating_to = i;
								break;
							}
						}
						//cout << " Minimo trovato " << min_weight << endl;
						//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
						//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
						double Fuel_before = s.quantity_fuel[index_updating_from];
						//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[aircraft_code].max_fuel << " e " << quantity_fuel[index_updating_from] + min_weight << endl;
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
			//qua � la parte che ho aggiunto io (NELLI) per il problema del nodo al deposito che non si aggiorna
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
	//cout << " Numero di passeggieri nella route dopo averli toglierli " << Npass << endl;



	return route_destroyed;

}


//************************************************************************************** Local Search **************************************************************
void destroy_ls(int index, int node_destroy, vector<Passenger>& passenger_removed, Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
	//cout << " Stampo la route prima di toccarla: " << endl;
	//r.print();
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
			//cout << " Deve valere che: " << fuel_consumed_check << " + " << map_airplane[r.aircraft_code].min_fuel << " <= " << r.quantity_fuel[node_destroy - 1] << endl;
		}
		if ((fuel_consumed_check + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
			check = false;
			int number_initial_node = r.index;
			for (int i = 0; i < 2; i++) {
				//cout << " Sono al giro i -> " << i << endl;
				if (i == 1 && (number_initial_node - r.index) > 1) break;
				//cout << " devo distruggere il nodo -> " << node_destroy << endl;
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
						//cout << " Aggiungo alla lista dei passegggieri il passeggiero ";
						//r.passengers_in_route[p].print();
						int_removed.push_back(p);
						for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
							r.capacity[t] -= r.passengers_in_route[p].capacity;
							r.weight[t] += r.passengers_in_route[p].weight;
							/*
							if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
							r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
							r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
							}
							*/
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
				//cout << " Sono alla riga 5946 dentro al detroy ho numero di passeggieri rimossi pari a " << passenger_removed.size() << endl;
				r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
				//r.print();
				int index_before = node_destroy - 1;
				//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
				//r.print();
				//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
				double diff = 0;
				//cout << " Index before: " << index_before << endl;
				for (int i = node_destroy + 1; i < r.index; i++) {
					//cout << " sono al i = " << i << endl;
					if (r.refueling[i]) break;
					//cout << "weight before: " << r.weight[i] << endl;
					if (index_before == (node_destroy - 1)) {
						//cout << " sono al primo giro faccio " << endl;
						diff = r.quantity_fuel[i];
						//cout << " diff: " << diff << endl;
						r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
						diff = diff - r.quantity_fuel[i];
						//cout << " diff aggiornata: " << diff << endl;

					}
					else {
						//cout << " non sono al primo giro " << endl;
						r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
						//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
					}
					//cout << " weight prima " << r.weight[i] << endl;
					r.weight[i] = r.weight[i] + diff;
					index_before = i + 1;
					//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
				}
				//*****************************************************************************************************************************************************************

				//cout << " Sono dentro al codice del destroy della ricerca locale sto per fare remove place " << endl;
				//cout << " Devo togliere nodo " << r.places[node_destroy] << endl;
				//r.print();
				r.removePlace(node_destroy, map_airplane);
				//cout << " Ho finito rimozione " << endl;
				//r.print();
				//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weight[j] < 0) {
						//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						//cout << " Aggiorno da " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Ho passato if aggiorno nodo " << t << endl;
							//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
						}
					}
				}

			}
			//r.print();
		}

		if (check) {
			r.index = -1;
			//cout << " Attenzione � il caso che non puo toglire il nodo quello dove prima si bloccava " << endl;
			//cout << " Nelle prossime stampe questo giro di A lo devo saltare devo comparire al prossimo A " << endl;
			//system("pause");
		}
	}
	//cout << "passengers removed" << endl;
	//for (Passenger p : passenger_removed) p.print();
}

#endif