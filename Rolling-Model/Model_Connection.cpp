#include "Model_Connection.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include "Route.h"

Model_Connection::Model_Connection( vector<vector<int>>& mat_depot_1_2, vector<vector<int>>& mat_depot_2_3, vector<vector<int>>& mat_depot_1_3, vector<vector<vector<double>>>& costo_connessione, vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, vector <Flightleg>& flight_leg_day1, vector <Flightleg>& flight_leg_day2, vector <Flightleg>& flight_leg_day3, vector<vector<int>>& FL_P_day1, vector<vector<int>>& FL_P_day2, vector<vector<int>>& FL_P_day3) {	//construtor
	this->A = A;
	this->B = B;
	this->C = C;
	this->flight_leg_day1 = flight_leg_day1;
	this->flight_leg_day2 = flight_leg_day2;
	this->flight_leg_day3 = flight_leg_day3;
	this->FL_P_day1 = FL_P_day1;
	this->FL_P_day2 = FL_P_day2;
	this->FL_P_day3 = FL_P_day3;
	this->costo_connessione = costo_connessione;
	this->mat_depot_1_2 = mat_depot_1_2;
	this->mat_depot_2_3 = mat_depot_2_3;
	this->mat_depot_1_3 = mat_depot_1_3;
	
}

Model_Connection::~Model_Connection() { // Destrutor

}




