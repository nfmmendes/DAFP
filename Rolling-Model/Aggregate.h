
#include <iostream>

#include "Route.h"

vector <Route> aggrezione_simple_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to) {
	vector <Route> solution_after;

	cout << "*****************************SOLUTION PRIMA DELL'AGGREAGZIONE********************************" << endl;
	for (Route r : solution_model) {
		r.print();
	}
	cout << "****************************************ora faccio aggregazione con l'ultimo pezzo che si attacca solo se i due posti fine e inizio sono uguali***************" << endl;
	//cerco di collegare le route se � possibile
	//vector <Route> solution_after;
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";

			if (r != r1 && solution_model[r].places[solution_model[r].index - 1] == solution_model[r1].places[0] &&
				solution_model[r].time_dep[solution_model[r].index - 1] <= solution_model[r1].time_arr[0] &&
				map_airplane[solution_model[r].aircraft_code].model == map_airplane[solution_model[r1].aircraft_code].model
				&& index_not.find(val, 0) > index_not.size() && index_not.find(val1, 0) > index_not.size()) {

				index_not += to_string(r) + ";" + to_string(r1) + ";";
				Route r_add(solution_model[r].aircraft_code, solution_model[r].passengers_in_route);
				r_add.primo_pass = true;
				for (int i = 0; i < solution_model[r].index - 1; i++) {
					if (i == solution_model[r].index - 2) {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], (solution_model[r1].time_arr[0] - (((from_to[solution_model[r].places[i]][solution_model[r1].places[0]]) / map_airplane[solution_model[r].aircraft_code].speed) * 60)));
					}
					else {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], solution_model[r].time_dep[i]);
					}
				}

				for (int i = 0; i < solution_model[r1].index; i++) {
					r_add.addPlace(solution_model[r1].places[i], solution_model[r1].refueling[i], solution_model[r1].quantity_fuel[i], solution_model[r1].weight[i],
						solution_model[r1].capacity[i], solution_model[r1].time_arr[i], solution_model[r1].time_dep[i]);
				}
				for (Passenger& p : solution_model[r1].passengers_in_route) {
					p.solution_from += (solution_model[r].index - 1);
					p.solution_to += (solution_model[r].index - 1);
					r_add.passengers_in_route.push_back(p);

				}

				solution_after.push_back(r_add);
			}
		}
	}

	for (int r = 0; r < (int)solution_model.size(); r++) {
		string val = ";" + to_string(r) + ";";
		if (index_not.find(val, 0) > index_not.size()) {
			solution_after.push_back(solution_model[r]);
		}
	}





	cout << "*****************************SOLUTION MODEL -POSTERIORI- ********************************" << endl;
	cout << " Numero di soluzioni aggregate: " << solution_after.size() << endl;
	//for (Route r : solution_after) {
		//r.print();
	//}

	return solution_after;



}

vector <Route> aggrezione_complex_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector <Route> solution_after;

	//cout << "****************************************ora faccio aggregazione complessa***************" << endl;
	//cerco di collegare le route se � possibile
	//vector <Route> solution_after;
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";
			double time_trascorso = (from_to[solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]] / map_airplane[solution_model[r].aircraft_code].speed) * 60;
			double fuel_consumed = from_to_FuelConsumed[solution_model[r].aircraft_code][solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]];


			double costo_aggiunta = fuel_consumed + from_to[solution_model[r].places[solution_model[r].index - 1]][solution_model[r1].places[0]];
			//READ ME: non � importante fare il check se i due posti (finale di una route, inziale della seconda) siano uguali, questa casistica viene eliminata dall'aggregazione semplice
			//non devo nemmeno fare il check sul fuel per andare in un altro posto in quanto vado sicuramente in un deposito dato che tutte le route partono in un depot
			//CONSIDERAZIONE, SI POTREBBE VALUTARE DI TOGLIERE IL PRIMO ARCO DAL DEPOT ALLA PRIMA LOCATION NEL CASO QUESTO SIA VUOTO, MA QUESTO PORTEREBBE ALLA CREAZIONE DI SCENARI DIFFERENTI:
			//		-posso arrivare a tutte le altre locazioni se tolgo il refuel al depot???????????????????????????????????????????????????????????????????????????
			// ho aggiunto il fatto che il costo del tratto aggiuntivo deve essere minore o uguale al costo fisso dell'aereo
			if (r != r1 &&
				solution_model[r].time_dep[solution_model[r].index - 1] + time_trascorso <= solution_model[r1].time_arr[0] &&
				solution_model[r].quantity_fuel[solution_model[r].index - 1] - fuel_consumed >= map_airplane[solution_model[r].aircraft_code].min_fuel &&
				map_airplane[solution_model[r].aircraft_code].model == map_airplane[solution_model[r1].aircraft_code].model
				&& index_not.find(val, 0) > index_not.size() && index_not.find(val1, 0) > index_not.size()
				&& costo_aggiunta <= map_airplane[solution_model[r1].aircraft_code].fixed_cost
				) {

				index_not += to_string(r) + ";" + to_string(r1) + ";";
				Route r_add(solution_model[r].aircraft_code, solution_model[r].passengers_in_route);
				r_add.primo_pass = true; //N.B. commentare questa riga se si vuole vedere quelle che aggrega
				for (int i = 0; i <= solution_model[r].index - 1; i++) {
					if (i == solution_model[r].index - 1) {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], (solution_model[r1].time_arr[0] - (((from_to[solution_model[r].places[i]][solution_model[r1].places[0]]) / map_airplane[solution_model[r].aircraft_code].speed) * 60)));
					}
					else {
						r_add.addPlace(solution_model[r].places[i], solution_model[r].refueling[i], solution_model[r].quantity_fuel[i], solution_model[r].weight[i],
							solution_model[r].capacity[i], solution_model[r].time_arr[i], solution_model[r].time_dep[i]);
					}
				}

				for (int i = 0; i < solution_model[r1].index; i++) {
					r_add.addPlace(solution_model[r1].places[i], solution_model[r1].refueling[i], solution_model[r1].quantity_fuel[i], solution_model[r1].weight[i],
						solution_model[r1].capacity[i], solution_model[r1].time_arr[i], solution_model[r1].time_dep[i]);
				}
				for (Passenger& p : solution_model[r1].passengers_in_route) {
					p.solution_from += (solution_model[r].index);
					p.solution_to += (solution_model[r].index);
					r_add.passengers_in_route.push_back(p);

				}

				solution_after.push_back(r_add);
			}
		}
	}

	for (int r = 0; r < (int)solution_model.size(); r++) {
		string val = ";" + to_string(r) + ";";
		if (index_not.find(val, 0) > index_not.size()) {
			solution_after.push_back(solution_model[r]);
		}
	}


	std::cout << "*****************************SOLUTION MODEL -POSTERIORI- ********************************" << std::endl;
	for (Route& r : solution_after) {
		r.print();
	}

	return solution_after;



}

