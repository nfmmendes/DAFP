#include "Route.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;


Route::Route()
{
	aircraft_code = 0;
	aircraft_code_company_comparison = "";
	places;
	time_arr;
	time_dep;
	index = 0;
	refueling;
	quantity_fuel;
	weight;
	capacity;
	places_company;
	
	primo_pass[0] = false;
	primo_pass[1] = false;
	primo_pass[2] = false;
	starting_locations;
	//first index is the route, second 0=start, 1 = end
	start_end_routes[0][0] = -1;
	start_end_routes[0][1] = -1;
	start_end_routes[1][0] = -1;
	start_end_routes[1][1] = -1;
	start_end_routes[2][0] = -1;
	start_end_routes[2][1] = -1;
	//airplane_day_before = false;

	passengers_in_route;
	cost = 0.0;

}


Route::Route(int aircraft_code, vector<Passenger>& pass)
{
	this->aircraft_code = aircraft_code;
	places;
	index = 0;
	refueling;
	quantity_fuel;
	time_arr;
	time_dep;
	places_company;


	weight;
	capacity;

	primo_pass[0] = false;
	primo_pass[1] = false;
	primo_pass[2] = false;
	starting_locations;
	//first index is the route, second 0=start, 1 = end
	start_end_routes[0][0] = -1;
	start_end_routes[0][1] = -1;
	start_end_routes[1][0] = -1;
	start_end_routes[1][1] = -1;
	start_end_routes[2][0] = -1;
	start_end_routes[2][1] = -1;
	//airplane_day_before = false;
	aircraft_code_company_comparison = "";
	passengers_in_route = pass;
	cost = 0.0;
}

void Route::print() {
				
	cout << "-" << to_string(aircraft_code) << endl;
	cout << "ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
	cout << "ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
	cout << "ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
	cout << "starting locations: " << endl;
	for (int x : starting_locations) cout << " - " << x << endl;
	cout << "la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
	cout << "la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
	cout << "la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;

	//cout << "aereo giorno prima: " << airplane_day_before << endl;
				cout << "\t";
				for (int t = 0; t < index; t++) {
					cout << " ->" << to_string(places[t]) << "***\t";
				}
				cout << endl;
				cout << "\t";
				for (int z = 0; z < index; z++) {
					cout << " t_a(" << time_arr[z] << ")\t";
				}
				cout << endl;
				cout << "\t";
				for (int z = 0; z < index; z++) {
					cout << " t_d(" << time_dep[z] << ")\t";
				}
				cout << endl;
				cout << "\t";
				for (int z = 0; z < index; z++) {
					cout << " q_f(" << quantity_fuel[z] << ")\t";
				}
				cout << endl;
				cout << "\t";
				for (int z = 0; z < index; z++) {
					cout << " c_p(" << capacity[z] << "***)\t";
				}
				cout << endl;
				cout << "\t";
				for (int z = 0; z < index; z++) {
					cout << " w_p(" << weight[z] << ")\t";
				}
				cout << endl;
				cout << "\t";
				for (int z = 0; z < index; z++) {
					cout << " s_f(" << refueling[z] << "***)\t";
				}
				cout << endl;
				cout << "Ha i seguenti passeggeri: " << endl;
				for (auto p : passengers_in_route) {
					p.print();
				}

				cout << endl;
	



}

void Route::Print() {
	cout << "Index: " << index << endl;
	print1();
	cout << endl;
	print2();
	cout << endl;
	print3();
	cout << endl;




}

