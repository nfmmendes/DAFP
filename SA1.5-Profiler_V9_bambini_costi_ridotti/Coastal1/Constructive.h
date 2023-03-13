#ifndef CONSTRUCTIVE_H
#define CONSTRUCTIVE_H

//***************************************************************COSTRUTTIVI************************************************************************
#include <iostream>
#include <set>
#include <vector>

#include "Evaluate.h"
#include "Route.h"
#include "Util.h"

inline void update_best_route(const double &end_day, const double &peso_TW, Route& best_route, const Route& r_support, double& best_cost)
{
	if (r_support.get_arrivals()[r_support.index - 1] <= end_day) {
		double cost_support = cost_time_windows_for_route(r_support, peso_TW);
		if (cost_support < best_cost) {
			best_route = r_support;
			best_cost = cost_support;
		}
	}
}

vector<Route> heuristic_costructive_second_fase(vector<Route>& solution, double end_day, double peso_TW, const double maximum_wait = 60) {
	vector<Route> sol;

	for (Route& r : solution) {
		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, peso_TW);
		
		if (cost_time_windows_for_route(r, peso_TW) != 0 && r.index > 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			for (unsigned int i = 0; i < r.index - 1; i++) { 
				double wait_time = 0;
				auto start_index = (i == 0 ? 0 : i + 1);
				
				while (wait_time <= maximum_wait) {
					if(i != start_index)
						r_support.get_departure_at(i) += 2;
					for (unsigned int j = start_index; j < r.index; j++) {
						r_support.get_arrival_at(j) += 2;
						r_support.get_departure_at(j) += 2;
					}
						
					update_best_route(end_day, peso_TW, best_route, r_support, best_cost);
					wait_time += 2;  
				}
				r_support = best_route;
			}
		}
		sol.push_back(best_route);
	}
	return sol;
}

vector<Route> heuristic_costructive_second_fase_SP(vector<Route>& solution, double end_day, double peso_TW) {
	return heuristic_costructive_second_fase(solution, end_day, peso_TW, 30);
}

namespace heuristic_costructive_first_fase_namespace {

	void run_situation_1(ProcessedInput* input, Passenger& p, Route* route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		Airstrip* airstrip_origin = &map_airstrip[p.origin];
		Airstrip* airstrip_destination = &map_airstrip[p.destination];

		Airplane* airplane = &map_airplane[route->aircraft_code];
		
		route->primo_pass = true;
		//time
		route->get_departure_at(route->index - 1) = p.early_departure + airstrip_origin->ground_time;
		route->get_arrival_at(route->index - 1) = p.early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

		p.solution_from = route->index - 1;
		route->add_capacity_at(route->index - 1, p.capacity);

		//for the weights
		//here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
		route->get_weight_at(route->index - 1) = airplane->load_weight - route->fuel[route->index - 1] - p.weight;  
		if (route->get_weights()[route->index - 1] < 0) {
			route->fuel[route->index - 1] += route->get_weights()[route->index - 1];
			route->get_weight_at(route->index - 1) = airplane->load_weight - route->fuel[route->index - 1] - p.weight;
		}
		//end for weights

		double travel_time = airplane->travelTime(from_to[p.origin][p.destination]);
		double time_from = route->get_departure_at(route->index - 1);
		double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][p.origin][p.destination];
		double arrival = time_from + travel_time;
		