bool Model_Connection::create_Model_connection(vector<vector<bool>>& R, vector<vector<bool>>& S, vector<vector<bool>>& T, int num_req_day1, int num_req_day2, int num_req_day3, int num_aerei) {
	
	bool infeasible = false;
	IloEnv env;
	try {
		IloModel model(env);
		typedef IloArray<IloBoolVarArray> NumVarMatrix;
		//populatebyrow(model, var, con);
		IloCplex cplex(model);


		NumVarMatrix x(env, (int)flight_leg_day1.size());
		cout << "----------- Sto Creando la prima Variabile X---------------" << endl;
		cout << " Ne devo creare un numero paari a " << flight_leg_day1.size() << endl;
		system("pause");
		for (size_t j = 0; j < flight_leg_day1.size(); j++) {
			//IloBoolVarArray x(env, airplane.size());
			//cout << " J: " << j << endl;
			x[j] = IloBoolVarArray(env, flight_leg_day2.size());
			//x.add(IloNumVar(env, 0, 1, ILOBOOL));
		}
		cout << " variables created!!" << endl;


		NumVarMatrix y(env, (int)flight_leg_day2.size());
		cout << "----------- Sto Creando la prima Variabile y---------------" << endl;
		cout << " Ne devo creare un numero paari a " << flight_leg_day2.size() << endl;
		system("pause");
		for (size_t j = 0; j < flight_leg_day2.size(); j++) {
			//IloBoolVarArray x(env, airplane.size());
			y[j] = IloBoolVarArray(env, flight_leg_day3.size());
			//x.add(IloNumVar(env, 0, 1, ILOBOOL));
		}
		cout << " variables created!!" << endl;
		system("pause");

		NumVarMatrix z(env, (int)flight_leg_day1.size());
		cout << "----------- Sto Creando la prima Variabile z---------------" << endl;
		cout << " Ne devo creare un numero paari a " << flight_leg_day3.size() << endl;
		for (size_t j = 0; j < flight_leg_day1.size(); j++) {
			//IloBoolVarArray x(env, airplane.size());
			z[j] = IloBoolVarArray(env, flight_leg_day3.size());
			//x.add(IloNumVar(env, 0, 1, ILOBOOL));
		}
		cout << " variables created!!" << endl;

		cout << "variabile d...................." << endl;
		IloBoolVarArray  d = IloBoolVarArray(env, (int)flight_leg_day2.size());
		//IloNumVarArray d(env, 0.0, 1.0, ILOFLOAT);
		system("pause");


		cout << " Inizio Eliminazione di alcune Variabili " << endl;
		for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
			for (size_t j = 0; j < flight_leg_day2.size(); j++) if (A[i][j] == 0) x[i][j].setUB(0);
			for (size_t k = 0; k < flight_leg_day3.size(); k++) if (C[i][k] == 0)  z[i][k].setUB(0);
		}

		for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
			for (size_t k = 0; k < flight_leg_day3.size(); k++) if (B[j][k] == 0) y[j][k].setUB(0);
		}
		cout << " Fine Eliminazione di alcune Variabili " << endl;
		string cname1;
		cout << "Creating constraint 1..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		//cout << " Ne devo creare un numero paari a " << flight_leg_day1.size() << endl;
		for (int i = 0; i < (int)flight_leg_day1.size()-1; i++) {
			IloExpr C1(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t j = 0; j < flight_leg_day2.size(); j++) {
				C1 += x[i][j];
			}
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				C1 += z[i][k];
			}

			cname1 = "C1_" + to_string(i);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C1 <= 1);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C1.end();
		}


		cout << "Creating constraint 2..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
			IloExpr C2(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				C2 +=  y[j][k];
			}

			cname1 = "C2_" + to_string(j);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C2 <= 1);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C2.end();
		}

		cout << "Creating constraint 3..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int k = 0; k < (int)flight_leg_day3.size()-1; k++) {
			IloExpr C3(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t j = 0; j < flight_leg_day2.size(); j++) {
				C3 += y[j][k];
			}
			for (size_t i = 0; i < flight_leg_day1.size(); i++) {
				C3 +=  z[i][k];
			}
			cname1 = "C3_" + to_string(k);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C3 <= 1);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C3.end();
		}


		cout << "Creating constraint 4..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
			IloExpr C4(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t i = 0; i < flight_leg_day1.size(); i++) {
				C4 += x[i][j];
			}

			cname1 = "C4_" + to_string(j);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C4 <= 1);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C4.end();
		}


		cout << "Creating constraint 5..." << endl;
		bool trovato = false;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int p = 0; p < num_req_day1; p++) {
			IloExpr C5(env);
			trovato = false;
			for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
				if (FL_P_day1[i][p] == 1) {
					for (size_t j = 0; j < flight_leg_day2.size(); j++) {
						//cout << " Guardo Flight Leg Collegamento  " << j << " su disponibili  " << flight_leg_day2.size() << endl;
						//C5 += A[i][j] * x[i][j];
						C5 += x[i][j];
					}
					for (size_t k = 0; k < flight_leg_day3.size(); k++) {
						//cout << " Guardo Flight Leg Collegamento  " << k << endl;
						//C5 += C[i][k] * z[i][k];
						C5 += z[i][k];
					}
					trovato = true;
				}

			}

			cname1 = "C5_" + to_string(p);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C5 == 1);
			cons.setName(cname1.c_str());
			model.add(cons);
			C5.end();

			if (!trovato) {
				cout << " Passeggiero nuermo p " << p << " con codice passeggiero = " << endl;
				cin.get();
			}


		}


		cout << "Creating constraint 6..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int p = 0; p < num_req_day3; p++) {
			IloExpr C6(env);
			bool trovato = false;
			//cout << " Creo vincolo per passeggiero " << endl;
			for (int k = 0; k < (int)flight_leg_day3.size(); k++) {
				if (FL_P_day3[k][p] == 1) {
					for (size_t j = 0; j < flight_leg_day2.size(); j++) {
						C6 += y[j][k];
					}
					for (size_t i = 0; i < flight_leg_day1.size(); i++) {
						C6 += z[i][k];
					}
					trovato = true;
				}
			}
			if (!trovato) {
				cout << " Per il Passeggiero " << p << " non esiste route per lui " << endl;
				cin.get();
			}
			cname1 = "C6_" + to_string(p);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C6 == 1);
			cons.setName(cname1.c_str());
			model.add(cons);
			C6.end();
		}



		cout << "Creating constraint 7..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int p = 0; p < num_req_day2; p++) {
			IloExpr C7(env);
			trovato = false;
			for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
				if (FL_P_day2[j][p] == 1) {
					C7 += d[j];
					trovato = true;
				}
			}
			if (!trovato) {
				cout << " Per il Passeggiero " << p << " non esiste route per lui " << endl;
				cin.get();
			}
			cname1 = "C7_" + to_string(p);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C7 == 1);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C7.end();
		}


		cout << "Creating constraint 8..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
			IloExpr C8(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t i = 0; i < flight_leg_day1.size(); i++) {
				C8 += x[i][j];
			}
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				C8 += y[j][k];
			}

			C8 -= (d[j] * 2);

			cname1 = "C8_" + to_string(j);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C8 <= 0);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C8.end();
		}


	
	cout << "Creating constraint Prova..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
			IloExpr C20(env);
			//cout << " Creo vincolo per passeggiero " << endl;
			for (size_t i = 0; i < flight_leg_day1.size(); i++) {
				C20 += x[i][j];
			}
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				C20 += y[j][k];
			}

			C20 -= (d[j]);

			cname1 = "C20_" + to_string(j);
			//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
			IloRange cons(C20 >= 0);
			//cons.setName(cname1.c_str());
			model.add(cons);
			C20.end();
		}
	
		

		cout << "Creating constraint 9..." << endl;
		IloExpr C9(env);
		//cout << " Creo vincolo per passeggiero " << endl;
		for (size_t j = 0; j < flight_leg_day2.size(); j++) {
			C9 += d[j];
		}
		for (size_t i = 0; i < flight_leg_day1.size(); i++) {
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				C9 += z[i][k];
			}
		}

		string cname9;
		cname9 = "C9_" + to_string(0);
		//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
		IloRange cons(C9 <= num_aerei);
		//cons.setName(cname9.c_str());
		model.add(cons);
		C9.end();

		
		cout << "Creating constraint 10..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
			if (!flight_leg_day1[i].flight_leg_fittizzio) {
				IloExpr C10(env);
				//cout << " Creo vincolo per passeggiero " << endl;
				for (size_t j = 0; j < flight_leg_day2.size(); j++) {
					C10 += mat_depot_1_2[i][j] * x[i][j];
				}
				for (size_t k = 0; k < flight_leg_day3.size(); k++) {
					if (!flight_leg_day3[k].flight_leg_fittizzio) {
						C10 += mat_depot_1_3[i][k] * z[i][k];
					}
				}
				cname1 = "C10_" + to_string(i);
				//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
				IloRange cons(C10 >= 1);
				//cons.setName(cname1.c_str());
				model.add(cons);
				C10.end();
			}

		}

		cout << "Creating constraint 11..." << endl;
		// Devo capite come fare il For su tutti i Passeggieri........... Non so quanti siano
		for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
			if (!flight_leg_day2[j].flight_leg_fittizzio) {
				IloExpr C11(env);
				//cout << " Creo vincolo per passeggiero " << endl;
				for (size_t i = 0; i < flight_leg_day1.size(); i++) {
					C11 += mat_depot_1_2[i][j] * x[i][j];
				}
				for (size_t k = 0; k < flight_leg_day3.size(); k++) {
					if (!flight_leg_day3[k].flight_leg_fittizzio) {
						C11 += mat_depot_2_3[j][k] * y[j][k];
					}
				}
				cname1 = "C11_" + to_string(j);
				//cout << " sto aggiungendo il vincolo: " << cname1 << endl;
				IloRange cons(C11 >= 1);
				//cons.setName(cname1.c_str());
				model.add(cons);
				C11.end();
			}

		}

		cout << "vincolo del numero massimo di aerei da prendere" << endl;
		cout << "vincolo del ritorno al deposito" << endl;


		cout << "Creating objective function ...." << endl;
		IloExpr objective(env);
		for (size_t i = 0; i < flight_leg_day1.size(); i++) {
			for (size_t j = 0; j < flight_leg_day2.size(); j++) {
				objective += x[i][j] * (costo_connessione[flight_leg_day1[i].cod_airplane][flight_leg_day1[i].to][flight_leg_day2[j].from] +
					flight_leg_day1[i].cost + flight_leg_day2[j].cost);
			}
		}
		for (size_t j = 0; j < flight_leg_day2.size(); j++) {
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				objective += y[j][k] * (costo_connessione[flight_leg_day2[j].cod_airplane][flight_leg_day2[j].to][flight_leg_day3[k].from] +
					flight_leg_day2[j].cost + flight_leg_day3[k].cost);
			}
		}
		for (size_t i = 0; i < flight_leg_day1.size(); i++) {
			for (size_t k = 0; k < flight_leg_day3.size(); k++) {
				objective += z[i][k] * (costo_connessione[flight_leg_day1[i].cod_airplane][flight_leg_day1[i].to][flight_leg_day3[k].from] +
					flight_leg_day1[i].cost + flight_leg_day3[k].cost);
			}
		}

		for (size_t j = 0; j < flight_leg_day2.size(); j++) {
			//IloInt c = j;// column to sum
			IloExpr sc(env);
			for (IloInt i = 0; i < flight_leg_day1.size(); i++) {
				sc += x[i][j];
			}
			for (IloInt k = 0; k < flight_leg_day3.size(); k++) {
				sc += y[j][k];
			}
			objective += ((d[j] - sc ) * (flight_leg_day2[j].cost + 1060));

		}

		model.add(IloMinimize(env, objective));
		objective.end();




		// Set Parametres
		//cplex.setParam(IloCplex::IntParam::MIPEmphasis, 4);
		cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 0.05);			// MIP GAP
		cplex.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 0.05);
		cplex.setParam(IloCplex::NumParam::TiLim, 1200);			// Time Limit
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
		cplex.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);
		
		// End of Paramenters setting
		//cplex.exportModel("modello.lp");

		
		cplex.solve();
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value  = " << cplex.getObjValue() << endl;
		if (cplex.getStatus() == IloAlgorithm::Infeasible ||
			cplex.getStatus() == IloAlgorithm::InfeasibleOrUnbounded) {
			env.out() << endl << "*** Model is infeasible ***" << endl << endl;
			infeasible = true;
		}
		else {
			double val;
			cout << " SOolution Feasible or Optimal .... Printing the Results .. " << endl;
			 //  R; = X
		    // S =  Y 
			// T = Z
			cout << endl;
			cout << " Stampo variabili X  .. " << endl;
			for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
				vector<bool> GG1;
				for (size_t j = 0; j < flight_leg_day2.size(); j++) {
					val = cplex.getValue(x[i][j]);
					
					if (val > 0.01) {
						GG1.push_back(1);
						cout << "Variabile   x[" << i << "," << j << "] = " << val << endl;
						cout << "giorno 1" << endl;
						flight_leg_day1[i].print();
						cout << "giorno 2" << endl;
						flight_leg_day2[j].print();
					}else GG1.push_back(0);
				}
				R.push_back(GG1);
			}

			cout << endl;
			cout << " Stampo variabili Y .. " << endl;
			for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
				vector<bool> GG2;
				for (size_t k = 0; k < flight_leg_day3.size(); k++) {
					val = cplex.getValue(y[j][k]);
					
					if (val > 0.01) {
						GG2.push_back(1);
						cout << "Variabile   y[" << j << "," << k << "] = " << val << endl;
						cout << "giorno 2" << endl;
						flight_leg_day2[j].print();
						cout << "giorno 3" << endl;
						flight_leg_day3[k].print();
					}else GG2.push_back(0);
				}
				S.push_back(GG2);
			}

			cout << endl;
			double val_d = 0;
			cout << " Stampo variabile D .. " << endl;
			for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
				val_d = cplex.getValue(d[j]);
				if (val_d > 0.01) {
					cout << "Variabile   D[" << j << "] = " << val_d << endl;
					flight_leg_day2[j].print();
				}
			}

			cout << endl;
			cout << " Stampo variabili Z  .. " << endl;
			for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
				vector<bool> GG1;
				for (size_t k = 0; k < flight_leg_day3.size(); k++) {
					val = cplex.getValue(z[i][k]);
					
					if (val > 0.01) {
						GG1.push_back(1);
						cout << "Variabile   z[" << i << "," << k << "] = " << val << endl;
						cout << "giorno 1" << endl;
						flight_leg_day1[i].print();
						cout << "giorno 3" << endl;
						flight_leg_day3[k].print();
					}else GG1.push_back(0);
				}
				T.push_back(GG1);
			}
		

			


		}
	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}

	env.end();
	return infeasible;

}

