#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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


inline double Sum(std::vector<double> Weight) {
	double sum = 0;
	for (double i : Weight) sum += i;
	return sum;
}

inline double Accumulated(int j, std::vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) sum += i;
	for (int i = 0; i <= j; i++) accum += ((Weight[i]) / sum);
	return accum;
}

inline double Ob_Funct_Improvement(double newsolution, double oldsolution) {
	double rate_max_improvement = 10000;
	if (newsolution >= oldsolution) {
		return 0;
	}
	else {
		return ((oldsolution - newsolution) / rate_max_improvement);
	}
}

inline double Time_Spend(double time) {
	double rate_time = 20;
	return time / rate_time;

}

//function of split
inline std::vector<std::string> split(std::string stringa, char separatore) {
	std::vector<std::string> words;
	std::stringstream ss(stringa);
	std::string individual_string;
	while (getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
}


inline void fix_key_passenger_for_regret(vector<Passenger>& passengers_day1, vector<Passenger>& passengers_day2, vector<Passenger>& passengers_day3) {

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


inline void Build_structure_Model(vector<Airplane> airplane, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
	int tipo = 0;
	for (int j = 0; j < (int)airplane.size(); j++) {
		auto it = mappa_aereo_tipo.find(airplane[j].model + to_string(airplane[j].depot));
		if (it == mappa_aereo_tipo.end()) {
			mappa_aereo_tipo.insert(make_pair(airplane[j].model + to_string(airplane[j].depot), tipo));
			tipo++;
		}
	}
	for (int j = 0; j < (int)airplane.size(); j++) {
		auto it = mappa_aereo_tipo.find(airplane[j].model + to_string(airplane[j].depot));
		codice_aereo_tipo.insert(make_pair(airplane[j].code, it->second));
	}
	for (auto j : codice_aereo_tipo) {
		auto it = tipo_numero.find(j.second);
		if (it == tipo_numero.end()) tipo_numero.insert(make_pair(j.second, 1));
		else tipo_numero[j.second] += 1;
	}

	for (auto it = mappa_aereo_tipo.begin(); it != mappa_aereo_tipo.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
	}
	for (auto it = codice_aereo_tipo.begin(); it != codice_aereo_tipo.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
	}
	for (auto it = tipo_numero.begin(); it != tipo_numero.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
	}


}

//bambini splittati
inline void unione_children_INTELLIGENTE(vector<Passenger>& passengers, vector<Passenger>& passengers_for_company_solution) {
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
					std::cout << "errore, ci sono dei bambini senza genitore" << endl;
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
					std::cout << "c'e' un errore, controlla" << endl;
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
		std::cout << endl;
		std::cout << endl;
		std::cout << endl;
		std::cout << endl;
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
inline int best_location(map <int, int>& location_request) {
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
inline int best_location_4place(map<int, int>& location_request, vector<vector<double>>& from_to, vector<Airstrip>& airstrips) {
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

#endif