		if (airstrip_destination->fuel) {
			route->addPlace(p.destination, true, { airplane->max_fuel, 0.0, 0 }, arrival, arrival + airstrip_destination->ground_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel;
		}
		else {
			double fuel = route->fuel[route->index - 1] - fuel_consumed;
			route->addPlace(p.destination, false, { fuel, 0.0, 0 }, arrival, arrival + airstrip_destination->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + fuel_consumed + p.weight;
		}
		
		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}

	inline void run_situation_2(ProcessedInput* input, Passenger& p, Route* route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		Airplane* airplane = &map_airplane[route->aircraft_code];
		Airstrip* origin = &map_airstrip[p.origin];
		Airstrip* destination = &map_airstrip[p.destination];
		auto airplane_consumption = from_to_FuelConsumed[route->aircraft_code];
		
		route->primo_pass = true;
		auto previous_airstrip = route->get_airstrips()[route->index - 1];
		double travel_time = airplane->travelTime(from_to[route->get_airstrips()[route->index - 1]][p.origin]);
		route->get_arrival_at(route->index - 1) = p.early_departure - travel_time - map_airstrip[previous_airstrip].ground_time;
		route->get_departure_at(route->index - 1) = p.early_departure - travel_time;
		
		route->get_weight_at(route->index - 1) = airplane->load_weight - route->fuel[route->index - 1];  

		double fuel_consumed = airplane_consumption[route->get_airstrips()[route->index - 1]][p.origin];

		if (origin->fuel) {
			AirplaneStatus airplane_status{ airplane->max_fuel, 0.0, 1 };
			route->addPlace(p.origin, true, airplane_status, p.early_departure, p.early_departure + origin->ground_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel - p.weight;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];

			AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, p.capacity };
			route->addPlace(p.origin, false, airplane_status, p.early_departure, p.early_departure + origin->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] - p.weight + fuel_consumed;
		}
		
		p.solution_from = route->index - 1;
		
		// Understand why ground time is used here
		double time = p.early_departure + origin->ground_time + airplane->travelTime(from_to[p.origin][p.destination]);

		if (map_airstrip[p.destination].fuel) {
			route->addPlace(p.destination, destination->fuel, { airplane->max_fuel, 0.0, 0 }, time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight -airplane->max_fuel;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			double fuel_consumed = airplane_consumption[route->get_airstrips()[route->index - 1]][p.destination];

			route->addPlace(p.destination, destination->fuel, { fuel_before - fuel_consumed, 0.0, 0 } , time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + p.weight + fuel_consumed;
		}
		
		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}

	inline void run_situation_3(ProcessedInput* input, Passenger& p, Route* route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		Airplane* airplane = &map_airplane[route->aircraft_code];
		Airstrip* origin = &map_airstrip[p.origin];
		Airstrip* destination = &map_airstrip[p.destination];
		
		auto airplane_consumption = from_to_FuelConsumed[route->aircraft_code];
		auto previous_airstrip = route->get_airstrips()[route->index - 1];

		const double travel_time = airplane->travelTime((from_to[previous_airstrip][p.origin]));
		const double arrival_time = travel_time + route->get_departures()[route->index - 1];
		const double departure_time = arrival_time + map_airstrip[p.origin].ground_time;
		
		if (origin->fuel) {
			route->addPlace(p.origin, true, { airplane->max_fuel, 0.0, p.capacity }, arrival_time, departure_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel - p.weight;
		}
		else {
			const double fuel_before = route->fuel[route->index - 1];
			const double fuel_consumed = airplane_consumption[previous_airstrip][p.origin];

			route->addPlace(p.origin, false, { fuel_before - fuel_consumed, 0.0, p.capacity }, arrival_time, departure_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] - p.weight + fuel_consumed;
		}
		
		p.solution_from = route->index - 1;
		double time = route->get_departures()[route->index - 1] + airplane->travelTime(from_to[p.origin][p.destination]);

		if (destination->fuel) {
			route->addPlace(p.destination, true, { airplane->max_fuel, 0.0, 0 }, time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			double fuel_consumed = airplane_consumption[previous_airstrip][p.destination];
			route->addPlace(p.destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + fuel_consumed + p.weight;
		}

		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}

	inline void run_situation_4(ProcessedInput* input, Passenger& p, int& best_from, int& best_to, Route* route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

		for (int h = best_from; h < best_to; h++) {
			route->add_capacity_at(h, p.capacity);
			route->get_weight_at(h) -= p.weight;
		}

		//**************************************************************************
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (unsigned int j = best_from; j < route->index; j++) {
			if (route->get_weights()[j] < 0) {

				add_fuel = route->get_weights()[j];
				index_weight_neg = j;
				unsigned int index_refueling = index_weight_neg;
				for (auto i = index_weight_neg; i >= 0; i--) {
					if (route->get_refueling()[i]) {
						index_refueling = i;
						break;
					}
				}

				for (unsigned int t = index_refueling; t < route->index; t++) {
					if (route->get_refueling()[t] && t != index_refueling) 
						break;
					route->fuel[t] += add_fuel;
					route->get_weight_at(t) -= add_fuel;
				}
			}
		}

		p.solution_from = best_from;
		p.solution_to = best_to;
		route->add_passenger(p);
	}

	inline void run_situation_5(ProcessedInput* input, Passenger& p, int& best_from, Route* route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

		Airplane* airplane = &map_airplane[route->aircraft_code];
		
		for (unsigned int h = best_from; h < route->index; h++) {
			route->add_capacity_at(h, p.capacity);
			route->get_weights()[h] -= p.weight;
		}

		//******************************************************************************
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = best_from; j < route->index; j++) {
			if (route->get_weights()[j] < 0) {
				add_fuel = route->get_weights()[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (route->get_refueling()[i]) {
						index_refueling = i;
						break;
					}
				}

				for (unsigned int t = index_refueling; t < route->index; t++) {
					if (route->get_refueling()[t] && t != index_refueling) 
						break;
					route->fuel[t] += add_fuel;
					route->get_weight_at(t) -= add_fuel;
				}
			}
		}
		//******************************************************************************

		p.solution_from = best_from;
		int place_1 = route->get_airstrips()[route->index - 1];
		double time = route->get_departures()[route->index - 1] + map_airplane[route->aircraft_code].travelTime(from_to[place_1][p.destination]);
		
		if (map_airstrip[p.destination].fuel) {
			route->addPlace(p.destination, true, { airplane->max_fuel, 0.0, 0 }, time, time + map_airstrip[p.destination].ground_time);
			route->get_weight_at(route->index - 1) = map_airplane[route->aircraft_code].load_weight - airplane->max_fuel;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][route->get_airstrips()[route->index - 1]][p.destination];

			route->addPlace(p.destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, time, time + map_airstrip[p.destination].ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + fuel_consumed + p.weight;
		}

		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}
}

vector<Route> costructive_first_fase(ProcessedInput* input, const PenaltyWeights& penalty_weights, vector<Airplane>& airplanes, 
									 double end_day, vector<Passenger>& passengers, int number_of_aircraft) {
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	double peso_TW = penalty_weights.time_window;
	
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector fuel_consumption = input->get_from_to_fuel_consumed();

	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		AirplaneStatus airplane_status{ map_airplane[r.aircraft_code].max_fuel, 0.0, 0 };
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, airplane_status, 0.0, 0.0);
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
			Airplane *airplane= &map_airplane[r.aircraft_code];
			
			if (r.primo_pass == false) {
				if (r.index > 0 && r.get_airstrips()[r.index - 1] == p.origin) {
					//in questo caso c'? solo lui nella route, il costo ? dato dalla sua
					//inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = airplane->fixed_cost + from_to[p.origin][p.destination];
					double fuel_consumed = fuel_consumption[r.aircraft_code][p.origin][p.destination];

					cost += fuel_consumed;
					//per il check sul fuel:
					double fuel_after_trip = 0.0;
					fuel_after_trip = r.fuel[r.index - 1] - fuel_consumed;
					if (best_cost > cost) {
						if (fuel_after_trip >= (airplane->min_fuel + location_fuel[r.aircraft_code][p.destination])) {
							best_cost = cost;
							best_route = i;
							situation = 1;
						}
					}
				}
				else {
					auto previous_airstrip = r.get_airstrips()[r.index - 1];
					
					//qui c'? solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla
					// partenza per il cliente, devo aggiungere pi? kilometri e un landing stop
					//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
					double cost = airplane->fixed_cost + from_to[previous_airstrip][p.origin] + from_to[p.origin][p.destination];
					auto airplane_consumption = fuel_consumption[r.aircraft_code];
					double fuel_consumed = airplane_consumption[p.origin][p.destination] + airplane_consumption[previous_airstrip][p.origin];

					cost += fuel_consumed;

					//per il check sul fuel
					double fuel_consumed_primo_tratto = airplane_consumption[previous_airstrip][p.origin];
					double fuel_consumed_secondo_tratto = airplane_consumption[p.origin][p.destination];

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il
					//passeggero potrebbe esserci il caso in cui l'aero possa fare refuel
					double f_after_primo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.origin].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					if (best_cost > cost) {
						if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
							if (f_after_secondo_tratto >= map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.destination]) {
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
				for (unsigned int t = 0; t < r.index; t++) {
					if (r.get_airstrips()[t] == p.origin) 
						FROM.push_back(t);
				}

				if (!FROM.empty()) {
					//dentro questo abbiamo trovato dei FROM 
					for (unsigned int t = FROM[0]; t < r.index; t++) {
						if (r.get_airstrips()[t] == p.destination) 
							TO.push_back(t);
					}

					if (!TO.empty()) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from >= to || to - from > p.stop)
									continue;

								Airplane* airplane = &map_airplane[r.aircraft_code];
								//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
								//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
								bool capacity_satisfy = true;
								for (int c = from; c < to; c++) {
									if ((r.get_capacities()[c] + p.capacity) > airplane->capacity) 
										capacity_satisfy = false;
										
									double fuel_consumed = fuel_consumption[airplane->code][r.get_airstrips()[c]][r.get_airstrips()[c + 1]];
									double fuel_i_j = fuel_consumed + airplane->min_fuel;

									//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel
									if (c == to - 1) 
										fuel_i_j += location_fuel[airplane->code][p.destination];
										
									if (!r.get_refueling()[c] && r.get_weights()[c] - p.weight + (r.fuel[c] - fuel_i_j) < 0) {
										capacity_satisfy = false;
										break;
									}
								}
									
								//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
								if (!capacity_satisfy) 
									break; //check also for the fuel;

								//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider?
								// per l'aggiunta del passeggero qui la time windows, se calcolo la time windows qua vuol dire che
								//? sfrutto il tempo in cui mi trovo in queste posizioni
								double TW_departure = max(0.0, p.early_departure - r.get_departure_at(from)) + 
														max(0.0, r.get_departure_at(from) - p.late_departure);
								double TW_arrival = max(0.0, p.early_arrival - r.get_arrival_at(to)) +
													max(0.0, r.get_arrival_at(to) - p.late_arrival);
								double TW_penalty = (TW_departure + TW_arrival) * peso_TW;
								
								double cost = TW_penalty + ((to - from - 1) * peso_intermediate_stop) * p.capacity;
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
					// da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi
					// conviene farlo fermare li quindi il for per tutti i from ma al di fuori della route e poi 
					// considero anche il costo di aggiungerlo a parte il pezzetto che manca
					if (r.get_airstrips()[r.index - 1] != p.destination) {
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (unsigned int c = from; c < r.index; c++) {
								if ((r.get_capacities()[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) 
									capacity_satisfy = false;

								if (c >= r.index - 1)
									continue;
								
								double fuel_consumed = fuel_consumption[r.aircraft_code][r.get_airstrips()[c]][r.get_airstrips()[c + 1]];
								double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

								if (!r.get_refueling()[c] && r.get_weights()[c] - p.weight + (r.fuel[c] - fuel_i_j) < 0)
									capacity_satisfy = false;
							}
							
							//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
							if (!capacity_satisfy || (r.index - from) > p.stop)
								continue;
														
							double TW_departure = max(0.0, p.early_departure - r.get_departure_at(from)) + 
													max(0.0, r.get_departure_at(from) - p.late_departure);

							double cost = (TW_departure * peso_TW);
							auto previous_airstrip = r.get_airstrips()[r.index - 1];
							// ora ci metto chilometri e fuel dell'ultimo pezzo;
							cost += from_to[r.get_airstrips()[r.index - 1]][p.destination];
							double fuel_consumed = fuel_consumption[r.aircraft_code][previous_airstrip][p.destination];

							cost += fuel_consumed;

							bool fuel_ok = true;
							if (!r.get_refueling()[r.index - 1]) {
								double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
								if (r.get_weights()[r.index - 1] - p.weight + (r.fuel[r.index - 1] - fuel_i_j) < 0) 
									fuel_ok = false;
							}

							double travel_time = airplane->travelTime(from_to[previous_airstrip][p.destination]);
							double t_arr_arrival = r.get_arrivals()[r.index - 1] + travel_time + map_airstrip[previous_airstrip].ground_time;

							double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);
							cost += (TW_arrival * peso_TW) * p.capacity + (peso_intermediate_stop * (r.index - from - 1)) * p.capacity;

							if (fuel_ok && best_cost > cost && t_arr_arrival <= end_day) {	
								double fuel = airplane->min_fuel + location_fuel[r.aircraft_code][p.destination]; 
								if (r.fuel[r.index - 1] - fuel_consumed >= fuel) {
									best_cost = cost;
									best_route = i;
									best_from = from;
									situation = 5;
								}
							}
						}
					}
				}

				if (r.get_airstrips()[r.index - 1] != p.origin)
					continue;

				auto previous_airstrip = r.get_airstrips()[r.index - 1];
				//guardo il costo di aggiungere un pezzo alla fine
				auto airplane_consumption = fuel_consumption[r.aircraft_code];
				double fuel_consumed = airplane_consumption[previous_airstrip][p.origin] + 
										airplane_consumption[p.origin][p.destination];

				double cost = fuel_consumed + from_to[previous_airstrip][p.origin] + from_to[p.origin][p.destination];

				double travel_time = map_airplane[r.aircraft_code].travelTime(from_to[previous_airstrip][p.origin]);
				double t_arr_departure = r.get_arrivals()[r.index - 1] + map_airstrip[previous_airstrip].ground_time + travel_time;
				double TW_departure = max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);

				travel_time = map_airplane[r.aircraft_code].travelTime(from_to[p.origin][p.destination]);
				double t_arr_arrival = t_arr_departure + map_airstrip[p.origin].ground_time + travel_time;
				double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);
					
				cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

				//per il check sul fuel:
				double consumption_first_leg = airplane_consumption[previous_airstrip][p.origin];
				double consumption_second_leg = airplane_consumption[p.origin][p.destination];

				//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci
				//il caso in cui l'aero possa fare 
				//refuel
				double f_after_primo_tratto = r.fuel[r.index - 1] - consumption_first_leg;
				double f_after_secondo_tratto = 0.0;
				if (map_airstrip[p.origin].fuel) {
					f_after_secondo_tratto = airplane->max_fuel - consumption_second_leg;
				}
				else {
					f_after_secondo_tratto = f_after_primo_tratto - consumption_second_leg;
				}

				if (best_cost > cost && t_arr_arrival <= end_day && f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
					if (f_after_secondo_tratto >= (airplane->min_fuel + location_fuel[r.aircraft_code][p.destination])) {
						best_cost = cost;
						best_route = i;
						situation = 3;
					}
				}				
			}
			i++;
		}

