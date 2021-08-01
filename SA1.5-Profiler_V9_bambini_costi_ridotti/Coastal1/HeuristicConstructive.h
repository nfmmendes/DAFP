#ifndef HEURISTIC_CONSTRUCTIVE_H
#define HEURISTIC_CONSTRUCTIVE_H
#include <float.h>
#include <iostream>
#include "Route.h"
#include "Util.h"

using Double3DVector = vector<vector<vector<double>>>;


double evaluate_tw_departure(const Passenger& p, double time_arrival)
{

	if (time_arrival < p.early_departure)
		return p.early_departure - time_arrival;

	if (time_arrival > p.late_departure)
		return  time_arrival - p.late_departure;

	return 0;
}


double evaluate_tw_arrival( const Passenger& p, const double & time_arrival)
{
	if (time_arrival< p.early_arrival)
		return p.early_arrival - time_arrival;
	else if (time_arrival > p.late_arrival)
		return time_arrival - p.late_arrival;

	return 0; 
}

inline double cost_time_windows_for_route(const Route& r, double peso_TW) {
	double cost = 0.0;

	for (const Passenger& p : r.passengers_in_route) {

		double TW_departure = evaluate_tw_departure(p, r.time_arr[p.solution_from]);
		double TW_arrival = evaluate_tw_arrival(p, r.time_arr[p.solution_to]);
		cost += (TW_departure + TW_arrival) * p.capacity;
	}
	cost = cost * peso_TW;
	return cost;
}




