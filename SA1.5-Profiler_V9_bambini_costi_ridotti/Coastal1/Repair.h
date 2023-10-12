#ifndef  REPAIR_H
#define REPAIR_H

#include <random>

#include "Constructive.h"
#include "Evaluate.h"
#include "Feasibility.h"
#include "Model_Cplex.h"
#include "Route.h"

double get_time_window_cost(Passenger p, Route* route, double time)
{
	double t_arr_departure = route->get_arrivals()[route->index - 1];
	double t_arr_arrival = route->get_departures()[route->index - 1] + time;

	double time_window_cost = max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
	time_window_cost += max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);

	return time_window_cost;
}

vector <Route> repair_perturbation(ProcessedInput* input, const PenaltyWeights penalty_weights, double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	//if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool case_first_passenger = false; 
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		
		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index != 1)
					continue; 

				//c'? solo il deposito in questo rebuilt_case
				double cost = from_to[routes_destroyed[r].get_airstrips()[0]][p.origin] + from_to[p.origin][p.destination] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
				cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].get_airstrips()[0]][p.origin];
				cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.origin][p.destination];

				//code for casual number [0.8;1.2]*****************************************************
				double f = (double)rand() / RAND_MAX;
				f = 0.8 + (f * 0.4);
				cost = cost * f;
					
				if (cost >= best_cost)
					continue; 

				best_cost = cost;
				best_route = r;
				case_first_passenger = false;
			}
			else {

				double cost_route_before = cost_single_route(input, penalty_weights, routes_destroyed[r]);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.origin == routes_destroyed[r].get_airstrips()[n] && n != 0)
						continue;
					
					for (unsigned int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
						if (p.destination == routes_destroyed[r].get_airstrips()[n1])
							continue; 

						Route r_support = routes_destroyed[r];

						bool non_to = false;
						bool non_to_final = false;
						bool num_equals = false;
						int node_add_from = n;
						int node_add_to = n1;

						r_support.update_rebuilt_one_first_fase(input, node_add_from, node_add_to, p.origin, p.destination, p, non_to, non_to_final, num_equals);

						if (r_support.get_arrivals()[r_support.index - 1] > end_day)
							continue;

						r_support.update_rebuilt_one_second_fase(input, node_add_from, node_add_to, p.destination, p, non_to, non_to_final, num_equals);

						if ((p.solution_to - p.solution_from > p.stop))
							continue; 
								
						if (!route_feasible(input, r_support, end_day))
							continue;
								
						double cost = (cost_single_route(input, penalty_weights, r_support) + cost_time_windows_for_route_passenger(r_support, p, penalty_weights.time_window)) + (penalty_weights.intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;

						//code for casual number [0.8;1.2]*****************************************************
						double f = (double)rand() / RAND_MAX;
						f = 0.8 + (f * 0.4);
						cost = cost * f;
						//*************************************************************************************

						if (best_cost <= cost)
							continue;

						best_route = r;
						best_cost = cost;
						move_c = false;
						route_best = r_support;
						from_per_route = p.solution_from;
						to_per_route = p.solution_to;
					}
				}
			}

			//for move C
			if (routes_destroyed[r].primo_pass) {
				Route* route = &routes_destroyed[r];
				Airplane* airplane = &map_airplane[route->aircraft_code];
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (route->get_airstrips()[routes_destroyed[r].index - 1] == p.origin) {
					double dist = from_to[p.origin][p.destination];
					double time = airplane->travelTime(from_to[p.origin][p.destination]);

					double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][p.origin][p.destination];
					double time_window_cost = get_time_window_cost(p, route, time);
					
					double cost = dist + (time_window_cost * penalty_weights.time_window) + fuel_consumed;

					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************

					if (best_cost <= cost) {
						if (route->get_departures()[route->index - 1] + time > end_day) {
							if (route->fuel[route->index - 1] - fuel_consumed < (airplane->min_fuel + location_fuel[route->aircraft_code][p.destination])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
							}
						}
					}
				}
				else {
					Route* route = &routes_destroyed[r];
					airplane = &map_airplane[routes_destroyed[r].aircraft_code];
					
					// the departure is not equals to the last place of the route
					double cost = from_to[route->get_airstrips()[route->index - 1]][p.origin] + from_to[p.origin][p.destination];
					double ground_time = map_airstrip[p.origin].ground_time;
					double travel_time = airplane->travelTime(from_to[route->get_airstrips()[route->index - 1]][p.origin]);
					double t_arr_departure = route->get_departures()[route->index - 1] + travel_time;

					travel_time = airplane->travelTime(from_to[p.origin][p.destination]);
					double t_arr_arrival = t_arr_departure + ground_time + travel_time;
					
					double TW_departure = max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
					double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival)+ max(0.0, t_arr_arrival - p.late_arrival);

					double TW_cost = (TW_departure + TW_arrival);
					cost += TW_cost * penalty_weights.time_window * p.capacity;

					//per il check sul fuel:
					double first_hour_fuel = from_to_FuelConsumed[route->aircraft_code][route->get_airstrips()[route->index - 1]][p.origin];
					double after_first_hour_fuel = from_to_FuelConsumed[route->aircraft_code][p.origin][p.destination];
					cost += first_hour_fuel + after_first_hour_fuel;
					
					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il rebuilt_case in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = routes_destroyed[r].fuel[routes_destroyed[r].index - 1] - first_hour_fuel;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.destination].fuel) {
						f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - after_first_hour_fuel;
					}
					else {
						f_after_secondo_tratto = routes_destroyed[r].fuel[routes_destroyed[r].index - 1] - first_hour_fuel - after_first_hour_fuel;
					}

					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************
					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= airplane->min_fuel) {
								if (f_after_secondo_tratto >= (airplane->min_fuel + location_fuel[route->aircraft_code][p.destination])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
								}
							}
						}
					}
				}
			}

		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
		else {

			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'? solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {
				if (move_c) {
					//i have to do the move C
					routes_destroyed[best_route].move_c(input, p, p.origin, p.destination);
				}
				else {
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}
			routes_destroyed[best_route].add_passenger(p);
		}
	}

		return routes_infeasible ? std::vector<Route>() : routes_destroyed;
}

