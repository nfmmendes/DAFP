#include <chrono>
#include <iostream>
#include <vector>

#include "Aggregate.h"
#include "Constructive.h"
#include "Destroyers.h"
#include "Evaluate.h"
#include "Garbage.h"
#include "Model_Cplex.h"
#include "Move.h"
#include "Route.h"

typedef vector<vector<vector<int>>> int3DVector;
typedef vector<vector<Route>> Route2DVector;

vector<Route> solution_one_day(ProcessedInput* input, Route2DVector& vector_solution_for_FL, int Iter_FirstDo, int iterMAX_FirstDo, int number_airplane, vector<Airplane> airplanes, double peso_TW, double peso_itermediate_stop, vector<Passenger>& passengers_per_casostrano, map<string, double>& from_to_company, map<int, string>& legenda_ritroso, map<int, Passenger>& map_id_passenger, vector<Passenger>& passengers, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector location_fuel = input->get_location_fuel();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();

	ofstream outfile; //per il file di input grafico.txt++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	outfile.open("grafico.txt");
	int number_iteration_in_heuristic = 0;

	double time_spent_model = 0.0;
	double tempo_finale;
	time_t time_now = time(NULL);

	const int NDestroy = 4;
	const int NRepair = 5;
	const int NHeuristic = 2;
	int initial_weigth = 5;
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
				//auto rng1 = default_random_engine{};
				std::random_device rd;
				std::mt19937 g(rd());
				shuffle(begin(passengers), end(passengers), g);

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					start_solution_route = heuristic_costructive_first_fase(input, peso_TW, peso_itermediate_stop, airplanes, end_day, passengers, number_of_aircraft);
				}
				else {
					start_solution_route = heuristic_costructive_first_fase_sequential(input, peso_TW, peso_itermediate_stop, airplanes, end_day, passengers, number_of_aircraft);
				}
				for (Route& r : start_solution_route) npass += (int)r.passengers_in_route.size();
				cout << "numero di passeggeri " << npass << endl;
				cout << "passengers.size()= " << passengers.size() << endl;
				number_iteration_in_heuristic++;
			} while ((int)passengers.size() > npass);
		}

		start_solution = calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, start_solution_route);
		cout << "ho appena fatto il costruttivo" << endl;
		stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
		solutionAll.push_back(start_solution_route);
		vector_solution_for_FL.push_back(start_solution_route);

		//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
		outfile << "soluzione euristica di partenza:;0;" << start_solution << endl;
		//outfile.close();
		//*********************************************************************************************************************************************************************************************************************

		vector<Route> best_solution_route = start_solution_route;
		best_solution = start_solution;
		// SI INIZI IL CILCO CON LA RICERCA LOCALE E SIMULATING ANNEALING
		do {
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
			double destroy_choice = (double)rand() / RAND_MAX;
			vector<Route> route_destroy;
			auto start = chrono::high_resolution_clock::now();

			if (destroy_choice < Accumulated(0, Weigth_destroy)) {
				int num_passenger_cluster = (int)((passengers.size() * 0.14) + (random * ((passengers.size() * 0.24) - (passengers.size() * 0.14)))); //era 0.24 prima
				route_destroy = destroy_cluster_aggr2(input, peso_TW, num_passenger_cluster, passenger_removed, Input_destroy, passengers, map_id_passenger, peso_itermediate_stop);
				choosen_destroy = 0;
			}
			else if (destroy_choice < Accumulated(1, Weigth_destroy)) {
				//cout << "*************** I am using the Worst Destroy ************" << endl;
				route_destroy = destroy_worst(input, peso_TW, peso_itermediate_stop, percentage_route_destroy, passenger_removed, Input_destroy);
				choosen_destroy = 1;
			}
			else if (destroy_choice < Accumulated(2, Weigth_destroy)) {
				//cout << "*************** I am using the Casual Destroy ************" << endl;
				route_destroy = destroy_casual(input, percentage_route_destroy, passenger_removed, Input_destroy);
				choosen_destroy = 2;
			}
			else {
				//cout << "*************** I am using the Thanos Destroy ************" << endl;
				route_destroy = destroy_thanos(input, percentage_route_destroy, passenger_removed, Input_destroy);
				choosen_destroy = 3;
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
			double time_spent = Time_Spend((double)duration.count());

			//cout << "********************************REBUILT***************************" << endl;
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;

			auto start_r = chrono::high_resolution_clock::now();
			if (repair_choice < Accumulated(0, Weigth_repair)) {
				//cout << "*************** I am using the Repair One ************" << endl;
				solution_rebuilt = repair_one(input, peso_TW, peso_itermediate_stop, end_day, route_destroy, passenger_removed);
				choosen_repair = 0;
			}
			else if (repair_choice < Accumulated(1, Weigth_repair)) {
				//cout << "*************** I am using the Repair SP ************" << endl;
				solution_rebuilt = repairSP(input, peso_TW, peso_itermediate_stop, route_destroy, passenger_removed, airplanes, end_day, passengers, number_of_aircraft);
				choosen_repair = 1;
			}
			else if (repair_choice < Accumulated(2, Weigth_repair)) {
				//cout << "*************** I am using the Repair 2 Regret ************" << endl;
				solution_rebuilt = two_regret_repair_aggragati(input, peso_TW, peso_itermediate_stop, end_day, route_destroy, passenger_removed);
				choosen_repair = 2;
			}
			else if (repair_choice < Accumulated(3, Weigth_repair)) {
				//cout << "*************** I am using the Repair Forbidden ************" << endl;
				solution_rebuilt = repair_forbidden(input, peso_TW, peso_itermediate_stop, end_day, route_destroy, passenger_removed);
				choosen_repair = 3;
			}
			else {
				//cout << "*************** I am using the Repair Perturbation ************" << endl;
				solution_rebuilt = repair_perturbation(input, peso_TW, peso_itermediate_stop, end_day, route_destroy, passenger_removed);
				choosen_repair = 4;
			}
			auto stop_r = chrono::high_resolution_clock::now();
			auto duration_r = chrono::duration_cast<chrono::seconds>(stop_r - start_r);
			double time_spent_r = Time_Spend((double)duration_r.count());

			if (solution_rebuilt.size() > 0) {
				double initial_cost = calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, solution_rebuilt);
				MoveParamSet param_set{ peso_TW, peso_itermediate_stop, end_day };
				solution_rebuilt = move(input, param_set, solution_rebuilt);
				solution_rebuilt = swap(input,param_set, solution_rebuilt);

				if (initial_cost == calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, solution_rebuilt)) {
					solution_rebuilt = inter_move(input, param_set, solution_rebuilt);
				}
				
				solution_rebuilt = heuristic_costructive_second_fase(solution_rebuilt, end_day, peso_TW);
				double cost_objectiveFunction_second_fase_after_rebuilt = calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, solution_rebuilt);
				
				if(NumeroSA%1000 == 0 && NumeroSA > 0 && NumeroSA <= 60000){
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
						
						for (Route& s : solution_rebuilt) best_solution_route.push_back(s);
						best_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						best_iteration = iteration;
						start_route_bs = (int)solutionAll.size();
						size_route_bs = (int)solution_rebuilt.size();
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
				vector_solution_for_FL.push_back(solution_rebuilt);
				cout << "Numero iterazioni ---> " << NumeroSA << endl;
			}
			else {
				iterazioni_fallite++;
			}
			k++;

			if (k >= DeltaK) {
				T = (int)(alpha * T);
				k = 0;
			}

		} while ((iteration <= iteration_max) && (T > Tmin));

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
					// If � la modififica
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
		cout << " il costo della soluzione che mi sono salvato nella mappa: " << calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, Prova) << endl;
		int n = 0;
		for (Route& path : Prova) {
			//path.print();
			n += (int)path.passengers_in_route.size();
		}
		cout << " Soluzione contiene numero di passeggieri pari a " << n << endl;
		//finito di fissare gli aerei
		int3DVector A3;  //matrix A
		vector<vector<double>> C;  //cost
		vector<Route> routes;
		for (auto& airplane : airplanes_model) {
			vector<double> c;
			vector<vector<int>> A2;
			cout << "la size di airplanc routes e': " << endl;
			cout << airplane_routes.size() << endl;
			for (Route& r : airplane_routes[airplane.code]) {
				routes.push_back(r);
				vector<int> A1;
				for (Passenger& p : passengers) {
					auto it = find_if(r.passengers_in_route.begin(), r.passengers_in_route.end(),
						[&p](Passenger other)
						{
							return p.pnr == other.pnr;
						});

					A1.push_back((it != r.passengers_in_route.end()) ? 1 : 0);

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
		Model_Cplex* model = new Model_Cplex(A3, C, airplanes_model, airplane_routes, routes, (int)passengers.size(), passengers, solution_modello, map_airplane);
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
		start_solution = calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, start_solution_route);
		cout << " Costo del Modello che sara nuovo costo di partenza = " << start_solution << endl;

		vector_solution_for_FL.push_back(start_solution_route);
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
	cout << "costo routing: " << costo_senza_time_windows(input, start_solution_route) << endl;
	cout << "costo time windows: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	cout << " Alla iterazione numero: " << best_iteration << endl;
	for (Route s : start_solution_route) cout << s.cost << " -- " << endl;
	//*********************************************************************************AGGREGAZIONE SEMPLICE***************************************************************
	cout << "ora faccio l'aggregazione semplice" << endl;
	start_solution_route = aggrezione_simple_after_model(start_solution_route, map_airplane, from_to);
	cout << calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(input, start_solution_route) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//*********************************************************************************AGGREGAZIONE COMPLESSA***************************************************************
	cout << "ora faccio l'aggregazione complessa" << endl;
	start_solution_route = aggrezione_complex_after_model(start_solution_route, map_airplane, from_to, from_to_FuelConsumed);
	cout << calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(input, start_solution_route) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//*********************************************************************************************************************************************************************
	cout << " ********** Final Weight ************ " << endl;
	cout << " Cluster || Worst || 2-Regret || Thanos " << endl;
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	cout << " One || SP || Regret || Forbidden ||Perturbation " << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	tempo_finale = difftime(time(NULL), time_now);
	cout << "Time at the end: " << tempo_finale << endl;
	cout << "Time model: " << time_spent_model << endl;
	cout << "Numero SA= " << NumeroSA << endl;
	cout << "Iterazioni Fallite= " << iterazioni_fallite << endl;
	cout << "number of iteration in the heuristic: " << number_iteration_in_heuristic << endl;
	outfile.close(); // per il grafico++++++++++++++++++++++++++++++++++++++++++++++++++++++


	cout << " Seleziono solo le migliori che abbiamo ... " << endl;
	map<double, int> cost;
	map<double, int>::iterator it;

	int i = 0;
	double valore = 0.0;
	for (auto vettore : vector_solution_for_FL) {
		valore = calculate_ObjectiveFunction(input, peso_TW, peso_itermediate_stop, vettore);
		it = cost.find(valore);
		if (it == cost.end()) {
			cost.insert(make_pair(valore, i));
		}
		i++;
	}
	cout << " Stampo soluzioni con costo ...." << endl;

	vector<vector<Route>> migliori;
	i = 0;
	for (auto& elemnt : cost) {
		if (i == 80) break;
		std::cout << " Butto dentro route con costo .. " << elemnt.first << " numero .. " << elemnt.second << std::endl;
		std::vector<Route> vettore = (vector_solution_for_FL.at(elemnt.second));
		migliori.push_back(vector_solution_for_FL.at(elemnt.second));
		i++;
	}

	vector_solution_for_FL.clear();
	vector_solution_for_FL = migliori;

	return start_solution_route; // ritorno le route migliori

}