vector<Route> heuristic_costructive_first_fase(double peso_TW, double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	//TODO: Replace it by a for
	do {
		Route r(airplanes[cont].code);
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
					double f_after_secondo_tratto = (map_airstrip[p.departure_location].fuel ?
													map_airplane[r.aircraft_code].max_fuel : r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto)
												     - fuel_consumed_secondo_tratto;

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
					for (int t = FROM[0]; t < r.index; t++) 
						if (r.places[t] == p.arrival_location) 
							TO.push_back(t);
					

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
									if (r.time_arr[to] < p.early_arrival) 
										TW_arrival = p.early_arrival - r.time_arr[to];
									else if (r.time_arr[to] > p.late_arrival) 
										TW_arrival = r.time_arr[to] - p.late_arrival;

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
							if (capacity_satisfy && (r.index - from) <= p.stop) {

								double TW_departure = evaluate_tw_departure(p, r.time_arr[from]);

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

								
								double travel_time = (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
								double t_arr_arrival = r.time_arr[r.index - 1] + travel_time + map_airstrip[r.places[r.index - 1]].ground_time;
								double TW_arrival = evaluate_tw_arrival(p, t_arr_arrival);
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


					double time_1 = (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
					double time_2 = (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
					double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + time_1;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + time_2;
					double TW_departure = evaluate_tw_departure(p, t_arr_departure);
					
					double TW_arrival = evaluate_tw_arrival(p, t_arr_arrival);
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
					double f_after_secondo_tratto = (map_airstrip[p.departure_location].fuel ? map_airplane[r.aircraft_code].max_fuel:
													 r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto )- fuel_consumed_secondo_tratto;

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
			int index = solution[best_route].index - 1;
			//time
			solution[best_route].time_dep[index] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[index] = p.early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

			p.solution_from = index;
			solution[best_route].capacity[index] += p.capacity;

			//for the weight
			solution[best_route].weight[index] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[index] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[index] < 0) {
				solution[best_route].quantity_fuel[index] += solution[best_route].weight[index];
				solution[best_route].weight[index] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[index] - p.weight;
			}
			//end for weight

			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[index];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[index] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[index];
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[index - 1] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = index;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == 2) {
			auto index = solution[best_route].index - 1;
			auto& airplane = map_airplane[solution[best_route].aircraft_code];
			double time_to_go = (from_to[solution[best_route].places[index]][p.departure_location] / airplane.speed) * 60;

			solution[best_route].primo_pass = true;
			solution[best_route].time_arr[index] = p.early_departure - time_to_go - map_airstrip[solution[best_route].places[index]].ground_time;
			solution[best_route].time_dep[index] = p.early_departure - time_to_go;

			//for the weight
			solution[best_route].weight[index] = airplane.weight_fuel_people - solution[best_route].quantity_fuel[index];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[index]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, airplane.max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[index] = airplane.weight_fuel_people - airplane.max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[index];
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[index] = solution[best_route].weight[index -1 ] - p.weight + fuel_consumed;

			}
			p.solution_from = index;
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
		if (s.index != 1) solution_clean.push_back(s);
	}
	
	return solution_clean;
}


#endif



//*************************************************************************************COSTRUTTIVI************************************************************************
vector<Route> heuristic_costructive_second_fase(vector<Route>& solution, double end_day, double peso_TW) {
	vector<Route> sol;

	for (Route& r : solution) {
		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, peso_TW);
		if (cost_time_windows_for_route(r, peso_TW) != 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			for (int i = 0; i < r.index - 1; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
				double wait_time = 0;
				//TODO: Transform it in a regular while
				if (i == 0) {
					do {
						for (int j = 0; j < r.index; j++) {
							r_support.time_arr[j] += 2; //TODO: Check what does it mean
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;  //prima c'era ++ per ottimizzare ho messo pi? due in modo da valutare ogni due minuti non ogni minuto
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

vector<Route> heuristic_costructive_second_fase_SP(const Solution& solution, const double& end_day, const double& peso_TW) {
	vector<Route> sol;

	for (const Route& r : solution) {
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
						wait_time += 2;  //prima c'era ++ per ottimizzare ho messo pi? due in modo da valutare ogni due minuti non ogni minuto
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

vector<Route> heuristic_costructive_first_fase_sequential(double peso_TW, double peso_intermediate_stop, Fleet& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger> passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	//TODO: Replace by for
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
	//now i have created only the possible route, now i that one of them and fill it untill the condition stop

	for (Route& r : solution) {
		//cout << "sto valutando la route *******************************************************************************************************************************************************************" << endl;
		int situation = -1;
		do {
			situation = -1; //se situation rimane error vuol dire che nessun passeggero si pu? inserire, quindi che la route ? piena e devo passare ad un'altra
			int best_passenger = -1;
			int best_from = -1;
			int best_to = -1;
			double best_cost = DBL_MAX;

			for (int p = 0; p < (int)passengers.size(); p++) {

				if (r.primo_pass == false) {
					if (r.places[r.index - 1] == passengers[p].departure_location) {
						auto& departure_location = passengers[p].departure_location;
						auto& arrival_location = passengers[p].arrival_location;
						auto& aircraft_code = r.aircraft_code;

						//in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = map_airplane[aircraft_code].fixed_cost + from_to[departure_location][arrival_location];
						//double travel_time = from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location] / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[aircraft_code][departure_location][arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;

						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][arrival_location])) {
								best_cost = cost;
								best_passenger = p;
								situation = 1;
							}
						}
					}
					else {
						//qui c'? solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi? kilometri e un landing stop
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

						//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = (map_airstrip[passengers[p].departure_location].fuel ? 
														map_airplane[r.aircraft_code].max_fuel: r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto)
														- fuel_consumed_secondo_tratto;
						

						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
									best_cost = cost;
									best_passenger = p;
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
										//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											if (c == to - 1) 
												fuel_i_j += location_fuel[r.aircraft_code][passengers[p].arrival_location];

											if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) 
												capacity_satisfy = false;

										}
										//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;

										//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider? per l'aggiunta del passeggero qui
										//? la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni


										double t_arr_departure = r.time_arr[from];
										double TW_departure = evaluate_tw_departure(passengers[p], t_arr_departure);
										double t_arr_arrival = r.time_arr[to];
										double TW_arrival = evaluate_tw_arrival(passengers[p], t_arr_arrival);
										double cost = ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;
										cost += (peso_intermediate_stop * (to - from - 1));
										
										if (best_cost > cost) {
											best_cost = cost;
											best_passenger = p;
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
						if (r.places[r.index - 1] != passengers[p].arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) 
											capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
								if (capacity_satisfy && (r.index - from) <= passengers[p].stop) {
									double t_arr_departure = r.time_arr[from];
									double TW_departure = evaluate_tw_departure(passengers[p], t_arr_departure);
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

					}
					if (r.places[r.index - 1] != passengers[p].departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location] + from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location] + from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][passengers[p].departure_location] + from_to[passengers[p].departure_location][passengers[p].arrival_location];

						
						//double t_arr_departure = r.time_arr[r.index - 1] + (map_airstrip[r.places[r.index - 1]].ground_time + (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60;
						double t_arr_departure = r.time_dep[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						double TW_departure = evaluate_tw_departure(passengers[p], t_arr_departure);

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[passengers[p].departure_location].ground_time + (((from_to[passengers[p].departure_location][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < passengers[p].early_arrival)
							TW_arrival = passengers[p].early_arrival - t_arr_arrival;
						else if (t_arr_arrival > passengers[p].late_arrival)
							TW_arrival = t_arr_arrival - passengers[p].late_arrival;

						cost += ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

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

						}

					}
				}
			}
			//qui dovrei avere tutte le cose per fare la mossa sempre se la route non ? piena


			if (situation == 1) {
				r.primo_pass = true;
				//time
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

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
				//end for weight


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


					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
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

					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
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

					if (r.weight[j] < 0) {
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}

						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling)
								break;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
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
					if (r.weight[j] < 0) {
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}

						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling)
								break;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
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
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}

				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);

			}

			if (situation != -1)
				passengers.erase(passengers.begin() + best_passenger);

		} while (situation != -1);
		if (passengers.empty())
			break;
	}

	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}

	return solution_clean;
}
