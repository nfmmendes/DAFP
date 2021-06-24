#include <iostream>
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include "Util.h"


void stampo_caso_strano_single_tempi(Route r, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane) {

	for (int i = 1; i < r.index; i++) {

		if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
			cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece ? " << r.time_arr[i] << endl;
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

		if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
			cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece ? " << r.time_arr[i] << endl;
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
			if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
				cout << "dovrebbe essere: " << fuel_consumed << " mentre invece ? " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
				r.print();
				system("pause");
			}
		}

	}


	for (int i = 0; i < r.index - 1; i++) {
		if ((r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1]) || (r.weight[i] < -1)) {
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
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
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



void stampo_caso_strano_destroy(vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa) {

	map<int, int> test_codici;
	for (auto x : routes) {
		if (test_codici.count(x.aircraft_code) >= 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono pi? codici aereo uguali" << endl;
			for (Route r : routes) cout << "codice: " << r.aircraft_code << endl;
			system("pause");
			cin.get();
		}
		else {
			test_codici.insert(make_pair(x.aircraft_code, 1));
		}
	}


	for (Route r : routes) {

		if (r.index > (int)r.places.size()) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
			r.print();
			system("pause");
			cin.get();
		}


		for (Passenger p : r.passengers_in_route) {
			if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
				p.print();
				cout << "nella route: " << endl;
				r.print();
				system("pause");
				cin.get();
			}

			if (p.solution_from >= p.solution_to) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
				p.print();
				cout << "nella route: " << endl;
				r.print();
				system("pause");
				cin.get();
			}

		}



		for (int i = 1; i < r.index; i++) {

			if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO che un problema con la capacita nel nodo: " << i << endl;
				r.print();
				system("pause");
				cin.get();
			}



			if (r.time_arr[i] <= (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece ? " << r.time_arr[i] << endl;
				r.print();
				system("pause");
				cin.get();
			}


			double fly_time = (from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece ? " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					r.print();
					system("pause");
					cin.get();
				}
			}
		}


		for (int i = 0; i < r.index - 1; i++) {
			if ((r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1]) || r.weight[i] < -0.005) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
				r.print();
				system("pause");
				cin.get();
			}


			if (r.places[i] == r.places[i + 1]) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
				r.print();
				system("pause");
				cin.get();
			}
		}

		for (int i = 0; i < r.index; i++) {
			double peso_arco_passeggeri = 0.0;
			for (Passenger p : r.passengers_in_route) {
				if (p.solution_from <= i && p.solution_to > i) {
					peso_arco_passeggeri += p.weight;
				}
			}

			if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
				r.print();
				system("pause");
				cin.get();
			}
		}


		for (int i = 0; i < r.index - 1; i++) {
			double minimo_peso = r.weight[i];
			if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
				for (int j = i + 1; j < r.index; j++) {
					if (r.refueling[j]) break;
					if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
				}
				if (minimo_peso >= 1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
					r.print();
					system("pause");
					cin.get();
				}

			}
		}
	}
}




//funzione che TUTTI GLI ERRORI
void stampo_caso_strano(map<int, Airstrip> map_airstrip, int peso_TW, int peso_intermediate_stop, vector<vector<double>> from_to_costi, vector<vector<vector<double>>> from_to_fuel_costi, vector<Passenger> passengers, vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa) {

	int numero = 0;
	for (auto z : routes) numero += z.passengers_in_route.size();
	if (numero != (int)passengers.size() && numero > 0) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono meno/piu' passeggeri" << endl;
		cout << numero << " -- " << passengers.size() << endl;
		system("pause");
		cin.get();
	}



	map<int, int> test_codici;
	for (auto x : routes) {
		if (test_codici.count(x.aircraft_code) >= 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono pi? codici aereo uguali" << endl;
			for (Route r : routes) cout << "codice: " << r.aircraft_code << endl;
			system("pause");
			cin.get();
		}
		else {
			test_codici.insert(make_pair(x.aircraft_code, 1));
		}
	}


	for (Route r : routes) {

		if (r.index > (int)r.places.size()) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
			r.print();
			system("pause");
			cin.get();
		}


		for (Passenger p : r.passengers_in_route) {
			if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
				p.print();
				cout << "nella route: " << endl;
				r.print();
				system("pause");
				cin.get();
			}

			if (p.solution_from >= p.solution_to) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
				p.print();
				cout << "nella route: " << endl;
				r.print();
				system("pause");
				cin.get();
			}

		}



		for (int i = 1; i < r.index; i++) {


			if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO che un problema con la capacita nel nodo: " << i << endl;
				r.print();
				system("pause");
				cin.get();
			}



			if (r.time_arr[i] <= (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece ? " << r.time_arr[i] << endl;
				r.print();
				system("pause");
				cin.get();
			}


			double fly_time = (from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece ? " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					r.print();
					system("pause");
					cin.get();
				}
			}

		}


		for (int i = 0; i < r.index - 1; i++) {
			if ((r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1]) || r.weight[i] < -0.005) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
				r.print();
				system("pause");
				cin.get();
			}


			if (r.places[i] == r.places[i + 1]) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
				r.print();
				system("pause");
				cin.get();
			}
		}

		for (int i = 0; i < r.index; i++) {
			double peso_arco_passeggeri = 0.0;
			for (Passenger p : r.passengers_in_route) {
				if (p.solution_from <= i && p.solution_to > i) {
					peso_arco_passeggeri += p.weight;
				}
			}

			if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
				r.print();
				system("pause");
				cin.get();
			}
		}


		for (int i = 0; i < r.index - 1; i++) {
			double minimo_peso = r.weight[i];
			if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
				for (int j = i + 1; j < r.index; j++) {
					if (r.refueling[j]) break;
					if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
				}
				if (minimo_peso >= 1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
					r.print();
					system("pause");
					cin.get();
				}

			}
		}

	}
}

