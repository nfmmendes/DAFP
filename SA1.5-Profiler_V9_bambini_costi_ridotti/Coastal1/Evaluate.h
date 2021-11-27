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
		int min_dep;
		int min_arr;

		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');

			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');

			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		//p.print();
		passengers_solution.push_back(p);

	}
	file.close();
}

double calculationCostCompany(PenaltyWeights penalty_weights, string route_azienza, string passengers_azienda, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers, map<string, double>& from_to_company) {
	double peso_TW = penalty_weights.time_window;
	double peso_trashipment = penalty_weights.peso_transhipment;
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
	for (int i = 0; i < (int)airstrips.size(); i++) {
		//cout << "airstrip code: " << airstrips[i].code_string << endl;
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
	}
	//look if the airstrips map are well saved

	map<string, Airplane> airplanes_map;
	for (int i = 0; i < (int)airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
	}

	for (int r = 0; r < (int)routes_company_solution.size(); r++) {
		Airplane* airplane = &airplanes_map[routes_company_solution[r].aircraft_code_company_comparison];
		Route* route = &routes_company_solution[r];
		double c = airplane->fixed_cost; //ho aggiunto il costo fisso
		final_cost_fissi += airplane->fixed_cost;
		string places = ";";
		
		for (int i = 0; i < (int)route->places_company.size(); i++) {
			places += route->places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				c += airstrips_map[route->places_company[i]].landing_cost; //aggiungo il leading cost
				final_cost_landing += airstrips_map[route->places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < (int)route->places_company.size() - 1) {
				c += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				final_cost_km += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];

				double time_flight = (from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]]) / airplanes_map[route->aircraft_code_company_comparison].speed;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplane->fuel_burn_first;
				}
				else {
					cost_fuel = airplane->fuel_burn_first + (time_flight - 1) * airplane->fuel_burn_second;
				}

				c += cost_fuel;
				final_cost_fuel += cost_fuel;
			}
		}

		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
	}


	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing per la compagnia: " << costo_routing << endl;

	vector<Passenger> passengers_solution; //il code_flight qui ? l'aereo

	write_on_file(passengers_azienda, passengers_solution);
	
	//calcolo matrice A e costo della penalit? per essere fuori dall'orario previsto
	for (int p = 0; p < (int)passengers.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
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
					if (differenza_dep > 25) {
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						c += (differenza_dep + 5) * (-1);
					}

				}
				if (pass_trovato[z].arrival_location_company == passengers[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						c += (differenza_arr) * (-1);
					}
				}
			}
		}

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to
			//scorro tutte le localit? della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == passengers[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == passengers[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {

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
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
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

				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;
				numeroTrashiment++;
			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
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

				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;
				numeroTrashiment++;
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			numeroTrashiment++;
			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to
			//scorro tutte le localit? della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

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

			costi_intermediate_stop += (peso_intermediate_stop * (best_to1 - best_from1 - 1));
			//*******************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to
			//scorro tutte le localit? della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			costi_intermediate_stop += (peso_intermediate_stop * (best_to2 - best_from2 - 1));
			//*************************************************************************************************************
		}
		else if (controllo > 2) {
			cout << "c'? un problema con il passeggero: ";
			passengers[p].print();
			cout << "in quanto trova pi? di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}

		costi_time_windows += c;
	}

	costi_time_windows = costi_time_windows * peso_TW; //per valutare cosa succede al cambiare del peso dato alle time windows
	final_cost_IS = costi_intermediate_stop;
	final_cost_TW = costi_time_windows;

	double costo_Soluzione = costi_time_windows + costi_intermediate_stop;
	for (int i = 0; i < (int)cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
	}

	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;

	cout << final_cost_fissi << ";" << final_cost_landing << ";" << final_cost_fuel << ";" << final_cost_km << ";" << final_cost_IS << ";"
	<< final_cost_TW << ";" << costo_Soluzione << ";" << final_cost_fissi / airplanes_map["SUN"].fixed_cost << ";" << final_cost_fuel << ";"
	<< final_cost_km << ";" << ((final_cost_IS - (numeroTrashiment * peso_trashipment)) / peso_intermediate_stop) << ";" << (final_cost_TW / peso_TW) << ";";
	return costo_Soluzione;


}

void calculate_ObjectiveFunction_final(ProcessedInput*input, double costo_company, const PenaltyWeights& penalty_weights, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	double cost = 0.0;
	double costo_fisso = 0.0;
	double costo_km = 0.0;
	double costo_fuel = 0.0;
	double costo_landing = 0.0;
	double costo_Time_Window = 0.0;
	double costo_Intermediate = 0.0;

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
			if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
			else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			costo_Time_Window += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
	}

	double minuti_TW = (costo_Time_Window / peso_TW);
	double n_intermediate = (costo_Intermediate / peso_intermediate_stop);
	double gap_perc = ((cost - costo_company) / costo_company) * 100;
	cout << costo_fisso << ";" << costo_landing << ";" << costo_fuel << ";" << costo_km << ";" << costo_Intermediate << ";" << costo_Time_Window << ";" << minuti_TW << ";" << n_intermediate << ";" << gap_perc << ";";

}

