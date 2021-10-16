#ifndef FILL_H
#define FILL_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>


#include "Utils.h"
#include "Airplane.h"
#include "Airstrip.h"
#include "Fuel.h"
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
		//cout << e[0] << " - ";
		//cout << e[9] << endl;
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

void fill_from_to_fuel_consumed(vector<vector<vector<double>>>& from_to_fuel_consumed, vector<vector<double>>& from_to, vector<Airplane> airplanes) {


	from_to_fuel_consumed.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++) {
		from_to_fuel_consumed[i].resize((size_t)numero_airstrip_const);
		for (int j = 0; j < numero_airstrip_const; j++) {
			from_to_fuel_consumed[i][j].resize((size_t)numero_airstrip_const);
		}
	}



	for (Airplane a : airplanes) {

		for (int i = 1; i < numero_airstrip_const; i++) {
			for (int j = 1; j < numero_airstrip_const; j++) {
				double time_fly = from_to[i][j] / a.speed;
				double fuel_consumed = 0.0;
				if (time_fly <= 1) {
					fuel_consumed = time_fly * a.fuel_burn_first;
				}
				else {
					fuel_consumed = a.fuel_burn_first + (time_fly - 1) * a.fuel_burn_second;
				}
				from_to_fuel_consumed[a.code][i][j] = fuel_consumed;
				//cout << " da/a: " << x.second << " time fly: " << time_fly << " fuel consumed: " << fuel_consumed << endl;
			}
		}




	}




}

map <int, Passenger> fillMapPassenger(vector<Passenger> passengers) {
	map <int, Passenger> map_pass;
	for (Passenger p : passengers)
	{
		map_pass.insert(make_pair(p.pnr, p));
	}



	return  map_pass;
}

void fillMatrix(vector<vector<double>>& from_to, string file_input, vector<Airstrip> airstrips) {


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
			//cout << "String: " << e[i] << "\n";
			//cout << "Double: " << setprecision(14) << d << endl;
			from_to.insert(make_pair(airstrips[index_row].code_string + ";" + airstrips[i].code_string, d));
		}
		index_row++;
	}
	file.close();

	return from_to;
}


void check_mappa_from_to(map<string, double> from_to_string, vector<vector<double>>& from_to, map<string, int> legenda) {


	for (auto x : from_to_string) {
		cout << "le due localita' sono: " << x.first << endl;
		string loc1 = split(x.first, ';')[0];
		string loc2 = split(x.first, ';')[1];

		cout << "con stringa: " << x.second << endl;
		cout << "con matrix: " << from_to[legenda[loc1]][legenda[loc2]] << endl;

		if (x.second != from_to[legenda[loc1]][legenda[loc2]]) {
			cout << "erroreeeeeeeeeeeeeeeeeeeeeeeee" << endl;
			system("pause");
		}





	}







}

map<string, double> fill_from_to_fuel_consumed_string(map<string, double>& from_to, vector<Airplane> airplanes) {

	map<string, double> from_to_fuel_consumed;

	for (Airplane a : airplanes) {
		for (auto x : from_to) {
			double time_fly = x.second / a.speed;
			double fuel_consumed = 0.0;
			if (time_fly <= 1) {
				fuel_consumed = time_fly * a.fuel_burn_first;
			}
			else {
				fuel_consumed = a.fuel_burn_first + (time_fly - 1) * a.fuel_burn_second;
			}
			from_to_fuel_consumed.insert(make_pair(a.code_company + ";" + x.first, fuel_consumed));
			//cout << " da/a: " << x.second << " time fly: " << time_fly << " fuel consumed: " << fuel_consumed << endl;
		}
	}
	return from_to_fuel_consumed;
}


void check_mappa_from_to_fuel_consumed(map<string, Airplane> map_airplane, map<string, double> from_to_fuel_consumed_string, vector<vector<vector<double>>>& from_to_FuelConsumed, map<string, int> legenda) {


	for (auto x : from_to_fuel_consumed_string) {
		cout << "l'aereo e': " << split(x.first, ';')[0] << endl;;
		cout << "le due localita' sono: " << x.first << endl;
		string loc1 = split(x.first, ';')[1];
		string loc2 = split(x.first, ';')[2];

		cout << "con stringa: " << x.second << endl;
		cout << "con matrix: " << from_to_FuelConsumed[map_airplane[split(x.first, ';')[0]].code][legenda[loc1]][legenda[loc2]] << endl;

		if (x.second != from_to_FuelConsumed[map_airplane[split(x.first, ';')[0]].code][legenda[loc1]][legenda[loc2]]) {
			cout << "erroreeeeeeeeeeeeeeeeeeeeeeeee" << endl;
			system("pause");
		}





	}







}

