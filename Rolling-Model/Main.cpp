#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <map>
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include <chrono>


#include "Evaluate.h"
#include "Fill.h"
#include "Move.h"
#include "Flightleg.h"
#include "Fuel.h"
#include "Matrix.h"
#include "Model_Connection.h"
#include "SolutionOneDay.h"
#include "Utils.h"

using namespace std;

//const int numero_airstrip_const = 58;
//const int numero_airplane_const = 36;



void Print_Model_Result(vector<vector<bool>>& R, vector<vector<bool>>& S, vector<vector<bool>>& T, vector <Flightleg> flight_leg_day1, vector <Flightleg> flight_leg_day2, vector <Flightleg> flight_leg_day3) {
	cout << " Stampo variabili X  .. " << endl;
	for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
		for (size_t j = 0; j < flight_leg_day2.size(); j++) if (R[i][j] > 0.01) cout << "Variabile   x[" << i << "," << j << "] = " << R[i][j] << endl;
	}

	cout << endl;
	cout << " Stampo variabili Y .. " << endl;
	for (int j = 0; j < (int)flight_leg_day2.size(); j++) {
		for (size_t k = 0; k < flight_leg_day3.size(); k++) if (S[j][k] > 0.01) cout << "Variabile   y[" << j << "," << k << "] = " << S[j][k] << endl;
	}

	cout << endl;
	cout << " Stampo variabili Z  .. " << endl;
	for (int i = 0; i < (int)flight_leg_day1.size(); i++) {
		for (size_t k = 0; k < flight_leg_day3.size(); k++) if (T[i][k] > 0.01) cout << "Variabile   z[" << i << "," << k << "] = " << T[i][k] << endl;
	}
}


