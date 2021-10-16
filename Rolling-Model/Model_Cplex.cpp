#include "Model_Cplex.h"
#include <ilconcert/iloenv.h>

#include <iostream>
#include<string>
#include <vector>
#include <ilconcert/iloexpression.h>
#include <ilcplex/ilocplex.h>


#include "Route.h"

Model_Cplex::Model_Cplex(vector<vector<vector<int>>> a, vector<vector<double>> c, vector<Airplane>& airplaneNew, map<int, vector<Route>>& airplane_routesNew, vector<Route>& all_Route, const int nrichieste, vector<Passenger>& all_passengers, vector<Route>& solution_model_new, map <int, Airplane>& map_airplane_new) {	//construtor
	A = a;
	C = c;
	airplane = airplaneNew;
	All_Route = all_Route;
	NRichieste = nrichieste;
	All_Passenger = all_passengers;
	airplane_routes = airplane_routesNew;
	solution_model = solution_model_new;
	map_airplane = map_airplane_new;
	solution_after;
}

Model_Cplex::~Model_Cplex() { // Destrutor

}




void Model_Cplex::create_Model_cplex(int NumberAirplane, map<int, int>& solution_warm_up, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero, vector<vector<Route>>& solutionAll) {

	vector<vector<Route>> t_route(mappa_aereo_tipo.size());
	vector<vector<double>> t_route_costi(mappa_aereo_tipo.size());
	vector<vector<vector<int>>> A2(mappa_aereo_tipo.size());
	vector<vector<int>> first_solution(mappa_aereo_tipo.size());
	vector<vector<int>> airplane_available(mappa_aereo_tipo.size());
	map<int, int> airplane_available_position;
	double threshold = 500;
	for (auto x : codice_aereo_tipo) {
		airplane_available.at(x.second).push_back(x.first);
		airplane_available_position.insert(make_pair(x.first, (int)airplane_available.at(x.second).size() - 1));
	}

	//for (auto x : airplane_available_position) cout << " Codice Aereo " << x.first << " poiszione " << x.second << endl;
	//for (size_t i = 0; i < airplane_available.size(); i++){
		//cout << " Tipo " << i << endl;
		//for (int j : airplane_available[i]) cout << " Aereo: " << j << endl;
	//}



	for (int j = 0; j < (int)airplane.size(); j++) {
		int tipo = mappa_aereo_tipo[airplane[j].model + to_string(airplane[j].depot)];
		//cout << " Sono all areoplaneo " << airplane[j].code << " tipo: " << tipo << endl;
		if (t_route.at(tipo).size() == 0) {
			map<int, int>::iterator it = solution_warm_up.find(airplane[j].code);
			if (it != solution_warm_up.end()) {
				//cout << " Ho torvato nella mia soluzione ho questo aereo " << airplane[j].code << endl;
				//cout << " La route di questo aereo che va nella soluzione: " << solution_warm_up[airplane[j].code] << endl;
				first_solution.at(tipo).push_back(solution_warm_up[airplane[j].code]);
				//cout << " Size del nostro warm up pari a: " << first_solution.at(tipo).size() << endl;
			}
			//cout << " Ho size = 0 pe rtipo " << to_string(tipo) << endl;
			t_route.at(tipo) = airplane_routes[airplane[j].code];
			t_route_costi.at(tipo) = C[j];
			// Devo mettere anche A2
			A2.at(tipo) = A.at(j);

		}
		else {
			map<int, int>::iterator it = solution_warm_up.find(airplane[j].code);
			if (it != solution_warm_up.end()) {
				//cout << " Ho torvato nella mia soluzione ho questo aereo " << airplane[j].code << endl;
				//cout << " La route di questo aereo che va nella soluzione: " << solution_warm_up[airplane[j].code] << " ma queisto tipo ha gia numero di rpute pari a " << t_route.at(tipo).size() << endl;
				first_solution.at(tipo).push_back((int)t_route.at(tipo).size() + solution_warm_up[airplane[j].code]);
				//cout << " Size del nostro warm up pari a: " << first_solution.at(tipo).size() << endl;
			}
			//cout << " Ho size != 0 pe rtipo " << to_string(tipo) << " size: " << t_route.at(tipo).size() << " pool da aggiungere eha size " << airplane_routes[airplane[j].code].size() << endl;
			t_route.at(tipo).insert(t_route.at(tipo).end(), airplane_routes[airplane[j].code].begin(), airplane_routes[airplane[j].code].end());
			t_route_costi.at(tipo).insert(t_route_costi.at(tipo).end(), C[j].begin(), C[j].end());
			//cout << "Adesso pe rtipo " << to_string(tipo) << " ho size: " << t_route.at(tipo).size() << endl;
			A2.at(tipo).insert(A2.at(tipo).end(), A.at(j).begin(), A.at(j).end());
		}

	}

	IloEnv env;
	try {
		IloModel model(env);
		typedef IloArray<IloBoolVarArray> NumVarMatrix;
		IloRangeArray con(env);
		//populatebyrow(model, var, con);

		cout << " Nuemro di Aereri " << (int)airplane.size() << endl;

		NumVarMatrix x(env, (int)airplane.size());
		cout << "----------- Sto Creando la prima Variabile X---------------" << endl;
		for (size_t j = 0; j < t_route.size(); j++) {
			//IloBoolVarArray x(env, airplane.size());
			x[j] = IloBoolVarArray(env, t_route[j].size());
			//x.add(IloNumVar(env, 0, 1, ILOBOOL));
		}
		cout << " variables created!!" << endl;
		cout << "Creating constraint 1..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int i = 0; i < NRichieste; i++) {
			IloExpr C1(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t z = 0; z < t_route.size(); z++) {
				for (size_t j = 0; j < t_route[z].size(); j++) {
					C1 += x[z][j] * A2[z][j][i];
				}
			}

			string cname1;
			cname1 = "C1_" + to_string(i);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C1 == 1);
			cons.setName(cname1.c_str());
			model.add(cons);
			C1.end();
		}

		cout << "Creating constraint 2..." << endl;
		for (size_t j = 0; j < t_route.size(); j++) {
			//cout << " sto per il tipo : " << to_string(j) << " Meno aerei di " << tipo_numero[j]<<  endl;
			model.add(IloSum(x[j]) <= tipo_numero[j]);
		}

		cout << "Creating constraint 3..." << endl;
		IloExpr C3(env);
		for (size_t j = 0; j < t_route.size(); j++) {
			for (size_t i = 0; i < t_route[j].size(); i++) {
				C3 += x[j][i];
			}
		}
		string cname3;
		cname3 = "C3_";
		//cout << " sto aggiungendo il vincolo: " << cname3 << endl;
		IloRange cons(C3 <= NumberAirplane);
		cons.setName(cname3.c_str());
		model.add(cons);
		C3.end();



		cout << "Creating objective function ...." << endl;
		IloExpr objective(env);
		for (size_t j = 0; j < t_route.size(); j++) {
			for (size_t i = 0; i < t_route[j].size(); i++) {
				objective += x[j][i] * t_route_costi[j][i];
			}
		}
		model.add(IloMinimize(env, objective));
		objective.end();
		IloCplex cplex(model);
		// WARM-UP

		cout << " Numero aerei soluzione ALNS " << solution_warm_up.size() << endl;
		cout << " Input Numero Aerei " << NumberAirplane << endl;
		IloNumVarArray startVar(env);
		IloNumArray startVal(env);
		if (solution_warm_up.size() <= NumberAirplane) {
			cout << " OK! la soluzione del ALNS puo essere usata come MipStart " << endl;
			for (size_t j = 0; j < t_route.size(); j++) {
				//cout << " Sto guardando tipo " << j << endl;
				if (first_solution[j].size() > 0) {
					//cout << " Ha qualche Route nel WARM-UP" << endl;
					for (size_t i = 0; i < t_route[j].size(); i++) {
						//cout << " Sto gurdando la coppia i: " << i << " j: " << j << endl;
						startVar.add(x[j][i]);
						if (find(first_solution[j].begin(), first_solution[j].end(), i) != first_solution[j].end()) {
							//cout << " I = " << i << " allora metti a 1 " << endl;
							startVal.add(1);
						}
						else {
							startVal.add(0);
						}
					}

				}
				else {
					//cout << " Nella mia soluzione NON ho questo tipo " << j << endl;
					for (size_t i = 0; i < t_route[j].size(); i++) {
						startVar.add(x[j][i]);
						startVal.add(0);
					}

				}

			}
			cplex.addMIPStart(startVar, startVal);
			startVal.end();
			startVar.end();
		}


		// Set Parametres
		cplex.setParam(IloCplex::NumParam::TiLim, 600);			// Time Limit
		cplex.setParam(IloCplex::IntParam::RootAlg, 0);			// Methodology 1= Primal simplex 2	= Dual simplex 3 = Network simplex, 4	= 	Barrier, 5	= 	Sifting, 6	= 	Concurrent(Dual, Barrier, and Primal in opportunistic parallel mode; Dual and Barrier in deterministic parallel mode)
		cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 0.0);			// MIP GAP
		cplex.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 0.0);
		cplex.setParam(IloCplex::IntParam::MIPEmphasis, 4);
		cplex.setParam(IloCplex::Param::MIP::Cuts::Cliques, 2);
		cplex.setParam(IloCplex::Param::MIP::Cuts::LiftProj, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::BQP, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::Covers, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::Disjunctive, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::FlowCovers, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::GUBCovers, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::Implied, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::LocalImplied, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::Gomory, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::MCFCut, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::MIRCut, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::PathCut, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::RLT, -1);
		cplex.setParam(IloCplex::Param::MIP::Cuts::ZeroHalfCut, 1);
		//cplex.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);
		// End of Paramenters setting
		cplex.exportModel("model.sav");
		IloEnv new_env;
		IloModel new_model(new_env);
		IloCplex new_cplex(new_model);
		new_cplex.importModel(new_model, "model.sav");
		for (size_t j = 0; j < t_route.size(); j++)  new_model.add(IloConversion(env, x[j], ILOFLOAT));
		new_cplex.solve();
		cout << "----------------------- Finito la risoluzione del rilassamento -------------------------" << endl;
		vector<Route> Imp_columns;
		for (size_t j = 0; j < t_route.size(); j++) {
			for (size_t i = 0; i < t_route[j].size(); i++) {
				//cout << "Reduced Cost " << "  x[" << j << "," << i << "] = " << new_cplex.getReducedCost(x[j][i]) << endl;
				//cout << "Variabile " << "  x[" << j << "," << i << "] = " << new_cplex.getValue(x[j][i]) << endl;
				if (new_cplex.getReducedCost((const IloIntVar)x[j][i]) <= threshold) Imp_columns.push_back(t_route[j][i]);
			}
		}
		solutionAll.push_back(Imp_columns);
		cout << "Number of important columns: " << Imp_columns.size() << endl;
		//system("pause");
		cplex.solve();
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value  = " << cplex.getObjValue() << endl;

		if (cplex.getStatus() == IloAlgorithm::Optimal || cplex.getStatus() == IloAlgorithm::Feasible) {
			int NPass = 0;
			double val;
			int z = 0;
			vector<vector<int>> Duplicate;
			for (size_t j = 0; j < t_route.size(); j++) {
				vector<int> to_remove;
				cout << "==================== Stampo le route SCELTE per tipo: " << j << "  ===================== \n" << endl;
				for (size_t i = 0; i < t_route[j].size(); i++) {
					val = cplex.getValue(x[j][i]);
					if (val > 0.01) {
						cout << "Variabile " << j + i << "  x[" << j << "," << i << "] = " << val << endl;
						cout << " Con il costo:  " << t_route_costi[j][i] << endl;
						//cout << " ----->  Stampo la route <--------- " << endl;
						//t_route[j][i].print();
						for (int z = 0; z < NRichieste; z++) {
							if (A2[j][i][z] == 1) {
								NPass++;
							}
						}
						solution_model.push_back(t_route[j][i]);
						//cout << " Devo eliminare aereo " << t_route[j][i].aircraft_code << endl;
						vector<int>::iterator it;
						it = find(to_remove.begin(), to_remove.end(), airplane_available_position[t_route[j][i].aircraft_code]);
						if (it == to_remove.end()) to_remove.push_back(airplane_available_position[t_route[j][i].aircraft_code]);
					}
				}
				//for (int i = to_remove.size() - 1; i >= 0; i--) cout << to_remove[i] << endl;
				for (int i = to_remove.size() - 1; i >= 0; i--)  airplane_available.at(j).erase(airplane_available.at(j).begin() + to_remove[i]);
			}
			/*
			 cout << " Numero passeggieri soddisfatti " << NPass << endl;

			for (size_t i = 0; i < airplane_available.size(); i++) {
				cout << " Tipo " << i << endl;
				for (int j : airplane_available[i]) cout << " Aereo: " << j << endl;
			}
			system("pause");
			*/

			//for (Route& r : solution_model) cout << r.aircraft_code << endl;
			
			for (size_t i = 0; i < solution_model.size(); i++) {
				for (size_t j = 0; j < solution_model.size(); j++) {
					if (i != j && solution_model[i].aircraft_code == solution_model[j].aircraft_code) {
						int tipo = codice_aereo_tipo[solution_model[i].aircraft_code];
						cout << " HAi un doppione " << solution_model[i].aircraft_code << " del tipo " << tipo << endl;
						solution_model[i].aircraft_code = airplane_available[tipo].back();
						airplane_available[tipo].pop_back();
					}
				}
			}
			cout << " ------------------------------- DOPO LA MODIFICA ---------------------------------- " << endl;
			for (Route& r : solution_model) cout << r.aircraft_code << " || ";
			cout << endl;


		}

	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}

	env.end();

}

