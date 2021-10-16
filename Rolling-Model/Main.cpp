#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <random>
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include <float.h>
#include <chrono>



#include "Constructive.h"
#include "Destroyers.h"
#include "Evaluate.h"
#include "Fill.h"
#include "Move.h"
#include "Flightleg.h"
#include "Model_Connection.h"
#include "Model_Cplex.h"
#include "Repair.h"
#include "Utils.h"

using namespace std;

//const int numero_airstrip_const = 58;
//const int numero_airplane_const = 36;


vector <Route> repair_one_inter_move(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {
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
					/*
					if (travel_primo_tratto <= 1) {
						fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_primo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}
					*/
					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

					/*
					if (travel_secondo_tratto <= 1) {
						fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}
					*/

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
				//	routes_destroyed[best_route].primo_pass = true;
				//	routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
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

vector <Route> inter_move(double peso_TW, double peso_intermediate_stop, vector<Route> routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;
	int n_route = -1;
	//cout << " Stampo le route prima di cominciare " << endl;
	//for (Route r : routes) r.print();
	//cout << "************************************************** INCOMINCIO ********************************************************************************************* " << endl;
	//for (Route r : routes) {
	/*int t = 0;
	for (auto x : routes) t += x.passengers_in_route.size();
	cout << "ecco i passeggeri che arrivano in input dalle routes: " << t << endl;
	*/

	for (int r = 0; r < routes.size(); r++) {
		n_route += 1;
		vector<Route> routes_destroyed;



		//cout << " Adesso anallizzo la route numero " << r << " dell'aereo " << routes[r].aircraft_code << endl;
		//Route distrutte sono uguali tranne quella a cui sto togliendo la coppia
		//for (Route s : routes) if (s.aircraft_code != r.aircraft_code) routes_destroyed.push_back(s);
		for (const Route& s : routes) {
			//cout << "if: " << to_string(s.aircraft_code) << "  ----  " << to_string(routes[r].aircraft_code) << endl;
			if (s.aircraft_code != routes[r].aircraft_code) routes_destroyed.push_back(s);
		}
		Route r_support = routes[r];
		//cout << "r_support ha questi passeggeri: " << r_support.passengers_in_route.size() << endl;


		for (int A = 1; A < (r_support.index - 1); A++) {
			//cout << " Sto guardando Nodo A: " << A << " di " << r_support.aircraft_code << " size: " << r_support.index <<  endl;
			// Solution Rebuilt � come se fosse il nostro best improvement
			vector<Route> solution_rebuilt;
			vector<Passenger> passenger_removed;
			//cout << " Considero di spostare nodo -> " << A << " e nodo " << A+1 << endl;
			// Ora devo eliminare i nodi solo se sono allowed
			if (move_flightleg_is_allowed(A, r_support)) {
				//cout << " CONSENTITO sono nel main -----> provo a fare move di " << A << " e " << A+1 << " da " << r_support.aircraft_code << " un altra route " << endl;
				Route r_new = r_support;
				//cout << " La route adesso ha numero di nodi: " << r_new.index << endl;
				int NomeA1 = r_new.places[A + 1];


				destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);

				//cout << " La route adesso ha numero di nodi: " << r_new.index << " con numero di passeggieri rimossi " << passenger_removed.size() << endl;
				//cout << " Stampo la  route distrutta " << endl;
				//r_new.print();
				if (r_new.index != -1) {
					/*if (A != (r_new.index - 1) && r_new.places[A] == NomeA1 && r_new.index > 1) {
						//cout << " Ok vado a togliere anche il nodo A " << endl;
						destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);
						//for (Passenger p : passenger_removed) p.print();
						//r_new.print();
					}*/
					if (r_new.index != -1) {


						solution_rebuilt = repair_one_inter_move(peso_TW, peso_intermediate_stop, end_day, routes_destroyed, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);



						if (solution_rebuilt.size() != 0) {
							//int Npass = 0;
							//for (Route p : solution_rebuilt) Npass += p.passengers_in_route.size();
							//cout << " Numero Passeggeri dopo  repair dentro alla ricerca locale riga 6799 : " << Npass << " Sono i passeggieri senza route distrutta " << endl;
							//cout << " Numero Passegger nella route distrutta: " << r_new.passengers_in_route.size() << endl;
							solution_rebuilt.push_back(r_new);





							double before = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed); // Qui non va bene devi considerare che dopo un primo miglioramneto cambi la route
							double after = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (before > after) {
								//cout << " OK ! dopo mossa MIGLIORAAAAAAAAAAAAAAA di " << before <<" - "<<after << "------ Ma prima devo controllare che non sia con nodi doppi " << endl;
								// Qui sto usando tutto solution_rebuilt.back() ma in realta potrei usare r_new e poi un volta che la ho istemanta switcharla con solution_rebuilt.back()
								int node = sequential_same_node(solution_rebuilt.back());
								//cout << "node fuori dal while: " << node << endl;
								//r_new.print();
								bool fatto = false;
								int num_aggregazioni = 0;
								while (node != -1) {
									//cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << solution_rebuilt.back().aircraft_code<< endl;
									//solution_rebuilt.back().print();
									// i have two airports close to each other
									//cout << " Nodo dove inizia coppia --> " << node << endl;
									//if (node == 1) solution_rebuilt.back().print();
									//system("pause");
									//cout << "******* Aggrego nodo in posizione **********" << node << endl;
									//cout << " Numero nodi prima:  " << solution_rebuilt.back().index << endl;
									aggregate_same_nodes_inter_ls(solution_rebuilt.back(), node);
									//cout << " Numero nodi dopo:  " << solution_rebuilt.back().index << endl;
									if (A > node) num_aggregazioni++;
									//cout << " Ho finito codice di aggregazione " << endl;
									after = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to, location_fuel) << endl;
									//system("pause");
									node = sequential_same_node(solution_rebuilt.back());
									//solution_rebuilt.back().print();
									fatto = true;
								}

								if (fatto == false) {
									//cout << " Ok non aveva nodi doppi ! " << endl;
									//cout << " Il for degli A prima andava fino a " << r_support.index  << endl;
									r_support = r_new;
									//cout << " Il for degli A adesso va fino a " << r_support.index  << endl;
									routes_after_move = solution_rebuilt;
									routes = solution_rebuilt;
									// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
									routes_destroyed.clear();
									//routes_destroyed.shrink_to_fit();
									for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
									A = 1;
									r--;
									if (A >= (r_support.index - 1)) {
										//cout << " caso Loop " << endl;
									}
								}
								else {
									if (before > after) {
										// bisognerebbe mettere l'aggiornamento una cosa del tipo
										//cout << " Ok MIGLIORAAAAAAAAAAAAAAA aveva nodi doppi ! " << endl;
										//system("pause");
										//cout << " Il for degli A prima andava fino a " << r_support.index  << endl;
										r_support = r_new;
										//cout << " Il for degli A adesso va fino a " << r_support.index  << endl;
										routes_after_move = solution_rebuilt;
										routes = solution_rebuilt;
										//cout << " Ho finito while di aggregazione ma devo aggiornare A tolgo " << num_aggregazioni << endl;
										// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
										routes_destroyed.clear();
										//routes_destroyed.shrink_to_fit();
										for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
										A = 1;
										r--;
									}
								}
							}
						}
					}
				}
			}



		}



	}




	for (Route r_support : routes_after_move) {
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
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
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
	}
	if (routes_after_move.size() == 0) return routes;
	else {
		if (calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) != calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
			cout << " Costo Routes: " << calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
			cout << " Costo routes_after_move: " << calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
			//cin.get();
		}
		return routes_after_move;
	}
}

//************************************************************************************************************************************************************************

//data una location mi restituisce la location pi� vicina
int location_closest_with_fuel(int location, vector<vector<double>>& from_to, map<int, Airstrip>& map_airstrip) {
	int result = -1;
	double value = DBL_MAX;

	for (int i = 1; i < numero_airstrip_const; i++) {
		for (int j = 1; j < numero_airstrip_const; j++) {
			if (i == location && from_to[i][j] < value && map_airstrip[j].fuel) {
				result = j;
				value = from_to[i][j];
			}
		}
	}



	/*for (auto x : from_to) {
		if (split(x.first, ';')[0] == location && x.second < value && x.second != 0 && map_airstrip[split(x.first, ';')[1]].fuel) {
			result = split(x.first, ';')[1];
			value = x.second;
		}
	}*/


	return result;

}



//per il tratto finale
void fillLocation_fuel(vector<vector<double>>& risultato, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<vector<double>>& from_to, map<int, Airstrip>& map_airstrip) {


	risultato.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++)
		risultato[i].resize((size_t)numero_airstrip_const);



	for (Airplane f : airplanes) {
		for (Airstrip a : airstrips) {
			if (a.fuel) {
				risultato[f.code][a.code] = 0.0;
				//risultato.insert(make_pair(f.code + "/" + a.code, 0.0));
			}
			else {
				double fuel_needed = 0;
				double time_fly = from_to[a.code][location_closest_with_fuel(a.code, from_to, map_airstrip)] / f.speed;
				//double time_fly = from_to[a.code + ";" + location_closest_with_fuel(a.code, from_to, map_airstrip)] / f.speed;
				if (time_fly <= 1) {
					fuel_needed = time_fly * f.fuel_burn_first;
				}
				else {
					fuel_needed = f.fuel_burn_first + (time_fly - 1) * f.fuel_burn_second;
				}

				risultato[f.code][a.code] = fuel_needed;
				//risultato.insert(make_pair(f.code + "/" + a.code, fuel_needed));
			}
		}
	}




}

//funzione che TUTTI GLI ERRORI
void stampo_caso_strano(map<int, Airstrip> map_airstrip, int peso_TW, int peso_intermediate_stop, vector<vector<double>> from_to_costi, vector<vector<vector<double>>> from_to_fuel_costi, vector<Passenger> passengers, vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa) {


	/*double cost_solution = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to_costi, from_to_fuel_costi);
	double cost_single = 0.0;
	for (auto x : routes) {
		if(x.index > 1) cost_single += cost_single_route(peso_TW, peso_intermediate_stop, x, map_airstrip, map_airplane, from_to_costi, from_to_fuel_costi);
	}
	if (cost_single <= (cost_solution - 1)  || cost_single >= (cost_solution + 1) ) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i costi" << endl;
		cout << "costi singoli: " << cost_single << " -- costo totale: " << cost_solution << endl;
		system("pause");
		cin.get();
	}*/




	int numero = 0;
	for (auto z : routes) numero += z.passengers_in_route.size();
	if (numero != (int)passengers.size() && numero > 0) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono meno/piu' passeggeri" << endl;
		cout << numero << " -- " << passengers.size() << endl;
		system("pause");
		cin.get();
	}



	map<int, int> test_codici;
	for (auto x : routes) {
		if (test_codici.count(x.aircraft_code) >= 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono pi� codici aereo uguali" << endl;
			for (Route r : routes) cout << "codice: " << r.aircraft_code << endl;
			system("pause");
			cin.get();
		}
		else {
			test_codici.insert(make_pair(x.aircraft_code, 1));
		}
	}


	for (Route r : routes) {

		if (r.primo_pass) {
			if (r.index > (int)r.places.size()) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
				r.print();
				system("pause");
				cin.get();
			}


			for (Passenger p : r.passengers_in_route) {
				if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
					p.print();
					cout << "nella route: " << endl;
					r.print();
					system("pause");
					cin.get();
				}

				if (p.solution_from >= p.solution_to) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
					p.print();
					cout << "nella route: " << endl;
					r.print();
					system("pause");
					cin.get();
				}

			}



			for (int i = 1; i < r.index; i++) {


				if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO che un problema con la capacita nel nodo: " << i << endl;
					r.print();
					system("pause");
					cin.get();
				}



				if (r.time_arr[i] <= (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
					cout << "dovrebbe essere: " << (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece � " << r.time_arr[i] << endl;
					r.print();
					system("pause");
					cin.get();
				}


				double fly_time = (from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed);
				double fuel_consumed = 0.0;
				if (fly_time <= 1) {
					fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
				}

				if (!r.refueling[i]) {
					if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
						cout << "dovrebbe essere: " << fuel_consumed << " mentre invece � " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
						r.print();
						system("pause");
						cin.get();
					}
				}






			}


			for (int i = 0; i < r.index - 1; i++) {
				if ((r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1]) || r.weight[i] < -0.005) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
					r.print();
					system("pause");
					cin.get();
				}


				if (r.places[i] == r.places[i + 1]) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
					r.print();
					system("pause");
					cin.get();
				}


			}




			for (int i = 0; i < r.index; i++) {
				double peso_arco_passeggeri = 0.0;
				for (Passenger p : r.passengers_in_route) {
					if (p.solution_from <= i && p.solution_to > i) {
						peso_arco_passeggeri += p.weight;
					}
				}

				if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
					r.print();
					system("pause");
					cin.get();
				}
			}


			for (int i = 0; i < r.index - 1; i++) {
				double minimo_peso = r.weight[i];
				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
					for (int j = i + 1; j < r.index; j++) {
						if (r.refueling[j]) break;
						if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
					}
					if (minimo_peso >= 1) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
						r.print();
						system("pause");
						cin.get();
					}

				}
			}

		}





	}




}


