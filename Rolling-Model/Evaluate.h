#ifndef EVALUATE_H
#define EVALUATE_H

#include <iostream>
#include <map>
#include <set>
#include <vector>


#include "Airplane.h"
#include "Airstrip.h"
#include "Fill.h"
#include "Passenger.h"
#include "Route.h"
#include "ProcessedInput.h"

struct Input
{
	std::vector<Airstrip> airstrips;
	vector<Airplane> airplanes;
	vector<Passenger> passengers;
};

void printError1(vector<Passenger> passengers_day3, int p)
{
	std::cout << "Error 1\n";
	std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
	passengers_day3[p].print();
	std::cout << "in quanto la sua soluzione non trova from e to nel volo" << std::endl;
	system("pause");
}

void print_error_2()
{
	std::cout << "Error 2\n";
	std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << std::endl;
	system("pause");
}

void print_error_3(vector<Passenger> pass_trovato)
{
	std::cout << "Error 3\n";
	std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << std::endl;
	pass_trovato[0].print();
}

void print_error_4(vector<Passenger> pass_trovato)
{
	std::cout << "Error 4\n";
	std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << std::endl;
	pass_trovato[1].print();
	system("pause");
}

void print_error_5(vector<Passenger> passengers_day3, int p)
{
	std::cout << "Error 5\n";
	std::cout << "c'è un problema con il passeggero: ";
	passengers_day3[p].print();
	cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
	system("pause");
}

void print_error_6(vector<Passenger> passengers, int p)
{
	std::cout << "Error 6\n";
	cout << "ce un problema con il passeggero: ";
	passengers[p].print();
	cout << "in quanto trova pi� di due corrispondenze nelle soluzioni" << endl;
	system("pause");
}

void print_error_7(vector<Passenger> pass_trovato)
{
	std::cout << "Error 7\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
	pass_trovato[1].print();
	system("pause");
}

void print_error_8(vector<Passenger> pass_trovato)
{
	std::cout << "Error 8\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
	pass_trovato[0].print();
}

void print_error_9(vector<Passenger> passengers, int p)
{
	std::cout << "Error 9\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'� un problema con il passeggero: ";
	passengers[p].print();
	cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
	system("pause");
}

void print_error_10()
{
	std::cout << "Error 10\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
	system("pause");
}

void print_error_11()
{
	std::cout << "Error 11\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
	system("pause");
}

void print_error_12(vector<Passenger> pass_trovato)
{
	std::cout << "Error 12\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
	pass_trovato[1].print();
	system("pause");
}

void print_error_13(vector<Passenger> passengers_day1, int p)
{
	std::cout << "Error 13\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
	passengers_day1[p].print();
	cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
	system("pause");
}

void print_error_14()
{
	std::cout << "Error 14\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
	system("pause");
}

void print_error_15()
{
	std::cout << "Error 15\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
	system("pause");
}

void print_error_16(vector<Passenger> pass_trovato)
{
	std::cout << "Error 16\n";
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
	pass_trovato[0].print();
}

void print_error_17(vector<Passenger> passengers_day1, int p)
{
	cout << "c'è un problema con il passeggero: ";
	passengers_day1[p].print();
	cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
	system("pause");
}

void print_error_18()
{
	cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
	system("pause");
}

void manage_transhipment(map<string, Route>& codice_routeAzienda_day2, vector<Passenger>& pass_trovato, vector<int>& int_to, int& best_from, int& best_to)
{
	vector<int> int_from_soluz;
	for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
		if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
			int_from_soluz.push_back(i);
		}
	}
	if (int_from_soluz.empty()) {
		print_error_18();
	}

	double best_differenza = DBL_MAX;
	best_from = -1;
	best_to = -1;
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
}