void Route::print1() {

	cout << "-" << to_string(aircraft_code) << endl;
	cout << " Route del Gionro 1 " << endl;
	cout << " ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
	cout << " ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
	cout << " ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
	cout << " la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
	cout << " la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
	cout << " la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;
	if (start_end_routes[0][1] > -1 && start_end_routes[0][0] != -1){
		cout << " starting locations: " << endl;
		for (int x : starting_locations) cout << " - " << x << endl;
		

		//cout << "aereo giorno prima: " << airplane_day_before << endl;
		cout << "\t";
		for (int t = 0; t <= start_end_routes[0][1]; t++) {
			cout << " ->" << to_string(places[t]) << "***\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= start_end_routes[0][1]; z++) {
			cout << " t_a(" << time_arr[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= start_end_routes[0][1]; z++) {
			cout << " t_d(" << time_dep[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= start_end_routes[0][1]; z++) {
			cout << " q_f(" << quantity_fuel[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= start_end_routes[0][1]; z++) {
			cout << " c_p(" << capacity[z] << "***)\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= start_end_routes[0][1]; z++) {
			cout << " w_p(" << weight[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= start_end_routes[0][1]; z++) {
			cout << " s_f(" << refueling[z] << "***)\t";
		}
		cout << endl;
		cout << "Ha i seguenti passeggeri: " << endl;
		for (auto p : passengers_in_route) {
			if (p.giorno == 1)p.print();
		}

		cout << endl;
	}
	else if (start_end_routes[0][1] == -1 && start_end_routes[0][0] == 0) {
		cout << " ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
		cout << " ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
		cout << " ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
		cout << " starting locations: " << endl;
		for (int x : starting_locations) cout << " - " << x << endl;
		cout << " la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
		cout << " la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
		cout << " la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;

		//cout << "aereo giorno prima: " << airplane_day_before << endl;
		cout << "\t";
		for (int t = 0; t <= 0; t++) {
			cout << " ->" << places[t] << "***\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " t_a(" << time_arr[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " t_d(" << time_dep[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " q_f(" << quantity_fuel[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " c_p(" << capacity[z] << "***)\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " w_p(" << weight[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " s_f(" << refueling[z] << "***)\t";
		}
		cout << endl;
		cout << "Ha i seguenti passeggeri: " << endl;
		for (auto p : passengers_in_route) {
			if (p.giorno == 1)p.print();
		}

		cout << endl;
	}
	else {
		cout << " No route nel giorno 1 " << endl;
	}





}
void Route::print2() {

	cout << "-" << to_string(aircraft_code) << endl;
	cout << " Route del Gionro 2 " << endl;
	cout << " ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
	cout << " ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
	cout << " ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
	cout << " la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
	cout << " la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
	cout << " la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;

	if (start_end_routes[1][1] > -1 && start_end_routes[1][0] != -1) {
		
		cout << " starting locations: " << endl;
		for (int x : starting_locations) cout << " - " << x << endl;
		
		//cout << "aereo giorno prima: " << airplane_day_before << endl;
		cout << "\t";
		for (int t = start_end_routes[1][0]; t <= start_end_routes[1][1]; t++) {
			cout << " ->" << to_string(places[t]) << "***\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[1][0]; z <= start_end_routes[1][1]; z++) {
			cout << " t_a(" << time_arr[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[1][0]; z <= start_end_routes[1][1]; z++) {
			cout << " t_d(" << time_dep[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[1][0]; z <= start_end_routes[1][1]; z++) {
			cout << " q_f(" << quantity_fuel[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[1][0]; z <= start_end_routes[1][1]; z++) {
			cout << " c_p(" << capacity[z] << "***)\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[1][0]; z <= start_end_routes[1][1]; z++) {
			cout << " w_p(" << weight[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[1][0]; z <= start_end_routes[1][1]; z++) {
			cout << " s_f(" << refueling[z] << "***)\t";
		}
		cout << endl;
		cout << "Ha i seguenti passeggeri: " << endl;
		for (auto p : passengers_in_route) {
			if (p.giorno == 2)p.print();
		}

		cout << endl;
	}
	else if (start_end_routes[1][1] == -1 && start_end_routes[1][0] == 0) {
		cout << " ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
		cout << " ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
		cout << " ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
		cout << " starting locations: " << endl;
		for (int x : starting_locations) cout << " - " << x << endl;
		cout << " la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
		cout << " la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
		cout << " la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;

		//cout << "aereo giorno prima: " << airplane_day_before << endl;
		cout << "\t";
		for (int t = 0; t <= 0; t++) {
			cout << " ->" << places[t] << "***\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " t_a(" << time_arr[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " t_d(" << time_dep[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " q_f(" << quantity_fuel[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " c_p(" << capacity[z] << "***)\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " w_p(" << weight[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " s_f(" << refueling[z] << "***)\t";
		}
		cout << endl;
		cout << "Ha i seguenti passeggeri: " << endl;
		for (auto p : passengers_in_route) {
			if (p.giorno == 1)p.print();
		}

		cout << endl;
	}
	else {
		cout << " No route nel giorno 2 " << endl;
	}





}
void Route::print3() {

	cout << "-" << to_string(aircraft_code) << endl;
	cout << " Route del Gionro 3 " << endl;
	cout << " ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
	cout << " ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
	cout << " ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
	cout << " la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
	cout << " la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
	cout << " la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;
	if (start_end_routes[2][1] > -1 && start_end_routes[2][0] != -1) {
		
		cout << " starting locations: " << endl;
		for (int x : starting_locations) cout << " - " << x << endl;
		

		//cout << "aereo giorno prima: " << airplane_day_before << endl;
		cout << "\t";
		for (int t = start_end_routes[2][0]; t <= start_end_routes[2][1]; t++) {
			cout << " ->" << to_string(places[t]) << "***\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[2][0]; z <= start_end_routes[2][1]; z++) {
			cout << " t_a(" << time_arr[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[2][0]; z <= start_end_routes[2][1]; z++) {
			cout << " t_d(" << time_dep[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[2][0]; z <= start_end_routes[2][1]; z++) {
			cout << " q_f(" << quantity_fuel[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[2][0]; z <= start_end_routes[2][1]; z++) {
			cout << " c_p(" << capacity[z] << "***)\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[2][0]; z <= start_end_routes[2][1]; z++) {
			cout << " w_p(" << weight[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = start_end_routes[2][0]; z <= start_end_routes[2][1]; z++) {
			cout << " s_f(" << refueling[z] << "***)\t";
		}
		cout << endl;
		cout << "Ha i seguenti passeggeri: " << endl;
		for (auto p : passengers_in_route) {
			if (p.giorno == 3)p.print();
		}

		cout << endl;
	}
	else if (start_end_routes[2][1] == -1 && start_end_routes[2][0] == 0) {
		cout << " ha passeggeri nel giorno 1: " << primo_pass[0] << endl;
		cout << " ha passeggeri nel giorno 2: " << primo_pass[1] << endl;
		cout << " ha passeggeri nel giorno 3: " << primo_pass[2] << endl;
		cout << " starting locations: " << endl;
		for (int x : starting_locations) cout << " - " << x << endl;
		cout << " la parte del primo giorno va da: " << start_end_routes[0][0] << " a " << start_end_routes[0][1] << endl;
		cout << " la parte del secondo giorno va da: " << start_end_routes[1][0] << " a " << start_end_routes[1][1] << endl;
		cout << " la parte del terzo giorno va da: " << start_end_routes[2][0] << " a " << start_end_routes[2][1] << endl;

		//cout << "aereo giorno prima: " << airplane_day_before << endl;
		cout << "\t";
		for (int t = 0; t <= 0; t++) {
			cout << " ->" << places[t] << "***\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " t_a(" << time_arr[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " t_d(" << time_dep[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " q_f(" << quantity_fuel[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " c_p(" << capacity[z] << "***)\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " w_p(" << weight[z] << ")\t";
		}
		cout << endl;
		cout << "\t";
		for (int z = 0; z <= 0; z++) {
			cout << " s_f(" << refueling[z] << "***)\t";
		}
		cout << endl;
		cout << "Ha i seguenti passeggeri: " << endl;
		for (auto p : passengers_in_route) {
			if (p.giorno == 1)p.print();
		}

		cout << endl;
	}
	else {
		cout << " No route nel giorno 3 " << endl;
	}
}
void Route::printCompany() {
	cout << "-" << aircraft_code_company_comparison << endl;
	cout << "\t";
	for (int t = 0; t < index; t++) {
		cout << " ->" << places_company[t] << "***\t";
	}
	cout << endl;
	cout << "\t";
	for (int z = 0; z < index; z++) {
		cout << " t_a(" << time_arr[z] << ")\t";
	}

	cout << endl;


}




void Route::addPlace_companySolution(string place, int minute) {
	places_company.push_back(place);
	time_arr.push_back(minute);
	index++;
	return;
}

void Route::addPlace(int place, bool refueling, double quantity_fuel, double weight, int capacity, double minute_arr, double minute_dep) {
	places.push_back(place);
	time_arr.push_back(minute_arr);
	time_dep.push_back(minute_dep);


	this->refueling.push_back(refueling);
	this->quantity_fuel.push_back(quantity_fuel);
	index++;

	this->weight.push_back(weight);
	this->capacity.push_back(capacity);
	//index_cap++;



	return;
}


void Route::removePlace(int position, map<int, Airplane>& map_airplane) {
	index = index - 1;
	/*cout << " Sto per valutare casisitiche appena introdotte linea Route 376" << endl;
	cout << "start_end_routes[0][0] " << start_end_routes[0][0] << endl;
	cout << "start_end_routes[0][1] " << start_end_routes[0][1] << endl;
	cout << "start_end_routes[1][0] " << start_end_routes[1][0] << endl;
	cout << "start_end_routes[1][1] " << start_end_routes[1][1] << endl;
	cout << "start_end_routes[2][0] " << start_end_routes[2][0] << endl;
	cout << "start_end_routes[2][1] " << start_end_routes[2][1] << endl;
	cout << "r.primo_pass[0] " << primo_pass[0] << endl;
	cout << "r.primo_pass[1] " << primo_pass[1] << endl;
	cout << "r.primo_pass[2] " << primo_pass[2] << endl;
	cout << " r.index: " << index << endl;
	cout << " Dvo rimuovere nodo in posizione " << position << endl;*/
	if (position <= start_end_routes[0][1]) {
		if (start_end_routes[0][1] != -1) start_end_routes[0][1]--;
		if (start_end_routes[1][0] != -1)start_end_routes[1][0] --;
		if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
		if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
		if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
		//cout << " Ho abbassato tutti caso 1" << endl;
	}
	else if (position <= start_end_routes[1][1]) {
		if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
		if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
		if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
		//cout << " Ho abbassato 2 e 3" << endl;
	}
	else {
		//cout << " Ho abbassato solo fine di 3 caso 3" << endl;
		if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
	}
	//index_cap = index_cap - 1;


	places.erase(places.begin() + position);
	time_arr.erase(time_arr.begin() + position);
	time_dep.erase(time_dep.begin() + position);
	refueling.erase(refueling.begin() + position);
	quantity_fuel.erase(quantity_fuel.begin() + position);
	weight.erase(weight.begin() + position);
	capacity.erase(capacity.begin() + position);





	int cont = 0;
	for (int i = 0; i < index; i++) {
		if (capacity[i] == 0) cont++;
	}
	if (cont == index) {
		//print();
		primo_pass[0] = false;
		//cout << "verificato" << endl;
	}

	int giorno = -1;
	int index_sup = index;
	for (int i = index_sup - 1; i > 1; i--) {

		if (capacity[i - 1] != 0) break;

		if (capacity[i - 1] == 0) {
			//Print();
			//cout << " Devo eliminare archi vuoti ... devo eliminare nodo: " << i << endl;
			places.erase(places.begin() + i);
			time_arr.erase(time_arr.begin() + i);
			time_dep.erase(time_dep.begin() + i);
			refueling.erase(refueling.begin() + i);
			quantity_fuel.erase(quantity_fuel.begin() + i);
			weight.erase(weight.begin() + i);
			capacity.erase(capacity.begin() + i);
			index = index - 1;
			
			//cout << " start_end_routes[0][1] " << start_end_routes[0][1] << " start_end_routes[1][1] " << start_end_routes[1][1] << " start_end_routes[2][1] " << start_end_routes[2][1] << endl;
			if (i <= start_end_routes[0][1]) {
				if (start_end_routes[0][1] != -1) start_end_routes[0][1]--;
				if (start_end_routes[1][0] != -1)start_end_routes[1][0] --;
				if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
				if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
				if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
				//cout << " Ho abbassato tutti caso 1" << endl;
			}
			else if (i <= start_end_routes[1][1]) {
				if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
				if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
				if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
				//cout << " Ho abbassato 3 caso 2" << endl;
			}
			else {
				//cout << " Ho abbassato solo fine di 3 caso 3" << endl;
				if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
			}
		}
	}


	//cout << " r.index: " << index << endl;
	if (index == 2 && capacity[0] == 0) {
		//cout << " CASO DA VALUTARE sono dove ho modificato " << endl;
		places.erase(places.begin() + 1);
		time_arr.erase(time_arr.begin() + 1);
		time_dep.erase(time_dep.begin() + 1);
		refueling.erase(refueling.begin() + 1);
		quantity_fuel.erase(quantity_fuel.begin() + 1);
		weight.erase(weight.begin() + 1);
		capacity.erase(capacity.begin() + 1);
		index = index - 1;
		start_end_routes[0][1] = -1;
		start_end_routes[1][1] = -1;
		start_end_routes[1][0] = -1;
		start_end_routes[2][1] = -1;
		start_end_routes[2][0] = -1;

		/*	if ((start_end_routes[0][1] - start_end_routes[0][0]) == 1) {
			start_end_routes[0][1] = -1;
			start_end_routes[1][1] = -1;
			start_end_routes[1][0] = 0;
			start_end_routes[2][1] = -1;
			start_end_routes[2][0] = 0;
			cout << " Devo sistemare 1 route " << endl;
		}
		else if ((start_end_routes[1][1] - start_end_routes[1][0]) == 1) {
			start_end_routes[1][1] = -1;
			start_end_routes[1][0] = 0;
			cout << " Devo sistemare 2 route " << endl;
		}
		else {
			start_end_routes[2][1] = -1;
			start_end_routes[2][0] = 0;
			cout << " Devo sistemare 3 route " << endl;
		}*/
		// qui hai scritto la capppella non è detto che sia del primo gg 
	

		//index_cap = index_cap - 1;
	}

	/*cout << " Finito eliminazione archi vuoti " << endl;
	cout << "start_end_routes[0][0]" << start_end_routes[0][0] << endl;
	cout << "start_end_routes[0][1]" << start_end_routes[0][1] << endl;
	cout << "start_end_routes[1][0]" << start_end_routes[1][0] << endl;
	cout << "start_end_routes[1][1]" << start_end_routes[1][1] << endl;
	cout << "start_end_routes[2][0]" << start_end_routes[2][0] << endl;
	cout << "start_end_routes[2][1]" << start_end_routes[2][1] << endl;
	cout << " r.index: " << index << endl;*/

	if (places.size() == 1 && capacity[0] == 0) {
		quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
		weight[0] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
	}

	for (int g = 0; g < 3; g++) {
		bool check_Pass = true;
		if (primo_pass[g] == 1) {
			check_Pass = false;
			for (Passenger p : passengers_in_route) {
				if (p.giorno == (g + 1)) {
					check_Pass = true;
					break;
				}

			}

			if (!check_Pass) primo_pass[g] = 0;

		}
	}


}

void Route::removePlace_New(int position, map<int, Airplane>& map_airplane) {
	index = index - 1;
	//cout << " Devo rimuovere nodo in posizione " << position << endl;
	if (position <= start_end_routes[0][1]) {
		if (start_end_routes[0][1] != -1) start_end_routes[0][1]--;
		if (start_end_routes[1][0] != -1)start_end_routes[1][0] --;
		if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
		if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
		if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
		//cout << " Ho abbassato tutti caso 1" << endl;
	}
	else if (position <= start_end_routes[1][1]) {
		if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
		if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
		if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
		//cout << " Ho abbassato 3 caso 2" << endl;
	}
	else {
		//cout << " Ho abbassato solo fine di 3 caso 3" << endl;
		if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
	}
	//index_cap = index_cap - 1;


	places.erase(places.begin() + position);
	time_arr.erase(time_arr.begin() + position);
	time_dep.erase(time_dep.begin() + position);
	refueling.erase(refueling.begin() + position);
	quantity_fuel.erase(quantity_fuel.begin() + position);
	weight.erase(weight.begin() + position);
	capacity.erase(capacity.begin() + position);





	int cont = 0;
	for (int i = 0; i < index; i++) {
		if (capacity[i] == 0) cont++;
	}
	if (cont == index) {
		//print();
		primo_pass[0] = false;
		//cout << "verificato" << endl;
	}


	for (int g = 0; g < 3; g++) {
		bool check_Pass = true;
		if (primo_pass[g] == 1) {
			check_Pass = false;
			for (Passenger p : passengers_in_route) {
				if (p.giorno == (g + 1)) {
					check_Pass = true;
					break;
				}

			}

			if (!check_Pass) primo_pass[g] = 0;

		}
	}


	if (places.size() == 1 && capacity[0] == 0) {
		quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
		weight[0] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
	}





}

void Route::removePlace_Arc(map<int, Airplane>& map_airplane) {

	int cont = 0;
	for (int i = 0; i < index; i++) {
		if (capacity[i] == 0) cont++;
	}
	if (cont == index) {
		//print();
		primo_pass[0] = false;
		//cout << "verificato" << endl;
	}

	int giorno = -1;
	int index_sup = index;
	for (int i = index_sup - 1; i > 1; i--) {

		if (capacity[i - 1] != 0) break;

		if (capacity[i - 1] == 0) {
			//if (aircraft_code == "5H-ARK") Print();
			//cout << " Devo eliminare archi vuoti ... devo eliminare nodo: " << i << endl;
			places.erase(places.begin() + i);
			time_arr.erase(time_arr.begin() + i);
			time_dep.erase(time_dep.begin() + i);
			refueling.erase(refueling.begin() + i);
			quantity_fuel.erase(quantity_fuel.begin() + i);
			weight.erase(weight.begin() + i);
			capacity.erase(capacity.begin() + i);
			index = index - 1;
			//index_cap = index_cap - 1;
			/*
			if (i == start_end_routes[0][1]) giorno = 0;
			else if (position == start_end_routes[1][1]) giorno = 1;
			else giorno = 2;
			start_end_routes[giorno][1]--;
			cout << " start_end_routes[0][1] " << start_end_routes[0][1] << " start_end_routes[1][1] " << start_end_routes[1][1] << " start_end_routes[2][1] " << start_end_routes[2][1] << endl;
			*/
			if (i <= start_end_routes[0][1]) {
				if (start_end_routes[0][1] != -1) start_end_routes[0][1]--;
				if (start_end_routes[1][0] != -1)start_end_routes[1][0] --;
				if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
				if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
				if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
				//cout << " Ho abbassato tutti caso 1" << endl;
			}
			else if (i <= start_end_routes[1][1]) {
				if (start_end_routes[1][1] != -1) start_end_routes[1][1] --;
				if (start_end_routes[2][0] != -1) start_end_routes[2][0] --;
				if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
				//cout << " Ho abbassato 3 caso 2" << endl;
			}
			else {
				//cout << " Ho abbassato solo fine di 3 caso 3" << endl;
				if (start_end_routes[2][1] != -1) start_end_routes[2][1] --;
			}
		}
	}
	//cout << " Finito eliminazione archi vuoti " << endl;

	if (index == 2 && capacity[0] == 0) {
		//cout << " --------------------------------->>>>> CASO DA VALUTARE  <<<<<----------------------------------------------------" << endl;
		//cout << " --------------------------------->>>>> ENTRO A ROUTE RIGA 667 POTREBBE DARE ERRORE CON I TEMPI  <<<<<----------------------------------------------------" << endl;
		places.erase(places.begin() + 1);
		time_arr.erase(time_arr.begin() + 1);
		time_dep.erase(time_dep.begin() + 1);
		refueling.erase(refueling.begin() + 1);
		quantity_fuel.erase(quantity_fuel.begin() + 1);
		weight.erase(weight.begin() + 1);
		capacity.erase(capacity.begin() + 1);
		index = index - 1;
		start_end_routes[0][1] = -1;
		start_end_routes[0][0] = 0;
		start_end_routes[1][1] = -1;
		start_end_routes[1][0] = -1;
		start_end_routes[2][1] = -1;
		start_end_routes[2][0] = -1;	
	}



	if (places.size() == 1 && capacity[0] == 0) {
		quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
		weight[0] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
	}





}

void Route::update_route_destroy_middleNode(int giorno, int node_destroy, int min_from_pass, int max_to_pass, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip) {

	for (Passenger& p : passengers_in_route) {
		if (p.solution_from > node_destroy) p.solution_from--;
		if (p.solution_to > node_destroy) p.solution_to--;
	}


	//cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
	//cout << "******************* Sono in Route.cpp voglio ridare fuel  *******************" << endl;
	//cout << " CODE ------> " << aircraft_code << endl;
	//cout << " Minimo from passeggeri ----> " << min_from_pass << endl;
	//cout << " Massimo to passeggeri ----> " << max_to_pass << endl;
	//print();
	//cout << " Node destroyed ------> " << node_destroy << endl;
	int index_min_from;
	for (int i = min_from_pass; i >= 0; i--) {
		if (refueling[i] && i != node_destroy) {
			index_min_from = i;
			break;
		}
	}
	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	// Codice che da reduel ai nodi con refuel non massimo 
	for (int k = index_min_from; k < max_to_pass; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel && k != node_destroy) { //&& k!= node_destroy
		//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i <= max_to_pass; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) break;
				if (weight[i] < min_weight && i != node_destroy) {
					min_weight = weight[i];
					Node_min = i;
				}
				//}
			}
			if (Node_min >= 0) {
				for (int i = k + 1; i < index; i++) {
					if (refueling[i]) {  // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
						index_updating_to = i;
						break;
					}
				}
				//cout << " Minimo trovato " << min_weight << endl;
				//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
				//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
				double Fuel_before = quantity_fuel[index_updating_from];
				//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[aircraft_code].max_fuel << " e " << quantity_fuel[index_updating_from] + min_weight << endl;
				quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
				weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
				for (int i = index_updating_from + 1; i < index_updating_to; i++) {
					if (i != node_destroy) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}

	}
}

void Route::update_route_destroy(int giorno, int node_destroy, int min_from_pass, int max_to_pass, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip) {

	time_arr[node_destroy + 1] = time_dep[node_destroy - 1] + from_to[places[node_destroy - 1]][places[node_destroy + 1]] / map_airplane[aircraft_code].speed * 60;
	if ((node_destroy + 1) < start_end_routes[giorno][1]) time_dep[node_destroy + 1] = time_arr[node_destroy + 1] + map_airstrip[places[node_destroy + 1]].ground_time;
	for (int i = node_destroy + 1; i < start_end_routes[giorno][1]; i++) {
		//cout << " Valore di i " << i << " Aggiorno " << (i + 1) << " con gg importante:" << start_end_routes[giorno][1] << endl;
		if ((i + 1) == start_end_routes[giorno][1] && (giorno != 2)) {
			//cout << " Sono nel punto importante aggiorno solo partenza " << endl;
			time_arr[i + 1] = time_dep[i] + from_to[places[i]][places[i + 1]] / map_airplane[aircraft_code].speed * 60;
		}
		else {
			time_arr[i + 1] = time_dep[i] + from_to[places[i]][places[i + 1]] / map_airplane[aircraft_code].speed * 60;
			time_dep[i + 1] = time_arr[i + 1] + map_airstrip[places[i + 1]].ground_time;
		}
	}

	for (Passenger& p : passengers_in_route) {
		if (p.solution_from > node_destroy) p.solution_from--;
		if (p.solution_to > node_destroy) p.solution_to--;
	}


	//cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
	//cout << "******************* Sono in Route.cpp voglio ridare fuel  *******************" << endl;
	//cout << " CODE ------> " << aircraft_code << endl;
	//cout << " Minimo from passeggeri ----> " << min_from_pass << endl;
	//cout << " Massimo to passeggeri ----> " << max_to_pass << endl;
	//print();
	//cout << " Node destroyed ------> " << node_destroy << endl;
	int index_min_from;
	for (int i = min_from_pass; i >= 0; i--) {
		if (refueling[i] && i != node_destroy) {
			index_min_from = i;
			break;
		}
	}
	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	// Codice che da reduel ai nodi con refuel non massimo 
	for (int k = index_min_from; k < max_to_pass; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel && k != node_destroy) { //&& k!= node_destroy
		//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i <= max_to_pass; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) break;
				if (weight[i] < min_weight && i != node_destroy) {
					min_weight = weight[i];
					Node_min = i;
				}
				//}
			}
			if (Node_min >= 0) {
				for (int i = k + 1; i < index; i++) {
					if (refueling[i]) {  // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
						index_updating_to = i;
						break;
					}
				}
				//cout << " Minimo trovato " << min_weight << endl;
				//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
				//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
				double Fuel_before = quantity_fuel[index_updating_from];
				//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[aircraft_code].max_fuel << " e " << quantity_fuel[index_updating_from] + min_weight << endl;
				quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
				weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
				for (int i = index_updating_from + 1; i < index_updating_to; i++) {
					if (i != node_destroy) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}

	}
}





void Route::update_route_rebuilt_one(int node_add_from, int node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed) {

	bool non_to = false;
	bool non_to_final = false;
	bool num_equals = false;

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == index - 1) non_to_final = true;



	if (places[node_add_from] == location_from) {
		//il nodo prima al from è uguale devo quindi aggiornare le capacità e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}
		//cout << "pippo1" << endl;
		/*
		if (refueling[node_add_from] && weight[node_add_from] < 0) {
		quantity_fuel[node_add_from] += weight[node_add_from];
		for (int k = node_add_from + 1; k < index; k++) {
		if (refueling[k]) break;   //HO AGGIUNTO K >= node_add_to qua, prima non c'era
		quantity_fuel[k] += weight[node_add_from];
		weight[k] -= weight[node_add_from];
		}
		weight[node_add_from] = 0;
		}
		MAGARI E' QUESTO L'ERRORE
		else if (refueling[node_add_from]) {
		quantity_fuel[node_add_from] = map_airplane[aircraft_code].max_fuel;
		}
		*/



		/*int index_refueling = node_add_from;
		for (int i = node_add_from; i >= 0; i--) {
		if (refueling[i]) {
		index_refueling = i;
		break;
		}
		}
		double add_fuel = 0;
		//cout << "pippo2" << endl;
		for (int j = node_add_from; j < node_add_to; j++) {
		if (weight[j] < add_fuel) add_fuel = weight[j];
		}
		//cout <<"ttt---"<<index_refueling << endl;
		for (int t = index_refueling; t < index; t++) {
		if (refueling[t] && t != index_refueling) break;
		quantity_fuel[t] += add_fuel;
		weight[t] -= add_fuel;
		}
		*/
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j < index; j++) {
			if (weight[j] < 0) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 467 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				//ccout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//ccout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//ccout << " Weight dopo " << weight[t] << endl;
				}
			}
		}





	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		int node_support_to = node_add_to;


		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {


			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}


		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}
		/*
		if (refueling[node_add_from + 1] && weight[node_add_from + 1] < 0) {
		quantity_fuel[node_add_from + 1] += weight[node_add_from + 1];
		for (int k = node_add_from + 2; k < index; k++) {
		if (refueling[k]) break;
		quantity_fuel[k] += weight[node_add_from+1];
		weight[k] -= weight[node_add_from+1];
		}
		weight[node_add_from + 1] = 0;
		}
		MAGARI E' QUESTO L'ERRORE
		else if (refueling[node_add_from + 1]) {
		quantity_fuel[node_add_from + 1] = map_airplane[aircraft_code].max_fuel;
		}
		*/

		/*
		int index_refueling = node_add_from + 1;
		for (int i = node_add_from + 1; i >= 0; i--) {
		if (refueling[i]) {
		index_refueling = i;
		break;
		}
		}
		double add_fuel = 0;
		for (int j = node_add_from + 1; j < node_add_to; j++) {
		if (weight[j] < add_fuel) add_fuel = weight[j];
		}
		for (int t = index_refueling; t < index; t++) {
		if (refueling[t] && t != index_refueling) break;
		quantity_fuel[t] += add_fuel;
		weight[t] -= add_fuel;
		}
		*/


		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j < index; j++) {
			if (weight[j] < 0) {
				//ccout << " Sono nel rapeir devo aggiunger fuel linea 581 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
				//ccout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//ccout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//ccout << " Weight dopo " << weight[t] << endl;
				}
			}
		}

	}
	else {
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;
		p.solution_from = node_add_from + 1;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
			//node_add_to = node_add_to + 1;
			//non_to = true;
			//}

		}

		double time_add = time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_from + 1, time_add);
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}
		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}


		quantity_fuel.insert(quantity_fuel.begin() + node_add_from + 1, 0.0);

		weight.insert(weight.begin() + node_add_from + 1, 0.0);
		capacity.insert(capacity.begin() + node_add_from + 1, 0);

		//update of fuel, capacity and weight
		capacity[node_add_from + 1] = capacity[node_add_from];
		weight[node_add_from + 1] = weight[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		//fuel consumed
		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[node_add_from + 1]];
		//double travel_time = (from_to[places[node_add_from] + ";" + places[node_add_from + 1]] / map_airplane[aircraft_code].speed) * 60;
		/*
		if (travel_time <= 60) {
			fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
		}
		*/

		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[node_add_from + 1]) {
			quantity_fuel[node_add_from + 1] = map_airplane[aircraft_code].max_fuel;
			weight[node_add_from + 1] = weight[node_add_from + 1] - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[node_add_from + 1]) {
			quantity_fuel[node_add_from + 1] = quantity_fuel[node_add_from] - fuel_consumed;
			weight[node_add_from + 1] = weight[node_add_from + 1] + fuel_consumed;
		}

		/*
		if (refueling[node_add_from + 1] && weight[node_add_from + 1] < 0) {
		quantity_fuel[node_add_from + 1] += weight[node_add_from + 1];
		for (int k = node_add_from + 2; k < index; k++) {
		if (refueling[k] ) break;
		quantity_fuel[k] += weight[node_add_from + 1];
		weight[k] -= weight[node_add_from + 1];
		}
		weight[node_add_from + 1] = 0;
		}
		*/






		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[node_add_from + 1]][places[node_add_from + 2]];
		//double travel_time1 = (from_to[places[node_add_from + 1] + ";" + places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60;

		/*if (travel_time1 <= 60) {
			fuel_consumed1 += (travel_time1 / 60)*map_airplane[aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed1 += map_airplane[aircraft_code].fuel_burn_first + ((travel_time1 - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
		}*/

		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[node_add_from + 2]];
		/*double travel_time2 = (from_to[places[node_add_from] + ";" + places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60;
		if (travel_time2 <= 60) {
			fuel_consumed2 += (travel_time2 / 60)*map_airplane[aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed2 += map_airplane[aircraft_code].fuel_burn_first + ((travel_time2 - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
		}
		*/
		if (!refueling[node_add_from + 1]) {
			for (int i = node_add_from + 2; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
					weight[i] = weight[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
				}
				else {
					break;
				}
			}
		}
		else {
			//caso bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = node_add_from + 2; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
					weight[i] = weight[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
				}
				else {
					break;
				}
			}
		}


		/*
		int index_refueling = node_add_from + 1;
		for (int i = node_add_from + 1; i >= 0; i--) {
		if (refueling[i]) {
		index_refueling = i;
		break;
		}
		}
		double add_fuel = 0;
		for (int j = node_add_from + 1; j < node_add_to; j++) {
		if (weight[j] < add_fuel) add_fuel = weight[j];
		}

		for (int t = index_refueling; t < index; t++) {
		if (refueling[t] && t != index_refueling) break;
		quantity_fuel[t] += add_fuel;
		weight[t] -= add_fuel;
		}
		*/

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from + 1; j < index; j++) { //secondo me qua non è NODE_ADD_TO ma deve essere INDEX
			if (weight[j] < 0) {
				//ccout << " Sono nel rapeir devo aggiunger fuel linea 789 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
				//ccout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//ccout << " Weight dopo " << weight[t] << endl;
				}
			}
		}


	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		//cout << "places.size()=" << places.size() << endl;
		//cout << "node_add_to" <<node_add_to << endl;
		p.solution_to = node_add_to;
		double time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_to, time_add1);
		time_dep.insert(time_dep.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);
		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		//index_cap++;

		for (int i = node_add_to + 1; i < index; i++) {
			time_arr[i] += time1;
			time_dep[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from >= node_add_to) p.solution_from++;
			if (p.solution_to >= node_add_to) p.solution_to++;
		}

		quantity_fuel.insert(quantity_fuel.begin() + node_add_to, 0.0);

		double weight_add = 0.0;

		weight.insert(weight.begin() + node_add_to, weight_add);
		capacity.insert(capacity.begin() + node_add_to, 0);




		/////////////////////////////////////////
		//fuel consumed
		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];
		//double travel_time3 = (from_to[places[node_add_to - 1] + ";" + places[node_add_to]] / map_airplane[aircraft_code].speed) * 60;


		/*if (travel_time3 <= 60) {
			fuel_consumed3 += (travel_time3 / 60)*map_airplane[aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed3 += map_airplane[aircraft_code].fuel_burn_first + ((travel_time3 - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
		}*/


		if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}

		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacity[node_add_to] = capacity[node_add_to - 1] - 1;
			if (refueling[node_add_to]) {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
			}
			else {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}
		//********************************************************************************************************************************************************************************


		/*if (refueling[node_add_to] && weight[node_add_to] < 0) {
			quantity_fuel[node_add_to] += weight[node_add_to];
			for (int k = node_add_to + 1; k < index; k++) {
				if (refueling[k]) break;
				quantity_fuel[k] += weight[node_add_to];
				weight[k] -= weight[node_add_to];
			}
			weight[node_add_to] = 0;
		}
		else if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}*/


		double fuel_consumed4 = 0.0;
		double fuel_consumed5 = 0.0;
		//double travel_time4 = 0.0;

		if (!non_to_final) {

			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
			//travel_time4 = (from_to[places[node_add_to] + ";" + places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60;
			/*if (travel_time4 <= 60) {
				fuel_consumed4 += (travel_time4 / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed4 += map_airplane[aircraft_code].fuel_burn_first + ((travel_time4 - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}


		//double travel_time5 = 0.0;
		//if (!non_to_final) {


			/*travel_time5 = (from_to[places[node_add_to - 1] + ";" + places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60;
			if (travel_time5 <= 60) {
				fuel_consumed5 += (travel_time5 / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed5 += map_airplane[aircraft_code].fuel_burn_first + ((travel_time5 - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/
			//cout << "fuel 5: " << fuel_consumed5 << endl;
		//}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weight[i] = weight[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
					weight[i] = weight[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}
















		if (non_to_final) {
			//double time = (from_to[places[node_add_to - 1] + ";" + places[node_add_to]] / map_airplane[aircraft_code].speed) * 60;
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];

			/*if (time <= 60) {
				fuel_consumed += (time / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/



			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[node_add_to - 1] - fuel_consumed);
			}
			weight[node_add_to] += weight_add;
		}


		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO

		/*
		double add_fuel = 0;
		for (int j = node_add_to; j < index; j++) {
		if (weight[j] < add_fuel) add_fuel = weight[j];
		}
		if (add_fuel < 0.0) {
		int index_refueling = node_add_to;
		for (int i = node_add_to; i >= 0; i--) {
		if (refueling[i]) {
		index_refueling = i;
		break;
		}
		}
		for (int t = index_refueling; t < index; t++) {
		if (refueling[t] && t != index_refueling) break;
		quantity_fuel[t] += add_fuel;
		weight[t] -= add_fuel;
		}
		}
		*/

		//cout << "sono davanti all'aggiornamento del caso in cui ho inserito il to" << endl;
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_to; j < index; j++) {
			//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
			if (weight[j] < 0) {
				//cout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//cout << " Weight dopo " << weight[t] << endl;
				}
			}
		}



	}
	else {
		p.solution_to = node_add_to;
	}


	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	//cout << "caso prima dell'aggiornamento in fondo del repair" << endl;
	//print();
	for (int k = 0; k < index; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}


}

void Route::move_c(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {


	if (places[index - 1] == location_from) {

		capacity[index - 1]++;
		weight[index - 1] -= p.weight;

		double time_arri = time_dep[index - 1] + (from_to[location_from][location_to] / map_airplane[aircraft_code].speed) * 60;
		double time_depa = time_arri + map_airstrip[location_to].ground_time;
		p.solution_from = index - 1;

		//double time_fly = (from_to[location_from + ";" + location_to] / map_airplane[aircraft_code].speed) * 60;


		double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_from][location_to];
		/*if (time_fly <= 60) {
			fuel_consumed += (time_fly / 60)*map_airplane[aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
		}*/

		if (map_airstrip[location_to].fuel) {
			addPlace(location_to, map_airstrip[location_to].fuel, map_airplane[aircraft_code].max_fuel, (map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel), 0, time_arri, time_depa);
		}
		else {
			addPlace(location_to, map_airstrip[location_to].fuel, quantity_fuel[index - 1] - fuel_consumed, (map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[index - 1] - fuel_consumed)), 0, time_arri, time_depa);
		}

		p.solution_to = index - 1;


	}
	else {

		if (map_airstrip[p.departure_location].fuel) {
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1,
				time_dep[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				time_dep[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
				map_airplane[aircraft_code].max_fuel - p.weight;


		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;


			/*if (travel_time <= 60) {
				fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, time_dep[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				time_dep[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1] - p.weight;

		}
		p.solution_from = index - 1;
		double time = time_dep[index - 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;





		if (map_airstrip[p.arrival_location].fuel) {
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;

		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


			/*if (travel_time <= 60) {
				fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = weight[index - 2] + fuel_consumed + p.weight;

		}


		p.solution_to = index - 1;





	}







}


void Route::update_time_for_check_repair(int node_add_from, int node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip) {

	bool non_to = false;
	bool non_to_final = false;
	bool num_equals = false;

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == index - 1) non_to_final = true;



	if (places[node_add_from] == location_from) {
		//il nodo prima al from è uguale devo quindi aggiornare le capacità e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}

	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		int node_support_to = node_add_to;


		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {


			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}



	}
	else {
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
				//node_add_to = node_add_to + 1;
				//non_to = true;
			//}

		}

		time_arr.insert(time_arr.begin() + node_add_from + 1, time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60));
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}

	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		//cout << "places.size()=" << places.size() << endl;
		//cout << "node_add_to" <<node_add_to << endl;
		time_arr.insert(time_arr.begin() + node_add_to, time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60));
		time_dep.insert(time_dep.begin() + node_add_to, time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[location_to].ground_time);
		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		//index_cap++;

		for (int i = node_add_to + 1; i < index; i++) {
			time_arr[i] += time1;
			time_dep[i] += time1;
		}


	}




}



//for updating in order to improve the computational time
void Route::update_route_rebuilt_one_first_fase(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == index - 1) non_to_final = true;



	if (places[node_add_from] == location_from) {
		caso = 1; //impongo che questo è il caso 1
		//il nodo prima al from è uguale devo quindi aggiornare le capacità e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}



	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		caso = 2;
		int node_support_to = node_add_to;


		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {


			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= index) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= index) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= index) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}


	}
	else {
		caso = 3;
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
			//node_add_to = node_add_to + 1;
			//non_to = true;
			//}

		}

		double time_add = time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_from + 1, time_add);
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		//index_cap++;

		for (int i = node_add_from + 2; i < index; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}

		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_from + 1, 0.0);
		weight.insert(weight.begin() + node_add_from + 1, 0.0);
		capacity.insert(capacity.begin() + node_add_from + 1, 0);



	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		double time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_to, time_add1);
		time_dep.insert(time_dep.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);

		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		//index_cap++;


		for (int i = node_add_to + 1; i < index; i++) {
			time_arr[i] += time1;
			time_dep[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_to, 0.0);
		weight.insert(weight.begin() + node_add_to, 0.0);
		capacity.insert(capacity.begin() + node_add_to, 0);




	}



}

void Route::update_route_rebuilt_one_second_fase(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	//in questo commento qua sotto ci sono i vari casi con l'if al posto dello switch

	if (caso == -1) {
		cout << "problema in route.cpp con la variabile caso" << endl;
		cin.get();
	}
	else if (caso == 1) {

		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j < index; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 467 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
							//ccout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
			}

		}





	}
	else if (caso == 2) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j < index; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 581 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}

						//ccout << " Weight dopo " << weight[t] << endl;
					}
				}
			}

		}

	}
	else if (caso == 3) {

		p.solution_from = node_add_from + 1;

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}

		//update of fuel, capacity and weight
		capacity[node_add_from + 1] = capacity[node_add_from];
		weight[node_add_from + 1] = weight[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		//fuel consumed
		int A = 0;
		int B = 0;
		if (node_add_to == node_add_from + 1 && non_to) {
			A = node_add_from + 2;
			B = node_add_from + 3;
		}
		else if (node_add_to == node_add_from + 2 && non_to) {
			A = node_add_from + 1;
			B = node_add_from + 3;
		}
		else {
			A = node_add_from + 1;
			B = node_add_from + 2;
		}



		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[A]];
		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[A]) {
			quantity_fuel[A] = map_airplane[aircraft_code].max_fuel;
			weight[A] = weight[A] - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[A]) {
			quantity_fuel[A] = quantity_fuel[node_add_from] - fuel_consumed;
			weight[A] = weight[A] + fuel_consumed;
		}

		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[A]][places[B]];
		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[B]];
		if (!refueling[A]) {
			for (int i = B; i < index; i++) {
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
						weight[i] = weight[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
					}
					else {
						break;
					}
				}

			}
		}
		else {
			//caso bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = B; i < index; i++) {
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
						weight[i] = weight[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
					}
					else {
						break;
					}
				}
			}
		}


		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = A; j < index; j++) { //secondo me qua non è NODE_ADD_TO ma deve essere INDEX
			if (non_to && j == node_add_to) {
				//non devo fare niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 789 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (non_to && i == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (non_to && t == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}
						//ccout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
		}


	}


	if (non_to) {

		p.solution_to = node_add_to;
		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from >= node_add_to) p.solution_from++;
			if (p.solution_to >= node_add_to) p.solution_to++;
		}



		double weight_add = 0.0;
		/////////////////////////////////////////
		//fuel consumed


		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];


		if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}




		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacity[node_add_to] = capacity[node_add_to - 1] - 1;
			if (refueling[node_add_to]) {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
			}
			else {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}
		//********************************************************************************************************************************************************************************


		/*if (refueling[node_add_to] && weight[node_add_to] < 0) {
			quantity_fuel[node_add_to] += weight[node_add_to];
			for (int k = node_add_to + 1; k < index; k++) {
				if (refueling[k]) break;
				quantity_fuel[k] += weight[node_add_to];
				weight[k] -= weight[node_add_to];
			}
			weight[node_add_to] = 0;
		}
		else if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}*/



		double fuel_consumed4 = 0.0;
		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
		}

		double fuel_consumed5 = 0.0;
		if (!non_to_final) {
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weight[i] = weight[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
					weight[i] = weight[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}


		if (non_to_final) {
			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[node_add_to - 1] - from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]]);
			}
			weight[node_add_to] += weight_add;
		}


		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO


		



	}
	else {
		p.solution_to = node_add_to;
	}


	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	//cout << "caso prima dell'aggiornamento in fondo del repair" << endl;
	//print();
	for (int k = 0; k < index; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}


}










void Route::add_update_only_one_node_first_passanger_MP(double start_day1, double start_day2, double start_day3, Passenger& p, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {



	int number_add_locations = 0;
	bool add_number_in_passenger = false;


	//il vero codice inizia qua, prima è solo per fare copia incolla
	if (p.giorno == 1) {
		if (index > 1) {
			add_number_in_passenger = true;
			//in questo caso ho sicuramente qualcosa dopo, sia che sia giorno 2 che giorno tre
			//copio il deposito, in questo caso sicuramente sarà il depositio dell'aereo
			//in questo caso non devo neanche aggiornare dopo in quanto sicuramente avrò il deposito dopo, sto copiando infatti il deposito di partenza
			//devo però modificare l'orario di arrivo della locazione finale
			if (capacity[0] > 0) {
				places.insert(places.begin() + 0, places[0]);
				refueling.insert(refueling.begin() + 0, refueling[0]);
				quantity_fuel.insert(quantity_fuel.begin() + 0, map_airplane[aircraft_code].max_fuel);
				weight.insert(weight.begin() + 0, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
				capacity.insert(capacity.begin() + 0, 0);
				time_arr.insert(time_arr.begin() + 0, 0.0);
				time_dep.insert(time_dep.begin() + 0, 0.0);
				start_end_routes[0][0] = 0;
				index++;
				number_add_locations++;

				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}



				if (places[0] == p.departure_location) {
					time_arr[0] = p.early_departure;
					time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
					capacity[0] = 1;
					quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
					weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


					//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);

					places.insert(places.begin() + 1, p.arrival_location);
					refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
					quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
					weight.insert(weight.begin() + 1, 0.0);
					capacity.insert(capacity.begin() + 1, 0);
					time_arr.insert(time_arr.begin() + 1, time_dep[0] + time);
					time_dep.insert(time_dep.begin() + 1, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
					start_end_routes[0][1] = 2; //perchè c'è il nodo di collegamento
					index++;
					number_add_locations++;

					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}



					if (refueling[1] == true) {
						quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
						weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
					}
					else {
						quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
						weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
					}

					p.solution_from = 0;
					p.solution_to = 1;

					double time_volo = (from_to[p.arrival_location][places[2]] / map_airplane[aircraft_code].speed) * 60;
					time_arr[2] = time_dep[1] + time_volo;


				}
				else {

					time_arr[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
						map_airstrip[places[0]].ground_time;
					time_dep[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

					//for the weight
					weight[0] = map_airplane[aircraft_code].weight_fuel_people -
						quantity_fuel[0];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
					//end for weight

					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[0]][p.departure_location];
					if (map_airstrip[p.departure_location].fuel) {
						//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

						places.insert(places.begin() + 1, p.departure_location);
						refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + 1, map_airplane[aircraft_code].max_fuel);
						weight.insert(weight.begin() + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
						capacity.insert(capacity.begin() + 1, 1);
						time_arr.insert(time_arr.begin() + 1, p.early_departure);
						time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
						number_add_locations++;
						index++;

						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][0]++;
							start_end_routes[1][1]++;
						}
						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;
						}


						//weight[1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					}
					else {
						double fuel_before = quantity_fuel[0];


						places.insert(places.begin() + 1, p.departure_location);
						refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + 1, fuel_before - fuel_consumed);
						weight.insert(weight.begin() + 1, weight[0] - p.weight + fuel_consumed);
						capacity.insert(capacity.begin() + 1, 1);
						time_arr.insert(time_arr.begin() + 1, p.early_departure);
						time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
						number_add_locations++;
						index++;

						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][0]++;
							start_end_routes[1][1]++;
						}
						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;
						}


						//weight[1] = weight[0] - p.weight + fuel_consumed;

					}
					p.solution_from = 1;
					double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



					if (p.arrival_location != places[2]) {
						if (map_airstrip[p.arrival_location].fuel) {
							//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

							places.insert(places.begin() + 2, p.arrival_location);
							refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 2, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
							capacity.insert(capacity.begin() + 2, 0);
							time_arr.insert(time_arr.begin() + 2, time);
							time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
							number_add_locations++;
							index++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}



							//weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
						}
						else {
							double fuel_before = quantity_fuel[1];
							double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[1]][p.arrival_location];
							//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


							places.insert(places.begin() + 2, p.arrival_location);
							refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 2, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + 2, weight[1] + p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + 2, 0);
							time_arr.insert(time_arr.begin() + 2, time);
							time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
							number_add_locations++;
							index++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}



						}

						double time_volo = (from_to[places[2]][places[3]] / map_airplane[aircraft_code].speed) * 60;
						time_arr[3] = time_dep[2] + time_volo;
						start_end_routes[0][1] = 3;

					}
					else {
						double time_volo = (from_to[places[1]][places[2]] / map_airplane[aircraft_code].speed) * 60;
						time_arr[2] = time_dep[1] + time_volo;
						start_end_routes[0][1] = 2;

					}
					p.solution_to = 2;

				}

			}
			else {

				//check particolare per far si che non parta nel giorno dopo prima dello starting day
				double fly_check = (from_to[p.arrival_location][places[1]] / map_airplane[aircraft_code].speed) * 60;
				//cout << "time_arr[1]: " << time_arr[1] << endl;
				//cout << "fly_check: " << fly_check << endl;
				double start_day = 0.0;
				if (start_end_routes[1][0] != -1) start_day = start_day2;
				else start_day = start_day3;
				//questo check lo puoi cambiare con qualcosa di più evoluto tipo spostare in avanti tutto di N minuti, tanti quanto basta per rientrare nello starting day
				if (time_arr[1] - fly_check < start_day) {
					places.insert(places.begin() + 0, places[0]);
					refueling.insert(refueling.begin() + 0, refueling[0]);
					quantity_fuel.insert(quantity_fuel.begin() + 0, map_airplane[aircraft_code].max_fuel);
					weight.insert(weight.begin() + 0, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
					capacity.insert(capacity.begin() + 0, 0);
					time_arr.insert(time_arr.begin() + 0, 0.0);
					time_dep.insert(time_dep.begin() + 0, 0.0);
					start_end_routes[0][0] = 0;
					index++;
					number_add_locations++;

					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}



					if (places[0] == p.departure_location) {
						time_arr[0] = p.early_departure;
						time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
						capacity[0] = 1;
						quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
						weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


						//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);

						places.insert(places.begin() + 1, p.arrival_location);
						refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
						weight.insert(weight.begin() + 1, 0.0);
						capacity.insert(capacity.begin() + 1, 0);
						time_arr.insert(time_arr.begin() + 1, time_dep[0] + time);
						time_dep.insert(time_dep.begin() + 1, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
						start_end_routes[0][1] = 2; //perchè c'è il nodo di collegamento
						index++;
						number_add_locations++;

						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][0]++;
							start_end_routes[1][1]++;
						}
						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;
						}



						if (refueling[1] == true) {
							quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
							weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
						}
						else {
							quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
							weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
						}

						p.solution_from = 0;
						p.solution_to = 1;

						double time_volo = (from_to[p.arrival_location][places[2]] / map_airplane[aircraft_code].speed) * 60;
						time_arr[2] = time_dep[1] + time_volo;


					}
					else {

						time_arr[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
							map_airstrip[places[0]].ground_time;
						time_dep[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

						//for the weight
						weight[0] = map_airplane[aircraft_code].weight_fuel_people -
							quantity_fuel[0];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
						//end for weight

						double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[0]][p.departure_location];
						if (map_airstrip[p.departure_location].fuel) {
							//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}


							//weight[1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
						}
						else {
							double fuel_before = quantity_fuel[0];


							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + 1, weight[0] - p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}


							//weight[1] = weight[0] - p.weight + fuel_consumed;

						}
						p.solution_from = 1;
						double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



						if (p.arrival_location != places[2]) {
							if (map_airstrip[p.arrival_location].fuel) {
								//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, map_airplane[aircraft_code].max_fuel);
								weight.insert(weight.begin() + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;

								if (start_end_routes[1][1] != -1) {
									start_end_routes[1][0]++;
									start_end_routes[1][1]++;
								}
								if (start_end_routes[2][1] != -1) {
									start_end_routes[2][0]++;
									start_end_routes[2][1]++;
								}



								//weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
							}
							else {
								double fuel_before = quantity_fuel[1];
								double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[1]][p.arrival_location];
								//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, fuel_before - fuel_consumed);
								weight.insert(weight.begin() + 2, weight[1] + p.weight + fuel_consumed);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;

								if (start_end_routes[1][1] != -1) {
									start_end_routes[1][0]++;
									start_end_routes[1][1]++;
								}
								if (start_end_routes[2][1] != -1) {
									start_end_routes[2][0]++;
									start_end_routes[2][1]++;
								}



							}

							double time_volo = (from_to[places[2]][places[3]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[3] = time_dep[2] + time_volo;
							start_end_routes[0][1] = 3;

						}
						else {
							double time_volo = (from_to[places[1]][places[2]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[2] = time_dep[1] + time_volo;
							start_end_routes[0][1] = 2;

						}
						p.solution_to = 2;

					}

				}
				else {
					if (places[0] == p.departure_location) {
						time_arr[0] = p.early_departure;
						time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
						capacity[0] = 1;
						quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
						weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;
						start_end_routes[0][0] = 0;

						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


						if (p.arrival_location != places[1]) {
							//in questo caso il to diventa la nuova connessione
							double time_volo = (from_to[p.arrival_location][places[1]] / map_airplane[aircraft_code].speed) * 60;

							places.insert(places.begin() + 1, p.arrival_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
							weight.insert(weight.begin() + 1, 0.0);
							capacity.insert(capacity.begin() + 1, 0);
							time_arr.insert(time_arr.begin() + 1, time_dep[0] + time);
							time_dep.insert(time_dep.begin() + 1, time_arr[2] - time_volo);
							start_end_routes[0][1] = 1;
							index++;
							number_add_locations++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}



							if (refueling[1] == true) {
								quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
								weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
							}
							else {
								quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
								weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
							}

							p.solution_from = 0;
							p.solution_to = 1;


						}
						else {


							time_arr[1] = time_dep[0] + time;
							start_end_routes[0][1] = 1;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;

							}
							else if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;

							}




							p.solution_from = 0;
							p.solution_to = 1;


						}



					}
					else {
						start_end_routes[0][0] = 0;
						if (start_end_routes[1][1] != -1) start_end_routes[1][0]++;
						else start_end_routes[2][0]++;

						time_arr[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
							map_airstrip[places[0]].ground_time;
						time_dep[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

						//for the weight
						weight[0] = map_airplane[aircraft_code].weight_fuel_people -
							quantity_fuel[0];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
						//end for weight

						double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[0]][p.departure_location];
						if (map_airstrip[p.departure_location].fuel) {
							//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}


							//weight[1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
						}
						else {
							double fuel_before = quantity_fuel[0];


							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + 1, weight[0] - p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][0]++;
								start_end_routes[1][1]++;
							}
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;
							}


							//weight[1] = weight[0] - p.weight + fuel_consumed;

						}
						p.solution_from = 1;
						double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



						if (p.arrival_location != places[2]) {
							if (map_airstrip[p.arrival_location].fuel) {
								//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, map_airplane[aircraft_code].max_fuel);
								weight.insert(weight.begin() + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;

								if (start_end_routes[1][1] != -1) {
									start_end_routes[1][0]++;
									start_end_routes[1][1]++;
								}
								if (start_end_routes[2][1] != -1) {
									start_end_routes[2][0]++;
									start_end_routes[2][1]++;
								}



								//weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
							}
							else {
								double fuel_before = quantity_fuel[1];
								double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[1]][p.arrival_location];
								//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, fuel_before - fuel_consumed);
								weight.insert(weight.begin() + 2, weight[1] + p.weight + fuel_consumed);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;

								if (start_end_routes[1][1] != -1) {
									start_end_routes[1][0]++;
									start_end_routes[1][1]++;
								}
								if (start_end_routes[2][1] != -1) {
									start_end_routes[2][0]++;
									start_end_routes[2][1]++;
								}



							}

							double time_volo = (from_to[places[2]][places[3]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[3] = time_dep[2] + time_volo;
							start_end_routes[0][1] = 3;

						}
						else {
							double time_volo = (from_to[places[1]][places[2]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[2] = time_dep[1] + time_volo;
							start_end_routes[0][1] = 2;

						}
						p.solution_to = 2;

					}
					for (int i = start_end_routes[p.giorno - 1][1]; i < index; i++) {
						if (refueling[i] && i > start_end_routes[p.giorno - 1][1]) break;
						if (!refueling[i]) {
							double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
							double fb = quantity_fuel[i];
							quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
							weight[i] -= (quantity_fuel[i] - fb);
						}
					}
				}
			}


			//pezzo che pesca fuel;
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = 0; j < index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						quantity_fuel[t] += add_fuel;
						weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}


			//pezzo che ridà fuel
			for (int k = start_end_routes[p.giorno - 1][0]; k < index; k++) {
				if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
					//cout << " Sto valutando il caso del nodo " << k << endl;
					int index_updating_from = k;
					int index_updating_to = index;  //qua prima c'era -1
					int Node_min = k;
					double min_weight = weight[k];
					for (int i = k + 1; i < index; i++) {
						//cout << " Sto guardando il nodo " << i << endl;
						//cout << " Ho passato IF di controllo " << endl;
						if (refueling[i]) {
							index_updating_to = i;
							break;
						}
						if (weight[i] < min_weight) {
							min_weight = weight[i];
							Node_min = i;
						}

					}
					double Fuel_before = quantity_fuel[index_updating_from];
					quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
					weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}

				}
			}










		}
		else {
			//in questo caso la route è vuota e c'è solo il deposito, in questo caso posso usare index in quanto la route è vuota
			start_end_routes[0][0] = 0;

			if (places[0] == p.departure_location) {
				time_arr[0] = p.early_departure;
				time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
				capacity[0] = 1;
				quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
				weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


				double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];
				/*if (time <= 60) {
					fuel_consumed += (time / 60)*map_airplane[aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
				}*/

				addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
				if (refueling[1] == true) {
					quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
					weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
				}
				else {
					quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
					weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
				}

				p.solution_from = 0;
				p.solution_to = 1;


			}
			else {

				time_arr[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
					map_airstrip[places[index - 1]].ground_time;
				time_dep[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

				//for the weight
				weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
					quantity_fuel[index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				//end for weight

				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
				if (map_airstrip[p.departure_location].fuel) {
					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel - p.weight;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];

					//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;

					/*if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}*/

					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
					weight[index - 1] = weight[index - 2] - p.weight + fuel_consumed;

				}
				p.solution_from = index - 1;
				double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;




				if (map_airstrip[p.arrival_location].fuel) {
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
					//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}
					*/
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
					weight[index - 1] = weight[index - 2] + p.weight + fuel_consumed;

				}
				p.solution_to = index - 1;

			}

			start_end_routes[0][1] = index - 1;
		}
	}
	if (p.giorno == 3) {


		if (index > 1) {
			//ho sicuramente qualcosa prima del giorno 3 

			start_end_routes[2][0] = index - 1;
			//addPlace(places[index - 1], refueling[index - 1], quantity_fuel[index - 1], weight[index - 1], 0, 0.0, 0.0);


			if (places[index - 1] == p.departure_location) {
				//in questo caso non metto il controllo in quanto se le locazioni coincidono è impossibile che debba partire prima delle 6
				time_dep[index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
				capacity[index - 1] = 1;
				//quantity_fuel[index - 1] = map_airplane[aircraft_code].max_fuel;
				weight[index - 1] -= p.weight;


				double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


				addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[index - 1] + time, time_dep[index - 1] + time + map_airstrip[p.arrival_location].ground_time);
				if (refueling[index - 1] == true) {
					quantity_fuel[index - 1] = map_airplane[aircraft_code].max_fuel;
					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1];
				}
				else {
					quantity_fuel[index - 1] = quantity_fuel[index - 2] - fuel_consumed;
					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1];
				}

				p.solution_from = index - 2;
				p.solution_to = index - 1;


			}
			else {

				//qua devo mettere il controllo per non partire prima dello starting day
				double time_volo = (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				double time_new = p.early_departure - time_volo;
				if (time_new < start_day3 && places[index - 1] != 1 &&  places[index - 1] != 4) time_new = start_day3;


				time_dep[index - 1] = time_new;


				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
				if (map_airstrip[p.departure_location].fuel) {
					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel - p.weight;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];



					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
					weight[index - 1] = weight[index - 2] - p.weight + fuel_consumed;

				}
				p.solution_from = index - 1;
				double time = time_new + time_volo + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;




				if (map_airstrip[p.arrival_location].fuel) {
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
					//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}
					*/
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
					weight[index - 1] = weight[index - 2] + p.weight + fuel_consumed;

				}
				p.solution_to = index - 1;

			}

			start_end_routes[2][1] = index - 1;



		}
		else {
			//in questo caso la route è vuota ed ha solo il deposito, in questo caso posso usare index in quanto la route è vuota
			start_end_routes[2][0] = 0;

			if (places[0] == p.departure_location) {
				time_arr[0] = p.early_departure;
				time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
				capacity[0] = 1;
				quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
				weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


				double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];
				/*if (time <= 60) {
					fuel_consumed += (time / 60)*map_airplane[aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
				}*/

				addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
				if (refueling[1] == true) {
					quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
					weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
				}
				else {
					quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
					weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
				}

				p.solution_from = 0;
				p.solution_to = 1;


			}
			else {

				time_arr[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
					map_airstrip[places[index - 1]].ground_time;
				time_dep[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

				//for the weight
				weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
					quantity_fuel[index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				//end for weight

				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
				if (map_airstrip[p.departure_location].fuel) {
					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel - p.weight;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];

					//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;

					/*if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}*/

					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
					weight[index - 1] = weight[index - 2] - p.weight + fuel_consumed;

				}
				p.solution_from = index - 1;
				double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;




				if (map_airstrip[p.arrival_location].fuel) {
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
					//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}
					*/
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
					weight[index - 1] = weight[index - 2] + p.weight + fuel_consumed;

				}
				p.solution_to = index - 1;

			}

			start_end_routes[2][1] = index - 1;

		}
	}
	if (p.giorno == 2) {
		if (index <= 1) {
			//la route è vuota


			start_end_routes[1][0] = 0;

			if (places[0] == p.departure_location) {
				time_arr[0] = p.early_departure;
				time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
				capacity[0] = 1;
				quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
				weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


				double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];
				/*if (time <= 60) {
					fuel_consumed += (time / 60)*map_airplane[aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
				}*/

				addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
				if (refueling[1] == true) {
					quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
					weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
				}
				else {
					quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
					weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
				}

				p.solution_from = 0;
				p.solution_to = 1;


			}
			else {

				time_arr[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
					map_airstrip[places[index - 1]].ground_time;
				time_dep[index - 1] = p.early_departure - (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

				//for the weight
				weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
					quantity_fuel[index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				//end for weight

				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
				if (map_airstrip[p.departure_location].fuel) {
					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel - p.weight;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];

					//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;

					/*if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}*/

					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
					weight[index - 1] = weight[index - 2] - p.weight + fuel_consumed;

				}
				p.solution_from = index - 1;
				double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;




				if (map_airstrip[p.arrival_location].fuel) {
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
					//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}
					*/
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
					weight[index - 1] = weight[index - 2] + p.weight + fuel_consumed;

				}
				p.solution_to = index - 1;

			}

			start_end_routes[1][1] = index - 1;






		}
		else if (start_end_routes[0][1] != -1 && start_end_routes[2][1] != -1 && start_end_routes[1][0] == -1) {
			add_number_in_passenger = true;




			if (capacity[start_end_routes[2][0]] > 0) {
				double fuel_consumed_trip = 0.0; //questa variabile mi servirà per aggiornare il la route dopo
				//copio l'ultimo nodo della route del giorno 1
				places.insert(places.begin() + start_end_routes[2][0], places[start_end_routes[0][1]]);
				refueling.insert(refueling.begin() + start_end_routes[2][0], refueling[start_end_routes[0][1]]);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0], quantity_fuel[start_end_routes[0][1]]);
				weight.insert(weight.begin() + start_end_routes[2][0], map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[0][1]]);
				capacity.insert(capacity.begin() + start_end_routes[2][0], 0);
				time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_arr[start_end_routes[2][0]]);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0], 0.0);
				start_end_routes[1][0] = start_end_routes[2][0];
				index++;
				number_add_locations++;

				start_end_routes[2][0]++;
				start_end_routes[2][1]++;


				if (places[start_end_routes[1][0]] == p.departure_location) {
					//in questo caso non devo fare il check sui tempi in quanto sarà impossibile avere una richiesta di questo tipo
					//time_arr[start_end_routes[1][0]] = p.early_departure;
					time_dep[start_end_routes[1][0]] = p.early_departure + map_airstrip[p.departure_location].ground_time;
					capacity[start_end_routes[1][0]] = 1;
					//quantity_fuel[start_end_routes[1][0]] = map_airplane[aircraft_code].max_fuel;
					weight[start_end_routes[1][0]] -= p.weight;


					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


					//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);

					places.insert(places.begin() + start_end_routes[1][0] + 1, p.arrival_location);
					refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.arrival_location].fuel);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, 0.0);
					weight.insert(weight.begin() + start_end_routes[1][0] + 1, 0.0);
					capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 0);
					time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_dep[start_end_routes[1][0]] + time);
					time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_dep[start_end_routes[1][0]] + time + map_airstrip[p.arrival_location].ground_time);
					start_end_routes[1][1] = start_end_routes[1][0] + 1;
					index++;
					number_add_locations++;

					start_end_routes[2][0]++;
					start_end_routes[2][1]++;




					if (refueling[start_end_routes[1][1]] == true) {
						quantity_fuel[start_end_routes[1][1]] = map_airplane[aircraft_code].max_fuel;
						weight[start_end_routes[1][1]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][1]];
					}
					else {
						quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][0]] - fuel_consumed;
						weight[start_end_routes[1][1]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][1]];
					}

					p.solution_from = start_end_routes[1][0];
					p.solution_to = start_end_routes[1][1];
					start_end_routes[1][1]++;


					if (!map_airstrip[places[start_end_routes[1][1]]].fuel) {
						double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][1] - 1]][places[start_end_routes[1][1]]];
						double quantity_before = quantity_fuel[start_end_routes[1][1]];
						quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed_new;
						weight[start_end_routes[1][1]] -= (quantity_fuel[start_end_routes[1][1]] - quantity_before);
					}



				}
				else {

					//check per non partire prima dello start day
					double time_volo = (from_to[places[start_end_routes[1][0]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
					double time_new = p.early_departure - time_volo;
					if (time_new < start_day2 && places[start_end_routes[1][0]] != 1 && places[start_end_routes[1][0]] != 4) time_new = start_day2;

					time_dep[start_end_routes[1][0]] = time_new;

					//for the weight
					weight[start_end_routes[1][0]] = map_airplane[aircraft_code].weight_fuel_people -
						quantity_fuel[start_end_routes[1][0]];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
					//end for weight

					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][0]]][p.departure_location];
					if (map_airstrip[p.departure_location].fuel) {
						//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

						places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
						refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].max_fuel);
						weight.insert(weight.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
						capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
						time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
						time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
						number_add_locations++;
						index++;

						start_end_routes[2][0]++;
						start_end_routes[2][1]++;


						//weight[1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					}
					else {
						double fuel_before = quantity_fuel[start_end_routes[1][0]];


						places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
						refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, fuel_before - fuel_consumed);
						weight.insert(weight.begin() + start_end_routes[1][0] + 1, weight[start_end_routes[1][0]] - p.weight + fuel_consumed);
						capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
						time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
						time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
						number_add_locations++;
						index++;

						start_end_routes[2][0]++;
						start_end_routes[2][1]++;



						//weight[1] = weight[0] - p.weight + fuel_consumed;

					}
					p.solution_from = start_end_routes[1][0] + 1;



					if (p.arrival_location != places[start_end_routes[2][0]]) {
						double time = time_dep[start_end_routes[1][0] + 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						if (map_airstrip[p.arrival_location].fuel) {
							//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

							places.insert(places.begin() + start_end_routes[1][0] + 2, p.arrival_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 2, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 2, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + start_end_routes[1][0] + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 2, 0);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 2, time);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 2, time + map_airstrip[p.arrival_location].ground_time);
							start_end_routes[1][1] = start_end_routes[1][0] + 3;
							index++;
							number_add_locations++;
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;




							//weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
						}
						else {
							double fuel_before = quantity_fuel[start_end_routes[1][0] + 1];
							double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][0] + 1]][p.arrival_location];


							places.insert(places.begin() + start_end_routes[1][0] + 2, p.arrival_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 2, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 2, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + start_end_routes[1][0] + 2, weight[start_end_routes[1][0] + 1] + p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 2, 0);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 2, time);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 2, time + map_airstrip[p.arrival_location].ground_time);
							start_end_routes[1][1] = start_end_routes[1][0] + 3;
							index++;
							number_add_locations++;
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;




						}
						p.solution_to = start_end_routes[1][1] - 1;
					}
					else {
						start_end_routes[1][1] = start_end_routes[1][0] + 2;
						p.solution_to = start_end_routes[1][1];
					}


					if (!map_airstrip[places[start_end_routes[1][1]]].fuel) {
						double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][1] - 1]][places[start_end_routes[1][1]]];
						double quantity_before = quantity_fuel[start_end_routes[1][1]];
						quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed_new;
						weight[start_end_routes[1][1]] -= (quantity_fuel[start_end_routes[1][1]] - quantity_before);
					}



				}


				double time_fly = (from_to[places[start_end_routes[1][1] - 1]][places[start_end_routes[1][1]]] / map_airplane[aircraft_code].speed) * 60;
				time_arr[start_end_routes[1][1]] = time_dep[start_end_routes[1][1] - 1] + time_fly;

			}
			else {
				//in questo caso la capacità non è zero quindi non aggiungo il pezzo a vuoto
				//forse qua dovrò salvarmi qualche valore della partenza
				double fly_check = (from_to[p.arrival_location][places[start_end_routes[2][0]]] / map_airplane[aircraft_code].speed) * 60;
				if (time_arr[start_end_routes[2][0]] - fly_check < start_day3) {
					double fuel_consumed_trip = 0.0; //questa variabile mi servirà per aggiornare il la route dopo
					//copio l'ultimo nodo della route del giorno 1
					places.insert(places.begin() + start_end_routes[2][0], places[start_end_routes[0][1]]);
					refueling.insert(refueling.begin() + start_end_routes[2][0], refueling[start_end_routes[0][1]]);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0], quantity_fuel[start_end_routes[0][1]]);
					weight.insert(weight.begin() + start_end_routes[2][0], map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[0][1]]);
					capacity.insert(capacity.begin() + start_end_routes[2][0], 0);
					time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_arr[start_end_routes[2][0]]);
					time_dep.insert(time_dep.begin() + start_end_routes[2][0], 0.0);
					start_end_routes[1][0] = start_end_routes[2][0];
					index++;
					number_add_locations++;

					start_end_routes[2][0]++;
					start_end_routes[2][1]++;


					if (places[start_end_routes[1][0]] == p.departure_location) {
						//in questo caso non devo fare il check sui tempi in quanto sarà impossibile avere una richiesta di questo tipo
						//time_arr[start_end_routes[1][0]] = p.early_departure;
						time_dep[start_end_routes[1][0]] = p.early_departure + map_airstrip[p.departure_location].ground_time;
						capacity[start_end_routes[1][0]] = 1;
						//quantity_fuel[start_end_routes[1][0]] = map_airplane[aircraft_code].max_fuel;
						weight[start_end_routes[1][0]] -= p.weight;


						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


						//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);

						places.insert(places.begin() + start_end_routes[1][0] + 1, p.arrival_location);
						refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.arrival_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, 0.0);
						weight.insert(weight.begin() + start_end_routes[1][0] + 1, 0.0);
						capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 0);
						time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_dep[start_end_routes[1][0]] + time);
						time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_dep[start_end_routes[1][0]] + time + map_airstrip[p.arrival_location].ground_time);
						start_end_routes[1][1] = start_end_routes[1][0] + 1;
						index++;
						number_add_locations++;

						start_end_routes[2][0]++;
						start_end_routes[2][1]++;




						if (refueling[start_end_routes[1][1]] == true) {
							quantity_fuel[start_end_routes[1][1]] = map_airplane[aircraft_code].max_fuel;
							weight[start_end_routes[1][1]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][1]];
						}
						else {
							quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][0]] - fuel_consumed;
							weight[start_end_routes[1][1]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][1]];
						}

						p.solution_from = start_end_routes[1][0];
						p.solution_to = start_end_routes[1][1];
						start_end_routes[1][1]++;


						if (!map_airstrip[places[start_end_routes[1][1]]].fuel) {
							double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][1] - 1]][places[start_end_routes[1][1]]];
							double quantity_before = quantity_fuel[start_end_routes[1][1]];
							quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed_new;
							weight[start_end_routes[1][1]] -= (quantity_fuel[start_end_routes[1][1]] - quantity_before);
						}



					}
					else {

						//check per non partire prima dello start day
						double time_volo = (from_to[places[start_end_routes[1][0]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
						double time_new = p.early_departure - time_volo;
						if (time_new < start_day2 && places[start_end_routes[1][0]] != 1 && places[start_end_routes[1][0]] != 4) time_new = start_day2;

						time_dep[start_end_routes[1][0]] = time_new;

						//for the weight
						weight[start_end_routes[1][0]] = map_airplane[aircraft_code].weight_fuel_people -
							quantity_fuel[start_end_routes[1][0]];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
						//end for weight

						double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][0]]][p.departure_location];
						if (map_airstrip[p.departure_location].fuel) {
							//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

							places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							start_end_routes[2][0]++;
							start_end_routes[2][1]++;


							//weight[1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
						}
						else {
							double fuel_before = quantity_fuel[start_end_routes[1][0]];


							places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + start_end_routes[1][0] + 1, weight[start_end_routes[1][0]] - p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							start_end_routes[2][0]++;
							start_end_routes[2][1]++;



							//weight[1] = weight[0] - p.weight + fuel_consumed;

						}
						p.solution_from = start_end_routes[1][0] + 1;



						if (p.arrival_location != places[start_end_routes[2][0]]) {
							double time = time_dep[start_end_routes[1][0] + 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
							if (map_airstrip[p.arrival_location].fuel) {
								//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

								places.insert(places.begin() + start_end_routes[1][0] + 2, p.arrival_location);
								refueling.insert(refueling.begin() + start_end_routes[1][0] + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 2, map_airplane[aircraft_code].max_fuel);
								weight.insert(weight.begin() + start_end_routes[1][0] + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
								capacity.insert(capacity.begin() + start_end_routes[1][0] + 2, 0);
								time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 2, time);
								time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 2, time + map_airstrip[p.arrival_location].ground_time);
								start_end_routes[1][1] = start_end_routes[1][0] + 3;
								index++;
								number_add_locations++;
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;




								//weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
							}
							else {
								double fuel_before = quantity_fuel[start_end_routes[1][0] + 1];
								double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][0] + 1]][p.arrival_location];


								places.insert(places.begin() + start_end_routes[1][0] + 2, p.arrival_location);
								refueling.insert(refueling.begin() + start_end_routes[1][0] + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 2, fuel_before - fuel_consumed);
								weight.insert(weight.begin() + start_end_routes[1][0] + 2, weight[start_end_routes[1][0] + 1] + p.weight + fuel_consumed);
								capacity.insert(capacity.begin() + start_end_routes[1][0] + 2, 0);
								time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 2, time);
								time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 2, time + map_airstrip[p.arrival_location].ground_time);
								start_end_routes[1][1] = start_end_routes[1][0] + 3;
								index++;
								number_add_locations++;
								start_end_routes[2][0]++;
								start_end_routes[2][1]++;




							}
							p.solution_to = start_end_routes[1][1] - 1;
						}
						else {
							start_end_routes[1][1] = start_end_routes[1][0] + 2;
							p.solution_to = start_end_routes[1][1];
						}


						if (!map_airstrip[places[start_end_routes[1][1]]].fuel) {
							double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][1] - 1]][places[start_end_routes[1][1]]];
							double quantity_before = quantity_fuel[start_end_routes[1][1]];
							quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed_new;
							weight[start_end_routes[1][1]] -= (quantity_fuel[start_end_routes[1][1]] - quantity_before);
						}



					}


					double time_fly = (from_to[places[start_end_routes[1][1] - 1]][places[start_end_routes[1][1]]] / map_airplane[aircraft_code].speed) * 60;
					time_arr[start_end_routes[1][1]] = time_dep[start_end_routes[1][1] - 1] + time_fly;

				}
				else {

					start_end_routes[1][0] = start_end_routes[0][1];

					if (places[start_end_routes[1][0]] == p.departure_location) {
						time_arr[start_end_routes[1][0]] = p.early_departure;
						time_dep[start_end_routes[1][0]] = p.early_departure + map_airstrip[p.departure_location].ground_time;
						capacity[start_end_routes[1][0]] = 1;
						quantity_fuel[start_end_routes[1][0]] = map_airplane[aircraft_code].max_fuel;
						weight[start_end_routes[1][0]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][0]] - p.weight;


						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


						if (p.arrival_location != places[start_end_routes[1][0] + 1]) {
							//in questo caso il to diventa la nuova connessione
							double time_volo = (from_to[p.arrival_location][places[start_end_routes[1][0] + 1]] / map_airplane[aircraft_code].speed) * 60;

							places.insert(places.begin() + start_end_routes[1][0] + 1, p.arrival_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, 0.0);
							weight.insert(weight.begin() + start_end_routes[1][0] + 1, 0.0);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 0);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_dep[start_end_routes[1][0]] + time);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_arr[start_end_routes[1][0] + 2] - time_volo);
							start_end_routes[1][1] = start_end_routes[1][0] + 1;
							index++;
							number_add_locations++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;




							if (refueling[start_end_routes[1][0] + 1] == true) {
								quantity_fuel[start_end_routes[1][0] + 1] = map_airplane[aircraft_code].max_fuel;
								weight[start_end_routes[1][0] + 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][0] + 1];
							}
							else {
								quantity_fuel[start_end_routes[1][0] + 1] = quantity_fuel[start_end_routes[1][0]] - fuel_consumed;
								weight[start_end_routes[1][0] + 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[1][0] + 1];
							}

							p.solution_from = start_end_routes[1][0];
							p.solution_to = start_end_routes[1][0] + 1;


						}
						else {

							time_arr[start_end_routes[1][0] + 1] = time_dep[start_end_routes[1][0]] + time;
							start_end_routes[1][1] = start_end_routes[1][0] + 1;

							start_end_routes[2][0]++;


							p.solution_from = start_end_routes[1][0];
							p.solution_to = start_end_routes[1][0] + 1;


						}



					}
					else {
						start_end_routes[1][0] = start_end_routes[0][1];
						start_end_routes[2][0]++;

						//time_arr[start_end_routes[1][0]] = p.early_departure - (from_to[places[start_end_routes[1][0]] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60 - map_airstrip[places[start_end_routes[1][0]]].ground_time;
						double time_volo = (from_to[places[start_end_routes[1][0]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
						double time_new = p.early_departure - time_volo;
						if (time_new < start_day2 && places[start_end_routes[1][0]] != 1 && places[start_end_routes[1][0]] != 4) time_new = start_day2;


						time_dep[start_end_routes[1][0]] = time_new;

						//for the weight
						weight[start_end_routes[1][0]] = map_airplane[aircraft_code].weight_fuel_people -
							quantity_fuel[start_end_routes[1][0]];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
						//end for weight

						double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][0]]][p.departure_location];
						if (map_airstrip[p.departure_location].fuel) {
							//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

							places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;



						}
						else {
							double fuel_before = quantity_fuel[start_end_routes[1][0]];


							places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
							refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + start_end_routes[1][0] + 1, weight[start_end_routes[1][0]] - p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
							time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
							time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;





						}
						p.solution_from = start_end_routes[1][0] + 1;
						double time = time_new + time_volo + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



						if (p.arrival_location != places[start_end_routes[1][0] + 2]) {
							if (map_airstrip[p.arrival_location].fuel) {
								//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

								places.insert(places.begin() + start_end_routes[1][0] + 2, p.arrival_location);
								refueling.insert(refueling.begin() + start_end_routes[1][0] + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 2, map_airplane[aircraft_code].max_fuel);
								weight.insert(weight.begin() + start_end_routes[1][0] + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
								capacity.insert(capacity.begin() + start_end_routes[1][0] + 2, 0);
								time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 2, time);
								time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;


								start_end_routes[2][0]++;
								start_end_routes[2][1]++;


							}
							else {
								double fuel_before = quantity_fuel[start_end_routes[1][0] + 1];
								double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][0] + 1]][p.arrival_location];
								//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


								places.insert(places.begin() + start_end_routes[1][0] + 2, p.arrival_location);
								refueling.insert(refueling.begin() + start_end_routes[1][0] + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 2, fuel_before - fuel_consumed);
								weight.insert(weight.begin() + start_end_routes[1][0] + 2, weight[start_end_routes[1][0] + 1] + p.weight + fuel_consumed);
								capacity.insert(capacity.begin() + start_end_routes[1][0] + 2, 0);
								time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 2, time);
								time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;


								start_end_routes[2][0]++;
								start_end_routes[2][1]++;




							}

							double time_volo = (from_to[places[start_end_routes[1][0] + 2]][places[start_end_routes[1][0] + 3]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[start_end_routes[1][0] + 3] = time_dep[start_end_routes[1][0] + 2] + time_volo;
							start_end_routes[1][1] = start_end_routes[1][0] + 3;

						}
						else {
							double time_volo = (from_to[places[start_end_routes[1][0] + 1]][places[start_end_routes[1][0] + 2]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[start_end_routes[1][0] + 2] = time_dep[start_end_routes[1][0] + 1] + time_volo;
							start_end_routes[1][1] = start_end_routes[1][0] + 2;

						}
						p.solution_to = start_end_routes[1][0] + 2;

					}





					for (int i = start_end_routes[p.giorno - 1][1]; i < index; i++) {
						if (refueling[i] && i > start_end_routes[p.giorno - 1][1]) break;
						if (!refueling[i]) {
							double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
							double fb = quantity_fuel[i];
							quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
							weight[i] -= (quantity_fuel[i] - fb);
						}
					}


				}


			}




			//ATTENZIONE, DEVI AGGIORNARE IL FUEL DOPO
			int place_end = places[start_end_routes[p.giorno - 1][1]];
			double fuel_consumed_new = 0.0;
			if (!map_airstrip[place_end].fuel) {
				for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
					if (refueling[i]) break;
					fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_node_early = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
					weight[i] -= (quantity_fuel[i] - fuel_node_early);
				}
			}
			//******************************************************************************************************************************************************



			//ora devi rimettere fuel se c'è n'è bisogno o pescare fuel************************************************************************************************************

			//pezzo che pesca fuel;
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = p.solution_from; j < index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						quantity_fuel[t] += add_fuel;
						weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}


			//pezzo che ridà fuel
			for (int k = 0; k < index; k++) {
				if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
					//cout << " Sto valutando il caso del nodo " << k << endl;
					int index_updating_from = k;
					int index_updating_to = index;  //qua prima c'era -1
					int Node_min = k;
					double min_weight = weight[k];
					for (int i = k + 1; i < index; i++) {
						//cout << " Sto guardando il nodo " << i << endl;
						//cout << " Ho passato IF di controllo " << endl;
						if (refueling[i]) {
							index_updating_to = i;
							break;
						}
						if (weight[i] < min_weight) {
							min_weight = weight[i];
							Node_min = i;
						}

					}
					double Fuel_before = quantity_fuel[index_updating_from];
					quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
					weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}

				}
			}



		}
		else if (start_end_routes[0][1] == -1 && start_end_routes[2][1] != -1) {
			//ho nodi nel giorno 3 ma non nel giorno 1
			add_number_in_passenger = true;

			if (capacity[0] > 0) {
				places.insert(places.begin() + 0, places[0]);
				refueling.insert(refueling.begin() + 0, refueling[0]);
				quantity_fuel.insert(quantity_fuel.begin() + 0, map_airplane[aircraft_code].max_fuel);
				weight.insert(weight.begin() + 0, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
				capacity.insert(capacity.begin() + 0, 0);
				time_arr.insert(time_arr.begin() + 0, 0.0);
				time_dep.insert(time_dep.begin() + 0, 0.0);
				start_end_routes[1][0] = 0;
				index++;
				number_add_locations++;


				start_end_routes[2][0]++;
				start_end_routes[2][1]++;



				if (places[0] == p.departure_location) {
					time_arr[0] = p.early_departure;
					time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
					capacity[0] = 1;
					quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
					weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


					//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);

					places.insert(places.begin() + 1, p.arrival_location);
					refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
					quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
					weight.insert(weight.begin() + 1, 0.0);
					capacity.insert(capacity.begin() + 1, 0);
					time_arr.insert(time_arr.begin() + 1, time_dep[0] + time);
					time_dep.insert(time_dep.begin() + 1, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
					start_end_routes[1][1] = 2; //perchè c'è il nodo di collegamento
					index++;
					number_add_locations++;

					start_end_routes[2][0]++;
					start_end_routes[2][1]++;



					if (refueling[1] == true) {
						quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
						weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
					}
					else {
						quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
						weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
					}

					p.solution_from = 0;
					p.solution_to = 1;

					double time_volo = (from_to[p.arrival_location][places[2]] / map_airplane[aircraft_code].speed) * 60;
					time_arr[2] = time_dep[1] + time_volo;


				}
				else {

					time_arr[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
						map_airstrip[places[0]].ground_time;
					time_dep[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

					//for the weight
					weight[0] = map_airplane[aircraft_code].weight_fuel_people -
						quantity_fuel[0];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
					//end for weight

					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[0]][p.departure_location];
					if (map_airstrip[p.departure_location].fuel) {
						//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

						places.insert(places.begin() + 1, p.departure_location);
						refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + 1, map_airplane[aircraft_code].max_fuel);
						weight.insert(weight.begin() + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
						capacity.insert(capacity.begin() + 1, 1);
						time_arr.insert(time_arr.begin() + 1, p.early_departure);
						time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
						number_add_locations++;
						index++;


						start_end_routes[2][0]++;
						start_end_routes[2][1]++;




					}
					else {
						double fuel_before = quantity_fuel[0];


						places.insert(places.begin() + 1, p.departure_location);
						refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + 1, fuel_before - fuel_consumed);
						weight.insert(weight.begin() + 1, weight[0] - p.weight + fuel_consumed);
						capacity.insert(capacity.begin() + 1, 1);
						time_arr.insert(time_arr.begin() + 1, p.early_departure);
						time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
						number_add_locations++;
						index++;

						start_end_routes[2][0]++;
						start_end_routes[2][1]++;


					}
					p.solution_from = 1;
					double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



					if (p.arrival_location != places[2]) {
						if (map_airstrip[p.arrival_location].fuel) {
							//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

							places.insert(places.begin() + 2, p.arrival_location);
							refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 2, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
							capacity.insert(capacity.begin() + 2, 0);
							time_arr.insert(time_arr.begin() + 2, time);
							time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;



						}
						else {
							double fuel_before = quantity_fuel[1];
							double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[1]][p.arrival_location];
							//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


							places.insert(places.begin() + 2, p.arrival_location);
							refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 2, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + 2, weight[1] + p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + 2, 0);
							time_arr.insert(time_arr.begin() + 2, time);
							time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;




						}

						double time_volo = (from_to[places[2]][places[3]] / map_airplane[aircraft_code].speed) * 60;
						time_arr[3] = time_dep[2] + time_volo;
						start_end_routes[1][1] = 3;

					}
					else {
						double time_volo = (from_to[places[1]][places[2]] / map_airplane[aircraft_code].speed) * 60;
						time_arr[2] = time_dep[1] + time_volo;
						start_end_routes[1][1] = 2;

					}
					p.solution_to = 2;

				}

			}
			else {
				//in questo caso la capacità non è zero quindi non aggiungo il pezzo a vuoto
				//forse qua dovrò salvarmi qualche valore della partenza
				double fly_check = (from_to[p.arrival_location][places[1]] / map_airplane[aircraft_code].speed) * 60;
				if (time_arr[1] - fly_check < start_day3) {
					places.insert(places.begin() + 0, places[0]);
					refueling.insert(refueling.begin() + 0, refueling[0]);
					quantity_fuel.insert(quantity_fuel.begin() + 0, map_airplane[aircraft_code].max_fuel);
					weight.insert(weight.begin() + 0, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
					capacity.insert(capacity.begin() + 0, 0);
					time_arr.insert(time_arr.begin() + 0, 0.0);
					time_dep.insert(time_dep.begin() + 0, 0.0);
					start_end_routes[1][0] = 0;
					index++;
					number_add_locations++;


					start_end_routes[2][0]++;
					start_end_routes[2][1]++;



					if (places[0] == p.departure_location) {
						time_arr[0] = p.early_departure;
						time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
						capacity[0] = 1;
						quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
						weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;


						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


						//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[0] + time, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);

						places.insert(places.begin() + 1, p.arrival_location);
						refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
						quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
						weight.insert(weight.begin() + 1, 0.0);
						capacity.insert(capacity.begin() + 1, 0);
						time_arr.insert(time_arr.begin() + 1, time_dep[0] + time);
						time_dep.insert(time_dep.begin() + 1, time_dep[0] + time + map_airstrip[p.arrival_location].ground_time);
						start_end_routes[1][1] = 2; //perchè c'è il nodo di collegamento
						index++;
						number_add_locations++;

						start_end_routes[2][0]++;
						start_end_routes[2][1]++;



						if (refueling[1] == true) {
							quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
							weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
						}
						else {
							quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
							weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
						}

						p.solution_from = 0;
						p.solution_to = 1;

						double time_volo = (from_to[p.arrival_location][places[2]] / map_airplane[aircraft_code].speed) * 60;
						time_arr[2] = time_dep[1] + time_volo;


					}
					else {

						time_arr[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
							map_airstrip[places[0]].ground_time;
						time_dep[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

						//for the weight
						weight[0] = map_airplane[aircraft_code].weight_fuel_people -
							quantity_fuel[0];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
						//end for weight

						double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[0]][p.departure_location];
						if (map_airstrip[p.departure_location].fuel) {
							//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;




						}
						else {
							double fuel_before = quantity_fuel[0];


							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + 1, weight[0] - p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;

							start_end_routes[2][0]++;
							start_end_routes[2][1]++;


						}
						p.solution_from = 1;
						double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



						if (p.arrival_location != places[2]) {
							if (map_airstrip[p.arrival_location].fuel) {
								//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, map_airplane[aircraft_code].max_fuel);
								weight.insert(weight.begin() + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;


								start_end_routes[2][0]++;
								start_end_routes[2][1]++;



							}
							else {
								double fuel_before = quantity_fuel[1];
								double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[1]][p.arrival_location];
								//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, fuel_before - fuel_consumed);
								weight.insert(weight.begin() + 2, weight[1] + p.weight + fuel_consumed);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;


								start_end_routes[2][0]++;
								start_end_routes[2][1]++;




							}

							double time_volo = (from_to[places[2]][places[3]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[3] = time_dep[2] + time_volo;
							start_end_routes[1][1] = 3;

						}
						else {
							double time_volo = (from_to[places[1]][places[2]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[2] = time_dep[1] + time_volo;
							start_end_routes[1][1] = 2;

						}
						p.solution_to = 2;

					}

				}
				else {

					if (places[0] == p.departure_location) {
						time_arr[0] = p.early_departure;
						time_dep[0] = p.early_departure + map_airstrip[p.departure_location].ground_time;
						capacity[0] = 1;
						quantity_fuel[0] = map_airplane[aircraft_code].max_fuel;
						weight[0] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[0] - p.weight;
						start_end_routes[1][0] = 0;

						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
						double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];


						if (p.arrival_location != places[1]) {
							//in questo caso il to diventa la nuova connessione
							double time_volo = (from_to[p.arrival_location][places[1]] / map_airplane[aircraft_code].speed) * 60;

							places.insert(places.begin() + 1, p.arrival_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
							weight.insert(weight.begin() + 1, 0.0);
							capacity.insert(capacity.begin() + 1, 0);
							time_arr.insert(time_arr.begin() + 1, time_dep[0] + time);
							time_dep.insert(time_dep.begin() + 1, time_arr[2] - time_volo);
							start_end_routes[1][1] = 1;
							index++;
							number_add_locations++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;




							if (refueling[1] == true) {
								quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
								weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
							}
							else {
								quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
								weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
							}

							p.solution_from = 0;
							p.solution_to = 1;


						}
						else {


							time_arr[1] = time_dep[0] + time;
							//time_dep[1] = time_dep[0] + time + map_airstrip[p.arrival_location].ground_time;
							start_end_routes[1][1] = 1;
							start_end_routes[2][0]++;





							/*if (refueling[1] == true) {
								quantity_fuel[1] = map_airplane[aircraft_code].max_fuel;
								weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
							}
							else {
								quantity_fuel[1] = quantity_fuel[0] - fuel_consumed;
								weight[1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[1];
							}*/

							p.solution_from = 0;
							p.solution_to = 1;

							//double time_volo = (from_to[p.arrival_location + ";" + places[2]] / map_airplane[aircraft_code].speed) * 60;
							//time_arr[2] = time_dep[1] + time_volo;

						}



					}
					else {
						start_end_routes[1][0] = 0;
						start_end_routes[2][0]++;

						time_arr[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 -
							map_airstrip[places[0]].ground_time;
						time_dep[0] = p.early_departure - (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;

						//for the weight
						weight[0] = map_airplane[aircraft_code].weight_fuel_people -
							quantity_fuel[0];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
						//end for weight

						double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[0]][p.departure_location];
						if (map_airstrip[p.departure_location].fuel) {
							//addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, map_airplane[aircraft_code].max_fuel);
							weight.insert(weight.begin() + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;



						}
						else {
							double fuel_before = quantity_fuel[0];


							places.insert(places.begin() + 1, p.departure_location);
							refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
							quantity_fuel.insert(quantity_fuel.begin() + 1, fuel_before - fuel_consumed);
							weight.insert(weight.begin() + 1, weight[0] - p.weight + fuel_consumed);
							capacity.insert(capacity.begin() + 1, 1);
							time_arr.insert(time_arr.begin() + 1, p.early_departure);
							time_dep.insert(time_dep.begin() + 1, p.early_departure + map_airstrip[p.departure_location].ground_time);
							number_add_locations++;
							index++;


							start_end_routes[2][0]++;
							start_end_routes[2][1]++;





						}
						p.solution_from = 1;
						double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;



						if (p.arrival_location != places[2]) {
							if (map_airstrip[p.arrival_location].fuel) {
								//addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, map_airplane[aircraft_code].max_fuel);
								weight.insert(weight.begin() + 2, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;


								start_end_routes[2][0]++;
								start_end_routes[2][1]++;


							}
							else {
								double fuel_before = quantity_fuel[1];
								double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[1]][p.arrival_location];
								//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


								places.insert(places.begin() + 2, p.arrival_location);
								refueling.insert(refueling.begin() + 2, map_airstrip[p.arrival_location].fuel);
								quantity_fuel.insert(quantity_fuel.begin() + 2, fuel_before - fuel_consumed);
								weight.insert(weight.begin() + 2, weight[1] + p.weight + fuel_consumed);
								capacity.insert(capacity.begin() + 2, 0);
								time_arr.insert(time_arr.begin() + 2, time);
								time_dep.insert(time_dep.begin() + 2, time + map_airstrip[p.arrival_location].ground_time);
								number_add_locations++;
								index++;


								start_end_routes[2][0]++;
								start_end_routes[2][1]++;




							}

							double time_volo = (from_to[places[2]][places[3]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[3] = time_dep[2] + time_volo;
							start_end_routes[1][1] = 3;

						}
						else {
							double time_volo = (from_to[places[1]][places[2]] / map_airplane[aircraft_code].speed) * 60;
							time_arr[2] = time_dep[1] + time_volo;
							start_end_routes[1][1] = 2;

						}
						p.solution_to = 2;

					}


					for (int i = start_end_routes[p.giorno - 1][1]; i < index; i++) {
						if (refueling[i] && i > start_end_routes[p.giorno - 1][1]) break;
						if (!refueling[i]) {
							double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
							double fb = quantity_fuel[i];
							quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
							weight[i] -= (quantity_fuel[i] - fb);
						}
					}


				}


			}

			//pezzo che pesca fuel;
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = 0; j < index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						quantity_fuel[t] += add_fuel;
						weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}


			//pezzo che ridà fuel
			for (int k = start_end_routes[p.giorno - 1][0]; k < index; k++) {
				if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
					//cout << " Sto valutando il caso del nodo " << k << endl;
					int index_updating_from = k;
					int index_updating_to = index;  //qua prima c'era -1
					int Node_min = k;
					double min_weight = weight[k];
					for (int i = k + 1; i < index; i++) {
						//cout << " Sto guardando il nodo " << i << endl;
						//cout << " Ho passato IF di controllo " << endl;
						if (refueling[i]) {
							index_updating_to = i;
							break;
						}
						if (weight[i] < min_weight) {
							min_weight = weight[i];
							Node_min = i;
						}

					}
					double Fuel_before = quantity_fuel[index_updating_from];
					quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
					weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}

				}
			}






		}
		else if (start_end_routes[0][1] != -1 && start_end_routes[2][1] == -1) {
			//ho nodi nel giorno 1 ma non nel giorno 3

			start_end_routes[1][0] = index - 1;
			//addPlace(places[index - 1], refueling[index - 1], quantity_fuel[index - 1], weight[index - 1], 0, 0.0, 0.0);


			if (places[index - 1] == p.departure_location) {
				//time_arr[index - 1] = p.early_departure;
				time_dep[index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
				capacity[index - 1] = 1;
				//quantity_fuel[index - 1] = map_airplane[aircraft_code].max_fuel;
				weight[index - 1] -= p.weight;


				double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.departure_location][p.arrival_location];
				/*if (time <= 60) {
					fuel_consumed += (time / 60)*map_airplane[aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
				}*/

				addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, 0.0, 0.0, 0, time_dep[index - 1] + time, time_dep[index - 1] + time + map_airstrip[p.arrival_location].ground_time);
				if (refueling[index - 1] == true) {
					quantity_fuel[index - 1] = map_airplane[aircraft_code].max_fuel;
					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1];
				}
				else {
					quantity_fuel[index - 1] = quantity_fuel[index - 2] - fuel_consumed;
					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1];
				}

				p.solution_from = index - 2;
				p.solution_to = index - 1;


			}
			else {

				//time_arr[index - 1] = p.early_departure - (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60 - map_airstrip[places[index - 1]].ground_time;
				double time_volo = (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				double time_new = p.early_departure - time_volo;
				if (time_new < start_day2 && places[index - 1] != 1 && places[index - 1] != 4) time_new = start_day2;


				time_dep[index - 1] = time_new;


				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
				if (map_airstrip[p.departure_location].fuel) {
					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel - p.weight;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];





					addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
					weight[index - 1] = weight[index - 2] - p.weight + fuel_consumed;

				}
				p.solution_from = index - 1;
				double time = time_new + time_volo + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;




				if (map_airstrip[p.arrival_location].fuel) {
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

					weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
						map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_before = quantity_fuel[index - 1];
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
					//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
					}
					*/
					addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
					weight[index - 1] = weight[index - 2] + p.weight + fuel_consumed;

				}
				p.solution_to = index - 1;

			}

			start_end_routes[1][1] = index - 1;





		}
		else if (start_end_routes[0][1] != -1 && start_end_routes[2][1] != -1 && start_end_routes[1][0] != -1) {
			add_number_in_passenger = true;


			//prendo qualche dato che avevo in A e B che sono i due nodi che rimangono senza passeggeri nella route del giorno due
			int location_A = places[start_end_routes[0][1]];
			double quantity_fuel_A = quantity_fuel[start_end_routes[0][1]];
			double weight_A = weight[start_end_routes[0][1]];
			int location_B = places[start_end_routes[2][0]];
			double quantity_fuel_B = quantity_fuel[start_end_routes[2][0]];
			double weight_B = weight[start_end_routes[2][0]];

			//aggiungo la location di partenza del primo passeggero
			if (p.departure_location == location_A) {
				capacity[start_end_routes[0][1]] = 1;
				weight[start_end_routes[0][1]] -= p.weight;
				time_dep[start_end_routes[0][1]] = p.early_departure + map_airstrip[location_A].ground_time;
				p.solution_from = start_end_routes[0][1];
			}
			else {

				double time_volo = (from_to[location_A][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				double time_new = p.early_departure - time_volo;
				if (time_new < start_day2 && location_A != 1 && location_A != 4) time_new = start_day2;


				if (map_airstrip[p.departure_location].fuel) {
					places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
					refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, true);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].max_fuel);
					weight.insert(weight.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight);
					capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
					time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
					time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
					number_add_locations++;
					index++;

					//ora aggiorno l'orario precedente
					time_dep[start_end_routes[1][0]] = time_new;

				}
				else {
					//metti codice se non c'è il fuel
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_A][p.departure_location];

					places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
					refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, false);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, quantity_fuel_A - fuel_consumed);
					weight.insert(weight.begin() + start_end_routes[1][0] + 1, map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel_A - fuel_consumed) - p.weight);
					capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
					time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_new + time_volo);
					time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
					number_add_locations++;
					index++;

					//ora aggiorno l'orario precedente
					time_dep[start_end_routes[1][0]] = time_new;

				}

				p.solution_from = start_end_routes[1][0] + 1;
				//aggiorna gli indici delle barriere di tre e quella di fine di due

				start_end_routes[1][1]++;
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;



			}

			//aggiungo la location di arrivo del primo passeggero
			if (p.arrival_location == location_B) {
				//devo aggiornare il fuel e il peso ed il tempo di arrivo
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][1] - 1]][location_B];
				double time_volo = (from_to[places[start_end_routes[1][1] - 1]][location_B] / map_airplane[aircraft_code].speed) * 60;
				//per aggiornare il tempo di arrivo e facile, per aggiornare il fuel devi valutare se è una airstrip che fa fuel o no
				time_arr[start_end_routes[1][1]] = time_dep[start_end_routes[1][1] - 1] + time_volo;

				if (!map_airstrip[location_B].fuel) {
					quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed;
					weight[start_end_routes[1][1]] = weight_B - (quantity_fuel[start_end_routes[1][1]] - quantity_fuel_B);
				}

				p.solution_to = start_end_routes[1][1];

			}
			else {
				//caso in cui non è uguale e devo aggiungerlo prima della fine ed aggiornare tutto
				double time_volo = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				double time_dep_before = time_dep[start_end_routes[1][1] - 1];

				if (map_airstrip[p.arrival_location].fuel) {
					places.insert(places.begin() + start_end_routes[1][1], p.arrival_location);
					refueling.insert(refueling.begin() + start_end_routes[1][1], true);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][1], map_airplane[aircraft_code].max_fuel);
					weight.insert(weight.begin() + start_end_routes[1][1], map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel);
					capacity.insert(capacity.begin() + start_end_routes[1][1], 0);
					time_arr.insert(time_arr.begin() + start_end_routes[1][1], time_dep_before + time_volo);
					time_dep.insert(time_dep.begin() + start_end_routes[1][1], time_dep_before + time_volo + map_airstrip[p.arrival_location].ground_time);
					number_add_locations++;
					index++;



				}
				else {
					//metti codice se non c'è il fuel
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[1][1] - 1]][p.arrival_location];

					places.insert(places.begin() + start_end_routes[1][1], p.arrival_location);
					refueling.insert(refueling.begin() + start_end_routes[1][1], false);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][1], quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed);
					weight.insert(weight.begin() + start_end_routes[1][1], map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[start_end_routes[1][1]]));
					capacity.insert(capacity.begin() + start_end_routes[1][1], 0);
					time_arr.insert(time_arr.begin() + start_end_routes[1][1], time_dep_before + time_volo);
					time_dep.insert(time_dep.begin() + start_end_routes[1][1], time_dep_before + time_volo + map_airstrip[p.arrival_location].ground_time);
					number_add_locations++;
					index++;


				}

				p.solution_to = start_end_routes[1][1];
				//aggiorna gli indici delle barriere di tre e quella di fine di due

				start_end_routes[1][1]++;
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;

				double time_pezzo_after = (from_to[p.arrival_location][location_B] / map_airplane[aircraft_code].speed) * 60;
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][p.arrival_location][location_B];
				time_arr[start_end_routes[1][1]] = time_dep[start_end_routes[1][1] - 1] + time_pezzo_after;

				if (!map_airstrip[location_B].fuel) {
					quantity_fuel[start_end_routes[1][1]] = quantity_fuel[start_end_routes[1][1] - 1] - fuel_consumed;
					weight[start_end_routes[1][1]] = weight_B - (quantity_fuel[start_end_routes[1][1]] - quantity_fuel_B);
				}

			}




			//ATTENZIONE, DEVI AGGIORNARE IL FUEL DOPO
			//metto a posto il fuel e il weight nella route dopo dato che ho fatto la mossa C***********************************************************************
			int place_end = places[start_end_routes[p.giorno - 1][1]];
			double fuel_consumed_new = 0.0;
			if (!map_airstrip[place_end].fuel) {
				for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
					if (refueling[i]) break;
					fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_node_early = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
					weight[i] -= (quantity_fuel[i] - fuel_node_early);
				}
			}
			//******************************************************************************************************************************************************



			//ora devi rimettere fuel se c'è n'è bisogno o pescare fuel************************************************************************************************************

			//pezzo che pesca fuel;
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = p.solution_from; j < index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						quantity_fuel[t] += add_fuel;
						weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}


			//pezzo che ridà fuel
			for (int k = start_end_routes[p.giorno - 1][0]; k < start_end_routes[p.giorno - 1][1]; k++) {
				if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
					//cout << " Sto valutando il caso del nodo " << k << endl;
					int index_updating_from = k;
					int index_updating_to = index;  //qua prima c'era -1
					int Node_min = k;
					double min_weight = weight[k];
					for (int i = k + 1; i < index; i++) {
						//cout << " Sto guardando il nodo " << i << endl;
						//cout << " Ho passato IF di controllo " << endl;
						if (refueling[i]) {
							index_updating_to = i;
							break;
						}
						if (weight[i] < min_weight) {
							min_weight = weight[i];
							Node_min = i;
						}

					}
					double Fuel_before = quantity_fuel[index_updating_from];
					quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
					weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
						weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
					}

				}
			}



		}




	}


	if (add_number_in_passenger) {
		for (Passenger& pas : passengers_in_route) {
			if (pas.giorno > p.giorno) {
				pas.solution_from += number_add_locations;
				pas.solution_to += number_add_locations;
			}
		}
	}






}

