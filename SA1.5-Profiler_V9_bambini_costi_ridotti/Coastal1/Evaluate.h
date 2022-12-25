#ifndef EVALUATE_H
#define EVALUATE_H

#include "Fill.h"
#include "Passenger.h"
#include "Route.h"
#include "Util.h"
#include "ProcessedInput.h"

void write_on_file(string passengers_azienda, vector<Passenger> &passengers_solution)
{
	ifstream file;
	
	file.open(passengers_azienda);
	if (file.fail()) {
		cerr << "Error Opening File passenger azienda" << endl;
		system("pause");
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		int min_dep = 0;
		int min_arr = 0;

		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');

			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');

			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution.push_back(p);
	}
	
	file.close();
}

void do_work_1(double peso_intermediate_stop, double& costi_intermediate_stop, const vector<int>& int_from, const vector<int>& int_to)
{
	double best_differenza = DBL_MAX;
	int best_from = -1;
	int best_to = -1;
	for (int from : int_from) {
		for (int to : int_to) {
			if (to > from) {
				double diff = to - from;
				if (diff < best_differenza) {
					best_differenza = diff;
					best_from = from;
					best_to = to;
				}
			}
		}
	}

	costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1));
}

double get_cost_intermediate_stop(double peso_intermediate_stop, vector<int> int_from1, vector<int> int_to1)
{
	double best_differenza1 = DBL_MAX;
	int best_from1 = -1;
	int best_to1 = -1;
	for (int from : int_from1) {
		for (int to : int_to1) {
			if (to > from) {
				double diff = to - from;
				if (diff < best_differenza1) {
					best_differenza1 = diff;
					best_from1 = from;
					best_to1 = to;
				}
			}
		}
	}

	return peso_intermediate_stop * (best_to1 - best_from1 - 1);
}

double get_cost_intermediate_stop(Route* route, const string &departure_location, const string& arrival_location, double peso)
{
	vector<int> int_from1; //vettore con tutti i from
	vector<int> int_to1; // vettore con tutti i to

	for (int i = 0; i < route->index; i++) {
		if (route->get_places_company()[i] == departure_location) {
			int_from1.push_back(i);
		}
		if (route->get_places_company()[i] == arrival_location) {
			int_to1.push_back(i);
		}

		return get_cost_intermediate_stop(peso, int_from1, int_to1);
	}
}

void do_work_control_1(PenaltyWeights penalty_weights, vector<Passenger> passengers, map<string, Route> codice_routeAzienda, int& numeroTrashiment, double& costi_intermediate_stop, int p, vector<Passenger> pass_trovato)
{
	double peso_trashipment = penalty_weights.transhipment;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	vector<int> int_from; //vettore con tutti i from
	vector<int> int_to; // vettore con tutti i to
	//scorro tutte le localit? della route
	auto code_flight = pass_trovato[0].code_flight;
	for (int i = 0; i < codice_routeAzienda[code_flight].index; i++) {
		//salvo tutti i from e tutti i to che trovo
		if (codice_routeAzienda[code_flight].get_places_company()[i] == passengers[p].departure_location_company) {
			int_from.push_back(i);
		}
		if (codice_routeAzienda[code_flight].get_places_company()[i] == passengers[p].arrival_location_company) {
			int_to.push_back(i);
		}
	}

	if (!int_from.empty() && !int_to.empty()) 
		do_work_1(peso_intermediate_stop, costi_intermediate_stop, int_from, int_to);
	else if (int_from.empty() && !int_to.empty()) {
		//qua ha fatto un transhipment sul from
		//devo cercare tutti i from partendo dal from sulla soluzione
		vector<int> int_from_soluz;
		for (int i = 0; i < codice_routeAzienda[code_flight].index; i++) {
			if (codice_routeAzienda[code_flight].get_places_company()[i] == pass_trovato[0].departure_location_company)
				int_from_soluz.push_back(i);
		}

		do_work_1(penalty_weights.intermediate_stop, costi_intermediate_stop, int_to, int_from_soluz);
		costi_intermediate_stop += +penalty_weights.transhipment;
		numeroTrashiment++;
	}
	else if (!int_from.empty() && int_to.empty()) {
		//qua ha fatto un transhipment sul to
		//devo cercare tutti i to partendo dal from sulla soluzione
		vector<int> int_to_soluz;
		for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
			if (codice_routeAzienda[pass_trovato[0].code_flight].get_places_company()[i] == pass_trovato[0].arrival_location_company) {
				int_to_soluz.push_back(i);
			}
		}

		costi_intermediate_stop += get_cost_intermediate_stop(peso_intermediate_stop, int_from, int_to_soluz)+ peso_trashipment;
		numeroTrashiment++;
	}
}