vector <Route> aggrezione_simple_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to) {
	vector <Route> solution_after;

	cout << "*****************************SOLUTION PRIMA DELL'AGGREAGZIONE********************************" << endl;
	for (Route r : solution_model) {
		r.print();
	}
	cout << "****************************************ora faccio aggregazione con l'ultimo pezzo che si attacca solo se i due posti fine e inizio sono uguali***************" << endl;
	//cerco di collegare le route se � possibile
	//vector <Route> solution_after;
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";

			if (r != r1 && solution_model[r].places[solution_model[r].index - 1] == solution_model[r1].places[0] &&
				solution_model[r].time_dep[solution_model[r].index - 1] <= solution_model[r1].time_arr[0] &&
				map_airplane[solution_model[r].aircraft_code].model == map_airplane[solution_model[r1].aircraft_code].model
				&& index_not.find(val, 0) > index_not.size() && index_not.find(val1, 0) > index_not.size()) {

				index_not += to_string(r) + ";" + to_string(r1) + ";";
				Route r_add(solution_model[r].aircraft_code, solution_model[r].passengers_in_route);
				r_add.primo_pass = true;
				for (int i = 0; i < solution_model[r].index - 1; i++) {
					if (i == solution_model[r].index - 2) {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], (solution_model[r1].time_arr[0] - (((from_to[solution_model[r].places[i]][solution_model[r1].places[0]]) / map_airplane[solution_model[r].aircraft_code].speed) * 60)));
					}
					else {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], solution_model[r].time_dep[i]);
					}
				}

				for (int i = 0; i < solution_model[r1].index; i++) {
					r_add.addPlace(solution_model[r1].places[i], solution_model[r1].refueling[i], solution_model[r1].quantity_fuel[i], solution_model[r1].weight[i],
						solution_model[r1].capacity[i], solution_model[r1].time_arr[i], solution_model[r1].time_dep[i]);
				}
				for (Passenger& p : solution_model[r1].passengers_in_route) {
					p.solution_from += (solution_model[r].index - 1);
					p.solution_to += (solution_model[r].index - 1);
					r_add.passengers_in_route.push_back(p);

				}

				solution_after.push_back(r_add);
			}
		}
	}

	for (int r = 0; r < (int)solution_model.size(); r++) {
		string val = ";" + to_string(r) + ";";
		if (index_not.find(val, 0) > index_not.size()) {
			solution_after.push_back(solution_model[r]);
		}
	}





	cout << "*****************************SOLUTION MODEL -POSTERIORI- ********************************" << endl;
	cout << " Numero di soluzioni aggregate: " << solution_after.size() << endl;
	//for (Route r : solution_after) {
		//r.print();
	//}

	return solution_after;



}

vector <Route> aggrezione_complex_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector <Route> solution_after;

	//cout << "****************************************ora faccio aggregazione complessa***************" << endl;
	//cerco di collegare le route se � possibile
	//vector <Route> solution_after;
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";
			double time_trascorso = (from_to[solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]] / map_airplane[solution_model[r].aircraft_code].speed) * 60;
			double fuel_consumed = from_to_FuelConsumed[solution_model[r].aircraft_code][solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]];


			double costo_aggiunta = fuel_consumed + from_to[solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]];
			//READ ME: non � importante fare il check se i due posti (finale di una route, inziale della seconda) siano uguali, questa casistica viene eliminata dall'aggregazione semplice
			//non devo nemmeno fare il check sul fuel per andare in un altro posto in quanto vado sicuramente in un deposito dato che tutte le route partono in un depot
			//CONSIDERAZIONE, SI POTREBBE VALUTARE DI TOGLIERE IL PRIMO ARCO DAL DEPOT ALLA PRIMA LOCATION NEL CASO QUESTO SIA VUOTO, MA QUESTO PORTEREBBE ALLA CREAZIONE DI SCENARI DIFFERENTI:
			//		-posso arrivare a tutte le altre locazioni se tolgo il refuel al depot???????????????????????????????????????????????????????????????????????????
			// ho aggiunto il fatto che il costo del tratto aggiuntivo deve essere minore o uguale al costo fisso dell'aereo
			if (r != r1 &&
				solution_model[r].time_dep[solution_model[r].index - 1] + time_trascorso <= solution_model[r1].time_arr[0] &&
				solution_model[r].quantity_fuel[solution_model[r].index - 1] - fuel_consumed >= map_airplane[solution_model[r].aircraft_code].min_fuel &&
				map_airplane[solution_model[r].aircraft_code].model == map_airplane[solution_model[r1].aircraft_code].model
				&& index_not.find(val, 0) > index_not.size() && index_not.find(val1, 0) > index_not.size()
				&& costo_aggiunta <= map_airplane[solution_model[r1].aircraft_code].fixed_cost
				) {

				index_not += to_string(r) + ";" + to_string(r1) + ";";
				Route r_add(solution_model[r].aircraft_code, solution_model[r].passengers_in_route);
				r_add.primo_pass = true; //N.B. commentare questa riga se si vuole vedere quelle che aggrega
				for (int i = 0; i <= solution_model[r].index - 1; i++) {
					if (i == solution_model[r].index - 1) {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], (solution_model[r1].time_arr[0] - (((from_to[solution_model[r].places[i]][solution_model[r1].places[0]]) / map_airplane[solution_model[r].aircraft_code].speed) * 60)));
					}
					else {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], solution_model[r].time_dep[i]);
					}
				}

				for (int i = 0; i < solution_model[r1].index; i++) {
					r_add.addPlace(solution_model[r1].places[i], solution_model[r1].refueling[i], solution_model[r1].quantity_fuel[i], solution_model[r1].weight[i],
						solution_model[r1].capacity[i], solution_model[r1].time_arr[i], solution_model[r1].time_dep[i]);
				}
				for (Passenger& p : solution_model[r1].passengers_in_route) {
					p.solution_from += (solution_model[r].index);
					p.solution_to += (solution_model[r].index);
					r_add.passengers_in_route.push_back(p);

				}

				solution_after.push_back(r_add);
			}
		}
	}

	for (int r = 0; r < (int)solution_model.size(); r++) {
		string val = ";" + to_string(r) + ";";
		if (index_not.find(val, 0) > index_not.size()) {
			solution_after.push_back(solution_model[r]);
		}
	}





	cout << "*****************************SOLUTION MODEL -POSTERIORI- ********************************" << endl;
	for (Route& r : solution_after) {
		r.print();
	}

	return solution_after;



}

string location_closest_with_fuel_string(string location, map<string, double> from_to, map<string, Airstrip>& map_airstrip) {
	string result = "";
	double value = DBL_MAX;

	for (auto x : from_to) {
		if (split(x.first, ';')[0] == location && x.second < value && x.second != 0 && map_airstrip[split(x.first, ';')[1]].fuel) {
			result = split(x.first, ';')[1];
			value = x.second;
		}
	}


	return result;

}

map<string, double> fillLocation_fuel_string(vector<Airstrip> airstrips, vector<Airplane> airplanes, map<string, double>& from_to, map<string, Airstrip>& map_airstrip) {

	map <string, double> risultato;
	for (Airplane f : airplanes) {
		for (Airstrip a : airstrips) {
			if (a.fuel) {
				risultato.insert(make_pair(f.code_company + "/" + a.code_string, 0.0));
			}
			else {
				double fuel_needed = 0;
				double time_fly = from_to[a.code_string + ";" + location_closest_with_fuel_string(a.code_string, from_to, map_airstrip)] / f.speed;
				if (time_fly <= 1) {
					fuel_needed = time_fly * f.fuel_burn_first;
				}
				else {
					fuel_needed = f.fuel_burn_first + (time_fly - 1) * f.fuel_burn_second;
				}
				risultato.insert(make_pair(f.code_company + "/" + a.code_string, fuel_needed));
			}
		}
	}


	return risultato;

}


void fix_key_passenger_for_regret(vector<Passenger>& passengers_day1, vector<Passenger>& passengers_day2, vector<Passenger>& passengers_day3) {

	int chiave = 1;
	map<string, int> string_key;

	//per i passeggeri del giorno uno
	for (Passenger& p : passengers_day1) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}

	//per i passeggeri del giorno due
	for (Passenger& p : passengers_day2) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}


	//per i passeggeri del giorno tre
	for (Passenger& p : passengers_day3) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}



}


void Build_structure_Model(vector<Airplane> airplane, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
	int tipo = 0;
	for (int j = 0; j < (int)airplane.size(); j++) {
		//cout << " Creo per aereo " << airplane[j].code << endl;
		map<string, int>::iterator it = mappa_aereo_tipo.find(airplane[j].model + to_string(airplane[j].depot));
		if (it == mappa_aereo_tipo.end()) {
			mappa_aereo_tipo.insert(make_pair(airplane[j].model + to_string(airplane[j].depot), tipo));
			tipo++;
		}
	}
	for (int j = 0; j < (int)airplane.size(); j++) {
		map<string, int>::iterator it = mappa_aereo_tipo.find(airplane[j].model + to_string(airplane[j].depot));
		codice_aereo_tipo.insert(make_pair(airplane[j].code, it->second));
	}
	for (auto j : codice_aereo_tipo) {
		//cout << j.second << endl;
		map<int, int>::iterator it = tipo_numero.find(j.second);
		if (it == tipo_numero.end()) tipo_numero.insert(make_pair(j.second, 1));
		else tipo_numero[j.second] += 1;
	}

	for (map<string, int>::iterator it = mappa_aereo_tipo.begin(); it != mappa_aereo_tipo.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}
	for (map<int, int>::iterator it = codice_aereo_tipo.begin(); it != codice_aereo_tipo.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}
	for (map<int, int>::iterator it = tipo_numero.begin(); it != tipo_numero.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}


}

