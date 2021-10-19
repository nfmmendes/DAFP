#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>

#include "Flightleg.h"

typedef vector<vector<double>> double2DVector;
typedef vector<vector<vector<double>>> double3DVector;

template<typename type>
struct MyCOMP {
	bool operator() (
		const type& first, const type& second) const {
		return first > second;
	}

};

template<typename type>
struct MyCOMP1 {
	bool operator() (
		const type& first, const type& second) const {
		return first < second;
	}

};


double Sum(std::vector<double> Weight) {
	double sum = 0;
	for (double i : Weight) sum += i;
	return sum;
}

double Accumulated(int j, std::vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) sum += i;
	for (int i = 0; i <= j; i++) accum += ((Weight[i]) / sum);
	return accum;
}

double Ob_Funct_Improvement(double newsolution, double oldsolution) {
	double rate_max_improvement = 10000;
	if (newsolution >= oldsolution) {
		return 0;
	}
	else {
		return ((oldsolution - newsolution) / rate_max_improvement);
	}
}

double Time_Spend(double time) {
	double rate_time = 20;
	return time / rate_time;

}

//function of split
std::vector<std::string> split(std::string stringa, char separatore) {
	std::vector<std::string> words;
	std::stringstream ss(stringa);
	std::string individual_string;
	while (getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
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
						}
					}
				}
				if ((int)genitori_maschi.size() > 0) {
					//mettili con il papa
					for (Passenger& g : genitori_maschi) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time && time_arr == g.arrival_time) {
							genitori_gruppo.push_back(g);
							non_ha_genitore = false;
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
		if (index >= map_support.size() - 4) best_places.push_back(x.second);
		index++;
	}

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

//************************************************************************************************************************************************************************
//
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

#endif
