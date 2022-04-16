#ifndef MODEL_CONNECTION_H
#define MODEL_CONNECTION_H

#include <vector>
#include "Flightleg.h";  
using namespace std;

class Model_Connection {

public:
	int nbRows, nbColumns, grb_solStatus, NRichieste;
	double nbNodes, modelLB, modelUB, modelGap, modelExec_time;
	vector<vector<int>> A;
	vector<vector<int>> B;
	vector<vector<int>> C;
	vector <Flightleg> flight_leg_day1;
	vector <Flightleg> flight_leg_day2;
	vector <Flightleg> flight_leg_day3;
	vector<vector<int>> FL_P_day1;
	vector<vector<int>> FL_P_day2;
	vector<vector<int>> FL_P_day3;
	vector<vector<vector<double>>> costo_connessione;
	vector<vector<int>> mat_depot_1_2;
	vector<vector<int>> mat_depot_2_3;
	vector<vector<int>> mat_depot_1_3;
	

	//Methods
	Model_Connection( vector<vector<int>>& mat_depot_1_2, vector<vector<int>>& mat_depot_2_3, vector<vector<int>>& mat_depot_1_3, vector<vector<vector<double>>>& costo_connessione, vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, vector <Flightleg>& flight_leg_day1, vector <Flightleg>& flight_leg_day2, vector <Flightleg>& flight_leg_day3, vector<vector<int>>& FL_P_day1, vector<vector<int>>& FL_P_day2, vector<vector<int>>& FL_P_day3 );	//constructor
	virtual~Model_Connection();			//destructor
	bool create_Model_connection(vector<vector<bool>>&, vector<vector<bool>>&, vector<vector<bool>>&, int, int, int, int);


};

#endif