//for updating in order to improve the computational time
void Route::update_route_rebuilt_one_first_fase_without_after(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == start_end_routes[p.giorno - 1][1]) non_to_final = true;



	if (places[node_add_from] == location_from) {
		caso = 1; //impongo che questo è il caso 1
		//il nodo prima al from è uguale devo quindi aggiornare le capacità e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}



	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		caso = 2;
		int node_support_to = node_add_to;


		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {


			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= start_end_routes[p.giorno - 1][1] + 1) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= start_end_routes[p.giorno - 1][1] + 1) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= start_end_routes[p.giorno - 1][1] + 1) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}


	}
	else {
		caso = 3;
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
			//node_add_to = node_add_to + 1;
			//non_to = true;
			//}

		}

		double time_add = time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_from + 1, time_add);
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		start_end_routes[p.giorno - 1][1]++;
		//index_cap++;

		for (int i = node_add_from + 2; i <= start_end_routes[p.giorno - 1][1]; i++) {
			time_arr[i] += time;
			time_dep[i] += time;
		}

		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_from + 1, 0.0);
		weight.insert(weight.begin() + node_add_from + 1, 0.0);
		capacity.insert(capacity.begin() + node_add_from + 1, 0);



	}


	if (non_to) {
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		double time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_to, time_add1);
		time_dep.insert(time_dep.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);

		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		start_end_routes[p.giorno - 1][1]++;
		//index_cap++;


		for (int i = node_add_to + 1; i <= start_end_routes[p.giorno - 1][1]; i++) {
			time_arr[i] += time1;
			time_dep[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_to, 0.0);
		weight.insert(weight.begin() + node_add_to, 0.0);
		capacity.insert(capacity.begin() + node_add_to, 0);




	}



}