double calculationCostCompany(PenaltyWeights penalty_weights, string route_azienza, string passengers_azienda, 
							  vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers, map<string, double>& from_to_company) {
	double peso_TW = penalty_weights.time_window;
	double peso_trashipment = penalty_weights.transhipment;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;

	vector<Route> routes_company_solution = fillRoute(route_azienza);

	map <string, Route> codice_routeAzienda;
	for (Route r : routes_company_solution)
		codice_routeAzienda.insert(make_pair(r.aircraft_code_company_comparison, r));

	int numeroTrashiment = 0;
	double final_cost_fissi = 0.0;
	double final_cost_landing = 0.0;
	double final_cost_km = 0.0;
	double final_cost_fuel = 0.0;
	double final_cost_IS = 0.0;
	double final_cost_TW = 0.0;

	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;

	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < (int)airstrips.size(); i++)
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
	//look if the airstrips map are well saved

	map<string, Airplane> airplanes_map;
	for (int i = 0; i < (int)airplanes.size(); i++)
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));

	for (int r = 0; r < (int)routes_company_solution.size(); r++) {
		Airplane* airplane = &airplanes_map[routes_company_solution[r].aircraft_code_company_comparison];
		Route* route = &routes_company_solution[r];
		double c = airplane->fixed_cost; //ho aggiunto il costo fisso
		final_cost_fissi += airplane->fixed_cost;
		string places = ";";
		
		for (int i = 0; i < (int)route->get_places_company().size(); i++) {
			places += route->get_places_company()[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				c += airstrips_map[route->get_places_company()[i]].landing_cost; //aggiungo il leading cost
				final_cost_landing += airstrips_map[route->get_places_company()[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < (int)route->get_places_company().size() - 1) {
				string key = route->get_places_company()[i] + ";" + route->get_places_company()[i + 1];
				c += from_to_company[key];
				final_cost_km += from_to_company[key];

				double time_flight = airplanes_map[route->aircraft_code_company_comparison].travelTime((from_to_company[key]));
				double cost_fuel = airplane->fuel_burn_first + max(0.0 , (time_flight - 1) * airplane->fuel_burn_second);
				
				c += cost_fuel;
				final_cost_fuel += cost_fuel;
			}
		}

		cost_route.push_back(c);
	}

	double costo_routing = 0.0;
	for (double c : cost_route) 
		costo_routing += c;
	cout << "Costo Routing per la compagnia: " << costo_routing << endl;

	vector<Passenger> passengers_solution; //il code_flight qui ? l'aereo

	write_on_file(passengers_azienda, passengers_solution);
	
	//calcolo matrice A e costo della penalit? per essere fuori dall'orario previsto
	for (int p = 0; p < (int)passengers.size(); p++) {
		int passenger_tw_cost = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		//TODO: remove this transform
		transform(passengers[p].name.begin(), passengers[p].name.end(), passengers[p].name.begin(), ::tolower);
		transform(passengers[p].surname.begin(), passengers[p].surname.end(), passengers[p].surname.begin(), ::tolower);

		for (int j = 0; j < (int)passengers_solution.size(); j++) {
			trovato_pass_name = passengers_solution[j].name.find(passengers[p].name);
			if (trovato_pass_name <= passengers_solution[j].name.size()) {
				trovato_pass_cognome = passengers_solution[j].name.find(passengers[p].surname);
				if (trovato_pass_cognome <= passengers_solution[j].name.size()) {
					pass_trovato.push_back(passengers_solution[j]);
					controllo += 1;
				}
			}
		}

		//questa ? la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < (int)pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers[p].departure_time - pass_trovato[z].departure_time;
					
					passenger_tw_cost += max(0, differenza_dep - 25);

					if (differenza_dep < -5) {
						passenger_tw_cost -= (differenza_dep + 5);
					}
				}
				if (pass_trovato[z].arrival_location_company == passengers[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						passenger_tw_cost += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						passenger_tw_cost -= (differenza_arr);
					}
				}
			}
		}

		//parte per il calcolo del costo degli intermediate stop********************************************
		if (controllo == 1) {
			do_work_control_1(penalty_weights,
							  passengers,
							  codice_routeAzienda,
							  numeroTrashiment,
							  costi_intermediate_stop,
							  p,
							  pass_trovato);
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			numeroTrashiment++;

			Route* route_pass_trovato_zero = &codice_routeAzienda[pass_trovato[0].code_flight];
			Route* route_pass_trovato_one = &codice_routeAzienda[pass_trovato[1].code_flight];

			//*********************************primo passeggero trovato************************************************************
			costi_intermediate_stop +=  get_cost_intermediate_stop(route_pass_trovato_zero, pass_trovato[0].departure_location_company, 
																   pass_trovato[0].arrival_location_company, peso_intermediate_stop);

			//*********************************secondo pezzo del passeggero trovato**********************************************
			costi_intermediate_stop += get_cost_intermediate_stop(route_pass_trovato_one, pass_trovato[1].departure_location_company, 
										pass_trovato[1].arrival_location_company, peso_intermediate_stop);
		}

		costi_time_windows += passenger_tw_cost;
	}

	costi_time_windows = costi_time_windows * peso_TW; //per valutare cosa succede al cambiare del peso dato alle time windows
	final_cost_IS = costi_intermediate_stop;
	final_cost_TW = costi_time_windows;

	double costo_Soluzione = costi_time_windows + costi_intermediate_stop;
	for (int i = 0; i < (int)cost_route.size(); i++)
		costo_Soluzione += cost_route[i];
	
	return costo_Soluzione;
}