		Route* route = &solution[best_route];
		if (situation == 1) {
			heuristic_costructive_first_fase_namespace::run_situation_1(input, p, route);
		}
		else if (situation == 2) {
			heuristic_costructive_first_fase_namespace::run_situation_2(input, p, route);
		}
		else if (situation == 3) {
			heuristic_costructive_first_fase_namespace::run_situation_3(input, p, route);
		}
		else if (situation == 4) {
			heuristic_costructive_first_fase_namespace::run_situation_4(input, p, best_from, best_to, route);
		}
		else if (situation == 5) {
			heuristic_costructive_first_fase_namespace::run_situation_5(input, p, best_from, route);
		}
		else if (situation == -1) {
			cout << "**********il passeggero: " << p.name << " non trova sistemazione" << endl;
		}
	}

	vector<Route> solution_clean;
	for (auto s : solution) {
		if (s.index != 1)
			solution_clean.push_back(s);
	}

	return solution_clean;
}

void run_situation_1(ProcessedInput* input, vector<Passenger>& passengers, Route& r, int& best_passenger)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	Airplane* airplane = &map_airplane[r.aircraft_code];
	
	r.primo_pass = true;
	//time
	r.get_departure_at(r.index - 1) = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].origin].ground_time;
	// in questo caso anche l'arrivo in quanto ? la partenza dal depot
	r.get_arrival_at(r.index - 1) = passengers[best_passenger].early_departure; 

	passengers[best_passenger].solution_from = r.index - 1;
	r.add_capacity_at(r.index - 1, passengers[best_passenger].capacity);

	//for the weights
	//here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
	r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight -
		r.fuel[r.index - 1] - passengers[best_passenger].weight;  
	if (r.get_weight_at(r.index - 1) < 0) {
		r.fuel[r.index - 1] += r.get_weight_at(r.index - 1);
		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight - r.fuel[r.index - 1] - passengers[best_passenger].weight;
	}
	//end for weights

	auto best_passenger_origin = passengers[best_passenger].origin;
	auto passenger_destination = passengers[best_passenger].destination;
	double travel_time = airplane->travelTime(from_to[best_passenger_origin][passenger_destination]);
	double time_from = r.get_departures()[r.index - 1];
	double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][best_passenger_origin][passenger_destination];
	auto arrival_time = time_from + travel_time;
	auto departure_time = arrival_time + map_airstrip[passenger_destination].ground_time;
	
	if (map_airstrip[passenger_destination].fuel) {
		r.addPlace(passengers[best_passenger].destination, true, { airplane->max_fuel, 0.0, 0 }, arrival_time, departure_time);
		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight - map_airplane[r.aircraft_code].max_fuel;
	}
	else {
		double fuel_before = r.fuel[r.index - 1];
		r.addPlace(passenger_destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, arrival_time, departure_time);
		r.get_weight_at(r.index - 1) = r.get_weight_at(r.index - 2) + fuel_consumed + passengers[best_passenger].weight;;
	}
	passengers[best_passenger].solution_to = r.index - 1;
	r.add_passenger(passengers[best_passenger]);
}

