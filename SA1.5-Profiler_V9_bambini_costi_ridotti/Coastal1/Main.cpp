#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>
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
#include "Model_Cplex.h"
#include "Util.h"
#include "HeuristicConstructive.h"
#include "Input.h"
#include "UtilFill.h"

using namespace std;




//const int numero_airstrip_const = 58;
//const int numero_airplane_const = 36;


double Ob_Funct_Improvement(double newsolution, double oldsolution) {
	double rate_max_improvement = 10000;
	return (newsolution >= oldsolution) ? 0 : ((oldsolution - newsolution) / rate_max_improvement);
}

double Time_Spend(double time) {
	double rate_time = 20;
	return time / rate_time;
}


vector<Passenger> read_passengers_solution(string passengers_azienda)
{
	vector<Passenger> passengers_solution; 
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
	return passengers_solution;
}

//********************************************************FUNZIONI CHE CALCOLANO I COSTI*******************************************************************************
double calculationCostCompany(Penalty_Weights& penalty_weights, string route_azienza, string passengers_azienda, Input &input, map<string, double>& from_to_company)
 {

	double peso_TW = penalty_weights.time_window_weight;
	double peso_trashipment = penalty_weights.transhipment_weight;
	double peso_intermediate_stop = penalty_weights.stop_weight;
	const vector<Airplane> airplanes = input.get_airplanes();
	const vector<Airstrip> airstrips = input.get_airstrips();
	vector<Passenger> passengers = input.get_passengers(); 
	
	vector<Route> routes_company_solution = fillRoute(route_azienza);

	map <string, Route> codice_routeAzienda;
	for (Route r : routes_company_solution) {
		//r.printCompany();
		codice_routeAzienda.insert(make_pair(r.aircraft_code_company_comparison, r));
	}

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
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
	}

	map<string, Airplane> airplanes_map;
	for (int i = 0; i < (int)airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
	}

	for (int r = 0; r < (int)routes_company_solution.size(); r++) {
		const auto route = routes_company_solution[r];
		const auto aircraftCode = route.aircraft_code_company_comparison;
		
		double c = airplanes_map[aircraftCode].fixed_cost; //ho aggiunto il costo fisso
		final_cost_fissi += airplanes_map[aircraftCode].fixed_cost;
		string places = ";";
		for (int i = 0; i < (int)route.places_company.size(); i++) {
			places += route.places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				c += airstrips_map[route.places_company[i]].landing_cost; //aggiungo il leading cost
				final_cost_landing += airstrips_map[route.places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < (int)route.places_company.size() - 1) {
				c += from_to_company[route.places_company[i] + ";" + route.places_company[i + 1]];
				final_cost_km += from_to_company[route.places_company[i] + ";" + route.places_company[i + 1]];
				
				double time_flight = (from_to_company[route.places_company[i] + ";" + route.places_company[i + 1]]) / airplanes_map[aircraftCode].speed;
				double cost_fuel = 0;
				//TODO: Generalize it in a function
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[aircraftCode].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[aircraftCode].fuel_burn_first + (time_flight - 1) * airplanes_map[aircraftCode].fuel_burn_second;
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
	vector<Passenger> passengers_solution = read_passengers_solution(passengers_azienda);

	//***********************************************************************************************************************************
	//calcolo matrice A e costo della penalit? per essere fuori dall'orario previsto
	for (int p = 0; p < (int)passengers.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;

		string name, surname; 
		transform(passengers[p].name.begin(), passengers[p].name.end(), name.begin(), ::tolower);
		transform(passengers[p].surname.begin(), passengers[p].surname.end(),surname.begin(), ::tolower);


		for (int j = 0; j < (int)passengers_solution.size(); j++) {
			trovato_pass_name = passengers_solution[j].name.find(name);
			if (trovato_pass_name <= passengers_solution[j].name.size()) {
				trovato_pass_cognome = passengers_solution[j].name.find(surname);
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

		//parte per gli intermediate stop

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
				if (int_from_soluz.empty()) {
					//cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					//system("pause");
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
				if (int_to_soluz.empty()) {
					//cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					//system("pause");
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
			else {
				//cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'? un problema con il passeggero: ";
				//passengers[p].print();
				//cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				//system("pause");
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
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				//cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				//pass_trovato[0].print();
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
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
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

			if (int_from2.empty() || int_to2.empty()) {
				//cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				//pass_trovato[1].print();
				//system("pause");
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
			//****************************************************************************************************************************************************************
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

	cout << final_cost_fissi << ";" << final_cost_landing << ";" << final_cost_fuel << ";" << final_cost_km << ";" << final_cost_IS << ";" << final_cost_TW << ";" << costo_Soluzione << ";" << final_cost_fissi / airplanes_map["SUN"].fixed_cost << ";" << final_cost_fuel << ";" << final_cost_km << ";" << ((final_cost_IS - (numeroTrashiment*peso_trashipment)) / peso_intermediate_stop) << ";" << (final_cost_TW / peso_TW) << ";";
	return costo_Soluzione;


}




void calculate_ObjectiveFunction_final(double costo_company, Penalty_Weights & weights,  vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, Double3DVector & from_to_FuelConsumed) {

	double peso_TW = weights.time_window_weight; 
	double peso_intermediate_stop = weights.stop_weight; 

	double cost = 0.0;
	double costo_fisso = 0.0;
	double costo_km = 0.0;
	double costo_fuel = 0.0;
	double costo_landing = 0.0;
	double costo_Time_Window = 0.0;
	double costo_Intermediate = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;

		auto& aircraft_code = r.aircraft_code;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass) {
			cost += map_airplane[aircraft_code].fixed_cost;
			cost_route += map_airplane[aircraft_code].fixed_cost;
			costo_fisso += map_airplane[aircraft_code].fixed_cost;
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
			double increase_cost = ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;
			cost += increase_cost;   //riga aggiunta per le intermediate stop
			cost_route += increase_cost;  //riga aggiunta per le intermediate stop
			costo_Intermediate += increase_cost;


			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) 
				TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) 
				TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			increase_cost = ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost += increase_cost; 
			cost_route += increase_cost;
			costo_Time_Window += increase_cost;
		}

		r.cost = cost_route;
	}

	double minuti_TW = (costo_Time_Window / peso_TW);
	double n_intermediate = (costo_Intermediate / peso_intermediate_stop);
	double gap_perc = ((cost - costo_company) / costo_company) * 100;
	cout << costo_fisso << ";" << costo_landing << ";" << costo_fuel << ";" << costo_km << ";" << costo_Intermediate << ";" << costo_Time_Window << ";" << minuti_TW << ";" << n_intermediate << ";" << gap_perc << ";";

}


double cost_single_route(Penalty_Weights &penalty_weights, const Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	double peso_TW = penalty_weights.time_window_weight; 
	double peso_intermediate_stop = penalty_weights.stop_weight;
	
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
	//cost += first_hour * map_airplane[r.aircraft_code].fuel_burn_first + second_hour * map_airplane[r.aircraft_code].fuel_burn_second;
	cost += fuel_consumed;

	for (const auto& p : r.passengers_in_route) {

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];
		cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) 
			TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) 
			TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;

		if (r.time_arr[p.solution_to] < p.early_arrival) 
			TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) 
			TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double calculate_ObjectiveFunction(Penalty_Weights weights, vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	double peso_TW = weights.time_window_weight; 
	double peso_intermediate_stop = weights.stop_weight;
	
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
			if (r.time_arr[p.solution_from] < p.early_departure) 
				TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) 
				TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) 
				TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) 
				TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
	}

	return cost;
}

double cost__for_route_passenger_destroyCluster(const Route& r, const Passenger& p, int fattore_inter_stop, double peso_TW) {
	double cost = 0.0;
	
	double TW_departure = 0.0;
	if (r.time_arr[p.solution_from] < p.early_departure) 
		TW_departure = p.early_departure - r.time_arr[p.solution_from];
	if (r.time_arr[p.solution_from] > p.late_departure) 
		TW_departure = r.time_arr[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) 
		TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	if (r.time_arr[p.solution_to] > p.late_arrival) 
		TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW + fattore_inter_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;

	return cost;
}

double cost_time_windows_for_node(const Route& r, vector<Passenger>& pass, double peso_TW) {
	double cost = 0.0;
	for (const Passenger& p : pass) {

		//TODO: Create a unique function to this
		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) 
			TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) 
			TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) 
			TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) 
			TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double cost_time_windows_for_route_passenger(const Route& r, const Passenger& p, double peso_TW) {
	double cost = 0.0;
	
	double TW_departure = 0.0;
	if (r.time_arr[p.solution_from] < p.early_departure) 
		TW_departure = p.early_departure - r.time_arr[p.solution_from];
	else if (r.time_arr[p.solution_from] > p.late_departure) 
		TW_departure = r.time_arr[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) 
		TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	else if (r.time_arr[p.solution_to] > p.late_arrival) 
		TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

	return cost;
}



void handleError_relateness_passenger(int idP)
{
	cout << " ******************************* ERRORE ***********************************" << endl;
	cout << " Problema con il passeggiero con Pnr: " << idP << endl;
	cin.get();
}

bool relateness_passenger2(int trheshold, vector<vector<double>>& from_to, int idP, Passenger& r, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequenza, string pointer, int y, map<int, Passenger>& map_id_passenger) {
	// Poi questo for si potra togliere
	Passenger pass = map_id_passenger[idP];

	// Questo poi va cambiato
	if (pass.name.size() == 0) {
		handleError_relateness_passenger(idP); 
	}
	double Relateness = from_to[r.arrival_location][pass.arrival_location] + from_to[r.departure_location][pass.departure_location] + 
								abs((double)r.arrival_time - pass.arrival_time) + abs((double)r.departure_time - pass.departure_time);

	if (Relateness <= trheshold) {

		for (int j : agr_pass[pointer]) 
			passengers_removed.push_back(map_id_passenger[j]);
		sequenza.erase(sequenza.begin() + y);
		agr_pass.erase(agr_pass.find(pointer));
		
		return true;
	}

	return false;
}




bool route_feasible(Route& r, map<int, Airplane>& map_airplane, double end_day, vector<vector<double>>& location_fuel, Double3DVector & from_to_FuelConsumed) {

	//check and day
	auto aux_fuel = (location_fuel[r.aircraft_code][r.places[r.index - 1]] + map_airplane[r.aircraft_code].min_fuel);
	if (r.time_arr[r.index - 1] > end_day  || r.quantity_fuel[r.index - 1] < aux_fuel)
		return false;

	for (int i = 0; i < r.index; i++) {
		if (i >= 1) {
			//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
			if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) 
				return false;
			
			if (r.refueling[i] && !r.refueling[i - 1]) {
				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
				if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel)
					return false;
			}
		}
		
		if (i < r.index - 1) 
			if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) 
				return false;
	}

	//check of the intermediate stop
	for (Passenger& p : r.passengers_in_route) 
		if (p.solution_to - p.solution_from > p.stop) 
			return false;

	return true;
}