void calculate_ObjectiveFunction_final(ProcessedInput*input, double cost_company, const PenaltyWeights& penalty_weights, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	double cost = 0.0;
	double fixed_cost = 0.0;
	double km_cost = 0.0;
	double fuel_cost = 0.0;
	double landing_cost = 0.0;
	double time_window_cost = 0.0;
	double stop_cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;

		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
			fixed_cost += map_airplane[r.aircraft_code].fixed_cost;
		}

		double mileage = 0.0;
		double fuel_consumed = 0.0;
		for (int i = 0; i < r.index; i++) {
			const auto curr_airstrip = r.get_airstrips()[i];
			const auto next_airstrip = r.get_airstrips()[i + 1];
			Airstrip* airstrip = &map_airstrip[curr_airstrip];
			if (i >= 1) {
				cost += airstrip->landing_cost;
				cost_route += airstrip->landing_cost;
				landing_cost += airstrip->landing_cost;
			}
			if (i < r.index - 1) {
				mileage += from_to[curr_airstrip][next_airstrip];
				km_cost += from_to[curr_airstrip][next_airstrip];
					
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][curr_airstrip][next_airstrip];
				fuel_cost += from_to_FuelConsumed[r.aircraft_code][curr_airstrip][next_airstrip];
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (const auto& p : r.get_passengers()) {
			cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			stop_cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;

			const auto departure = r.get_departure_at(p.solution_from);
			const auto arrival = r.get_arrival_at(p.solution_to);
			const auto TW_departure = max(0.0, p.early_departure - departure) + max(0.0, departure - p.late_departure);
			const auto TW_arrival = max(0.0, p.early_arrival - arrival) + max(0.0, arrival- p.late_arrival);

			double deviation_cost = (TW_departure + TW_arrival) * peso_TW * p.capacity;
			cost += deviation_cost;
			cost_route += deviation_cost;
			time_window_cost += deviation_cost;
		}

		r.cost = cost_route;
	}
}

