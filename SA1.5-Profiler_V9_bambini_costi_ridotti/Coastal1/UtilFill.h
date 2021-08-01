#ifndef UTIL_FILLS
#define UTIL_FILLS

#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"

//function that read the file Airplane and create a vector of them
vector<Airplane> fillAirplane(string file_input, map<string, int> legenda) {

	vector<Airplane> airplanes;
	ifstream  file;
	file.open(file_input);
	//check for error
	if (file.fail()) {
		cerr << "Error Opening File Airplane.csv" << endl;
		exit(1);
	}
	//read the file untill the end
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
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
		cerr << "Error Opening File Airstrip.csv" << endl;
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
		p.pnr_group = e[11];

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
		vector<Passenger> Pass;
		vector<string> e = split(row, ';');
		Route r(0, Pass);
		r.aircraft_code_company_comparison = e[1];
		for (int i = 2; i < (int)(e.size() - 1); i = i + 2) {
			r.addPlace_companySolution(e[i], stoi(e[i + 1]));
		}
		routes.push_back(r);
	}
	file.close();

	return routes;
}

void fill_from_to_fuel_consumed(Double3DVector& from_to_fuel_consumed, const vector<vector<double>>& from_to, const vector<Airplane>& airplanes) {

	//TODO: TRANSFORM IT IN A SINGLE LOOP
	from_to_fuel_consumed.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++) {
		from_to_fuel_consumed[i].resize((size_t)numero_airstrip_const);
		for (int j = 0; j < numero_airstrip_const; j++) {
			from_to_fuel_consumed[i][j].resize((size_t)numero_airstrip_const);
		}
	}


	double time_fly, fuel_consumed;
	for (const Airplane& airplane : airplanes) {

		for (int i = 1; i < numero_airstrip_const; i++) {
			for (int j = 1; j < numero_airstrip_const; j++) {
				time_fly = from_to[i][j] / airplane.speed;
				fuel_consumed = (time_fly <= 1) ? time_fly * airplane.fuel_burn_first : airplane.fuel_burn_first + (time_fly - 1) * airplane.fuel_burn_second;
				from_to_fuel_consumed[airplane.code][i][j] = fuel_consumed;
			}
		}
	}

}

map <int, Passenger> fillMapPassenger(vector<Passenger> passengers) {
	map <int, Passenger> map_pass;
	for (Passenger p : passengers)
		map_pass.insert(make_pair(p.pnr, p));

	return  map_pass;
}

void fillMatrix(vector<vector<double>>& from_to, string file_input, const vector<Airstrip>& airstrips) {


	from_to.resize((size_t)numero_airstrip_const);
	for (int i = 0; i < numero_airstrip_const; i++)
		from_to[i].resize((size_t)numero_airstrip_const);


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
		for (int i = 0; i < (int)e.size(); i++) {
			istringstream is(e[i]);
			double d;
			is >> d;
			from_to[airstrips[index_row].code][airstrips[i].code] = d;
			//from_to.insert(make_pair(airstrips[index_row].code + ";" + airstrips[i].code, d));
		}
		index_row++;
	}
	file.close();


}

map<string, double> fillMatrixCompany(string file_input, vector<Airstrip> airstrips) {

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
		for (int i = 0; i < (int)e.size(); i++) {
			istringstream is(e[i]);
			double d;
			is >> d;
			from_to.insert(make_pair(airstrips[index_row].code_string + ";" + airstrips[i].code_string, d));
		}
		index_row++;
	}
	file.close();

	return from_to;
}




#endif