void calculate_ObjectiveFunction_final_arc_iori(ProcessedInput* input, double costo_company, const PenaltyWeights& penalt_weights, vector<Route>& solution) {
	double peso_TW = penalt_weights.time_window;
	double peso_intermediate_stop = penalt_weights.intermediate_stop;
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

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
			cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			costo_Intermediate += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;


			double TW_departure = 0.0;
			if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
			else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			costo_Time_Window += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
	}

	double minuti_TW = (costo_Time_Window / peso_TW);
	double n_intermediate = (costo_Intermediate / peso_intermediate_stop);
	double gap_perc = ((cost - costo_company) / costo_company) * 100;
	cout << costo_fisso << ";" << costo_landing << ";" << costo_fuel << ";" << costo_km << ";" << costo_Intermediate << ";" << costo_Time_Window << ";" << minuti_TW << ";" << n_intermediate << ";" << gap_perc << ";";
}

double cost_single_route(ProcessedInput* input, const PenaltyWeights& penalty_weights, Route& r) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	map<int, Airplane> map_airplane = input->get_map_airplane();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	//double cost = 0.0;
	double cost = map_airplane[r.aircraft_code].fixed_cost;

	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	for (int i = 0; i < r.index; i++) {
		//double time_travel = 0.0;
		if (i >= 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
		}

		if (i < r.index - 1) {
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}
		}
	}

	cost += mileage;
	cost += fuel_consumed;

	for (const auto& p : r.passengers_in_route) {

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];
		cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

		double TW_departure = 0.0;
		if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
		else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;
		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double calculate_ObjectiveFunction(ProcessedInput* input, const  PenaltyWeights&penalty_weights, vector<Route>& solution) {
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
		if (r.primo_pass) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}

		double mileage = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				cost_route += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
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
			cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

			double TW_departure = 0.0;
			if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
			else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
	}

	return cost;
}

double cost_time_windows_for_route(Route& r, double peso_TW) {
	double cost = 0.0;

	for (const Passenger& p : r.passengers_in_route) {
		
		double TW_departure = 0.0;
		if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
		else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

		cost += (TW_departure + TW_arrival) * p.capacity;
	}

	cost = cost * peso_TW;

	return cost;
}

double cost__for_route_passenger_destroyCluster(Route& r, Passenger& p, int fattore_inter_stop, double peso_TW) {
	double cost = 0.0;

	double TW_departure = 0.0;
	if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
	if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW + fattore_inter_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;

	return cost;
}

double cost_time_windows_for_node(Route& r, vector<Passenger>& pass, double peso_TW) {
	double cost = 0.0;
	for (const Passenger& p : pass) {

		double TW_departure = 0.0;
		if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
		else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double cost_time_windows_for_route_passenger(Route& r, Passenger& p, double peso_TW) {
	double cost = 0.0;

	double TW_departure = 0.0;
	if (r.time_dep[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_dep[p.solution_from];
	else if (r.time_dep[p.solution_from] > p.late_departure) TW_departure = r.time_dep[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

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
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		cost += map_airplane[r.aircraft_code].fixed_cost;
		cost_route += map_airplane[r.aircraft_code].fixed_cost;
		//cout << "costo fisso: " << cost << endl;
		for (int i = 1; i < r.index; i++) {
			cost += map_airstrip[r.places[i]].landing_cost;
			cost_route += map_airstrip[r.places[i]].landing_cost;
		}

		//second of all calculate the cost of the Km and the fuel burn
		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;
		for (int i = 0; i < r.index - 1; i++) {
			//double time_travel = 0.0;
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}


		}
		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;
		//now i have to calculate the penalitis regarding the time windows for each passeger
	}

	return cost;
};

double costo_time_windows_and_intermediate_stop(const PenaltyWeights& penalty_weights, vector<Route>& solution) {
	double peso_TW = penalty_weights.time_window;
	double peso_intermediate_stop = penalty_weights.intermediate_stop;
	double cost = 0.0;

	for (auto& r : solution) {

		for (const auto& p : r.passengers_in_route) {
			double time_departure = r.time_arr[p.solution_from];
			double time_arrival = r.time_arr[p.solution_to];

			double TW_departure = 0.0;
			if (time_departure < p.early_departure) TW_departure = p.early_departure - time_departure;
			else if (time_departure > p.late_departure) TW_departure = time_departure - p.late_departure;

			double TW_arrival = 0.0;
			if (time_arrival < p.early_arrival) TW_arrival = p.early_arrival - time_arrival;
			else if (time_arrival > p.late_arrival) TW_arrival = time_arrival - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost += (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;
		}
	}

	return cost;
}

#endif