//bambini splittati
void unione_children_INTELLIGENTE(vector<Passenger>& passengers, vector<Passenger>& passengers_for_company_solution) {
	map<string, vector<Passenger>> groups;
	for (Passenger p : passengers) {
		if (groups.count(p.pnr_group) >= 1) {
			//qua vuol dire che c`é gia quel pnr
			groups[p.pnr_group].push_back(p);
		}
		else {
			//in questo caso il pnr va messo
			vector<Passenger> prr;
			prr.push_back(p);
			groups.insert(make_pair(p.pnr_group, prr));

		}
	}
	int numero_in = 0;
	vector<Passenger> passengers_aggr;

	for (auto x : groups) {
		vector<Passenger> genitori_maschi;
		vector<Passenger> genitori_donna;
		vector<Passenger> genitori;
		vector<Passenger> bambini;

		int numero_bimbi = 0;
		cout << "con il codice: " << x.first << endl;
		for (Passenger p : x.second) {
			p.print();
			numero_in++;
			if (p.gender == ("C")) {
				numero_bimbi++;
				bambini.push_back(p);
			}
			else {
				if (p.gender == "F") genitori_donna.push_back(p);
				if (p.gender == "M") genitori_maschi.push_back(p);
			}
		}
		cout << "bimbi in questo gruppo: " << numero_bimbi << endl;
		/*cout << "ora li guardiamo separati"<< endl;
		cout << "i genitori sono: " << endl;
		for(Passenger p: genitori) {
			p.print();
		}
		cout << "i bambini sono: " << endl;
		for(Passenger p: bambini) {
			p.print();
		}*/

		if ((int)bambini.size() > 0) {
			//incomincio a metterli con la mamma
			int indice_parent = 0;
			string precedente = "";
			for (Passenger p : bambini) {
				int loc_dep = p.departure_location;
				int loc_arr = p.arrival_location;
				int time_dep = p.departure_time;
				int time_arr = p.arrival_time;
				string attuale = to_string(p.departure_location) + to_string(p.arrival_location) + to_string(p.departure_time) + to_string(p.arrival_time);
				if (precedente != attuale) indice_parent = 0;
				precedente = attuale;


				vector<Passenger> genitori_gruppo;

				bool non_ha_genitore = true;
				if ((int)genitori_donna.size() > 0) {
					// metti i bambini con la mamma
					for (Passenger& g : genitori_donna) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time && time_arr == g.arrival_time) {
							genitori_gruppo.push_back(g);
							non_ha_genitore = false;
							//g.capacity += p.capacity;
							//g.weight += p.weight;
							//break;
						}
					}
				}
				if ((int)genitori_maschi.size() > 0) {
					//mettili con il papa
					for (Passenger& g : genitori_maschi) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time && time_arr == g.arrival_time) {
							genitori_gruppo.push_back(g);
							non_ha_genitore = false;
							/*g.capacity += p.capacity;
							g.weight += p.weight;
							break;*/
						}
					}


				}
				if (non_ha_genitore) {
					cout << "errore, ci sono dei bambini senza genitore" << endl;
				}

				//ora devo splittare
				bool trovato = false;
				for (Passenger& g : genitori_donna) {
					if (g.pnr == genitori_gruppo[indice_parent].pnr) {
						g.weight += p.weight;
						g.capacity++;
						trovato = true;
					}
				}
				if (!trovato) {
					for (Passenger& g : genitori_maschi) {
						if (g.pnr == genitori_gruppo[indice_parent].pnr) {
							g.weight += p.weight;
							g.capacity++;
							trovato = true;
						}
					}
				}
				if (!trovato) {
					cout << "c'e' un errore, controlla" << endl;
				}

				indice_parent++;
				if (indice_parent == genitori_gruppo.size()) indice_parent = 0;


			}



		}

		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << "ora i genitori sono cosi: " << endl;
		for (Passenger& g : genitori_donna) {
			g.print();
			passengers_aggr.push_back(g);
		}
		for (Passenger& g : genitori_maschi) {
			g.print();
			passengers_aggr.push_back(g);
		}
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		//system("pause");

		//cin.get();
	}
	cout << "ora faccio il check per vedere se le capacita' dei due gruppi tornano" << endl;
	int capacita_aggr = 0;
	for (Passenger p : passengers_aggr) capacita_aggr += p.capacity;
	cout << "le due capacita' sono: " << endl;
	cout << " - " << passengers.size();
	cout << " - " << capacita_aggr << endl;
	//system("pause");
	cout << "ho finito di vedere i gruppi" << endl;
	cout << "i passeggeri nei gruppi sono: " << numero_in << endl;
	if (numero_in != (int)passengers.size()) cout << "ERROREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
	//cin.get();


	passengers_for_company_solution = passengers;
	passengers = passengers_aggr;
}


//**********************************************************************************per rolling************************************************************************


int best_location(map <int, int>& location_request) {
	int best_richieste = 0;
	int best_location = -1;


	for (auto x : location_request) {
		if (best_richieste < x.second) {
			best_richieste = x.second;
			best_location = x.first;
		}
	}


	return best_location;
}

//questa funzione ritorna il posto più vicino alle 4 best location rispetto alle richieste che si hanno in quel giorno ma che non è uno di quei posti
int best_location_4place(map<int, int>& location_request, vector<vector<double>>& from_to, vector<Airstrip>& airstrips) {
	map <int, int> map_support;
	for (auto x : location_request) {
		cout << x.first << " ---> " << x.second << endl;
		map_support.insert(make_pair(x.second, x.first));
	}

	vector <int> best_places;
	int index = 0;
	for (auto x : map_support) {
		//cout << x.first << " ***---> " << x.second << endl;
		if (index >= map_support.size() - 4) best_places.push_back(x.second);
		index++;
	}
	//for (auto x : best_places) cout << "- " << x << endl;

	int best_place = -1;
	double best_distance = DBL_MAX;


	for (const Airstrip& a : airstrips) {
		if (a.code != best_places[0] && a.code != best_places[1] && a.code != best_places[2] && a.code != best_places[3]) {
			double distance = from_to[a.code][best_places[0]] + from_to[a.code][best_places[1]] + from_to[a.code][best_places[2]] + from_to[a.code][best_places[3]];
			if (distance < best_distance) {
				best_distance = distance;
				best_place = a.code;
			}

		}
	}



	return best_place;
}


void fuel_min_80_major_places(vector<vector<double>>& mappa_da_tornare, vector<vector<vector<double>>>& from_to_FuelConsumed, map<int, int>& location_request, vector<Airplane>& airplanes, vector<Airstrip>& airstrips) {

	//ho inizializato la mappa
	mappa_da_tornare.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++)
		mappa_da_tornare[i].resize((size_t)numero_airstrip_const);




	map <int, int> map_support;
	for (auto x : location_request) {
		cout << x.first << " ---> " << x.second << endl;
		map_support.insert(make_pair(x.second, x.first));
	}

	vector <int> best_places;
	int index = 0;
	double value = round(map_support.size() * 0.2);
	for (auto x : map_support) {
		//cout << x.first << " ***---> " << x.second << endl;
		if (index >= value) best_places.push_back(x.second);
		index++;
	}
	for (auto x : best_places) cout << " - " << x << endl;


	//codice che crea la mappa
	for (const Airstrip& a : airstrips) {
		double best_fuel = 0.0;
		int destination = -1;
		for (int a1 : best_places) {
			if (best_fuel < from_to_FuelConsumed[1][a.code][a1]) {
				best_fuel = from_to_FuelConsumed[1][a.code][a1];
				destination = a1;
			}
		}
		for (const Airplane& f : airplanes) {
			mappa_da_tornare[f.code][a.code] = from_to_FuelConsumed[f.code][a.code][destination];
			//mappa_da_tornare.insert(make_pair(f.code + ";" + a.code, from_to_FuelConsumed[f.code + ";" + a.code + ";" + destination]));
		}
	}

	//return mappa_da_tornare;
}



