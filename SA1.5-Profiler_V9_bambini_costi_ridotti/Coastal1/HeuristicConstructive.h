#ifndef HEURISTIC_CONSTRUCTIVE_H
#define HEURISTIC_CONSTRUCTIVE_H
#include <float.h>
#include <iostream>
#include "Route.h"

using Double3DVector = vector<vector<vector<double>>>;

vector<Route> heuristic_costructive_first_fase(double peso_TW, double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed) {
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

		//qui calcolo solo dove mettere il passeggero
		for (auto r : solution) {
			if (r.primo_pass == false) {
				if (r.places[r.index - 1] == p.departure_location) {
					//in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
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
					//qui c'? solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi? kilometri e un landing stop
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

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
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
				//nella route c'? almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit? della route
				//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'? il FROM, non cerco il TO
				vector<int> FROM;
				vector<int> TO;
				for (int t = 0; t < r.index; t++) {
					if (r.places[t] == p.departure_location)
						FROM.push_back(t);
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

									//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
									//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
									bool capacity_satisfy = true;
									for (int c = from; c < to; c++) {
										capacity_satisfy = (r.capacity[c] + p.capacity <= map_airplane[r.aircraft_code].capacity);

										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
										if (c == to - 1)
											fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0)
											capacity_satisfy = false;

									}
									//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
									if (!capacity_satisfy) break; //check also for the fuel;

									//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider? per l'aggiunta del passeggero qui
									//? la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

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
							//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
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


					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
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
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1] += p.capacity;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}
			//end for weight

			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
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
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

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

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
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
						if (solution[best_route].refueling[t] && t != index_refueling)
							break;
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

				if (solution[best_route].weight[j] < 0) {

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
						if (solution[best_route].refueling[t] && t != index_refueling)
							break;

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
		}
	}


	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}


	return solution_clean;
}


#endif