void Route::update_route_rebuilt_one_second_fase_without_after(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	//in questo commento qua sotto ci sono i vari casi con l'if al posto dello switch

	if (caso == -1) {
		cout << "problema in route.cpp con la variabile caso" << endl;
		cin.get();
	}
	else if (caso == 1) {

		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j <= start_end_routes[p.giorno - 1][1]; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 467 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t <= index - 1; t++) { //start_end_routes[p.giorno - 1][1]
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
							//ccout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
			}

		}





	}
	else if (caso == 2) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j <= start_end_routes[p.giorno - 1][1]; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 581 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t <= index - 1; t++) {  //start_end_routes[p.giorno - 1][1]
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}

						//ccout << " Weight dopo " << weight[t] << endl;
					}
				}
			}

		}

	}
	else if (caso == 3) {

		p.solution_from = node_add_from + 1;

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}

		//update of fuel, capacity and weight
		capacity[node_add_from + 1] = capacity[node_add_from];
		weight[node_add_from + 1] = weight[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		//fuel consumed
		int A = 0;
		int B = 0;
		if (node_add_to == node_add_from + 1 && non_to) {
			A = node_add_from + 2;
			B = node_add_from + 3;
		}
		else if (node_add_to == node_add_from + 2 && non_to) {
			A = node_add_from + 1;
			B = node_add_from + 3;
		}
		else {
			A = node_add_from + 1;
			B = node_add_from + 2;
		}



		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[A]];
		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[A]) {
			quantity_fuel[A] = map_airplane[aircraft_code].max_fuel;
			weight[A] = weight[A] - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[A]) {
			quantity_fuel[A] = quantity_fuel[node_add_from] - fuel_consumed;
			weight[A] = weight[A] + fuel_consumed;
		}

		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[A]][places[B]];
		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[B]];
		if (!refueling[A]) {
			for (int i = B; i <= index - 1; i++) { //start_end_routes[p.giorno - 1][1]
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
						weight[i] = weight[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
					}
					else {
						break;
					}
				}

			}
		}
		else {
			//caso bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = B; i < index; i++) {  //start_end_routes[p.giorno - 1][1]
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
						weight[i] = weight[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
					}
					else {
						break;
					}
				}
			}
		}


		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = A; j <= start_end_routes[p.giorno - 1][1]; j++) { //forse deve essere INDEX
			if (non_to && j == node_add_to) {
				//non devo fare niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 789 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (non_to && i == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t <= index - 1; t++) { //start_end_routes[p.giorno - 1][1]
						if (non_to && t == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}
						//ccout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
		}


	}


	if (non_to) {

		p.solution_to = node_add_to;
		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from >= node_add_to) p.solution_from++;
			if (p.solution_to >= node_add_to) p.solution_to++;
		}



		double weight_add = 0.0;
		/////////////////////////////////////////
		//fuel consumed


		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];


		if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}




		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacity[node_add_to] = capacity[node_add_to - 1] - 1;
			if (refueling[node_add_to]) {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
			}
			else {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}
		//********************************************************************************************************************************************************************************


		/*if (refueling[node_add_to] && weight[node_add_to] < 0) {
			quantity_fuel[node_add_to] += weight[node_add_to];
			for (int k = node_add_to + 1; k < index; k++) {
				if (refueling[k]) break;
				quantity_fuel[k] += weight[node_add_to];
				weight[k] -= weight[node_add_to];
			}
			weight[node_add_to] = 0;
		}
		else if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}*/



		double fuel_consumed4 = 0.0;
		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
		}

		double fuel_consumed5 = 0.0;
		if (!non_to_final) {
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) { //start_end_routes[p.giorno - 1][1]
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weight[i] = weight[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) { //start_end_routes[p.giorno - 1][1]
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
					weight[i] = weight[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}


		if (non_to_final) {
			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[node_add_to - 1] - from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]]);
			}
			weight[node_add_to] += weight_add;
		}


		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO


		//cout << "sono davanti all'aggiornamento del caso in cui ho inserito il to" << endl;
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_to; j <= start_end_routes[p.giorno - 1][1]; j++) {
			//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
			if (weight[j] < 0) {
				//cout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t <= index - 1; t++) { //start_end_routes[p.giorno - 1][1]
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//cout << " Weight dopo " << weight[t] << endl;
				}
			}
		}



	}
	else {
		p.solution_to = node_add_to;
	}


	//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
	//cout << " Finisco in ------> " << max_to_pass << endl;
	//cout << "caso prima dell'aggiornamento in fondo del repair" << endl;
	//print();
	for (int k = 0; k <= start_end_routes[p.giorno - 1][1]; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i <= start_end_routes[p.giorno - 1][1]; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}


}