map<int, int> Compute_WorstNode(Penalty_Weights &weights, const Route& route, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
	//double costoWorstNode = 0.00;
	//int WorstNode = -1;
	double peso_TW = weights.time_window_weight; 
	double peso_intermediate_stop = weights.stop_weight; 
	
	map<double, int> Node;
	map<int, int> NodeOrder;
	set<double, MyCOMP<double>> Myset;
	if (route.index <= 2) {
		Node.insert(make_pair(100000, 1));
		Myset.insert(100000);
	}
	else {
		for (int n = 1; n < route.index; n++) {
			double dist = 0.0;
			double cost_IS = 0.0;
			vector<Passenger> PassengerNodo;
			if (n != (route.index - 1)) {

				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]] + from_to[route.places[n]][route.places[n + 1]] - from_to[route.places[n - 1]][route.places[n + 1]];
				for (int p = 0; p < (int)route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n || route.passengers_in_route[p].solution_from == n)
						PassengerNodo.push_back(route.passengers_in_route[p]);

					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) 
						if (route.passengers_in_route[p].solution_to > n) 
							cost_IS += (peso_intermediate_stop)* route.passengers_in_route[p].capacity;
				}
			}
			else {
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]];
				for (int p = 0; p < (int)route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n) 
						PassengerNodo.push_back(route.passengers_in_route[p]);
					
					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n)
						if (route.passengers_in_route[p].solution_to > n) 
							cost_IS += (peso_intermediate_stop)* route.passengers_in_route[p].capacity;
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


void destroy_route(Route & r, map<int, Airplane>& map_airplane)
{
	// qua devo distruggere tutta la route e lasciare solo il depot
	for (int i = (int)(r.places.size()) - 1; i >= 1; i--) {

		r.places.erase(r.places.begin() + i);
		r.refueling.erase(r.refueling.begin() + i);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + i);
		r.time_arr.erase(r.time_arr.begin() + i);
		r.time_dep.erase(r.time_dep.begin() + i);
		r.capacity.erase(r.capacity.begin() + i);
		r.weight.erase(r.weight.begin() + i);
		r.index--;
	}

	//qua ci dovrebbe essere solo il deposito
	//fisso il peso, capacita e fuel ai valori di default e la variabile passeggeri dentro a false
	r.quantity_fuel[0] = map_airplane[r.aircraft_code].max_fuel;
	r.weight[0] = map_airplane[r.aircraft_code].weight_fuel_people - r.quantity_fuel[0];
	r.capacity[0] = 0;
	r.primo_pass = false;
}


//ECCO I NOSTRI DESTROY
vector<Route> destroy_thanos(double destroy_coef_route, vector<Passenger>& passenger_removed, Solution& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, Double3DVector & from_to_FuelConsumed) {


	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {

			//genero il numero random di nodi da distruggere da 2 a place.size()-1;
			double numero_random = (double)rand() / RAND_MAX;
			if (r.places.size() == 2) {
				numero_random = round(1 + (numero_random * (r.places.size() - 2)));
				//numero_random = 0;
			}
			else {
				numero_random = round(2 + (numero_random * (r.places.size() - 3)));
			}

			if (numero_random == r.places.size() - 1) {
				destroy_route(r, map_airplane);
				
				//ora devo togliere tutti i passeggeri
				for (auto p = r.passengers_in_route.size() - 1; p >= 0; p--) {
					r.passengers_in_route[p].route_before = index;
					passenger_removed.push_back(r.passengers_in_route[p]);
					r.passengers_in_route.erase(r.passengers_in_route.begin() + p);
				}

			}
			else {
				//qua devo pescare un numero di nodi da distruggere pari a numero_random
				int nodi_rimossi = 0; //finch? non ho rimosso 
				do {
					bool check = true;
					do {
						//if i'm here i can destroy the route
						//at the moment i destroy only a node
						int node_destroy = (int)(round(1 + ((double)rand() / RAND_MAX) * (r.index - 2)));

						if (node_destroy == r.index - 1) { //this in the case of that node is at the end
							check = false;
							
							vector<int> int_removed;
							

							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t] -= r.passengers_in_route[p].capacity;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}
							for (auto i = int_removed.size() - 1; i >= 0; i--) {
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							
							int index_min_from = 0;
							for (int i = Min_From_Pass; i >= 0; i--) {
								if (r.refueling[i] && i != node_destroy) {
									index_min_from = i;
									break;
								}
							}

							for (int k = index_min_from; k < Max_To_Pass; k++) {
								if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
								//cout << " Sto valutando il caso del nodo " << k << endl;
									int Node_min = k;
									double min_weight = r.weight[k];
									int index_updating_from = k;
									int index_updating_to = r.index;  //qua prima c'era -1
									for (int i = k + 1; i <= Max_To_Pass; i++) {
										//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
										
										if (r.refueling[i]) break;
										if (r.weight[i] < min_weight && i != node_destroy) {
											min_weight = r.weight[i];
											Node_min = i;
										}
										//}
									}
									if (Node_min >= 0) {
										for (int i = k + 1; i < r.index; i++) {
											if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
												index_updating_to = i;
												break;
											}
										}

										double Fuel_before = r.quantity_fuel[index_updating_from];
										r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
										r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
										for (int i = index_updating_from + 1; i < index_updating_to; i++) {
											if (i != node_destroy) {
												r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
												r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
											}
										}
									}
								}

							}

							int nodi_mancanti = (int)(r.places.size());
							r.removePlace(node_destroy, map_airplane);
							nodi_mancanti -= (int)r.places.size();
							nodi_rimossi += nodi_mancanti;
						}
						else {

							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];

							if (fuel_consumed <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
								check = false;
								vector<int> int_removed;
								int Min_From_Pass = node_destroy;
								int Max_To_Pass = node_destroy;
								for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
									if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
										if (r.passengers_in_route[p].solution_from < Min_From_Pass) 
											Min_From_Pass = r.passengers_in_route[p].solution_from;
										if (r.passengers_in_route[p].solution_to > Max_To_Pass)
											Max_To_Pass = r.passengers_in_route[p].solution_to;
										int_removed.push_back(p);
										for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
											r.capacity[t] -= r.passengers_in_route[p].capacity;
											r.weight[t] += r.passengers_in_route[p].weight;
										}

									}
								}

								for (int i = int_removed.size() - 1; i >= 0; i--) {
									// code for repair forbidden***********************************************************
									r.passengers_in_route[int_removed[i]].route_before = index;
									//*************************************************************************************
									passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
									vector<Passenger>::iterator it;
									it = r.passengers_in_route.begin();
									r.passengers_in_route.erase(it + int_removed[i]);
								}

								int nodi_mancanti = (int)r.places.size();
								r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
								//double fuel_consumed1 = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy]];
								//double fuel_consumed2 = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy]][r.places[node_destroy + 1]];

								int index_before = node_destroy - 1;
								double diff = 0;

								for (int i = node_destroy + 1; i < r.index; i++) {

									if (r.refueling[i]) break;
									if (index_before == (node_destroy - 1)) {

										diff = r.quantity_fuel[i];
										r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
										diff = diff - r.quantity_fuel[i];
									}
									else {
										r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
									}

									//TODO: Possibly an error
									if (r.refueling[node_destroy]) {
										r.weight[i] = r.weight[i] + diff;
									}
									else {
										r.weight[i] = r.weight[i] + diff;
									}

									//cout << "weight after: " << r.weight[i] << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

									//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
									//cout << " inidce before = " << index_before << endl;
									index_before = i + 1;
								}
								//*****************************************************************************************************************************************************************
								r.removePlace(node_destroy, map_airplane);
								nodi_mancanti -= (int)(r.places.size());
								nodi_rimossi += (int)(nodi_mancanti);
								//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weight[j] < 0) {
										add_fuel = r.weight[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.refueling[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}

										for (int t = index_refueling; t < r.index; t++) {
											if (r.refueling[t] && t != index_refueling) break;
											r.quantity_fuel[t] += add_fuel;
											r.weight[t] -= add_fuel;
										}
									}
								}
							}
						}
					} while (check);
				} while (nodi_rimossi < numero_random);
			}

		}
		index++;
	}
	return solution;
}

vector<Route> destroy_casual(double destroy_coef_route, vector<Passenger>& passenger_removed, Solution& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, Double3DVector& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);		//TODO: check why 10
		
		if (f <= destroy_coef_route && r.index > 1) {
			bool check = true;
			do {
				//if i'm here i can destroy the route
				//at the moment i destroy only a node
				double n_destroy = (double)rand() / RAND_MAX;
				int node_destroy = (int)(round(1 + n_destroy * (r.index - 2)));
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					vector<int> int_removed;

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) 
								Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
								/*
								if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
								}
								*/
							}


						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						r.passengers_in_route[int_removed[i]].route_before = index;
						//*************************************************************************************

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}

					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					// Codice che serve per cercare il minimo nel range
					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								if (r.refueling[i]) break;
							
								if (r.weight[i] < min_weight && i != node_destroy) {
									min_weight = r.weight[i];
									Node_min = i;
								}
							}

							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								double Fuel_before = r.quantity_fuel[index_updating_from];
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);

								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);

					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {

							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
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
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {   //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
						check = false;
						vector<int> int_removed;
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
							if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
								if (r.passengers_in_route[p].solution_from < Min_From_Pass) 
									Min_From_Pass = r.passengers_in_route[p].solution_from;
								if (r.passengers_in_route[p].solution_to > Max_To_Pass) 
									Max_To_Pass = r.passengers_in_route[p].solution_to;
								int_removed.push_back(p);
								for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
									r.capacity[t] -= r.passengers_in_route[p].capacity;
									r.weight[t] += r.passengers_in_route[p].weight;
									/*
									if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
									}
									*/
								}

							}
						}

						for (int i = int_removed.size() - 1; i >= 0; i--) {
							// code for repair forbidden***********************************************************
							r.passengers_in_route[int_removed[i]].route_before = index;
							//*************************************************************************************
							passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
							vector<Passenger>::iterator it;
							it = r.passengers_in_route.begin();
							r.passengers_in_route.erase(it + int_removed[i]);
						}
						r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time

						int index_before = node_destroy - 1;
						double diff = 0;

						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i]) 
								break;
							if (index_before == (node_destroy - 1)) {
								diff = r.quantity_fuel[i];
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];

							}
							else {
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
							}

							r.weight[i] = r.weight[i] + diff;
							index_before = i + 1;

						}
						//*****************************************************************************************************************************************************************


						r.removePlace(node_destroy, map_airplane);
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {

								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}

								for (int t = index_refueling; t < r.index; t++) {
									if (r.refueling[t] && t != index_refueling) break;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
								}
							}
						}

						//r.print();
					}
				}

			} while (check);
		}
		
		index++;
	}
	return solution;
}