void Model_Cplex::create_Model_for_SP_cplex(int NumberAirplane) {	//Formulation - Gurobi
	
	IloEnv env;
	try {
		IloModel model(env);
		typedef IloArray<IloBoolVarArray> NumVarMatrix;
		IloRangeArray con(env);
		//populatebyrow(model, var, con);

		//cout << " Nuemro di Aereri " << (int)airplane.size() << endl;

		NumVarMatrix x(env, (int)airplane.size());
		//cout << "----------- Sto Creando la prima Variabile X---------------" << endl;
		for (int j = 0; j < (int)airplane.size(); j++) {
			//IloBoolVarArray x(env, airplane.size());
			x[j] = IloBoolVarArray(env, airplane_routes[airplane[j].code].size());
			//x.add(IloNumVar(env, 0, 1, ILOBOOL));
		}
		//cout << " variables created!!" << endl;
		//cout << "Creating constraint 1..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int i = 0; i < NRichieste; i++) {
			IloExpr C1(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (int z = 0; z < (int)airplane.size(); z++) {
				for (int j = 0; j < airplane_routes[airplane[z].code].size(); j++) {
					C1 += x[z][j] * A[z][j][i];
				}
			}

			string cname1;
			cname1 = "C1_" + to_string(i);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C1 == 1);
			cons.setName(cname1.c_str());

			model.add(cons);

			C1.end();
		}

		//cout << "Creating constraint 2..." << endl;
		for (int j = 0; j < (int)airplane.size(); j++) {
			model.add(IloSum(x[j]) <= 1);
		}

		//cout << "Creating constraint 3..." << endl;
		IloExpr C3(env);
		for (int i = 0; i < airplane.size(); i++) {
			for (int j = 0; j < (int)airplane_routes[airplane[i].code].size(); j++) {
				C3 += x[i][j];
			}
		}
		string cname3;
		cname3 = "C3_";
		//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
		IloRange cons(C3 <= NumberAirplane);
		cons.setName(cname3.c_str());
		model.add(cons);
		C3.end();



		//cout << "Creating objective function ...." << endl;
		IloExpr objective(env);
		for (int j = 0; j < (int)airplane.size(); j++) {
			for (int i = 0; i < (int)airplane_routes[airplane[j].code].size(); i++) {
				objective += x[j][i] * C[j][i];
			}
		}
		model.add(IloMinimize(env, objective));
		objective.end();
		IloCplex cplex(model);



		// Set Parametres
		//cplex.setParam(IloCplex::IntParam::Threads, 1);			// Number of used threads
		cplex.setParam(IloCplex::NumParam::TiLim, 10);			// Time Limit
		cplex.setParam(IloCplex::IntParam::RootAlg, 0);			// Methodology 1= Primal simplex 2	= Dual simplex 3 = Network simplex, 4	= 	Barrier, 5	= 	Sifting, 6	= 	Concurrent(Dual, Barrier, and Primal in opportunistic parallel mode; Dual and Barrier in deterministic parallel mode)
		cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 0.0);			// MIP GAP
		cplex.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 0.0);
		cplex.setParam(IloCplex::IntParam::MIPEmphasis, 2);			//1	=	Emphasize feasibility over optimality, 2=	Emphasize optimality over feasibility, 3=	Emphasize moving best bound, 4	=	Emphasize finding hidden feasible solutions
		cplex.setOut(env.getNullStream());
		cplex.solve();

		if (cplex.getStatus() == IloAlgorithm::Optimal || cplex.getStatus() == IloAlgorithm::Feasible) {
			double val;
			for (int j = 0; j < (int)airplane.size(); j++) {
				//cout << "==================== Stampo le route SCELTE dell'aereo: " << airplane[j].code << "  ===================== \n" << endl;
				for (int i = 0; i < (int)airplane_routes[airplane[j].code].size(); i++) {
					val = cplex.getValue(x[j][i]);
					if (val > 0.01) {
						for (int z = 0; z < NRichieste; z++) {
							if (A[j][i][z] == 1) {
							}
						}
					}
				}
			}
			// QUi la soluzione deve essere prese
			for (int j = 0; j < (int)airplane.size(); j++) {
				for (int i = 0; i < (int)airplane_routes[airplane[j].code].size(); i++) {
					val = cplex.getValue(x[j][i]);
					if (val > 0.01) {
						//airplane_routes[airplane[j].code][i].print();
						solution_model.push_back(airplane_routes[airplane[j].code][i]);
					}

				}

			}

		}
		else {
			cout << " Non ha Funzionato si vede che non ha chiuso all'ottimo " << endl;
			cout << " Status " << cplex.getStatus() << endl;
			//system("pause");
		}


	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}

	env.end();
}