void Route::move_c_without_after(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {


	if (places[index - 1] == location_from) {

		capacity[index - 1]++;
		weight[index - 1] -= p.weight;

		double time_arri = time_dep[index - 1] + (from_to[location_from][location_to] / map_airplane[aircraft_code].speed) * 60;
		double time_depa = time_arri + map_airstrip[location_to].ground_time;
		p.solution_from = index - 1;

		//double time_fly = (from_to[location_from + ";" + location_to] / map_airplane[aircraft_code].speed) * 60;


		double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_from][location_to];
		/*if (time_fly <= 60) {
			fuel_consumed += (time_fly / 60)*map_airplane[aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
		}*/

		if (map_airstrip[location_to].fuel) {
			addPlace(location_to, map_airstrip[location_to].fuel, map_airplane[aircraft_code].max_fuel, (map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel), 0, time_arri, time_depa);
		}
		else {
			addPlace(location_to, map_airstrip[location_to].fuel, quantity_fuel[index - 1] - fuel_consumed, (map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[index - 1] - fuel_consumed)), 0, time_arri, time_depa);
		}
		start_end_routes[p.giorno - 1][1]++;
		p.solution_to = index - 1;


	}
	else {

		if (map_airstrip[p.departure_location].fuel) {
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 1,
				time_dep[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				time_dep[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people -
				map_airplane[aircraft_code].max_fuel - p.weight;


		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.departure_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;


			
			addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, time_dep[index - 1] +
				(from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60,
				time_dep[index - 1] + (from_to[places[index - 1]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);

			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[index - 1] - p.weight;

		}
		start_end_routes[p.giorno - 1][1]++;
		p.solution_from = index - 1;
		double time = time_dep[index - 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;





		if (map_airstrip[p.arrival_location].fuel) {
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;

		}
		else {
			double fuel_before = quantity_fuel[index - 1];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[index - 1]][p.arrival_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


			/*if (travel_time <= 60) {
				fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
			}*/
			addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
			weight[index - 1] = weight[index - 2] + fuel_consumed + p.weight;

		}
		start_end_routes[p.giorno - 1][1]++;

		p.solution_to = index - 1;





	}







}

void Route::update_route_rebuilt_one_first_fase_with_after(bool& messa_connessione, double start_day, double&  fuel_connection, double& weight_connection, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	if (node_add_from == node_add_to) num_equals = true;
	if (node_add_to == start_end_routes[p.giorno - 1][1]) non_to_final = true;


	if (places[node_add_from] == location_from) {
		caso = 1; //impongo che questo è il caso 1
		//il nodo prima al from è uguale devo quindi aggiornare le capacità e basta, non i tempi
		//li aggiorno fino a node_add_to
		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}
			if (num_equals && location_to != places[node_support_to + 1]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}



	}
	else if (places[node_add_from + 1] == location_from) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		caso = 2;
		int node_support_to = node_add_to;


		if (non_to_final) {

			if (location_to != places[node_support_to]) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {


			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
				if (node_add_to >= (start_end_routes[p.giorno - 1][1] + 1)) non_to_final = true; // ho cambiato qua forse la condizione dopo la puoi commentare
				if (num_equals && node_add_to >= (start_end_routes[p.giorno - 1][1] + 1)) non_to_final = true;
			}
			if (num_equals) { //&& location_to == places[node_support_to] // ho cambiato qua
				if (node_add_to + 2 >= (start_end_routes[p.giorno - 1][1] + 1)) {
					non_to_final = true;
					node_add_to = node_add_to + 2;
					non_to = true;
				}
				else {
					if (location_to == places[node_support_to + 2]) {
						node_add_to = node_add_to + 2;
					}
					else {
						node_add_to = node_add_to + 2;
						non_to = true;
					}
				}
			}
		}


	}
	else {
		caso = 3;
		places.insert(places.begin() + node_add_from + 1, location_from);
		node_add_to++;

		int node_support_to = node_add_to;

		if (non_to_final) {

			if (location_to != places[node_support_to]) { // ho cambiato qua
				node_add_to = node_add_to + 1;
				non_to = true;
			}


		}
		else {

			if (places[node_support_to + 1] == location_to) {
				node_add_to = node_add_to + 1;
			}
			if (location_to != places[node_support_to + 1] && location_to != places[node_support_to] && !num_equals) {
				node_add_to = node_add_to + 1;
				non_to = true;
			}

			//cout << "num_equals: " << num_equals << "node support_to: "<< node_support_to << "places.size(): "<< places.size() << endl;
			if (num_equals && location_to != places[node_support_to + 1]) { // ho aggiunto questa
				node_add_to = node_add_to + 1;
				non_to = true;
				//cout << "ci sono entrato" << endl;
				//cout << "places.size()=" << places.size() << endl;
			}



			//if (num_equals && location_to == places[node_support_to]) {
			//node_add_to = node_add_to + 1;
			//non_to = true;
			//}

		}

		double time_add = time_dep[node_add_from] + ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60);
		time_arr.insert(time_arr.begin() + node_add_from + 1, time_add);
		time_dep.insert(time_dep.begin() + node_add_from + 1, time_add + map_airstrip[location_from].ground_time);

		double time = ((from_to[places[node_add_from]][location_from] / map_airplane[aircraft_code].speed) * 60)
			+ ((from_to[location_from][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60)
			+ map_airstrip[location_from].ground_time
			- ((from_to[places[node_add_from]][places[node_add_from + 2]] / map_airplane[aircraft_code].speed) * 60);

		index++;
		start_end_routes[p.giorno - 1][1]++;
		if (p.giorno == 1) {
			if (start_end_routes[1][1] != -1) {
				start_end_routes[1][0]++;
				start_end_routes[1][1]++;
			}
			if (start_end_routes[2][1] != -1) {
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;
			}
		}
		if (p.giorno == 2) {
			start_end_routes[2][0]++;
			start_end_routes[2][1]++;
		}




		//index_cap++;

		for (int i = node_add_from + 2; i <= start_end_routes[p.giorno - 1][1]; i++) {
			time_arr[i] += time;
			if(i != start_end_routes[p.giorno - 1][1]) time_dep[i] += time;
		}

		refueling.insert(refueling.begin() + node_add_from + 1, map_airstrip[location_from].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_from + 1, 0.0);
		weight.insert(weight.begin() + node_add_from + 1, 0.0);
		capacity.insert(capacity.begin() + node_add_from + 1, 0);


		
	}


	if (non_to) {
		
		
		//to
		//cout << "pippo3" << endl;

		places.insert(places.begin() + node_add_to, location_to);
		double time_add1 = 0.0;
		
		
			
		//mi salvo le informazioni del giorno di connession, prima queste erano fuori
		int location_connection = places[node_add_to - 1];
		double time_departure_day_connection = time_dep[node_add_to - 1];
		int capacity_connection = capacity[node_add_to - 1];
		fuel_connection = quantity_fuel[node_add_to - 1];
		weight_connection = weight[node_add_to - 1];
		int per_pass = start_end_routes[p.giorno - 1][1];

		if (non_to_final) {
		
			time_dep[node_add_to - 1] = time_arr[node_add_to - 1] + map_airstrip[places[node_add_to - 1]].ground_time;
			capacity[node_add_to - 1] = 0;
			weight[node_add_to - 1] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[node_add_to - 1]; //non convintissimo che vada qua
			time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		}
		else {
			time_add1 = time_dep[node_add_to - 1] + ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60);
		}
		
		
		time_arr.insert(time_arr.begin() + node_add_to, time_add1);
		time_dep.insert(time_dep.begin() + node_add_to, time_add1 + map_airstrip[location_to].ground_time);

		double time1 = 0.0;
		if (!non_to_final) {
			//cout << "sono qua dentro" << endl;
			time1 = ((from_to[places[node_add_to - 1]][location_to] / map_airplane[aircraft_code].speed) * 60)
				+ ((from_to[location_to][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60)
				+ map_airstrip[location_to].ground_time
				- ((from_to[places[node_add_to - 1]][places[node_add_to + 1]] / map_airplane[aircraft_code].speed) * 60);
		}

		index++;
		start_end_routes[p.giorno - 1][1]++;
		if (p.giorno == 1) {
			if (start_end_routes[1][1] != -1) {
				start_end_routes[1][0]++;
				start_end_routes[1][1]++;
			}
			if (start_end_routes[2][1] != -1) {
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;
			}
		}
		if (p.giorno == 2) {
			start_end_routes[2][0]++;
			start_end_routes[2][1]++;
		}

		//index_cap++;


		for (int i = node_add_to + 1; i <= start_end_routes[p.giorno - 1][1]; i++) {
			time_arr[i] += time1;
			if (i != start_end_routes[p.giorno - 1][1]) time_dep[i] += time1;
		}
		refueling.insert(refueling.begin() + node_add_to, map_airstrip[location_to].fuel);
		quantity_fuel.insert(quantity_fuel.begin() + node_add_to, 0.0);
		weight.insert(weight.begin() + node_add_to, 0.0);
		capacity.insert(capacity.begin() + node_add_to, 0);


		if (non_to_final) {
			//qua devi aggiungere la connessione
			
			if (capacity_connection > 0) {
				messa_connessione = true;

				double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1]]][location_connection] / map_airplane[aircraft_code].speed) * 60;

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, location_connection);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + time_fly);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_departure_day_connection);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[location_connection].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, 0.0);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, 0.0);
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, capacity_connection);



				//aggiorno indici passeggeri 
				for (Passenger& pi : passengers_in_route) {
					if (pi.giorno > p.giorno) {
						pi.solution_from++;
						pi.solution_to++;
					}
				}

				//questo è comune ai due casi, ovvero sia che la connessione possa fare fuel che no
				index++;
				start_end_routes[p.giorno - 1][1]++;

				if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}
				}
				if (p.giorno == 2) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}
			}
			else {
				double fly_check = (from_to[places[start_end_routes[p.giorno - 1][1]]][places[start_end_routes[p.giorno - 1][1] + 1]] / map_airplane[aircraft_code].speed) * 60;
				if (time_arr[start_end_routes[p.giorno - 1][1] + 1] - fly_check < start_day) {
					messa_connessione = true;

					double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1]]][location_connection] / map_airplane[aircraft_code].speed) * 60;

					places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, location_connection);
					time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + time_fly);
					time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_departure_day_connection);
					refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[location_connection].fuel);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, 0.0);
					weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, 0.0);
					capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, capacity_connection);



					//aggiorno indici passeggeri 
					for (Passenger& pi : passengers_in_route) {
						if (pi.giorno > p.giorno) {
							pi.solution_from++;
							pi.solution_to++;
						}
					}

					//questo è comune ai due casi, ovvero sia che la connessione possa fare fuel che no
					index++;
					start_end_routes[p.giorno - 1][1]++;

					if (p.giorno == 1) {
						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][0]++;
							start_end_routes[1][1]++;
						}
						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;
						}
					}
					if (p.giorno == 2) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}
				}
				else {
					

					//nuovo codice aggiunto ora
					if (location_to == places[start_end_routes[p.giorno - 1][1] + 1]) {
						
						node_add_to = start_end_routes[p.giorno - 1][1];

						time_dep[start_end_routes[p.giorno - 1][1]] = time_dep[start_end_routes[p.giorno - 1][1] + 1];
						capacity[start_end_routes[p.giorno - 1][1]] = capacity[start_end_routes[p.giorno - 1][1] + 1];
						//quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
						//weight[start_end_routes[p.giorno - 1][1]] = weight[start_end_routes[p.giorno - 1][1] + 1];
						messa_connessione = true;
						fuel_connection = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
						weight_connection = weight[start_end_routes[p.giorno - 1][1] + 1];



						places.erase(places.begin() + start_end_routes[p.giorno - 1][1] + 1);
						time_arr.erase(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1);
						time_dep.erase(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1);
						refueling.erase(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1);
						quantity_fuel.erase(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1);
						weight.erase(weight.begin() + start_end_routes[p.giorno - 1][1] + 1);
						capacity.erase(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1);
						index--;
						
						if (p.giorno == 1) {
							if (start_end_routes[1][1] != -1) {
								start_end_routes[1][1]--;
							
								if (start_end_routes[2][1] != -1) {
									start_end_routes[2][0]--;
									start_end_routes[2][1]--;
								}
							}
							else if (start_end_routes[2][1] != -1) {
								start_end_routes[2][1]--;
							}

							//CONTROLLA QUESTO PEZZO
							//pezzo aggiunto per l'ultimo errore trovato da ted nel repair regret
							if (start_end_routes[1][1] != -1) {
								if (start_end_routes[1][1] == start_end_routes[1][0]) {
									start_end_routes[1][0] = -1;
									start_end_routes[1][1] = -1;
									//fuel_connection = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
									//weight_connection = weight[start_end_routes[p.giorno - 1][1] + 1];
									//cout << "il nodo in cui cambio il time dep ha indice " << start_end_routes[p.giorno - 1][1] << endl;
									//cout << "il nodo da cui prendo il time arrivo ha indice " << start_end_routes[p.giorno - 1][1] + 1 << endl;
									//cout << "che ha places pari a " << places[start_end_routes[p.giorno - 1][1] + 1] << endl;
									//cout << "che ha time arr pari a " << time_arr[start_end_routes[p.giorno - 1][1] + 1] << endl;
			

									double time_fl = (from_to[places[start_end_routes[p.giorno - 1][1]]][places[start_end_routes[p.giorno - 1][1] + 1]] / map_airplane[aircraft_code].speed) * 60;
									//cout << "gli sottraggo " << time_fl << endl;
									time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1] + 1] - time_fl;
									//cout << "il nuovo time dep diventa" << time_dep[start_end_routes[p.giorno - 1][1]] << endl;
									//capacity[start_end_routes[p.giorno - 1][1]] = capacity[start_end_routes[p.giorno - 1][1] + 1];
								}
							}



						}
						if (p.giorno == 2) {
							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][1]--;
							}
						}


						


						
						//aggiorno indici passeggeri
						for (Passenger& pi : passengers_in_route) {
							if (pi.giorno > p.giorno) {
								pi.solution_from--;
								pi.solution_to--;
							}
						}


					}
					else {
						//questo c'era gia prima dei due if
						time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1] + 1] - fly_check;
					}




				}
				
			}



		}

	}



	



}