void run_situation_2(ProcessedInput* input, vector<Passenger>& passengers, Route& r, const int& i_best_passenger)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector fuel_consumption = input->get_from_to_fuel_consumed();
	
	r.primo_pass = true;
	Airplane* airplane = &map_airplane[r.aircraft_code];
	auto previous_airstrip = r.get_airstrips()[r.index - 1];
	const Passenger best_passenger = passengers[i_best_passenger];
	auto origin = best_passenger.origin;
	auto destination = best_passenger.destination;
	

	double travel_time = airplane->travelTime(from_to[previous_airstrip][origin]);
	r.get_arrival_at(r.index - 1) = best_passenger.early_departure - travel_time - map_airstrip[previous_airstrip].ground_time;

	travel_time = airplane->travelTime(from_to[r.get_airstrips()[r.index - 1]][origin]);
	r.get_departure_at(r.index - 1) = best_passenger.early_departure - travel_time;

	r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight -
		r.fuel[r.index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel

	double fuel_consumed = fuel_consumption[r.aircraft_code][previous_airstrip][origin];

	if (map_airstrip[origin].fuel) {
		AirplaneStatus airplane_status{ map_airplane[r.aircraft_code].max_fuel, 0.0, best_passenger.capacity };
		r.addPlace(origin, map_airstrip[origin].fuel, airplane_status, best_passenger.early_departure, 
				   best_passenger.early_departure + map_airstrip[origin].ground_time);

		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight -
			map_airplane[r.aircraft_code].max_fuel - best_passenger.weight;
	}
	else {
		double fuel_before = r.fuel[r.index - 1];
		AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, best_passenger.capacity };
		r.addPlace(origin, map_airstrip[origin].fuel, 
				   airplane_status, best_passenger.early_departure, 
				   best_passenger.early_departure + map_airstrip[origin].ground_time);
		r.get_weight_at(r.index - 1) = r.get_weights()[r.index - 2] - best_passenger.weight + fuel_consumed;
	}
	
	passengers[i_best_passenger].solution_from = r.index - 1;
	
	travel_time = airplane->travelTime(from_to[origin][destination]);
	double time = best_passenger.early_departure + map_airstrip[origin].ground_time + travel_time;

	if (map_airstrip[destination].fuel) {
		AirplaneStatus airplane_status{ map_airplane[r.aircraft_code].max_fuel, 0.0, 0 };
		r.addPlace(destination, true, airplane_status, time, time + map_airstrip[destination].ground_time);

		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight - map_airplane[r.aircraft_code].max_fuel;
	}
	else {
		
		double fuel_before = r.fuel[r.index - 1];
		double fuel_consumed = fuel_consumption[r.aircraft_code][r.get_airstrips()[r.index - 1]][destination];

		AirplaneStatus airplane_status{fuel_before - fuel_consumed, 0.0, 0 }; 
		r.addPlace(destination, false, airplane_status, time, time + map_airstrip[destination].ground_time);
		r.get_weight_at(r.index - 1) = r.get_weights()[r.index - 2] + best_passenger.weight + fuel_consumed;

	}
	passengers[i_best_passenger].solution_to = r.index - 1;
	r.add_passenger(passengers[i_best_passenger]);
}

void run_situation_3(ProcessedInput* input, vector<Passenger>& passengers, Route& r, int& best_passenger)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	auto origin = passengers[best_passenger].origin; 
	auto destination = passengers[best_passenger].destination; 
	
	Airplane* airplane = &map_airplane[r.aircraft_code]; 
	
	if (map_airstrip[passengers[best_passenger].origin].fuel) {
		const double travel_time = airplane->travelTime(from_to[r.get_airstrips()[r.index - 1]][origin]);
		const auto arrival_time = r.get_departures()[r.index - 1] + travel_time;
		const auto departure_time = r.get_departures()[r.index - 1] + travel_time + map_airstrip[origin].ground_time; 

		AirplaneStatus airplane_status{ airplane->max_fuel, 0.0, passengers[best_passenger].capacity };
		r.addPlace(origin, map_airstrip[origin].fuel, airplane_status , arrival_time
				   , departure_time);

		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight -
			map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;

	}
	else {
		auto previous_airstrip = r.get_airstrips()[r.index - 1];
		double fuel_before = r.fuel[r.index - 1];
		double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][previous_airstrip][origin];
		double remaining_fuel = fuel_before - fuel_consumed;

		double travel_time = airplane->travelTime(from_to[previous_airstrip][origin]);
		auto time_arrival = r.get_departures()[r.index - 1] + travel_time;
		auto time_departure = time_arrival + map_airstrip[origin].ground_time;

		AirplaneStatus airplane_status{ remaining_fuel, 0.0, passengers[best_passenger].capacity };
		r.addPlace(origin, map_airstrip[origin].fuel, airplane_status , time_arrival, time_departure);

		r.get_weight_at(r.index - 1) = r.get_weights()[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

	}
	passengers[best_passenger].solution_from = r.index - 1;
	double time = r.get_departures()[r.index - 1] +  map_airplane[r.aircraft_code].travelTime(from_to[origin][destination]);


	if (map_airstrip[destination].fuel) {
		r.addPlace(destination, true, { map_airplane[r.aircraft_code].max_fuel, 0.0, 0 }, time, time + map_airstrip[destination].ground_time);
		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight -
			map_airplane[r.aircraft_code].max_fuel;
	}
	else {
		double fuel_before = r.fuel[r.index - 1];
		double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.get_airstrips()[r.index - 1]][destination];

		r.addPlace(destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, time, time + map_airstrip[destination].ground_time);
		r.get_weight_at(r.index - 1) = r.get_weights()[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;
	}

	passengers[best_passenger].solution_to = r.index - 1;
	r.add_passenger(passengers[best_passenger]);
}

void run_situation_4(ProcessedInput* input, vector<Passenger> &passengers, Route& r, int best_passenger, int best_from, int best_to)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	for (int h = best_from; h < best_to; h++) {
		r.add_capacity_at(h, passengers[best_passenger].capacity);
		r.get_weight_at(r.index - 1) -= passengers[best_passenger].weight;
	}

	double add_fuel = 0;
	int index_weight_neg = -1;
	for (unsigned int j = best_from; j < r.index; j++) {

		if (r.get_weights()[j] < 0) {
			add_fuel = r.get_weights()[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (r.get_refueling()[i]) {
					index_refueling = i;
					break;
				}
			}

			for (unsigned int t = index_refueling; t < r.index; t++) {
				if (r.get_refueling()[t] && t != index_refueling)
					break;
				r.fuel[t] += add_fuel;
				r.get_weight_at(t) -= add_fuel;
			}
		}
	}

	passengers[best_passenger].solution_from = best_from;
	passengers[best_passenger].solution_to = best_to;
	r.add_passenger(passengers[best_passenger]);
}

void run_situation_5(ProcessedInput* input, vector<Passenger>& passengers, Route& r, int& best_passenger, int& best_from)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	auto destination = passengers[best_passenger].destination; 
	
	for (unsigned int h = best_from; h < r.index; h++) {
		r.add_capacity_at(h, passengers[best_passenger].capacity);
		r.get_weight_at(h) -= passengers[best_passenger].weight;
	}

	//******************************************************************************
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (unsigned int j = best_from; j < r.index; j++) {
		if (r.get_weight_at(j) < 0) {
			add_fuel = r.get_weights()[j];
			index_weight_neg = j;
			unsigned int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (r.get_refueling()[i]) {
					index_refueling = i;
					break;
				}
			}

			for (unsigned int t = index_refueling; t < r.index; t++) {
				if (r.get_refueling()[t] && t != index_refueling)
					break;
				r.fuel[t] += add_fuel;
				r.get_weight_at(t) -= add_fuel;
			}
		}
	}

	passengers[best_passenger].solution_from = best_from;
	int place_1 = r.get_airstrips()[r.index - 1];
	double time = r.get_departures()[r.index - 1] + map_airplane[r.aircraft_code].travelTime(from_to[place_1][destination]);

	if (map_airstrip[destination].fuel) {
		AirplaneStatus airplane_status{ map_airplane[r.aircraft_code].max_fuel, 0.0, 0 };
		r.addPlace(destination, true, airplane_status , time, time + map_airstrip[destination].ground_time);
		r.get_weight_at(r.index - 1) = map_airplane[r.aircraft_code].load_weight -
			map_airplane[r.aircraft_code].max_fuel;
	}
	else {
		double fuel_before = r.fuel[r.index - 1];
		double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.get_airstrips()[r.index - 1]][destination];
		AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0, 0 };
		r.addPlace(destination, false, airplane_status, time, time + map_airstrip[destination].ground_time);
		r.get_weight_at(r.index - 1) = r.get_weights()[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;
	}

	passengers[best_passenger].solution_to = r.index - 1;
	r.add_passenger(passengers[best_passenger]);
}