void creazione_route_three_Days_for_evaluation_Cost(vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed, map<int, Airplane>& map_airplane, vector<Route>& route_day1, vector<Route>& route_day2, vector<Route>& route_day3, vector<vector<bool>>& x_i_j, vector<vector<bool>>& y_j_k, vector<vector<bool>>& z_i_k, vector<Flightleg>& flightlegs_day1, vector<Flightleg>& flightlegs_day2, vector<Flightleg>& flightlegs_day3, vector<vector<vector<double>>>& costo_connessione) {

	cout << "day 1: " << flightlegs_day1.size() << endl;
	cout << "day 2: " << flightlegs_day2.size() << endl;
	cout << "day 3: " << flightlegs_day3.size() << endl;
	double costo = 0.0;
	double costo_connessione_km = 0.0;
	double costo_connessione_fuel = 0.0;

	string check_per_j = ";";

	cout << "******************************************primo for con i j e dentro l'if con j e k*************************************************" << endl;
	for (int i = 0; i < (int)flightlegs_day1.size(); i++) {
		for (int j = 0; j < (int)flightlegs_day2.size(); j++) {

			if (x_i_j[i][j]) {
				cout << "i: " << i << "   j: " << j << endl;
				//qua c'è la connessione fra la route i e quella j
				if (!flightlegs_day1[i].flight_leg_fittizzio) {
					route_day1.push_back(flightlegs_day1[i].route_padre);
					flightlegs_day2[j].route_padre.airplane_day_before = true;
					route_day2.push_back(flightlegs_day2[j].route_padre);
					//per il costo***************************************************************************************************
					costo += costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day2[j].from];
					costo_connessione_km += from_to[flightlegs_day1[i].to][flightlegs_day2[j].from];
					costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day2[j].from];
					cout << "costo di connessione: " << costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day2[j].from] << endl;
					costo += flightlegs_day1[i].cost;
					cout << "costo flightleg1 " << flightlegs_day1[i].cost << endl;
					costo += flightlegs_day2[j].cost - map_airplane[flightlegs_day2[j].cod_airplane].fixed_cost;
					cout << "costo flightleg2 " << flightlegs_day2[j].cost << endl;
					cout << "costo fisso flightleg2 " << map_airplane[flightlegs_day2[j].cod_airplane].fixed_cost << endl;
					//***************************************************************************************************************


					//ora guardo quelli connessi attraverso j in k
					for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
						if (y_j_k[j][k]) {
							check_per_j += to_string(j) + ";";	//HO AGGIUNTO QUESTA RIGA PER NON CONTARE PIU' VOLTE Y_J_K
							cout << "j: " << j << "   k: " << k << endl;
							if (!flightlegs_day3[k].flight_leg_fittizzio) {
								flightlegs_day3[k].route_padre.airplane_day_before = true;
								route_day3.push_back(flightlegs_day3[k].route_padre);

								//per il costo***************************************************************************************************
								costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_km += from_to[flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								cout << "costo connessione" << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
								costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
								cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
								//***************************************************************************************************************
							}
						}
					}

				}
				else {
					route_day2.push_back(flightlegs_day2[j].route_padre);
					//per il costo***************************************************************************************************
					costo += flightlegs_day2[j].cost;
					cout << flightlegs_day2[j].cost << endl;
					//***************************************************************************************************************
					for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
						if (y_j_k[j][k]) {
							check_per_j += to_string(j) + ";";	//HO AGGIUNTO QUESTA RIGA PER NON CONTARE PIU' VOLTE Y_J_K
							if (!flightlegs_day3[k].flight_leg_fittizzio) {
								flightlegs_day3[k].route_padre.airplane_day_before = true;
								route_day3.push_back(flightlegs_day3[k].route_padre);
								//per il costo***************************************************************************************************
								costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_km += from_to[flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
								costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
								cout << "costo connessione " << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
								cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
								//***************************************************************************************************************
							}
						}
					}
				}

			}
			/*else {
				for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
					if (y_j_k[j][k]) {
						cout << "j: " << j << " k: " << k << endl;
						route_day2.push_back(flightlegs_day2[j].route_padre);
						//per il costo***************************************************************************************************
						costo += flightlegs_day2[j].cost;
						cout << flightlegs_day2[j].cost << endl;
						//***************************************************************************************************************
						if (!flightlegs_day3[k].flight_leg_fittizzio) {
							flightlegs_day3[k].route_padre.airplane_day_before = true;
							route_day3.push_back(flightlegs_day3[k].route_padre);
							//per il costo***************************************************************************************************
							costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
							cout << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
							costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
							cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
							//***************************************************************************************************************
						}
					}
				}



			}*/

		}
	}

	cout << "***************************secondo for con solo j e k che non sono stati beccati prima**********************************************" << endl;
	for (int j = 0; j < (int)flightlegs_day2.size(); j++) {
		for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
			if (y_j_k[j][k]) {
				string j_string = ";"+to_string(j)+";";
				size_t found = check_per_j.find(j_string);
				if (found == string::npos) {  //SE CI SONO DEI PROBLEMI GUARDA QUESTO IF
					cout << "j: " << j << " k: " << k << endl;
					route_day2.push_back(flightlegs_day2[j].route_padre);
					//per il costo***************************************************************************************************
					costo += flightlegs_day2[j].cost;
					cout << flightlegs_day2[j].cost << endl;
					//***************************************************************************************************************
					if (!flightlegs_day3[k].flight_leg_fittizzio) {
						flightlegs_day3[k].route_padre.airplane_day_before = true;
						route_day3.push_back(flightlegs_day3[k].route_padre);
						//per il costo***************************************************************************************************
						costo += costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
						costo_connessione_km += from_to[flightlegs_day2[j].to][flightlegs_day3[k].from];
						costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from];
						cout << costo_connessione[flightlegs_day2[j].cod_airplane][flightlegs_day2[j].to][flightlegs_day3[k].from] << endl;
						costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
						cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
						//***************************************************************************************************************
					}
				}
			}
		}
		
	}

	

	cout << "il costo e' prima del secondo big for: " << costo << endl;

	cout << "***********************************************************************************SECONDO BIG FOR***********************************************************************************" << endl;

	for (int i = 0; i < (int)flightlegs_day1.size(); i++) {
		for (int k = 0; k < (int)flightlegs_day3.size(); k++) {
			if (z_i_k[i][k]) {
				if (!flightlegs_day1[i].flight_leg_fittizzio) {
					route_day1.push_back(flightlegs_day1[i].route_padre);
					//per il costo***************************************************************************************************
					costo += flightlegs_day1[i].cost;
					cout << flightlegs_day1[i].cost << endl;
					//***************************************************************************************************************
					if (!flightlegs_day3[k].flight_leg_fittizzio) {
						flightlegs_day3[k].route_padre.airplane_day_before = true;
						route_day3.push_back(flightlegs_day3[k].route_padre);
						//per il costo***************************************************************************************************
						costo += costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day3[k].from];
						costo_connessione_km += from_to[flightlegs_day1[i].to][flightlegs_day3[k].from];
						costo_connessione_fuel += from_to_FuelConsumed[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day3[k].from];
						cout << "connessione " << costo_connessione[flightlegs_day1[i].cod_airplane][flightlegs_day1[i].to][flightlegs_day3[k].from] << endl;
						costo += flightlegs_day3[k].cost - map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost;
						cout << flightlegs_day3[k].cost << " - " << map_airplane[flightlegs_day3[k].cod_airplane].fixed_cost << endl;
						//***************************************************************************************************************
					}
				}
				else {
					if (!flightlegs_day3[k].flight_leg_fittizzio) {
						route_day3.push_back(flightlegs_day3[k].route_padre);
						//per il costo***************************************************************************************************
						costo += flightlegs_day3[k].cost;
						cout << flightlegs_day3[k].cost << endl;
						//***************************************************************************************************************
					}
				}

			}

		}
	}

	cout << "ecco il costo -------------------------------------> " << costo << endl;

	cout << "connessione km: " << costo_connessione_km << endl;
	cout << "connessione fuel: " << costo_connessione_fuel << endl;

}

