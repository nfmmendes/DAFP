#ifndef CONSTRUCTIVE_H
#define CONSTRUCTIVE_H

//*************************************************************************************COSTRUTTIVI************************************************************************
#include <iostream>
#include <vector>
#include "Evaluate.h"
#include "Route.h"

vector<Route> heuristic_costructive_second_fase(vector<Route>& solution, double end_day, double peso_TW) {
	vector<Route> sol;

	for (Route& r : solution) {
		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, peso_TW);
		if (cost_time_windows_for_route(r, peso_TW) != 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			for (int i = 0; i < r.index - 1; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
				double wait_time = 0;
				if (i == 0) {
					do {
						for (int j = 0; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;  //prima c'era ++ per ottimizzare ho messo pi� due in modo da valutare ogni due minuti non ogni minuto
					} while (wait_time <= 60);
				}
				else {
					do {
						r_support.time_dep[i] += 2;
						for (int j = i + 1; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;
					} while (wait_time <= 60);
				}
				r_support = best_route;
			}
		}
		sol.push_back(best_route);
	}
	return sol;
}

vector<Route> heuristic_costructive_second_fase_SP(vector<Route>& solution, double end_day, double peso_TW) {
	vector<Route> sol;

	for (Route& r : solution) {
		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, peso_TW);
		if (cost_time_windows_for_route(r, peso_TW) != 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			for (int i = 0; i < r.index - 1; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
				double wait_time = 0;
				if (i == 0) {
					do {
						for (int j = 0; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;  //prima c'era ++ per ottimizzare ho messo pi� due in modo da valutare ogni due minuti non ogni minuto
					} while (wait_time <= 30);
				}
				else {
					do {
						r_support.time_dep[i] += 2;
						for (int j = i + 1; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;
					} while (wait_time <= 30);
				}
				r_support = best_route;
			}
		}
		sol.push_back(best_route);
	}
	return sol;
}

vector<Route> heuristic_costructive_first_fase(double peso_TW, double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);

	for (auto p : passengers) {
		int situation = -1;
		int best_route = -1; //index of the best route where can i put my passenger
		int best_from = -1;
		int best_to = -1;
		double best_cost = DBL_MAX;
		int i = 0;
		//cout << "- sto calcolando dove metttere il passeggero: " << p.name << endl;
		//qui calcolo solo dove mettere il passeggero
		for (auto r : solution) {
			if (r.primo_pass == false) {
				if (r.places[r.index - 1] == p.departure_location) {
					//in questo caso c'� solo lui nella route, il costo � dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
					//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


					cost += fuel_consumed;
					//per il check sul fuel:
					double fuel_after_trip = 0.0;
					fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
					//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
							best_cost = cost;
							best_route = i;
							situation = 1;
						}
					}
				}
				else {
					//qui c'� solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi� kilometri e un landing stop
					//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
					//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					cost += fuel_consumed;

					//per il check sul fuel
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
							if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = 2;
							}
						}
					}
				}
			}
			else {
				//nella route c'� almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit� della route
				//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'� il FROM, non cerco il TO
				vector<int> FROM;
				vector<int> TO;
				for (int t = 0; t < r.index; t++) {
					if (r.places[t] == p.departure_location) FROM.push_back(t);
				}

				if (FROM.size() != 0) {
					//dentro questo abbiamo trovato dei FROM 
					for (int t = FROM[0]; t < r.index; t++) {
						if (r.places[t] == p.arrival_location) TO.push_back(t);
					}

					if (TO.size() != 0) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from < to && to - from <= p.stop) {
									//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
									//se sono qua dentro il to � maggiore del from e gli stop intermedi sono minori di tre
									//per prima cosa guardo se ci sto con la capacit�, altrimenti break sul primo for
									bool capacity_satisfy = true;
									for (int c = from; c < to; c++) {
										if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
										if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

										//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

									}
									//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
									if (!capacity_satisfy) break; //check also for the fuel;

									//se sorpasso il break vuol dire che soddisfo la capacit�, in questo caso il solo costo che consider� per l'aggiunta del passeggero qui
									//� la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
									else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

									double TW_arrival = 0.0;
									//double t_arr_arrival = r.time_arr[to];
									if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
									else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

									double cost = ((TW_departure + TW_arrival) * peso_TW + ((to - from - 1) * peso_intermediate_stop)) * p.capacity;
									if (best_cost > cost) {
										best_cost = cost;
										best_route = i;
										situation = 4;
										best_from = from;
										best_to = to;
									}
								}
							}
						}
					}
					//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
					//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
					if (r.places[r.index - 1] != p.arrival_location) {
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (int c = from; c < r.index; c++) {
								if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

								if (c < r.index - 1) {
									//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

									if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
								}
							}
							//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
							//cout << p.stop<< endl;
							if (capacity_satisfy && (r.index - from) <= p.stop) {

								double TW_departure = 0.0;
								//double t_arr_departure = r.time_arr[from];
								if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
								else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

								double cost = (TW_departure * peso_TW);
								// ora ci metto chilometri e fuel dell'ultimo pezzo;
								cost += from_to[r.places[r.index - 1]][p.arrival_location];
								double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];

								cost += fuel_consumed;

								bool fuel_ok = true;
								if (!r.refueling[r.index - 1]) {
									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
									if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
								}

								double TW_arrival = 0.0;
								double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
								if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
								else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
								cost += (TW_arrival * peso_TW) * p.capacity;

								cost += (peso_intermediate_stop * (r.index - from - 1)) * p.capacity;

								if (fuel_ok) {
									if (best_cost > cost) {
										if (t_arr_arrival <= end_day) {
											if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
												best_cost = cost;
												best_route = i;
												best_from = from;
												situation = 5;
											}

										}
									}
								}
							}
						}
					}

				}
				if (r.places[r.index - 1] != p.departure_location) {
					//guardo il costo di aggiungere un pezzo alla fine
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

					double TW_departure = 0.0;
					double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = 3;
								}
							}
						}
					}

				}
			}
			i++;
		}

		if (situation == 1) {
			solution[best_route].primo_pass = true;
			//time
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto � la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1] += p.capacity;

			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}

			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == 2) {
			solution[best_route].primo_pass = true;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 -
				map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];

				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == 3) {

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity,
					solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;


			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];
				
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}


			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 4) {
			for (int h = best_from; h < best_to; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;
			}

			//**************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}

					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
					}
				}
			}

			p.solution_from = best_from;
			p.solution_to = best_to;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 5) {


			for (int h = best_from; h < solution[best_route].index; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;


			}

			//******************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
					}
				}
			}
			//******************************************************************************

			p.solution_from = best_from;
			int place_1 = solution[best_route].places[solution[best_route].index - 1];
			double aircraft_speed = map_airplane[solution[best_route].aircraft_code].speed;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}

			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == -1) {
			cout << "*******************************************************il passeggero: " << p.name << " non trova sistemazione" << endl;
			//p.print();
			//for (Route r : solution) r.print();
			//system("pause");
		}
	}


	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}


	return solution_clean;
}

