#ifndef FUEL_H
#define FUEL_H

#include <iostream>
#include <map>

#include "Airstrip.h"
#include "Route.h"
#include "Utils.h"


//data una location mi restituisce la location pi� vicina
int location_closest_with_fuel(int location, vector<vector<double>>& from_to, map<int, Airstrip>& map_airstrip) {
	int result = -1;
	double value = DBL_MAX;

	for (int i = 1; i < numero_airstrip_const; i++) {
		for (int j = 1; j < numero_airstrip_const; j++) {
			if (i == location && from_to[i][j] < value && map_airstrip[j].fuel) {
				result = j;
				value = from_to[i][j];
			}
		}
	}



	/*for (auto x : from_to) {
		if (split(x.first, ';')[0] == location && x.second < value && x.second != 0 && map_airstrip[split(x.first, ';')[1]].fuel) {
			result = split(x.first, ';')[1];
			value = x.second;
		}
	}*/


	return result;

}


string location_closest_with_fuel_string(string location, map<string, double> from_to, map<string, Airstrip>& map_airstrip) {
	std::string result = "";
	double value = DBL_MAX;

	for (auto x : from_to) {
		if (split(x.first, ';')[0] == location && x.second < value && x.second != 0 && map_airstrip[split(x.first, ';')[1]].fuel) {
			result = split(x.first, ';')[1];
			value = x.second;
		}
	}


	return result;

}


void fuel_min_80_major_places(vector<vector<double>>& mappa_da_tornare, vector<vector<vector<double>>>& from_to_FuelConsumed, map<int, int>& location_request, vector<Airplane>& airplanes, vector<Airstrip>& airstrips) {

	//ho inizializato la mappa
	mappa_da_tornare.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++)
		mappa_da_tornare[i].resize((size_t)numero_airstrip_const);




	map <int, int> map_support;
	for (auto x : location_request) {
		cout << x.first << " ---> " << x.second << endl;
		map_support.insert(make_pair(x.second, x.first));
	}

	vector <int> best_places;
	int index = 0;
	double value = round(map_support.size() * 0.2);
	for (auto x : map_support) {
		//cout << x.first << " ***---> " << x.second << endl;
		if (index >= value) best_places.push_back(x.second);
		index++;
	}
	for (auto x : best_places) cout << " - " << x << endl;


	//codice che crea la mappa
	for (const Airstrip& a : airstrips) {
		double best_fuel = 0.0;
		int destination = -1;
		for (int a1 : best_places) {
			if (best_fuel < from_to_FuelConsumed[1][a.code][a1]) {
				best_fuel = from_to_FuelConsumed[1][a.code][a1];
				destination = a1;
			}
		}
		for (const Airplane& f : airplanes) {
			mappa_da_tornare[f.code][a.code] = from_to_FuelConsumed[f.code][a.code][destination];
			//mappa_da_tornare.insert(make_pair(f.code + ";" + a.code, from_to_FuelConsumed[f.code + ";" + a.code + ";" + destination]));
		}
	}

	//return mappa_da_tornare;
}


#endif