vector<Route> destroy_worst(Penalty_Weights weights, double destroy_coef_route, vector<Passenger>& passenger_removed, Solution& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, Double3DVector& from_to_FuelConsumed) {

	double peso_TW = weights.time_window_weight;
	double peso_intermediate_stop = weights.stop_weight;
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		
		if (f <= destroy_coef_route && r.index > 1) {
			// Primo elemanto la posizione il secondo il numero del nodo
			map<int, int> Node;
			bool check = true;
			int first = 0;

			//TODO: change it to a while loop
			do {
				// Qui va messa una funzione ch individua il nodo peggiore
				//cout << " ************** Chiamo funzione che mi calcoli il nodo peggiore **************" << endl;
				Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 }; 
				Node = Compute_WorstNode(weights, r, map_airstrip, from_to);
				int node_destroy = Node[first];

				if (node_destroy == 0 || first >= (int)Node.size()) 
					break;
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					vector<int> int_removed;

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
							}
						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						r.passengers_in_route[int_removed[i]].route_before = index;
						//*************************************************************************************

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}
				
					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}

					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								if (r.refueling[i]) 
									break;
								if (r.weight[i] < min_weight && i != node_destroy) {
									min_weight = r.weight[i];
									Node_min = i;
								}
							}
							
							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								double Fuel_before = r.quantity_fuel[index_updating_from];
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);

								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {   //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
						check = false;
						vector<int> int_removed;
						
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
							if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
								if (r.passengers_in_route[p].solution_from < Min_From_Pass) 
									Min_From_Pass = r.passengers_in_route[p].solution_from;
								if (r.passengers_in_route[p].solution_to > Max_To_Pass) 
									Max_To_Pass = r.passengers_in_route[p].solution_to;
								int_removed.push_back(p);
								for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
									r.capacity[t] -= r.passengers_in_route[p].capacity;
									r.weight[t] += r.passengers_in_route[p].weight;
									/*
									if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
									}
									*/
								}

							}
						}

						for (int i = int_removed.size() - 1; i >= 0; i--) {
							// code for repair forbidden***********************************************************
							r.passengers_in_route[int_removed[i]].route_before = index;
							//*************************************************************************************

							passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
							vector<Passenger>::iterator it;
							it = r.passengers_in_route.begin();
							r.passengers_in_route.erase(it + int_removed[i]);
						}
						r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time


						int index_before = node_destroy - 1;
						double diff = 0;
						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i]) 
								break;
							
							if (index_before == (node_destroy - 1)) {
								diff = r.quantity_fuel[i];
								
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];
							}
							else {
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
							}

							//TODO: possibly wrong
							if (r.refueling[node_destroy]) {
								r.weight[i] = r.weight[i] + diff;
							}
							else {
								r.weight[i] = r.weight[i] + diff;
							}

							index_before = i + 1;
						}
						//*****************************************************************************************************************************************************************


						r.removePlace(node_destroy, map_airplane);
						
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {
						
								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
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
					}
				}

				first++;
			} while (check);
		}
		index++;
	}
	return solution;
}

vector<Route> destroy_cluster_aggr2(Penalty_Weights weights, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<Passenger> all_passenegr, map<int, Passenger>& map_id_passenger) {

	double peso_TW = weights.time_window_weight;
	double& peso_itermediate_stop = weights.stop_weight;

	int soglia_relateness = 300; // Prima era 100
	vector<int> int_removed;
	vector<Route> route_destroyed;
	vector<Passenger> passengers;

	unordered_map<int, double> CostTWPass;
	set<double, MyCOMP<double>> Myset;

	route_destroyed.insert(route_destroyed.end(), solution.begin(), solution.end());
	passengers.insert(passengers.end(), all_passenegr.begin(), all_passenegr.end());
	//for (Passenger& p : all_passenegr) passengers.push_back(p);

	for (Passenger& p : passengers) CostTWPass.insert(make_pair(p.pnr, 0));
	for (Route& s : route_destroyed) {
		for (Passenger& pass : s.passengers_in_route) {
			int Codpass = pass.pnr;
			CostTWPass[Codpass] += cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW);
			Myset.insert(cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW));
		}
	}
	vector<string> OrderVectore;
	for (auto x : Myset) {
		for (auto y : CostTWPass) {
			if (y.second == x) {
				OrderVectore.push_back(to_string(y.first) + "|" + to_string(x));
			}
		}
	}
	
	map<string, vector<int>> agr_pass;
	vector<string> sequenza;
	for (string x : OrderVectore) {
		
		int codice = stoi(split(x, '|')[0]);
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[codice].departure_location) + "|" + to_string(map_id_passenger[codice].arrival_location);
		if (agr_pass.find(code) != agr_pass.end())
		{
			// c'? gia
			agr_pass[code].push_back(map_id_passenger[codice].pnr);
		}
		else {
			// non c'? gia
			sequenza.push_back(code);
			vector<int> id_passeggieri;
			id_passeggieri.push_back(map_id_passenger[codice].pnr);
			agr_pass.insert(make_pair(code, id_passeggieri));
		}

	}

	for (int x = 0; x < (int)sequenza.size(); x++) {
		int Codpass = agr_pass[sequenza[x]][0];
		
		// tolgo tutti di questa aggregazione
		for (int p : agr_pass[sequenza[x]]) passenger_removed.push_back(map_id_passenger[p]);
		agr_pass.erase(agr_pass.find(sequenza[x]));
		sequenza.erase(sequenza.begin() + x);
		
		for (int y = 0; y < (int)sequenza.size(); y++) {
	
			// Qui adesso chimao la funzione Relateness !! 
			bool retrocedi = relateness_passenger2(soglia_relateness, from_to, Codpass, map_id_passenger[agr_pass[sequenza[y]][0]], passenger_removed, agr_pass, sequenza, sequenza[y], y, map_id_passenger);
			if (retrocedi) 
				y--;
		}
		
		x--;
		if ((int)passenger_removed.size() >= num_passenger) break;
	}

	//int NRimossi = 0;
	int Npass = 0;
	int Nroute = -1;
	
	// Devo toglierli dalla route 
	for (Route& s : route_destroyed) {
		Nroute++;
		vector<int> int_removed;
		bool Update = false;
		int Min_From_Pass = s.index;
		int Max_To_Pass = -1;
		for (int p = 0; p < (int)s.passengers_in_route.size(); p++) {
			for (Passenger& pass : passenger_removed) {
				//if (s.passengers_in_route[p].name == pass.name && s.passengers_in_route[p].surname == pass.surname && s.passengers_in_route[p].pnr == pass.pnr && s.passengers_in_route[p].code_flight == pass.code_flight && s.passengers_in_route[p].gender == pass.gender  && s.passengers_in_route[p].arrival_location == pass.arrival_location  && s.passengers_in_route[p].departure_location == pass.departure_location && s.passengers_in_route[p].weight == pass.weight) {
				if (s.passengers_in_route[p].pnr == pass.pnr) {
					Update = true;
					pass.route_before = Nroute;
					if (s.passengers_in_route[p].solution_from < Min_From_Pass) 
						Min_From_Pass = s.passengers_in_route[p].solution_from;
					if (s.passengers_in_route[p].solution_to > Max_To_Pass) 
						Max_To_Pass = s.passengers_in_route[p].solution_to;
		
					int_removed.push_back(p);
					for (int t = s.passengers_in_route[p].solution_from; t < s.passengers_in_route[p].solution_to; t++) {
						s.capacity[t] -= s.passengers_in_route[p].capacity;
						s.weight[t] += s.passengers_in_route[p].weight;
					}

				}
			}
		}
		
		for (int i = int_removed.size() - 1; i >= 0; i--) {
			vector<Passenger>::iterator it;
			it = s.passengers_in_route.begin();
			s.passengers_in_route.erase(it + int_removed[i]);
		}

		if (Update) {


			// Codice che da reduel ai nodi con refuel non massimo 
			int index_min_from = Min_From_Pass;
			for (int i = Min_From_Pass; i >= 0; i--) {
				if (s.refueling[i]) {
					index_min_from = i;
					break;
				}
			}
			for (int k = index_min_from; k < Max_To_Pass; k++) {
				if (s.refueling[k] && s.quantity_fuel[k] < map_airplane[s.aircraft_code].max_fuel) { //&& k!= node_destroy
					int index_updating_from = k;
					int index_updating_to = s.index;
					int Node_min = k;
					double min_weight = s.weight[k];
					//appena aggiunto
					for (int i = Max_To_Pass; i < s.index; i++) {
						if (s.refueling[i]) break;
						Max_To_Pass = i;
					}

					for (int i = k + 1; i <= Max_To_Pass; i++) {
						
						//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
						//cout << " Ho passato IF di controllo " << endl;
						if (s.refueling[i]) break;
						
						if (s.weight[i] < min_weight) {
							min_weight = s.weight[i];
							Node_min = i;
						}
						//}
					}
					if (Node_min >= 0) {
						for (int i = k + 1; i < s.index; i++) {
							if (s.refueling[i]) {  // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
								index_updating_to = i;
								break;
							}
						}

						double Fuel_before = s.quantity_fuel[index_updating_from];
						
						s.quantity_fuel[index_updating_from] = min(map_airplane[s.aircraft_code].max_fuel, s.quantity_fuel[index_updating_from] + min_weight);
						s.weight[index_updating_from] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						for (int i = index_updating_from + 1; i < index_updating_to; i++) {
							s.quantity_fuel[i] += (s.quantity_fuel[index_updating_from] - Fuel_before);
							s.weight[i] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						}
					}
				}
			}


			int index_sup = s.index;
			for (int i = index_sup - 1; i > 1; i--) {

				if (s.capacity[i - 1] != 0) break;

				if (s.capacity[i - 1] == 0) {
					s.places.erase(s.places.begin() + i);
					s.time_arr.erase(s.time_arr.begin() + i);
					s.time_dep.erase(s.time_dep.begin() + i);
					s.refueling.erase(s.refueling.begin() + i);
					s.quantity_fuel.erase(s.quantity_fuel.begin() + i);
					s.weight.erase(s.weight.begin() + i);
					s.capacity.erase(s.capacity.begin() + i);
					s.index = s.index - 1;

				}
			}


			if (s.index == 2 && s.capacity[0] == 0) {
				s.places.erase(s.places.begin() + 1);
				s.time_arr.erase(s.time_arr.begin() + 1);
				s.time_dep.erase(s.time_dep.begin() + 1);
				s.refueling.erase(s.refueling.begin() + 1);
				s.quantity_fuel.erase(s.quantity_fuel.begin() + 1);
				s.weight.erase(s.weight.begin() + 1);
				s.capacity.erase(s.capacity.begin() + 1);
				s.index = s.index - 1;
			}

			// Questa parte qua forse si puo togliere
			//qua ? la parte che ho aggiunto io (NELLI) per il problema del nodo al deposito che non si aggiorna
			if ((int)s.places.size() == 1 && s.capacity[0] == 0) {
				s.quantity_fuel[0] = map_airplane[s.aircraft_code].max_fuel;
				s.weight[0] = map_airplane[s.aircraft_code].weight_fuel_people - map_airplane[s.aircraft_code].max_fuel;
			}
		}
	}

	Npass = 0;
	for (Route& s : route_destroyed) {
		Npass += (int)(s.passengers_in_route.size());
		if (s.passengers_in_route.size() == 0) 
			s.primo_pass = false;
	}
	
	return route_destroyed;

}