//***********************************MAIN****************************
int main(int argc, char* argv[]) {


	//cout << argv[1] << endl;
	//cout << argv[2] << endl;
	//cout << argv[3] << endl;

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
	/*cout << "stampo la legenda" << endl;
	for (auto x : legenda) {
		cout << x.first << "----->" << x.second << "il ritroso e':  "<< x.second << " -----> " << legenda_ritroso[x.second] << endl;
	}
	system("pause");*/



	//reading of AIRPLANE.CSV and creation VECTOR OF AIRPLANES
	vector<Airplane> airplanes = fillAirplane("NewAirplaneAll.csv", legenda);
	Build_structure_Model(airplanes, mappa_aereo_tipo, codice_aereo_tipo, tipo_numero);
	//cout << "***********************Print airplane***********************" << endl;
	//for (auto f : airplanes) cout << "il modello dell'aereo e': " << f.model << endl;
	//system("pause");
	//reading of AIRSTRIP.CSV and creation VECTOR OF AIRSTRIPS
	vector<Airstrip> airstrips = fillAirstrip("NewAirstrip.csv", legenda);
	//cout << "***********************Print airstrips***********************" << endl;
	//for (auto f : airstrips) f.print();
	//reading of INSTANCE.CSV and creation VECTOR OF PASSENGER
	vector<Passenger> passengers_day1 = fillPassenger(argv[1], legenda);
	vector<Passenger> passengers_day2 = fillPassenger(argv[5], legenda);
	vector<Passenger> passengers_day3 = fillPassenger(argv[9], legenda);
	//CODICE DA SCOMMENTARE PER BAMBINI
	vector<Passenger> passengers_for_company_solution_day1;
	vector<Passenger> passengers_for_company_solution_day2;
	vector<Passenger> passengers_for_company_solution_day3;
	unione_children_INTELLIGENTE(passengers_day1, passengers_for_company_solution_day1);
	unione_children_INTELLIGENTE(passengers_day2, passengers_for_company_solution_day2);
	unione_children_INTELLIGENTE(passengers_day3, passengers_for_company_solution_day3);


	fix_key_passenger_for_regret(passengers_day1, passengers_day2, passengers_day3);


	map<int, Passenger> map_id_passenger_day1 = fillMapPassenger(passengers_day1);
	map<int, Passenger> map_id_passenger_day2 = fillMapPassenger(passengers_day2);
	map<int, Passenger> map_id_passenger_day3 = fillMapPassenger(passengers_day3);

	map<string, double> from_to_company = fillMatrixCompany("Matrix.csv", airstrips);
	//bulding of hashmap from_to reading the file Matrix.txt
	vector<vector<double>> from_to;
	fillMatrix(from_to, "Matrix.csv", airstrips);

	map<int, Airplane> map_airplane;
	for (auto f : airplanes) map_airplane.insert(make_pair(f.code, f));

	map<int, Airstrip> map_airstrip;
	for (auto f : airstrips) map_airstrip.insert(make_pair(f.code, f));
	//ora faccio la mappa che mi permette di avere come chiave ogni aereo/aeroporto, come valore il fuel necessario, dato quell'aereo per andare nell'aereoporto pi� vicino con quel fuel
	vector<vector<double>> location_fuel;
	fillLocation_fuel(location_fuel, airstrips, airplanes, from_to, map_airstrip);

	vector<vector<vector<double>>> from_to_FuelConsumed;
	fill_from_to_fuel_consumed(from_to_FuelConsumed, from_to, airplanes);
	map<string, double> from_to_fuel_consumed_string = fill_from_to_fuel_consumed_string(from_to_company, airplanes);



	vector<Passenger> passengers_per_casostrano_day1 = passengers_day1;
	vector<Passenger> passengers_per_casostrano_day2 = passengers_day2;
	vector<Passenger> passengers_per_casostrano_day3 = passengers_day3;


	double Company_Solution = calculationCostCompany_three_days(peso_TW, from_to_fuel_consumed_string, peso_transhipment, peso_itermediate_stop, argv[2], argv[6], argv[10], argv[3], argv[7], argv[11], airstrips, airplanes, passengers_for_company_solution_day1, passengers_for_company_solution_day2, passengers_for_company_solution_day3, from_to_company);
	cout << " Costo della soluzione della compagnia per i tre giorni = " << Company_Solution << endl;
	//system("pause");



	//ora creo la mappa che mi dice per ogni locazione dov'è il posto più vicino per fare benzina, se nel posto considerato posso fare benzina il valore sarà uguale alla chiave
	map < int, int> to_closeness_fuel = fill_to_closeness_fuel(from_to, airstrips);

	map <int, int> location_request_day2 = fill_location_request(passengers_day2);

	int best_location_day2 = best_location(location_request_day2);

	map <int, int> location_request_day3 = fill_location_request(passengers_day3);

	int best_location_day3 = best_location(location_request_day3);

	sort(passengers_day2.begin(), passengers_day2.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
		//return lhs.mean_arrival < rhs.mean_arrival;
	});

	sort(passengers_day3.begin(), passengers_day3.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
		//return lhs.mean_arrival < rhs.mean_arrival;
	});

	int best_location_4places_day2 = best_location_4place(location_request_day2, from_to, airstrips);
	//cout << "ecco la migliore locazione più vincina alle migliori 4 per il giorno 2: " << best_location_4places_day2 << endl;
	int best_location_4places_day3 = best_location_4place(location_request_day3, from_to, airstrips);
	//cout << "ecco la migliore locazione più vincina alle migliori 4 per il giorno 3: " << best_location_4places_day3 << endl;

	vector<vector<double>> min_fuel_day2;
	vector<vector<double>> min_fuel_day3;
	fuel_min_80_major_places(min_fuel_day2, from_to_FuelConsumed, location_request_day2, airplanes, airstrips);
	fuel_min_80_major_places(min_fuel_day3, from_to_FuelConsumed, location_request_day3, airplanes, airstrips);


	//creo la matrice per il costo della connessione
	vector<vector<vector<double>>> costo_connessione = matrice_connessione(from_to, from_to_FuelConsumed, airplanes, airstrips);
	double start_day = 360.0;
	double end_day = 1110.0;
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

	int numero_aerei = 20;


	int numero_aerei_1 = stoi(argv[4]);
	int numero_aerei_2 = stoi(argv[8]);
	int numero_aerei_3 = stoi(argv[12]);

	int Iter_FirstDo = 0;
	int iterMAX_FirstDo = 5;
	vector<vector<bool>> R;
	vector<vector<bool>> S;
	vector<vector<bool>> T;
	//risolvo il giorno 1
	vector<vector<Route>> vector_solution_for_FL_day1;
	vector<Route> best_route_day1;
	best_route_day1 = solution_one_day(vector_solution_for_FL_day1, Iter_FirstDo, iterMAX_FirstDo, numero_aerei_1, airplanes, peso_TW, peso_itermediate_stop, map_airplane,
		map_airstrip, from_to, location_fuel, from_to_FuelConsumed, passengers_per_casostrano_day1, from_to_company, legenda_ritroso, map_id_passenger_day1, passengers_day1, mappa_aereo_tipo,
		codice_aereo_tipo, tipo_numero);
	cout << " Ho numero di route pari a ... " << vector_solution_for_FL_day1.size() << endl;
	vector<Flightleg> flightlegs_day1 = creazione_flightleg(true, vector_solution_for_FL_day1, map_airplane, from_to_FuelConsumed, from_to);
	cout << " Ho numero di flightlegs pari a ... " << flightlegs_day1.size() << endl;
	cout << " --------------->>>>>>>>>>>> Finito il primo giorno <<<<<<<<<<< -----------------------------------------" << endl;
	system("pause");
	//risolvo il giorno 2
	vector<vector<Route>> vector_solution_for_FL_day2;
	vector<Route> best_route_day2;
	best_route_day2 = solution_one_day(vector_solution_for_FL_day2, Iter_FirstDo, iterMAX_FirstDo, numero_aerei_2, airplanes, peso_TW, peso_itermediate_stop, map_airplane,
		map_airstrip, from_to, location_fuel, from_to_FuelConsumed, passengers_per_casostrano_day2, from_to_company, legenda_ritroso, map_id_passenger_day2, passengers_day2, mappa_aereo_tipo,
		codice_aereo_tipo, tipo_numero);
	cout << " Ho numero di route pari a ... " << vector_solution_for_FL_day2.size() << endl;
	vector<Flightleg> flightlegs_day2 = creazione_flightleg(false, vector_solution_for_FL_day2, map_airplane, from_to_FuelConsumed, from_to);
	cout << " --------------->>>>>>>>>>>> Finito il secondo giorno <<<<<<<<<<< -----------------------------------------" << endl;
	cout << " Ho numero di flightlegs pari a ... " << flightlegs_day2.size() << endl;
	system("pause");
	//risolvo il giorno 3
	vector<vector<Route>> vector_solution_for_FL_day3;
	vector<Route> best_route_day3;
	best_route_day3 = solution_one_day(vector_solution_for_FL_day3, Iter_FirstDo, iterMAX_FirstDo, numero_aerei_3, airplanes, peso_TW, peso_itermediate_stop, map_airplane,
		map_airstrip, from_to, location_fuel, from_to_FuelConsumed, passengers_per_casostrano_day3, from_to_company, legenda_ritroso, map_id_passenger_day3, passengers_day3, mappa_aereo_tipo,
		codice_aereo_tipo, tipo_numero);
	cout << " Ho numero di route pari a ... " << vector_solution_for_FL_day3.size() << endl;
	vector<Flightleg> flightlegs_day3 = creazione_flightleg(true, vector_solution_for_FL_day3, map_airplane, from_to_FuelConsumed, from_to);
	cout << " Ho numero di flightlegs pari a ... " << flightlegs_day3.size() << endl;
	cout << " --------------->>>>>>>>>>>> Finito il terzo giorno <<<<<<<<<<< -----------------------------------------" << endl;
	system("pause");

	//creo le strutture per il modello

	vector<vector<int>> A = creazione_matrice_connection_FL(start_day, end_day, flightlegs_day1, flightlegs_day2, map_airplane, from_to, from_to_FuelConsumed);
	vector<vector<int>> B = creazione_matrice_connection_FL(start_day, end_day, flightlegs_day2, flightlegs_day3, map_airplane, from_to, from_to_FuelConsumed);
	vector<vector<int>> C = creazione_matrice_connection_FL(start_day, end_day, flightlegs_day1, flightlegs_day3, map_airplane, from_to, from_to_FuelConsumed);
	vector<vector<int>> FL_pass_day1 = creazione_matrice_flightleg_passengers(flightlegs_day1, passengers_day1);
	vector<vector<int>> FL_pass_day2 = creazione_matrice_flightleg_passengers(flightlegs_day2, passengers_day2);
	vector<vector<int>> FL_pass_day3 = creazione_matrice_flightleg_passengers(flightlegs_day3, passengers_day3);
	vector<vector<int>> mat_depot_1_2 = creazione_matrice_for_depot(map_airplane, flightlegs_day1, flightlegs_day2);
	vector<vector<int>> mat_depot_1_3 = creazione_matrice_for_depot(map_airplane, flightlegs_day1, flightlegs_day3);
	vector<vector<int>> mat_depot_2_3 = creazione_matrice_for_depot(map_airplane, flightlegs_day2, flightlegs_day3);
	cout << " -------------------->>> CHIAMO IL MODELLO CHE ABBIAMO CREATO <<<<-------------------------------------" << endl;
	system("pause");
	//chiamo il modello
	Model_Connection *modello_connsessio = new Model_Connection(mat_depot_1_2, mat_depot_2_3, mat_depot_1_3, costo_connessione, A, B, C, flightlegs_day1, flightlegs_day2, flightlegs_day3, FL_pass_day1, FL_pass_day2, FL_pass_day3);
	bool infeasible = modello_connsessio->create_Model_connection(R,S,T,(int)passengers_day1.size(), (int)passengers_day2.size(), (int)passengers_day3.size(), numero_aerei);
	cout << " -------------------->>> FINITO LA CHIAMATA DEL MODELLO  <<<<-------------------------------------" << endl;
	if(!infeasible) Print_Model_Result(R, S, T, flightlegs_day1, flightlegs_day2, flightlegs_day3);
	
	//ora creo le route dei tre giorni
	vector<Route> route_finali_day1;
	vector<Route> route_finali_day2;
	vector<Route> route_finali_day3;
	cout << "sto iniziando la creazione delle route con la valutazione del loro costo" << endl;
	creazione_route_three_Days_for_evaluation_Cost(from_to, from_to_FuelConsumed,  map_airplane, route_finali_day1, route_finali_day2, route_finali_day3, R, S, T, flightlegs_day1, flightlegs_day2, flightlegs_day3, costo_connessione);
	
	
	
	
	cout << "QUESTA CHE RIPORTO è UNA STAMPA PER VALUTARE SE SONO STATE PRESE TUTTE LE ROUTE DEI DIVERSI GIORNI" << endl;
	cout << "*********************************ecco il giorno 1*********************************" << endl;
	int num_passenger_day1 = 0;
	for (Route r : route_finali_day1) {
		r.print();
		num_passenger_day1 += r.passengers_in_route.size();
	}
	cout << "num_pass ---  pass_veri day1" << endl;
	cout << num_passenger_day1 << " --- " << passengers_day1.size() << endl;

	cout << "*********************************ecco il giorno 2*********************************" << endl;
	int num_passenger_day2 = 0;
	for (Route r : route_finali_day2) {
		r.print();
		num_passenger_day2 += r.passengers_in_route.size();
	}
	cout << "num_pass ---  pass_veri day2" << endl;
	cout << num_passenger_day2 << " --- " << passengers_day2.size() << endl;
	cout << "*********************************ecco il giorno 3*********************************" << endl;
	int num_passenger_day3 = 0;
	for (Route r : route_finali_day3) {
		r.print();
		num_passenger_day3 += r.passengers_in_route.size();
	}
	cout << "num_pass ---  pass_veri day3" << endl;
	cout << num_passenger_day3 << " --- " << passengers_day3.size() << endl;
	
	
	
	
	cout << "il costo e': " <<
		calculate_ObjectiveFunction_complete_after_rolling(peso_TW, peso_itermediate_stop, route_finali_day1, route_finali_day2, route_finali_day3,
			map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	

	//ECCO LE STAMPE FINALI
	double stampa_compagnia = calculationCostCompany_three_days(peso_TW, from_to_fuel_consumed_string, peso_transhipment, peso_itermediate_stop, argv[2], argv[6], argv[10], argv[3], argv[7], argv[11], airstrips, airplanes, passengers_for_company_solution_day1, passengers_for_company_solution_day2, passengers_for_company_solution_day3, from_to_company);
	double stampa_nostra = calculate_ObjectiveFunction_complete_after_rolling(peso_TW, peso_itermediate_stop, route_finali_day1, route_finali_day2, route_finali_day3,
		map_airstrip, map_airplane, from_to, from_to_FuelConsumed);




	cout << "fine del main" << endl;
	system("pause");
	return 0;
}