void Route::update_route_rebuilt_one_second_fase_with_after(bool& messa_connessione, double&  fuel_connection, double& weight_connection, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals) {

	//in questo commento qua sotto ci sono i vari casi con l'if al posto dello switch

	if (caso == -1) {
		cout << "problema in route.cpp con la variabile caso" << endl;
		cin.get();
	}
	else if (caso == 1) {

		p.solution_from = node_add_from;
		for (int i = node_add_from; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_from; j <= start_end_routes[p.giorno - 1][1]; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 467 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {  //QUA HO LASCIATO 0 e non prendo la nuova info perchè potrei avere il refueling nel giorno prima
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
							//ccout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
			}

		}





	}
	else if (caso == 2) {
		//il nodo dopo il from è uguale quindi devo aggiornare le capacità e basta, non i tempi
		p.solution_from = node_add_from + 1;
		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		double add_fuel = 0;
		int index_weight_neg = -1;

		for (int j = node_add_from + 1; j <= start_end_routes[p.giorno - 1][1]; j++) {
			if (j == node_add_to && non_to) {
				// non faccio niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 581 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (i == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) {
						if (t == node_add_to && non_to) {
							// non faccio niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//ccout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}

						//ccout << " Weight dopo " << weight[t] << endl;
					}
				}
			}

		}

	}
	else if (caso == 3) {

		p.solution_from = node_add_from + 1;

		//update passengers in route
		for (Passenger& p : passengers_in_route) {
			if (p.solution_from > node_add_from) p.solution_from++;
			if (p.solution_to > node_add_from) p.solution_to++;
		}

		//update of fuel, capacity and weight
		capacity[node_add_from + 1] = capacity[node_add_from];
		weight[node_add_from + 1] = weight[node_add_from];

		for (int i = node_add_from + 1; i < node_add_to; i++) {
			capacity[i]++;
			weight[i] -= p.weight;
		}

		//fuel consumed
		int A = 0;
		int B = 0;
		if (node_add_to == node_add_from + 1 && non_to) {
			A = node_add_from + 2;
			B = node_add_from + 3;
		}
		else if (node_add_to == node_add_from + 2 && non_to) {
			A = node_add_from + 1;
			B = node_add_from + 3;
		}
		else {
			A = node_add_from + 1;
			B = node_add_from + 2;
		}



		double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[A]];
		//GUARDA BENE QUAAAAAAAAAAAAAAAAAA
		if (refueling[A]) {
			quantity_fuel[A] = map_airplane[aircraft_code].max_fuel;
			weight[A] = weight[A] - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);  //ho aggiunto questa riga
		}
		else if (!refueling[A]) {
			quantity_fuel[A] = quantity_fuel[node_add_from] - fuel_consumed;
			weight[A] = weight[A] + fuel_consumed;
		}

		double fuel_consumed1 = from_to_fuelConsumed[aircraft_code][places[A]][places[B]];
		double fuel_consumed2 = from_to_fuelConsumed[aircraft_code][places[node_add_from]][places[B]];
		if (!refueling[A]) {
			for (int i = B; i < index ; i++) {  //QUA PRIMA C'ERA MINORE UGUALE E start_end_routes[p.giorno - 1][1]
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed - fuel_consumed1 + fuel_consumed2; // qua ho aggiunto il meno fuel_consumed che secondo me mancava
						weight[i] = weight[i] + fuel_consumed + fuel_consumed1 - fuel_consumed2;
					}
					else {
						break;
					}
				}

			}
		}
		else {
			//caso bastardo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			for (int i = B; i < index ; i++) {  //qua prima c'era minore uguale e start_end_routes[p.giorno - 1][1]
				if (non_to && i == node_add_to) {
					//non devo fare niente
				}
				else {
					if (!refueling[i]) {
						quantity_fuel[i] = quantity_fuel[i] - fuel_consumed1 + fuel_consumed2 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
						weight[i] = weight[i] + fuel_consumed1 - fuel_consumed2 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_from]);
					}
					else {
						break;
					}
				}
			}
		}


		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = A; j <= start_end_routes[p.giorno - 1][1]; j++) { //secondo me qua non è NODE_ADD_TO ma deve essere INDEX
			if (non_to && j == node_add_to) {
				//non devo fare niente
			}
			else {
				if (weight[j] < 0) {
					//ccout << " Sono nel rapeir devo aggiunger fuel linea 789 devo guardare da " << node_add_from + 1 << " a " << node_add_to << endl;
					//ccout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (non_to && i == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[i]) {
								index_refueling = i;
								break;
							}
						}
					}
					//ccout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < index; t++) { //qua prima c'era <= start_end_routes[p.giorno - 1][1]
						if (non_to && t == node_add_to) {
							//non devo fare niente
						}
						else {
							if (refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							quantity_fuel[t] += add_fuel;
							weight[t] -= add_fuel;
						}
						//ccout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
		}

	}


	if (non_to) {

		p.solution_to = node_add_to;
		//update passengers in route
		for (Passenger& pi : passengers_in_route) {
			if (pi.solution_from >= node_add_to || pi.giorno > p.giorno) pi.solution_from++;
			if (pi.solution_to >= node_add_to || pi.giorno > p.giorno) pi.solution_to++;
		}



		double weight_add = 0.0;
		/////////////////////////////////////////
		//fuel consumed


		double fuel_consumed3 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]];


		if (refueling[node_add_to]) {
			quantity_fuel[node_add_to] = map_airplane[aircraft_code].max_fuel;
		}
		else if (!refueling[node_add_to]) {
			quantity_fuel[node_add_to] = quantity_fuel[node_add_to - 1] - fuel_consumed3;
		}




		//******************************************************************controlla bene***********************************************************************************************
		if (!non_to_final) {
			capacity[node_add_to] = capacity[node_add_to - 1] - 1;
			if (refueling[node_add_to]) {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
			}
			else {
				weight[node_add_to] = weight[node_add_to - 1] + p.weight + fuel_consumed3;

			}
		}
		//********************************************************************************************************************************************************************************




		double fuel_consumed4 = 0.0;
		if (!non_to_final) {
			fuel_consumed4 = from_to_fuelConsumed[aircraft_code][places[node_add_to]][places[node_add_to + 1]];
		}

		double fuel_consumed5 = 0.0;
		if (!non_to_final) {
			fuel_consumed5 = from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to + 1]];
		}

		//qua devo considerare il fottuto caso bastardo
		if (!refueling[node_add_to]) {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed3 - fuel_consumed4 + fuel_consumed5;
					weight[i] = weight[i] + fuel_consumed3 + fuel_consumed4 - fuel_consumed5;
				}
				else {
					break;
				}
			}
		}
		else {
			for (int i = node_add_to + 1; i < index; i++) {
				if (!refueling[i]) {
					quantity_fuel[i] = quantity_fuel[i] - fuel_consumed4 + fuel_consumed5 + (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
					weight[i] = weight[i] + fuel_consumed4 - fuel_consumed5 - (map_airplane[aircraft_code].max_fuel - quantity_fuel[node_add_to - 1]);
				}
				else {
					break;
				}
			}
		}


		if (non_to_final) {
			if (map_airstrip[location_to].fuel) weight_add = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
			else {
				weight_add = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[node_add_to - 1] - from_to_fuelConsumed[aircraft_code][places[node_add_to - 1]][places[node_add_to]]);
			}
			weight[node_add_to] += weight_add;

			if (messa_connessione) {
				if (refueling[start_end_routes[p.giorno - 1][1]]) {
					quantity_fuel[start_end_routes[p.giorno - 1][1]] = fuel_connection;
					weight[start_end_routes[p.giorno - 1][1]] = weight_connection;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1] - 1]][places[start_end_routes[p.giorno - 1][1]]];
					quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] - 1] - fuel_consumed;
					weight[start_end_routes[p.giorno - 1][1]] = weight_connection + (fuel_connection - quantity_fuel[start_end_routes[p.giorno - 1][1]]);
				}
			}
			


		}


		//ho aggiunto questa parte **************************** per l'errore sul peso e sul refuel MA NON NE SONO SICURO


		//cout << "sono davanti all'aggiornamento del caso in cui ho inserito il to" << endl;
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = node_add_to; j < index; j++) {
			//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
			if (weight[j] < 0) {
				//cout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//cout << " Weight dopo " << weight[t] << endl;
				}
			}
		}



		





	}
	else {
		p.solution_to = node_add_to;
	}



	


	
	
	//pezzo per aggiornare il fuel solo se necessario
	bool aggiorna_fuel = true;
	for (int i = node_add_to+1; i <= start_end_routes[p.giorno - 1][1]; i++) {  
		if (refueling[i]) {
			aggiorna_fuel = false;
			break;
		}
	}
	//se arrivo qua e non ho locazioni che fanno fuel in mezzo devo aggiornare
	if (aggiorna_fuel) {
		double fuel_consumed_new = 0.0;
		for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
			if (refueling[i]) break;
			fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
			double fuel_node_early = quantity_fuel[i];
			quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
			weight[i] -= (quantity_fuel[i] - fuel_node_early);

		}




		//ora potrebbe esserci un peso negativo dato dal fatto che ho più benzina in alcuni casi, devo mettere il codice che mi pesca fuel
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = p.solution_to; j < index; j++) {
			//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
			if (weight[j] < 0) {
				//cout << " Weigth negativo in " << j << weight[j] << endl;
				add_fuel = weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (refueling[i]) {
						index_refueling = i;
						break;
					}
				}
				//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
				for (int t = index_refueling; t < index; t++) {
					if (refueling[t] && t != index_refueling) break;
					//cout << " Aggiorno nodo " << t << endl;
					quantity_fuel[t] += add_fuel;
					weight[t] -= add_fuel;
					//cout << " Weight dopo " << weight[t] << endl;
				}
			}
		}




	}
	
	
	

	//pezzo che ridà fuel
	for (int k = 0; k <= start_end_routes[p.giorno - 1][1]; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}









}

