#ifndef MODEL_CPLEX_H
#define MODEL_CPLEX_H

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include"Airplane.h"
#include"Route.h"
#include"Passenger.h"
//#include <ilcplex/ilocplex.h>
using namespace std;

class Model_Cplex {

public:
	int nbRows, nbColumns, grb_solStatus, NRichieste;
	double nbNodes, modelLB, modelUB, modelGap, modelExec_time;
	vector<vector<vector<int>>> A;
	vector<vector<double>> C;
	vector<Airplane> airplane;
	map<int, vector<Route>> airplane_routes;
	vector<Airplane> Aerei;
	vector<Route> All_Route;
	vector<Passenger> All_Passenger;
	vector<Route> solution_model;
	vector<Route> solution_after;
	map <int, Airplane> map_airplane;
	int NumberAirplane;
	//Methods
	Model_Cplex(vector<vector<vector<int>>> A, vector<vector<double>> C, vector<Airplane>& airplane, map<int, vector<Route>>& airplane_routes, vector<Route>& All_Route, const int NRichieste, vector<Passenger>& All_Passenger, vector<Route>& solution_model, map <int, Airplane>& map_airplane);	//constructor
	virtual~Model_Cplex();			//destructor
	void create_Model_cplex(int NumberAirplane, map<int, int>& solution_warm_up, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero, vector<vector<Route>>& solutionAll);
	void create_Model_for_SP_cplex(int NumberAirplane);




};

#endif