vector<Route> heuristic_costructive_first_fase_sequential(double peso_TW, double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger> passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
	//now i have created only the possible route, now i that one of them and fill it untill the condition stop

	for (Route& r : solution) {
		//cout << "sto valutando la route *******************************************************************************************************************************************************************" << endl;
		int situation = -1;
		do {
			situation = -1; //se situation rimane error vuol dire che nessun passeggero si pu� inserire, quindi che la route � piena e devo passare ad un'altra
			int best_passenger = -1;
			int best_from = -1;
			int best_to = -1;
			double best_cost = DBL_MAX;
			//cout << "sto valutando nella route " << endl;
			//r.print();
			//cout << "passengers.size()= " << passengers.size() << endl;
			for (int p = 0; p < (int)passengers.size(); p++) {
				if (r.primo_pass == false) {
					if (r.places[r.index - 1] == passengers[p].departure_location) {
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[passengers[p].departure_location][passengers[p].arrival_location];
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;

						//cout << "validit� caso A" << endl;
						//cout << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;

						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
								best_cost = cost;
								best_passenger = p;
								situation = 1;
								//cout << "situation A, cost: " << cost << "passenger: " << p << endl;
							}
						}
					}
					else {
						//qui c'� solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi� kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[passengers[p].departure_location][passengers[p].arrival_location] + from_to[r.places[r.index - 1]][passengers[p].departure_location];
						//double travel_time = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location] + from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];

						cost += fuel_consumed;

						//per il check sul fuel
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];


						//****
						//double travel_secondo_tratto = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];

						//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "validit� caso B" << endl;
						//cout << best_cost << " > " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;


						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
									best_cost = cost;
									best_passenger = p;
									situation = 2;
									//cout << "situation B, cost: " << cost << "passenger: " << p << endl;
								}
							}
						}
					}
				}
				else {
					//nella route c'� almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit� della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'� il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = 0; t < r.index; t++) {
						if (r.places[t] == passengers[p].departure_location) FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++) {
							if (r.places[t] == passengers[p].arrival_location) TO.push_back(t);
						}

						if (TO.size() != 0) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= passengers[p].stop) {
										//se sono qua dentro il to � maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacit�, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][passengers[p].arrival_location];

											if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

										}
										//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;

										//se sorpasso il break vuol dire che soddisfo la capacit�, in questo caso il solo costo che consider� per l'aggiunta del passeggero qui
										//� la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

										double TW_departure = 0.0;
										double t_arr_departure = r.time_arr[from];
										if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
										else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

										double TW_arrival = 0.0;
										double t_arr_arrival = r.time_arr[to];
										if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
										else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;

										double cost = ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;
										cost += (peso_intermediate_stop * (to - from - 1));

										//cout << "if del caso D " << best_cost << " > " << cost << endl;
										if (best_cost > cost) {
											best_cost = cost;
											best_passenger = p;
											situation = 4;
											best_from = from;
											best_to = to;
											//cout << "situation D, cost: " << cost << "passenger: " << p << endl;
										}
									}
								}
							}
						}
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
						//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.places[r.index - 1] != passengers[p].arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
								if (capacity_satisfy && (r.index - from) <= passengers[p].stop) {

									double TW_departure = 0.0;
									double t_arr_departure = r.time_arr[from];
									if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
									else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

									double cost = (TW_departure * peso_TW);
									// ora ci metto chilometri e fuel dell'ultimo pezzo;
									cost += from_to[r.places[r.index - 1]][passengers[p].arrival_location];
									double travel_time = (from_to[r.places[r.index - 1]][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].arrival_location];

									cost += fuel_consumed;

									bool fuel_ok = true;
									if (!r.refueling[r.index - 1]) {
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
										if (r.weight[r.index - 1] - passengers[p].weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
									}

									double TW_arrival = 0.0;
									double t_arr_arrival = 0.0;
									t_arr_arrival = r.time_arr[r.index - 1] + (travel_time * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
									else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
									cost += ((TW_arrival * peso_TW)) * passengers[p].capacity;
									cost += (peso_intermediate_stop * (r.index - from - 1)) * passengers[p].capacity;

									//cout << "if caso E" << fuel_ok << " && " << best_cost << " > " << cost << " && " << t_arr_arrival << " <= " << end_day << " && " << r.quantity_fuel[r.index - 1] - fuel_consumed << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;
									if (fuel_ok) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
													best_cost = cost;
													best_passenger = p;
													best_from = from;
													situation = 5;
												}
											}
										}
									}
								}
							}
						}
						//ho aggiunto questo if che c'� quaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
					}
					if (r.places[r.index - 1] != passengers[p].departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location] + from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location] + from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][passengers[p].departure_location] + from_to[passengers[p].departure_location][passengers[p].arrival_location];

						double TW_departure = 0.0;
						//double t_arr_departure = r.time_arr[r.index - 1] + (map_airstrip[r.places[r.index - 1]].ground_time + (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60;
						double t_arr_departure = r.time_dep[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
						else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[passengers[p].departure_location].ground_time + (((from_to[passengers[p].departure_location][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
						else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
						cost += ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						
						//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}


						//cout << "if caso C " << best_cost << " > " << cost << " && " << t_arr_arrival << " <= " << end_day << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;
						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
										best_cost = cost;
										best_passenger = p;
										situation = 3;
									}
								}
							}
							//cout << "situation C, cost: " << cost << "passenger: " << p << endl;
						}

					}
				}
			}

			if (situation == 1) {
				r.primo_pass = true;
				//time
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure; // in questo caso anche l'arrivo in quanto � la partenza dal depot

				passengers[best_passenger].solution_from = r.index - 1;
				r.capacity[r.index - 1] += passengers[best_passenger].capacity;

				//for the weight
				r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
					r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				if (r.weight[r.index - 1] < 0) {
					r.quantity_fuel[r.index - 1] += r.weight[r.index - 1];
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;
				}

				double travel_time = ((from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60;
				double time_from = r.time_dep[r.index - 1];
				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location];

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people - map_airplane[r.aircraft_code].max_fuel;
					//end for weight
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;
					//end for weight

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);

			}
			else if (situation == 2) {
				r.primo_pass = true;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure - (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60 -
					map_airstrip[r.places[r.index - 1]].ground_time;
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure - (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;

				//for the weight
				r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
					r.quantity_fuel[r.index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				//end for weight

				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];
				if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, passengers[best_passenger].capacity, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];

					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, passengers[best_passenger].capacity, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_from = r.index - 1;
				double time = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time + (from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel;
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == 3) {

				if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, passengers[best_passenger].capacity,
						r.time_dep[r.index - 1] +
						((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60),
						r.time_dep[r.index - 1] + ((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];

					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, passengers[best_passenger].capacity, r.time_dep[r.index - 1] +
						((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60),
						r.time_dep[r.index - 1] + ((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_from = r.index - 1;
				double time = r.time_dep[r.index - 1] + ((from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60);

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel;

				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;


					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}

				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == 4) {
				for (int h = best_from; h < best_to; h++) {
					r.capacity[h] += passengers[best_passenger].capacity;
					r.weight[h] -= passengers[best_passenger].weight;
				}

				//**************************************************************************
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = best_from; j < r.index; j++) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					if (r.weight[j] < 0) {
						//cout << " Weigth negativo in " << j << weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Weight dopo " << weight[t] << endl;
						}
					}
				}

				passengers[best_passenger].solution_from = best_from;
				passengers[best_passenger].solution_to = best_to;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == 5) {
				for (int h = best_from; h < r.index; h++) {
					r.capacity[h] += passengers[best_passenger].capacity;
					r.weight[h] -= passengers[best_passenger].weight;
				}


				//******************************************************************************
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = best_from; j < r.index; j++) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					if (r.weight[j] < 0) {
						//cout << " Weigth negativo in " << j << weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Weight dopo " << weight[t] << endl;
						}
					}
				}

				passengers[best_passenger].solution_from = best_from;
				int place_1 = r.places[r.index - 1];
				double aircraft_speed = map_airplane[r.aircraft_code].speed;
				double time = r.time_dep[r.index - 1] + ((from_to[place_1][passengers[best_passenger].arrival_location] / aircraft_speed) * 60);

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel;

				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}

				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);

			}
			
			if (situation != -1) passengers.erase(passengers.begin() + best_passenger);
			
		} while (situation != -1);
		if (passengers.size() == 0) break;
	}


	std::vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}

	cout << "ecco i passeggeri rimasti" << endl;
	for (Passenger rimasto : passengers) rimasto.print();

	return solution_clean;
}


