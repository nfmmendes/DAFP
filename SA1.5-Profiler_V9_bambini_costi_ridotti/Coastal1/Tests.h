#include <iostream>
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include "Util.h"


void stampo_caso_strano_single_tempi(Route r, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane) {

	for (int i = 1; i < r.index; i++) {

		if (r.arrival[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.departure[i - 1] - 1) || r.arrival[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.departure[i - 1] + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
			cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.departure[i - 1]) << " mentre invece ? " << r.arrival[i] << endl;
			r.print();
			system("pause");
		}
	}
}


void stampo_caso_strano_single_all(Route r, vector<vector<double>>& from_to, map<int, Airplane> map_airplane) {




	for (Passenger p : r.passengers_in_route) {

		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			cout << " Sto chiamando questo codice dentro a Update di Route " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.print();
			system("pause");
		}
	}

	for (int i = 1; i < r.index; i++) {

		if (r.arrival[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.departure[i - 1] - 1) || r.arrival[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.departure[i - 1] + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
			cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.departure[i - 1]) << " mentre invece ? " << r.arrival[i] << endl;
			r.print();
			system("pause");
		}

		double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
		double fuel_consumed = 0.0;
		if (fly_time <= 1) {
			fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
		}
		else {
			fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
		}

		if (!r.refueling[i]) {
			if ((r.fuel[i - 1] - r.fuel[i]) <= (fuel_consumed - 1) || (r.fuel[i - 1] - r.fuel[i]) >= (fuel_consumed + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
				cout << "dovrebbe essere: " << fuel_consumed << " mentre invece ? " << (r.fuel[i - 1] - r.fuel[i]) << endl;
				r.print();
				system("pause");
			}
		}
	}

	for (int i = 0; i < r.index - 1; i++) {
		if ((r.fuel[i] < r.fuel[i + 1] && !r.refueling[i + 1]) || (r.weights[i] < -1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
			r.print();
			system("pause");
		}


		if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PI? POSTI UGUALI " << endl;
			r.print();
			system("pause");
		}


	}


	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weights[i];
		if (r.fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weights[j] < minimo_peso) minimo_peso = r.weights[j];
			}
			if (minimo_peso >= 1) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
				r.print();
				system("pause");
			}

		}
	}

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

void check_mappa_location_fuel(map<string, Airplane> map_airplane, map<string, double> location_fuel_string, vector<vector<double>>& location_fuel, map<string, int> legenda) {


	for (auto x : location_fuel_string) {
		cout << "l'aereo e': " << split(x.first, '/')[0] << endl;;
		string loc1 = split(x.first, '/')[1];
		cout << "la localit? ? " << loc1 << endl;


		cout << "con stringa: " << x.second << endl;
		cout << "con matrix: " << location_fuel[map_airplane[split(x.first, '/')[0]].code][legenda[loc1]] << endl;

		if (x.second != location_fuel[map_airplane[split(x.first, '/')[0]].code][legenda[loc1]]) {
			cout << "erroreeeeeeeeeeeeeeeeeeeeeeeee" << endl;
			system("pause");
		}
	}
}