vector <Route> repair_one_inter_move(ProcessedInput* input, const PenaltyWeights &penalty_weights, double end_day, vector<Route> routes_destroyed, vector<Passenger> passengers_removed) {
	
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_fuel_consumed = input->get_from_to_fuel_consumed();
	
	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {

			Route* route = &routes_destroyed[r];
			double2DVector fuel_consumed = from_to_fuel_consumed[route->aircraft_code];
			Airplane* airplane = &map_airplane[route->aircraft_code];

			if (routes_destroyed[r].primo_pass == false) {	
				if (routes_destroyed[r].index == 1) {
					//c'? solo il deposito in questo rebuilt_case
					double cost = from_to[route->get_airstrips()[0]][p.origin] + from_to[p.origin][p.destination] + airplane->fixed_cost;
					cost += fuel_consumed[route->get_airstrips()[0]][p.origin];
					cost += fuel_consumed[p.origin][p.destination];

					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				double cost_route_before = cost_single_route(input, penalty_weights, routes_destroyed[r]);
				for (unsigned int n = 0; n < route->index - 1; n++) {
					if (p.origin != route->get_airstrips()[n] || n == 0) {
						for (unsigned int n1 = n; (n1 < route->index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.destination != route->get_airstrips()[n1]) {
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_rebuilt_one_first_fase(input, node_add_from, node_add_to, p.origin, p.destination, p, non_to, non_to_final, num_equals);

								if (r_support.get_arrivals()[r_support.index - 1] <= end_day) {

									r_support.update_rebuilt_one_second_fase(input, node_add_from, node_add_to, p.destination, p, non_to, non_to_final, num_equals);

									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(input, r_support, end_day)) {
											double cost = (cost_single_route(input, penalty_weights, r_support) + cost_time_windows_for_route_passenger(r_support, p, penalty_weights.time_window)) + (penalty_weights.intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
											if (best_cost > cost) {
												best_route = r;
												best_cost = cost;
												move_c = false;
												route_best = r_support;
												from_per_route = p.solution_from;
												to_per_route = p.solution_to;
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//for move C
			if (route->primo_pass) {
				
				Airplane* airplane = &map_airplane[route->aircraft_code];
				
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (route->get_airstrips()[route->index - 1] == p.origin) {
					double dist = from_to[p.origin][p.destination];
					double time = airplane->travelTime(from_to[p.origin][p.destination]);

					double fuel_consumed = from_to_fuel_consumed[route->aircraft_code][p.origin][p.destination];
					double time_window_cost = get_time_window_cost(p, route, time);

					double cost = dist + ((time_window_cost * penalty_weights.time_window) * p.capacity) + fuel_consumed;
					if (best_cost > cost) {
						if (route->get_arrivals()[route->index - 1] + time <= end_day) {
							if (route->fuel[route->index - 1] - fuel_consumed >= (airplane->min_fuel + location_fuel[route->aircraft_code][p.destination])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
							}
						}
					}
				}
				else {
					
					// the departure is not equals to the last place of the route
					double cost = from_to[route->get_airstrips()[route->index - 1]][p.origin] + from_to[p.origin][p.destination];
					double travelTime = airplane->travelTime(from_to[route->get_airstrips()[route->index - 1]][p.origin]);
					double t_arr_departure = route->get_departure_at(route->index - 1) + travelTime;
					travelTime = airplane->travelTime(from_to[p.origin][p.destination]);

					double t_arr_arrival = t_arr_departure + map_airstrip[p.origin].ground_time + travelTime; 
					double TW_departure = max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
					double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);

					cost += ((TW_departure + TW_arrival) * penalty_weights.time_window) * p.capacity;

					//per il check sul fuel:
					double first_part_consumption = fuel_consumed[route->get_airstrips()[route->index - 1]][p.origin];
					double second_part_consumption = fuel_consumed[p.origin][p.destination];

					cost += first_part_consumption + second_part_consumption;

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per
					//il passeggero potrebbe esserci il rebuilt_case in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = route->fuel[route->index - 1] - first_part_consumption;
					double f_after_secondo_tratto = 0.0;
					f_after_secondo_tratto = (map_airstrip[p.origin].fuel ? airplane->max_fuel : f_after_primo_tratto) - second_part_consumption;

					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= airplane->min_fuel) {
								if (f_after_secondo_tratto >= (airplane->min_fuel + location_fuel[route->aircraft_code][p.destination])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
								}
							}
						}
					}
				}
			}
		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
		else {

			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'? solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {
				if (move_c) {
					//i have to do the move C
					routes_destroyed[best_route].move_c(input, p, p.origin, p.destination);
				}
				else {
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}

			routes_destroyed[best_route].add_passenger(p);
		}
	}

	return routes_infeasible? vector<Route>() : routes_destroyed;

}

vector<Route> repairSP(ProcessedInput* input, const PenaltyWeights &penalty_weights, vector<Route>& route_destroy, vector<Passenger>& passenger_removed, double end_day, vector<Passenger>& passengers, int number_of_aircraft) {
	
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	vector<vector<Route>> solutionAllSub;
	vector<Airplane> airplanes;
	int NAirplane = 20;

	for_each(map_airplane.begin(), map_airplane.end(), [&airplanes](auto entry) { airplanes.push_back(entry.second); });
	
	double dividendo = (double)(number_of_aircraft * passenger_removed.size());
	double denominatore = (double)(passengers.size());

	int number_of_aircraft_subset = 10 + (int)(round(dividendo / denominatore));
	if (number_of_aircraft_subset > number_of_aircraft)  
		number_of_aircraft_subset = number_of_aircraft;

	int iteration_subset = 0;
	int iteration_max_subset = (int)(passenger_removed.size()) / 3;
	
	int Best_iteration = 0;
	double Best_cost = std::numeric_limits<double>::max();
	do {
		// Lo ordino in modo random
		auto rng2 = default_random_engine{};
		shuffle(passenger_removed.begin(), passenger_removed.end(), rng2);
		vector<Route> solution;
		vector<Route> Route_to_fix = route_destroy;
		
		heuristic_costructive_first_fase_secIter(input, penalty_weights, Route_to_fix, end_day, passenger_removed, number_of_aircraft);
		double cost_fix_single = calculate_objective_function(input, penalty_weights, Route_to_fix);

		//cout << "***********************SOLUTION SECOND FASE***********************" << endl;
		solution = heuristic_costructive_second_fase_SP(Route_to_fix, end_day, penalty_weights.time_window);
		cost_fix_single = calculate_objective_function(input, penalty_weights, solution);
		if (cost_fix_single < Best_cost) {
			Best_cost = cost_fix_single;
			Best_iteration = iteration_subset;
		}

		solutionAllSub.push_back(solution);
		iteration_subset++;
	} while (iteration_subset < iteration_max_subset);

	//Mi preparo per chiamare il modello con il Pool di route generato
	vector<Airplane> airplanes_model;
	map<int, vector<Route>> airplane_routes;
	
	for_each(map_airplane.begin(), map_airplane.end(), [&airplanes_model](auto& k) { airplanes_model.push_back(k.second); });
	for_each(airplanes_model.begin(), airplanes_model.end(), [&airplane_routes](auto& k) { airplane_routes[k.code] = vector<Route>(); });
	
	for (auto& solution : solutionAllSub) {
		for (Route& r : solution) {
			airplane_routes[r.aircraft_code].push_back(r);
		}
	}

	int3DVector A3;  //matrix A
	vector<vector<double>> C;  //cost
	vector<Route> routes;
	for (auto& airplane : airplanes_model) {
		vector<double> c;
		vector<vector<int>> A2;
		for (Route& r : airplane_routes[airplane.code]) {
			routes.push_back(r);
			vector<int> A1;
			for (Passenger& p : passengers) {
				auto it = find_if(r.get_passengers().begin(), r.get_passengers().end(), [=](const Passenger& p_) { return p.pnr == p_.pnr; });

				if (it != r.get_passengers().end()) {
					A1.push_back(1);
				}
				else {
					A1.push_back(0);
				}
			}
			A2.push_back(A1);
			c.push_back(r.cost);
		}
		A3.push_back(A2);
		C.push_back(c);
	}

	//Creating model with gurobi
	//Create new Model object
	//Mi preparo a ricere le route scelte dal modello
	vector<Route> solution_model;
	Model_Cplex* model = new Model_Cplex(A3, C, airplanes_model, airplane_routes, routes, (int)(passengers.size()), passengers, solution_model, map_airplane);
	//Create and solve model with gurobi
	model->create_Model_for_SP_cplex(NAirplane);
	// adesso vettore delle Route non ? piu vuoto
	// Non ho piu tutti gli aerei disponibili cout<<endl
	int n = 0;
	for (Route& r : model->solution_model) 
		n += (int)r.get_passengers().size();

	if ((int)(model->solution_model.size()) > 0 && (n == (int)passengers.size())) {
		for (Route& s : model->solution_model) solution_model.push_back(s);
		delete model;
		return solution_model;
	}
	n = 0;
	for (Route& r : solutionAllSub[Best_iteration]) 
		n += (int)r.get_passengers().size();

	if (n == (int)passengers.size()) {
		delete model;
		return solutionAllSub[Best_iteration];
	}
	else {
		vector<Route> vuoto(0);
		delete model;
		return vuoto;
	}
}

double get_cost(ProcessedInput* input, const PenaltyWeights& penalty_weights, const Passenger& p, double cost_route_before,
	const Route& support)
{
	return (cost_single_route(input, penalty_weights, support) +
			cost_time_windows_for_route_passenger(support, p, penalty_weights.time_window)) +
		(penalty_weights.intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
}

vector <Route> repair_one(ProcessedInput* input, const PenaltyWeights& penalty_weights, double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {
	double peso_TW = penalty_weights.time_window;

	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			Route* route = &routes_destroyed[r];
			Airplane* airplane = &map_airplane[route->aircraft_code];
			double2DVector fuel_consumed = from_to_FuelConsumed[route->aircraft_code];
			
			if (route->primo_pass == false && route->index == 1) {
				//c'? solo il deposito in questo rebuilt_case
				double cost = from_to[route->get_airstrips()[0]][p.origin] + from_to[p.origin][p.destination] + airplane->fixed_cost;
				cost += fuel_consumed[route->get_airstrips()[0]][p.origin] + fuel_consumed[p.origin][p.destination];

				if (cost < best_cost) {
					best_cost = cost;
					best_route = r;
					case_first_passenger = false;
				}
			}
			else {

				double cost_route_before = cost_single_route(input, penalty_weights, routes_destroyed[r]);
				for (unsigned int n = 0; n < route->index - 1; n++) {
					if (p.origin == route->get_airstrips()[n] && n != 0)
						continue;
					
					for (unsigned int n1 = n; (n1 < route->index) && (n1 - n <= p.stop + 1); n1++) {
						if (p.destination == route->get_airstrips()[n1])
							continue; 
							
						Route support = routes_destroyed[r];

						bool non_to = false;
						bool non_to_final = false;
						bool num_equals = false;
						int node_add_from = n;
						int node_add_to = n1;

						support.update_rebuilt_one_first_fase(input, node_add_from, node_add_to, p.origin, 
															  p.destination, p, non_to, non_to_final, num_equals);

						if (support.get_arrivals()[support.index - 1] > end_day)
							continue;
						
						support.update_rebuilt_one_second_fase(input, node_add_from, node_add_to, p.destination, 
																p, non_to, non_to_final, num_equals);

						if ((p.solution_to - p.solution_from > p.stop))
							continue;

						if (!route_feasible(input, support, end_day))
							continue; 
								
						double cost = get_cost(input, penalty_weights, p, cost_route_before, support);
						if (best_cost <= cost)
							continue;
									
						best_route = r;
						best_cost = cost;
						move_c = false;

						route_best = support;
						from_per_route = p.solution_from;
						to_per_route = p.solution_to;
					}
				}
			}

			//for move C
			if (routes_destroyed[r].primo_pass) {
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (routes_destroyed[r].get_airstrips()[routes_destroyed[r].index - 1] == p.origin) {
					double dist = from_to[p.origin][p.destination];
					double time = airplane->travelTime(from_to[p.origin][p.destination]);

					double trip_fuel_consumed = fuel_consumed[p.origin][p.destination];

					double time_window_cost = 0.0;
					double t_arr_departure = route->get_arrivals()[route->index - 1];
					double t_arr_arrival = route->get_departures()[route->index - 1] + time;

					time_window_cost += max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
					time_window_cost += max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);

					double cost = dist + time_window_cost * peso_TW * p.capacity + trip_fuel_consumed;
					if (best_cost > cost) {
						if (route->get_departures()[route->index - 1] + time <= end_day) {
							auto right_hand = (airplane->min_fuel + location_fuel[route->aircraft_code][p.destination]);
							if (route->fuel[route->index - 1] - trip_fuel_consumed >= right_hand) {
								best_cost = cost;
								best_route = r;
								move_c = true;
							}
						}
					}
				}
				else {
					// the departure is not equals to the last place of the route
					double cost = from_to[route->get_airstrips()[route->index - 1]][p.origin] + from_to[p.origin][p.destination];
					double travel_time = airplane->travelTime(from_to[route->get_airstrips()[route->index - 1]][p.origin]);
					double t_arr_departure = route->get_departures()[route->index - 1] + travel_time;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.origin].ground_time + airplane->travelTime(from_to[p.origin][p.destination]);

					double TW_departure = max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
					double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);

					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					const double first_step_consumption = fuel_consumed[route->get_airstrips()[route->index - 1]][p.origin];
					const double second_step_consumption = fuel_consumed[p.origin][p.destination];

					cost += first_step_consumption + second_step_consumption;

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe
					//esserci il rebuilt_case in cui l'aero possa fare refuel
					double f_after_primo_tratto = route->fuel[route->index - 1] - first_step_consumption;
					double f_after_secondo_tratto = 0.0;
					
					f_after_secondo_tratto = (map_airstrip[p.origin].fuel ? airplane->max_fuel : f_after_primo_tratto) - second_step_consumption;

					if (best_cost > cost && t_arr_arrival <= end_day && f_after_primo_tratto >= airplane->min_fuel) {
						if (f_after_secondo_tratto >= (airplane->min_fuel + location_fuel[route->aircraft_code][p.destination])) {
							best_cost = cost;
							best_route = r;
							move_c = true;
						}
					}
				}
			}
		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
			
		if (!routes_destroyed[best_route].primo_pass) {
			//i have to do the new things, 
			if (case_first_passenger == false) {
				//c'? solo il depot
				routes_destroyed[best_route].primo_pass = true;
				routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
			}
		}
		else {
			if (move_c) {
				//i have to do the move C
				routes_destroyed[best_route].move_c(input, p, p.origin, p.destination);
			}
			else {
				routes_destroyed[best_route] = route_best;
				p.solution_from = from_per_route;
				p.solution_to = to_per_route;
			}
		}

		routes_destroyed[best_route].add_passenger(p);
	}

	return routes_infeasible ? vector<Route>() : routes_destroyed;
}

vector<Route> two_regret_repair_agregate(ProcessedInput* input, const PenaltyWeights& penalty_weights, double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;

	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	//bool routes_infeasible = false;
	vector<double> regret_diff;
	vector<int> regret_move_c; //1 move C, 0 no move C
	vector<int> regret_best_route;
	// if -1 = infeasible, passenger can insert in no-route
	// if 0...route.size() normal inserition with the function
	vector<int> regret_arc_from;
	vector<int> regret_arc_to;
	vector<int> regret_index_pass;

	map<int, int> map_pass_aggragati; //nella stringa, che ? la chiave, ci sono from/to/timedep/timearr, nell'int c'? l'indicie di tutti i valori

	while (!passengers_removed.empty()) {
		int index_p = 0;
		for (Passenger& p : passengers_removed) {

			vector<double> costs;
			vector<int> route;
			vector<int> arc_from;
			vector<int> arc_to;
			vector<int> move_c;

			int chiave = p.chiave_regret;
			if (map_pass_aggragati.count(chiave) >= 1) {
				regret_diff.push_back(regret_diff[map_pass_aggragati[chiave]]);
				regret_move_c.push_back(regret_move_c[map_pass_aggragati[chiave]]);
				regret_best_route.push_back(regret_best_route[map_pass_aggragati[chiave]]);
				regret_arc_from.push_back(regret_arc_from[map_pass_aggragati[chiave]]);
				regret_arc_to.push_back(regret_arc_to[map_pass_aggragati[chiave]]);
				regret_index_pass.push_back(regret_index_pass[map_pass_aggragati[chiave]]);
			}
			else {
				//Route r_support; //attento qua, ? quello di prova
				for (int r = 0; r < (int)routes_destroyed.size(); r++) {
					Route* d_route = &routes_destroyed[r]; 
					Airplane* airplane = &map_airplane[routes_destroyed[r].aircraft_code];
					
					if (routes_destroyed[r].primo_pass == false) {
						if (routes_destroyed[r].index == 1) {
							//c'? solo il deposito in questo rebuilt_case
							double cost = from_to[d_route->get_airstrips()[0]][p.origin] + from_to[p.origin][p.destination] + airplane->fixed_cost;
							cost += from_to_FuelConsumed[d_route->aircraft_code][d_route->get_airstrips()[0]][p.origin];
							cost += from_to_FuelConsumed[d_route->aircraft_code][p.origin][p.destination];

							costs.push_back(cost);
							route.push_back(r);
							arc_from.push_back(-1);
							arc_to.push_back(-1);
							move_c.push_back(0);
						}
					}
					else {
						
						double cost_route_before = cost_single_route(input, penalty_weights, routes_destroyed[r]);
						for (unsigned int n = 0; n < d_route->index - 1; n++) {
							if (p.origin != d_route->get_airstrips()[n] || n == 0) {
								for (unsigned int n1 = n; (n1 < d_route->index) && (n1 - n <= p.stop + 1); n1++) {
									if (p.destination != d_route->get_airstrips()[n1]) {
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										//int rebuilt_case = -1;
										int node_add_from = n;
										int node_add_to = n1;

										r_support.update_rebuilt_one_first_fase(input, node_add_from, node_add_to, p.origin, p.destination, p, non_to, non_to_final, num_equals);

										if (r_support.get_arrivals()[r_support.index - 1] <= end_day) {


											r_support.update_rebuilt_one_second_fase(input, node_add_from, node_add_to, p.destination, p, non_to, non_to_final, num_equals);
											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible(input, r_support, end_day)) {

													double cost = (cost_single_route(input, penalty_weights, r_support) + 
																  cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + 
																	(peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;

													if (costs.size() == 0) {
														costs.push_back(cost);
														arc_from.push_back(n);
														arc_to.push_back(n1);
														route.push_back(r);
														move_c.push_back(0);
													}

													if (costs.size() >= 1) {
														if (cost != costs[costs.size() - 1]) {
															costs.push_back(cost);
															arc_from.push_back(n);
															arc_to.push_back(n1);
															route.push_back(r);
															move_c.push_back(0);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}

					//for move C
					if (d_route->primo_pass) {
						//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
						if (d_route->get_airstrips()[d_route->index - 1] == p.origin) {
							double dist = from_to[p.origin][p.destination];
							double time = airplane->travelTime(from_to[p.origin][p.destination]);
							double fuel_consumed = from_to_FuelConsumed[d_route->aircraft_code][p.origin][p.destination];

							double time_window_cost = 0.0;
							if (d_route->get_arrivals()[d_route->index - 1] < p.early_departure)
								time_window_cost += p.early_departure - d_route->get_arrivals()[routes_destroyed[r].index - 1];
							else if (d_route->get_arrivals()[d_route->index - 1] > p.late_departure) 
								time_window_cost += d_route->get_arrivals()[d_route->index - 1] - p.late_departure;

							if ((d_route->get_departures()[d_route->index - 1] + time) < p.early_arrival) 
								time_window_cost += p.early_arrival - (d_route->get_departures()[d_route->index - 1] + time);
							else if ((d_route->get_departures()[d_route->index - 1] + time) > p.late_arrival) 
								time_window_cost += (d_route->get_departures()[d_route->index - 1] + time) - p.late_arrival;

							double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
							if(d_route->get_departures()[d_route->index - 1] + time <= end_day) {
								if (d_route->fuel[d_route->index - 1] - fuel_consumed >= (airplane->min_fuel + location_fuel[d_route->aircraft_code][p.destination])) {
									costs.push_back(cost);
									route.push_back(r);
									move_c.push_back(1);
									arc_from.push_back(-2);
									arc_to.push_back(-2);
								}
							}
						}
						else {
							// the departure is not equals to the last place of the route
							double cost = from_to[d_route->get_airstrips()[d_route->index - 1]][p.origin] + from_to[p.origin][p.destination];
							double time_to_go = airplane->travelTime(from_to[d_route->get_airstrips()[d_route->index - 1]][p.origin]);
							double t_arr_departure = d_route->get_departures()[d_route->index - 1] + time_to_go;

							time_to_go = airplane->travelTime(from_to[p.origin][p.destination]);
							double t_arr_arrival = t_arr_departure + map_airstrip[p.origin].ground_time + time_to_go ;

							double TW_departure = max(0.0, p.early_departure - t_arr_departure) + max(0.0, t_arr_departure - p.late_departure);
							double TW_arrival = max(0.0, p.early_arrival - t_arr_arrival) + max(0.0, t_arr_arrival - p.late_arrival);
							
							cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

							//per il check sul fuel:
							double fuel_consumed_primo_tratto = from_to_FuelConsumed[d_route->aircraft_code][d_route->get_airstrips()[d_route->index - 1]][p.origin];
							double fuel_consumed_secondo_tratto = from_to_FuelConsumed[d_route->aircraft_code][p.origin][p.destination];

							cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
							//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il rebuilt_case in cui l'aero possa fare 
							//refuel
							double f_after_primo_tratto = d_route->fuel[d_route->index - 1] - fuel_consumed_primo_tratto;
							double f_after_secondo_tratto = 0.0;
							if (map_airstrip[p.origin].fuel) {
								f_after_secondo_tratto = airplane->max_fuel - fuel_consumed_secondo_tratto;
							}
							else {
								f_after_secondo_tratto = d_route->fuel[d_route->index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
							}

							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= airplane->min_fuel) {
									if (f_after_secondo_tratto >= airplane->min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.destination]) {
										costs.push_back(cost);
										move_c.push_back(1);
										arc_from.push_back(-2);
										arc_to.push_back(-2);
										route.push_back(r);
									}
								}
							}
						}
					}
				}


				if (costs.size() == 1) {

					//regret_diff.push_back(0);
					regret_diff.push_back(0);  //metto valore zero ma non dovrebbe succedere
					regret_best_route.push_back(route[0]);
					regret_move_c.push_back(move_c[0]);
					regret_arc_from.push_back(arc_from[0]);
					regret_arc_to.push_back(arc_to[0]);
					regret_index_pass.push_back(index_p);
				}
				else if (costs.size() > 1) {
					//qua lo posso ottimizzare****************************************************************************************************************************************
					//con il codice di TED per ordinare, guarda bene come salvare l'indice********************************************************************************************

					vector<double>::iterator index_iterator = min_element(costs.begin(), costs.end());
					auto index_mosse = distance(costs.begin(), index_iterator);
					double best_cost = costs[index_mosse];
					costs.erase(costs.begin() + index_mosse);
					vector<double>::iterator index_iterator2 = min_element(costs.begin(), costs.end());
					double best_cost_2 = costs[distance(costs.begin(), index_iterator2)];

					regret_diff.push_back(best_cost_2 - best_cost);
					regret_best_route.push_back(route[index_mosse]);
					regret_move_c.push_back(move_c[index_mosse]);
					regret_arc_from.push_back(arc_from[index_mosse]);
					regret_arc_to.push_back(arc_to[index_mosse]);
					regret_index_pass.push_back(index_p);
				}
				else {
					//rebuilt_case in cui non ha trovato mosse per quella persona
					//in questo rebuilt_case vuol dire che la solutione ? infeasible
					cout << "LA SOLUZIONE E' INFEASIBLE" << endl;
					vector<Route> route_vuote;
					return route_vuote;

				}

				int val = (int)regret_diff.size() - 1;
				int primo_val = p.chiave_regret;
				map_pass_aggragati.insert(make_pair(primo_val, val));

				costs.clear();
				route.clear();
				arc_from.clear();
				arc_to.clear();
				move_c.clear();

			}

			index_p++;
		}

		int index = 0;
		double best = DBL_MIN; //prima era -10000
		for (int i = 0; i < (int)regret_diff.size(); i++) {
			if (best < regret_diff[i]) {
				index = i;
				best = regret_diff[i];
			}
		}

		if (!routes_destroyed[regret_best_route[index]].primo_pass) {
			//c'? solo il depot
			//cout << "devo mettere il passeggero dentro" << endl;
			routes_destroyed[regret_best_route[index]].primo_pass = true;
			routes_destroyed[regret_best_route[index]].add_update_only_one_node_first_passanger(input, passengers_removed[regret_index_pass[index]]);

		}
		else {

			if (regret_move_c[index] == 1) {
				//i have to do the move C
				routes_destroyed[regret_best_route[index]].move_c(input, passengers_removed[regret_index_pass[index]], passengers_removed[regret_index_pass[index]].origin, passengers_removed[regret_index_pass[index]].destination);
			}
			else {
				routes_destroyed[regret_best_route[index]].update_route_rebuilt_one(input, regret_arc_from[index], regret_arc_to[index], passengers_removed[regret_index_pass[index]].origin,  passengers_removed[regret_index_pass[index]].destination, passengers_removed[regret_index_pass[index]]);
			}
		}

		routes_destroyed[regret_best_route[index]].add_passenger(passengers_removed[regret_index_pass[index]]);
		
		//QUA DEVO TOGLIERE IL PASSEGGERO CHE HO APPENA INSERITO DAL POOL DI PASSENGERS_REMOVED
		vector<Passenger>::iterator it;
		it = passengers_removed.begin();
		passengers_removed.erase(it + regret_index_pass[index]);

		regret_diff.clear();
		regret_move_c.clear(); //1 move C, 0 no move C
		regret_best_route.clear();
		regret_arc_from.clear();
		regret_arc_to.clear();
		regret_index_pass.clear();
		map_pass_aggragati.clear();
	}

	return routes_destroyed;
}

vector <Route> repair_forbidden(ProcessedInput* input, const PenaltyWeights& penalty_weights, double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	map<unsigned int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (r != p.route_before) {
				if (routes_destroyed[r].primo_pass == false) {
					if (routes_destroyed[r].index != 1)
						continue;
					//c'? solo il deposito in questo rebuilt_case
					double cost = from_to[routes_destroyed[r].get_airstrips()[0]][p.origin] + from_to[p.origin][p.destination] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].get_airstrips()[0]][p.origin];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.origin][p.destination];

					if (cost >= best_cost)
						continue; 
					
					best_cost = cost;
					best_route = r;
					case_first_passenger = false;
				}
				else {
					double cost_route_before = cost_single_route(input, penalty_weights, routes_destroyed[r]);
					for (unsigned int n = 0; n < routes_destroyed[r].index - 1; n++) {
						if (p.origin != routes_destroyed[r].get_airstrips()[n] || n == 0) {
							for (unsigned int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
								if (p.destination == routes_destroyed[r].get_airstrips()[n1])
									continue; 
								
								Route r_support = routes_destroyed[r];
								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_rebuilt_one_first_fase(input, node_add_from, node_add_to, p.origin, p.destination, p, non_to, non_to_final, num_equals);

								if (r_support.get_arrivals()[r_support.index - 1] > end_day)
									continue; 
								   
								r_support.update_rebuilt_one_second_fase(input, node_add_from, node_add_to, p.destination, p, non_to, non_to_final, num_equals);

								if ((p.solution_to - p.solution_from > p.stop))
									continue; 
										
								if (!route_feasible(input, r_support, end_day))
									continue; 
										
								double cost = (cost_single_route(input, penalty_weights, r_support) + 
												cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + 
												(peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
												
								if (best_cost <= cost)
									continue;

								best_route = r;
								best_cost = cost;
								move_c = false;

								route_best = r_support;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
							}
						}
					}
				}

				//for move C
				if (routes_destroyed[r].primo_pass) {
					//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
					if (routes_destroyed[r].get_airstrips()[routes_destroyed[r].index - 1] == p.origin) {
						double dist = from_to[p.origin][p.destination];
						double time = map_airplane[routes_destroyed[r].aircraft_code].travelTime(from_to[p.origin][p.destination]);
						double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.origin][p.destination];

						double time_window_cost = 0.0;
						double t_arr_departure = routes_destroyed[r].get_arrivals()[routes_destroyed[r].index - 1];
						double t_arr_arrival = routes_destroyed[r].get_departures()[routes_destroyed[r].index - 1] + time;
						if (t_arr_departure < p.early_departure) 
							time_window_cost += p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) 
							time_window_cost += t_arr_departure - p.late_departure;

						if (t_arr_arrival < p.early_arrival) 
							time_window_cost += p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) 
							time_window_cost += t_arr_arrival - p.late_arrival;

						double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
						if (best_cost > cost) {
							if (routes_destroyed[r].get_departures()[routes_destroyed[r].index - 1] + time <= end_day) {
								if (routes_destroyed[r].fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.destination])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
								}
							}
						}
					}
					else {
						// the departure is not equals to the last place of the route
						double cost = from_to[routes_destroyed[r].get_airstrips()[routes_destroyed[r].index - 1]][p.origin] + from_to[p.origin][p.destination];
						double TW_departure = 0.0;
						double t_arr_departure = routes_destroyed[r].get_departures()[routes_destroyed[r].index - 1] + map_airplane[routes_destroyed[r].aircraft_code].travelTime(from_to[routes_destroyed[r].get_airstrips()[routes_destroyed[r].index - 1]][p.origin]);
						if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[p.origin].ground_time + map_airplane[routes_destroyed[r].aircraft_code].travelTime(from_to[p.origin][p.destination]);
						if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
						cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[routes_destroyed[r].get_airstrips()[routes_destroyed[r].index - 1] + ";" + p.origin]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].get_airstrips()[routes_destroyed[r].index - 1]][p.origin];
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.origin][p.destination];
						cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

						//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il rebuilt_case in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = routes_destroyed[r].fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.origin].fuel) {
							f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = routes_destroyed[r].fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.destination])) {
										best_cost = cost;
										best_route = r;
										move_c = true;
									}
								}
							}
						}
					}
				}
			}
		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
		else {
			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'? solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {

				if (move_c) {
					//i have to do the move C
					routes_destroyed[best_route].move_c(input, p, p.origin, p.destination);
					//cout << "dopo la mossa C" << endl;
				}
				else {
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}

			routes_destroyed[best_route].add_passenger(p);
		}
	}

	return routes_infeasible ? vector<Route>() : routes_destroyed;
}

#endif