//ECCO I NOSTRI REPAIR
vector <Route> repair_one(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		
		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			//routes_destroyed[r].print();
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'? solo il deposito in questo caso
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
				//int arc_from = -1;
				//int arc_to = -1;
				Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 }; 
				double cost_route_before = cost_single_route(weights, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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
							
								if (r_support.time_arr[r_support.index - 1] <= end_day) {
								
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									
									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
											double cost = (cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
										
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

					if (t_arr_arrival < p.early_arrival) 
						time_window_cost += p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) 
						time_window_cost += t_arr_arrival - p.late_arrival;

					double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
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
					if (t_arr_departure < p.early_departure) 
						TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) 
						TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) 
						TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) 
						TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
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

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}

		if (!routes_destroyed[best_route].primo_pass) {
			//i have to do the new things, 
			if (case_first_passenger == false) {
				//c'? solo il depot
				routes_destroyed[best_route].primo_pass = true;
				routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			//else {
				//ci sono pi? nodi
				//routes_destroyed[best_route].primo_pass = true;
				//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			//}
		}
		else {

			if (move_c) {
				//i have to do the move C
				routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			else {
				//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
				routes_destroyed[best_route] = route_best;
				p.solution_from = from_per_route;
				p.solution_to = to_per_route;
				//routes_destroyed[best_route].print();
			}
		}

		routes_destroyed[best_route].passengers_in_route.push_back(p);
		
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	
	return routes_destroyed;
	
}


vector<Route> two_regret_repair_aggragati(const Penalty_Weights &weights,  double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector & from_to_FuelConsumed) {

	double peso_TW = weights.time_window_weight;
	double peso_intermediate_stop = weights.stop_weight;
	
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


			//string chiave = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
			int chiave = p.chiave_regret;
			if (map_pass_aggragati.count(chiave) >= 1) {
				int index = map_pass_aggragati[chiave]; 
				regret_diff.push_back(regret_diff[index]);
				regret_move_c.push_back(regret_move_c[index]);
				regret_best_route.push_back(regret_best_route[index]);
				regret_arc_from.push_back(regret_arc_from[index]);
				regret_arc_to.push_back(regret_arc_to[index]);
				regret_index_pass.push_back(regret_index_pass[index]);


			}
			else {
				//Route r_support; //attento qua, ? quello di prova
				for (int r = 0; r < (int)routes_destroyed.size(); r++) {
					auto& loop_route = routes_destroyed[r];
					if (routes_destroyed[r].primo_pass == false) {
						if (routes_destroyed[r].index == 1) {
							//c'? solo il deposito in questo caso
							double cost = from_to[loop_route.places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[loop_route.aircraft_code].fixed_cost;
							cost += from_to_FuelConsumed[loop_route.aircraft_code][loop_route.places[0]][p.departure_location];
							cost += from_to_FuelConsumed[loop_route.aircraft_code][p.departure_location][p.arrival_location];

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
						Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 }; 
						double cost_route_before = cost_single_route(weights, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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

										if (r_support.time_arr[r_support.index - 1] <= end_day) {


											r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
													//TODO: use a struct to represent weight parameters
													Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
													double cost = (cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
													
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
							if (routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] < p.early_departure) 
								time_window_cost += p.early_departure - routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
							else if (routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] > p.late_departure) 
								time_window_cost += routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] - p.late_departure;

							if ((routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) < p.early_arrival) 
								time_window_cost += p.early_arrival - (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time);
							else if ((routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) > p.late_arrival) 
								time_window_cost += (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) - p.late_arrival;

							double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
							if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
								if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
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
							double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
							double TW_departure = 0.0;
							double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
							if (t_arr_departure < p.early_departure) 
								TW_departure = p.early_departure - t_arr_departure;
							else if (t_arr_departure > p.late_departure) 
								TW_departure = t_arr_departure - p.late_departure;

							double TW_arrival = 0.0;
							double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
							if (t_arr_arrival < p.early_arrival) 
								TW_arrival = p.early_arrival - t_arr_arrival;
							else if (t_arr_arrival > p.late_arrival) 
								TW_arrival = t_arr_arrival - p.late_arrival;
							cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

							//per il check sul fuel:
							//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
							double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
							double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

							cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
							//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
							//refuel
							double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
							double f_after_secondo_tratto = 0.0;
							if (map_airstrip[p.departure_location].fuel) {
								f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
							}
							else {
								f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
							}

							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
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
					//in questo caso vuol dire che la solutione ? infeasible
					cout << "LA SOLUZIONE E' INFEASIBLE" << endl;
					vector<Route> route_vuote;
					return route_vuote;

				}

				int val = (int)regret_diff.size() - 1;
				//string primo_val = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
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
			routes_destroyed[regret_best_route[index]].primo_pass = true;
			routes_destroyed[regret_best_route[index]].add_update_only_one_node_first_passanger(passengers_removed[regret_index_pass[index]], from_to, map_airplane, map_airstrip, from_to_FuelConsumed);

		}
		else {

			if (regret_move_c[index] == 1) {
				//i have to do the move C
				routes_destroyed[regret_best_route[index]].move_c(passengers_removed[regret_index_pass[index]], passengers_removed[regret_index_pass[index]].departure_location, passengers_removed[regret_index_pass[index]].arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			else {
				routes_destroyed[regret_best_route[index]].update_route_rebuilt_one(regret_arc_from[index], regret_arc_to[index], passengers_removed[regret_index_pass[index]].departure_location, passengers_removed[regret_index_pass[index]].arrival_location, from_to, map_airplane, map_airstrip, passengers_removed[regret_index_pass[index]], from_to_FuelConsumed);
				
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

	}
	
	return routes_destroyed;
}

vector <Route> repair_forbidden(const Penalty_Weights weights, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, Double3DVector & from_to_FuelConsumed) {

	double peso_TW = weights.time_window_weight;
	double peso_intermediate_stop = weights.stop_weight;
	
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
						//c'? solo il deposito in questo caso
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
					//int arc_from = -1;
					//int arc_to = -1;
					Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
					double cost_route_before = cost_single_route(weights, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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

									if (r_support.time_arr[r_support.index - 1] <= end_day) {
										
										r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										if ((p.solution_to - p.solution_from <= p.stop)) {
											if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
												Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
												double cost = (cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
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
						if (t_arr_departure < p.early_departure) 
							TW_departure = p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) 
							TW_departure = t_arr_departure - p.late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
						if (t_arr_arrival < p.early_arrival) 
							TW_arrival = p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) 
							TW_arrival = t_arr_arrival - p.late_arrival;
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

						//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
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

		if (best_route == -1) {
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'? solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				//else {
					//ci sono pi? nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				else {

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
				}
			}

			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	
	return routes_destroyed;
	
}

vector <Route> repair_perturbation(const Penalty_Weights weights, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	double peso_TW = weights.time_window_weight;
	double peso_intermediate_stop = weights.stop_weight;
	
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
					//c'? solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

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
				Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
				double cost_route_before = cost_single_route(weights, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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

								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									
									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
											double cost = (cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;

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
					if (t_arr_departure < p.early_departure) 
						TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) 
						TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) 
						TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) 
						TW_arrival = t_arr_arrival - p.late_arrival;
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

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					
					double firstCase = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					double secondCase = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					f_after_secondo_tratto = (map_airstrip[p.departure_location].fuel) ? firstCase : secondCase;

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

		if (best_route == -1) {
			routes_infeasible = true;
			break;
		}

		if (!routes_destroyed[best_route].primo_pass) {
			//i have to do the new things, 
			if (case_first_passenger == false) {
				//c'? solo il depot
				routes_destroyed[best_route].primo_pass = true;
				routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			//else {
				//ci sono pi? nodi
				//routes_destroyed[best_route].primo_pass = true;
				//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			//}
		}
		else {

			if (move_c) {
				//i have to do the move C
				routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			else {
				//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
				routes_destroyed[best_route] = route_best;
				p.solution_from = from_per_route;
				p.solution_to = to_per_route;
			}
		}

		routes_destroyed[best_route].passengers_in_route.push_back(p);
	
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	
	return routes_destroyed;
	
}





//in questi ancora non messi

void heuristic_costructive_first_fase_secIter(double peso_TW, double peso_intermediate_stop, vector<Route>& solution, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	int situation = -1;
	int cont = 0;
	vector<int> AereiUsati;
	for (Route& r : solution) 
		AereiUsati.push_back(r.aircraft_code);

	do {
		auto it = std::find(AereiUsati.begin(), AereiUsati.end(), airplanes[cont].code);
		if (it == AereiUsati.end()) {
			vector<Passenger> pass_in_solution;
			Route r(airplanes[cont].code, pass_in_solution);
			r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
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
			if (r.primo_pass == false) {
				if (r.places[r.index - 1] == p.departure_location) {
					//in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
					//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					/*
					if (travel_time <= 1) {
						fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/

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
					/*
					if (travel_primo_tratto <= 1) {
						fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					//****
					//double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					
					if (map_airstrip[p.departure_location].fuel) 
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					else 
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					

					
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
					for (int t = FROM[0]; t < r.index; t++) {
						if (r.places[t] == p.arrival_location) TO.push_back(t);
					}

					if (TO.size() != 0) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from < to && to - from <= p.stop) {
									//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
									//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
									bool capacity_satisfy = true;
									double aux; 
									for (int c = from; c < to; c++) {
										if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) 
											capacity_satisfy = false;

										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										/*
										if (travel_time <= 1) {
											fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
										}
										else {
											fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
										}
										*/
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
										if (c == to - 1) 
											fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];


										aux = r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j); 
										capacity_satisfy = !(!r.refueling[c] && aux < 0);

									}
									//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
									if (!capacity_satisfy) 
										break; //check also for the fuel;


									//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider? per l'aggiunta del passeggero qui
									//? la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (r.time_arr[from] < p.early_departure) 
										TW_departure = p.early_departure - r.time_arr[from];
									else if (r.time_arr[from] > p.late_departure) 
										TW_departure = r.time_arr[from] - p.late_departure;

									double TW_arrival = 0.0;
									//double t_arr_arrival = r.time_arr[to];
									if (r.time_arr[to] < p.early_arrival) TW_arrival = 
										p.early_arrival - r.time_arr[to];
									else if (r.time_arr[to] > p.late_arrival) TW_arrival = 
										r.time_arr[to] - p.late_arrival;

									double cost = (((TW_departure + TW_arrival) * peso_TW) + ((to - from - 1) * peso_intermediate_stop)) * p.capacity;
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
						auto& airplane = map_airplane[r.aircraft_code];
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (int c = from; c < r.index; c++) {
								
								if ((r.capacity[c] + p.capacity) > airplane.capacity) 
									capacity_satisfy = false;

								if (c < r.index - 1) {
									//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

									double fuel_i_j = fuel_consumed + airplane.min_fuel;

									if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) 
										capacity_satisfy = false;
								}
							}
							//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
							//cout << p.stop<< endl;
							if (capacity_satisfy && (r.index - from) <= p.stop) {

								double TW_departure = 0.0;
								//double t_arr_departure = r.time_arr[from];
								if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
								else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

								double cost = (TW_departure * peso_TW);
								// ora ci metto chilometri e fuel dell'ultimo pezzo;
								cost += from_to[r.places[r.index - 1]][p.arrival_location];
								double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];
								/*
								if (travel_time <= 1) {
									fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
								}
								else {
									fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
								}
								*/
								cost += fuel_consumed;

								bool fuel_ok = true;
								if (!r.refueling[r.index - 1]) {
									double fuel_i_j = fuel_consumed + airplane.min_fuel;
									if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
								}

								double TW_arrival = 0.0;
								double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / airplane.speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
								if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
								else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
								cost += (TW_arrival * peso_TW) * p.capacity;

								cost += ((peso_intermediate_stop * (r.index - from - 1))) * p.capacity;

								if (fuel_ok) {
									if (best_cost > cost) {
										if (t_arr_arrival <= end_day) {
											if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (airplane.min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
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
					/*
					if (travel_time <= 1) {
						fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

					double TW_departure = 0.0;
					double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) 
						TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) 
						TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) 
						TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) 
						TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					/*if (travel_secondo_tratto <= 1) {
						fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}*/

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

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

		
		//ora devo aggiungere il passeggero nel posto migliore, quindi serve valutare in che caso sono A,B,C,D
		if (situation == 1) {
			auto index_best_route = solution[best_route].index - 1;
			solution[best_route].primo_pass = true;
			//time
			solution[best_route].time_dep[index_best_route] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[index_best_route] = p.early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

			p.solution_from = index_best_route;
			solution[best_route].capacity[index_best_route] += p.capacity;

			//for the weight
			solution[best_route].weight[index_best_route] = map_airplane[index_best_route].weight_fuel_people -
				solution[best_route].quantity_fuel[index_best_route] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[index_best_route] < 0) {
				solution[best_route].quantity_fuel[index_best_route] += solution[best_route].weight[index_best_route];
				solution[best_route].weight[index_best_route] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[index_best_route] - p.weight;
			}
			//end for weight

			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[index_best_route];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];
			
			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[index_best_route] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[index_best_route] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = index_best_route;
			solution[best_route].passengers_in_route.push_back(p);
			
		}
		else if (situation == 2) {
			solution[best_route].primo_pass = true;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 -
				map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
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
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == 3) {
			auto & departure_location = map_airstrip[p.departure_location];
			auto & airplane = map_airplane[solution[best_route].aircraft_code];
			auto index_best_route = solution[best_route].index - 1;
			auto  aux_from_to = ((from_to[solution[best_route].places[index_best_route]][p.departure_location] / airplane.speed) * 60);
			
			if (departure_location.fuel) {
				solution[best_route].addPlace(p.departure_location, departure_location.fuel, airplane.max_fuel, 0.0, p.capacity,
					solution[best_route].time_dep[index_best_route] + aux_from_to,
					solution[best_route].time_dep[index_best_route] + aux_from_to + departure_location.ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = airplane.weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[index_best_route]][p.departure_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				solution[best_route].addPlace(p.departure_location, departure_location.fuel, fuel_before - fuel_consumed, 0.0, p.capacity,
					solution[best_route].time_dep[index_best_route] + aux_from_to,
					solution[best_route].time_dep[index_best_route] + aux_from_to + departure_location.ground_time);

				solution[best_route].weight[index_best_route] = solution[best_route].weight[index_best_route - 1] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / airplane.speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, airplane.max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[index_best_route] = airplane.weight_fuel_people - airplane.max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


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
			//**************************************************************************

			p.solution_from = best_from;
			p.solution_to = best_to;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 5) {
			auto  best_route_index = solution[best_route].index - 1;
			auto & airplane = map_airplane[solution[best_route].aircraft_code];
			
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
			int place_1 = solution[best_route].places[best_route_index];
			auto& arrival_location = map_airstrip[p.arrival_location];
			double aircraft_speed = airplane.speed;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, arrival_location.fuel, airplane.max_fuel, 0.0, 0, time, time + arrival_location.ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = airplane.weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[best_route_index];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[best_route_index]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, arrival_location.fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + arrival_location.ground_time);
				solution[best_route].weight[best_route_index] = solution[best_route].weight[best_route_index - 1] + fuel_consumed + p.weight;

			}

			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == -1) {
			
		}
	}

	vector<int> Elimina;
	for (int s = 0; s < (int)solution.size(); s++) 
		if (solution[s].index <= 1) 
			Elimina.push_back(s);

	for (int i = (int)(Elimina.size()) - 1; i >= 0; i--) 
		solution.erase(solution.begin() + Elimina[i]);
}

vector<Route> repairSP(double peso_TW, double peso_intermediate_stop, vector<Route>& route_destroy, vector<Passenger>& passenger_removed, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<Airplane>& airplanes, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<vector<Route>> solutionAllSub;
	int NAirplane = 20;

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
		// Lo ordino in modo random
		auto rng2 = default_random_engine{};
		shuffle(passenger_removed.begin(), passenger_removed.end(), rng2);
		vector<Route> solution;
		vector<Route> Route_to_fix = route_destroy;
		//cout << " Route da dare al costruittivo = "<< Route_to_fix.size() << endl;
		heuristic_costructive_first_fase_secIter(peso_TW, peso_intermediate_stop, Route_to_fix, airplanes, map_airplane, map_airstrip, end_day, passenger_removed, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
		Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 }; 
		double cost_fix_single = calculate_ObjectiveFunction(weights, Route_to_fix, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

		//cout << "***********************SOLUTION SECOND FASE***********************" << endl;
		solution = heuristic_costructive_second_fase_SP(Route_to_fix, end_day, peso_TW);
		weights = { peso_intermediate_stop, peso_TW, 0 };
		cost_fix_single = calculate_ObjectiveFunction(weights, solution, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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
				auto it = find_if(r.passengers_in_route.begin(), r.passengers_in_route.end(), [=](Passenger &p_){ return p.pnr == p_.pnr; });
				
				if (it != r.passengers_in_route.end()){
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
	// adesso vettore delle Route non ? piu vuoto
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
		
		n = 0;
		for (Route& r : solutionAllSub[Best_iteration]) n += (int)r.passengers_in_route.size();
		
		if (n == (int)passengers.size())  
			return solutionAllSub[Best_iteration];
		else {
			vector<Route> vuoto(0);
			return vuoto;
		}
	}

	delete model;
}



//************************************************************************************** Local Search **************************************************************
void destroy_ls(int index, int node_destroy, vector<Passenger>& passenger_removed, Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
	
	if (r.index > 1) {
		bool check = true;
		double fuel_consumed_check = 0.0;
		if ((node_destroy + 2) < r.index) {
			//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; 
			double time_travel = from_to[r.places[node_destroy - 1]][r.places[node_destroy + 2]] / map_airplane[r.aircraft_code].speed;
			if (time_travel <= 1) fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
			else {
				fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
			}
			//cout << " Deve valere che: " << fuel_consumed_check << " + " << map_airplane[r.aircraft_code].min_fuel << " <= " << r.quantity_fuel[node_destroy - 1] << endl;
		}
		if ((fuel_consumed_check + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
			check = false;
			int number_initial_node = r.index;
			for (int i = 0; i < 2; i++) {
				//cout << " Sono al giro i -> " << i << endl;
				if (i == 1 && (number_initial_node - r.index) > 1) break;
				//cout << " devo distruggere il nodo -> " << node_destroy << endl;
				double fuel_consumed = 0.0;
				double time_travel = from_to[r.places[node_destroy - 1]][r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed;
				if (time_travel <= 1) fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
				else {
					fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
				}
				vector<int> int_removed;
				int Min_From_Pass = node_destroy;
				int Max_To_Pass = node_destroy;
				for (int p = 0; p < r.passengers_in_route.size(); p++) {
					if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
						if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
						if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;

						int_removed.push_back(p);
						for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
							r.capacity[t] -= r.passengers_in_route[p].capacity;
							r.weight[t] += r.passengers_in_route[p].weight;
						}

					}
				}

				for (int i = int_removed.size() - 1; i >= 0; i--) {
					// code for repair forbidden***********************************************************
					r.passengers_in_route[int_removed[i]].route_before = index;
					//*************************************************************************************
					passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
					vector<Passenger>::iterator it;
					it = r.passengers_in_route.begin();
					r.passengers_in_route.erase(it + int_removed[i]);
				}

				r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
				int index_before = node_destroy - 1;
				double diff = 0;

				for (int i = node_destroy + 1; i < r.index; i++) {

					if (r.refueling[i]) break;

					if (index_before == (node_destroy - 1)) {
						diff = r.quantity_fuel[i];
						r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
						diff = diff - r.quantity_fuel[i];

					}
					else {
						r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
					}

					r.weight[i] = r.weight[i] + diff;
					index_before = i + 1;

				}
				//*****************************************************************************************************************************************************************

				
				r.removePlace(node_destroy, map_airplane);
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weight[j] < 0) {
						//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						for (int t = index_refueling; t < r.index; t++) {
						
							if (r.refueling[t] && t != index_refueling) break;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
						}
					}
				}

			}
		}

		if (check) {
			r.index = -1;
		}
	}
}

int sequential_same_node(const Route& r) {
	int node = -1;
	for (int i = 0; i < ((int)r.places.size()) - 1; i++) {
		if (r.places[i] == r.places[i + 1]) {
			node = i;
			break;
		}
	}
	return node;
}

void aggregate_same_nodes(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node))	p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		//cout << " Nodo doppio che devo eliminare risiede nell'ultima posizione" << endl;
		r.time_dep[node + 1] = r.time_dep[node];
		r.time_arr[node + 1] = r.time_arr[node];
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
	else {
		//cout << " Nodo doppio che devo eliminare NON risiede nell'ultima posizione" << endl;
		// I remove the extra time that was considered
		double time_to_remove = r.time_dep[node + 1] - r.time_dep[node];
		for (int i = node + 1; i < r.index; i++) {
			r.time_dep[i] -= time_to_remove;
			r.time_arr[i] -= time_to_remove;
		}
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}



}

void aggregate_same_nodes_inter_ls(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node))	p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		//cout << " Nodo doppio che devo eliminare risiede nell'ultima posizione" << endl;
		r.time_dep[node + 1] = r.time_dep[node];
		r.time_arr[node + 1] = r.time_arr[node];
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
	else {
		//cout << " Nodo doppio che devo eliminare NON risiede nell'ultima posizione" << endl;
		// I remove the extra time that was considered
		double time_to_remove = r.time_dep[node + 1] - r.time_dep[node];
		for (int i = node + 1; i < r.index; i++) {
			if (i == node + 1) {
				r.time_dep[i] = r.time_dep[node];
				r.time_arr[i] = r.time_arr[node];
			}
			else {
				r.time_dep[i] -= time_to_remove;
				r.time_arr[i] -= time_to_remove;
			}

		}
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}



}

int swap_from(int node, const Route& r) {

	int swap_from = 0;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}


	return swap_from;

}

int swap_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}

	}


	return swap_to;
}


bool swap_is_allowed(int A, int B, const Route& r) {
	bool swap = false;
	if (B < swap_to(A, r) && swap_from(B, r) < A) swap = true;
	return swap;
}

//qua purtroppo non posso mettere usare i puntatori
Route update_route_after_swap(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;

	//cout << "sto inserendo i vari posti nella nuova route" << endl;
	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	for (int i = 1; i < r.index; i++) {
		if (i == A) {
			//in questo posto ci devo mettere B
			r_new.addPlace(r.places[B], r.refueling[B], r.quantity_fuel[B], 0.0, 0, r.time_arr[B], r.time_dep[B]);
		}
		else if (i == B) {
			//in questo posto ci devo mettere A
			r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
		}
		else {
			//in questo posto ci devo mettere normalmente i
			r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
		}
	}

	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua ? come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			if (r_new.refueling[i]) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
			}
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}

	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri spostati
	for (Passenger p : r.passengers_in_route) {
		if (p.solution_from == A) p.solution_from = B;
		else if (p.solution_to == A) p.solution_to = B;
		else if (p.solution_from == B) p.solution_from = A;
		else if (p.solution_to == B) p.solution_to = A;
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
	}
	
	//aggiorno fuel se il peso ? negatico
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			//	cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					//	cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			r_new.weight[index_refueling] -= r_new.weight[i];

			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					r_new.weight[j] -= Update_value;
				}
			}

			//r_new.weight[i] = 0;

		}
	}



	return r_new;
}

