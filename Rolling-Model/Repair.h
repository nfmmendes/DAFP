//ECCO I NOSTRI REPAIR
#include <random>
#include <vector>



#include "Constructive.h"
#include "Feasibility.h"
#include "Model_Cplex.h"
#include "Route.h"

vector <Route> repair_one(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//cout << endl;
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			//cout << "Route: " << r << endl;
			//routes_destroyed[r].print();
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'� solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
					//cout << "costo caso primo passeggero: " << cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				//int arc_from = -1;
				//int arc_to = -1;
				double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
						for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes_destroyed[r].places[n1]) {
								//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
								//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);
								//cout << "route per il check" << endl;
								//r_for_check.print();
								/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
									cout << "caso strano: " << endl;
									r_support.print();
									cin.get();
								}*/


								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//r_support.print();
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//cout << "ristampo i valori" << endl;
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//cout << endl;
									//stampo_caso_strano_single_all(r_support, from_to, map_airplane);
									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
											//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
											if (best_cost > cost) {
												//best_arc_from = n;
												//best_arc_to = n1;
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
			if (routes_destroyed[r].primo_pass) {
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
					double dist = from_to[p.departure_location][p.arrival_location];
					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
					//cout << "tempo di volo: " << time << endl;
					double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					/*if (time <= 60) {
						fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

					double time_window_cost = 0.0;
					double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
					double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
					if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

					if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

					double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
					//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
					//cout << "cost for move c" << cost << endl;
					//cout << t_arr_arrival << " : tempo arrivo" << endl;
					if (best_cost > cost) {
						if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
							if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
								//best_arc_from = -2;
								//best_arc_to = -2;
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
					//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
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

					//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
					//cout << "cost for move c" << cost << endl;
					//cout << t_arr_arrival << " : tempo arrivo" << endl;
					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
								}
							}
						}
					}
				}
			}

		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "caso primo passeggero" << endl;
					//routes_destroyed[best_route].print();
				}
				//else {
					//ci sono pi� nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "caso mossa C" << endl;
					//routes_destroyed[best_route].print();

				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;
					//cout << "route prima dell'inserimento" << endl;
					//routes_destroyed[best_route].print();
					//cout << "caso inserimento speciale ------>" << best_arc_from << " - " << best_arc_to << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//routes_destroyed[best_route].print();
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector<Route> two_regret_repair_aggragati(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//bool routes_infeasible = false;
	vector<double> regret_diff;
	vector<int> regret_move_c; //1 move C, 0 no move C
	vector<int> regret_best_route;
	// if -1 = infeasible, passenger can insert in no-route
	// if 0...route.size() normal inserition with the function
	vector<int> regret_arc_from;
	vector<int> regret_arc_to;
	vector<int> regret_index_pass;

	map<int, int> map_pass_aggragati; //nella stringa, che � la chiave, ci sono from/to/timedep/timearr, nell'int c'� l'indicie di tutti i valori



	//cout << "ecco il numero di passeggeri rimossi " << passengers_removed.size();
	while (!passengers_removed.empty()) {
		int index_p = 0;
		for (Passenger& p : passengers_removed) {
			//p.print();

			vector<double> costs;
			vector<int> route;
			vector<int> arc_from;
			vector<int> arc_to;
			vector<int> move_c;


			//string chiave = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
			int chiave = p.chiave_regret;
			if (map_pass_aggragati.count(chiave) >= 1) {
				//cout << "ho gia calcolato un passeggero simile a lui" << endl;

				regret_diff.push_back(regret_diff[map_pass_aggragati[chiave]]);
				regret_move_c.push_back(regret_move_c[map_pass_aggragati[chiave]]);
				regret_best_route.push_back(regret_best_route[map_pass_aggragati[chiave]]);
				regret_arc_from.push_back(regret_arc_from[map_pass_aggragati[chiave]]);
				regret_arc_to.push_back(regret_arc_to[map_pass_aggragati[chiave]]);
				regret_index_pass.push_back(regret_index_pass[map_pass_aggragati[chiave]]);


			}
			else {


				//Route r_support; //attento qua, � quello di prova
				for (int r = 0; r < (int)routes_destroyed.size(); r++) {
					//cout << "nuova route-----------> " << r << endl;
					//routes_destroyed[r].print();
					if (routes_destroyed[r].primo_pass == false) {
						if (routes_destroyed[r].index == 1) {
							//c'� solo il deposito in questo caso
							double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
							cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
							cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

							//cout << "costo caso primo passeggero: " << cost << endl;

							//cout << "cost: " << cost << " from: " << -1 << " to: " << -1 << " route: " << r << " mossa c: " << 0 << endl;
							costs.push_back(cost);
							route.push_back(r);
							arc_from.push_back(-1);
							arc_to.push_back(-1);
							move_c.push_back(0);
						}
					}
					else {
						//int arc_from1 = -1;
						//int arc_to1 = -1;
						double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						//cout << "inizio for" << endl;
						for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
								for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										Route r_support = routes_destroyed[r];
										//r_support = routes_destroyed[r];
										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
										//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);


										if (r_support.time_arr[r_support.index - 1] <= end_day) {


											r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
													//cout << "prima che calcolo il cost" << endl;
													double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << "dopo che calcolo il cost" << endl;
													if (costs.size() == 0) {
														//cout << "cost: " << cost << " from: " << n << " to: " << n1 << " route: " << r << " mossa c: " << 0 << endl;
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
															//cout << "cost: " << cost << " from: " << n << " to: " << n1 << " route: " << r << " mossa c: " << 0 << endl;
														}
													}
													//cout << "route " << r << "archi: " << arc_from1 << " _ " << n1 << endl;
												}
											}
										}
									}
								}
							}
						}
						//cout << "fine for" << endl;
					}

					//for move C
					if (routes_destroyed[r].primo_pass) {
						//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
						if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
							//cout << "sto considerando la mossa C" << endl;
							double dist = from_to[p.departure_location][p.arrival_location];
							double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
							double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];


							double time_window_cost = 0.0;
							//double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
							//double t_arr_arrival = (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time);
							if (routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] < p.early_departure) time_window_cost += p.early_departure - routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
							else if (routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] > p.late_departure) time_window_cost += routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] - p.late_departure;

							if ((routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) < p.early_arrival) time_window_cost += p.early_arrival - (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time);
							else if ((routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) > p.late_arrival) time_window_cost += (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) - p.late_arrival;

							double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
							//cout << "cost for move c" << cost << endl;
							//cout << routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time << " <= " << end_day << " && " << routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed << " >= " << map_airplane[routes_destroyed[r].aircraft_code].min_fuel << endl;
							if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
								if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									costs.push_back(cost);
									route.push_back(r);
									move_c.push_back(1);
									arc_from.push_back(-2);
									arc_to.push_back(-2);

									//cout << "cost: " << cost << " from: " << -2 << " to: " << -2 << " route: " << r << " mossa c: " << 1 << endl;
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
							//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
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

							//cout << "cost for move c" << cost << endl;
							//cout << t_arr_arrival << " <= " << end_day << " && " << f_after_primo_tratto << " >= " << map_airplane[routes_destroyed[r].aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << map_airplane[routes_destroyed[r].aircraft_code].min_fuel << endl;
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
										costs.push_back(cost);
										move_c.push_back(1);
										arc_from.push_back(-2);
										arc_to.push_back(-2);
										route.push_back(r);

										//cout << "cost: " << cost << " from: " << -2 << " to: " << -2 << " route: " << r << " mossa c: " << 1 << endl;
									}
								}
							}
						}
					}
				}



				if (costs.size() == 1) {

					//cout << "il passegero:";
					//p.print();
					//cout << " ha solo una mossa di inserimento possibile" << endl;


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
					int index_mosse = distance(costs.begin(), index_iterator);
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
					//caso in cui non ha trovato mosse per quella persona
					//in questo caso vuol dire che la solutione � infeasible
					cout << "LA SOLUZIONE E' INFEASIBLE" << endl;
					vector<Route> route_vuote;
					return route_vuote;

				}

				//cout << "regret.size() - 1 " << (regret_diff.size() - 1) << endl;
				int val = (int)regret_diff.size() - 1;
				//cout << val << endl;
				//string primo_val = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
				int primo_val = p.chiave_regret;
				map_pass_aggragati.insert(make_pair(primo_val, val));
				//cout << map_pass_aggragati[primo_val] << endl;


				costs.clear();
				route.clear();
				arc_from.clear();
				arc_to.clear();
				move_c.clear();

			}

			index_p++;
		}

		//cout << "regret.size(): " << regret_best_route.size() << endl;




		int index = 0;
		double best = DBL_MIN; //prima era -10000
		for (int i = 0; i < (int)regret_diff.size(); i++) {
			//cout << "***********" << regret_diff[i] << endl;

			if (best < regret_diff[i]) {
				index = i;
				best = regret_diff[i];
			}
		}



		if (!routes_destroyed[regret_best_route[index]].primo_pass) {
			//c'� solo il depot
			//cout << "devo mettere il passeggero dentro" << endl;
			routes_destroyed[regret_best_route[index]].primo_pass = true;
			routes_destroyed[regret_best_route[index]].add_update_only_one_node_first_passanger(passengers_removed[regret_index_pass[index]], from_to, map_airplane, map_airstrip, from_to_FuelConsumed);

		}
		else {

			if (regret_move_c[index] == 1) {
				//i have to do the move C
				//cout << "devo mettere il passeggero dentro" << endl;
				routes_destroyed[regret_best_route[index]].move_c(passengers_removed[regret_index_pass[index]], passengers_removed[regret_index_pass[index]].departure_location, passengers_removed[regret_index_pass[index]].arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			else {
				//cout << "devo mettere il passeggero dentro" << endl;
				routes_destroyed[regret_best_route[index]].update_route_rebuilt_one(regret_arc_from[index], regret_arc_to[index], passengers_removed[regret_index_pass[index]].departure_location, passengers_removed[regret_index_pass[index]].arrival_location, from_to, map_airplane, map_airstrip, passengers_removed[regret_index_pass[index]], from_to_FuelConsumed);
				//cout << "ho messo il passeggero dentro" << endl;
			}
		}


		routes_destroyed[regret_best_route[index]].passengers_in_route.push_back(passengers_removed[regret_index_pass[index]]);


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


		//cout << "size dei passeggeri rimossi " << passengers_removed.size() << endl;
	}


	return routes_destroyed;

}

vector <Route> repair_forbidden(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (r != p.route_before) {
				if (routes_destroyed[r].primo_pass == false) {
					if (routes_destroyed[r].index == 1) {
						//c'� solo il deposito in questo caso
						double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
						cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
						cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

						//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
						//cout << "costo caso primo passeggero: " << cost << endl;
						//cout << r << " != " << p.route_before << endl;
						if (cost < best_cost) {
							best_cost = cost;
							best_route = r;
							case_first_passenger = false;
						}
					}
				}
				else {
					//int arc_from = -1;
					//int arc_to = -1;
					double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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

									r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);
									/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
										cout << "caso strano: " << endl;
										r_support.print();
										cin.get();
									}*/

									if (r_support.time_arr[r_support.index - 1] <= end_day) {
										//p.print();
										//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
										//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
										//r_support.print();
										r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
										//cout << "ristampo i valori" << endl;
										//p.print();
										//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
										//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
										//cout << endl;
										//stampo_caso_strano_single_all(r_support, from_to, map_airplane);

										if ((p.solution_to - p.solution_from <= p.stop)) {
											if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
												double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
												if (best_cost > cost) {
													//best_arc_from = n;
													//best_arc_to = n1;
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
				if (routes_destroyed[r].primo_pass) {
					//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
					if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
						double dist = from_to[p.departure_location][p.arrival_location];
						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
						//cout << "tempo di volo: " << time << endl;
						double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
						/*if (time <= 60) {
							fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
						}*/

						double time_window_cost = 0.0;
						double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
						double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
						if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

						if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

						double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
						//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
						//cout << "cost for move c" << cost << endl;
						//cout << t_arr_arrival << " : tempo arrivo" << endl;
						//cout << r << " != " << p.route_before << endl;
						if (best_cost > cost) {
							if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
								if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
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
						//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
						/*if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
						}*/
						//****
						//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
						cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
						/*if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
						}*/

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

						//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
						//cout << "cost for move c" << cost << endl;
						//cout << t_arr_arrival << " : tempo arrivo" << endl;
						//cout << r << " != " << p.route_before << endl;
						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
										best_cost = cost;
										best_route = r;
										move_c = true;
										//best_arc_from = -2;
										//best_arc_to = -2;
									}
								}
							}
						}
					}
				}
			}
		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				//else {
					//ci sono pi� nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "dopo la mossa C" << endl;
				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//cout << "dopo dell'inserimento speciale" << endl;
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector <Route> repair_perturbation(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'� solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
					//cout << "costo caso primo passeggero: " << cost << endl;

					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************

					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				//int arc_from = -1;
				//int arc_to = -1;
				double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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

								r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
								//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);

								/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
									cout << "caso strano: " << endl;
									r_support.print();
									cin.get();
								}*/

								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//r_support.print();
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//cout << "ristampo i valori" << endl;
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//cout << endl;
									//stampo_caso_strano_single_all(r_support, from_to, map_airplane);

									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;

											//code for casual number [0.8;1.2]*****************************************************
											double f = (double)rand() / RAND_MAX;
											f = 0.8 + (f * 0.4);
											cost = cost * f;
											//*************************************************************************************

											if (best_cost > cost) {
												//best_arc_from = n;
												//best_arc_to = n1;
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
			if (routes_destroyed[r].primo_pass) {
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
					double dist = from_to[p.departure_location][p.arrival_location];
					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
					//cout << "tempo di volo: " << time << endl;
					double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					/*if (time <= 60) {
						fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

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
							if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
								//best_arc_from = -2;
								//best_arc_to = -2;
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
					//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
					/*if (travel_primo_tratto <= 1) {
						fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_primo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/
					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
					/*if (travel_secondo_tratto <= 1) {
						fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

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


					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************


					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
								}
							}
						}
					}
				}
			}

		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				//else {
					//ci sono pi� nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "dopo la mossa C" << endl;
				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//cout << "dopo dell'inserimento speciale" << endl;
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}





//in questi ancora non messi

vector<Route> repairSP(double peso_TW, double peso_intermediate_stop, vector<Route>& route_destroy, vector<Passenger>& passenger_removed, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<Airplane>& airplanes, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<vector<Route>> solutionAllSub;
	int NAirplane = 20;
	/* cout << " Stampo Le route distrutte: " << endl;
	int Npass = 0;
	for (Route r : route_destroy) {
		r.print();
		Npass += r.passengers_in_route.size();
	}
	cout << " Dopo la distruzione ho " << Npass << endl;
	*/
	double dividendo = (double)(number_of_aircraft * passenger_removed.size());
	double denominatore = (double)(passengers.size());
	int number_of_aircraft_subset = 10 + (int)(round(dividendo / denominatore));
	if (number_of_aircraft_subset > number_of_aircraft)  number_of_aircraft_subset = number_of_aircraft;
	int iteration_subset = 0;
	int iteration_max_subset = (int)(passenger_removed.size()) / 3;
	//cout << " Numero di Itearazioni per questa distruzione: " << iteration_max_subset << endl;
	int Best_iteration = 0;
	double Best_cost = std::numeric_limits<double>::max();
	do {
		//cout << " ------------------ Sono al Itarzione del sottoinsime numero = " << iteration << " Di: " << iteration_max << " ---------------- " << endl;
		//cout << " ------------------ Sono al Itarzione del Sort numero = " << iteration_subset << " Di: " << iteration_max_subset << " ---------------- " << endl;
		// Lo ordino in modo random
		auto rng2 = default_random_engine{};
		shuffle(passenger_removed.begin(), passenger_removed.end(), rng2);
		vector<Route> solution;
		vector<Route> Route_to_fix = route_destroy;
		//cout << " Route da dare al costruittivo = "<< Route_to_fix.size() << endl;
		heuristic_costructive_first_fase_secIter(peso_TW, peso_intermediate_stop, Route_to_fix, airplanes, map_airplane, map_airstrip, end_day, passenger_removed, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
		double cost_fix_single = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, Route_to_fix, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		//cout << "The cost of the objective function first fase is: " << cost_objectiveFunction_fisrt_fase << endl;
		//cout << "***********************SOLUTION SECOND FASE***********************" << endl;
		solution = heuristic_costructive_second_fase_SP(Route_to_fix, end_day, peso_TW);
		cost_fix_single = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		if (cost_fix_single < Best_cost) {
			Best_cost = cost_fix_single;
			Best_iteration = iteration_subset;
		}
		//cout << "The cost of the objective function second fase is: " << cost_objectiveFunction_second_fase << endl;
		//cout << " Passo al modello una soluzione di route numero " << solution.size();

		solutionAllSub.push_back(solution);
		iteration_subset++;
	} while (iteration_subset < iteration_max_subset);
	//cout << " CHECK -----> Passo al modello un numero di soluzioni ---> " << solutionAllSub.size() << endl;
	//cout << " CHECK -----> Passo al modello un numero di passeggieri ---> " << passengers.size() << endl;
	//cout << " CHECK -----> La migliore iterazione e la numero ---> " << Best_iteration << endl;
	//cout << " CHECK -----> Costo migliore iterazione  ---> " << Best_cost << endl;
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






	vector<vector<vector<int>>> A3;  //matrix A
	vector<vector<double>> C;  //cost
	vector<Route> routes;
	for (auto& airplane : airplanes_model) {
		//cout << "Airplane: " << airplane.code << endl;
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
	//cout << "..................Finish to build all the structure for the Model.................." << endl;
	//cout << " Al modello sto passando un numero di Route generate: " << routes.size() << endl;
	//Creating model with gurobi
	//Create new Model object
	//Mi preparo a ricere le route scelte dal modello
	vector<Route> solution_model;
	Model_Cplex* model = new Model_Cplex(A3, C, airplanes_model, airplane_routes, routes, (int)(passengers.size()), passengers, solution_model, map_airplane);
	//Create and solve model with gurobi
	model->create_Model_for_SP_cplex(NAirplane);
	// adesso vettore delle Route non � piu vuoto
	// Non ho piu tutti gli aerei disponibili cout<<endl
	//cout << " Ho finito con il modello ---- Copio le Ruoute --- che sono: " << model->solution_model.size() << endl;
	int n = 0;
	for (Route& r : model->solution_model) n += (int)r.passengers_in_route.size();
	//cout << " Numero passeggieri della soluzione del modello: " << n << endl;
	if ((int)(model->solution_model.size()) > 0 && (n == (int)passengers.size())) {
		for (Route& s : model->solution_model) solution_model.push_back(s);
		//for (auto x : contPass) cout << x.first << " --- " << x.second << " In realta = " << (x.second / ContNomi[x.first]) << endl;
		return solution_model;
	}
	else {
		//cout << " Modello mi ha dato Infeassible stampo soluzione con costo ----> " << calculate_ObjectiveFunction(solutionAllSub[Best_iteration], map_airstrip, map_airplane, from_to,from_to_FuelConsumed) << endl;
		//for (auto x : contPass) cout << x.first << " --- " << x.second << " In realta = " << (x.second / ContNomi[x.first]) << endl;
		n = 0;
		for (Route& r : solutionAllSub[Best_iteration]) n += (int)r.passengers_in_route.size();
		//cout << " Numero di passeggieri della best solution " << n << endl;
		//cout << " Mentre dovrebbero essere: " << passengers.size() << endl;
		if (n == (int)passengers.size())  return solutionAllSub[Best_iteration];
		else {
			std::vector<Route> vuoto(0);
			//cout << " Ritorno Vettore vuoto " << endl;
			return vuoto;
		}
	}

	delete model;
}