void Route::move_c_with_after(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {


	int locations_added = 0;
	

	if (places[start_end_routes[p.giorno - 1][1]] == location_from) {
		//in questo caso dato che la connessione è uguale al from, sicuramente il to non sarà uguale alla connessione


		//mi salvo le informazioni della connessione
		int location_connection = places[start_end_routes[p.giorno - 1][1]];
		double time_depa_connection = time_dep[start_end_routes[p.giorno - 1][1]]; //mi salvo il tempo in cui partiva l'aereo nel giorno precedente
		double capacity_connection = capacity[start_end_routes[p.giorno - 1][1]];
		double weight_connection = weight[start_end_routes[p.giorno - 1][1]];
		double fuel_connection = quantity_fuel[start_end_routes[p.giorno - 1][1]];
		
		
		//ora modifico capacità, fuel e weight di quel nodo che non sarà più il finale
		capacity[start_end_routes[p.giorno - 1][1]] = 1;
		//modifico il fuel ed il weight
		if (map_airstrip[location_from].fuel) {
			quantity_fuel[start_end_routes[p.giorno - 1][1]] = map_airplane[aircraft_code].max_fuel;
			weight[start_end_routes[p.giorno - 1][1]] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
		}
		else {
			quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] - 1] - from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1] - 1]][location_from];
			weight[start_end_routes[p.giorno - 1][1]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[p.giorno - 1][1]] - p.weight;
		}
		
		
		time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1]] + map_airstrip[location_from].ground_time;

		double time_arri = time_dep[start_end_routes[p.giorno - 1][1]] + (from_to[location_from][location_to] / map_airplane[aircraft_code].speed) * 60;
		double time_depa = time_arri + map_airstrip[location_to].ground_time;
		

		
		


		p.solution_from = start_end_routes[p.giorno - 1][1];

		//double time_fly = (from_to[location_from + ";" + location_to] / map_airplane[aircraft_code].speed) * 60;


		double fuel_consumed = from_to_fuelConsumed[aircraft_code][location_from][location_to];
		

		if (map_airstrip[location_to].fuel) {
			places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, location_to);
			refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[location_to].fuel);
			quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airplane[aircraft_code].max_fuel);
			weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, (map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel));
			capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, 0);
			time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_arri);
			time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_depa);
			
			locations_added++;
		}
		else {
			//addPlace(location_to, map_airstrip[location_to].fuel, quantity_fuel[start_end_routes[p.giorno - 1][1]] - fuel_consumed, (map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[start_end_routes[p.giorno - 1][1]] - fuel_consumed)), 0, time_arri, time_depa);
			places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, location_to);
			refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[location_to].fuel);
			quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, quantity_fuel[start_end_routes[p.giorno - 1][1]] - fuel_consumed);
			weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, (map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[start_end_routes[p.giorno - 1][1]] - fuel_consumed)));
			capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, 0);
			time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_arri);
			time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_depa);
			locations_added++;
		}
		index++;
		start_end_routes[p.giorno - 1][1]++;
		p.solution_to = start_end_routes[p.giorno - 1][1];
		//start_end_routes[p.giorno - 1][1]++; lo devo fare due volte perchè la fine sarà il giorno successivo
		

		if (p.giorno == 1) {
			if (start_end_routes[1][1] != -1) {
				start_end_routes[1][0]++;
				start_end_routes[1][1]++;

			}
			if (start_end_routes[2][1] != -1) {
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;
				

			}
		}
		if (p.giorno == 2) {
			start_end_routes[2][0]++;
			start_end_routes[2][1]++;
		}

		
		

		//in questo pezzo del codice si aggiunge la connessione, se nella connessione c'è capacità rimane uguale, altrimenti non aggiungere la connessione ma modifica il tempo di partenza e fuel e qweight
		if (capacity_connection > 0) {
			double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1]]][location_connection] / map_airplane[aircraft_code].speed) * 60;
			if (map_airstrip[location_connection].fuel) {

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, location_connection);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[location_connection].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, fuel_connection);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, weight_connection);
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, capacity_connection);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + time_fly);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_depa_connection);
				locations_added++;
				index++;
			}
			else {
				//in questo caso devi aggiornare il fuel e il peso del giorno di connessione e dopo di che aggiorni fino al prossimo fuel

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, location_connection);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[location_connection].fuel);
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1]]][location_connection];
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, quantity_fuel[start_end_routes[p.giorno - 1][1]] - fuel_consumed);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, weight_connection + (fuel_connection - quantity_fuel[start_end_routes[p.giorno - 1][1]+1]));
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, capacity_connection);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + time_fly);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_depa_connection);
				index++;
				locations_added++;
				for (int i = start_end_routes[p.giorno - 1][1] + 2; i < index; i++) {
					if (refueling[i]) break;
					double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fb = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
					weight[i] -= (quantity_fuel[i] - fb);
				}




			}

			start_end_routes[p.giorno - 1][1]++;
			if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;

					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;


					}
				}
			if (p.giorno == 2) {
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;
			}

		}
		else {
			//nuovo codice aggiunto ora
			if (location_to == places[start_end_routes[p.giorno - 1][1] + 1]) {
				
				for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
					if (refueling[i]) break;
					double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fb = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
					weight[i] -= (quantity_fuel[i] - fb);
				}
				
				time_dep[start_end_routes[p.giorno - 1][1]] = time_dep[start_end_routes[p.giorno - 1][1] + 1];
				capacity[start_end_routes[p.giorno - 1][1]] = capacity[start_end_routes[p.giorno - 1][1] + 1];
				quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
				weight[start_end_routes[p.giorno - 1][1]] = weight[start_end_routes[p.giorno - 1][1] + 1];

				places.erase(places.begin() + start_end_routes[p.giorno - 1][1] + 1);
				time_arr.erase(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1);
				time_dep.erase(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1);
				refueling.erase(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1);
				quantity_fuel.erase(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1);
				weight.erase(weight.begin() + start_end_routes[p.giorno - 1][1] + 1);
				capacity.erase(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1);
				index--;

				if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][1]--;

						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]--;
							start_end_routes[2][1]--;
						}
					}
					else if (start_end_routes[2][1] != -1) {
						start_end_routes[2][1]--;
					}



					//CONTROLLA QUESTO PEZZO
							//pezzo aggiunto per l'ultimo errore trovato da ted nel repair regret
					if (start_end_routes[1][1] != -1) {
						if (start_end_routes[1][1] == start_end_routes[1][0]) {
							start_end_routes[1][0] = -1;
							start_end_routes[1][1] = -1;
							//prima era con il +1
							//fuel_connection = quantity_fuel[start_end_routes[p.giorno - 1][1]];
							//weight_connection = weight[start_end_routes[p.giorno - 1][1]];
							double time_fl = (from_to[places[start_end_routes[p.giorno - 1][1]]][places[start_end_routes[p.giorno - 1][1] + 1]] / map_airplane[aircraft_code].speed) * 60;
							time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1] + 1] - time_fl;
							//capacity[start_end_routes[p.giorno - 1][1]] = capacity[start_end_routes[p.giorno - 1][1] + 1];
							//quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
							//weight[start_end_routes[p.giorno - 1][1]] = weight[start_end_routes[p.giorno - 1][1] + 1];
						}
					}




				}
				if (p.giorno == 2) {
					start_end_routes[2][1]--;
				}

				//aggiorno indici passeggeri 
				for (Passenger& pi : passengers_in_route) {
					if (pi.giorno > p.giorno) {
						pi.solution_from--;
						pi.solution_to--;
					}
				}


			}
			else {
				//questo pezzo c'era prima dell'IF
				double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1]]][places[start_end_routes[p.giorno - 1][1] + 1]] / map_airplane[aircraft_code].speed) * 60;
				time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1] + 1] - time_fly;

				for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
					if (refueling[i]) break;
					double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fb = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
					weight[i] -= (quantity_fuel[i] - fb);
				}

			}
		}




	}
	else {
		//in questo caso devo valutare se il to è uguale o no alla connessione prima di valutare la capacità e quindi se posso toglierlo

		//mi salvo le informazioni della connessione
		int location_connection = places[start_end_routes[p.giorno - 1][1]];
		double time_depa_connection = time_dep[start_end_routes[p.giorno - 1][1]]; //mi salvo il tempo in cui partiva l'aereo nel giorno precedente
		double capacity_connection = capacity[start_end_routes[p.giorno - 1][1]];
		double weight_connection = weight[start_end_routes[p.giorno - 1][1]];
		double fuel_connection = quantity_fuel[start_end_routes[p.giorno - 1][1]];

		//aggiorno il tempo di partenza del nodo che prima era connessione
		time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1]] + map_airstrip[location_connection].ground_time;
		capacity[start_end_routes[p.giorno - 1][1]] = 0;
		weight[start_end_routes[p.giorno - 1][1]] = map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[p.giorno - 1][1]];
		
		if (map_airstrip[p.departure_location].fuel) {
			

			//cout << " p.giorno e': " << p.giorno << endl;
			//cout << "lo start end_day e': " << start_end_routes[p.giorno - 1][1] + 1 << endl;

			places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, p.departure_location);
			refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[p.departure_location].fuel);
			quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airplane[aircraft_code].max_fuel);
			weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, (map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight));
			capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, 1);
			time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] +  (from_to[places[start_end_routes[p.giorno - 1][1]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60);
			time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + (from_to[places[start_end_routes[p.giorno - 1][1]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60 + map_airstrip[p.departure_location].ground_time);
			locations_added++;
			index++;
			start_end_routes[p.giorno - 1][1]++;


			if (p.giorno == 1) {
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}
			}
			if (p.giorno == 2) {
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;
			}


		}
		else {
			double fuel_before = quantity_fuel[start_end_routes[p.giorno-1][1]];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1]]][p.departure_location];
			//double travel_time = (from_to[places[index - 1] + ";" + p.departure_location] / map_airplane[aircraft_code].speed) * 60;


			

			places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, p.departure_location);
			refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[p.departure_location].fuel);
			quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, fuel_before - fuel_consumed);
			weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, (map_airplane[aircraft_code].weight_fuel_people - quantity_fuel[start_end_routes[p.giorno - 1][1]+1] - p.weight));
			capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, 1);
			time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + ((from_to[places[start_end_routes[p.giorno - 1][1]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60));
			time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time_dep[start_end_routes[p.giorno - 1][1]] + ((from_to[places[start_end_routes[p.giorno - 1][1]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);
			locations_added++;
			index++;
			start_end_routes[p.giorno - 1][1]++;

			if (p.giorno == 1) {
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}
			}
			if (p.giorno == 2) {
				start_end_routes[2][0]++;
				start_end_routes[2][1]++;
			}



		}

		p.solution_from = start_end_routes[p.giorno - 1][1];
		double time = time_dep[start_end_routes[p.giorno - 1][1]] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60);




		if (p.arrival_location != location_connection) {
			if (map_airstrip[p.arrival_location].fuel) {
				/*addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				weight[index - 1] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				*/

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, p.arrival_location);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airplane[aircraft_code].max_fuel);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, (map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel));
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, 0);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time + map_airstrip[p.arrival_location].ground_time);
				locations_added++;
				index++;
				start_end_routes[p.giorno - 1][1]++;


				if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}
				}
				if (p.giorno == 2) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

			}
			else {
				double fuel_before = quantity_fuel[start_end_routes[p.giorno - 1][1]];
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1]]][p.arrival_location];
				//double travel_time = (from_to[places[index - 1] + ";" + p.arrival_location] / map_airplane[aircraft_code].speed) * 60;


				/*if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[aircraft_code].fuel_burn_second;
				}*/
				/*addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				weight[index - 1] = weight[index - 2] + fuel_consumed + p.weight;
				*/

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1] + 1, p.arrival_location);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1, map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1, fuel_before - fuel_consumed);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1] + 1, (weight[start_end_routes[p.giorno - 1][1]] + fuel_consumed + p.weight));
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1, 0);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1, time);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1, time + map_airstrip[p.arrival_location].ground_time);
				locations_added++;
				index++;
				start_end_routes[p.giorno - 1][1]++;



				if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}
				}
				if (p.giorno == 2) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}





			}
		
			p.solution_to = start_end_routes[p.giorno - 1][1];
			


			if (capacity_connection > 0) {
				start_end_routes[p.giorno - 1][1]++;

				//ora devo aggiungere il nodo di connessione
				double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1] - 1]][location_connection] / map_airplane[aircraft_code].speed) * 60;
				double fuel_before = quantity_fuel[start_end_routes[p.giorno - 1][1] - 1];
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1] - 1]][location_connection];

				if (map_airstrip[location_connection].fuel) {

					places.insert(places.begin() + start_end_routes[p.giorno - 1][1], location_connection);
					refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1], map_airstrip[location_connection].fuel);
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1], fuel_connection);
					weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1], weight_connection);
					capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1], capacity_connection);
					time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1], time_dep[start_end_routes[p.giorno - 1][1] - 1] + time_fly);
					time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1], time_depa_connection);
					locations_added++;
					index++;


					if (p.giorno == 1) {
						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][0]++;
							start_end_routes[1][1]++;
						}
						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;
						}
					}
					if (p.giorno == 2) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}




				}
				else {
					//in questo caso devi aggiornare il fuel e il peso del giorno di connessione e dopo di che aggiorni fino al prossimo fuel

					places.insert(places.begin() + start_end_routes[p.giorno - 1][1], location_connection);
					refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1], map_airstrip[location_connection].fuel);
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1] - 1]][location_connection];
					quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1], quantity_fuel[start_end_routes[p.giorno - 1][1] - 1] - fuel_consumed);
					weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1], weight_connection + (fuel_connection - quantity_fuel[start_end_routes[p.giorno - 1][1]]));
					capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1], capacity_connection);
					time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1], time_dep[start_end_routes[p.giorno - 1][1] - 1] + time_fly);
					time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1], time_depa_connection);
					index++;
					locations_added++;
				
					for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
						if (refueling[i]) break;
						double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
						double fb = quantity_fuel[i];
						quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
						weight[i] -= (quantity_fuel[i] - fb);
					}



					if (p.giorno == 1) {
						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][0]++;
							start_end_routes[1][1]++;
						}
						if (start_end_routes[2][1] != -1) {
							start_end_routes[2][0]++;
							start_end_routes[2][1]++;
						}
					}
					if (p.giorno == 2) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}






				}

			}
			else {
				
				//nuovo codice aggiunto ora
				if (location_to == places[start_end_routes[p.giorno - 1][1] + 1]) {
					
					for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
						if (refueling[i]) break;
						double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
						double fb = quantity_fuel[i];
						quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
						weight[i] -= (quantity_fuel[i] - fb);
					}

					time_dep[start_end_routes[p.giorno - 1][1]] = time_dep[start_end_routes[p.giorno - 1][1] + 1];
					capacity[start_end_routes[p.giorno - 1][1]] = capacity[start_end_routes[p.giorno - 1][1] + 1];
					quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
					weight[start_end_routes[p.giorno - 1][1]] = weight[start_end_routes[p.giorno - 1][1] + 1];

					places.erase(places.begin() + start_end_routes[p.giorno - 1][1] + 1);
					time_arr.erase(time_arr.begin() + start_end_routes[p.giorno - 1][1] + 1);
					time_dep.erase(time_dep.begin() + start_end_routes[p.giorno - 1][1] + 1);
					refueling.erase(refueling.begin() + start_end_routes[p.giorno - 1][1] + 1);
					quantity_fuel.erase(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1] + 1);
					weight.erase(weight.begin() + start_end_routes[p.giorno - 1][1] + 1);
					capacity.erase(capacity.begin() + start_end_routes[p.giorno - 1][1] + 1);
					index--;

					if (p.giorno == 1) {
						if (start_end_routes[1][1] != -1) {
							start_end_routes[1][1]--;

							if (start_end_routes[2][1] != -1) {
								start_end_routes[2][0]--;
								start_end_routes[2][1]--;
							}
						}
						else if (start_end_routes[2][1] != -1) {
							start_end_routes[2][1]--;
						}
					
						//CONTROLLA QUESTO PEZZO
							//pezzo aggiunto per l'ultimo errore trovato da ted nel repair regret
						if (start_end_routes[1][1] != -1) {
							if (start_end_routes[1][1] == start_end_routes[1][0]) {
								start_end_routes[1][0] = -1;
								start_end_routes[1][1] = -1;
								//fuel_connection = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
								//weight_connection = weight[start_end_routes[p.giorno - 1][1] + 1];
								double time_fl = (from_to[places[start_end_routes[p.giorno - 1][1]]][places[start_end_routes[p.giorno - 1][1] + 1]] / map_airplane[aircraft_code].speed) * 60;
								time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1] + 1] - time_fl;
								//capacity[start_end_routes[p.giorno - 1][1]] = capacity[start_end_routes[p.giorno - 1][1] + 1];
								//quantity_fuel[start_end_routes[p.giorno - 1][1]] = quantity_fuel[start_end_routes[p.giorno - 1][1] + 1];
								//weight[start_end_routes[p.giorno - 1][1]] = weight[start_end_routes[p.giorno - 1][1] + 1];
							}
						}
					
					
					
					
					}
					if (p.giorno == 2) {
						start_end_routes[2][1]--;
					}

					//aggiorno indici passeggeri 
					for (Passenger& pi : passengers_in_route) {
						if (pi.giorno > p.giorno) {
							pi.solution_from--;
							pi.solution_to--;
						}
					}


				}
				else {
				
					//questo pezzo c'era gia prima dell'IF GRANDE
				
					double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1]]][places[start_end_routes[p.giorno - 1][1] + 1]] / map_airplane[aircraft_code].speed) * 60;
					time_dep[start_end_routes[p.giorno - 1][1]] = time_arr[start_end_routes[p.giorno - 1][1] + 1] - time_fly;

					//devo aggiornare il fuel ed il peso fino al prossimo refueling  CASO NUOVO
					//if (!map_airstrip[p.arrival_location].fuel) {
					for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
						if (refueling[i]) break;
						double fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
						double fb = quantity_fuel[i];
						quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
						weight[i] -= (quantity_fuel[i] - fb);
					}
				}
			}


		}
		else {

			double time_fly = (from_to[places[start_end_routes[p.giorno - 1][1]]][location_connection] / map_airplane[aircraft_code].speed) * 60;
			double fuel_before = quantity_fuel[start_end_routes[p.giorno - 1][1]];
			double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1]]][p.arrival_location];

			start_end_routes[p.giorno - 1][1]++;

			if (map_airstrip[location_connection].fuel) {

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1], location_connection);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1], map_airstrip[location_connection].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1], fuel_connection);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1], weight_connection);
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1], capacity_connection);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1], time_dep[start_end_routes[p.giorno - 1][1]-1] + time_fly);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1], time_depa_connection);
				locations_added++;
				index++;


				if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}
				}
				if (p.giorno == 2) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}



			}
			else {
				//in questo caso devi aggiornare il fuel e il peso del giorno di connessione e dopo di che aggiorni fino al prossimo fuel

				places.insert(places.begin() + start_end_routes[p.giorno - 1][1], location_connection);
				refueling.insert(refueling.begin() + start_end_routes[p.giorno - 1][1], map_airstrip[location_connection].fuel);
				double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[start_end_routes[p.giorno - 1][1]-1]][location_connection];
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[p.giorno - 1][1], quantity_fuel[start_end_routes[p.giorno - 1][1]-1] - fuel_consumed);
				weight.insert(weight.begin() + start_end_routes[p.giorno - 1][1], weight_connection + (fuel_connection - quantity_fuel[start_end_routes[p.giorno - 1][1]]));
				capacity.insert(capacity.begin() + start_end_routes[p.giorno - 1][1], capacity_connection);
				time_arr.insert(time_arr.begin() + start_end_routes[p.giorno - 1][1], time_dep[start_end_routes[p.giorno - 1][1]-1] + time_fly);
				time_dep.insert(time_dep.begin() + start_end_routes[p.giorno - 1][1], time_depa_connection);
				index++;
				locations_added++;
				


				if (p.giorno == 1) {
					if (start_end_routes[1][1] != -1) {
						start_end_routes[1][0]++;
						start_end_routes[1][1]++;
					}
					if (start_end_routes[2][1] != -1) {
						start_end_routes[2][0]++;
						start_end_routes[2][1]++;
					}
				}
				if (p.giorno == 2) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}



			}
			
			
	
			
			
			//start_end_routes[p.giorno - 1][1]++;
			p.solution_to = start_end_routes[p.giorno - 1][1];

			


		}

		



	}



	for (Passenger& pi : passengers_in_route) {
		if (pi.giorno > p.giorno) {
			pi.solution_from += locations_added;
			pi.solution_to += locations_added;
		}
	}



	//PARTICOLARIA' DEL FATTO CHE CI SONO DELLE ROUTE DOPO

	//metto a posto il fuel e il weight nella route dopo dato che ho fatto la mossa C***********************************************************************
	int place_end = places[start_end_routes[p.giorno - 1][1]];
	double fuel_consumed_new = 0.0;
	if (!map_airstrip[place_end].fuel) {
		for (int i = start_end_routes[p.giorno - 1][1] + 1; i < index; i++) {
			if (refueling[i]) break;
			fuel_consumed_new = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
			double fuel_node_early = quantity_fuel[i];
			quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed_new;
			weight[i] -= (quantity_fuel[i] - fuel_node_early);
		}
	}
	//******************************************************************************************************************************************************



	//ora devi rimettere fuel se c'è n'è bisogno o pescare fuel************************************************************************************************************
	
	//pezzo che pesca fuel;
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = p.solution_from; j < index; j++) {
		//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
		if (weight[j] < 0) {
			//cout << " Weigth negativo in " << j << weight[j] << endl;
			add_fuel = weight[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (refueling[i]) {
					index_refueling = i;
					break;
				}
			}
			//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
			for (int t = index_refueling; t < index; t++) {
				if (refueling[t] && t != index_refueling) break;
				//cout << " Aggiorno nodo " << t << endl;
				quantity_fuel[t] += add_fuel;
				weight[t] -= add_fuel;
				//cout << " Weight dopo " << weight[t] << endl;
			}
		}
	}


	//pezzo che ridà fuel
	for (int k = start_end_routes[p.giorno-1][0]; k < start_end_routes[p.giorno - 1][1]; k++) {
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}









	//******************************************************************************************************************************************************

	


}