//PER MODELLO CONNESIONE*****************************************************************************************************************************************************************************************
//creazione dei flightleg
vector <Flightleg> creazione_flightleg(bool fittizio_da_mettere, vector<vector<Route>>& vector_solution_for_FL, map<int, Airplane>& map_airplane, vector<vector<vector<double>>>& from_to_fuelConsumed, vector<vector<double>>& from_to) {

	vector<Flightleg> flightlegs;

	for (vector<Route>& routes : vector_solution_for_FL) {
		for (Route& r : routes) {


			if (r.index > 1) {
				if (r.capacity[0] >= 1) {
					//in questo caso sono obbligato a creare un solo flight leg
					Flightleg f;
					f.cod_airplane = r.aircraft_code;
					f.from = r.places[0];
					f.to = r.places[r.index - 1];
					f.time_arr_from = r.time_arr[0];
					f.time_arr_to = r.time_arr[r.index - 1];
					f.time_dep_from = r.time_dep[0];
					f.time_dep_to = r.time_dep[r.index - 1];
					f.refuel_from = r.refueling[0];
					f.refuel_to = r.refueling[r.index - 1];
					f.quantity_fuel_from = r.quantity_fuel[0];
					f.quantity_fuel_to = r.quantity_fuel[r.index - 1];
					f.cost = r.cost;
					f.passengers_in_flightleg = r.passengers_in_route;
					f.route_padre = r;

					//in questo caso devo calcolare solo il fuel che può essere pescato
					if (f.refuel_to) {
						f.fuel_pescabile = map_airplane[f.cod_airplane].max_fuel - map_airplane[f.cod_airplane].min_fuel;
					}
					else {
						int index_refueling = 0;
						for (int i = r.index - 1; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						double fuel_pesc = r.quantity_fuel[index_refueling] - map_airplane[r.aircraft_code].min_fuel;
						for (int i = index_refueling + 1; i < r.index; i++) {
							fuel_pesc -= from_to_fuelConsumed[r.aircraft_code][i - 1][i];
						}

						f.fuel_pescabile = fuel_pesc;
					}


					flightlegs.push_back(f);
				}
				else {
					//in questo caso posso creare due flightleg
					//faccio la prima creazione uguale a quella precedente anche se il primo arco è 0
					Flightleg f;
					f.cod_airplane = r.aircraft_code;
					f.from = r.places[0];
					f.to = r.places[r.index - 1];
					f.time_arr_from = r.time_arr[0];
					f.time_arr_to = r.time_arr[r.index - 1];
					f.time_dep_from = r.time_dep[0];
					f.time_dep_to = r.time_dep[r.index - 1];
					f.refuel_from = r.refueling[0];
					f.refuel_to = r.refueling[r.index - 1];
					f.quantity_fuel_from = r.quantity_fuel[0];
					f.quantity_fuel_to = r.quantity_fuel[r.index - 1];
					f.cost = r.cost;
					f.passengers_in_flightleg = r.passengers_in_route;
					f.route_padre = r;

					//in questo caso devo calcolare solo il fuel che può essere pescato
					if (f.refuel_to) {
						f.fuel_pescabile = map_airplane[f.cod_airplane].max_fuel - map_airplane[f.cod_airplane].min_fuel;
					}
					else {
						int index_refueling = 0;
						for (int i = r.index - 1; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						double fuel_pesc = r.quantity_fuel[index_refueling] - map_airplane[r.aircraft_code].min_fuel;
						for (int i = index_refueling + 1; i < r.index; i++) {
							fuel_pesc -= from_to_fuelConsumed[r.aircraft_code][i - 1][i];
						}

						f.fuel_pescabile = fuel_pesc;
					}

					flightlegs.push_back(f);

					//ora faccio la seconda creazione se e solo se index è maggiore di 2 
					if (r.index > 2) {

						double costo_da_sottrarre = from_to[r.places[0]][r.places[1]] + from_to_fuelConsumed[r.aircraft_code][r.places[0]][r.places[1]];

						Flightleg f1;
						f1.cod_airplane = r.aircraft_code;
						f1.from = r.places[1];
						f1.to = r.places[r.index - 1];
						f1.time_arr_from = r.time_arr[1];
						f1.time_arr_to = r.time_arr[r.index - 1];
						f1.time_dep_from = r.time_dep[1];
						f1.time_dep_to = r.time_dep[r.index - 1];
						f1.refuel_from = r.refueling[1];
						f1.refuel_to = r.refueling[r.index - 1];
						f1.quantity_fuel_from = r.quantity_fuel[1];
						f1.quantity_fuel_to = r.quantity_fuel[r.index - 1];
						f1.cost = r.cost - costo_da_sottrarre;
						f1.passengers_in_flightleg = r.passengers_in_route;
						f1.route_padre = r;

						//devo calcolare il fuel minimo che necessita per essere percorsa
						if (!f1.refuel_from) {
							double fuel_necess = map_airplane[r.aircraft_code].min_fuel;
							for (int i = 2; i < r.index; i++) {
								fuel_necess += from_to_fuelConsumed[r.aircraft_code][i - 1][i];
								if (r.refueling[i])	break;
							}
							f1.fuel_necessario_per_essere_fatta = fuel_necess;
						}



						if (f1.refuel_to) {
							f1.fuel_pescabile = map_airplane[f1.cod_airplane].max_fuel - map_airplane[f1.cod_airplane].min_fuel;
						}
						else {
							int index_refueling = 0;
							for (int i = r.index - 1; i >= 0; i--) {
								if (r.refueling[i]) {
									index_refueling = i;
									break;
								}
							}
							double fuel_pesc = r.quantity_fuel[index_refueling] - map_airplane[r.aircraft_code].min_fuel;
							for (int i = index_refueling + 1; i < r.index; i++) {
								fuel_pesc -= from_to_fuelConsumed[r.aircraft_code][i - 1][i];
							}

							f1.fuel_pescabile = fuel_pesc;
						}


						flightlegs.push_back(f1);

					}





				}



			}







		}
	}


	//ora aggiungo il flightleg fittizio solo se è necessario aggiungerlo, quindi se la variabile fittizio_da_aggiungere è vera
	if (fittizio_da_mettere) {
		Flightleg f;
		f.flight_leg_fittizzio = true;
		flightlegs.push_back(f);
	}




	return flightlegs;
}
//matrice uno o zero se possono essere collegati i due flightleg
vector<vector<int>> creazione_matrice_connection_FL(double start_day, double end_day, vector<Flightleg>& flightlegs_1, vector<Flightleg>& flightlegs_2, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_fuelConsumed) {


	vector<vector<int>> matrice_da_tornare;

	for (Flightleg& f1 : flightlegs_1) {
		vector<int> riga_f1;
		if (!f1.flight_leg_fittizzio) {
			for (Flightleg& f2 : flightlegs_2) {
				if (map_airplane[f1.cod_airplane].model == map_airplane[f2.cod_airplane].model && map_airplane[f1.cod_airplane].depot == map_airplane[f2.cod_airplane].depot) {
					//se sono qua dentro i due aerei hanno lo stesso modello e deposito
					double fuel_necessario_f2 = f2.fuel_necessario_per_essere_fatta + from_to_fuelConsumed[f1.cod_airplane][f1.to][f2.from]; //fuel per essere fatta più fuel connessione
					if (f1.fuel_pescabile >= fuel_necessario_f2) {
						//se sono qua dentro il fuel va bene, ora devo controllare che il tempo vada bene
						double time_fly_connection = (from_to[f1.to][f2.from] / map_airplane[f1.cod_airplane].speed) * 60;
						bool time_ok = false;

						if (f1.time_dep_to + time_fly_connection <= end_day) time_ok = true;
						if (f2.time_arr_from - time_fly_connection >= start_day) time_ok = true;




						if (time_ok) {
							riga_f1.push_back(1);
						}
						else {
							riga_f1.push_back(0);
						}
					}
					else {
						riga_f1.push_back(0);
					}

				}
				else {
					riga_f1.push_back(0);

				}



			}

		}//qua attacco a matrice da tornare la riga del flightleg1
		else {
			//qua metto tutti uno dove c'è il flight leg fittizio
			for (Flightleg& f2 : flightlegs_2) riga_f1.push_back(1);
		}

		matrice_da_tornare.push_back(riga_f1);
	}





	return matrice_da_tornare;
}

//codice per la risoluzione di un giorno e che salva anche dentro un vettore di route tutte le route create, sarà questo da passare a creazione flightleg
vector<Route> solution_one_day(vector<vector<Route>>& vector_solution_for_FL, int Iter_FirstDo, int iterMAX_FirstDo, int number_airplane, vector<Airplane> airplanes, double peso_TW, double peso_itermediate_stop, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed, vector<Passenger>& passengers_per_casostrano, map<string, double>& from_to_company, map<int, string>& legenda_ritroso, map<int, Passenger>& map_id_passenger, vector<Passenger>& passengers, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
	ofstream outfile; //per il file di input grafico.txt++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	outfile.open("grafico.txt");
	int number_iteration_in_heuristic = 0;

	double time_spent_model = 0.0;
	double tempo_finale;
	time_t time_now = time(NULL);
	//input heuristic
	//int number_airplane = stoi(argv[4]); // Per Modello
	//int number_airplane = 12;
	//cout << "il numero degli aerei e: " << number_airplane << endl;
	//cin.get();
	//int Iter_FirstDo = 0;
	//int iterMAX_FirstDo = 10;
	//double best_solution_ever = DBL_MAX;
	int NDestroy = 4;
	int NRepair = 5;
	int NHeuristic = 2;
	int initial_weigth = 5;
	vector<double> Weigth_destroy;
	vector<double> Weigth_repair;
	vector<double> Weigth_heuristic(NHeuristic);
	for (int i = 0; i < NDestroy; i++) Weigth_destroy.push_back(initial_weigth);
	for (int i = 0; i < NRepair; i++) Weigth_repair.push_back(initial_weigth);
	Weigth_heuristic[0] = 1.00; //parallel
	Weigth_heuristic[1] = 0.00; //sequential
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	vector<vector<Route>> solutionAll;
	//vector<Route> best_solution_route_ever;
	srand(time(NULL));
	int NumeroSA = 0;
	int iterazioni_fallite = 0;
	//double start_day = 360.0;    //ATTENTION, MAY BE YOU WILL HAVE TO USE IT
	double end_day = 1110.0;
	int number_of_aircraft = 20;
	vector<Route> start_solution_route;
	double start_solution = 0.0;
	int start_route_bs = 0;
	int size_route_bs = 0;  //se ci sono dei problemi rimettilo
	int best_iteration = 0;
	do {
		cout << " ************************************************************ Inizio un altro giro nel ciclo grande ************************************************************ " << endl;
		double r;
		double best_solution = DBL_MAX;
		int iteration = 0;
		int iteration_max = 30; //35 se vuopi fare un solo giro 
		int T = 25000;
		int Tmin = 150;    //50 un solo giro 
		int k = 0;
		double MinLevel = 2;
		double MaxLevel = 6;
		// Valore che puo variare 0.8 a 0.99
		double alpha = 0.87; // 0.98 un solo giro
		int DeltaK = 55;      // 25 prima
		if (Iter_FirstDo > 4) {
			T = 2500;        // Almeno 2000
			alpha = 0.95; // Almeno 0.997
			DeltaK = 30; //Almeno 25
			Tmin = 50;
			iteration_max = 80;
		}


		if (Iter_FirstDo == 0) {

			double heuristic_choice = (double)rand() / RAND_MAX;
			//cout << " heuristic_choice =  " << heuristic_choice << endl;
			//for (int i = 0; i < NHeuristic; i++) cout << Accumulated(i, Weigth_heuristic) << " || ";
			//cout << endl;
			int npass = 0;
			do {
				//auto rng1 = default_random_engine{};
				std::random_device rd;
				std::mt19937 g(rd());
				shuffle(begin(passengers), end(passengers), g);

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					//cout << "*************** I am using the Parallel Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
				}
				else {
					//cout << "*************** I am using the Sequential Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase_sequential(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
				}
				for (Route& r : start_solution_route) npass += (int)r.passengers_in_route.size();
				//cout << "ecco le route" << endl;
				//for (Route r : start_solution_route) r.print();
				cout << "numero di passeggeri " << npass << endl;
				cout << "passengers.size()= " << passengers.size() << endl;
				//system("pause");
				//cin.get();
				number_iteration_in_heuristic++;
			} while ((int)passengers.size() > npass);
		}

		start_solution = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		//cout << "il costo del costruttivo e': " << start_solution << endl;
		//system("pause");
		cout << "ho appena fatto il costruttivo" << endl;
		stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
		solutionAll.push_back(start_solution_route);
		vector_solution_for_FL.push_back(start_solution_route);
		//cout << "Costo Starting Greedy Solution: " << start_solution << endl;
		//for (Route r : start_solution_route) r.print();
		//system("pause");

		//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
		outfile << "soluzione euristica di partenza:;0;" << start_solution << endl;
		//outfile.close();
		//*********************************************************************************************************************************************************************************************************************

		vector<Route> best_solution_route = start_solution_route;
		best_solution = start_solution;
		// SI INIZI IL CILCO CON LA RICERCA LOCALE E SIMULATING ANNEALING
		do {
			NumeroSA++;
			//*************************************heuristic costructive***************************************************************************************
			//cout << "Iteration number: " << iteration << endl;
			//cout << "Temperatura: " << T << endl;
			//cout << "K: " << k << endl;
			//cout << "The cost of the best solution X* is: " << best_solution << endl;
			//cout << "The cost of the current X is: " << start_solution << endl;
			double random = (double)rand() / RAND_MAX;
			double percentage_route_destroy = (MinLevel + (random * (MaxLevel - MinLevel)));
			//cout << "---> Percentuale di distruzione " << percentage_route_destroy << endl;
			int choosen_destroy = -1;
			int choosen_repair = -1;
			vector<Passenger> passenger_removed;
			// REMOVING OF A RANDOM NODE
			vector<Route> Input_destroy = start_solution_route;
			//cout << "***************************DISTRUCTION*******************" << endl;
			double destroy_choice = (double)rand() / RAND_MAX;
			vector<Route> route_destroy;
			//double starting_range = (Weigth_destroy[0]) / (Sum(Weigth_destroy));
			//cout << " destroy_choice =  " << destroy_choice << endl;
			//for (int i = 0; i < Weigth_destroy.size(); i++) cout << Accumulated(i, Weigth_destroy) << " || ";
			//cout << endl;
			//using namespace std::chrono;
			auto start = chrono::high_resolution_clock::now();
			if (destroy_choice < Accumulated(0, Weigth_destroy)) {
				//cout << "*************** I am using the Cluster Destroy ************" << endl;
				int num_passenger_cluster = (int)((passengers.size() * 0.14) + (random * ((passengers.size() * 0.24) - (passengers.size() * 0.14)))); //era 0.24 prima
				route_destroy = destroy_cluster_aggr2(peso_TW, num_passenger_cluster, passenger_removed, Input_destroy, map_airplane, from_to, passengers, map_id_passenger, peso_itermediate_stop);
				choosen_destroy = 0;
			}
			else if (destroy_choice < Accumulated(1, Weigth_destroy)) {
				//cout << "*************** I am using the Worst Destroy ************" << endl;
				route_destroy = destroy_worst(peso_TW, peso_itermediate_stop, percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 1;
			}
			else if (destroy_choice < Accumulated(2, Weigth_destroy)) {
				//cout << "*************** I am using the Casual Destroy ************" << endl;
				route_destroy = destroy_casual(percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 2;
			}
			else {
				//cout << "*************** I am using the Thanos Destroy ************" << endl;
				route_destroy = destroy_thanos(percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 3;
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
			double time_spent = Time_Spend((double)duration.count());
			//cout << " Tempo Destroy vero: " << duration.count() << " ma corrisponde al valore trasformato in double = " << time_spent << endl;
			//cout << "***************************END DISTRUCTION*******************" << endl;
			//cout << "ho appena fatto la distruzione" << endl;
			//vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa
			//stampo_caso_strano_destroy(route_destroy, from_to_company, map_airplane, legenda_ritroso);
			//for (Route p : route_destroy) p.print();
			//for(Route p : route_destroy) p.print();
			//cout << "ecco i passeggeri da reinserire: " << endl;
			//for (Passenger p : passenger_removed) p.print();
			//cout << "********************************REBUILT***************************" << endl;
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;
			//cout << " repair_choice =  " << repair_choice << endl;
			//for (int i = 0; i < Weigth_repair.size(); i++) cout << Accumulated(i, Weigth_repair) << " || ";
			//cout << endl;


			//PER TEST CON PUNTATORI************************************************************************
			/*cout << "sono prima dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;;
			int pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluione " << pass_soluzione << endl;
			int pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl;
			if (passengers.size() != (pass_dist + passenger_removed.size())) {
				cout << "si creano dei passeggeri" << endl;
				system("pause");
			}
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			*/

			auto start_r = chrono::high_resolution_clock::now();
			if (repair_choice < Accumulated(0, Weigth_repair)) {
				//cout << "*************** I am using the Repair One ************" << endl;
				solution_rebuilt = repair_one(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//for (Route r : solution_rebuilt) r.print();
				//system("pause");


				choosen_repair = 0;
			}
			else if (repair_choice < Accumulated(1, Weigth_repair)) {
				//cout << "*************** I am using the Repair SP ************" << endl;
				solution_rebuilt = repairSP(peso_TW, peso_itermediate_stop, route_destroy, passenger_removed, map_airplane, map_airstrip, from_to, airplanes, end_day, passengers, number_of_aircraft, location_fuel, from_to_FuelConsumed);
				choosen_repair = 1;
			}
			else if (repair_choice < Accumulated(2, Weigth_repair)) {
				//cout << "*************** I am using the Repair 2 Regret ************" << endl;
				solution_rebuilt = two_regret_repair_aggragati(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 2;
			}
			else if (repair_choice < Accumulated(3, Weigth_repair)) {
				//cout << "*************** I am using the Repair Forbidden ************" << endl;
				solution_rebuilt = repair_forbidden(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 3;
			}
			else {
				//cout << "*************** I am using the Repair Perturbation ************" << endl;
				solution_rebuilt = repair_perturbation(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 4;
			}
			auto stop_r = chrono::high_resolution_clock::now();
			auto duration_r = chrono::duration_cast<chrono::seconds>(stop_r - start_r);
			double time_spent_r = Time_Spend((double)duration_r.count());

			/*cout << "sono dopo dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;
			pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluzione " << pass_soluzione << endl;
			pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl;
			*/


			//cout << "ho appena fatto i repair" << endl;
			//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);


			if (solution_rebuilt.size() > 0) {
				double initial_cost = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt: " << initial_cost << endl;
				//vector<Route> solution_rebuilt_without_LS = solution_rebuilt;
				//cout << "sto iniziando il move" << endl;
				solution_rebuilt = move(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
				//cout << "ho appena fatto il move" << endl;
				//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);
				//cout << "Cost of the solution rebuilt after Move: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				solution_rebuilt = swap(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
				//cout << "ho appena fatto lo swap" << endl;
				//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);
				//cout << "Cost of the solution rebuilt after Move + Swap: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				if (initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
					//cout << "in questo caso ho fatto l'inter-move" << endl;
					//int pass_per_test = 0;
					//for (Route r : solution_rebuilt) cout << "- " << r.aircraft_code << endl;

					solution_rebuilt = inter_move(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
					//cout << "ho appena fatto lo inter move" << endl;
					//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);

					/*cout << "sono dentro l'inter-move" << endl;
					cout << "passeggeri rimossi " << passenger_removed.size() << endl;
					pass_soluzione = 0;
					for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
					cout << "passenger nella soluzione " << pass_soluzione << endl;
					pass_dist = 0;
					for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
					cout << "passenger nella soluzione distrutta " << pass_dist << endl;
					cout << "prima del inter-move erano: " << pass_per_test << endl;
					if(pass_per_test != pass_soluzione) system("pause");
					*/


					//cout << "Cost of the solution rebuilt after Move + Swap: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				}
				//cout << "passato if intermove" << endl;
				solution_rebuilt = heuristic_costructive_second_fase(solution_rebuilt, end_day, peso_TW);
				double cost_objectiveFunction_second_fase_after_rebuilt = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt with Move + Swap + Second phase: " << cost_objectiveFunction_second_fase_after_rebuilt << endl;
				//cout << "la best solution fino a questo punto e': " << best_solution << endl;
				//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
				if (NumeroSA == 1000 || NumeroSA == 2000 || NumeroSA == 3000 || NumeroSA == 4000 || NumeroSA == 5000 || NumeroSA == 6000 || NumeroSA == 7000 || NumeroSA == 8000 || NumeroSA == 9000 || NumeroSA == 10000 ||
					NumeroSA == 11000 || NumeroSA == 12000 || NumeroSA == 13000 || NumeroSA == 14000 || NumeroSA == 15000 || NumeroSA == 16000 || NumeroSA == 17000 || NumeroSA == 18000 || NumeroSA == 19000 || NumeroSA == 20000 ||
					NumeroSA == 21000 || NumeroSA == 22000 || NumeroSA == 23000 || NumeroSA == 24000 || NumeroSA == 25000 || NumeroSA == 26000 || NumeroSA == 27000 || NumeroSA == 28000 || NumeroSA == 29000 || NumeroSA == 30000 ||
					NumeroSA == 31000 || NumeroSA == 32000 || NumeroSA == 33000 || NumeroSA == 34000 || NumeroSA == 35000 || NumeroSA == 36000 || NumeroSA == 37000 || NumeroSA == 38000 || NumeroSA == 39000 || NumeroSA == 40000 ||
					NumeroSA == 41000 || NumeroSA == 42000 || NumeroSA == 43000 || NumeroSA == 44000 || NumeroSA == 45000 || NumeroSA == 46000 || NumeroSA == 47000 || NumeroSA == 48000 || NumeroSA == 49000 || NumeroSA == 50000 ||
					NumeroSA == 51000 || NumeroSA == 52000 || NumeroSA == 53000 || NumeroSA == 54000 || NumeroSA == 55000 || NumeroSA == 56000 || NumeroSA == 57000 || NumeroSA == 58000 || NumeroSA == 59000 || NumeroSA == 60000) {
					//ofstream outfile1;
					//outfile1.open("grafico.txt");
					tempo_finale = difftime(time(NULL), time_now);
					outfile << "NumeroSA:;" << NumeroSA << ";" << best_solution << ";tempo:;" << to_string(tempo_finale) << endl;
					//outfile1.close();
				}
				//*********************************************************************************************************************************************************************************************************************





				//if (cost_objectiveFunction_second_fase_after_rebuilt < initial_cost) solutionAll.push_back(solution_rebuilt_without_LS);
				//cout << "costo routing: " << costo_senza_time_windows(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//cout << "costo time windows: " << costo_time_windows(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				if (cost_objectiveFunction_second_fase_after_rebuilt < start_solution) {
					//cout << "Ottimo aggiorno la soluzione da cui continuare a fare perturbazioni " << endl;
					start_solution_route.clear();
					//start_solution_route.shrink_to_fit();
					//cout << " Ho cancellato " << endl;
					//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					for (Route& so : solution_rebuilt) start_solution_route.push_back(so);
					//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					double award = Ob_Funct_Improvement(cost_objectiveFunction_second_fase_after_rebuilt, start_solution);
					//cout << " Con un miglioramento di " << start_solution - cost_objectiveFunction_second_fase_after_rebuilt << " si ha un award di " << award << endl;
					//cout << " ed un fattore tempo di Destroy di " << time_spent << endl;
					//cout << " ed un fattore tempo di Repair di " << time_spent_r << endl;
					start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
					iteration = 0;
					//cout << " Aggiungo al destroy ad al Repair 1 + award =  " << award << endl;
					Weigth_destroy[choosen_destroy] += (1 + award - time_spent);
					Weigth_repair[choosen_repair] += (1 + award - time_spent_r);
					if (cost_objectiveFunction_second_fase_after_rebuilt < best_solution) {
						//cout << "Aggiorno migliore soluzione " << endl;
						best_solution_route.clear();
						//best_solution_route.shrink_to_fit();
						for (Route& s : solution_rebuilt) best_solution_route.push_back(s);
						best_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						best_iteration = iteration;
						start_route_bs = (int)solutionAll.size();
						size_route_bs = (int)solution_rebuilt.size();
						//cout << " Ho aggiornato la migliore soluzione le route adesso si trovano nella posizione: " << start_route_bs << " con size: " << size_route_bs << endl;
						//system("pause");
					}
				}
				else {

					r = double(rand()) / (double(RAND_MAX) + 1.0);
					//cout << " Ho generato numero random " << r << endl;
					//cout << "-( " << cost_objectiveFunction_second_fase_after_rebuilt << " - " << start_solution << ") / " << T;
					double espo = -(cost_objectiveFunction_second_fase_after_rebuilt - start_solution) / T;
					//cout << " Adesso ho generato espo " << espo << endl;
					//cout << " Se non lo � devo incrementare iterazione se lo � aggiorno valori X con X'  " << endl;
					if (r <= exp(espo) && exp(espo) != 1) {
						//cout << "Confrona i costi sono entrato perche: " << r << " < " << exp(espo) << endl;
						start_solution_route.clear();
						//start_solution_route.shrink_to_fit();
						//cout << " Ho cancellato " << endl;
						//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
						for (Route& s : solution_rebuilt) start_solution_route.push_back(s);
						//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
						start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						iteration = 0;
						Weigth_destroy[choosen_destroy] ++;
						Weigth_repair[choosen_repair] ++;
					}
					else {
						iteration++;
					}
				}
				solutionAll.push_back(solution_rebuilt);
				vector_solution_for_FL.push_back(solution_rebuilt);
				cout << "Numero iterazioni ---> " << NumeroSA << endl;
			}
			else {
				//cout << " ************ ITERAZIONE FALLITA ************************* " << endl;
				iterazioni_fallite++;
				//system("pause");
			}
			k++;
			//cout << " Valore attuale di K = " << k << " aggiorno temperatura dopo: " << DeltaK << endl;
			if (k >= DeltaK) {
				T = (int)(alpha * T);
				//cout << " Aggiorno temperatura A: " << T << endl;
				k = 0;
			}
			//cout << " A T < " << Tmin << " devi fermarti T= " << T << " or " << iteration << " > " << iteration_max << " Ciclo Grande fuori iterazione ------> " << Iter_FirstDo << endl;
		} while ((iteration <= iteration_max) && (T > Tmin));
		if (Iter_FirstDo > 4) cout << " Iterazioni senza migliorare " << iteration << " Temperaruta: " << T << endl;
		tempo_finale = difftime(time(NULL), time_now);
		outfile << "soluzione prima del modello:;" << NumeroSA << ";" << best_solution << ";tempo:; " << to_string(tempo_finale) << endl;
		cout << ".........Build all the structure for the Model........." << endl;
		auto start_model = chrono::high_resolution_clock::now();
		vector<Airplane> airplanes_model;
		for (const auto& k : map_airplane) {
			airplanes_model.push_back(k.second);
		}
		map<int, vector<Route>> airplane_routes;
		map<int, int> solution_warm_up;
		for (const Airplane& a : airplanes_model) {
			vector<Route> routes;
			airplane_routes.insert(make_pair(a.code, routes));
		}
		int counter = 0;
		for (const vector<Route>& solution : solutionAll) {
			if (counter == start_route_bs) {
				for (const Route& r : solution) {
					// If � la modififica
					if (r.passengers_in_route.size() > 0) {
						solution_warm_up[r.aircraft_code] = (int)(airplane_routes[r.aircraft_code].size());
						airplane_routes[r.aircraft_code].push_back(r);
					}
				}
			}
			else {
				for (const Route& r : solution) {
					airplane_routes[r.aircraft_code].push_back(r);
				}
			}

			counter++;
		}

		cout << " CHECK SE LA MAPPA CHE HO CREATO VA BENE " << endl;
		vector<Route> Prova;
		for (const auto& a : airplane_routes) {
			if (solution_warm_up.find(a.first) != solution_warm_up.end()) {
				cout << " Aereo: " << a.first << endl;
				Prova.push_back(a.second[solution_warm_up[a.first]]);
			}
		}
		cout << " il costo della soluzione che mi sono salvato nella mappa: " << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, Prova, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
		int n = 0;
		for (Route& path : Prova) {
			//path.print();
			n += (int)path.passengers_in_route.size();
		}
		cout << " Soluzione contiene numero di passeggieri pari a " << n << endl;
		//finito di fissare gli aerei
		vector<vector<vector<int>>> A3;  //matrix A
		vector<vector<double>> C;  //cost
		vector<Route> routes;
		for (auto& airplane : airplanes_model) {
			//cout << "Airplane: " << airplane.code << endl;
			vector<double> c;
			vector<vector<int>> A2;
			cout << "la size di airplanc routes e': " << endl;
			cout << airplane_routes.size() << endl;
			for (Route& r : airplane_routes[airplane.code]) {
				//cout << "sto stampando il codice" << endl;
				//cout << "-->" << r.aircraft_code << endl;
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
				//cout << "- " << r.cost << endl;
				c.push_back(r.cost);
			}
			A3.push_back(A2);
			C.push_back(c);
		}
		//Creating model with Cplex
		//Create new Model object
		cout << "********************************************************	0	*********************************************************************************" << endl;
		vector<Route> solution_modello;
		solutionAll.clear();
		cout << " Size del Pool di Routes Prima di chimare modello " << solutionAll.size() << endl;
		Model_Cplex *model = new Model_Cplex(A3, C, airplanes_model, airplane_routes, routes, (int)passengers.size(), passengers, solution_modello, map_airplane);
		//Create and solve model with gurobi
		model->create_Model_cplex(number_airplane, solution_warm_up, mappa_aereo_tipo, codice_aereo_tipo, tipo_numero, solutionAll);
		cout << " Size del Pool di Routes Dopo chiamata del modello " << solutionAll.size() << endl;
		cout << " Size del Pool di Routes salvate " << solutionAll[0].size() << endl;
		for (Route& s : model->solution_model) cout << " Codici aereo della soluzione: " << s.aircraft_code << endl;
		start_solution_route.clear();
		//start_solution_route.shrink_to_fit();
		if (model->solution_model.size() > 0) {
			for (Route& s : model->solution_model) start_solution_route.push_back(s);
		}
		else {
			for (Route& s : best_solution_route) start_solution_route.push_back(s);
		}
		start_solution = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		cout << " Costo del Modello che sara nuovo costo di partenza = " << start_solution << endl;
		
		//********************************************************************************************************************************************************************


		vector_solution_for_FL.push_back(start_solution_route);
		start_route_bs = (int)solutionAll.size();
		size_route_bs = (int)start_solution_route.size();
		solutionAll.push_back(start_solution_route);
		Iter_FirstDo++;
		cout << "*******************************************************************************************************************************************************" << endl;
		cout << " ********************************************** ITERATION WHILE: " << Iter_FirstDo << " OF: " << iterMAX_FirstDo << "**********************************" << endl;
		cout << "*******************************************************************************************************************************************************" << endl;
		auto stop_model = chrono::high_resolution_clock::now();
		auto duration_model = chrono::duration_cast<chrono::seconds>(stop_model - start_model);
		time_spent_model += (double)duration_model.count();
		//cin.get();
	} while ((Iter_FirstDo <= iterMAX_FirstDo));

	cout << "Best solution herurisic: " << start_solution << endl;
	cout << "costo routing: " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << "costo time windows: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	cout << " Alla iterazione numero: " << best_iteration << endl;
	for (Route s : start_solution_route) cout << s.cost << " -- " << endl;
	//*********************************************************************************AGGREGAZIONE SEMPLICE***************************************************************
	cout << "ora faccio l'aggregazione semplice" << endl;
	start_solution_route = aggrezione_simple_after_model(start_solution_route, map_airplane, from_to);
	cout << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************AGGREGAZIONE COMPLESSA***************************************************************
	cout << "ora faccio l'aggregazione complessa" << endl;
	start_solution_route = aggrezione_complex_after_model(start_solution_route, map_airplane, from_to, from_to_FuelConsumed);
	cout << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************************************************************************************************
	cout << " ********** Final Weight ************ " << endl;
	cout << " Cluster || Worst || 2-Regret || Thanos " << endl;
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	cout << " One || SP || Regret || Forbidden ||Perturbation " << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	tempo_finale = difftime(time(NULL), time_now);
	cout << "Time at the end: " << tempo_finale << endl;
	cout << "Time model: " << time_spent_model << endl;
	cout << "Numero SA= " << NumeroSA << endl;
	cout << "Iterazioni Fallite= " << iterazioni_fallite << endl;
	cout << "number of iteration in the heuristic: " << number_iteration_in_heuristic << endl;
	outfile.close(); // per il grafico++++++++++++++++++++++++++++++++++++++++++++++++++++++


	cout << " Seleziono solo le migliori che abbiamo ... " << endl;
	map<double, int> cost; 
	map<double, int>::iterator it;


	

	int i = 0; 
	double valore = 0.0;
	for (auto vettore : vector_solution_for_FL) {
		valore = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, vettore, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		it = cost.find(valore);
		if (it == cost.end()) {
			cost.insert(make_pair(valore, i));
		}	
		i++;
	}
	cout << " Stampo soluzioni con costo ...." << endl;
	//for (auto& elemnt : cost) cout << elemnt.first << " " << elemnt.second << endl;
	vector<vector<Route>> migliori;
	i = 0;
	for (auto& elemnt : cost) {
		if (i == 80) break;
		cout << " Butto dentro route con costo .. " << elemnt.first << " numero .. " << elemnt.second << endl;
		vector<Route> vettore = (vector_solution_for_FL.at(elemnt.second));
		//cout << " Controllo per sicurezza costo .. " << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, vettore, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
		migliori.push_back(vector_solution_for_FL.at(elemnt.second));
		i++;
	}
		
	vector_solution_for_FL.clear();
	vector_solution_for_FL = migliori;

	return start_solution_route; // ritorno le route migliori

}

//matrice che calcola il costo della connessione
vector<vector<vector<double>>> matrice_connessione(vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed, vector<Airplane>& airplanes, vector<Airstrip>& airstrip) {
	vector<vector<vector<double>>> matrice_da_tornare;

	matrice_da_tornare.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++) {
		matrice_da_tornare[i].resize((size_t)numero_airstrip_const);
		for (int j = 0; j < numero_airstrip_const; j++) {
			matrice_da_tornare[i][j].resize((size_t)numero_airstrip_const);
		}
	}

	for (Airplane& a : airplanes) {
		for (Airstrip& loc1 : airstrip) {
			for (Airstrip& loc2 : airstrip) {
				matrice_da_tornare[a.code][loc1.code][loc2.code] = from_to_FuelConsumed[a.code][loc1.code][loc2.code] + from_to[loc1.code][loc2.code];
			}

		}

	}

	return matrice_da_tornare;

}

//matrice che per ogni flightleg ha 1 se contiente il passenger x
vector<vector<int>> creazione_matrice_flightleg_passengers(vector<Flightleg>& flightlegs, vector<Passenger>& passengers) {

	vector<vector<int>> matrice_da_tornare;
	vector<int> id_passengers;
	for (Passenger& p : passengers) id_passengers.push_back(p.pnr);



	for (Flightleg& f : flightlegs) {
		vector<int> riga_f;
		vector<int> id_FL;
		for (Passenger& p : f.passengers_in_flightleg) id_FL.push_back(p.pnr);

		for (int id_p : id_passengers) {
			std::vector<int>::iterator it = std::find(id_FL.begin(), id_FL.end(), id_p);
			if (it != id_FL.end()) {
				//l'ho trovato
				riga_f.push_back(1);
			}
			else {
				riga_f.push_back(0);
			}
		}


		matrice_da_tornare.push_back(riga_f);
	}

	bool trovato = false;
	for (int j = 0; j < id_passengers.size(); j++) {
		trovato = false;
		for (int i = 0; i < flightlegs.size(); i++) {
			if (matrice_da_tornare[i][j] == 1) trovato = true;
		}
		if (!trovato) {
			cout << " Passeggiero nuermo j " << j << " con codice passeggiero = " << id_passengers[j] << " non ha route " << endl;
			cin.get();
		}

	}
	return matrice_da_tornare;


}

//creazione matrici per il depot
vector<vector<int>> creazione_matrice_for_depot(map<int, Airplane>& map_airplane, vector<Flightleg>& flightlegs_1, vector<Flightleg>& flightlegs_2) {

	vector<vector<int>> matrice_da_tornare;

	for (Flightleg& f1 : flightlegs_1) {
		vector<int> riga_f;
		for (Flightleg& f2 : flightlegs_2) {
			if (f2.to == map_airplane[f1.cod_airplane].depot) {
				riga_f.push_back(1);
			}
			else {
				riga_f.push_back(0);
			}
		}
		matrice_da_tornare.push_back(riga_f);
	}

	return matrice_da_tornare;
}


void Print_Model_Result(vector<vector<bool>>& R, vector<vector<bool>>& S, vector<vector<bool>>& T, vector <Flightleg> flight_leg_day1, vector <Flightleg> flight_leg_day2, vector <Flightleg> flight_leg_day3) {
	cout << " Stampo variabili X  .. " << endl;
	for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
		for (size_t j = 0; j < flight_leg_day2.size(); j++) if (R[i][j] > 0.01) cout << "Variabile   x[" << i << "," << j << "] = " << R[i][j] << endl;
	}

	cout << endl;
	cout << " Stampo variabili Y .. " << endl;
	for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
		for (size_t k = 0; k < flight_leg_day3.size(); k++) if (S[j][k] > 0.01) cout << "Variabile   y[" << j << "," << k << "] = " << S[j][k] << endl;
	}

	cout << endl;
	cout << " Stampo variabili Z  .. " << endl;
	for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
		for (size_t k = 0; k < flight_leg_day3.size(); k++) if (T[i][k] > 0.01) cout << "Variabile   z[" << i << "," << k << "] = " << T[i][k] << endl;
	}
}


void creazione_route_three_Days_for_evaluation_Cost(vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed, map<int, Airplane>& map_airplane, vector<Route>& route_day1, vector<Route>& route_day2, vector<Route>& route_day3, vector<vector<bool>>& x_i_j, vector<vector<bool>>& y_j_k, vector<vector<bool>>& z_i_k, vector<Flightleg>& flightlegs_day1, vector<Flightleg>& flightlegs_day2, vector<Flightleg>& flightlegs_day3, vector<vector<vector<double>>>& costo_connessione) {

	cout << "day 1: " << flightlegs_day1.size() << endl;
	cout << "day 2: " << flightlegs_day2.size() << endl;
	cout << "day 3: " << flightlegs_day3.size() << endl;
	double costo = 0.0;
	double costo_connessione_km = 0.0;
	double costo_connessione_fuel = 0.0;

	string check_per_j = ";";

	cout << "******************************************primo for con i j e dentro l'if con j e k*************************************************" << endl;
	for (int i = 0; i < (int)flightlegs_day1.size(); i++) {
		for (int j = 0; j < (int)flightlegs_day2.size(); j++) {

			if (x_i_j[i][j]) {
				cout << "i: " << i << "   j: " << j << endl;
				//qua c'è la connessione fra la route i e quella j
				if (!flightlegs_day1[i].flight_leg_fittizzio) {
					route_day1.push_back(flightlegs_day1[i].route_padre);
					flightlegs_day2[j].route_padre.airplane_day_before = true;
					route_day2.push_back(flightlegs_day2[j].route_padre);
					//per il costo***************************************************************************************************
					costo += costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day2[j].from];
					costo_connessione_km += from_to[flightlegs_day1[i].to][flightlegs_day2[j].from];
					costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day2[j].from];
					cout << "costo di connessione: " << costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day2[j].from] << endl;
					costo += flightlegs_day1[i].cost;
					cout << "costo flightleg1 " << flightlegs_day1[i].cost << endl;
					costo += flightlegs_day2[j].cost - map_airplane[flightlegs_day2[j].cod_airplane].fixed_cost;
					cout << "costo flightleg2 " << flightlegs_day2[j].cost << endl;
					cout << "costo fisso flightleg2 " << map_airplane[flightlegs_day2[j].cod_airplane].fixed_cost << endl;
					//***************************************************************************************************************


					//ora guardo quelli connessi attraverso j in k
					for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
						if (y_j_k[j][k]) {
							check_per_j += to_string(j) + ";";	//HO AGGIUNTO QUESTA RIGA PER NON CONTARE PIU' VOLTE Y_J_K
							cout << "j: " << j << "   k: " << k << endl;
							if (!flightlegs_day3[k].flight_leg_fittizzio) {
								flightlegs_day3[k].route_padre.airplane_day_before = true;
								route_day3.push_back(flightlegs_day3[k].route_padre);

								//per il costo***************************************************************************************************
								costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_km += from_to[flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								cout << "costo connessione" << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
								costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
								cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
								//***************************************************************************************************************
							}
						}
					}

				}
				else {
					route_day2.push_back(flightlegs_day2[j].route_padre);
					//per il costo***************************************************************************************************
					costo += flightlegs_day2[j].cost;
					cout << flightlegs_day2[j].cost << endl;
					//***************************************************************************************************************
					for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
						if (y_j_k[j][k]) {
							check_per_j += to_string(j) + ";";	//HO AGGIUNTO QUESTA RIGA PER NON CONTARE PIU' VOLTE Y_J_K
							if (!flightlegs_day3[k].flight_leg_fittizzio) {
								flightlegs_day3[k].route_padre.airplane_day_before = true;
								route_day3.push_back(flightlegs_day3[k].route_padre);
								//per il costo***************************************************************************************************
								costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_km += from_to[flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
								cout << "costo connessione " << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
								cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
								//***************************************************************************************************************
							}
						}
					}
				}

			}
			/*else {
				for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
					if (y_j_k[j][k]) {
						cout << "j: " << j << " k: " << k << endl;
						route_day2.push_back(flightlegs_day2[j].route_padre);
						//per il costo***************************************************************************************************
						costo += flightlegs_day2[j].cost;
						cout << flightlegs_day2[j].cost << endl;
						//***************************************************************************************************************
						if (!flightlegs_day3[k].flight_leg_fittizzio) {
							flightlegs_day3[k].route_padre.airplane_day_before = true;
							route_day3.push_back(flightlegs_day3[k].route_padre);
							//per il costo***************************************************************************************************
							costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
							cout << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
							costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
							cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
							//***************************************************************************************************************
						}
					}
				}



			}*/

		}
	}

	cout << "***************************secondo for con solo j e k che non sono stati beccati prima**********************************************" << endl;
	for (int j = 0; j < (int)flightlegs_day2.size(); j++) {
		for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
			if (y_j_k[j][k]) {
				string j_string = ";"+to_string(j)+";";
				size_t found = check_per_j.find(j_string);
				if (found == string::npos) {  //SE CI SONO DEI PROBLEMI GUARDA QUESTO IF
					cout << "j: " << j << " k: " << k << endl;
					route_day2.push_back(flightlegs_day2[j].route_padre);
					//per il costo***************************************************************************************************
					costo += flightlegs_day2[j].cost;
					cout << flightlegs_day2[j].cost << endl;
					//***************************************************************************************************************
					if (!flightlegs_day3[k].flight_leg_fittizzio) {
						flightlegs_day3[k].route_padre.airplane_day_before = true;
						route_day3.push_back(flightlegs_day3[k].route_padre);
						//per il costo***************************************************************************************************
						costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
						costo_connessione_km += from_to[flightlegs_day2[j].to][flightlegs_day3[k].from];
						costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
						cout << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
						costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
						cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
						//***************************************************************************************************************
					}
				}
			}
		}
		
	}

	

	cout << "il costo e' prima del secondo big for: " << costo << endl;

	cout << "***********************************************************************************SECONDO BIG FOR***********************************************************************************" << endl;

	for (int i = 0; i < (int)flightlegs_day1.size(); i++) {
		for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
			if (z_i_k[i][k]) {
				if (!flightlegs_day1[i].flight_leg_fittizzio) {
					route_day1.push_back(flightlegs_day1[i].route_padre);
					//per il costo***************************************************************************************************
					costo += flightlegs_day1[i].cost;
					cout << flightlegs_day1[i].cost << endl;
					//***************************************************************************************************************
					if (!flightlegs_day3[k].flight_leg_fittizzio) {
						flightlegs_day3[k].route_padre.airplane_day_before = true;
						route_day3.push_back(flightlegs_day3[k].route_padre);
						//per il costo***************************************************************************************************
						costo += costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day3[k].from];
						costo_connessione_km += from_to[flightlegs_day1[i].to][flightlegs_day3[k].from];
						costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day3[k].from];
						cout << "connessione " << costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day3[k].from] << endl;
						costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
						cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
						//***************************************************************************************************************
					}
				}
				else {
					if (!flightlegs_day3[k].flight_leg_fittizzio) {
						route_day3.push_back(flightlegs_day3[k].route_padre);
						//per il costo***************************************************************************************************
						costo += flightlegs_day3[k].cost;
						cout << flightlegs_day3[k].cost << endl;
						//***************************************************************************************************************
					}
				}

			}

		}
	}

	cout << "ecco il costo -------------------------------------> " << costo << endl;

	cout << "connessione km: " << costo_connessione_km << endl;
	cout << "connessione fuel: " << costo_connessione_fuel << endl;

}






