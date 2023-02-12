#ifndef FILL_H
#define FILL_H

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include "Airstrip.h"
#include "Fuel.h"
#include "Route.h"
#include "Util.h"

using std::string;
using std::ifstream;
using std::vector;

//per il tratto finale
void fillLocation_fuel(double2DVector& result, vector<Airstrip> airstrips, vector<Airplane> airplanes, double2DVector& from_to, map<int, Airstrip>& map_airstrip) {

	result.resize((size_t)numero_airplane_const);
	for (unsigned int i = 0; i < numero_airplane_const; i++)
		result[i].resize((size_t)numero_airstrip_const);

	for (Airplane f : airplanes) {
		for (Airstrip a : airstrips) {
			if (a.fuel) {
				result[f.code][a.code] = 0.0;
			}
			else {
				double fuel_needed = 0;
				double time_fly = f.travelTime(from_to[a.code][location_closest_with_fuel(a.code, from_to, map_airstrip)]);

				fuel_needed = min(1.0, time_fly/60) * f.fuel_burn_first + max(time_fly/60 - 1, 0.0) * f.fuel_burn_second;
				result[f.code][a.code] = fuel_needed;
			}
		}
	}
}


vector<Airplane> fillAirplane(string file_input, map<string, int> legenda) {

	vector<Airplane> airplanes;
	std::ifstream  file;
	file.open(file_input);
	//check for error
	if (file.fail()) {
		cerr << "Error Opening File Airplane.csv" << endl;
		exit(1);
	}
	//read the file untill the end
	while (!file.eof()) {
		string row;
		std::getline(file, row);
		vector<string> e = split(row, ';');
		// TODO: Improve this
		Airplane a(stoi(e[13]), e[1], stoi(e[6]), stod(e[5]), e[0], stoi(e[3]), stoi(e[4]), legenda[e[7]], stod(e[10]), stoi(e[8]), stoi(e[9]), stod(e[11]), stod(e[12]));
		airplanes.push_back(a);
	}
	file.close();

	return airplanes;
};

map<string, int> fillLegenda(string file_input, map<int, string>& legenda_ritroso) {

	map<string, int> legenda;

	ifstream  file;
	file.open(file_input);
	//check for error
	if (file.fail()) {
		cerr << "Error Opening File legenda.csv" << endl;
		exit(1);
	}
	//read the file untill the end
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		legenda.insert(make_pair(e[0], stoi(e[1])));
		legenda_ritroso.insert(make_pair(stoi(e[1]), e[0]));
	}
	file.close();

	return legenda;
};

//function that read the file Airstrip and create a vector of them
vector<Airstrip> fillAirstrip(string file_input, map<string, int> legenda) {

	vector<Airstrip> airstrips;
	ifstream file;
	file.open(file_input);
	
	if (file.fail()) {
		cerr << "Error Opening File Airstrip.csv" << endl;
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		bool fuel = false;
		if (e[6] == "1") fuel = true;
		Airstrip a(legenda[e[1]], e[0], e[1], stoi(e[2]), stoi(e[3]), stod(e[4]), stod(e[5]), fuel, stod(e[7]));
		airstrips.push_back(a);
	}
	file.close();

	return airstrips;
}

//function that read the file instance and create a vector of passenger
vector<Passenger> fillPassenger(string file_input, map<string, int> legenda) {

	vector<Passenger> passengers;
	ifstream file;
	file.open(file_input);
	
	if (file.fail()) {
		cerr << "Error Opening File Passenger.csv" << endl;
		exit(1);
	}
	
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;

		if (e[9] != "Charter") {
			vector<string> dep_min = split(e[9], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[10], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(legenda[e[3]], legenda[e[4]], stoi(e[0]), e[1], e[3], e[4], e[5], e[6], e[7], e[8], min_dep, min_arr);
		//TODO: Uncomment later
		//p.pnr_group = e[11];
		cout << e[0] << " ";
		passengers.push_back(p);

	}
	file.close();

	return passengers;
}

//function that read the file instance and create a vector of routes
vector<Route> fillRoute(string file_input) {

	vector<Route> routes;
	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File" << file_input << endl;
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		Route route(0, vector<Passenger>());

		if (e.size() >= 2) {
			route.aircraft_code_company_comparison = e[1];
			for (unsigned int i = 2; i < e.size() - 1; i = i + 2) {
				route.addPlace_companySolution(e[i], stoi(e[i + 1]));
			}
		}
		routes.push_back(route);
	}
	
	file.close();

	return routes;
}