vector<Route> sequential_costructive_first_fase(ProcessedInput* input, const PenaltyWeights& penalty_weights, 
												double end_day, vector<Passenger>& passengers, int number_of_aircraft) {
	double peso_TW{ penalty_weights.time_window };
	double peso_intermediate_stop {penalty_weights.intermediate_stop };
	
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	const double3DVector fuel_consumption = input->get_from_to_fuel_consumed();

	vector<Airplane> airplanes;
	for_each(map_airplane.begin(), map_airplane.end(), [&airplanes](auto entry) { airplanes.push_back(entry.second); });

	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		AirplaneStatus airplane_status{ map_airplane[r.aircraft_code].max_fuel, 0.0, 0 }; 
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, airplane_status , 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
	//now i have created only the possible route, now i that one of them and fill it untill the condition stop

	for (Route& r : solution) {
		int situation = -1;
		Airplane* airplane = &map_airplane[r.aircraft_code];
		const double2DVector& airplane_consumption = fuel_consumption[r.aircraft_code];
		
		do {
			situation = -1; //se situation rimane error vuol dire che nessun passeggero si pu? inserire, quindi che la route ?
			int best_passenger = -1;
			int best_from = -1;
			int best_to = -1;
			double best_cost = DBL_MAX;

			for (int p = 0; p < (int)passengers.size(); p++) {
				auto origin = passengers[p].origin;
				auto destination = passengers[p].destination;
				auto previous_airstrip = r.get_airstrips()[r.index - 1];
				
				if (r.primo_pass == false) {
					if (previous_airstrip == origin) {
						// in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione,
						// quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = airplane->fixed_cost + from_to[origin][destination];
						double fuel_consumed = airplane_consumption[origin][destination];

						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.fuel[r.index - 1] - fuel_consumed;

						if (best_cost > cost) {
							if (fuel_after_trip >= (airplane->min_fuel + location_fuel[r.aircraft_code][destination])) {
								best_cost = cost;
								best_passenger = p;
								situation = 1;
							}
						}
					}
					else {
						auto previous_airstrip = r.get_airstrips()[r.index - 1];
						//qui c'? solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per
						// il cliente, devo aggiungere pi? kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = airplane->fixed_cost + from_to[origin][destination] + from_to[previous_airstrip][origin];
						double fuel_consumed = airplane_consumption[origin][destination] + airplane_consumption[previous_airstrip][origin];
						cost += fuel_consumed;

						//per il check sul fuel
						double fuel_consumed_primo_tratto = airplane_consumption[r.get_airstrips()[r.index - 1]][passengers[p].origin];
						double fuel_consumed_secondo_tratto = airplane_consumption[origin][destination];

						// ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero
						// potrebbe esserci il caso in cui l'aero possa fare refuel
						double f_after_primo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].origin].fuel) {
							f_after_secondo_tratto = airplane->max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						if (best_cost > cost && f_after_primo_tratto >= airplane->min_fuel) {
							if (f_after_secondo_tratto >= (airplane->min_fuel + location_fuel[r.aircraft_code][destination])) {
								best_cost = cost;
								best_passenger = p;
								situation = 2;
							}
						}
					}
				}
				else {
					//nella route c'? almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit? della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'? il FROM, non cerco il TO
					std::vector<int> FROM;
					std::vector<int> TO;
					for (unsigned int t = 0; t < r.index; t++) {
						if (r.get_airstrips()[t] == passengers[p].origin) 
							FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (unsigned int t = FROM[0]; t < r.index; t++) {
							if (r.get_airstrips()[t] == passengers[p].destination) 
								TO.push_back(t);
						}

						if (!TO.empty()) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= passengers[p].stop) {
										//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.get_capacities()[c] + passengers[p].capacity) > airplane->capacity) 
												capacity_satisfy = false;

											auto current_airstrip = r.get_airstrips()[c];
											auto next_airstrip = r.get_airstrips()[c + 1];
											double fuel_consumed = airplane_consumption[current_airstrip][next_airstrip];

											double fuel_i_j = fuel_consumed + airplane->min_fuel;

											if (c == to - 1)
												fuel_i_j += location_fuel[r.aircraft_code][destination];

											if (!r.get_refueling()[c] && r.get_weights()[c] - passengers[p].weight + (r.fuel[c] - fuel_i_j) < 0) 
												capacity_satisfy = false;
										}
										//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) 
											break; //check also for the fuel;

										// se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo
										// che consider? per l'aggiunta del passeggero qui
										// la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui
										// mi trovo in queste posizioni
										double TW_departure = 0.0;
										double t_origin_departure = r.get_arrivals()[from];
										
										TW_departure = max(0.0, passengers[p].early_departure - t_origin_departure)
														+ max(0.0, t_origin_departure - passengers[p].late_departure);

										double TW_arrival = 0;
										double t_destination_arrival = r.get_arrivals()[to];

										TW_arrival = max(0.0, passengers[p].early_arrival - t_destination_arrival)
												   + max(0.0, t_destination_arrival - passengers[p].late_arrival);   

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
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi
						// conviene farlo fermare li quindi il for per tutti i from ma al di fuori della
						// route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.index > 0 && r.get_airstrips()[r.index - 1] != passengers[p].destination) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (unsigned int c = from; c < r.index; c++) {
									if ((r.get_capacities()[c] + passengers[p].capacity) > airplane->capacity) {
										capacity_satisfy = false;
										break;
									}
										
									if (r.index > 0 && c < r.index - 1) {
										double fuel_consumed = airplane_consumption[r.get_airstrips()[c]][r.get_airstrips()[c + 1]];

										double fuel_i_j = fuel_consumed + airplane->min_fuel;

										if (!r.get_refueling()[c] && r.get_weights()[c] - passengers[p].weight + (r.fuel[c] - fuel_i_j) < 0) 
											capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
								if (!capacity_satisfy || (r.index - from) > passengers[p].stop)
									continue; 
								
								double TW_departure = 0.0;
								double t_arr_departure = r.get_arrivals()[from];
								auto previous_airstrip = r.get_airstrips()[r.index - 1];
								TW_departure = max(0.0, passengers[p].early_departure - t_arr_departure)
												+ max(0.0, t_arr_departure - passengers[p].late_departure);

								double cost = (TW_departure * peso_TW);
								// ora ci metto chilometri e fuel dell'ultimo pezzo;
								cost += from_to[r.get_airstrips()[r.index - 1]][destination];
								double travel_time =  airplane->travelTime(from_to[r.get_airstrips()[r.index - 1]][destination]);
								double fuel_consumed = airplane_consumption[previous_airstrip][destination];

								cost += fuel_consumed;

								bool fuel_ok = true;
								if (!r.get_refueling()[r.index - 1]) {
									double fuel_i_j = fuel_consumed + airplane->min_fuel;
									if (r.get_weights()[r.index - 1] - passengers[p].weight + (r.fuel[r.index - 1] - fuel_i_j) < 0) 
										fuel_ok = false;
								}

								double TW_arrival = 0.0;
								double t_arr_arrival = 0.0;
								t_arr_arrival = r.get_arrivals()[r.index - 1] + (travel_time) + map_airstrip[previous_airstrip].ground_time;
									
								TW_arrival = max(0.0, passengers[p].early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - passengers[p].late_arrival);
								cost += ((TW_arrival * peso_TW)) * passengers[p].capacity;
								cost += (peso_intermediate_stop * (r.index - from - 1)) * passengers[p].capacity;

								if (fuel_ok && best_cost > cost && t_arr_arrival <= end_day) {
									auto current_fuel = r.fuel[r.index - 1] - fuel_consumed;
									auto fuel_needed = (airplane->min_fuel + location_fuel[r.aircraft_code][destination]);
									if (current_fuel >= fuel_needed) {
										best_cost = cost;
										best_passenger = p;
										best_from = from;
										situation = 5;
									}
								}
							}
						}
					}

					// This is the last if before closing the loop 
					if (r.get_airstrips()[r.index - 1] == passengers[p].origin)
						continue;

					//guardo il costo di aggiungere un pezzo alla fine
					auto previous_airstrip = r.get_airstrips()[r.index - 1];
					double fuel_consumed = airplane_consumption[previous_airstrip][origin] + airplane_consumption[origin][destination];
					double cost = fuel_consumed + from_to[previous_airstrip][origin] + from_to[origin][destination];

					double TW_departure = 0.0;
					double travel_time = airplane->travelTime(from_to[previous_airstrip][origin]);
					double t_arr_departure = previous_airstrip + map_airstrip[previous_airstrip].ground_time + travel_time;
					if (t_arr_departure < passengers[p].early_departure) 
						TW_departure = passengers[p].early_departure - t_arr_departure;
					else if (t_arr_departure > passengers[p].late_departure) 
						TW_departure = t_arr_departure - passengers[p].late_departure;

					double TW_arrival = 0.0;
					travel_time = airplane->travelTime(from_to[origin][destination]);
					double t_arr_arrival = t_arr_departure + map_airstrip[origin].ground_time + travel_time;
					TW_arrival = max(0.0, passengers[p].early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - passengers[p].late_arrival);
					cost += ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;

					//per il check sul fuel:
					double first_step_fuel_consumption = airplane_consumption[previous_airstrip][origin];
					double second_step_fuel_consumption = airplane_consumption[origin][destination];
					double f_after_primo_tratto = r.fuel[r.index - 1] - first_step_fuel_consumption;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[origin].fuel) {
						f_after_secondo_tratto = airplane->max_fuel - second_step_fuel_consumption;
					}
					else {
						f_after_secondo_tratto = r.fuel[r.index - 1] - first_step_fuel_consumption - second_step_fuel_consumption;
					}

					if (best_cost > cost && t_arr_arrival <= end_day && f_after_primo_tratto >= airplane->min_fuel) {
						if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][destination])) {
							best_cost = cost;
							best_passenger = p;
							situation = 3;
						}
					}
				}
			}
			//qui dovrei avere tutte le cose per fare la mossa sempre se la route non ? piena
			if (situation == 1) {
				run_situation_1(input, passengers, r, best_passenger);
			}
			else if (situation == 2) {
				run_situation_2(input, passengers, r, best_passenger);
			}
			else if (situation == 3) {
				run_situation_3(input, passengers, r, best_passenger);
			}
			else if (situation == 4) {
				run_situation_4(input, passengers, r, best_passenger, best_from, best_to);
			}
			else if (situation == 5) {
				run_situation_5(input, passengers, r, best_passenger, best_from);
			}

			if (situation != -1) 
				passengers.erase(passengers.begin() + best_passenger);

		} while (situation != -1);
		if (passengers.empty()) 
			break;
	}

	std::vector<Route> solution_clean;
	for (auto s : solution) {
		if (s.index != 1) 
			solution_clean.push_back(s);
	}

	cout << "ecco i passeggeri rimasti" << endl;
	for (Passenger rimasto : passengers) rimasto.print();

	return solution_clean;
}