vector <Route> swap(double peso_TW, double peso_intermediate_stop, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_swap;

	for (const Route& r : routes) {
		//cout << " sto provando a fare lo swap in questa route: " << endl;
		//r.print();
		Route r_support = r;
		for (int A = 1; A < r_support.index - 1; A++) {
			for (int B = A + 1; B < r_support.index; B++) {

				if (swap_is_allowed(A, B, r_support)) {
					//cout << "******************************************************Sto provando a fare lo swap fra " << A << " e " << B << endl;
					Route r_new = update_route_after_swap(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
					//cout << "ho finito l'update" << endl;
					//r_new.print();
					Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
					double cost_route_support = cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					double cost_route_new = cost_single_route(weights, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					//cout << cost_route_support << " c_supp > c_new " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to) << endl;
					if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
						//cout << " OK ! dopo move Migliora ------- Ma prima devo controllare che non sia con nodi doppi " << endl;
						int node = sequential_same_node(r_new);
						bool fatto = false;
						while (node != -1) {
							
							aggregate_same_nodes(r_new, node);
							Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
							cost_route_new = cost_single_route(weights, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							fatto = true;
							node = sequential_same_node(r_new);
						}
						//cout << " Ok non aveva nodi doppi ! " << endl;
						if (fatto == false) {
							r_support = r_new;
							A = 1;
							B = A + 1;
						}
						else {
							if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
								r_support = r_new;
								A = 1;
								B = A + 1;
								cost_route_support = cost_route_new;
							}
						}
					}

				}

			}
		}
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {
					
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
				}

				if (Node_min >= 0) {
					
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_swap.push_back(r_support);
	}

	return routes_after_swap;
}

int move_from(int node, const Route& r) {
	int swap_from = 0;

	for (const Passenger& p : r.passengers_in_route) 
		if (p.solution_to == node && p.solution_from > swap_from) 
			swap_from = p.solution_from;
	
	return swap_from;

}

int move_to(int node, const Route& r) {
	int swap_to = 1000;;
	for (const Passenger& p : r.passengers_in_route)
		if (p.solution_from == node && p.solution_to < swap_to)
			swap_to = p.solution_to;

	return swap_to;
}



bool move_flightleg_is_allowed(int A, Route& r) {
	bool move = true;
	//TODO: Improve this code 
	for (Passenger& p : r.passengers_in_route) {
		if ((p.solution_from == A) && (p.solution_to != (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_from != A) && (p.solution_to == (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_to == A) || (p.solution_from == (A + 1))) {
			move = false;
			break;
		}
	}
	return move;
}

Route update_route_after_move(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, Double3DVector & from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;


	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	if (B > A) {
		for (int i = 1; i < r.index; i++) {
			if ((i < A) || (i > B)) {
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if ((i >= A) && (i < B)) {
				r_new.addPlace(r.places[i + 1], r.refueling[i + 1], r.quantity_fuel[i + 1], 0.0, 0, r.time_arr[i + 1], r.time_dep[i + 1]);
			}
			else if (i == B) {
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
		}
	}
	else {
		for (int i = 1; i < r.index; i++) {
		
			if (i < (B + 1)) {		
				//in questo posto ci devo mettere normalmente i
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if (i == (B + 1)) {
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
			else if (i >= A + 1) {
				//in questo posto ci devo mettere normalmente i-1
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else {
				r_new.addPlace(r.places[i - 1], r.refueling[i - 1], r.quantity_fuel[i - 1], 0.0, 0, r.time_arr[i - 1], r.time_dep[i - 1]);
			}
		}
	}
	
	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua ? come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {			
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			if (r_new.refueling[i]) 
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			else 
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
	
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}
	
	for (Passenger p : r.passengers_in_route) {
		if (p.solution_from == A && A > B) {
			p.solution_from = B + 1;
		}
		else if (p.solution_to == A && A > B) {
			p.solution_to = B + 1;
		}
		else if (p.solution_to == A && A < B) {
			p.solution_to = B;
		}
		else if (p.solution_from == A && A < B) {
			p.solution_from = B;
		}
		else if (p.solution_from <= B && B < p.solution_to && A > p.solution_to) {
			p.solution_to++;
		}
		else if (p.solution_from <= B && B < p.solution_to && A < p.solution_from) {
			p.solution_from--;
		}
		else if (p.solution_from > B && A > p.solution_to) {
			p.solution_from++;
			p.solution_to++;

		}
		else if (p.solution_from < A && A < p.solution_to && B < p.solution_from) {
			p.solution_from++;
		}
		else if (p.solution_from < A && A < p.solution_to && B >= p.solution_to && A < B) {
			p.solution_to--;
		}
		else if (p.solution_from > A && A < p.solution_to && B >= p.solution_to) {
			p.solution_to--;
			p.solution_from--;
		}
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
	}
	
	//aggiorno fuel se il peso ? negatico */
	for (int i = 0; i < r_new.index; i++) {
		if (r_new.weight[i] < 0) {
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			r_new.weight[index_refueling] -= r_new.weight[i];

			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) 
					break;
				
				r_new.quantity_fuel[j] += Update_value;
				r_new.weight[j] -= Update_value;
			}
		}
	}

	return r_new;
}

vector <Route> move(double peso_TW, double peso_intermediate_stop, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;
	auto move_is_allowed = [](int A, int B, const Route& r) { return ((B > A && B < move_to(A, r)) || (B < A&& move_from(A, r) < B));  };
	
	for (const Route& r : routes) {
		
		Route r_support = r;
		for (int A = 1; A < r_support.index; A++) {
			// we have to move the Node A form its position to th enext after B
			for (int B = 1; B < r_support.index; B++) {
				if (A != B && B != (A - 1) && B != (A - 2)) {    //Il caso B != (A-1) || B!= (A-2) sono casi che valutiamo gia quando sposriamo avanti
				
					if (move_is_allowed(A, B, r_support)) {
						Route r_new = update_route_after_move(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);

						Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
						double cost_route_support = cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						double cost_route_new = cost_single_route(weights, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						
						if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
							
							int node = sequential_same_node(r_new);
							
							bool fatto = false;
							int num_aggregazioni = 0;
							while (node != -1) {
								
								aggregate_same_nodes(r_new, node);
								if (A > node) 
									num_aggregazioni++;

								Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
								cost_route_new = cost_single_route(weights, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								node = sequential_same_node(r_new);
								fatto = true;
							}

							if (fatto == false) r_support = r_new;
							else {
								if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
									r_support = r_new;
									cost_route_support = cost_route_new;
									A -= num_aggregazioni;
									if (A <= 0) A = 1;
								}
							}
						}
					}
				}

			}
		}
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
				
				if (Node_min >= 0) {
					
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_move.push_back(r_support);
	}

	return routes_after_move;
}



vector <Route> repair_one_inter_move(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'? solo il deposito in questo caso
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
				//int arc_from = -1;
				//int arc_to = -1;
				Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 }; 
				double cost_route_before = cost_single_route(weights, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
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


								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									
									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
											double cost = (cost_single_route(weights, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
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

					//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
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
									//best_arc_from = -2;
									//best_arc_to = -2;
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
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//routes_destroyed[best_route].print();
				}
				//else {
					//ci sono pi? nodi
				//	routes_destroyed[best_route].primo_pass = true;
				//	routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//routes_destroyed[best_route].print();

				}
				else {
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
	
	return routes_destroyed;
	
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

		//Route distrutte sono uguali tranne quella a cui sto togliendo la coppia
		//for (Route s : routes) if (s.aircraft_code != r.aircraft_code) routes_destroyed.push_back(s);
		for (const Route& s : routes) {
			//cout << "if: " << to_string(s.aircraft_code) << "  ----  " << to_string(routes[r].aircraft_code) << endl;
			if (s.aircraft_code != routes[r].aircraft_code) routes_destroyed.push_back(s);
		}
		Route r_support = routes[r];

		for (int A = 1; A < (r_support.index - 1); A++) {
			// Solution Rebuilt ? come se fosse il nostro best improvement
			vector<Route> solution_rebuilt;
			vector<Passenger> passenger_removed;

			// Ora devo eliminare i nodi solo se sono allowed
			if (move_flightleg_is_allowed(A, r_support)) {
				//cout << " CONSENTITO sono nel main -----> provo a fare move di " << A << " e " << A+1 << " da " << r_support.aircraft_code << " un altra route " << endl;
				Route r_new = r_support;

				int NomeA1 = r_new.places[A + 1];
				destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);

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
							solution_rebuilt.push_back(r_new);

							Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
							double before = calculate_ObjectiveFunction(weights, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed); // Qui non va bene devi considerare che dopo un primo miglioramneto cambi la route
							double after = calculate_ObjectiveFunction(weights, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (before > after) {
								// Qui sto usando tutto solution_rebuilt.back() ma in realta potrei usare r_new e poi un volta che la ho istemanta switcharla con solution_rebuilt.back()
								int node = sequential_same_node(solution_rebuilt.back());
								//cout << "node fuori dal while: " << node << endl;
								//r_new.print();
								bool fatto = false;
								int num_aggregazioni = 0;
								while (node != -1) {
									aggregate_same_nodes_inter_ls(solution_rebuilt.back(), node);
									if (A > node) num_aggregazioni++;
									weights = { peso_intermediate_stop, peso_TW, 0 };
									after = calculate_ObjectiveFunction(weights, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									node = sequential_same_node(solution_rebuilt.back());
									fatto = true;
								}

								if (fatto == false) {
									r_support = r_new;

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
										r_support = r_new;
										routes_after_move = solution_rebuilt;
										routes = solution_rebuilt;
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
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch? se no se oltre quel nodo non c'? ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}

					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
	}
	if (routes_after_move.size() == 0) 
		return routes;
	
	Penalty_Weights weights = { peso_intermediate_stop, peso_TW, 0 };
	double objective_function_before = calculate_ObjectiveFunction(weights, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
	double objective_function_after = calculate_ObjectiveFunction(weights, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
	if ( abs( objective_function_before - objective_function_after) > 1e5 ) {
		cout << " Costo Routes: " << objective_function_before << endl;
		cout << " Costo routes_after_move: " << objective_function_after << endl;
		//cin.get();
	}
	return routes_after_move;
	
}

//************************************************************************************************************************************************************************


double costo_senza_time_windows(vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		cost += map_airplane[r.aircraft_code].fixed_cost;
		cost_route += map_airplane[r.aircraft_code].fixed_cost;

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
	}
	return cost;
};

double costo_time_windows_and_intermediate_stop(double peso_TW, double peso_intermediate_stop, vector<Route>& solution) {
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


//data una location mi restituisce la location pi? vicina
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


vector <Route> aggrezione_simple_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to) {
	vector <Route> solution_after;

	cout << "*****************************SOLUTION PRIMA DELL'AGGREAGZIONE********************************" << endl;
	for (Route r : solution_model) {
		r.print();
	}
	cout << "****************************************ora faccio aggregazione con l'ultimo pezzo che si attacca solo se i due posti fine e inizio sono uguali***************" << endl;
	//cerco di collegare le route se ? possibile
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
				Route * _route = & solution_model[r]; 
				for (int i = 0; i < _route->index - 1; i++) {
					if (i == solution_model[r].index - 2) {
						double time_to_go = ((from_to[_route->places[i]][solution_model[r1].places[0]]) / map_airplane[solution_model[r].aircraft_code].speed) * 60;
						r_add.addPlace(_route->places[i], _route->refueling[i], _route->quantity_fuel[i], _route->weight[i],_route->capacity[i], 
										_route->time_arr[i], (solution_model[r1].time_arr[0] - time_to_go ));
					}
					else {
						r_add.addPlace(_route->places[i], _route->refueling[i], _route->quantity_fuel[i], _route->weight[i], 
										_route->capacity[i], _route->time_arr[i], _route->time_dep[i]);
					}
				}

				_route = & solution_model[r1]; 
				for (int i = 0; i < _route->index; i++) {
					r_add.addPlace(_route->places[i], _route->refueling[i], _route->quantity_fuel[i], _route->weight[i],
						_route->capacity[i], _route->time_arr[i], _route->time_dep[i]);
				}
				for (Passenger& p : _route->passengers_in_route) {
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
	
	return solution_after;
}

vector <Route> aggrezione_complex_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector <Route> solution_after;

	//cout << "****************************************ora faccio aggregazione complessa***************" << endl;
	//cerco di collegare le route se ? possibile
	//vector <Route> solution_after;
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";
			double time_trascorso = (from_to[solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]] / map_airplane[solution_model[r].aircraft_code].speed) * 60;
			double fuel_consumed = from_to_FuelConsumed[solution_model[r].aircraft_code][solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]];


			double costo_aggiunta = fuel_consumed + from_to[solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]];
			//READ ME: non ? importante fare il check se i due posti (finale di una route, inziale della seconda) siano uguali, questa casistica viene eliminata dall'aggregazione semplice
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


void fix_key_passenger_for_regret(vector<Passenger>& passengers) {

	int chiave = 1;
	map<string, int> string_key;

	for (Passenger& p : passengers) {
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


void Build_structure_Model(const Fleet & airplane, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
	int tipo = 0;
	for (int j = 0; j < (int)airplane.size(); j++) {
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
		map<int, int>::iterator it = tipo_numero.find(j.second);
		if (it == tipo_numero.end()) 
			tipo_numero.insert(make_pair(j.second, 1));
		else 
			tipo_numero[j.second] += 1;
	}
}


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
		//cout << "con il codice: " << x.first << endl;
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
		/*
		*/
		for (Passenger& g : genitori_donna) {
			g.print();
			passengers_aggr.push_back(g);
		}
		for (Passenger& g : genitori_maschi) {
			g.print();
			passengers_aggr.push_back(g);
		}
		/*
		*/
		//cin.get();
	}
	cout << "ora faccio il check per vedere se le capacita' dei due gruppi tornano" << endl;
	int capacita_aggr = 0;
	for (Passenger p : passengers_aggr) 
		capacita_aggr += p.capacity;
	cout << "le due capacita' sono: " << endl;
	cout << " - " << passengers.size();
	cout << " - " << capacita_aggr << endl;
	
	cout << "ho finito di vedere i gruppi" << endl;
	cout << "i passeggeri nei gruppi sono: " << numero_in << endl;
	if (numero_in != (int)passengers.size()) cout << "ERROREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;


	passengers_for_company_solution = passengers;
	passengers = passengers_aggr;
}

void unione_children_VECCHIA(vector<Passenger>& passengers, vector<Passenger>& passengers_for_company_solution) {

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
		

		if ((int)bambini.size() > 0) {
			//incomincio a metterli con la mamma
			for (Passenger p : bambini) {
				int loc_dep = p.departure_location;
				int loc_arr = p.arrival_location;
				int time_dep = p.departure_time;
				int time_arr = p.arrival_time;

				if ((int)genitori_donna.size() > 0) {
					// metti i bambini con la mamma
					for (Passenger& g : genitori_donna) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time  &&  time_arr == g.arrival_time) {
							g.capacity += p.capacity;
							g.weight += p.weight;
							break;
						}
					}
				}
				else if ((int)genitori_maschi.size() > 0) {
					//mettili con il papa
					for (Passenger& g : genitori_maschi) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time  &&  time_arr == g.arrival_time) {
							g.capacity += p.capacity;
							g.weight += p.weight;
							break;
						}
					}
				}
				else {
					cout << "errore, ci sono dei bambini senza genitore" << endl;
				}

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

		//cin.get();
	}
	cout << "ho finito di vedere i gruppi" << endl;
	cout << "i passeggeri nei gruppi sono: " << numero_in << endl;
	if (numero_in != (int)passengers.size()) cout << "ERROREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
	//cin.get();


	passengers_for_company_solution = passengers;
	passengers = passengers_aggr;

}


void printTimes(double time_spent_model, double tempo_finale, int NumeroSA, double time_incumbent)
{
	int tempo_Alns, h, m, s;
	tempo_Alns = (int)(tempo_finale - time_spent_model);
	h = (tempo_Alns / 3600);
	m = (tempo_Alns - (3600 * h)) / 60;
	s = (tempo_Alns - (3600 * h) - (m * 60));
	cout << h << ":" << m << ":" << s << ";";
	tempo_Alns = (int)(time_spent_model);
	h = (tempo_Alns / 3600);
	m = (tempo_Alns - (3600 * h)) / 60;
	s = (tempo_Alns - (3600 * h) - (m * 60));
	cout << h << ":" << m << ":" << s << ";";
	tempo_Alns = (int)(time_incumbent);
	h = (tempo_Alns / 3600);
	m = (tempo_Alns - (3600 * h)) / 60;
	s = (tempo_Alns - (3600 * h) - (m * 60));
	cout << h << ":" << m << ":" << s << ";";
	cout << NumeroSA << endl;
}

//***********************************MAIN****************************
int main(int argc, char* argv[]) {

	//cout << argv[1] << endl;
	//cout << argv[2] << endl;
	//cout << argv[3] << endl;

	/*
	ATTENTO, adesso le mosse degli eurisitici sono:
		error ---> -1
		A	---> 1
		B	---> 2
		C	---> 3
		D	---> 4
		E	---> 5
	*/


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


	//reading of AIRPLANE.CSV and creation VECTOR OF AIRPLANES
	vector<Airplane> airplanes = fillAirplane("NewAirplaneAll.csv", legenda);
	Build_structure_Model(airplanes, mappa_aereo_tipo, codice_aereo_tipo, tipo_numero);
	//cout << "***********************Print airplane***********************" << endl;
	//for (auto f : airplanes) f.print();
	//reading of AIRSTRIP.CSV and creation VECTOR OF AIRSTRIPS
	vector<Airstrip> airstrips = fillAirstrip("NewAirstrip.csv", legenda);
	//cout << "***********************Print airstrips***********************" << endl;
	//for (auto f : airstrips) f.print();
	//reading of INSTANCE.CSV and creation VECTOR OF PASSENGER
	//vector<Passenger> passengers = fillPassenger("17_11_18_ISTANZA.csv", legenda); 
	vector<Passenger> passengers = fillPassenger(argv[1], legenda);
	vector<Passenger> passengers_for_company_solution;

	unione_children_INTELLIGENTE(passengers, passengers_for_company_solution);



	fix_key_passenger_for_regret(passengers);

	map<int, Passenger> map_id_passenger = fillMapPassenger(passengers);

	map<string, double> from_to_company = fillMatrixCompany("Matrix.csv", airstrips);


	//bulding of hashmap from_to reading the file Matrix.txt
	vector<vector<double>> from_to;
	fillMatrix(from_to, "Matrix.csv", airstrips);


	map<int, Airplane> map_airplane;
	for (auto f : airplanes) map_airplane.insert(make_pair(f.code, f));

	map<int, Airstrip> map_airstrip;
	for (auto f : airstrips) map_airstrip.insert(make_pair(f.code, f));
	//ora faccio la mappa che mi permette di avere come chiave ogni aereo/aeroporto, come valore il fuel necessario, dato quell'aereo per andare nell'aereoporto pi? vicino con quel fuel

	vector<vector<double>> location_fuel;
	fillLocation_fuel(location_fuel, airstrips, airplanes, from_to, map_airstrip);
	
	/*map<string, Airstrip> map_airstrip_check;
	for (auto f : airstrips) map_airstrip_check.insert(make_pair(f.code_string, f));
	map<string, double> location_fuel_string = fillLocation_fuel_string(airstrips, airplanes, from_to_company, map_airstrip_check);
	cout << "faccio il check della mappe location_fuel" << endl;
	map<string, Airplane> map_airplane_check;
	for (auto f : airplanes) map_airplane_check.insert(make_pair(f.code_company, f));
	check_mappa_location_fuel(map_airplane_check, location_fuel_string, location_fuel, legenda);
	cout << "ho finto il check sul from_to_fuel_consumed" << endl;*/
	//system("pause");

	vector<vector<vector<double>>> from_to_FuelConsumed;
	fill_from_to_fuel_consumed(from_to_FuelConsumed, from_to, airplanes);
	/*map<string, double> from_to_fuel_consumed_string = fill_from_to_fuel_consumed_string(from_to_company, airplanes);
	cout << "faccio il check della mappe from_to_fuel_consumed" << endl;
	map<string, Airplane> map_airplane_check;
	for (auto f : airplanes) map_airplane_check.insert(make_pair(f.code_company, f));
	check_mappa_from_to_fuel_consumed(map_airplane_check, from_to_fuel_consumed_string, from_to_FuelConsumed, legenda);
	cout << "ho finto il check sul from_to_fuel_consumed" << endl;
	system("pause");
	*/



	Input input;
	input.set_airplanes(airplanes);
	input.set_airstrips(airstrips);
	input.set_passengers(passengers_for_company_solution);
	vector <Passenger> passengers_per_casostrano = passengers;
	vector<Passenger> passengers_solution = passengers;

	Penalty_Weights weights = { peso_itermediate_stop, peso_TW, peso_transhipment };
	
	double Company_Solution = calculationCostCompany(weights, argv[2], argv[3], input, from_to_company);
	//double Company_Solution = calculationCostCompany(peso_TW, peso_transhipment, peso_itermediate_stop, "17_11_18_AEREI.txt", "Passenger_17_11_18.txt", airstrips, airplanes, passengers_for_company_solution, from_to_company);
	cout << " Costo della soluzione della compagnia = " << Company_Solution << endl;
	cout << " Costo della soluzione della compagnia = " << Company_Solution << endl;
	cout << "now passengers has this number of passengers: " << passengers.size() << endl;
	int num_true = 0;
	for (Passenger p : passengers) num_true += p.capacity;
	cout << ", but the real number was: " << num_true << endl;
	//system("pause");


	cout << "***********************End reading input quindi comincio a contare il tempo***********************" << endl;

	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//*******************************************************DA QUA INCOMINCIA IL CODICE DEL MAIN, PRIMA CI SONO SOLO STRUTTURE DATI***********************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	ofstream outfile; //per il file di input grafico.txt++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	vector<string> data = split(argv[1], '.');
	//vector<string> data = split("17_11_18_ISTANZA.csv", '.');
	string Nome_grafico = data[0] + "grafico.txt";
	cout << " Nome del Grafico: " << Nome_grafico << endl;
	outfile.open(Nome_grafico);
	//outfile.open("grafico.txt");
	int number_iteration_in_heuristic = 0;

	double time_spent_model = 0.0;
	double tempo_finale;
	time_t time_now = time(NULL);
	//input heuristic
	int number_airplane = stoi(argv[4]); // Per Modello
	//int number_airplane = 12;
	//cout << "il numero degli aerei e: " << number_airplane << endl;
	//cin.get();
	int Iter_FirstDo = 0;
	int iterMAX_FirstDo = 10; //CHAIAMATA MODELLO
	//double best_solution_ever = DBL_MAX;
	int NDestroy = 4;
	int NRepair = 5;
	int NHeuristic = 2;
	int initial_weigth = 10;
	vector<double> Weigth_destroy;
	vector<double> Weigth_repair;
	vector<double> Weigth_heuristic(NHeuristic);
	for (int i = 0; i < NDestroy; i++) 
		Weigth_destroy.push_back(initial_weigth);
	for (int i = 0; i < NRepair; i++) 
		Weigth_repair.push_back(initial_weigth);
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
	double time_incumbent = 0.0;


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
		int choosen_heuristic = -1;
		if (Iter_FirstDo > 4) {
			T = 2500;        // Almeno 2000
			alpha = 0.95; // Almeno 0.997
			DeltaK = 30; //Almeno 25
			Tmin = 50;
			iteration_max = 80;
		}
		if (Iter_FirstDo == 0) {

			double heuristic_choice = (double)rand() / RAND_MAX;
			int npass = 0;
			do {
				random_shuffle(begin(passengers), end(passengers));

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					start_solution_route = heuristic_costructive_first_fase(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
					choosen_heuristic = 0;
				}
				else {
					start_solution_route = heuristic_costructive_first_fase_sequential(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
					choosen_heuristic = 1;
				}
				for (Route& r : start_solution_route) npass += (int)r.passengers_in_route.size();
				
				cout << "numero di passeggeri " << npass << endl;
				cout << "passengers.size()= " << passengers.size() << endl;
				
				number_iteration_in_heuristic++;
			} while ((int)passengers.size() > npass);
		}

		Penalty_Weights weights = { peso_itermediate_stop, peso_TW, 0 }; 
		start_solution = calculate_ObjectiveFunction(weights, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		solutionAll.push_back(start_solution_route);

		//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
		outfile << "soluzione euristica di partenza:;0;" << start_solution << endl;
		//outfile.close();
		//*********************************************************************************************************************************************************************************************************************

		vector<Route> best_solution_route = start_solution_route;
		best_solution = start_solution;
		// SI INIZI IL CILCO CON LA RICERCA LOCALE E SIMULATING ANNEALING

		do {
			auto start = chrono::high_resolution_clock::now();
			NumeroSA++;
			//*************************************heuristic costructive***************************************************************************************
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

			Penalty_Weights weights = { peso_itermediate_stop, peso_TW, peso_transhipment }; 
			if (destroy_choice < Accumulated(0, Weigth_destroy)) {
				int num_passenger_cluster = (int)((passengers.size() * 0.14) + (random * ((passengers.size() * 0.24) - (passengers.size() * 0.14)))); //era 0.24 prima
				route_destroy = destroy_cluster_aggr2(weights, num_passenger_cluster,passenger_removed, Input_destroy, map_airplane, from_to, passengers, map_id_passenger);
				choosen_destroy = 0;
			}
			else if (destroy_choice < Accumulated(1, Weigth_destroy)) {
				route_destroy = destroy_worst(weights, percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 1;
			}
			else if (destroy_choice < Accumulated(2, Weigth_destroy)) {
				route_destroy = destroy_casual(percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 2;
			}
			else {
				route_destroy = destroy_thanos(percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 3;
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
			double time_spent = Time_Spend((double)duration.count());
			
			//vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa
			//stampo_caso_strano_destroy(route_destroy, from_to_company, map_airplane, legenda_ritroso);
			//for (Route p : route_destroy) p.print();
			//for(Route p : route_destroy) p.print();
			//cout << "ecco i passeggeri da reinserire: " << endl;
			//for (Passenger p : passenger_removed) p.print();
			//cout << "********************************REBUILT***************************" << endl;
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;

			//PER TEST CON PUNTATORI************************************************************************
			/*cout << "sono prima dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;;
			int pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluione " << pass_soluzione << endl;
			int pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl; */

			auto start_r = chrono::high_resolution_clock::now();

			if (repair_choice < Accumulated(0, Weigth_repair)) {
				solution_rebuilt = repair_one(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 0;
			}
			else if (repair_choice < Accumulated(1, Weigth_repair)) {
				solution_rebuilt = repairSP(peso_TW, peso_itermediate_stop, route_destroy, passenger_removed, map_airplane, map_airstrip, from_to, airplanes, end_day, passengers, number_of_aircraft, location_fuel, from_to_FuelConsumed);
				choosen_repair = 1;
			}
			else if (repair_choice < Accumulated(2, Weigth_repair)) {
				weights = { peso_itermediate_stop, peso_TW, peso_transhipment };
				solution_rebuilt = two_regret_repair_aggragati(weights, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 2;
			}
			else if (repair_choice < Accumulated(3, Weigth_repair)) {
				weights = { peso_itermediate_stop, peso_TW, peso_transhipment };
				solution_rebuilt = repair_forbidden(weights, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 3;
			}
			else {
				weights = { peso_itermediate_stop, peso_TW, peso_transhipment };
				solution_rebuilt = repair_perturbation(weights, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
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

			if (solution_rebuilt.size() > 0) {
				Penalty_Weights weights = { peso_itermediate_stop, peso_TW, peso_transhipment }; 
				double initial_cost = calculate_ObjectiveFunction(weights, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt: " << initial_cost << endl;
				//vector<Route> solution_rebuilt_without_LS = solution_rebuilt;
				//cout << "sto iniziando il move" << endl;
				//QUA C'Eè IL MOVE
				solution_rebuilt = move(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
				solution_rebuilt = swap(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)

				if (initial_cost == calculate_ObjectiveFunction(weights, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
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
				weights = { peso_itermediate_stop, peso_TW, peso_transhipment }; 
				double cost_objectiveFunction_second_fase_after_rebuilt = calculate_ObjectiveFunction(weights, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
				set<int> values = { 1000, 2000, 3000 , 4000 , 5000 , 6000 , 7000 , 8000, 9000 , 10000, 11000 , 12000 , 13000 , 14000 , 15000 , 16000 , 17000 , 18000 , 19000 , 20000,
									21000 , 22000 , 23000 , 24000 , 25000 , 26000 , 27000 , 28000 , 29000 , 30000 , 31000 , 32000 , 33000 , 34000 , 35000 , 36000 , 37000,
									38000 , 39000 , 40000 , 41000 , 42000 , 43000 , 44000 , 45000 , 46000 , 47000 , 48000 , 49000 , 50000,  52000 , 53000 , 54000 ,
									 55000 , 56000 , 57000 , 58000 , 59000 , 60000 };
				if (values.find(NumeroSA) != values.end() ) {
					tempo_finale = difftime(time(NULL), time_now);
					outfile << "NumeroSA:;" << NumeroSA << ";" << best_solution << ";tempo:;" << to_string(tempo_finale) << endl;
					//outfile1.close();
				}
				//*********************************************************************************************************************************************************************************************************************

				//if (cost_objectiveFunction_second_fase_after_rebuilt < initial_cost) solutionAll.push_back(solution_rebuilt_without_LS);
				if (cost_objectiveFunction_second_fase_after_rebuilt < start_solution) {
					//cout << "Ottimo aggiorno la soluzione da cui continuare a fare perturbazioni " << endl;
					start_solution_route.clear();
					//start_solution_route.shrink_to_fit();
					//cout << " Ho cancellato " << endl;
					//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					for (Route& so : solution_rebuilt) start_solution_route.push_back(so);
					//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					double award = Ob_Funct_Improvement(cost_objectiveFunction_second_fase_after_rebuilt, start_solution);
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

						time_incumbent = difftime(time(NULL), time_now);

					}
				}
				else {

					r = double(rand()) / (double(RAND_MAX) + 1.0);
					//cout << " Ho generato numero random " << r << endl;
					//cout << "-( " << cost_objectiveFunction_second_fase_after_rebuilt << " - " << start_solution << ") / " << T;
					double espo = -(cost_objectiveFunction_second_fase_after_rebuilt - start_solution) / T;
					
					if (r <= exp(espo) && exp(espo) != 1) {
						//cout << "Confrona i costi sono entrato perche: " << r << " < " << exp(espo) << endl;
						start_solution_route.clear();
						//start_solution_route.shrink_to_fit();
						for (Route& s : solution_rebuilt) 
							start_solution_route.push_back(s);
						
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
				cout << "Numero iterazioni ---> " << NumeroSA << endl;
			}
			else {
				//cout << " ************ ITERAZIONE FALLITA ************************* " << endl;
				iterazioni_fallite++;
				//system("pause");
			}
			k++;
			
			if (k >= DeltaK) {
				T = (int)(alpha * T);
			
				k = 0;
			}
			//cout << " A T < " << Tmin << " devi fermarti T= " << T << " or " << iteration << " > " << iteration_max << " Ciclo Grande fuori iterazione ------> " << Iter_FirstDo << endl;
		} while ((iteration <= iteration_max) && (T > Tmin) && (NumeroSA <= 20000));
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
					// If ? la modififica
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

		weights = { peso_itermediate_stop, peso_TW, peso_transhipment }; 
		double objective_function = calculate_ObjectiveFunction(weights, Prova, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		cout << " il costo della soluzione che mi sono salvato nella mappa: " << objective_function << endl;
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
			
			vector<double> c;
			vector<vector<int>> A2;
			for (Route& r : airplane_routes[airplane.code]) {
			
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

		weights = { peso_itermediate_stop, peso_TW, peso_transhipment };
		start_solution = calculate_ObjectiveFunction(weights, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		cout << " Costo del Modello che sara nuovo costo di partenza = " << start_solution << endl;

		cout << " Best: " << best_solution << " start: " << start_solution << endl;
		if (best_solution > start_solution) {
			//auto stop_incumbent = chrono::high_resolution_clock::now();
			//auto duration_incumbent = chrono::duration_cast<chrono::seconds>(stop_incumbent - start);
			time_incumbent = difftime(time(NULL), time_now);
			cout << " Aggiorno dopo modello .. time: " << time_incumbent << endl;

		}

		outfile << "modello:;**; " << start_solution << endl;
		//outfile2.close();
		//********************************************************************************************************************************************************************



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
	weights = { peso_itermediate_stop, peso_TW, peso_transhipment }; 
	cout << calculate_ObjectiveFunction(weights, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************AGGREGAZIONE COMPLESSA***************************************************************
	cout << "ora faccio l'aggregazione complessa" << endl;
	start_solution_route = aggrezione_complex_after_model(start_solution_route, map_airplane, from_to, from_to_FuelConsumed);
	weights = { peso_itermediate_stop, peso_TW, peso_transhipment };
	cout << calculate_ObjectiveFunction(weights, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << "************************************************************************************************************************************" << endl;
	cout << "LE SOLUZIONI" << endl;

	cout << "costo fisso; costo landing; costo fuel; costo km; costo intermediate; costo tw; Total; Numero Aerei; Fuel; KM; IS; min TW;";
	cout << "costo fisso; costo landing; costo fuel; costo km; costo intermediate; costo tw; minuti TW; N_intermediate; gap_perc; time_ALNS; time_model; time_incumbent; numeroSA" << endl;
	weights = { peso_itermediate_stop, peso_TW, peso_transhipment };

	input.set_airplanes(airplanes);
	input.set_airstrips(airstrips);
	input.set_passengers(passengers_for_company_solution); 
	Company_Solution = calculationCostCompany(weights, argv[2], argv[3], input, from_to_company);
	weights = { peso_itermediate_stop, peso_TW, peso_transhipment }; 
	calculate_ObjectiveFunction_final(Company_Solution, weights, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
	
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************************************************************************************************
	tempo_finale = difftime(time(NULL), time_now);
	printTimes(time_spent_model, tempo_finale, NumeroSA, time_incumbent);

	cout << " ********** Final Weight ************ " << endl;
	cout << " Cluster || Worst || 2-Regret || Thanos " << endl;
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	cout << " One || SP || Regret || Forbidden ||Perturbation " << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;



	cout << "Time at the end: " << tempo_finale << endl;
	cout << "Time model: " << time_spent_model << endl;
	cout << "Numero SA= " << NumeroSA << endl;
	cout << "Iterazioni Fallite= " << iterazioni_fallite << endl;
	cout << "number of iteration in the heuristic: " << number_iteration_in_heuristic << endl;


	//SCOMMENTARE QUESTE PER AVERE IL COSTO CON L'ULTIMO ARCO
	//cout << "//////////////////////////////////////////// ora vengono stampate le route con l'aggiunta dell'ultimo arco, e il relativo costo////////////////////////////////////////" << endl;
	//calculate_ObjectiveFunction_final_arc_iori(Company_Solution, peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

	outfile.close(); // per il grafico++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//system("pause");
	return 0;
}
