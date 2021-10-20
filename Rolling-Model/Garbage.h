
//funzione che TUTTI GLI ERRORI
#include <iostream>
#include <map>
#include <vector>


#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include "Evaluate.h"

void stampo_caso_strano(std::map<int, Airstrip> map_airstrip, int peso_TW, int peso_intermediate_stop, vector<vector<double>> from_to_costi, vector<vector<vector<double>>> from_to_fuel_costi, vector<Passenger> passengers, vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa) {

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
						std::cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << std::endl;
						r.print();
						system("pause");
						std::cin.get();
					}

				}
			}

		}

	}
}


double calculationCostCompany(double peso_TW, double peso_trashipment, double peso_intermediate_stop, std::string route_azienza, std::string passengers_azienda, Input* input, map<string, double>& from_to_company) {

	std::vector<Airstrip> airstrips = input->airstrips;
	vector<Airplane> airplanes = input->airplanes;
	vector<Passenger> passengers = input->passengers;

	vector<Route> routes_company_solution = fillRoute(route_azienza);

	map <string, Route> codice_routeAzienda;
	for (Route r : routes_company_solution) {
		codice_routeAzienda.insert(make_pair(r.aircraft_code_company_comparison, r));
	}

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
		double c = airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fixed_cost; //ho aggiunto il costo fisso
		string places = ";";
		for (int i = 0; i < (int)routes_company_solution[r].places_company.size(); i++) {
			places += routes_company_solution[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				c += airstrips_map[routes_company_solution[r].places_company[i]].landing_cost; //aggiungo il leading cost
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < (int)routes_company_solution[r].places_company.size() - 1) {
				c += from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]]) / airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].speed;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1) * airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_second;
				}

				c += cost_fuel;
			}
		}

		cost_route.push_back(c);
	}

	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing per la compagnia: " << costo_routing << endl;

	vector<Passenger> passengers_solution; //il code_flight qui � l'aereo
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

		//questa � la parte del calcolo delle time window
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
			//scorro tutte le localit� della route
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
					print_error_11();
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
					print_error_10();
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
			}
			else {
				print_error_9(passengers, p);
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
				print_error_8(pass_trovato);
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
			//scorro tutte le localit� della route
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
				print_error_7(pass_trovato);
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
			print_error_6(passengers, p);
		}
		costi_time_windows += c;
	}

	costi_time_windows = costi_time_windows * peso_TW; //per valutare cosa succede al cambiare del peso dato alle time windows

	double costo_Soluzione = costi_time_windows + costi_intermediate_stop;
	for (int i = 0; i < (int)cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
	}
	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;

	return costo_Soluzione;
};