map<int, int> Compute_WorstNode(double peso_TW, double stop_weight, Route& route, map<unsigned int, Airstrip>& map_airstrip, double2DVector& from_to) {
	
	map<double, int> Node;
	map<int, int> NodeOrder;
	set<double, MyCOMP<double>> Myset;
	if (route.index <= 2) {
		//route.print();
		Node.insert(make_pair(100000, 1));
		Myset.insert(100000);
	}
	else {
		for (int n = 1; n < route.index; n++) {
			double dist = 0.0;
			double cost_IS = 0.0;
			vector<Passenger> PassengerNodo;
			if (n != (route.index - 1)) {
				auto n_prev_airstrip = route.get_airstrips()[n - 1];
				auto n_esim_airstrip = route.get_airstrips()[n];
				auto n_next_airstrip = route.get_airstrips()[n+1];
				dist += map_airstrip[route.get_airstrips()[n]].landing_cost;
				dist += from_to[n_prev_airstrip][n_esim_airstrip] + from_to[n_esim_airstrip][n_next_airstrip] - from_to[n_prev_airstrip][n_next_airstrip];
				for (int p = 0; p < (int)route.get_passengers().size(); p++) {
					if (route.get_passengers()[p].solution_to == n || route.get_passengers()[p].solution_from == n) {
						PassengerNodo.push_back(route.get_passengers()[p]);
					}

					//pezzo aggiunto per intermediate stop***********************************
					if (route.get_passengers()[p].solution_from < n && route.get_passengers()[p].solution_to > n) {
						cost_IS += (stop_weight)*route.get_passengers()[p].capacity;
					}
				}
			}
			else {
				//cout << " Calcolo per ultimo nodo " << endl;
				dist += map_airstrip[route.get_airstrips()[n]].landing_cost;
				dist += from_to[route.get_airstrips()[n - 1]][route.get_airstrips()[n]];
				for (int p = 0; p < (int)route.get_passengers().size(); p++) {
					if (route.get_passengers()[p].solution_to == n)
						PassengerNodo.push_back(route.get_passengers()[p]);
					
					//pezzo aggiunto per intermediate stop**********************************************************
					if (route.get_passengers()[p].solution_from < n && route.get_passengers()[p].solution_to > n) {
						cost_IS += (stop_weight)*route.get_passengers()[p].capacity;
					}
				}
			}

			dist += (cost_time_windows_for_node(route, PassengerNodo, peso_TW)) + cost_IS;
			Node.insert(make_pair(dist, n));
			Myset.insert(dist);
		}
	}

	int i = 0;
	for (auto setval : Myset) {
		NodeOrder.insert(make_pair(i, Node[setval]));
		i++;
	}

	return NodeOrder;
}

namespace heuristic_costructive_first_fase_secIter_namespace
{
	void run_situation_1(ProcessedInput* input, vector<Route>& solution, Passenger& p, int& best_route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		
		Route* route = &solution[best_route];
		Airplane* airplane = &map_airplane[route->aircraft_code];
		route->primo_pass = true;
		//time
		route->get_departure_at(route->index - 1) = p.early_departure + map_airstrip[p.origin].ground_time;
		route->get_arrival_at(route->index - 1) = p.early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

		p.solution_from = route->index - 1;
		route->add_capacity_at(route->index - 1, p.capacity);

		//for the weights
		route->get_weight_at(route->index - 1) = airplane->load_weight - route->fuel[route->index - 1] - p.weight;
		//here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
		if (route->get_weights()[route->index - 1] < 0) {
			route->fuel[route->index - 1] += route->get_weights()[route->index - 1];
			route->get_weight_at(route->index - 1) = airplane->load_weight - route->fuel[route->index - 1] - p.weight;
		}
		//end for weights

		double travel_time = airplane->travelTime(from_to[p.origin][p.destination]);
		double time_from = route->get_departures()[route->index - 1];
		double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][p.origin][p.destination];

		double time_arrival = time_from + travel_time;
		double time_departure = time_arrival + map_airstrip[p.destination].ground_time;
		
