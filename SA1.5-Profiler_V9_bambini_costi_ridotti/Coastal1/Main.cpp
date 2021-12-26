#include <iostream>
#include <string>
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
#include "ProcessedInput.h"
#include "Repair.h"
#include "Swap.h"
#include "Union.h"

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

//data una location mi restituisce la location pi? vicina
void fix_key_passenger_for_regret(vector<Passenger>& passengers) {

	int chiave = 1;
	map<string, int> string_key;

	for (Passenger& p : passengers) {
		string conc = to_string(p.origin) + "/" + to_string(p.destination) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

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

void build_structure_model(vector<Airplane> airplane, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
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


void print_time(int tempo_Alns)
{
	int h;
	int m;
	int s;
	h = (tempo_Alns / 3600);
	m = (tempo_Alns - (3600 * h)) / 60;
	s = (tempo_Alns - (3600 * h) - (m * 60));
	cout << h << ":" << m << ":" << s << ";";
}


//***********************************MAIN****************************
int main(int argc, char* argv[]) {

	PenaltyWeights penalty_weights{ 10.0, 20.0, 1.0 }; //ISW, TSW, TWW
	
	//**************************INPUT***********************************
	map<string, int> mappa_aereo_tipo;
	map<int, int> codice_aereo_tipo;
	map<int, int> tipo_numero;

	map<int, string> legenda_ritroso;
	map<string, int> legenda = fillLegenda("Network/legenda.csv", legenda_ritroso);

	//reading of AIRPLANE.CSV and creation VECTOR OF AIRPLANES
	vector<Airplane> airplanes = fillAirplane("Network/NewAirplaneAll.csv", legenda);
	build_structure_model(airplanes, mappa_aereo_tipo, codice_aereo_tipo, tipo_numero);
	vector<Airstrip> airstrips = fillAirstrip("Network/NewAirstrip.csv", legenda);
	vector<Passenger> passengers = fillPassenger(argv[1], legenda);
	vector<Passenger> passengers_for_company_solution;

	unione_children_INTELLIGENTE(passengers, passengers_for_company_solution);

	fix_key_passenger_for_regret(passengers);
	map<string, double> from_to_company = fillMatrixCompany("Network/Matrix.csv", airstrips);

	//bulding of hashmap from_to reading the file Matrix.txt
	double2DVector from_to;
	fillMatrix(from_to, "Network/Matrix.csv", airstrips);

	map<int, Airplane> map_airplane;
	for (auto f : airplanes) 
		map_airplane.insert(make_pair(f.code, f));

	map<int, Airstrip> map_airstrip;
	for (auto f : airstrips) 
		map_airstrip.insert(make_pair(f.code, f));
	//ora faccio la mappa che mi permette di avere come chiave ogni aereo/aeroporto, come valore il fuel necessario,
	//dato quell'aereo per andare nell'aereoporto pi? vicino con quel fuel

	double2DVector location_fuel;
	fillLocation_fuel(location_fuel, airstrips, airplanes, from_to, map_airstrip);

	double3DVector from_to_FuelConsumed;
	fill_from_to_fuel_consumed(from_to_FuelConsumed, from_to, airplanes);

	ProcessedInput input(map_airplane, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
	
	vector<Passenger> passengers_solution = passengers;
	string company_routes = argv[2];
	string company_passengers = argv[3];
	double company_solution = calculationCostCompany(penalty_weights, company_routes, company_passengers, airstrips, airplanes, 
													 passengers_for_company_solution, from_to_company);
	
	cout << " Costo della soluzione della compagnia = " << company_solution << endl;
	cout << " Costo della soluzione della compagnia = " << company_solution << endl;
	cout << "now passengers has this number of passengers: " << passengers.size() << endl;
	int num_true = 0;
	for (Passenger p : passengers) num_true += p.capacity;
	cout << ", but the real number was: " << num_true << endl;
	//system("pause");


	cout << "***********************End reading input quindi comincio a contare il tempo***********************" << endl;

	//********************************************************************************************************************
	//********************************DA QUA INCOMINCIA IL CODICE DEL MAIN, PRIMA CI SONO SOLO STRUTTURE DATI*************
	//********************************************************************************************************************
	ofstream outfile; //per il file di input grafico.txt++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	vector<string> data = split(argv[1], '.');
	string Nome_grafico = data[0] + "grafico.txt";
	cout << " Nome del Grafico: " << Nome_grafico << endl;
	outfile.open(Nome_grafico);
	int number_iteration_in_heuristic = 0;

	double time_spent_model = 0.0;
	double tempo_finale;
	time_t time_now = time(NULL);
	//input heuristic
	int number_airplane = stoi(argv[4]); // Per Modello
	
	int Iter_FirstDo = 0;
	int iterMAX_FirstDo = 10; //CHAIAMATA MODELLO
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
	for (double i : Weigth_destroy) 
		cout << i << " || ";
	cout << endl;
	for (double i : Weigth_repair) 
		cout << i << " || ";
	cout << endl;
	vector<vector<Route>> solutionAll;
	//vector<Route> best_solution_route_ever;
	srand(time(NULL));
	int NumeroSA = 0;
	int iterazioni_fallite = 0;
	double end_day = 1110.0;
	int number_of_aircraft = 20;
	vector<Route> start_solution_route;
	double start_solution = 0.0;
	int start_route_bs = 0;
	int size_route_bs = 0;  //se ci sono dei problemi rimettilo
	int best_iteration = 0;
	double time_incumbent = 0.0;

	const int INITIAL_TEMPERATURE = 25000;
	const int MINIMAL_TEMPERATURE = 150;
	const int MAX_ITERATION = 30;
	
	do {
		cout << " ******************************* Inizio un altro giro nel ciclo grande ********************************* " << endl;
		double r;
		double best_solution = DBL_MAX;
		int iteration = 0;
		int iteration_max = 30; //35 se vuopi fare un solo giro 
		int T = 25000;
		int Tmin = 150;    //50 un solo giro 
		int k = 0;
		const double MinLevel = 2;
		const double MaxLevel = 6;
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
				auto rng1 = default_random_engine{};
				shuffle(begin(passengers), end(passengers), rng1);

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					start_solution_route = heuristic_costructive_first_fase(&input, penalty_weights, airplanes, end_day, passengers, number_of_aircraft);
					choosen_heuristic = 0;
				}
				else {
					start_solution_route = heuristic_costructive_first_fase_sequential(&input, penalty_weights, end_day, passengers, number_of_aircraft);
					choosen_heuristic = 1;
				}
				for (Route& r : start_solution_route) 
					npass += (int)r.get_passengers().size();

				cout << "numero di passeggeri " << npass << endl;
				cout << "passengers.size()= " << passengers.size() << endl;
				number_iteration_in_heuristic++;
			} while ((int)passengers.size() > npass);
		}

		start_solution = calculate_objective_function(&input, penalty_weights, start_solution_route);
		solutionAll.push_back(start_solution_route);
	
		//***********************************per scrittura su .txt per grafico*******************************************************
		outfile << "soluzione euristica di partenza:;0;" << start_solution << endl;

		vector<Route> best_solution_route = start_solution_route;
		best_solution = start_solution;
		// SI INIZI IL CILCO CON LA RICERCA LOCALE E SIMULATING ANNEALING

		do {
			auto start = chrono::high_resolution_clock::now();
			NumeroSA++;
			//*************************************heuristic costructive*************************************************************
			double random = (double)rand() / RAND_MAX;
			double percentage_route_destroy = (MinLevel + (random * (MaxLevel - MinLevel)));
			int choosen_destroy = -1;
			int choosen_repair = -1;
			vector<Passenger> passenger_removed;
			// REMOVING OF A RANDOM NODE
			vector<Route> Input_destroy = start_solution_route;
			double destroy_choice = (double)rand() / RAND_MAX;
			vector<Route> route_destroy;

			if (destroy_choice < Accumulated(0, Weigth_destroy)) {
				int num_passenger_cluster = (int)((passengers.size() * 0.14) + (random * passengers.size() * 0.10)); 
				route_destroy = destroy_cluster_aggr2(&input, penalty_weights, num_passenger_cluster, passenger_removed, Input_destroy, passengers);
				choosen_destroy = 0;
			}
			else if (destroy_choice < Accumulated(1, Weigth_destroy)) {
				route_destroy = destroy_worst(&input, penalty_weights, percentage_route_destroy, passenger_removed, Input_destroy);
				choosen_destroy = 1;
			}
			else if (destroy_choice < Accumulated(2, Weigth_destroy)) {
				route_destroy = destroy_casual(&input, percentage_route_destroy, passenger_removed, Input_destroy);
				choosen_destroy = 2;
			}
			else {
				route_destroy = destroy_thanos(&input, percentage_route_destroy, passenger_removed, Input_destroy);
				choosen_destroy = 3;
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
			double time_spent = Time_Spend((double)duration.count());
			
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;

			auto start_r = chrono::high_resolution_clock::now();

			if (repair_choice < Accumulated(0, Weigth_repair)) {
				solution_rebuilt = repair_one(&input, penalty_weights, end_day, route_destroy, passenger_removed);
				choosen_repair = 0;
			}
			else if (repair_choice < Accumulated(1, Weigth_repair)) {
				solution_rebuilt = repairSP(&input, penalty_weights, route_destroy, passenger_removed, end_day, passengers, number_of_aircraft);
				choosen_repair = 1;
			}
			else if (repair_choice < Accumulated(2, Weigth_repair)) {
				solution_rebuilt = two_regret_repair_agregate(&input, penalty_weights, end_day, route_destroy, passenger_removed);
				choosen_repair = 2;
			}
			else if (repair_choice < Accumulated(3, Weigth_repair)) {
				solution_rebuilt = repair_forbidden(&input, penalty_weights, end_day, route_destroy, passenger_removed);
				choosen_repair = 3;
			}
			else {
				//cout << "*************** I am using the Repair Perturbation ************" << endl;
				solution_rebuilt = repair_perturbation(&input, penalty_weights, end_day, route_destroy, passenger_removed);
				choosen_repair = 4;
			}
			auto stop_r = chrono::high_resolution_clock::now();
			auto duration_r = chrono::duration_cast<chrono::seconds>(stop_r - start_r);
			double time_spent_r = Time_Spend((double)duration_r.count());		

			if (solution_rebuilt.size() > 0) {
				double initial_cost = calculate_objective_function(&input, penalty_weights, solution_rebuilt);
				solution_rebuilt = move(&input, penalty_weights, solution_rebuilt, end_day);
				solution_rebuilt = swap(&input, penalty_weights, solution_rebuilt, end_day);
				
				if (initial_cost == calculate_objective_function(&input, penalty_weights, solution_rebuilt)) {
					solution_rebuilt = inter_move(&input, penalty_weights, solution_rebuilt, end_day);
				}

				solution_rebuilt = heuristic_costructive_second_fase(solution_rebuilt, end_day, penalty_weights.time_window);
				double cost_objectiveFunction_second_fase_after_rebuilt = calculate_objective_function(&input, penalty_weights, solution_rebuilt);
				//*************************************per scrittura su .txt per grafico**************************************
				if (NumeroSA%1000 == 0 && NumeroSA >= 1000 && NumeroSA <= 60000){
					tempo_finale = difftime(time(NULL), time_now);
					outfile << "NumeroSA:;" << NumeroSA << ";" << best_solution << ";tempo:;" << to_string(tempo_finale) << endl;
				}
				
				if (cost_objectiveFunction_second_fase_after_rebuilt < start_solution) {
					
					start_solution_route.clear();
					for (Route& so : solution_rebuilt) start_solution_route.push_back(so);
					double award = Ob_Funct_Improvement(cost_objectiveFunction_second_fase_after_rebuilt, start_solution);
					start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
					iteration = 0;
					
					Weigth_destroy[choosen_destroy] += (1 + award - time_spent);
					Weigth_repair[choosen_repair] += (1 + award - time_spent_r);
					if (cost_objectiveFunction_second_fase_after_rebuilt < best_solution) {
						best_solution_route.clear();
						
						for (Route& s : solution_rebuilt) 
							best_solution_route.push_back(s);
						best_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						best_iteration = iteration;
						start_route_bs = (int)solutionAll.size();
						size_route_bs = (int)solution_rebuilt.size();
						time_incumbent = difftime(time(NULL), time_now);
					}
				}
				else {

					r = double(rand()) / (double(RAND_MAX) + 1.0);
					double espo = -(cost_objectiveFunction_second_fase_after_rebuilt - start_solution) / T;

					if (r <= exp(espo) && exp(espo) != 1) {
						start_solution_route.clear();
						for (Route& s : solution_rebuilt) start_solution_route.push_back(s);
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
			}
			k++;
			if (k >= DeltaK) {
				T = (int)(alpha * T);
				k = 0;
			}
		} while ((iteration <= iteration_max) && (T > Tmin) && (NumeroSA <= 20000));

		if (Iter_FirstDo > 4) 
			cout << " Iterazioni senza migliorare " << iteration << " Temperaruta: " << T << endl;
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
					if (r.get_passengers().size() > 0) {
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

		int n = 0;
		for (Route& path : Prova) {
			n += (int)path.get_passengers().size();
		}
		cout << " Soluzione contiene numero di passeggieri pari a " << n << endl;
		//finito di fissare gli aerei
		int3DVector A3;  //matrix A
		double2DVector C;  //cost
		vector<Route> routes;
		for (auto& airplane : airplanes_model) {
			vector<double> c;
			vector<vector<int>> A2;
			for (Route& r : airplane_routes[airplane.code]) {
				routes.push_back(r);
				vector<int> A1;
				for (Passenger& p : passengers) {
					auto it = find_if(r.get_passengers().begin(), r.get_passengers().end(), [=](Passenger p_){
						return p.pnr == p_.pnr;
					});
					
					if (it != r.get_passengers().end())
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
		cout << "*************************************	0	****************************************************************" << endl;
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
			for (Route& s : model->solution_model) 
				start_solution_route.push_back(s);
		}
		else {
			for (Route& s : best_solution_route) 
				start_solution_route.push_back(s);
		}
		start_solution = calculate_objective_function(&input, penalty_weights, start_solution_route);
		cout << " Costo del Modello che sara nuovo costo di partenza = " << start_solution << endl;

		cout << " Best: " << best_solution << " start: " << start_solution << endl;
		if (best_solution > start_solution) {
			//auto stop_incumbent = chrono::high_resolution_clock::now();
			//auto duration_incumbent = chrono::duration_cast<chrono::seconds>(stop_incumbent - start);
			time_incumbent = difftime(time(NULL), time_now);
			cout << " Aggiorno dopo modello .. time: " << time_incumbent << endl;
		}

		outfile << "modello:;**; " << start_solution << endl;

		start_route_bs = (int)solutionAll.size();
		size_route_bs = (int)start_solution_route.size();
		solutionAll.push_back(start_solution_route);
		Iter_FirstDo++;
		cout << "*****************************************************************************************************************" << endl;
		cout << " ************************ ITERATION WHILE: " << Iter_FirstDo << " OF: " << iterMAX_FirstDo << "******************" << endl;
		cout << "*****************************************************************************************************************" << endl;
		auto stop_model = chrono::high_resolution_clock::now();
		auto duration_model = chrono::duration_cast<chrono::seconds>(stop_model - start_model);
		time_spent_model += (double)duration_model.count();

		delete model; 
	} while ((Iter_FirstDo <= iterMAX_FirstDo));

	cout << "Best solution herurisic: " << start_solution << endl;
	cout << "costo routing: " << costo_senza_time_windows(&input, start_solution_route) << endl;
	cout << "costo time windows: " << costo_time_windows_and_intermediate_stop(penalty_weights, start_solution_route) << endl;
	cout << " Alla iterazione numero: " << best_iteration << endl;
	for (Route s : start_solution_route) cout << s.cost << " -- " << endl;
	//*****************************************************AGGREGAZIONE SEMPLICE**************************
	cout << "ora faccio l'aggregazione semplice" << endl;
	start_solution_route = aggrezione_simple_after_model(start_solution_route, map_airplane, from_to);
	cout << calculate_objective_function(&input, penalty_weights, start_solution_route) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(&input, start_solution_route) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(penalty_weights, start_solution_route) << endl;
	//******************************************************AGGREGAZIONE COMPLESSA***********************
	cout << "ora faccio l'aggregazione complessa" << endl;
	start_solution_route = aggrezione_complex_after_model(start_solution_route, map_airplane, from_to, from_to_FuelConsumed);
	cout << calculate_objective_function(&input, penalty_weights, start_solution_route) << endl;
	cout << "******************************************************************************************" << endl;
	cout << "LE SOLUZIONI" << endl;

	const string basic_columns = "costo fisso; costo landing; costo fuel; costo km; costo intermediate; costo tw;";
	cout << basic_columns<<" Total; Numero Aerei; Fuel; KM; IS; min TW;";
	cout << basic_columns<<" minuti TW; N_intermediate; gap_perc; time_ALNS; time_model; time_incumbent; numeroSA" << endl;
	company_solution = calculationCostCompany( penalty_weights, argv[2], argv[3], airstrips, airplanes, passengers_for_company_solution, from_to_company);
	calculate_ObjectiveFunction_final(&input, company_solution, penalty_weights, start_solution_route);
	
	tempo_finale = difftime(time(NULL), time_now);
	int tempo_Alns, h, m, s;
	tempo_Alns = (int)(tempo_finale - time_spent_model);
	print_time(tempo_Alns);
	tempo_Alns = (int)(time_spent_model);
	print_time(tempo_Alns);
	tempo_Alns = (int)(time_incumbent);
	print_time(tempo_Alns);
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
	cout << "///////////// ora vengono stampate le route con l'aggiunta dell'ultimo arco, e il relativo costo////////////////////////" << endl;
	calculate_ObjectiveFunction_final_arc_iori(&input, company_solution, penalty_weights, start_solution_route);
	
	outfile.close(); // per il grafico+++++++++
	return 0;
}
