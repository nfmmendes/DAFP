#include "Route.h"
#include "Util.h"

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

	return result;

}

string location_closest_with_fuel_string(string location, map<string, double> from_to, map<string, Airstrip>& map_airstrip) {
	string result = "";
	double value = DBL_MAX;

	for (auto x : from_to) {
		if (split(x.first, ';')[0] == location && x.second < value && x.second != 0 && map_airstrip[split(x.first, ';')[1]].fuel) {
			result = split(x.first, ';')[1];
			value = x.second;
		}
	}

	return result;

}