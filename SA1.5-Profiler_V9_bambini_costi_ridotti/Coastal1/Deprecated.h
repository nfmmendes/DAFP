#ifndef DEPRECATED_H
#define DEPRECATED_H
#include <iostream>
#include <vector>


#include "HeuristicConstructive.h"
#include "Route.h"


void calculate_ObjectiveFunction_final_arc_iori(double costo_company, Penalty_Weights& weights, std::vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, Double3DVector& from_to_FuelConsumed) {

	double peso_TW = weights.time_window_weight;
	double peso_intermediate_stop = weights.stop_weight;

	double cost = 0.0;
	double costo_fisso = 0.0;
	double costo_km = 0.0;
	double costo_fuel = 0.0;
	double costo_landing = 0.0;
	double costo_Time_Window = 0.0;
	double costo_Intermediate = 0.0;

	int depot, maxFuel, groundTime, maxWeight;
	double speed, arrivalTime;

	for (Route& r : solution) {
		if (r.places[r.index - 1] != 1 || r.places[r.index - 1] != 4) {
			//int place, bool refueling, double quantity_fuel, double weight, int capacity, double minute_arr, double minute_dep

			depot = map_airplane[r.aircraft_code].depot;
			maxFuel = map_airplane[r.aircraft_code].max_fuel;
			maxWeight = map_airplane[r.aircraft_code].max_weight;
			speed = map_airplane[r.aircraft_code].speed;
			groundTime = map_airstrip[depot].ground_time;
			arrivalTime = r.time_dep[r.index - 1] + (from_to[r.places[r.index - 1]][depot] / speed);

			r.addPlace(depot, 1, maxFuel, maxWeight - maxFuel, 0, arrivalTime, arrivalTime + groundTime);
		}

		r.print();
	}

	for (auto& r : solution) {
		double cost_route = 0.0;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
			costo_fisso += map_airplane[r.aircraft_code].fixed_cost;
		}

		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				cost_route += map_airstrip[r.places[i]].landing_cost;
				costo_landing += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					costo_km += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
					costo_fuel += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (const auto& p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			costo_Intermediate += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;


			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			costo_Time_Window += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
	}

	//cout << "costo fisso; costo landing; costo fuel; costo km; costo intermediate; costo tw; minuti TW; N_intermediate; gap_perc; time_ALNS; time_model; time_incumbent; numeroSA" << endl;
	double minuti_TW = (costo_Time_Window / peso_TW);
	double n_intermediate = (costo_Intermediate / peso_intermediate_stop);
	double gap_perc = ((cost - costo_company) / costo_company) * 100;
	std::cout << costo_fisso << ";" << costo_landing << ";" << costo_fuel << ";" << costo_km << ";" << costo_Intermediate << ";" << costo_Time_Window << ";" << minuti_TW << ";" << n_intermediate << ";" << gap_perc << ";";



}




#endif