void check_mappa_location_fuel(std::map<std::string, Airplane> map_airplane, map<string, double> location_fuel_string, vector<std::vector<double>>& location_fuel, std::map<std::string, int> legenda) {


	for (auto x : location_fuel_string) {
		std::cout << "l'aereo e': " << split(x.first, '/')[0] << std::endl;;
		std::string loc1 = split(x.first, '/')[1];
		std::cout << "la localita" << loc1 << std::endl;


		std::cout << "con stringa: " << x.second << std::endl;
		std::cout << "con matrix: " << location_fuel[map_airplane[split(x.first, '/')[0]].code][legenda[loc1]] << std::endl;

		if (x.second != location_fuel[map_airplane[split(x.first, '/')[0]].code][legenda[loc1]]) {
			std::cout << "erroreeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
			system("pause");
		}





	}

}


map < int, int > fill_to_closeness_fuel(vector<vector<double>>& from_to, vector<Airstrip>& airstrips) {
	map <int, int> to_closeness_fuel;

	for (const Airstrip& a : airstrips) {
		double best_distance = DBL_MAX;
		int best_location = -1;
		if (a.fuel) to_closeness_fuel.insert(make_pair(a.code, a.code));
		else {
			for (const Airstrip& a1 : airstrips) {
				if (a.code != a1.code) {
					if (a1.fuel) {
						if (best_distance > from_to[a.code][a1.code]) {
							best_distance = from_to[a.code][a1.code];
							best_location = a1.code;
						}
					}
				}
			}

			to_closeness_fuel.insert(make_pair(a.code, best_location));
		}
	}

	return to_closeness_fuel;
}

map<int, int> fill_location_request(vector<Passenger>& passengers) {

	map<int, int> mappa;

	for (const Passenger& p : passengers) {
		if (mappa.count(p.departure_location) >= 1) {
			mappa[p.departure_location]++;
		}
		else {
			mappa.insert(make_pair(p.departure_location, 1));
		}
	}


	return mappa;
}


//per il tratto finale
void fillLocation_fuel(vector<vector<double>>& risultato, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<vector<double>>& from_to, map<int, Airstrip>& map_airstrip) {


	risultato.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++)
		risultato[i].resize((size_t)numero_airstrip_const);



	for (Airplane f : airplanes) {
		for (Airstrip a : airstrips) {
			if (a.fuel) {
				risultato[f.code][a.code] = 0.0;
				//risultato.insert(make_pair(f.code + "/" + a.code, 0.0));
			}
			else {
				double fuel_needed = 0;
				double time_fly = from_to[a.code][location_closest_with_fuel(a.code, from_to, map_airstrip)] / f.speed;
				//double time_fly = from_to[a.code + ";" + location_closest_with_fuel(a.code, from_to, map_airstrip)] / f.speed;
				if (time_fly <= 1) {
					fuel_needed = time_fly * f.fuel_burn_first;
				}
				else {
					fuel_needed = f.fuel_burn_first + (time_fly - 1) * f.fuel_burn_second;
				}

				risultato[f.code][a.code] = fuel_needed;
				//risultato.insert(make_pair(f.code + "/" + a.code, fuel_needed));
			}
		}
	}




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
				double time_fly = from_to[a.code_string + ";" + location_closest_with_fuel_string(a.code_string, from_to, map_airstrip)] / f.speed;
				if (time_fly <= 1) {
					fuel_needed = time_fly * f.fuel_burn_first;
				}
				else {
					fuel_needed = f.fuel_burn_first + (time_fly - 1) * f.fuel_burn_second;
				}
				risultato.insert(make_pair(f.code_company + "/" + a.code_string, fuel_needed));
			}
		}
	}


	return risultato;

}



#endif