void initialize_code_route_company(vector<Route>& routes_day1, vector<Route>& routes_solution_day2, vector<Route>& routes_day3, map<string, Route>& codice_day1, map<string, Route>& codice_day2, map<string, Route>& codice_day3)
{
	for (Route r : routes_day1) {
		codice_day1.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_solution_day2) {
		codice_day2.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_day3) {
		codice_day3.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
}

double print_error_20()
{
	cerr << "Error Opening File passenger azienda day 1" << endl;
	system("pause");
	exit(1);
}

void find_best_difference_1(vector<int> &int_from, vector<int>& int_to_soluz, int& best_from, int& best_to)
{
	double best_differenza = DBL_MAX;
	best_from = -1;
	best_to = -1;
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
}

void find_best_difference_2(vector<int> &int_from, vector<int> &int_to, int& best_from, int& best_to)
{
	double best_differenza = DBL_MAX;
	best_from = -1;
	best_to = -1;
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
}


void cost_days_iterated(std::map<std::string, double>& from_to_company, double& COSTO_landing, double& COSTO_fuel, double& COSTO_km, vector<Route>& routes_day1, vector<Route>& routes_day2, vector<Route>& routes_day3, vector<double>& cost_route, map<string, Airstrip>& airstrips_map, map<string, Airplane>& airplanes_map)
{
	cout << "giorno 1: " << endl;
	double giorno1 = 0.0;
	for (int r = 0; r < routes_day1.size(); r++) {
		double c = 0.0;

		string places = ";";
		for (int i = 0; i < routes_day1[r].places_company.size(); i++) {
			auto* route = &routes_day1[r];
			string aircraft_code = route->aircraft_code_company_comparison;
			places += route->places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				c += airstrips_map[route->places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[route->places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_day1[r].places_company.size() - 1) {
				
				c += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				COSTO_km += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				double time_flight = (from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]]) / airplanes_map[aircraft_code].speed;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[aircraft_code].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[aircraft_code].fuel_burn_first + (time_flight - 1) * airplanes_map[aircraft_code].fuel_burn_second;
				}

				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}

		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno1 += c;
	}
	cout << "costo --> " << giorno1 << endl;
	cout << "giorno 2: " << endl;
	double giorno2 = 0.0;
	for (int r = 0; r < routes_day2.size(); r++) {
		auto* route = &routes_day2[r];
		string aircraft_code = route->aircraft_code_company_comparison;
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < route->places_company.size(); i++) {
			places += route->places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[route->places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[route->places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < route->places_company.size() - 1) {
				c += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				COSTO_km += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				double time_flight = (from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]]) / airplanes_map[aircraft_code].speed;

				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[aircraft_code].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[aircraft_code].fuel_burn_first + (time_flight - 1) * airplanes_map[aircraft_code].fuel_burn_second;
				}

				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}

		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno2 += c;
	}
	cout << "costo --> " << giorno2 << endl;
	cout << "giorno 3: " << endl;
	double giorno3 = 0.0;
	for (int r = 0; r < routes_day3.size(); r++) {
		auto* route = &routes_day3[r];
		string aircraft_code = route->aircraft_code_company_comparison;
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		string places = ";";
		for (int i = 0; i < routes_day3[r].places_company.size(); i++) {
			places += routes_day3[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				c += airstrips_map[route->places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[route->places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < route->places_company.size() - 1) {
				c += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				COSTO_km += from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]];
				double time_flight = (from_to_company[route->places_company[i] + ";" + route->places_company[i + 1]]) / airplanes_map[aircraft_code].speed;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[aircraft_code].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[aircraft_code].fuel_burn_first + (time_flight - 1) * airplanes_map[aircraft_code].fuel_burn_second;
				}

				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}

		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno3 += c;
	}
	cout << "costo --> " << giorno3 << endl;
}