//***********************************MAIN****************************
int main(int argc, char* argv[]) {


	//cout << argv[1] << endl;
	//cout << argv[2] << endl;
	//cout << argv[3] << endl;

	double peso_itermediate_stop = 10.0;  //valore che si da ad una sosta intermedia 
	double peso_transhipment = 20.0;  //valore che si da ad un transhipment
	double peso_TW = 1.0; //peso dato ad un minuto di Time Windows
	map<string, int> mappa_aereo_tipo;
	map<int, int> codice_aereo_tipo;
	map<int, int> tipo_numero;

	//**************************INPUT***********************************
	//creo la legenda
	//cout << "start legenga_ritroso" << endl;
	map<int, string> legenda_ritroso;
	map<string, int> legenda = fillLegenda("legenda.csv", legenda_ritroso);
	/*cout << "stampo la legenda" << endl;
	for (auto x : legenda) {
		cout << x.first << "----->" << x.second << "il ritroso e':  "<< x.second << " -----> " << legenda_ritroso[x.second] << endl;
	}
	system("pause");*/



	//reading of AIRPLANE.CSV and creation VECTOR OF AIRPLANES
	vector<Airplane> airplanes = fillAirplane("NewAirplaneAll.csv", legenda);
	Build_structure_Model(airplanes, mappa_aereo_tipo, codice_aereo_tipo, tipo_numero);
	//cout << "***********************Print airplane***********************" << endl;
	//for (auto f : airplanes) cout << "il modello dell'aereo e': " << f.model << endl;
	//system("pause");
	//reading of AIRSTRIP.CSV and creation VECTOR OF AIRSTRIPS
	vector<Airstrip> airstrips = fillAirstrip("NewAirstrip.csv", legenda);
	//cout << "***********************Print airstrips***********************" << endl;
	//for (auto f : airstrips) f.print();
	//reading of INSTANCE.CSV and creation VECTOR OF PASSENGER
	vector<Passenger> passengers_day1 = fillPassenger(argv[1], legenda);
	vector<Passenger> passengers_day2 = fillPassenger(argv[5], legenda);
	vector<Passenger> passengers_day3 = fillPassenger(argv[9], legenda);
	//CODICE DA SCOMMENTARE PER BAMBINI
	vector<Passenger> passengers_for_company_solution_day1;
	vector<Passenger> passengers_for_company_solution_day2;
	vector<Passenger> passengers_for_company_solution_day3;
	unione_children_INTELLIGENTE(passengers_day1, passengers_for_company_solution_day1);
	unione_children_INTELLIGENTE(passengers_day2, passengers_for_company_solution_day2);
	unione_children_INTELLIGENTE(passengers_day3, passengers_for_company_solution_day3);


	fix_key_passenger_for_regret(passengers_day1, passengers_day2, passengers_day3);


	map<int, Passenger> map_id_passenger_day1 = fillMapPassenger(passengers_day1);
	map<int, Passenger> map_id_passenger_day2 = fillMapPassenger(passengers_day2);
	map<int, Passenger> map_id_passenger_day3 = fillMapPassenger(passengers_day3);

	map<string, double> from_to_company = fillMatrixCompany("Matrix.csv", airstrips);
	//bulding of hashmap from_to reading the file Matrix.txt
	vector<vector<double>> from_to;
	fillMatrix(from_to, "Matrix.csv", airstrips);

	map<int, Airplane> map_airplane;
	for (auto f : airplanes) map_airplane.insert(make_pair(f.code, f));

	map<int, Airstrip> map_airstrip;
	for (auto f : airstrips) map_airstrip.insert(make_pair(f.code, f));
	//ora faccio la mappa che mi permette di avere come chiave ogni aereo/aeroporto, come valore il fuel necessario, dato quell'aereo per andare nell'aereoporto pi� vicino con quel fuel
	vector<vector<double>> location_fuel;
	fillLocation_fuel(location_fuel, airstrips, airplanes, from_to, map_airstrip);

	vector<vector<vector<double>>> from_to_FuelConsumed;
	fill_from_to_fuel_consumed(from_to_FuelConsumed, from_to, airplanes);
	map<string, double> from_to_fuel_consumed_string = fill_from_to_fuel_consumed_string(from_to_company, airplanes);



	vector<Passenger> passengers_per_casostrano_day1 = passengers_day1;
	vector<Passenger> passengers_per_casostrano_day2 = passengers_day2;
	vector<Passenger> passengers_per_casostrano_day3 = passengers_day3;


	double Company_Solution = calculationCostCompany_three_days(peso_TW, from_to_fuel_consumed_string, peso_transhipment, peso_itermediate_stop, argv[2], argv[6], argv[10], argv[3], argv[7], argv[11], airstrips, airplanes, passengers_for_company_solution_day1, passengers_for_company_solution_day2, passengers_for_company_solution_day3, from_to_company);
	cout << " Costo della soluzione della compagnia per i tre giorni = " << Company_Solution << endl;
	//system("pause");



	//ora creo la mappa che mi dice per ogni locazione dov'è il posto più vicino per fare benzina, se nel posto considerato posso fare benzina il valore sarà uguale alla chiave
	map < int, int> to_closeness_fuel = fill_to_closeness_fuel(from_to, airstrips);

	map <int, int> location_request_day2 = fill_location_request(passengers_day2);

	int best_location_day2 = best_location(location_request_day2);

	map <int, int> location_request_day3 = fill_location_request(passengers_day3);

	int best_location_day3 = best_location(location_request_day3);

	sort(passengers_day2.begin(), passengers_day2.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
		//return lhs.mean_arrival < rhs.mean_arrival;
	});

	sort(passengers_day3.begin(), passengers_day3.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
		//return lhs.mean_arrival < rhs.mean_arrival;
	});

	int best_location_4places_day2 = best_location_4place(location_request_day2, from_to, airstrips);
	//cout << "ecco la migliore locazione più vincina alle migliori 4 per il giorno 2: " << best_location_4places_day2 << endl;
	int best_location_4places_day3 = best_location_4place(location_request_day3, from_to, airstrips);
	//cout << "ecco la migliore locazione più vincina alle migliori 4 per il giorno 3: " << best_location_4places_day3 << endl;

	vector<vector<double>> min_fuel_day2;
	vector<vector<double>> min_fuel_day3;
	fuel_min_80_major_places(min_fuel_day2, from_to_FuelConsumed, location_request_day2, airplanes, airstrips);
	fuel_min_80_major_places(min_fuel_day3, from_to_FuelConsumed, location_request_day3, airplanes, airstrips);


	//creo la matrice per il costo della connessione
	vector<vector<vector<double>>> costo_connessione = matrice_connessione(from_to, from_to_FuelConsumed, airplanes, airstrips);
	double start_day = 360.0;
	double end_day = 1110.0;
	cout << "***********************End reading input quindi comincio a contare il tempo***********************" << endl;

	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//*******************************************************DA QUA INCOMINCIA IL CODICE DEL MAIN, PRIMA CI SONO SOLO STRUTTURE DATI***********************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************

	int numero_aerei = 20;


	int numero_aerei_1 = stoi(argv[4]);
	int numero_aerei_2 = stoi(argv[8]);
	int numero_aerei_3 = stoi(argv[12]);

	int Iter_FirstDo = 0;
	int iterMAX_FirstDo = 5;
	vector<vector<bool>> R;
	vector<vector<bool>> S;
	vector<vector<bool>> T;
	//risolvo il giorno 1
	vector<vector<Route>> vector_solution_for_FL_day1;
	vector<Route> best_route_day1;
	best_route_day1 = solution_one_day(vector_solution_for_FL_day1, Iter_FirstDo, iterMAX_FirstDo, numero_aerei_1, airplanes, peso_TW, peso_itermediate_stop, map_airplane,
		map_airstrip, from_to, location_fuel, from_to_FuelConsumed, passengers_per_casostrano_day1, from_to_company, legenda_ritroso, map_id_passenger_day1, passengers_day1, mappa_aereo_tipo,
		codice_aereo_tipo, tipo_numero);
	cout << " Ho numero di route pari a ... " << vector_solution_for_FL_day1.size() << endl;
	vector<Flightleg> flightlegs_day1 = creazione_flightleg(true, vector_solution_for_FL_day1, map_airplane, from_to_FuelConsumed, from_to);
	cout << " Ho numero di flightlegs pari a ... " << flightlegs_day1.size() << endl;
	cout << " --------------->>>>>>>>>>>> Finito il primo giorno <<<<<<<<<<< -----------------------------------------" << endl;
	system("pause");
	//risolvo il giorno 2
	vector<vector<Route>> vector_solution_for_FL_day2;
	vector<Route> best_route_day2;
	best_route_day2 = solution_one_day(vector_solution_for_FL_day2, Iter_FirstDo, iterMAX_FirstDo, numero_aerei_2, airplanes, peso_TW, peso_itermediate_stop, map_airplane,
		map_airstrip, from_to, location_fuel, from_to_FuelConsumed, passengers_per_casostrano_day2, from_to_company, legenda_ritroso, map_id_passenger_day2, passengers_day2, mappa_aereo_tipo,
		codice_aereo_tipo, tipo_numero);
	cout << " Ho numero di route pari a ... " << vector_solution_for_FL_day2.size() << endl;
	vector<Flightleg> flightlegs_day2 = creazione_flightleg(false, vector_solution_for_FL_day2, map_airplane, from_to_FuelConsumed, from_to);
	cout << " --------------->>>>>>>>>>>> Finito il secondo giorno <<<<<<<<<<< -----------------------------------------" << endl;
	cout << " Ho numero di flightlegs pari a ... " << flightlegs_day2.size() << endl;
	system("pause");
	//risolvo il giorno 3
	vector<vector<Route>> vector_solution_for_FL_day3;
	vector<Route> best_route_day3;
	best_route_day3 = solution_one_day(vector_solution_for_FL_day3, Iter_FirstDo, iterMAX_FirstDo, numero_aerei_3, airplanes, peso_TW, peso_itermediate_stop, map_airplane,
		map_airstrip, from_to, location_fuel, from_to_FuelConsumed, passengers_per_casostrano_day3, from_to_company, legenda_ritroso, map_id_passenger_day3, passengers_day3, mappa_aereo_tipo,
		codice_aereo_tipo, tipo_numero);
	cout << " Ho numero di route pari a ... " << vector_solution_for_FL_day3.size() << endl;
	vector<Flightleg> flightlegs_day3 = creazione_flightleg(true, vector_solution_for_FL_day3, map_airplane, from_to_FuelConsumed, from_to);
	cout << " Ho numero di flightlegs pari a ... " << flightlegs_day3.size() << endl;
	cout << " --------------->>>>>>>>>>>> Finito il terzo giorno <<<<<<<<<<< -----------------------------------------" << endl;
	system("pause");

	//creo le strutture per il modello

	vector<vector<int>> A = creazione_matrice_connection_FL(start_day, end_day, flightlegs_day1, flightlegs_day2, map_airplane, from_to, from_to_FuelConsumed);
	vector<vector<int>> B = creazione_matrice_connection_FL(start_day, end_day, flightlegs_day2, flightlegs_day3, map_airplane, from_to, from_to_FuelConsumed);
	vector<vector<int>> C = creazione_matrice_connection_FL(start_day, end_day, flightlegs_day1, flightlegs_day3, map_airplane, from_to, from_to_FuelConsumed);
	vector<vector<int>> FL_pass_day1 = creazione_matrice_flightleg_passengers(flightlegs_day1, passengers_day1);
	vector<vector<int>> FL_pass_day2 = creazione_matrice_flightleg_passengers(flightlegs_day2, passengers_day2);
	vector<vector<int>> FL_pass_day3 = creazione_matrice_flightleg_passengers(flightlegs_day3, passengers_day3);
	vector<vector<int>> mat_depot_1_2 = creazione_matrice_for_depot(map_airplane, flightlegs_day1, flightlegs_day2);
	vector<vector<int>> mat_depot_1_3 = creazione_matrice_for_depot(map_airplane, flightlegs_day1, flightlegs_day3);
	vector<vector<int>> mat_depot_2_3 = creazione_matrice_for_depot(map_airplane, flightlegs_day2, flightlegs_day3);
	cout << " -------------------->>> CHIAMO IL MODELLO CHE ABBIAMO CREATO <<<<-------------------------------------" << endl;
	system("pause");
	//chiamo il modello
	Model_Connection *modello_connsessio = new Model_Connection(mat_depot_1_2, mat_depot_2_3, mat_depot_1_3, costo_connessione, A, B, C, flightlegs_day1, flightlegs_day2, flightlegs_day3, FL_pass_day1, FL_pass_day2, FL_pass_day3);
	bool infeasible = modello_connsessio->create_Model_connection(R,S,T,(int)passengers_day1.size(), (int)passengers_day2.size(), (int)passengers_day3.size(), numero_aerei);
	cout << " -------------------->>> FINITO LA CHIAMATA DEL MODELLO  <<<<-------------------------------------" << endl;
	if(!infeasible) Print_Model_Result(R, S, T, flightlegs_day1, flightlegs_day2, flightlegs_day3);
	
	//ora creo le route dei tre giorni
	vector<Route> route_finali_day1;
	vector<Route> route_finali_day2;
	vector<Route> route_finali_day3;
	cout << "sto iniziando la creazione delle route con la valutazione del loro costo" << endl;
	creazione_route_three_Days_for_evaluation_Cost(from_to, from_to_FuelConsumed,  map_airplane, route_finali_day1, route_finali_day2, route_finali_day3, R, S, T, flightlegs_day1, flightlegs_day2, flightlegs_day3, costo_connessione);
	
	
	
	
	cout << "QUESTA CHE RIPORTO è UNA STAMPA PER VALUTARE SE SONO STATE PRESE TUTTE LE ROUTE DEI DIVERSI GIORNI" << endl;
	cout << "*********************************ecco il giorno 1*********************************" << endl;
	int num_passenger_day1 = 0;
	for (Route r : route_finali_day1) {
		r.print();
		num_passenger_day1 += r.passengers_in_route.size();
	}
	cout << "num_pass ---  pass_veri day1" << endl;
	cout << num_passenger_day1 << " --- " << passengers_day1.size() << endl;

	cout << "*********************************ecco il giorno 2*********************************" << endl;
	int num_passenger_day2 = 0;
	for (Route r : route_finali_day2) {
		r.print();
		num_passenger_day2 += r.passengers_in_route.size();
	}
	cout << "num_pass ---  pass_veri day2" << endl;
	cout << num_passenger_day2 << " --- " << passengers_day2.size() << endl;
	cout << "*********************************ecco il giorno 3*********************************" << endl;
	int num_passenger_day3 = 0;
	for (Route r : route_finali_day3) {
		r.print();
		num_passenger_day3 += r.passengers_in_route.size();
	}
	cout << "num_pass ---  pass_veri day3" << endl;
	cout << num_passenger_day3 << " --- " << passengers_day3.size() << endl;
	
	
	
	
	cout << "il costo e': " <<
		calculate_ObjectiveFunction_complete_after_rolling(peso_TW, peso_itermediate_stop, route_finali_day1, route_finali_day2, route_finali_day3,
			map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	

	//ECCO LE STAMPE FINALI
	double stampa_compagnia = calculationCostCompany_three_days(peso_TW, from_to_fuel_consumed_string, peso_transhipment, peso_itermediate_stop, argv[2], argv[6], argv[10], argv[3], argv[7], argv[11], airstrips, airplanes, passengers_for_company_solution_day1, passengers_for_company_solution_day2, passengers_for_company_solution_day3, from_to_company);
	double stampa_nostra = calculate_ObjectiveFunction_complete_after_rolling(peso_TW, peso_itermediate_stop, route_finali_day1, route_finali_day2, route_finali_day3,
		map_airstrip, map_airplane, from_to, from_to_FuelConsumed);




	cout << "fine del main" << endl;
	system("pause");
	return 0;
}
