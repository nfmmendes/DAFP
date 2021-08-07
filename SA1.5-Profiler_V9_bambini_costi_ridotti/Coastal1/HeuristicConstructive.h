#pragma once 

#ifndef HEURISTIC_CONSTRUCTIVE_H
#define HEURISTIC_CONSTRUCTIVE_H

#include <iostream>
#include "Route.h"
#include "ConstructiveFirstFaseSequential.h"
#include "ConstructiveHeuristicFirstFase.h"

inline double cost_time_windows_for_route(const Route& r, double peso_TW) {
	double cost = 0.0;

	for (const Passenger& p : r.passengers_in_route) {

		double TW_departure = ConstructiveHeuristic::evaluate_tw_departure(p, r.time_arr[p.solution_from]);
		double TW_arrival = ConstructiveHeuristic::evaluate_tw_arrival(p, r.time_arr[p.solution_to]);
		cost += (TW_departure + TW_arrival) * p.capacity;
	}
	cost = cost * peso_TW;
	return cost;
}




vector<Route> heuristic_costructive_first_fase(Penalty_Weights penalties, Fleet & airplanes,Input_maps maps, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed) {

	return (new ConstructiveHeuristicFirstFase(penalties, airplanes, maps, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed))->Execute();
}






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
							r_support.time_arr[j] += 2; //TODO: Why 2?
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





vector<Route> heuristic_costructive_first_fase_sequential(const Penalty_Weights &penalties, Fleet& airplanes,const Input_maps &maps, double end_day, vector<Passenger> passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed) {

	return (new ConstructiveFirstFaseSequential(penalties, airplanes, maps, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed))->Execute();
	

}

#endif