		if (map_airstrip[p.destination].fuel) {
			route->addPlace(p.destination, true, { airplane->max_fuel, 0.0, 0 }, time_arrival, time_departure);
			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			route->addPlace(p.destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, time_arrival, time_departure);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + fuel_consumed + p.weight;
		}
		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}

	void run_situation_2(ProcessedInput* input, vector<Route>& solution, Passenger& p, int& best_route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

		Route* route = &solution[best_route];
		Airplane* airplane = &map_airplane[route->aircraft_code];
		Airstrip* origin = &map_airstrip[p.origin];
		Airstrip* destination = &map_airstrip[p.destination];

		auto previous_airstrip = route->get_airstrips()[route->index - 1];
		double travel_time = airplane->travelTime(from_to[previous_airstrip][p.origin]);
		
		route->primo_pass = true;
		route->get_arrival_at(route->index - 1) = p.early_departure -  travel_time - map_airstrip[previous_airstrip].ground_time;
		route->get_departure_at(route->index - 1) = p.early_departure - airplane->travelTime(from_to[previous_airstrip][p.origin]);

		double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][previous_airstrip][p.origin];
		route->get_weight_at(route->index - 1) = map_airplane[route->aircraft_code].load_weight - route->fuel[route->index - 1];

		if (origin->fuel) { 
			AirplaneStatus airplane_status{ airplane->max_fuel, 0.0, p.capacity };
			route->addPlace(p.origin, true, airplane_status, p.early_departure, p.early_departure + map_airstrip[p.origin].ground_time);

			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel - p.weight;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];

			AirplaneStatus airplane_status{ fuel_before - fuel_consumed, 0.0,p.capacity };
			route->addPlace(p.origin, origin->fuel, airplane_status, p.early_departure, p.early_departure + origin->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] - p.weight + fuel_consumed;
		}
		
		p.solution_from = route->index - 1;
		double time = p.early_departure + origin->ground_time + airplane->travelTime(from_to[p.origin][p.destination]);

		if (destination->fuel) {
			route->addPlace(p.destination, true, { airplane->max_fuel, 0.0, 0 }, time, time + destination->ground_time);

			route->get_weight_at(route->index - 1) = airplane->load_weight - map_airplane[route->aircraft_code].max_fuel;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][previous_airstrip][p.destination];
			route->addPlace(p.destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + p.weight + fuel_consumed;

		}
		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}

	void run_situation_3(ProcessedInput* input, vector<Route>& solution, Passenger& p, int& best_route)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		
		Route* route = &solution[best_route];
		Airplane* airplane = &map_airplane[route->aircraft_code];
		Airstrip* origin = &map_airstrip[p.origin];
		Airstrip* destination = &map_airstrip[p.destination];

		auto previous_airstrip = route->get_airstrips()[route->index - 1];
		double time_to_go = airplane->travelTime(from_to[previous_airstrip][p.origin]);
		auto previous_airstrip_departure = route->get_departures()[route->index - 1];
		auto origin_arrival = previous_airstrip_departure + time_to_go;
		
		if (origin->fuel) {
			route->addPlace(p.origin, true, { airplane->max_fuel, 0.0, p.capacity }, origin_arrival, origin_arrival + origin->ground_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight -  airplane->max_fuel - p.weight;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][previous_airstrip][p.origin];

			route->addPlace(p.origin, origin->fuel, { fuel_before - fuel_consumed, 0.0, p.capacity }, origin_arrival, origin_arrival + origin->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] - p.weight + fuel_consumed;
		}
		
		p.solution_from = route->index - 1;
		double time = route->get_departures()[route->index - 1] + airplane->travelTime(from_to[p.origin][p.destination]);

		if (map_airstrip[p.destination].fuel) {
			route->addPlace(p.destination, map_airstrip[p.destination].fuel, { airplane->max_fuel, 0.0, 0 }, time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = airplane->load_weight - airplane->max_fuel;
		}
		else {
			const double fuel_before = route->fuel[route->index - 1];
			const double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][route->get_airstrips()[route->index - 1]][p.destination];
			
			route->addPlace(p.destination, destination->fuel, { fuel_before - fuel_consumed, 0.0, 0 }, time, time + destination->ground_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + fuel_consumed + p.weight;
		}

		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}

	void run_situation_4(ProcessedInput* input, vector<Route>& solution, Passenger& p, int best_route, int best_from, int best_to)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		
		Route* route = &solution[best_route];
		for (int h = best_from; h < best_to; h++) {
			route->add_capacity_at(h, p.capacity);
			route->get_weight_at(h) -= p.weight;
		}

		//**************************************************************************
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = best_from; j < route->index; j++) {
			if (route->get_weights()[j] < 0) {
				add_fuel = route->get_weights()[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (route->get_refueling()[i]) {
						index_refueling = i;
						break;
					}
				}

				for (int t = index_refueling; t < route->index; t++) {
					if (route->get_refueling()[t] && t != index_refueling) break;
					route->fuel[t] += add_fuel;
					route->get_weight_at(t) -= add_fuel;
				}
			}
		}

		p.solution_from = best_from;
		p.solution_to = best_to;
		route->add_passenger(p);
	}

	void run_situation_5(ProcessedInput* input, vector<Route>& solution, Passenger& p, int& best_route, int& best_from)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		
		Route* route = &solution[best_route];
		for (int h = best_from; h < route->index; h++) {
			route->add_capacity_at(h, p.capacity);
			route->get_weight_at(h) -= p.weight;
		}

		//******************************************************************************
		double add_fuel = 0;
		unsigned int index_weight_neg;
		for (int j = best_from; j < route->index; j++) {
			if (route->get_weights()[j] < 0) {
				add_fuel = route->get_weights()[j];
				index_weight_neg = j;
				unsigned int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (route->get_refueling()[i]) {
						index_refueling = i;
						break;
					}
				}

				for (unsigned int t = index_refueling; t < route->index; t++) {
					if (route->get_refueling()[t] && t != index_refueling) break;
					route->fuel[t] += add_fuel;
					route->get_weight_at(t) -= add_fuel;
				}
			}
		}
		//******************************************************************************

		p.solution_from = best_from;
		int place_1 = route->get_airstrips()[route->index - 1];
		double arrival_time = route->get_departures()[route->index - 1] + map_airplane[route->aircraft_code].travelTime(from_to[place_1][p.destination]);
		double departure_time = arrival_time + map_airstrip[p.destination].ground_time;
		
		if (map_airstrip[p.destination].fuel) {
			route->addPlace(p.destination, true, { map_airplane[route->aircraft_code].max_fuel, 0.0, 0 }, arrival_time, departure_time);
			route->get_weight_at(route->index - 1) = map_airplane[route->aircraft_code].load_weight -
				map_airplane[route->aircraft_code].max_fuel;
		}
		else {
			double fuel_before = route->fuel[route->index - 1];
			double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][route->get_airstrips()[route->index - 1]][p.destination];

			route->addPlace(p.destination, false, { fuel_before - fuel_consumed, 0.0, 0 }, arrival_time, departure_time);
			route->get_weight_at(route->index - 1) = route->get_weights()[route->index - 2] + fuel_consumed + p.weight;
		}

		p.solution_to = route->index - 1;
		route->add_passenger(p);
	}
}

