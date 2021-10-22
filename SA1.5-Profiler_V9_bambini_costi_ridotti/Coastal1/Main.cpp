#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <set>
#include <random>
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include <chrono>

#include "Constructive.h"
#include "Destroy.h"
#include "Fill.h"
#include "Model_Cplex.h"
#include "Move.h"
#include "Repair.h"
#include "Util.h"
#include "Swap.h"

using namespace std;

//const int numero_airstrip_const = 58;
//const int numero_airplane_const = 36;


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

//************************************************************************************************************************************************************************

//data una location mi restituisce la location pi? vicina
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
	//ora faccio la mappa che ha chiave codiceAereo;from-to --> fuel consumed per quel tratto con quell'aereo
	//cout << "test: " << location_fuel[2][45] << endl;
	//system("pause");
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




	vector <Passenger> passengers_per_casostrano = passengers;
	vector<Passenger> passengers_solution = passengers;
	double Company_Solution = calculationCostCompany(peso_TW, peso_transhipment, peso_itermediate_stop, argv[2], argv[3], airstrips, airplanes, passengers_for_company_solution, from_to_company);
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
			//cout << " heuristic_choice =  " << heuristic_choice << endl;
			//for (int i = 0; i < NHeuristic; i++) cout << Accumulated(i, Weigth_heuristic) << " || ";
			//cout << endl;
			int npass = 0;
			do {
				//auto rng1 = default_random_engine{};
				random_shuffle(begin(passengers), end(passengers));

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					//cout << "*************** I am using the Parallel Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
					choosen_heuristic = 0;
				}
				else {
					//cout << "*************** I am using the Sequential Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase_sequential(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
					choosen_heuristic = 1;
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
		//cout << "ho appena fatto il costruttivo" << endl;
		//stampo_caso_strano(map_airstrip,peso_TW, peso_itermediate_stop, from_to,from_to_FuelConsumed, passengers_per_casostrano, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
		solutionAll.push_back(start_solution_route);
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
			auto start = chrono::high_resolution_clock::now();
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
			
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;
			//cout << " repair_choice =  " << repair_choice << endl;
			//for (int i = 0; i < Weigth_repair.size(); i++) cout << Accumulated(i, Weigth_repair) << " || ";
			//cout << endl;




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
				//QUA C'Eè IL MOVE
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


						//auto stop_incumbent = chrono::high_resolution_clock::now();
						//auto duration_incumbent = chrono::duration_cast<chrono::seconds>(stop_incumbent - start);
						time_incumbent = difftime(time(NULL), time_now);

					}
				}
				else {

					r = double(rand()) / (double(RAND_MAX) + 1.0);
					//cout << " Ho generato numero random " << r << endl;
					//cout << "-( " << cost_objectiveFunction_second_fase_after_rebuilt << " - " << start_solution << ") / " << T;
					double espo = -(cost_objectiveFunction_second_fase_after_rebuilt - start_solution) / T;
					//cout << " Adesso ho generato espo " << espo << endl;
					//cout << " Se non lo ? devo incrementare iterazione se lo ? aggiorno valori X con X'  " << endl;
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

		cout << " Best: " << best_solution << " start: " << start_solution << endl;
		if (best_solution > start_solution) {
			//auto stop_incumbent = chrono::high_resolution_clock::now();
			//auto duration_incumbent = chrono::duration_cast<chrono::seconds>(stop_incumbent - start);
			time_incumbent = difftime(time(NULL), time_now);
			cout << " Aggiorno dopo modello .. time: " << time_incumbent << endl;

		}


		//cout << "ho appena fatto il modello" << endl;
		//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
		//scrivo sul file del grafico il valore del modello*******************************************************************************************************************
		//ofstream outfile2;
		//outfile2.open("grafico.txt");
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
	cout << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************AGGREGAZIONE COMPLESSA***************************************************************
	cout << "ora faccio l'aggregazione complessa" << endl;
	start_solution_route = aggrezione_complex_after_model(start_solution_route, map_airplane, from_to, from_to_FuelConsumed);
	cout << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << "************************************************************************************************************************************" << endl;
	cout << "LE SOLUZIONI" << endl;

	cout << "costo fisso; costo landing; costo fuel; costo km; costo intermediate; costo tw; Total; Numero Aerei; Fuel; KM; IS; min TW;";
	cout << "costo fisso; costo landing; costo fuel; costo km; costo intermediate; costo tw; minuti TW; N_intermediate; gap_perc; time_ALNS; time_model; time_incumbent; numeroSA" << endl;
	Company_Solution = calculationCostCompany(peso_TW, peso_transhipment, peso_itermediate_stop, argv[2], argv[3], airstrips, airplanes, passengers_for_company_solution, from_to_company);
	calculate_ObjectiveFunction_final(Company_Solution, peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
	
	//cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	//cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************************************************************************************************
	tempo_finale = difftime(time(NULL), time_now);
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