void fill_from_to_fuel_consumed(double3DVector& from_to_fuel_consumed, vector<vector<double>>& from_to, vector<Airplane> airplanes) {

	from_to_fuel_consumed.resize((size_t)numero_airplane_const);
	for (unsigned int i = 0; i < numero_airplane_const; i++) {
		from_to_fuel_consumed[i].resize((size_t)numero_airstrip_const);
		for (unsigned int j = 0; j < numero_airstrip_const; j++) {
			from_to_fuel_consumed[i][j].resize((size_t)numero_airstrip_const);
		}
	}

	for (Airplane& airplane : airplanes) {

		for (unsigned int i = 1; i < numero_airstrip_const; i++) {
			for (unsigned int j = 1; j < numero_airstrip_const; j++) {
				double time_fly = airplane.travelTime(from_to[i][j]);
				double fuel_consumed = 0.0;
				if (time_fly >= 60) {
					fuel_consumed = time_fly/60 * airplane.fuel_burn_first;
				}
				else {
					fuel_consumed = airplane.fuel_burn_first + (time_fly/60 - 1) * airplane.fuel_burn_second;
				}
				from_to_fuel_consumed[airplane.code][i][j] = fuel_consumed;
			}
		}
	}
}

void fillMatrix(vector<vector<double>>& from_to, string file_input, const vector<Airstrip>& airstrips) {

	from_to.resize((size_t)numero_airstrip_const);
	for (unsigned int i = 0; i < numero_airstrip_const; i++)
		from_to[i].resize((size_t)numero_airstrip_const);


	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File Matrix.csv" << endl;
		exit(1);
	}

	unsigned int index_row = 0;
	while (!file.eof()) {
		std::string row;
		getline(file, row);
		std::vector<std::string> e = split(row, ';');
		for (unsigned int i = 0; i < e.size(); i++) {
			std::istringstream is(e[i]);
			double d;
			is >> d;
			from_to[airstrips[index_row].code][airstrips[i].code] = d;
			//from_to.insert(make_pair(airstrips[index_row].code + ";" + airstrips[i].code, d));
		}
		index_row++;
	}
	file.close();
}

std::map<std::string, double> fillMatrixCompany(string file_input, vector<Airstrip> airstrips) {

	map<string, double> from_to;

	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File Matrix.csv" << endl;
		exit(1);
	}
	int index_row = 0;
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		for (unsigned int i = 0; i < (int)e.size(); i++) {
			istringstream is(e[i]);
			double d;
			is >> d;
			from_to.insert(std::make_pair(airstrips[index_row].code_string + ";" + airstrips[i].code_string, d));
		}
		index_row++;
	}
	file.close();

	return from_to;
}

map<string, double> fillLocation_fuel_string(vector<Airstrip> airstrips, vector<Airplane> airplanes, map<string, double>& from_to, map<string, Airstrip>& map_airstrip) {

	map <string, double> risultato;
	for (Airplane f : airplanes) {
		for (Airstrip a : airstrips) {
			if (a.fuel) {
				risultato.insert(make_pair(f.code_company + "/" + a.code_string, 0.0));
			}
			else {
				double fuel_needed = 0;
				double time_fly = f.travelTime(from_to[a.code_string + ";" + location_closest_with_fuel_string(a.code_string, from_to, map_airstrip)]);
				if (time_fly/60 <= 1) {
					fuel_needed = time_fly/60 * f.fuel_burn_first;
				}
				else {
					fuel_needed = f.fuel_burn_first + (time_fly/60 - 1) * f.fuel_burn_second;
				}
				risultato.insert(make_pair(f.code_company + "/" + a.code_string, fuel_needed));
			}
		}
	}
	return risultato;
}

#endif