void iterate_routes_by_day(std::map<string, double>& from_to_FuelConsumed, std::map<std::string, double>& from_to_company, double& check_fixed_cost, vector<Route>& routes_day1, vector<Route>& routes_day2, vector<Route>& routes_day3, vector<double>& cost_route, map<string, Airstrip>& airstrips_map, map<string, Airplane>& airplanes_map)
{
	for (Route r1 : routes_day1) {
		double c = airplanes_map[r1.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r1.aircraft_code_company_comparison].fixed_cost;

		for (Route r2 : routes_day2) {
			if (r1.aircraft_code_company_comparison == r2.aircraft_code_company_comparison) {
				if (r1.places_company[r1.index - 1] != r2.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += from_to_FuelConsumed[r1.aircraft_code_company_comparison + ";" + r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += airstrips_map[r2.places_company[0]].landing_cost;
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r2 : routes_day2) {
		double c = airplanes_map[r2.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;

		for (Route r3 : routes_day3) {
			if (r2.aircraft_code_company_comparison == r3.aircraft_code_company_comparison) {
				if (r2.places_company[r2.index - 1] != r3.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					c += from_to_FuelConsumed[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					c += airstrips_map[r3.places_company[0]].landing_cost;
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r3 : routes_day3) {
		double c = airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
		check_fixed_cost += airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
	}
}

double calculationCostCompany_three_days(double peso_TW, std::map<string, double>& from_to_FuelConsumed_company, double peso_trashipment, double peso_intermediate_stop, string route_azienza_day1, string route_azienza_day2, string route_azienza_day3, string passengers_azienda_day1, string passengers_azienda_day2, string passengers_azienda_day3, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers_day1, vector<Passenger> passengers_day2, vector<Passenger> passengers_day3, std::map<std::string, double>& from_to_company) {
	
	double check_fixed_cost = 0.0;
	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;
	double COSTO_transhipment = 0.0;

	vector<Route> routes_company_solution_day1 = fillRoute(route_azienza_day1);
	vector<Route> routes_company_solution_day2 = fillRoute(route_azienza_day2);
	vector<Route> routes_company_solution_day3 = fillRoute(route_azienza_day3);

	//creo la mappa COD_AEREO ---> route delle compagnia
	map <string, Route> codice_routeAzienda_day1;
	map <string, Route> codice_routeAzienda_day2;
	map <string, Route> codice_routeAzienda_day3;
	initialize_code_route_company(routes_company_solution_day1,
								  routes_company_solution_day2,
								  routes_company_solution_day3,
								  codice_routeAzienda_day1,
								  codice_routeAzienda_day2,
								  codice_routeAzienda_day3);

	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;

	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < airstrips.size(); i++) {
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
	}
	map<string, Airplane> airplanes_map;
	for (int i = 0; i < airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
	}

	cost_days_iterated(from_to_company,
					   COSTO_landing,
					   COSTO_fuel,
					   COSTO_km,
					   routes_company_solution_day1,
					   routes_company_solution_day2,
					   routes_company_solution_day3,
					   cost_route,
					   airstrips_map,
					   airplanes_map);

	iterate_routes_by_day(from_to_FuelConsumed_company,
						  from_to_company,
						  check_fixed_cost,
						  routes_company_solution_day1,
						  routes_company_solution_day2,
						  routes_company_solution_day3,
						  cost_route,
						  airstrips_map,
						  airplanes_map);
	//ricordati di aggiungere il costo fisso più quello dell'arco aggiuntivo
	//se uso un nuovo aereo metto il costo fisso se uso quello vecchio pago il tratto aggiuntivo se manca
	cout << "il costo fisso di uso degli aerei e' " << check_fixed_cost << endl;
	COSTO_fisso += check_fixed_cost;
	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing + costo fisso + eventuali archi aggiuntivi per unire gli aerei per la compagnia: " << costo_routing << endl;
	double costo_Soluzione = 0.0;


	//********************************************_day1*****************************************************************************************
	vector<Passenger> passengers_solution_day1; //il code_flight qui è l'aereo
	ifstream file_day1;
	file_day1.open(passengers_azienda_day1);
	if (file_day1.fail()) {
		return print_error_20();
	}
	while (!file_day1.eof()) {
		string row;
		getline(file_day1, row);
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
		passengers_solution_day1.push_back(p);
	}

	file_day1.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day1.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day1[p].name.begin(), passengers_day1[p].name.end(), passengers_day1[p].name.begin(), ::tolower);
		transform(passengers_day1[p].surname.begin(), passengers_day1[p].surname.end(), passengers_day1[p].surname.begin(), ::tolower);

		for (int j = 0; j < passengers_solution_day1.size(); j++) {
			trovato_pass_name = passengers_solution_day1[j].name.find(passengers_day1[p].name);
			if (trovato_pass_name <= passengers_solution_day1[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day1[j].name.find(passengers_day1[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day1[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day1[j]);
					controllo += 1;
				}
			}
		}

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day1[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day1[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						c += (differenza_dep + 5) * (-1);
					}
				}
				if (pass_trovato[z].arrival_location_company == passengers_day1[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day1[p].arrival_time - pass_trovato[z].arrival_time;
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
			
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				int best_from;
				int best_to;
				find_best_difference_2(int_from, int_to, best_from, best_to);
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					print_error_15();
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
				COSTO_transhipment += peso_trashipment;
			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					print_error_14();
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
				COSTO_transhipment += peso_trashipment;
			}
			else {
				print_error_13(passengers_day1, p);
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				print_error_16(pass_trovato);
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
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				print_error_12(pass_trovato);
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
		}
		else if (controllo > 2) {
			print_error_17(passengers_day1, p);
		}

		costi_time_windows += (c * peso_TW);
	}
	//**************************************************end_day1*********************************************************************************
	//********************************************_day2*****************************************************************************************
	vector<Passenger> passengers_solution_day2; //il code_flight qui è l'aereo
	ifstream file_day2;
	//cout << passengers_azienda << endl;
	file_day2.open(passengers_azienda_day2);
	if (file_day2.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day2.eof()) {
		string row;
		getline(file_day2, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
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
		passengers_solution_day2.push_back(p);

	}
	file_day2.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day2.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day2[p].name.begin(), passengers_day2[p].name.end(), passengers_day2[p].name.begin(), ::tolower);
		transform(passengers_day2[p].surname.begin(), passengers_day2[p].surname.end(), passengers_day2[p].surname.begin(), ::tolower);

		for (int j = 0; j < passengers_solution_day2.size(); j++) {
			trovato_pass_name = passengers_solution_day2[j].name.find(passengers_day2[p].name);
			if (trovato_pass_name <= passengers_solution_day2[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day2[j].name.find(passengers_day2[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day2[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day2[j]);
					controllo += 1;
					//passengers_solution_day2[j].print();
				}
			}
		}

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day2[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day2[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						c += (differenza_dep + 5) * (-1);
					}
				}
				if (pass_trovato[z].arrival_location_company == passengers_day2[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day2[p].arrival_time - pass_trovato[z].arrival_time;
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
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].arrival_location_company) {
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
				int best_from;
				int best_to;
				manage_transhipment(codice_routeAzienda_day2, pass_trovato, int_to, best_from, best_to);

				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				int best_from;
				int best_to;
				find_best_difference_1(int_from, int_to_soluz, best_from, best_to);

				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day2[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo

				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
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

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[1].code_flight].index; i++) {
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
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

		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day2[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}

		costi_time_windows += (c * peso_TW);
	}
	//**************************************************end_day2*********************************************************************************
	//********************************************_day3*****************************************************************************************
	vector<Passenger> passengers_solution_day3; //il code_flight qui è l'aereo
	ifstream file_day3;

	file_day3.open(passengers_azienda_day3);
	if (file_day3.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day3.eof()) {
		string row;
		getline(file_day3, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
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
		passengers_solution_day3.push_back(p);

	}
	file_day3.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day3.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day3[p].name.begin(), passengers_day3[p].name.end(), passengers_day3[p].name.begin(), ::tolower);
		transform(passengers_day3[p].surname.begin(), passengers_day3[p].surname.end(), passengers_day3[p].surname.begin(), ::tolower);

		for (int j = 0; j < passengers_solution_day3.size(); j++) {
			trovato_pass_name = passengers_solution_day3[j].name.find(passengers_day3[p].name);
			if (trovato_pass_name <= passengers_solution_day3[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day3[j].name.find(passengers_day3[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day3[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day3[j]);
					controllo += 1;
				}
			}

		}

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day3[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day3[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						c += (differenza_dep + 5) * (-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day3[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day3[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
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
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].arrival_location_company) {
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
				std::vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << std::endl;
					system("pause");
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
				COSTO_transhipment += peso_trashipment;
			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				std::vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					print_error_2();
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
				COSTO_transhipment += peso_trashipment;

			}
			else {
				printError1(passengers_day3, p);
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			std::vector<int> int_from1; //vettore con tutti i from
			std::vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				print_error_3(pass_trovato);
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

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			std::vector<int> int_from2; //vettore con tutti i from
			std::vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				print_error_4(pass_trovato);
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
			print_error_5(passengers_day3, p);
		}
		costi_time_windows += (c * peso_TW);
	}

	costo_Soluzione += costi_time_windows + costi_intermediate_stop;
	COSTO_tw += costi_time_windows;
	COSTO_intermediate += costi_intermediate_stop;
	COSTO_intermediate = COSTO_intermediate - COSTO_transhipment;
	for (int i = 0; i < cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
	}

	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;

	//******************************************************************************DEVI METTERE QUA LA STAMPA DISAGGREGATA******************************************************************************
	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;n° transhipment;costo_totale" << std::endl;
	std::cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / airplanes_map[airplanes[0].code_company].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / peso_TW) << ";" << (COSTO_transhipment / peso_trashipment) << ";" << costo_Soluzione << ";";

	return costo_Soluzione;
};

double cost_single_route(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, Route& r) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	double cost = map_airplane[r.aircraft_code].fixed_cost;
	//for (int i = 1; i < r.index; i++) cost += map_airstrip[r.places[i]].landing_cost;

	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;

	for (int i = 0; i < r.index; i++) {
		if (i >= 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
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


	cost += mileage;
	cost += fuel_consumed;

	for (const auto& p : r.passengers_in_route) {

		cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double calculate_ObjectiveFunction(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, vector<Route>& solution) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
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
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

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
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

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
	if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
	if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW + fattore_inter_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;

	return cost;
}

double cost_time_windows_for_node(Route& r, vector<Passenger>& pass, double peso_TW) {
	double cost = 0.0;
	for (const Passenger& p : pass) {
		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

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
	
	if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
	else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

	return cost;
}

map<int, int> Compute_WorstNode(double peso_TW, double peso_intermediate_stop, Route& route, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
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
					if (route.passengers_in_route[p].solution_to == n || route.passengers_in_route[p].solution_from == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}

					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += (peso_intermediate_stop)*route.passengers_in_route[p].capacity;
						}
					}
				}
			}
			else {
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]];
				for (int p = 0; p < (int)route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}

					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += (peso_intermediate_stop)*route.passengers_in_route[p].capacity;
						}
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

//calcola il valore della funzione obiettivo sui 3 giorni
double calculate_ObjectiveFunction_complete_after_rolling(ProcessedInput* input, double peso_TW, double peso_intermediate_stop, vector<Route>& solution_day1, vector<Route>& solution_day2, vector<Route>& solution_day3) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	double cost = 0.0;

	double cost_time_intermediate = 0.0;
	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;

	for (Route& r : solution_day1) {

		if (r.primo_pass && !r.airplane_day_before) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
			//cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}

		double mileage = 0.0;
		double fuel_consumed = 0.0;

		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
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

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		COSTO_km += mileage;
		cost += fuel_consumed;
		COSTO_fuel += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto& p : r.passengers_in_route) {
			cost += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_time_intermediate += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;
			COSTO_intermediate += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += TW_departure + TW_arrival;
			cost_time_intermediate += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;

		}
	}

	for (Route& r : solution_day2) {

		if (r.primo_pass && !r.airplane_day_before) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
		}

		double mileage = 0.0;
		double fuel_consumed = 0.0;

		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
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
		COSTO_km += mileage;
		cost += fuel_consumed;
		COSTO_fuel += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto& p : r.passengers_in_route) {
			cost += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_time_intermediate += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;
			COSTO_intermediate += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += TW_departure + TW_arrival;
			cost_time_intermediate += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;
		}
	}

	for (Route& r : solution_day3) {

		if (r.primo_pass && !r.airplane_day_before) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
		}

		double mileage = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
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
		COSTO_km += mileage;
		cost += fuel_consumed;
		COSTO_fuel += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto& p : r.passengers_in_route) {
			cost += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_time_intermediate += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;
			COSTO_intermediate += (p.solution_to - p.solution_from - 1) * peso_intermediate_stop;

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += TW_departure + TW_arrival;
			cost_time_intermediate += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;

		}
	}

	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;costo_totale" << endl;
	cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / map_airplane[solution_day1[1].aircraft_code].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / peso_TW) << ";" << cost << endl;

	return cost;
}


#endif