void calculate_ObjectiveFunction_final_arc_iori(ProcessedInput* input, double costo_company, 
	const PenaltyWeights& penalt_weights, vector<Route>& solution) {
	double peso_TW = penalt_weights.time_window;
	double peso_intermediate_stop = penalt_weights.intermediate_stop;
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	double cost = 0.0;

	double fixed_cost = 0.0;
	double km_cost = 0.0;
	double fuel_cost = 0.0;
	double landing_cost = 0.0;
	double time_window_cost = 0.0;
	double stop_cost = 0.0;

	for (Route& r : solution) {
		auto origin = r.get_airstrips()[r.index - 1];
		if (origin != 1 || origin != 4) {
			Airplane* airplane = &map_airplane[r.aircraft_code];
			
			int depot = airplane->depot;
			double maxFuel = airplane->max_fuel;
			int groundTime = map_airstrip[depot].ground_time;
			double arrivalTime = r.get_departures()[r.index - 1] + airplane->travelTime(from_to[origin][depot]);

			r.addPlace(depot, 1, { maxFuel, double(airplane->max_weight - maxFuel), 0 }, arrivalTime, arrivalTime + groundTime);
		}

		r.print();
	}

	for (auto& r : solution) {
		double cost_route = 0.0;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass) {
			Airplane* airplane = &map_airplane[r.aircraft_code];
			cost += airplane->fixed_cost;
			cost_route += airplane->fixed_cost;
			fixed_cost += airplane->fixed_cost;
		}

		double mileage = 0.0;
		double fuel_consumed = 0.0;

		for (int i = 0; i < r.index; i++) {
			const auto curr_airstrip = r.get_airstrips()[i];
			const auto next_airstrip = r.get_airstrips()[i + 1];
			
			if (i >= 1) {
				cost += map_airstrip[curr_airstrip].landing_cost;
				cost_route += map_airstrip[curr_airstrip].landing_cost;
				landing_cost += map_airstrip[curr_airstrip].landing_cost;
			}
			if (i < r.index - 1) {
				mileage += from_to[curr_airstrip][next_airstrip];
				km_cost += from_to[curr_airstrip][next_airstrip];
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][curr_airstrip][next_airstrip];
				fuel_cost += from_to_FuelConsumed[r.aircraft_code][curr_airstrip][next_airstrip];
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (const auto& p : r.get_passengers()) {
			cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;
			stop_cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;

			const auto departure = r.get_departures()[p.solution_from];
			const auto arrival = r.get_arrival_at(p.solution_to);
			const auto TW_departure = max(0.0, p.early_departure - departure) + max(0.0, departure - p.late_departure);
			const auto TW_arrival = max(0.0, p.early_arrival - arrival) + max(0.0, arrival - p.late_arrival);

			const double deviations_cost = (TW_arrival + TW_departure) * peso_TW * p.capacity;
			cost += deviations_cost;
			cost_route += deviations_cost;
			time_window_cost += deviations_cost;
		}

		r.cost = cost_route;
	}
}

double cost_single_route(ProcessedInput* input, const PenaltyWeights& penalty_weights, const Route& r) {
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	double cost = map_airplane[r.aircraft_code].fixed_cost;

	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	for (int i = 0; i < r.index; i++) {
		if (i >= 1)
			cost += map_airstrip[r.get_airstrips()[i]].landing_cost;
		
		if (i < r.index - 1) {
			mileage += from_to[r.get_airstrips()[i]][r.get_airstrips()[i + 1]];
			fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.get_airstrips()[i]][r.get_airstrips()[i + 1]];
		}
	}

	cost += mileage;
	cost += fuel_consumed;

	for (const auto& p : r.get_passengers()) {
		cost += ((p.solution_to - p.solution_from - 1) * penalty_weights.intermediate_stop) * p.capacity; 

		double departure = r.get_departures()[p.solution_from];
		double arrival = r.get_arrivals()[p.solution_to];
		double TW_departure = max(0.0, p.early_departure - departure) + max(0.0, departure - p.late_departure);
		double TW_arrival = max(0.0, p.early_arrival - arrival) + max(0.0, arrival - p.late_arrival);
		
		cost += ((TW_departure + TW_arrival) * penalty_weights.time_window) * p.capacity;
	}

	return cost;
}