void Route::move_before(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed) {

	//if inzia_da_start_day è verà vuol dire che bisogna spostare i valori in avanti partendo da start_day
	//nel caso in cui sia false posso spostare a ritroso

	int locations_added = 0;


	if (p.giorno == 1) {

		if (capacity[0] == 0) {
			double time_node = time_arr[1];
			double index_until_updating = 1;
			//aggiungo le cose a ritroso
			if (places[1] == p.arrival_location) {
				p.solution_to = 1;
			}
			else {
				//in questo caso lo devo aggiungere 
				double time_ritroso = (from_to[p.arrival_location][places[1]] / map_airplane[aircraft_code].speed) * 60;

				p.solution_to = 1;
				index_until_updating++;
				places.insert(places.begin() + 1, p.arrival_location);
				refueling.insert(refueling.begin() + 1, map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
				weight.insert(weight.begin() + 1, 0.0);
				capacity.insert(capacity.begin() + 1, 0);
				time_arr.insert(time_arr.begin() + 1, time_node - time_ritroso - map_airstrip[p.arrival_location].ground_time);
				time_dep.insert(time_dep.begin() + 1, time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[1]; //aggiorno il valore del giorno prima
				start_end_routes[0][1]++;
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}
			}

			if (places[0] == p.departure_location) {
				p.solution_from = 0;
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				time_dep[0] = time_arr[1] - time_ritroso;
				time_arr[0] = time_dep[0] - map_airstrip[places[0]].ground_time;
				capacity[0] = 1;
			}
			else {
				p.solution_from = 1;
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				index_until_updating++;
				places.insert(places.begin() + 1, p.departure_location);
				refueling.insert(refueling.begin() + 1, map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + 1, 0.0);
				weight.insert(weight.begin() + 1, 0.0);
				capacity.insert(capacity.begin() + 1, 1);
				time_arr.insert(time_arr.begin() + 1, time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin() + 1, time_node - time_ritroso);
				locations_added++;
				index++;
				p.solution_to++;
				time_node = time_arr[1]; //aggiorno il valore del giorno prima
				start_end_routes[0][1]++;
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

				//ora devo aggiornare il nodo iniziale
				double time = (from_to[places[0]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				time_dep[0] = time_node - time;
				time_arr[0] = time_node - time - map_airstrip[places[0]].ground_time;



			}



			for (int i = 0; i < index; i++) {
				if (i >= index_until_updating && refueling[i]) break;
				if (refueling[i]) {
					quantity_fuel[i] = map_airplane[aircraft_code].max_fuel;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_before = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed) - p.weight;
					else if (i >= index_until_updating) weight[i] += fuel_before - quantity_fuel[i];
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed);
				}


			}


		}
		else {

			double time_node = time_arr[0];
			int location_partenza = places[0];
			int index_updating = 0; //in questo caso posso aggiornare fuel e peso fino a index updating in quanto so che quello è un deposito
			if (p.arrival_location == places[0]) {
				p.solution_to = 0;
			}
			else {

				double time_ritroso = (from_to[p.arrival_location][places[0]] / map_airplane[aircraft_code].speed) * 60;
				index_updating++;
				p.solution_to = 0;
				places.insert(places.begin(), p.arrival_location);
				refueling.insert(refueling.begin(), map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin(), 0.0);
				weight.insert(weight.begin(), 0.0);
				capacity.insert(capacity.begin(), 0);
				time_arr.insert(time_arr.begin(), time_node - time_ritroso - map_airstrip[p.arrival_location].ground_time);
				time_dep.insert(time_dep.begin(), time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[0]; //aggiorno il valore del giorno prima
				start_end_routes[0][1]++;
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}



			}

			if (p.departure_location == location_partenza) {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_from = 0;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin(), p.departure_location);
				refueling.insert(refueling.begin(), map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin(), 0.0);
				weight.insert(weight.begin(), 0.0);
				capacity.insert(capacity.begin(), 1);
				time_arr.insert(time_arr.begin(), time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin(), time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[0]; //aggiorno il valore del giorno prima
				start_end_routes[0][1]++;
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}


			}
			else {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_from = 1;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin(), p.departure_location);
				refueling.insert(refueling.begin(), map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin(), 0.0);
				weight.insert(weight.begin(), 0.0);
				capacity.insert(capacity.begin(), 1);
				time_arr.insert(time_arr.begin(), time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin(), time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[0]; //aggiorno il valore del giorno prima
				start_end_routes[0][1]++;
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

				time_ritroso = (from_to[location_partenza][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin(), location_partenza);
				refueling.insert(refueling.begin(), map_airstrip[location_partenza].fuel);
				quantity_fuel.insert(quantity_fuel.begin(), 0.0);
				weight.insert(weight.begin(), 0.0);
				capacity.insert(capacity.begin(), 0);
				time_arr.insert(time_arr.begin(), time_node - time_ritroso - map_airstrip[location_partenza].ground_time);
				time_dep.insert(time_dep.begin(), time_node - time_ritroso);
				locations_added++;
				index++;
				//time_node = time_arr[0]; //aggiorno il valore del giorno prima
				start_end_routes[0][1]++;
				if (start_end_routes[1][1] != -1) {
					start_end_routes[1][0]++;
					start_end_routes[1][1]++;
				}
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}





			}


			for (int i = 0; i < index_updating; i++) {

				if (refueling[i]) {
					quantity_fuel[i] = map_airplane[aircraft_code].max_fuel;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed) - p.weight;
					else  weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed);
				}


			}



		}



	}
	else if (p.giorno == 3) {
		//uguale al caso uno ma devi cambiare gli indici e stare attento che il primo nodo della route non è sempre un deposito

		if (capacity[start_end_routes[2][0]] == 0) {
			double time_arr_partenza = time_arr[start_end_routes[2][0]];

			double time_node = time_arr[start_end_routes[2][0] + 1];
			double index_until_updating = start_end_routes[2][0] + 1;

			//aggiungo le cose a ritroso
			if (places[start_end_routes[2][0] + 1] == p.arrival_location) {
				p.solution_to = start_end_routes[2][0] + 1;
			}
			else {
				//in questo caso lo devo aggiungere 
				double time_ritroso = (from_to[p.arrival_location][places[start_end_routes[2][0] + 1]] / map_airplane[aircraft_code].speed) * 60;
				p.solution_to = start_end_routes[2][0] + 1;
				index_until_updating++;
				places.insert(places.begin() + start_end_routes[2][0] + 1, p.arrival_location);
				refueling.insert(refueling.begin() + start_end_routes[2][0] + 1, map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0] + 1, 0.0);
				weight.insert(weight.begin() + start_end_routes[2][0] + 1, 0.0);
				capacity.insert(capacity.begin() + start_end_routes[2][0] + 1, 0);
				time_arr.insert(time_arr.begin() + start_end_routes[2][0] + 1, time_node - time_ritroso - map_airstrip[p.arrival_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0] + 1, time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[2][0] + 1]; //aggiorno il valore del giorno prima
				start_end_routes[2][1]++;

			}

			if (places[start_end_routes[2][0]] == p.departure_location) {
				p.solution_from = start_end_routes[2][0];
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				time_dep[start_end_routes[2][0]] = time_arr[start_end_routes[2][0] + 1] - time_ritroso;
				if (start_end_routes[2][0] == 0)  time_arr[start_end_routes[2][0]] = time_dep[start_end_routes[2][0]] - map_airstrip[places[start_end_routes[2][0]]].ground_time;
				else time_arr[start_end_routes[2][0]] = time_arr_partenza;
				capacity[start_end_routes[2][0]] = 1;
			}
			else {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				index_until_updating++;
				p.solution_from = start_end_routes[2][0] + 1;

				places.insert(places.begin() + start_end_routes[2][0] + 1, p.departure_location);
				refueling.insert(refueling.begin() + start_end_routes[2][0] + 1, map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0] + 1, 0.0);
				weight.insert(weight.begin() + start_end_routes[2][0] + 1, 0.0);
				capacity.insert(capacity.begin() + start_end_routes[2][0] + 1, 1);
				time_arr.insert(time_arr.begin() + start_end_routes[2][0] + 1, time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0] + 1, time_node - time_ritroso);
				locations_added++;
				index++;
				p.solution_to++;
				time_node = time_arr[start_end_routes[2][0] + 1]; //aggiorno il valore del giorno prima
				start_end_routes[2][1]++;


				//ora devo aggiornare il nodo iniziale
				double time = (from_to[places[start_end_routes[2][0]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				time_dep[start_end_routes[2][0]] = time_node - time;
				if (start_end_routes[2][0] == 0)  time_arr[start_end_routes[2][0]] = time_node - time - map_airstrip[places[start_end_routes[2][0]]].ground_time;



			}



			for (int i = start_end_routes[2][0]; i < index; i++) {
				if (i >= index_until_updating && refueling[i]) break;
				if (refueling[i]) {
					quantity_fuel[i] = map_airplane[aircraft_code].max_fuel;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_before = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed) - p.weight;
					else if (i >= index_until_updating) weight[i] += fuel_before - quantity_fuel[i];
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed);
				}


			}


		}
		else {
			double time_arr_partenza = time_arr[start_end_routes[2][0]];
			time_arr[start_end_routes[2][0]] = time_dep[start_end_routes[2][0]] - map_airstrip[places[start_end_routes[2][0]]].ground_time;

			double time_node = time_arr[start_end_routes[2][0]];
			int location_partenza = places[start_end_routes[2][0]];
			int index_updating = start_end_routes[2][0]; //in questo caso posso aggiornare fuel e peso fino a index updating in quanto so che quello è un deposito
			if (p.arrival_location == places[start_end_routes[2][0]]) {
				p.solution_to = start_end_routes[2][0];
			}
			else {

				double time_ritroso = (from_to[p.arrival_location][places[start_end_routes[2][0]]] / map_airplane[aircraft_code].speed) * 60;
				index_updating++;
				p.solution_to = start_end_routes[2][0];
				places.insert(places.begin() + start_end_routes[2][0], p.arrival_location);
				refueling.insert(refueling.begin() + start_end_routes[2][0], map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[2][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[2][0], 0);
				time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_node - time_ritroso - map_airstrip[p.arrival_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0], time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[2][0]]; //aggiorno il valore del giorno prima
				start_end_routes[2][1]++;



			}

			if (p.departure_location == location_partenza) {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_from = start_end_routes[2][0];
				p.solution_to++;
				index_updating++;
				places.insert(places.begin() + start_end_routes[2][0], p.departure_location);
				refueling.insert(refueling.begin() + start_end_routes[2][0], map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[2][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[2][0], 1);

				if (start_end_routes[2][0] == 0) time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_node - time_ritroso - map_airstrip[location_partenza].ground_time);
				else time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_arr_partenza);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0], time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[2][0]]; //aggiorno il valore del giorno prima
				start_end_routes[2][1]++;


			}
			else {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_from = start_end_routes[2][0] + 1;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin() + start_end_routes[2][0], p.departure_location);
				refueling.insert(refueling.begin() + start_end_routes[2][0], map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[2][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[2][0], 1);
				time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0], time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[2][0]]; //aggiorno il valore del giorno prima
				start_end_routes[2][1]++;


				time_ritroso = (from_to[location_partenza][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin() + start_end_routes[2][0], location_partenza);
				refueling.insert(refueling.begin() + start_end_routes[2][0], map_airstrip[location_partenza].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[2][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[2][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[2][0], 0);
				if (start_end_routes[2][0] == 0) time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_node - time_ritroso - map_airstrip[location_partenza].ground_time);
				else time_arr.insert(time_arr.begin() + start_end_routes[2][0], time_arr_partenza);
				time_dep.insert(time_dep.begin() + start_end_routes[2][0], time_node - time_ritroso);
				locations_added++;
				index++;
				//time_node = time_arr[0]; //aggiorno il valore del giorno prima
				start_end_routes[2][1]++;





			}


			for (int i = start_end_routes[2][0]; i < index; i++) {
				if (i >= index_updating && refueling[i]) break;
				if (refueling[i]) {
					quantity_fuel[i] = map_airplane[aircraft_code].max_fuel;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_before = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed) - p.weight;
					else if (i >= index_updating) weight[i] += fuel_before - quantity_fuel[i];
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed);
				}


			}



		}


	}
	else if (p.giorno == 2) {
		//uguale al caso tre ma fai attenzione agli indici e devi cambiare tutti i fuel dopo fino al primo posto in cui si fa benzina
		if (capacity[start_end_routes[1][0]] == 0) {
			double time_node = time_arr[start_end_routes[1][0] + 1];
			double index_until_updating = start_end_routes[1][0] + 1;
			double time_arr_partenza = time_arr[start_end_routes[1][0]];
			//aggiungo le cose a ritroso
			if (places[start_end_routes[1][0] + 1] == p.arrival_location) {
				p.solution_to = start_end_routes[1][0] + 1;
			}
			else {
				//in questo caso lo devo aggiungere 
				double time_ritroso = (from_to[p.arrival_location][places[start_end_routes[1][0] + 1]] / map_airplane[aircraft_code].speed) * 60;
				p.solution_to = start_end_routes[1][0] + 1;
				index_until_updating++;
				places.insert(places.begin() + start_end_routes[1][0] + 1, p.arrival_location);
				refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, 0.0);
				weight.insert(weight.begin() + start_end_routes[1][0] + 1, 0.0);
				capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 0);
				time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_node - time_ritroso - map_airstrip[p.arrival_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[1][0] + 1]; //aggiorno il valore del giorno prima
				start_end_routes[1][1]++;
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

			}

			if (places[start_end_routes[1][0]] == p.departure_location) {
				p.solution_from = start_end_routes[1][0];
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				time_dep[start_end_routes[1][0]] = time_arr[start_end_routes[1][0] + 1] - time_ritroso;
				if (start_end_routes[1][0] == 0) time_arr[start_end_routes[1][0]] = time_dep[start_end_routes[1][0]] - map_airstrip[places[start_end_routes[1][0]]].ground_time;
				else  time_arr[start_end_routes[1][0]] = time_arr_partenza;
				capacity[start_end_routes[1][0]] = 1;
			}
			else {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				index_until_updating++;
				p.solution_from = start_end_routes[1][0] + 1;
				places.insert(places.begin() + start_end_routes[1][0] + 1, p.departure_location);
				refueling.insert(refueling.begin() + start_end_routes[1][0] + 1, map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0] + 1, 0.0);
				weight.insert(weight.begin() + start_end_routes[1][0] + 1, 0.0);
				capacity.insert(capacity.begin() + start_end_routes[1][0] + 1, 1);
				time_arr.insert(time_arr.begin() + start_end_routes[1][0] + 1, time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[1][0] + 1, time_node - time_ritroso);
				locations_added++;
				index++;
				p.solution_to++;
				time_node = time_arr[start_end_routes[1][0] + 1]; //aggiorno il valore del giorno prima
				start_end_routes[1][1]++;
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

				//ora devo aggiornare il nodo iniziale
				double time = (from_to[places[start_end_routes[1][0]]][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				time_dep[start_end_routes[1][0]] = time_node - time;
				if (start_end_routes[1][0] == 0) time_arr[start_end_routes[1][0]] = time_node - time - map_airstrip[places[start_end_routes[1][0]]].ground_time;



			}



			for (int i = start_end_routes[1][0]; i < index; i++) {
				if (i >= index_until_updating && refueling[i]) break;
				if (refueling[i]) {
					quantity_fuel[i] = map_airplane[aircraft_code].max_fuel;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_before = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed) - p.weight;
					else if (i >= index_until_updating) weight[i] += fuel_before - quantity_fuel[i];
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed);
				}


			}


		}
		else {
			double time_arr_partenza = time_arr[start_end_routes[1][0]];

			time_arr[start_end_routes[1][0]] = time_dep[start_end_routes[1][0]] - map_airstrip[places[start_end_routes[1][0]]].ground_time;
			double time_node = time_arr[start_end_routes[1][0]];
			int location_partenza = places[start_end_routes[1][0]];
			int index_updating = start_end_routes[1][0]; //in questo caso posso aggiornare fuel e peso fino a index updating in quanto so che quello è un deposito
			if (p.arrival_location == places[start_end_routes[1][0]]) {
				p.solution_to = start_end_routes[1][0];
			}
			else {

				double time_ritroso = (from_to[p.arrival_location][places[start_end_routes[1][0]]] / map_airplane[aircraft_code].speed) * 60;
				index_updating++;
				p.solution_to = start_end_routes[1][0];
				places.insert(places.begin() + start_end_routes[1][0], p.arrival_location);
				refueling.insert(refueling.begin() + start_end_routes[1][0], map_airstrip[p.arrival_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[1][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[1][0], 0);
				time_arr.insert(time_arr.begin() + start_end_routes[1][0], time_node - time_ritroso - map_airstrip[p.arrival_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[1][0], time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[1][0]]; //aggiorno il valore del giorno prima
				start_end_routes[1][1]++;
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}


			}

			if (p.departure_location == location_partenza) {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_from = start_end_routes[1][0];
				p.solution_to++;
				index_updating++;
				places.insert(places.begin() + start_end_routes[1][0], p.departure_location);
				refueling.insert(refueling.begin() + start_end_routes[1][0], map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[1][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[1][0], 1);
				if (start_end_routes[1][0] == 0) time_arr.insert(time_arr.begin() + start_end_routes[1][0], time_node - time_ritroso - map_airstrip[location_partenza].ground_time);
				else time_arr.insert(time_arr.begin() + start_end_routes[1][0], time_arr_partenza);
				time_dep.insert(time_dep.begin() + start_end_routes[1][0], time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[1][0]]; //aggiorno il valore del giorno prima
				start_end_routes[1][1]++;
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

			}
			else {
				double time_ritroso = (from_to[p.departure_location][p.arrival_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_from = start_end_routes[1][0] + 1;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin() + start_end_routes[1][0], p.departure_location);
				refueling.insert(refueling.begin() + start_end_routes[1][0], map_airstrip[p.departure_location].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[1][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[1][0], 1);
				time_arr.insert(time_arr.begin() + start_end_routes[1][0], time_node - time_ritroso - map_airstrip[p.departure_location].ground_time);
				time_dep.insert(time_dep.begin() + start_end_routes[1][0], time_node - time_ritroso);
				locations_added++;
				index++;
				time_node = time_arr[start_end_routes[1][0]]; //aggiorno il valore del giorno prima
				start_end_routes[1][1]++;
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}

				time_ritroso = (from_to[location_partenza][p.departure_location] / map_airplane[aircraft_code].speed) * 60;
				p.solution_to++;
				index_updating++;
				places.insert(places.begin() + start_end_routes[1][0], location_partenza);
				refueling.insert(refueling.begin() + start_end_routes[1][0], map_airstrip[location_partenza].fuel);
				quantity_fuel.insert(quantity_fuel.begin() + start_end_routes[1][0], 0.0);
				weight.insert(weight.begin() + start_end_routes[1][0], 0.0);
				capacity.insert(capacity.begin() + start_end_routes[1][0], 0);
				if (start_end_routes[1][0] == 0) time_arr.insert(time_arr.begin() + start_end_routes[1][0], time_node - time_ritroso - map_airstrip[location_partenza].ground_time);
				else time_arr.insert(time_arr.begin() + start_end_routes[1][0], time_arr_partenza);
				time_dep.insert(time_dep.begin() + start_end_routes[1][0], time_node - time_ritroso);
				locations_added++;
				index++;
				//time_node = time_arr[0]; //aggiorno il valore del giorno prima
				start_end_routes[1][1]++;
				if (start_end_routes[2][1] != -1) {
					start_end_routes[2][0]++;
					start_end_routes[2][1]++;
				}




			}

			//cout << "index_updating è: " << index_updating << endl;
			for (int i = start_end_routes[1][0]; i < index; i++) {
				if (i >= index_updating && refueling[i]) break;
				//cout << "sono l'indice: " << i << endl;
				if (refueling[i]) {
					quantity_fuel[i] = map_airplane[aircraft_code].max_fuel;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel - p.weight;
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - map_airplane[aircraft_code].max_fuel;
				}
				else {
					double fuel_consumed = from_to_fuelConsumed[aircraft_code][places[i - 1]][places[i]];
					double fuel_before = quantity_fuel[i];
					quantity_fuel[i] = quantity_fuel[i - 1] - fuel_consumed;
					if (i == p.solution_from) weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed) - p.weight;
					else if (i >= index_updating) weight[i] += fuel_before - quantity_fuel[i];
					else weight[i] = map_airplane[aircraft_code].weight_fuel_people - (quantity_fuel[i - 1] - fuel_consumed);
				}


			}



		}


	}



	//metti codice per pescare fuel e per ridare fuel se necessario 
	//pezzo che pesca fuel;
	double add_fuel = 0;
	int index_weight_neg = -1;
	for (int j = 0; j < index; j++) {
		//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
		if (weight[j] < 0) {
			//cout << " Weigth negativo in " << j << weight[j] << endl;
			add_fuel = weight[j];
			index_weight_neg = j;
			int index_refueling = index_weight_neg;
			for (int i = index_weight_neg; i >= 0; i--) {
				if (refueling[i]) {
					index_refueling = i;
					break;
				}
			}
			//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
			for (int t = index_refueling; t < index; t++) {
				if (refueling[t] && t != index_refueling) break;
				//cout << " Aggiorno nodo " << t << endl;
				quantity_fuel[t] += add_fuel;
				weight[t] -= add_fuel;
				//cout << " Weight dopo " << weight[t] << endl;
			}
		}
	}


	//pezzo che ridà fuel
	for (int k = 0; k < start_end_routes[p.giorno - 1][1]; k++) {  //qua prima avevo messo start_end_routes[p.giorno - 1][0] ma ci vorrebbe il giorno prima a quello che sto considerando se c'è, per renderlo più ottimizzato, per il momento metto 0
		if (refueling[k] && quantity_fuel[k] < map_airplane[aircraft_code].max_fuel) {
			//cout << " Sto valutando il caso del nodo " << k << endl;
			int index_updating_from = k;
			int index_updating_to = index;  //qua prima c'era -1
			int Node_min = k;
			double min_weight = weight[k];
			for (int i = k + 1; i < index; i++) {
				//cout << " Sto guardando il nodo " << i << endl;
				//cout << " Ho passato IF di controllo " << endl;
				if (refueling[i]) {
					index_updating_to = i;
					break;
				}
				if (weight[i] < min_weight) {
					min_weight = weight[i];
					Node_min = i;
				}

			}
			double Fuel_before = quantity_fuel[index_updating_from];
			quantity_fuel[index_updating_from] = min(map_airplane[aircraft_code].max_fuel, quantity_fuel[index_updating_from] + min_weight);
			weight[index_updating_from] -= (quantity_fuel[index_updating_from] - Fuel_before);
			for (int i = index_updating_from + 1; i < index_updating_to; i++) {
				quantity_fuel[i] += (quantity_fuel[index_updating_from] - Fuel_before);
				weight[i] -= (quantity_fuel[index_updating_from] - Fuel_before);
			}

		}
	}






	for (Passenger& pi : passengers_in_route) {
		if (pi.giorno >= p.giorno) {
			pi.solution_from += locations_added;
			pi.solution_to += locations_added;
		}

	}




}





Route::~Route()
{
}