double costo_senza_time_windows(vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		cost += map_airplane[r.aircraft_code].fixed_cost;
		cost_route += map_airplane[r.aircraft_code].fixed_cost;

		for (int i = 1; i < r.index; i++) {
			cost += map_airstrip[r.places[i]].landing_cost;
			cost_route += map_airstrip[r.places[i]].landing_cost;
		}

		//second of all calculate the cost of the Km and the fuel burn
		double mileage = 0.0;
		double fuel_consumed = 0.0;
		for (int i = 0; i < r.index - 1; i++) {
			//double time_travel = 0.0;
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}
		}
		
		cost += mileage + fuel_consumed;
		cost_route += mileage + fuel_consumed;
	}

	return cost;


};

double costo_time_windows_and_intermediate_stop(double peso_TW, double peso_intermediate_stop, vector<Route>& solution) {
	double cost = 0.0;

	for (auto& r : solution) {

		for (const auto& p : r.passengers_in_route) {
			double time_departure = r.time_arr[p.solution_from];
			double time_arrival = r.time_arr[p.solution_to];

			double TW_departure = 0.0;
			if (time_departure < p.early_departure) TW_departure = p.early_departure - time_departure;
			else if (time_departure > p.late_departure) TW_departure = time_departure - p.late_departure;

			double TW_arrival = 0.0;
			if (time_arrival < p.early_arrival) TW_arrival = p.early_arrival - time_arrival;
			else if (time_arrival > p.late_arrival) TW_arrival = time_arrival - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost += (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;
		}
	}

	return cost;
}



void heuristic_costructive_first_fase_secIter(double peso_TW, double peso_intermediate_stop, vector<Route>& solution, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//cout << " ----------------- Sono al secondo giro adesso ho gia soddisfatto alcuni passeggieri me ne mancano alcuni -----------------" << endl;
	int situation = -1;
	int cont = 0;
	vector<int> AereiUsati;
	for (Route& r : solution) AereiUsati.push_back(r.aircraft_code);
	//cout << "  Stringa: " << AereiUsati << endl;
	do {
		std::vector<int>::iterator it = std::find(AereiUsati.begin(), AereiUsati.end(), airplanes[cont].code);
		if (it == AereiUsati.end()) {
			vector<Passenger> pass_in_solution;
			Route r(airplanes[cont].code, pass_in_solution);
			r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
			solution.push_back(r);
		}
		cont++;
	} while (cont < number_of_aircraft);

	for (auto& p : passengers) {
		situation = -1;
		int best_route = -1; //index of the best route where can i put my passenger
		int best_from = -1;
		int best_to = -1;
		double best_cost = DBL_MAX;
		int i = 0;
		//cout << "- sto calcolando dove metttere il passeggero: " << p.name << endl;
		//qui calcolo solo dove mettere il passeggero
		for (auto& r : solution) {
			if (r.primo_pass == false) {
				if (r.places[r.index - 1] == p.departure_location) {
					//in questo caso c'� solo lui nella route, il costo � dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
					//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					
					cost += fuel_consumed;
					//per il check sul fuel:
					double fuel_after_trip = 0.0;
					fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
					//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
							best_cost = cost;
							best_route = i;
							situation = 1;
						}
					}
				}
				else {
					//qui c'� solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi� kilometri e un landing stop
					//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
					//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					cost += fuel_consumed;

					//per il check sul fuel
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
							if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = 2;
							}
						}
					}
				}
			}
			else {
				//nella route c'� almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit� della route
				//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'� il FROM, non cerco il TO
				vector<int> FROM;
				vector<int> TO;
				for (int t = 0; t < r.index; t++) {
					if (r.places[t] == p.departure_location) FROM.push_back(t);
				}

				if (FROM.size() != 0) {
					//dentro questo abbiamo trovato dei FROM 
					for (int t = FROM[0]; t < r.index; t++) {
						if (r.places[t] == p.arrival_location) TO.push_back(t);
					}

					if (TO.size() != 0) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from < to && to - from <= p.stop) {
									//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
									//se sono qua dentro il to � maggiore del from e gli stop intermedi sono minori di tre
									//per prima cosa guardo se ci sto con la capacit�, altrimenti break sul primo for
									bool capacity_satisfy = true;
									for (int c = from; c < to; c++) {
										if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
										if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

										//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

									}
									//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
									if (!capacity_satisfy) break; //check also for the fuel;

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
									else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

									double TW_arrival = 0.0;
									//double t_arr_arrival = r.time_arr[to];
									if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
									else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

									double cost = (((TW_departure + TW_arrival) * peso_TW) + ((to - from - 1) * peso_intermediate_stop)) * p.capacity;
									if (best_cost > cost) {
										best_cost = cost;
										best_route = i;
										situation = 4;
										best_from = from;
										best_to = to;
									}
								}
							}
						}
					}
					//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
					//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
					if (r.places[r.index - 1] != p.arrival_location) {
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (int c = from; c < r.index; c++) {
								if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

								if (c < r.index - 1) {
									//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

									if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
								}
							}
							//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
							//cout << p.stop<< endl;
							if (capacity_satisfy && (r.index - from) <= p.stop) {

								double TW_departure = 0.0;
								//double t_arr_departure = r.time_arr[from];
								if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
								else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

								double cost = (TW_departure * peso_TW);
								// ora ci metto chilometri e fuel dell'ultimo pezzo;
								cost += from_to[r.places[r.index - 1]][p.arrival_location];
								double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];
								cost += fuel_consumed;

								bool fuel_ok = true;
								if (!r.refueling[r.index - 1]) {
									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
									if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
								}

								double TW_arrival = 0.0;
								double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
								if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
								else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
								cost += (TW_arrival * peso_TW) * p.capacity;

								cost += ((peso_intermediate_stop * (r.index - from - 1))) * p.capacity;

								if (fuel_ok) {
									if (best_cost > cost) {
										if (t_arr_arrival <= end_day) {
											if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
												best_cost = cost;
												best_route = i;
												best_from = from;
												situation = 5;
											}

										}
									}
								}
							}
						}
					}

				}
				if (r.places[r.index - 1] != p.departure_location) {
					//guardo il costo di aggiungere un pezzo alla fine
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

					double TW_departure = 0.0;
					double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					
					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = 3;
								}
							}
						}
					}

				}
			}
			i++;
		}

		if (situation == 1) {
			auto* best_route_ptr = &solution[best_route]; 
			best_route_ptr->primo_pass = true;
			//time
			best_route_ptr->time_dep[best_route_ptr->index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto � la partenza dal depot

			p.solution_from = best_route_ptr->index - 1;
			best_route_ptr->capacity[best_route_ptr->index - 1] += p.capacity;

			//for the weight
			best_route_ptr->weight[best_route_ptr->index - 1] = map_airplane[best_route_ptr->aircraft_code].weight_fuel_people -
				best_route_ptr->quantity_fuel[best_route_ptr->index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (best_route_ptr->weight[best_route_ptr->index - 1] < 0) {
				best_route_ptr->quantity_fuel[best_route_ptr->index - 1] += best_route_ptr->weight[best_route_ptr->index - 1];
				best_route_ptr->weight[best_route_ptr->index - 1] = map_airplane[best_route_ptr->aircraft_code].weight_fuel_people -
					best_route_ptr->quantity_fuel[best_route_ptr->index - 1] - p.weight;
			}
			//end for weight

			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = best_route_ptr->time_dep[best_route_ptr->index - 1];
			double fuel_consumed = from_to_FuelConsumed[best_route_ptr->aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				best_route_ptr->addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[best_route_ptr->aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				best_route_ptr->weight[solution[best_route].index - 1] = map_airplane[best_route_ptr->aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == 2) {
			solution[best_route].primo_pass = true;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 -
				map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;




			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);




		}
		else if (situation == 3) {

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity,
					solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;


			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60);





			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}


			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 4) {
			for (int h = best_from; h < best_to; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;
			}

			//**************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
					}
				}
			}
			//**************************************************************************

			p.solution_from = best_from;
			p.solution_to = best_to;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 5) {
			for (int h = best_from; h < solution[best_route].index; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;
			}

			//******************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//******************************************************************************

			p.solution_from = best_from;
			int place_1 = solution[best_route].places[solution[best_route].index - 1];
			double aircraft_speed = map_airplane[solution[best_route].aircraft_code].speed;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}

			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == -1) { }
	}

	vector<int> Elimina;
	for (int s = 0; s < (int)solution.size(); s++) {
		if (solution[s].index <= 1) {
			Elimina.push_back(s);
		}
	}

	for (int i = (int)(Elimina.size()) - 1; i >= 0; i--) {
		solution.erase(solution.begin() + Elimina[i]);
	}
}


#endif