double calculate_objective_function(ProcessedInput* input, const  PenaltyWeights&penalty_weights, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;

	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass)
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
		

		double mileage = 0.0;
		double fuel_consumed = 0.0;

		for (int i = 0; i < r.index; i++) {

			if (i >= 1)
				cost_route += map_airstrip[r.get_airstrips()[i]].landing_cost;
			
			if (i < r.index - 1) {
				mileage += from_to[r.get_airstrips()[i]][r.get_airstrips()[i + 1]];
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.get_airstrips()[i]][r.get_airstrips()[i + 1]];
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage + fuel_consumed;
		cost_route += mileage + fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (const auto& p : r.get_passengers()) {
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

			double departure = r.get_departures()[p.solution_from];
			double arrival = r.get_arrivals()[p.solution_to];
			double TW_departure = max(0.0, p.early_departure - departure) + max(0.0, departure - p.late_departure);
			double TW_arrival = max(0.0, p.early_arrival - arrival) + max(0.0, arrival - p.late_arrival);

			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
		cost += cost_route; 
	}

	return cost;
}

double cost_time_windows_for_route_passenger(const Route& r, const Passenger& p, double peso_TW) {
	double cost = 0.0;

	double departure = r.get_departures()[p.solution_from];
	double arrival = r.get_arrivals()[p.solution_to];
	double TW_departure = max(0.0, p.early_departure - departure) + max(0.0, departure - p.late_departure);
	double TW_arrival = max(0.0, p.early_arrival - arrival) + max(0.0, arrival - p.late_arrival);

	cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

	return cost;
}


inline double cost_time_windows_for_route(const Route& r, double peso_TW) {
	double cost = 0.0;

	for (const Passenger& p : r.get_passengers())
		cost += cost_time_windows_for_route_passenger(r, p, peso_TW);
	
	return cost;
}

double cost_for_route_passenger_destroyCluster(Route& r, const Passenger& p, int inter_stop_factor, double peso_TW) {

	double departure = r.get_departures()[p.solution_from];
	double arrival = r.get_arrivals()[p.solution_to];
	double TW_departure = max(0.0, p.early_departure - departure) + max(0.0, departure - p.late_departure);
	double TW_arrival = max(0.0, p.early_arrival - arrival) + max(0.0, arrival - p.late_arrival);

	return ((TW_departure + TW_arrival) * peso_TW + inter_stop_factor * (p.solution_to - p.solution_from - 1)) * p.capacity;
}

double cost_time_windows_for_node(Route& r, vector<Passenger>& pass, double peso_TW) {
	double cost = 0.0;
	for (const Passenger& p : pass) {
		cost += cost_time_windows_for_route_passenger(r, p, peso_TW);
	}

	return cost;
}

double costo_senza_time_windows(ProcessedInput* input, vector<Route>& solution) {
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		Airplane* airplane = &map_airplane[r.aircraft_code];
		auto airstrips = r.get_airstrips(); 
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		cost += airplane->fixed_cost;
		cost_route += airplane->fixed_cost;
		
		for (int i = 1; i < r.index; i++) {
			cost += map_airstrip[airstrips[i]].landing_cost;
			cost_route += map_airstrip[airstrips[i]].landing_cost;
		}

		//second of all calculate the cost of the Km and the fuel burn
		double mileage = 0.0;
		double fuel_consumed = 0.0;
		for (int i = 0; i < r.index - 1; i++) {
			mileage += from_to[airstrips[i]][airstrips[i + 1]];
			fuel_consumed += from_to_FuelConsumed[r.aircraft_code][airstrips[i]][airstrips[i + 1]];	
		}
		
		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;
		//now i have to calculate the penalities regarding the time windows for each passenger
	}

	return cost;
};

double costo_time_windows_and_intermediate_stop(const PenaltyWeights& penalty_weights, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	double cost = 0.0;

	for (auto& r : solution) {

		for (const auto& p : r.get_passengers()) {
			double time_departure = r.get_arrivals()[p.solution_from];
			double time_arrival = r.get_arrivals()[p.solution_to];

			double TW_departure = max(0.0, p.early_departure - time_departure) + max(0.0, time_departure - p.late_departure);
			double TW_arrival = max(0.0, p.early_arrival - time_arrival) + max(0.0, time_arrival - p.late_arrival);

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost += (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;
		}
	}
	
	return cost;
}

#endif