void heuristic_costructive_first_fase_secIter(ProcessedInput* input, const PenaltyWeights& penalty_weights, vector<Route>& solution, double end_day, vector<Passenger>& passengers, int number_of_aircraft) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector fuel_consumption = input->get_from_to_fuel_consumed();

	double peso_TW = penalty_weights.time_window;
	double inter_stop_weight = penalty_weights.intermediate_stop;

	vector<Airplane> airplanes; 
	for_each(map_airplane.begin(), map_airplane.end(), [&airplanes](auto& entry) { airplanes.push_back(entry.second); });
	
	int situation = -1;
	int cont = 0;
	vector<int> AereiUsati;
	for (Route& r : solution) 
		AereiUsati.push_back(r.aircraft_code);
	
	do {
		std::vector<int>::iterator it = std::find(AereiUsati.begin(), AereiUsati.end(), airplanes[cont].code);
		if (it == AereiUsati.end()) {
			vector<Passenger> pass_in_solution;
		
			Route r(airplanes[cont].code, pass_in_solution);
			AirplaneStatus airplane_status{ map_airplane[r.aircraft_code].max_fuel, 0.0, 0 };
			r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, airplane_status, 0.0, 0.0);
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
		//qui calcolo solo dove mettere il passeggero
		for (auto& r : solution) {
			Airplane* airplane = &map_airplane[r.aircraft_code];
			
			if (r.primo_pass == false) {
				if (r.index > 0 && r.get_airstrips()[r.index - 1] == p.origin) {
					//in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = airplane->fixed_cost + from_to[p.origin][p.destination];
					double fuel_consumed = fuel_consumption[r.aircraft_code][p.origin][p.destination];

					cost += fuel_consumed;
					//per il check sul fuel:
					double fuel_after_trip = 0.0;
					fuel_after_trip = r.fuel[r.index - 1] - fuel_consumed;

					if (best_cost > cost) {
						if (fuel_after_trip >= (airplane->min_fuel + location_fuel[r.aircraft_code][p.destination])) {
							best_cost = cost;
							best_route = i;
							situation = 1;
						}
					}
				}
				else {
					const auto previous_airstrip = r.get_airstrips()[r.index - 1];
					const double fuel_consumed_primo_tratto = fuel_consumption[r.aircraft_code][previous_airstrip][p.origin];
					const double fuel_consumed_secondo_tratto = fuel_consumption[r.aircraft_code][p.origin][p.destination];
					const double distance_first_leg = from_to[previous_airstrip][p.origin];
					const double distance_second_leg = from_to[p.origin][p.destination];
					
					//qui c'? solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente,
					//devo aggiungere pi? kilometri e un landing stop
					//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
					double cost = airplane->fixed_cost + distance_first_leg + distance_second_leg;
					
					cost += fuel_consumed_secondo_tratto + fuel_consumed_primo_tratto;

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci
					//il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					
					if (map_airstrip[p.origin].fuel) {
						f_after_secondo_tratto = airplane->max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					if (best_cost > cost) {
						if (f_after_primo_tratto >= airplane->min_fuel) {
							if (f_after_secondo_tratto >= (airplane->min_fuel + location_fuel[r.aircraft_code][p.destination])) {
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
				for (unsigned int t = 0; t < r.index; t++) {
					if (r.get_airstrips()[t] == p.origin) 
						FROM.push_back(t);
				}

				if (FROM.size() != 0) {
					//dentro questo abbiamo trovato dei FROM 
					for (unsigned int t = FROM[0]; t < r.index; t++) {
						if (r.get_airstrips()[t] == p.destination) 
							TO.push_back(t);
					}

					if (TO.size() != 0) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from >= to || to - from > p.stop)
									continue;
									
								//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
								//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
								bool capacity_satisfy = true;
								for (int c = from; c < to; c++) {
									if ((r.get_capacities()[c] + p.capacity) > airplane->capacity) 
										capacity_satisfy = false;

									double fuel_consumed = fuel_consumption[r.aircraft_code][r.get_airstrips()[c]][r.get_airstrips()[c + 1]];
									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

									//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAA
									if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.destination];
									if (!r.get_refueling()[c] && r.get_weights()[c] - p.weight + (r.fuel[c] - fuel_i_j) < 0)
										capacity_satisfy = false;
								}
								//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
								if (!capacity_satisfy) 
									break; //check also for the fuel;

								//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider?
								// per l'aggiunta del passeggero qui? la time windows, se calcolo la time windows qua vuol
								// dire che sfrutto il tempo in cui mi trovo in queste posizioni
								double TW_departure = max(0.0, p.early_departure - r.get_arrivals()[from])
													+ max(0.0, r.get_arrivals()[from] - p.late_departure);
								double TW_arrival = max(0.0, p.early_arrival - r.get_arrivals()[to])
													+ max(0.0, r.get_arrivals()[to] - p.late_arrival);

								double cost = (((TW_departure + TW_arrival) * peso_TW) + ((to - from - 1) * inter_stop_weight)) * p.capacity;
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
					//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
					//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
					if (r.get_airstrips()[r.index - 1] != p.destination) {
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (unsigned int c = from; c < r.index; c++) {
								if ((r.get_capacities()[c] + p.capacity) > airplane->capacity) 
									capacity_satisfy = false;

								if (c < r.index - 1) {
									double fuel_consumed = fuel_consumption[r.aircraft_code][r.get_airstrips()[c]][r.get_airstrips()[c + 1]];
									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

									if (!r.get_refueling()[c] && r.get_weights()[c] - p.weight + (r.fuel[c] - fuel_i_j) < 0)
										capacity_satisfy = false;
								}
							}
							//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
							if (capacity_satisfy && (r.index - from) <= p.stop) {
								int previous_airstrip = r.get_airstrips()[r.index - 1];

								double TW_departure = max(0.0, p.early_departure - r.get_arrivals()[from]) + 
													  max(0.0, r.get_arrivals()[from] - p.late_departure);

								const double fuel_consumed = fuel_consumption[r.aircraft_code][previous_airstrip][p.destination];
								double cost = (TW_departure * peso_TW) + fuel_consumed + from_to[r.get_airstrips()[r.index - 1]][p.destination];

								bool fuel_ok = true;
								double remaining_fuel = r.get_weights()[r.index - 1] - p.weight + r.fuel[r.index - 1] - (fuel_consumed + airplane->min_fuel);
							    fuel_ok = r.get_refueling()[r.index - 1] || remaining_fuel>= 0;

								double time_to_go = airplane->travelTime(from_to[previous_airstrip][p.destination]);
								double t_arr_arrival = r.get_arrivals()[r.index - 1] + time_to_go + map_airstrip[previous_airstrip].ground_time;
								double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival) + 
													max(0.0, t_arr_arrival - p.late_arrival);
								
								cost += (TW_arrival * peso_TW) * p.capacity + (inter_stop_weight * (r.index - from - 1)) * p.capacity;

								if (fuel_ok && best_cost > cost && t_arr_arrival <= end_day) {
									if (r.fuel[r.index - 1] - fuel_consumed >= (airplane->min_fuel + location_fuel[r.aircraft_code][p.destination])) {
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
				if (r.get_airstrips()[r.index - 1] != p.origin) {
					auto previous_airstrip = r.get_airstrips()[r.index - 1];
					//guardo il costo di aggiungere un pezzo alla fine
					double fuel_consumed = fuel_consumption[r.aircraft_code][r.get_airstrips()[r.index - 1]][p.origin]
										 + fuel_consumption[r.aircraft_code][p.origin][p.destination];
					double cost = fuel_consumed + from_to[previous_airstrip][p.origin] + from_to[p.origin][p.destination];

					double time_to_go = map_airplane[r.aircraft_code].travelTime(from_to[previous_airstrip][p.origin]);
					double t_arr_departure = r.get_arrivals()[r.index - 1] + map_airstrip[previous_airstrip].ground_time + time_to_go;
					double TW_departure = max(0.0,  p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
					
					time_to_go = map_airplane[r.aircraft_code].travelTime(from_to[p.origin][p.destination]);
					double t_arr_arrival = t_arr_departure + map_airstrip[p.origin].ground_time + time_to_go;
					double TW_arrival = max(0.0, t_arr_arrival - p.late_arrival) + 
										max(0.0, p.early_arrival - t_arr_arrival);
					cost += (TW_departure + TW_arrival) * peso_TW * p.capacity;

					//per il check sul fuel:
					double fuel_consumed_primo_tratto = fuel_consumption[r.aircraft_code][previous_airstrip][p.origin];

					//****
					double fuel_consumed_secondo_tratto = fuel_consumption[r.aircraft_code][p.origin][p.destination];
					double f_after_primo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.origin].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					if (best_cost > cost && t_arr_arrival <= end_day && f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
						if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.destination])) {
							best_cost = cost;
							best_route = i;
							situation = 3;
						}
					}
				}
			}
			i++;
		}

		//ora devo aggiungere il passeggero nel posto migliore, quindi serve valutare in che caso sono A,B,C,D
		if (situation == 1) {
			heuristic_costructive_first_fase_secIter_namespace::run_situation_1(input, solution, p, best_route);
		}
		else if (situation == 2) {
			heuristic_costructive_first_fase_secIter_namespace::run_situation_2(input, solution, p, best_route);
		}
		else if (situation == 3) {
			heuristic_costructive_first_fase_secIter_namespace::run_situation_3(input, solution, p, best_route);
		}
		else if (situation == 4) {
			heuristic_costructive_first_fase_secIter_namespace::run_situation_4(input, solution, p, best_route, best_from, best_to);
		}
		else if (situation == 5) {
			heuristic_costructive_first_fase_secIter_namespace::run_situation_5(input, solution, p, best_route, best_from);
		}
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