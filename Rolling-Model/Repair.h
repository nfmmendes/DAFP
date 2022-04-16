#ifndef REPAIR_H
#define REPAIR_H

//ECCO I NOSTRI REPAIR
#include <random>
#include <vector>



#include "Constructive.h"
#include "Feasibility.h"
#include "Model_Cplex.h"
#include "Route.h"
#include "Utils.h"


struct RepairParamSet
{
	double peso_TW;
	double inter_stop_weight;
	double end_day; 
};

void try_move_c0(ProcessedInput* input, double peso_TW, double end_day, vector<Route>& routes, bool& move_c, Passenger& p, int& best_route, double& best_cost, int r)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	if (routes[r].primo_pass) {
		//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
		if (routes[r].places[routes[r].index - 1] == p.departure_location) {
			double dist = from_to[p.departure_location][p.arrival_location];
			double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes[r].aircraft_code].speed) * 60;
			//cout << "tempo di volo: " << time << endl;
			double fuel_consumed = from_to_FuelConsumed[routes[r].aircraft_code][p.departure_location][p.arrival_location];

			double time_window_cost = 0.0;
			double t_arr_departure = routes[r].time_arr[routes[r].index - 1];
			double t_arr_arrival = routes[r].time_dep[routes[r].index - 1] + time;
			if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

			if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

			double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;

			if (best_cost > cost) {
				if (routes[r].time_dep[routes[r].index - 1] + time <= end_day) {
					if (routes[r].quantity_fuel[routes[r].index - 1] - fuel_consumed >= (map_airplane[routes[r].aircraft_code].min_fuel + location_fuel[routes[r].aircraft_code][p.arrival_location])) {
						best_cost = cost;
						best_route = r;
						move_c = true;
					}
				}
			}
		}
		else {
			// the departure is not equals to the last place of the route
			double cost = from_to[routes[r].places[routes[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
			double TW_departure = 0.0;
			double t_arr_departure = routes[r].time_dep[routes[r].index - 1] + (((from_to[routes[r].places[routes[r].index - 1]][p.departure_location]) / map_airplane[routes[r].aircraft_code].speed) * 60);
			if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

			double TW_arrival = 0.0;
			double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes[r].aircraft_code].speed) * 60);
			if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

			//per il check sul fuel:
			double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes[r].aircraft_code][routes[r].places[routes[r].index - 1]][p.departure_location];
			double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes[r].aircraft_code][p.departure_location][p.arrival_location];

			cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

			//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
			//refuel
			double f_after_primo_tratto = routes[r].quantity_fuel[routes[r].index - 1] - fuel_consumed_primo_tratto;
			double f_after_secondo_tratto = 0.0;
			if (map_airstrip[p.departure_location].fuel) {
				f_after_secondo_tratto = map_airplane[routes[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
			}
			else {
				f_after_secondo_tratto = routes[r].quantity_fuel[routes[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
			}

			if (best_cost > cost) {
				if (t_arr_arrival <= end_day) {
					if (f_after_primo_tratto >= map_airplane[routes[r].aircraft_code].min_fuel) {
						if (f_after_secondo_tratto >= (map_airplane[routes[r].aircraft_code].min_fuel + location_fuel[routes[r].aircraft_code][p.arrival_location])) {
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

vector <Route> repair_one(ProcessedInput* input, double peso_TW, double inter_stop_weight, double end_day, vector<Route>& routes, vector <Passenger>& passengers) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers) {
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;

		for (int r = 0; r < (int)routes.size(); r++) {
			auto* route_ptr = &routes[r];
			if (route_ptr->primo_pass == false) {
				if (route_ptr->index == 1) {
					//there is only the depot in this case
					double cost = from_to[route_ptr->places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[route_ptr->aircraft_code].fixed_cost;
					cost + from_to_FuelConsumed[route_ptr->aircraft_code][routes[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[route_ptr->aircraft_code][p.departure_location][p.arrival_location];

					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				double cost_route_before = cost_single_route(input, peso_TW, inter_stop_weight, routes[r]);
				for (int n = 0; n < route_ptr->index - 1; n++) {
					if (p.departure_location != route_ptr->places[n] || n == 0) {
						for (int n1 = n; (n1 < routes[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes[r].places[n1]) {
								Route r_support = routes[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									r_support.update_route_rebuilt_one_second_fase(input,  caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(input, peso_TW, inter_stop_weight, r_support) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (inter_stop_weight * (p.solution_to - p.solution_from - 1)) - cost_route_before;

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
			try_move_c0(input, peso_TW, end_day, routes, move_c, p, best_route, best_cost, r);
		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
		else {
			if (!routes[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//there is only the depot
					routes[best_route].primo_pass = true;
					routes[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {

				if (move_c) {
					//i have to do the move C
					routes[best_route].move_c(input, p, p.departure_location, p.arrival_location);
				}
				else {
					routes[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}

			routes[best_route].passengers_in_route.push_back(p);
		}
	}

	return routes_infeasible ? vector<Route>() : routes;
}

struct RoutesDataStructure
{
	vector<double> costs;
	vector<int> route;
	vector<int> arc_from;
	vector<int> arc_to;
	vector<int> move_c;
};

struct RegretDataStructure
{
	vector<double> regret_diff;
	vector<int> regret_move_c; //1 move C, 0 no move C
	vector<int> regret_best_route;
	// if -1 = infeasible, passenger can insert in no-route
	// if 0...route.size() normal inserition with the function
	vector<int> regret_arc_from;
	vector<int> regret_arc_to;
	vector<int> regret_index_pass;
};

namespace two_regret_repair_aggragati_namespace
{
	void standard_initialize_regret_data(RegretDataStructure& regretData, const int index)
	{
		regretData.regret_diff.push_back(regretData.regret_diff[index]);
		regretData.regret_move_c.push_back(regretData.regret_move_c[index]);
		regretData.regret_best_route.push_back(regretData.regret_best_route[index]);
		regretData.regret_arc_from.push_back(regretData.regret_arc_from[index]);
		regretData.regret_arc_to.push_back(regretData.regret_arc_to[index]);
		regretData.regret_index_pass.push_back(regretData.regret_index_pass[index]);
	}

	void update_aggregate_data(RoutesDataStructure& aggregateData, int r, int n, int n1, double cost)
	{
		//TODO: Check this
		if (aggregateData.costs.empty()) {
			aggregateData.costs.push_back(cost);
			aggregateData.arc_from.push_back(n);
			aggregateData.arc_to.push_back(n1);
			aggregateData.route.push_back(r);
			aggregateData.move_c.push_back(0);
		}

		if (aggregateData.costs.size() >= 1) {
			if (cost != aggregateData.costs[aggregateData.costs.size() - 1]) {
				aggregateData.costs.push_back(cost);
				aggregateData.arc_from.push_back(n);
				aggregateData.arc_to.push_back(n1);
				aggregateData.route.push_back(r);
				aggregateData.move_c.push_back(0);
			}
		}
	}

	void update_regret_data(RegretDataStructure& regretData, int index_p, RoutesDataStructure aggregateData)
	{
		regretData.regret_diff.push_back(0);  //metto valore zero ma non dovrebbe succedere
		regretData.regret_best_route.push_back(aggregateData.route[0]);
		regretData.regret_move_c.push_back(aggregateData.move_c[0]);
		regretData.regret_arc_from.push_back(aggregateData.arc_from[0]);
		regretData.regret_arc_to.push_back(aggregateData.arc_to[0]);
		regretData.regret_index_pass.push_back(index_p);
	}

	void update_regret_data(RegretDataStructure& regretData, int index_p, RoutesDataStructure& aggregateData, int index_mosse, double best_cost, double best_cost_2)
	{
		regretData.regret_diff.push_back(best_cost_2 - best_cost);
		regretData.regret_best_route.push_back(aggregateData.route[index_mosse]);
		regretData.regret_move_c.push_back(aggregateData.move_c[index_mosse]);
		regretData.regret_arc_from.push_back(aggregateData.arc_from[index_mosse]);
		regretData.regret_arc_to.push_back(aggregateData.arc_to[index_mosse]);
		regretData.regret_index_pass.push_back(index_p);
	}

	void update_regret_data_2(RegretDataStructure& regretData, int index_p, RoutesDataStructure& aggregateData)
	{
		if (aggregateData.costs.size() == 1) {
			update_regret_data(regretData, index_p, aggregateData);
		}
		else if (aggregateData.costs.size() > 1) {
			//qua lo posso ottimizzare****************************************************************************************************************************************
			//con il codice di TED per ordinare, guarda bene come salvare l'indice********************************************************************************************
			vector<double>::iterator index_iterator = min_element(aggregateData.costs.begin(), aggregateData.costs.end());
			int index_mosse = distance(aggregateData.costs.begin(), index_iterator);
			double best_cost = aggregateData.costs[index_mosse];

			aggregateData.costs.erase(aggregateData.costs.begin() + index_mosse);
			vector<double>::iterator index_iterator2 = min_element(aggregateData.costs.begin(), aggregateData.costs.end());
			double best_cost_2 = aggregateData.costs[distance(aggregateData.costs.begin(), index_iterator2)];

			update_regret_data(regretData, index_p, aggregateData, index_mosse, best_cost, best_cost_2);
		}
	}

	void iterate_on_destroyed_routes(ProcessedInput* input, double peso_TW, double inter_stop_weight, double end_day, vector<Route>& routes, Passenger& p, RoutesDataStructure& aggregateData, int r)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		double2DVector location_fuel = input->get_location_fuel();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		double cost_route_before = cost_single_route(input, peso_TW, inter_stop_weight, routes[r]);

		//cout << "inizio for" << endl;
		for (int n = 0; n < routes[r].index - 1; n++) {
			if (p.departure_location != routes[r].places[n] || n == 0) {
				for (int n1 = n; (n1 < routes[r].index) && (n1 - n <= p.stop + 1); n1++) {
					if (p.arrival_location != routes[r].places[n1]) {
						Route r_support = routes[r];
						//r_support = routes[r];
						bool non_to = false;
						bool non_to_final = false;
						bool num_equals = false;
						int caso = -1;
						int node_add_from = n;
						int node_add_to = n1;

						r_support.update_route_rebuilt_one_first_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

						if (r_support.time_arr[r_support.index - 1] <= end_day) {

							r_support.update_route_rebuilt_one_second_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);
							if ((p.solution_to - p.solution_from <= p.stop)) {
								if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {

									double cost = (cost_single_route(input, peso_TW, inter_stop_weight, r_support) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) +
										(inter_stop_weight * (p.solution_to - p.solution_from - 1)) - cost_route_before;

									update_aggregate_data(aggregateData, r, n, n1, cost);
								}
							}
						}
					}
				}
			}
		}
	}

	void try_put_passenger(ProcessedInput* input, double peso_TW, double end_day, vector<Route>& routes, Passenger& p, RoutesDataStructure& aggregateData, int r)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
		double dist = from_to[p.departure_location][p.arrival_location];
		double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes[r].aircraft_code].speed) * 60;
		double fuel_consumed = from_to_FuelConsumed[routes[r].aircraft_code][p.departure_location][p.arrival_location];

		double time_window_cost = 0.0;
		if (routes[r].time_arr[routes[r].index - 1] < p.early_departure) time_window_cost += p.early_departure - routes[r].time_arr[routes[r].index - 1];
		else if (routes[r].time_arr[routes[r].index - 1] > p.late_departure) time_window_cost += routes[r].time_arr[routes[r].index - 1] - p.late_departure;

		if ((routes[r].time_dep[routes[r].index - 1] + time) < p.early_arrival) time_window_cost += p.early_arrival - (routes[r].time_dep[routes[r].index - 1] + time);
		else if ((routes[r].time_dep[routes[r].index - 1] + time) > p.late_arrival) time_window_cost += (routes[r].time_dep[routes[r].index - 1] + time) - p.late_arrival;

		double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;

		if (routes[r].time_dep[routes[r].index - 1] + time <= end_day) {
			if (routes[r].quantity_fuel[routes[r].index - 1] - fuel_consumed >= (map_airplane[routes[r].aircraft_code].min_fuel + location_fuel[routes[r].aircraft_code][p.arrival_location])) {
				aggregateData.costs.push_back(cost);
				aggregateData.route.push_back(r);
				aggregateData.move_c.push_back(1);
				aggregateData.arc_from.push_back(-2);
				aggregateData.arc_to.push_back(-2);
			}
		}
	}

	void try_move_c(ProcessedInput* input, double peso_TW, double& end_day, vector<Route>& routes, Passenger& p, RoutesDataStructure& aggregateData, int r)
	{
		map<int, Airplane> map_airplane = input->get_map_airplane();
		map<int, Airstrip> map_airstrip = input->get_map_airstrip();
		double2DVector location_fuel = input->get_location_fuel();
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

		//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
		if (routes[r].places[routes[r].index - 1] == p.departure_location) {
			try_put_passenger(input, peso_TW, end_day, routes, p, aggregateData, r);
		}
		else {
			// the departure is not equals to the last place of the route
			double cost = from_to[routes[r].places[routes[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
			double TW_departure = 0.0;
			double time_to_go = (((from_to[routes[r].places[routes[r].index - 1]][p.departure_location]) / map_airplane[routes[r].aircraft_code].speed) * 60);
			double t_arr_departure = routes[r].time_dep[routes[r].index - 1] + time_to_go;
			if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

			double TW_arrival = 0.0;
			time_to_go = (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes[r].aircraft_code].speed) * 60);
			double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + time_to_go;
			if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

			//per il check sul fuel:
			double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes[r].aircraft_code][routes[r].places[routes[r].index - 1]][p.departure_location];
			double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes[r].aircraft_code][p.departure_location][p.arrival_location];

			cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
			//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
			//refuel
			double f_after_primo_tratto = routes[r].quantity_fuel[routes[r].index - 1] - fuel_consumed_primo_tratto;
			double f_after_secondo_tratto = 0.0;
			if (map_airstrip[p.departure_location].fuel) {
				f_after_secondo_tratto = map_airplane[routes[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
			}
			else {
				f_after_secondo_tratto = routes[r].quantity_fuel[routes[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
			}

			if (t_arr_arrival <= end_day) {
				if (f_after_primo_tratto >= map_airplane[routes[r].aircraft_code].min_fuel) {
					if (f_after_secondo_tratto >= (map_airplane[routes[r].aircraft_code].min_fuel + location_fuel[routes[r].aircraft_code][p.arrival_location])) {
						aggregateData.costs.push_back(cost);
						aggregateData.move_c.push_back(1);
						aggregateData.arc_from.push_back(-2);
						aggregateData.arc_to.push_back(-2);
						aggregateData.route.push_back(r);
					}
				}
			}
		}
	}

	void try_only_depot_case(ProcessedInput* input, vector<Route>& routes, Passenger& p, RoutesDataStructure& aggregateData, int r)
	{
		map<int, Airplane> map_airplane;
		double2DVector from_to = input->get_from_to();
		double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

		double cost = from_to[routes[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes[r].aircraft_code].fixed_cost;
		cost += from_to_FuelConsumed[routes[r].aircraft_code][routes[r].places[0]][p.departure_location];
		cost += from_to_FuelConsumed[routes[r].aircraft_code][p.departure_location][p.arrival_location];

		aggregateData.costs.push_back(cost);
		aggregateData.route.push_back(r);
		aggregateData.arc_from.push_back(-1);
		aggregateData.arc_to.push_back(-1);
		aggregateData.move_c.push_back(0);
	}

	bool iterate_on_passengers(ProcessedInput* input, double peso_TW, double inter_stop_weight, double end_day, vector<Route>& routes, map<int, int>& map_passengers,
		RegretDataStructure& regretData, int& index_p, Passenger& p, vector<Route>& returned_vector)
	{
		int chiave = p.chiave_regret;
		if (map_passengers.count(chiave) >= 1) {
			const int index = map_passengers[chiave];
			standard_initialize_regret_data(regretData, index);
		}
		else {
			RoutesDataStructure aggregateData;
			//Route r_support; //attento qua, � quello di prova
			for (int r = 0; r < (int)routes.size(); r++) {
				if (routes[r].primo_pass == false) {
					if (routes[r].index == 1) {
						//c'� solo il deposito in questo caso
						try_only_depot_case(input, routes, p, aggregateData, r);
					}
				}
				else {
					iterate_on_destroyed_routes(input, peso_TW, inter_stop_weight, end_day, routes, p, aggregateData, r);
				}

				//for move C
				if (routes[r].primo_pass) {
					try_move_c(input, peso_TW, end_day, routes, p, aggregateData, r);
				}
			}

			update_regret_data_2(regretData, index_p, aggregateData);
			if (aggregateData.costs.size() == 0) {
				//caso in cui non ha trovato mosse per quella persona
				//in questo caso vuol dire che la solutione � infeasible
				cout << "LA SOLUZIONE E' INFEASIBLE" << endl;
				returned_vector = vector<Route>();
				return true;
			}

			int val = (int)regretData.regret_diff.size() - 1;
			int primo_val = p.chiave_regret;
			map_passengers.insert(make_pair(primo_val, val));
		}
		index_p++;
		return false;
	}	
}

inline vector<Route> two_regret_repair_aggragati(ProcessedInput* input, double peso_TW, double inter_stop_weight, double end_day, vector<Route>& routes, vector <Passenger>& passengers_removed) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	
	map<int, int> map_pass_aggragati; //nella stringa, che � la chiave, ci sono from/to/timedep/timearr, nell'int c'� l'indicie di tutti i valori

	while (!passengers_removed.empty()) {
		RegretDataStructure regretData;
		int index_p = 0;
		for (Passenger& p : passengers_removed) {
			vector<Route> returned_vector;
			if (two_regret_repair_aggragati_namespace::iterate_on_passengers(input, peso_TW, inter_stop_weight, end_day, routes, map_pass_aggragati, regretData, index_p, p, returned_vector)) 
				return returned_vector;
		}

		int index = 0;
		double best = DBL_MIN;
		for (int i = 0; i < (int)regretData.regret_diff.size(); i++) {
			if (best < regretData.regret_diff[i]) {
				index = i;
				best = regretData.regret_diff[i];
			}
		}

		if (!routes[regretData.regret_best_route[index]].primo_pass) {
			//there is only the depot
			routes[regretData.regret_best_route[index]].primo_pass = true;
			routes[regretData.regret_best_route[index]].add_update_only_one_node_first_passanger(input, passengers_removed[regretData.regret_index_pass[index]]);

		}
		else {

			if (regretData.regret_move_c[index] == 1) {
				//i have to do the move C
				routes[regretData.regret_best_route[index]].move_c(input, passengers_removed[regretData.regret_index_pass[index]], passengers_removed[regretData.regret_index_pass[index]].departure_location, passengers_removed[regretData.regret_index_pass[index]].arrival_location);
			}
			else {
				int index_passenger = regretData.regret_index_pass[index];
				routes[regretData.regret_best_route[index]].update_route_rebuilt_one(input, regretData.regret_arc_from[index], regretData.regret_arc_to[index], 
					passengers_removed[index_passenger].departure_location, passengers_removed[index_passenger].arrival_location, 
					passengers_removed[index_passenger]);
			}
		}

		routes[regretData.regret_best_route[index]].passengers_in_route.push_back(passengers_removed[regretData.regret_index_pass[index]]);

		//QUA DEVO TOGLIERE IL PASSEGGERO CHE HO APPENA INSERITO DAL POOL DI PASSENGERS_REMOVED
		vector<Passenger>::iterator it;
		it = passengers_removed.begin();
		passengers_removed.erase(it + regretData.regret_index_pass[index]);

		map_pass_aggragati.clear();
	}

	return routes;
}

void try_move_c(ProcessedInput* input, double peso_TW, const double end_day, vector<Route>& routes_destroyed, bool& move_c, Passenger& p, int& best_route, double& best_cost, int r)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	if (routes_destroyed[r].primo_pass) {
		auto* route_ptr = &routes_destroyed[r];
		//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
		if (route_ptr->places[route_ptr->index - 1] == p.departure_location) {
			double dist = from_to[p.departure_location][p.arrival_location];
			double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[route_ptr->aircraft_code].speed) * 60;

			double fuel_consumed = from_to_FuelConsumed[route_ptr->aircraft_code][p.departure_location][p.arrival_location];

			double time_window_cost = 0.0;
			double t_arr_departure = route_ptr->time_arr[route_ptr->index - 1];
			double t_arr_arrival = route_ptr->time_dep[route_ptr->index - 1] + time;
			if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

			if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

			double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
			if (best_cost > cost) {
				if (route_ptr->time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
					if (route_ptr->quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[route_ptr->aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
						best_cost = cost;
						best_route = r;
						move_c = true;
					}
				}
			}
		}
		else {
			// the departure is not equals to the last place of the route
			double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
			double TW_departure = 0.0;
			double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
			if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

			double TW_arrival = 0.0;
			double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
			if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

			//per il check sul fuel:
			double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
			double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
			cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

			//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
			//refuel
			double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
			double f_after_secondo_tratto = 0.0;
			if (map_airstrip[p.departure_location].fuel) {
				f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
			}
			else {
				f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
			}

			if (best_cost > cost) {
				if (t_arr_arrival <= end_day) {
					if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
						if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
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

vector <Route> repair_forbidden(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, const double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
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
					if (routes_destroyed[r].index == 1) {
						//c'� solo il deposito in questo caso
						double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
						cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
						cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

						if (cost < best_cost) {
							best_cost = cost;
							best_route = r;
							case_first_passenger = false;
						}
					}
				}
				else {

					double cost_route_before = cost_single_route(input, peso_TW, peso_intermediate_stop, routes_destroyed[r]);
					for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
						if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
							for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
								if (p.arrival_location != routes_destroyed[r].places[n1]) {
									Route r_support = routes_destroyed[r];
									bool non_to = false;
									bool non_to_final = false;
									bool num_equals = false;
									int caso = -1;
									int node_add_from = n;
									int node_add_to = n1;

									r_support.update_route_rebuilt_one_first_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

									if (r_support.time_arr[r_support.index - 1] <= end_day) {
										r_support.update_route_rebuilt_one_second_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

										if ((p.solution_to - p.solution_from <= p.stop)) {
											if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
												double cost = (cost_single_route(input, peso_TW, peso_intermediate_stop, r_support) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
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
				try_move_c(input, peso_TW, end_day, routes_destroyed, move_c, p, best_route, best_cost, r);
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
					//there is only the depot 
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {

				if (move_c) {
					//i have to do the move C
					routes_destroyed[best_route].move_c(input, p, p.departure_location, p.arrival_location);
				}
				else {
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}
			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}

	return routes_infeasible ? vector<Route>() : routes_destroyed;
}

void try_move_c_2(ProcessedInput* input, double peso_TW, double end_day, vector<Route>& routes_destroyed, bool& move_c, Passenger& p, int& best_route, double& best_cost, int r)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	if (routes_destroyed[r].primo_pass) {
		//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
		if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
			double dist = from_to[p.departure_location][p.arrival_location];
			double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
			double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

			double time_window_cost = 0.0;
			double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
			double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
			if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

			if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

			double cost = dist + (time_window_cost * peso_TW) + fuel_consumed;

			//code for casual number [0.8;1.2]*****************************************************
			double f = (double)rand() / RAND_MAX;
			f = 0.8 + (f * 0.4);
			cost = cost * f;
			//**************************************************************************************


			if (best_cost > cost) {
				if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
					double value1 = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed;
					double value2 = (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location]);
					if (value1 >= value2) {
						best_cost = cost;
						best_route = r;
						move_c = true;
					}
				}
			}
		}
		else {
			// the departure is not equals to the last place of the route
			double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
			double TW_departure = 0.0;
			double time_to_go = (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
			double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time_to_go;
			if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

			double TW_arrival = 0.0;
			time_to_go = (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
			double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + time_to_go;
			if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

			//per il check sul fuel:
			double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];

			double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
			cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

			//refuel
			double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
			double f_after_secondo_tratto = 0.0;
			if (map_airstrip[p.departure_location].fuel) {
				f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
			}
			else {
				f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
			}

			double f = (double)rand() / RAND_MAX;
			f = 0.8 + (f * 0.4);
			cost = cost * f;

			if (best_cost > cost) {
				if (t_arr_arrival <= end_day) {
					if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
						if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
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

vector <Route> repair_perturbation(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
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
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'� solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;

					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				double cost_route_before = cost_single_route(input, peso_TW, peso_intermediate_stop, routes_destroyed[r]);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
						for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes_destroyed[r].places[n1]) {
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									r_support.update_route_rebuilt_one_second_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(input, peso_TW, peso_intermediate_stop, r_support) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;

											//code for casual number [0.8;1.2]*****************************************************
											double f = (double)rand() / RAND_MAX;
											f = 0.8 + (f * 0.4);
											cost = cost * f;

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
			try_move_c_2(input, peso_TW, end_day, routes_destroyed, move_c, p, best_route, best_cost, r);
		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
		else {
			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {
				if (move_c) {
					routes_destroyed[best_route].move_c(input, p, p.departure_location, p.arrival_location);
				}
				else {
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}
			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	return routes_infeasible ? vector<Route>() : routes_destroyed;
}

vector<Route> test_model_solution(map<int, Airplane>& map_airplane, vector<Passenger>& passengers, vector<vector<Route>>& solutionAllSub, int Best_iteration, vector<Airplane>& airplanes_model, map<int, vector<Route>>& airplane_routes)
{
	const int NAirplane = 20;
	vector<vector<vector<int>>> A3;  //matrix A
	vector<vector<double>> C;  //cost
	vector<Route> routes;
	for (auto& airplane : airplanes_model) {
		vector<double> c;
		vector<vector<int>> A2;
		for (Route& r : airplane_routes[airplane.code]) {
			//r.print();
			routes.push_back(r);
			vector<int> A1;
			for (Passenger& p : passengers) {
				auto it = find_if(r.passengers_in_route.begin(), r.passengers_in_route.end(),
					[=](Passenger p_)
					{
						return p.pnr == p_.pnr;
					});
				if (it != r.passengers_in_route.end())
				{
					A1.push_back(1);
				}
				else {
					A1.push_back(0);
				}
			}
			A2.push_back(A1);
			//cout << "Costo: " << r.cost << endl;
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

	int n = 0;
	for (Route& r : model->solution_model) n += static_cast<int>(r.passengers_in_route.size());

	if (!model->solution_model.empty() && (n == static_cast<int>(passengers.size()))) {
		for (Route& s : model->solution_model) solution_model.push_back(s);

		delete model;
		return solution_model;
	}
	else {
		delete model;
		n = 0;
		for (Route& r : solutionAllSub[Best_iteration]) n += (int)r.passengers_in_route.size();

		return n == static_cast<int>(passengers.size()) ? solutionAllSub[Best_iteration] : std::vector<Route>();
	}
}

//in questi ancora non messi
vector<Route> repairSP(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, vector<Route>& route_destroy, vector<Passenger>& passenger_removed, vector<Airplane>& airplanes, double end_day, vector<Passenger>& passengers, int number_of_aircraft) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	vector<vector<Route>> solutionAllSub;

	double dividendo = static_cast<double>(number_of_aircraft * passenger_removed.size());
	double denominatore = static_cast<double>(passengers.size());
	int number_of_aircraft_subset = 10 + (int)(round(dividendo / denominatore));
	if (number_of_aircraft_subset > number_of_aircraft)  number_of_aircraft_subset = number_of_aircraft;
	int iteration_subset = 0;
	int iteration_max_subset = static_cast<int>(passenger_removed.size()) / 3;

	int Best_iteration = 0;
	double Best_cost = std::numeric_limits<double>::max();
	do {
		auto rng2 = default_random_engine{};
		shuffle(passenger_removed.begin(), passenger_removed.end(), rng2);
		vector<Route> solution;
		vector<Route> Route_to_fix = route_destroy;

		heuristic_costructive_first_fase_secIter(input, peso_TW, peso_intermediate_stop, Route_to_fix, airplanes, end_day, passenger_removed, number_of_aircraft);
		double cost_fix_single = calculate_ObjectiveFunction(input, peso_TW, peso_intermediate_stop, Route_to_fix);

		solution = heuristic_costructive_second_fase_SP(Route_to_fix, end_day, peso_TW);
		cost_fix_single = calculate_ObjectiveFunction(input,peso_TW, peso_intermediate_stop, solution);
		if (cost_fix_single < Best_cost) {
			Best_cost = cost_fix_single;
			Best_iteration = iteration_subset;
		}

		solutionAllSub.push_back(solution);
		iteration_subset++;
	} while (iteration_subset < iteration_max_subset);

	//Mi preparo per chiamare il modello con il Pool di route generato
	vector<Airplane> airplanes_model;
	for (auto& k : map_airplane) {
		airplanes_model.push_back(k.second);
	}
	map<int, vector<Route>> airplane_routes;
	for (Airplane& a : airplanes_model) {
		vector<Route> routes;
		airplane_routes.insert(make_pair(a.code, routes));
	}
	for (auto& solution : solutionAllSub) {
		for (Route& r : solution) {
			airplane_routes[r.aircraft_code].push_back(r);
		}
	}

	return test_model_solution(map_airplane, passengers, solutionAllSub, Best_iteration, airplanes_model, airplane_routes);
}

void try_move_c3(ProcessedInput* input, double peso_TW, double end_day, vector<Route>& routes_destroyed, bool& move_c, Passenger p, int& best_route, double& best_cost, int r)
{
	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	if (routes_destroyed[r].primo_pass) {
		//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
		if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
			double dist = from_to[p.departure_location][p.arrival_location];
			double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
			double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

			double time_window_cost = 0.0;
			double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
			double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
			if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

			if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

			double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;

			if (best_cost > cost) {
				if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
					if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
						best_cost = cost;
						best_route = r;
						move_c = true;
					}
				}
			}
		}
		else {
			// the departure is not equals to the last place of the route
			double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
			double TW_departure = 0.0;
			double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
			if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
			else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

			double TW_arrival = 0.0;
			double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
			if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
			else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

			//per il check sul fuel:
			double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
			double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

			cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

			double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
			double f_after_secondo_tratto = 0.0;
			if (map_airstrip[p.departure_location].fuel) {
				f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
			}
			else {
				f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
			}

			if (best_cost > cost) {
				if (t_arr_arrival <= end_day) {
					if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
						if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
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

vector <Route> repair_one_inter_move(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, vector <Passenger>& passengers_removed) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
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
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				double cost_route_before = cost_single_route(input, peso_TW, peso_intermediate_stop, routes_destroyed[r]);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
						for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes_destroyed[r].places[n1]) {
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									r_support.update_route_rebuilt_one_second_fase(input, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, p, non_to, non_to_final, num_equals);

									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(input, peso_TW, peso_intermediate_stop, r_support) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + 
															(peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
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
			try_move_c3(input, peso_TW, end_day, routes_destroyed, move_c, p, best_route, best_cost, r);
		}

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}
		else {

			if (!routes_destroyed[best_route].primo_pass) {
				if (case_first_passenger == false) {
					//there is only the depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(input, p);
				}
			}
			else {
				if (move_c) {
					routes_destroyed[best_route].move_c(input, p, p.departure_location, p.arrival_location);
				}
				else {
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//routes_destroyed[best_route].print();
				}
			}

			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	return routes_infeasible ? vector<Route>() : routes_destroyed;
}

#endif