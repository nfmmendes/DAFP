#ifndef EVALUATE_H
#define EVALUATE_H

//********************************************************FUNZIONI CHE CALCOLANO I COSTI*******************************************************************************
#include <iostream>
#include <map>
#include <vector>


#include "Airplane.h"
#include "Airstrip.h"
#include "Fill.h"
#include "Passenger.h"
#include "Route.h"

double calculationCostCompany(double peso_TW, double peso_trashipment, double peso_intermediate_stop, std::string route_azienza, std::string passengers_azienda, std::vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers, map<string, double>& from_to_company) {
	vector<Route> routes_company_solution = fillRoute(route_azienza);

	map <string, Route> codice_routeAzienda;
	for (Route r : routes_company_solution) {
		//r.printCompany();
		codice_routeAzienda.insert(make_pair(r.aircraft_code_company_comparison, r));
	}



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
	//for (auto& x : airstrips_map)  x.second.print();

	map<string, Airplane> airplanes_map;
	for (int i = 0; i < (int)airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
	}

	for (int r = 0; r < (int)routes_company_solution.size(); r++) {
		double c = airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fixed_cost; //ho aggiunto il costo fisso
		//cout << "ecco il fixed cost" << airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fixed_cost << endl;;
		string places = ";";
		for (int i = 0; i < (int)routes_company_solution[r].places_company.size(); i++) {
			places += routes_company_solution[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution[r].places_company[i]].landing_cost; //aggiungo il leading cost
				//cout << "ecco il landing cost: " << airstrips_map[routes_company_solution[r].places_company[i]].landing_cost << endl;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < (int)routes_company_solution[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]];
				//cout << "ecco il costo della distanza percorsa: " << from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]] << endl;
				double time_flight = (from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]]) / airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1) * airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " costo del fuel " << cost_fuel;
				c += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
	}


	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing per la compagnia: " << costo_routing << endl;



	vector<Passenger> passengers_solution; //il code_flight qui � l'aereo
	ifstream file;
	//cout << passengers_azienda << endl;
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
		passengers_solution.push_back(p);

	}
	file.close();
	//***********************************************************************************************************************************
	//calcolo matrice A e costo della penalit� per essere fuori dall'orario previsto
	for (int p = 0; p < (int)passengers.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers[p].name.begin(), passengers[p].name.end(), passengers[p].name.begin(), ::tolower);
		transform(passengers[p].surname.begin(), passengers[p].surname.end(), passengers[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < (int)passengers_solution.size(); j++) {
			trovato_pass_name = passengers_solution[j].name.find(passengers[p].name);
			if (trovato_pass_name <= passengers_solution[j].name.size()) {
				trovato_pass_cognome = passengers_solution[j].name.find(passengers[p].surname);
				if (trovato_pass_cognome <= passengers_solution[j].name.size()) {
					pass_trovato.push_back(passengers_solution[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa � la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < (int)pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5) * (-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr) * (-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le localit� della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == passengers[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == passengers[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

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
				//cout << "la differenza �: " << best_differenza << endl;
				//cout << "il miglior from �: " << best_from << endl;
				//cout << "il miglior to �: " << best_to << endl;
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
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
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
				//cout << "la differenza �: " << best_differenza << endl;
				//cout << "il miglior from �: " << best_from << endl;
				//cout << "il miglior to �: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;

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
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
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
				//cout << "la differenza �: " << best_differenza << endl;
				//cout << "il miglior from �: " << best_from << endl;
				//cout << "il miglior to �: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;


			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'� un problema con il passeggero: ";
				passengers[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le localit� della route
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
			//cout << "la differenza �: " << best_differenza1 << endl;
			//cout << "il miglior from �: " << best_from1 << endl;
			//cout << "il miglior to �: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop * (best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le localit� della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
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

			//cout << "la differenza �: " << best_differenza2 << endl;
			//cout << "il miglior from �: " << best_from2 << endl;
			//cout << "il miglior to �: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop * (best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'� un problema con il passeggero: ";
			passengers[p].print();
			cout << "in quanto trova pi� di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += c;
	}

	costi_time_windows = costi_time_windows * peso_TW; //per valutare cosa succede al cambiare del peso dato alle time windows

	double costo_Soluzione = costi_time_windows + costi_intermediate_stop;
	for (int i = 0; i < (int)cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
		//cout << "->" << cost_route[i] << endl;
	}
	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;

	return costo_Soluzione;


};

double calculationCostCompany_three_days(double peso_TW, std::map<string, double>& from_to_FuelConsumed_company, double peso_trashipment, double peso_intermediate_stop, string route_azienza_day1, string route_azienza_day2, string route_azienza_day3, string passengers_azienda_day1, string passengers_azienda_day2, string passengers_azienda_day3, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers_day1, vector<Passenger> passengers_day2, vector<Passenger> passengers_day3, std::map<std::string, double>& from_to_company) {
	double check_fixed_cost = 0.0;



	//variabili per la stampa disaggregata dei costi
	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;
	double COSTO_transhipment = 0.0;



	//creo le routes per goni giornata
	vector<Route> routes_company_solution_day1 = fillRoute(route_azienza_day1);
	vector<Route> routes_company_solution_day2 = fillRoute(route_azienza_day2);
	vector<Route> routes_company_solution_day3 = fillRoute(route_azienza_day3);
	//for (Route r : routes_company_solution) cout << "codice volo: " << r.aircraft_code << endl;
	//system("pause");
	//creo la mappa COD_AEREO ---> route delle compagnia
	map <string, Route> codice_routeAzienda_day1;
	map <string, Route> codice_routeAzienda_day2;
	map <string, Route> codice_routeAzienda_day3;
	for (Route r : routes_company_solution_day1) {
		codice_routeAzienda_day1.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day2) {
		codice_routeAzienda_day2.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day3) {
		codice_routeAzienda_day3.insert(make_pair(r.aircraft_code_company_comparison, r));
	}

	/*for (auto x : codice_routeAzienda_day3) {
		cout << x.first << endl;
		x.second.printCompany();
	}
	system("pause");*/

	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;


	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < airstrips.size(); i++) {
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
		//cout << "- " << airstrips[i].code_string << endl;
	}
	map<string, Airplane> airplanes_map;
	for (int i = 0; i < airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
		//cout << "- " << airplanes[i].code_company << endl;
	}
	//system("pause");





	cout << "giorno 1: " << endl;
	double giorno1 = 0.0;
	for (int r = 0; r < routes_company_solution_day1.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day1[r].places_company.size(); i++) {
			places += routes_company_solution_day1[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day1[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1) * airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno1 += c;
	}
	cout << "costo --> " << giorno1 << endl;
	cout << "giorno 2: " << endl;
	double giorno2 = 0.0;
	for (int r = 0; r < routes_company_solution_day2.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day2[r].places_company.size(); i++) {
			places += routes_company_solution_day2[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day2[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1) * airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno2 += c;
	}
	cout << "costo --> " << giorno2 << endl;
	cout << "giorno 3: " << endl;
	double giorno3 = 0.0;
	for (int r = 0; r < routes_company_solution_day3.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day3[r].places_company.size(); i++) {
			places += routes_company_solution_day3[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day3[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1) * airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno3 += c;
	}
	cout << "costo --> " << giorno3 << endl;



	for (Route r1 : routes_company_solution_day1) {
		double c = airplanes_map[r1.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r1.aircraft_code_company_comparison].fixed_cost;

		for (Route r2 : routes_company_solution_day2) {
			if (r1.aircraft_code_company_comparison == r2.aircraft_code_company_comparison) {
				if (r1.places_company[r1.index - 1] != r2.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += from_to_FuelConsumed_company[r1.aircraft_code_company_comparison + ";" + r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
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
	for (Route r2 : routes_company_solution_day2) {
		double c = airplanes_map[r2.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;

		for (Route r3 : routes_company_solution_day3) {
			if (r2.aircraft_code_company_comparison == r3.aircraft_code_company_comparison) {
				if (r2.places_company[r2.index - 1] != r3.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------>" << from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------------->" << from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
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
	for (Route r3 : routes_company_solution_day3) {
		double c = airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
		check_fixed_cost += airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
	}
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
	//cout << passengers_azienda << endl;
	file_day1.open(passengers_azienda_day1);
	if (file_day1.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day1.eof()) {
		string row;
		getline(file_day1, row);
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
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day1[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day1.size(); j++) {
			trovato_pass_name = passengers_solution_day1[j].name.find(passengers_day1[p].name);
			if (trovato_pass_name <= passengers_solution_day1[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day1[j].name.find(passengers_day1[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day1[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day1[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day1[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day1[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day1[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5) * (-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day1[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day1[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr) * (-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
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
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
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
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day1[p].print();
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
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
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
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop * (best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
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

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop * (best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day1[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



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
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day2[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day2.size(); j++) {
			trovato_pass_name = passengers_solution_day2[j].name.find(passengers_day2[p].name);
			if (trovato_pass_name <= passengers_solution_day2[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day2[j].name.find(passengers_day2[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day2[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day2[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day2[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day2[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day2[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5) * (-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day2[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day2[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr) * (-1);
					}

					//cout << "- time: " << c << endl;
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
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
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
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
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
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop * (best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
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

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop * (best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day2[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c * peso_TW);
	}
	//**************************************************end_day2*********************************************************************************
	//********************************************_day3*****************************************************************************************
	vector<Passenger> passengers_solution_day3; //il code_flight qui è l'aereo
	ifstream file_day3;
	//cout << passengers_azienda << endl;
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
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day3[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day3.size(); j++) {
			trovato_pass_name = passengers_solution_day3[j].name.find(passengers_day3[p].name);
			if (trovato_pass_name <= passengers_solution_day3[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day3[j].name.find(passengers_day3[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day3[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day3[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day3[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day3[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day3[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
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

					//cout << "- time: " << c << endl;
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
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
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
					std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << std::endl;
					system("pause");
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
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day3[p].print();
				std::cout << "in quanto la sua soluzione non trova from e to nel volo" << std::endl;
				system("pause");
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
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << std::endl;
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
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop * (best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			std::vector<int> int_from2; //vettore con tutti i from
			std::vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				std::cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << std::endl;
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

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop * (best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			std::cout << "c'è un problema con il passeggero: ";
			passengers_day3[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c * peso_TW);
	}
	//**************************************************end_day3*********************************************************************************





	costo_Soluzione += costi_time_windows + costi_intermediate_stop;
	COSTO_tw += costi_time_windows;
	COSTO_intermediate += costi_intermediate_stop;
	COSTO_intermediate = COSTO_intermediate - COSTO_transhipment;
	for (int i = 0; i < cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
		//cout << "->" << cost_route[i] << endl;
	}





	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;


	//******************************************************************************DEVI METTERE QUA LA STAMPA DISAGGREGATA******************************************************************************
	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;n° transhipment;costo_totale" << std::endl;
	std::cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / airplanes_map[airplanes[0].code_company].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / peso_TW) << ";" << (COSTO_transhipment / peso_trashipment) << ";" << costo_Soluzione << ";";




	return costo_Soluzione;


};

double cost_single_route(double peso_TW, double peso_intermediate_stop, Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//double cost = 0.0;
	double cost = map_airplane[r.aircraft_code].fixed_cost;
	//for (int i = 1; i < r.index; i++) cost += map_airstrip[r.places[i]].landing_cost;

	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	//double first_hour = 0.0;
	//double second_hour = 0.0;
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
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double calculate_ObjectiveFunction(double peso_TW, double peso_intermediate_stop, vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		//cout << "Route-----" << endl;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}

		//cout << "costo fisso: " << cost << endl;

		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
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
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
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
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
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

	//cout << "*********************************************************dentro alla funzione*********************************************************" << endl;
	//cout << "indice del vettore degli arrivi " << r.time_arr.size();



	for (const Passenger& p : r.passengers_in_route) {
		//cout << " Indice from: " << p.solution_from << endl;
		//cout << " Indice to: " << p.solution_to << endl;
		//r.print();

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

		cost += (TW_departure + TW_arrival) * p.capacity;
	}

	//cout << "*********************************************************fine dentro funzione*********************************************************" << endl;
	cost = cost * peso_TW;

	return cost;
}

double cost__for_route_passenger_destroyCluster(Route& r, Passenger& p, int fattore_inter_stop, double peso_TW) {
	double cost = 0.0;

	//cout << "**************************************" << endl;
	//cout << "r.time_arr.size()" << r.time_arr.size();
	//p.print();

	//double time_departure = r.time_arr[p.solution_from];
	//double time_arrival = r.time_arr[p.solution_to];

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
	//double costoWorstNode = 0.00;
	//int WorstNode = -1;
	map<double, int> Node;
	map<int, int> NodeOrder;
	set<double, MyCOMP<double>> Myset;
	if (route.index <= 2) {
		//route.print();
		Node.insert(make_pair(100000, 1));
		Myset.insert(100000);
		//cout << " Route troppo piccola fisso indice a " << 1 << endl;
	}
	else {
		for (int n = 1; n < route.index; n++) {
			double dist = 0.0;
			double cost_IS = 0.0;
			vector<Passenger> PassengerNodo;
			if (n != (route.index - 1)) {
				//cout << " Calcolo per nodo n mezzo " << endl;
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
					//****************************************************************************************************************************************


				}
			}
			else {
				//cout << " Calcolo per ultimo nodo " << endl;
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]];
				for (int p = 0; p < (int)route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}


					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += (peso_intermediate_stop)*route.passengers_in_route[p].capacity;
						}
					}
					//****************************************************************************************************************************************

				}


			}


			dist += (cost_time_windows_for_node(route, PassengerNodo, peso_TW)) + cost_IS;
			// / PassengerNodo.size());
			//cout << " Inserisco nodo: " << n << " con il costo: " << dist << endl;
			Node.insert(make_pair(dist, n));
			Myset.insert(dist);
			//cout << " ************** Nodo: " << n << " Ha un costo: " << dist << endl;
			//cout << " ************** Nodo Peggiore finora: " << WorstNode << endl;
			//if (costoWorstNode < dist) {
				//costoWorstNode = dist;
				//WorstNode = n;
			//}
		}
	}

	//cout << " **************Stampo la mappa non ordinata: "<< endl;
	//for (auto x : Node) cout << x.first << " -----> " << x.second << endl;
	//cout << " **************Stampo il Set: " << endl;
	//for (auto setval : Myset) {
		//cout << setval << endl;
	//}

	int i = 0;
	for (auto setval : Myset) {
		NodeOrder.insert(make_pair(i, Node[setval]));
		i++;
	}
	//cout << " **************Stampo la mappa Ordinata: " << endl;
	//for (auto x : NodeOrder) cout << x.first << " -----> " << x.second << endl;
	return NodeOrder;
}



//calcola il valore della funzione obiettivo sui 3 giorni
double calculate_ObjectiveFunction_complete_after_rolling(double peso_TW, double peso_intermediate_stop, vector<Route>& solution_day1, vector<Route>& solution_day2, vector<Route>& solution_day3, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
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
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
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
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
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
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
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


	//cout << "costo delle time window e dell'intermediate stop: " << cost_time_intermediate << endl;
	return cost;
}


#endif