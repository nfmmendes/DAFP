#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <random>
#include "Airplane.h"
#include "Node.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include <float.h>
#include <chrono>
#include "Flightleg.h"
#include "Model_Connection.h"
#include "Model_Cplex.h"

using namespace std;

//const int numero_airstrip_const = 58;
//const int numero_airplane_const = 36;

template<typename type>
struct MyCOMP {
	bool operator() (
		const type& first, const type& second) const {
		return first > second;
	}

};

template<typename type>
struct MyCOMP1 {
	bool operator() (
		const type& first, const type& second) const {
		return first < second;
	}

};


double Sum(vector<double> Weight) {
	double sum = 0;
	for (double i : Weight) sum += i;
	return sum;
}

double Accumulated(int j, vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) sum += i;
	for (int i = 0; i <= j; i++) accum += ((Weight[i]) / sum);
	return accum;
}

double Ob_Funct_Improvement(double newsolution, double oldsolution) {
	double rate_max_improvement = 10000;
	if (newsolution >= oldsolution) {
		return 0;
	}
	else {
		return ((oldsolution - newsolution) / rate_max_improvement);
	}
}

double Time_Spend(double time) {
	double rate_time = 20;
	return time / rate_time;

}


void stampo_caso_strano_single_tempi(Route r, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane) {





	for (int i = 1; i < r.index; i++) {

		if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
			cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece � " << r.time_arr[i] << endl;
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
			cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece � " << r.time_arr[i] << endl;
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
				cout << "dovrebbe essere: " << fuel_consumed << " mentre invece � " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
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
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PI� POSTI UGUALI " << endl;
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


//function of split
vector<string> split(string stringa, char separatore) {
	vector<string> words;
	stringstream ss(stringa);
	string individual_string;
	while (getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
}

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

void check_mappa_location_fuel(map<string, Airplane> map_airplane, map<string, double> location_fuel_string, vector<vector<double>>& location_fuel, map<string, int> legenda) {


	for (auto x : location_fuel_string) {
		cout << "l'aereo e': " << split(x.first, '/')[0] << endl;;
		string loc1 = split(x.first, '/')[1];
		cout << "la localit� � " << loc1 << endl;


		cout << "con stringa: " << x.second << endl;
		cout << "con matrix: " << location_fuel[map_airplane[split(x.first, '/')[0]].code][legenda[loc1]] << endl;

		if (x.second != location_fuel[map_airplane[split(x.first, '/')[0]].code][legenda[loc1]]) {
			cout << "erroreeeeeeeeeeeeeeeeeeeeeeeee" << endl;
			system("pause");
		}





	}







}









//********************************************************FUNZIONI CHE CALCOLANO I COSTI*******************************************************************************
double calculationCostCompany(double peso_TW, double peso_trashipment, double peso_intermediate_stop, string route_azienza, string passengers_azienda, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers, map<string, double>& from_to_company) {
	vector<Route> routes_company_solution = fillRoute(route_azienza);

	map <string, Route> codice_routeAzienda;
	for (Route r : routes_company_solution) {
		//r.printCompany();
		codice_routeAzienda.insert(make_pair(r.aircraft_code_company_comparison, r));
	}



	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;

	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < (int)airstrips.size(); i++) {
		//cout << "airstrip code: " << airstrips[i].code_string << endl;
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
	}
	//look if the airstrips map are well saved
	//for (auto& x : airstrips_map)  x.second.print();

	map<string, Airplane> airplanes_map;
	for (int i = 0; i < (int)airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
	}

	for (int r = 0; r < (int)routes_company_solution.size(); r++) {
		double c = airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fixed_cost; //ho aggiunto il costo fisso
		//cout << "ecco il fixed cost" << airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fixed_cost << endl;;
		string places = ";";
		for (int i = 0; i < (int)routes_company_solution[r].places_company.size(); i++) {
			places += routes_company_solution[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution[r].places_company[i]].landing_cost; //aggiungo il leading cost
				//cout << "ecco il landing cost: " << airstrips_map[routes_company_solution[r].places_company[i]].landing_cost << endl;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < (int)routes_company_solution[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]];
				//cout << "ecco il costo della distanza percorsa: " << from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]] << endl;
				double time_flight = (from_to_company[routes_company_solution[r].places_company[i] + ";" + routes_company_solution[r].places_company[i + 1]]) / airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1) * airplanes_map[routes_company_solution[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " costo del fuel " << cost_fuel;
				c += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
	}


	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing per la compagnia: " << costo_routing << endl;



	vector<Passenger> passengers_solution; //il code_flight qui � l'aereo
	ifstream file;
	//cout << passengers_azienda << endl;
	file.open(passengers_azienda);
	if (file.fail()) {
		cerr << "Error Opening File passenger azienda" << endl;
		system("pause");
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution.push_back(p);

	}
	file.close();
	//***********************************************************************************************************************************
	//calcolo matrice A e costo della penalit� per essere fuori dall'orario previsto
	for (int p = 0; p < (int)passengers.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers[p].name.begin(), passengers[p].name.end(), passengers[p].name.begin(), ::tolower);
		transform(passengers[p].surname.begin(), passengers[p].surname.end(), passengers[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < (int)passengers_solution.size(); j++) {
			trovato_pass_name = passengers_solution[j].name.find(passengers[p].name);
			if (trovato_pass_name <= passengers_solution[j].name.size()) {
				trovato_pass_cognome = passengers_solution[j].name.find(passengers[p].surname);
				if (trovato_pass_cognome <= passengers_solution[j].name.size()) {
					pass_trovato.push_back(passengers_solution[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa � la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < (int)pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5) * (-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr) * (-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le localit� della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == passengers[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == passengers[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza �: " << best_differenza << endl;
				//cout << "il miglior from �: " << best_from << endl;
				//cout << "il miglior to �: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza �: " << best_differenza << endl;
				//cout << "il miglior from �: " << best_from << endl;
				//cout << "il miglior to �: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza �: " << best_differenza << endl;
				//cout << "il miglior from �: " << best_from << endl;
				//cout << "il miglior to �: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop * (best_to - best_from - 1)) + peso_trashipment;


			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'� un problema con il passeggero: ";
				passengers[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le localit� della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza �: " << best_differenza1 << endl;
			//cout << "il miglior from �: " << best_from1 << endl;
			//cout << "il miglior to �: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop * (best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le localit� della route
			for (int i = 0; i < codice_routeAzienda[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza �: " << best_differenza2 << endl;
			//cout << "il miglior from �: " << best_from2 << endl;
			//cout << "il miglior to �: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop * (best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'� un problema con il passeggero: ";
			passengers[p].print();
			cout << "in quanto trova pi� di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += c;
	}

	costi_time_windows = costi_time_windows * peso_TW; //per valutare cosa succede al cambiare del peso dato alle time windows

	double costo_Soluzione = costi_time_windows + costi_intermediate_stop;
	for (int i = 0; i < (int)cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
		//cout << "->" << cost_route[i] << endl;
	}
	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;

	return costo_Soluzione;


};

double calculationCostCompany_three_days(double peso_TW, map<string, double>& from_to_FuelConsumed_company, double peso_trashipment, double peso_intermediate_stop, string route_azienza_day1, string route_azienza_day2, string route_azienza_day3, string passengers_azienda_day1, string passengers_azienda_day2, string passengers_azienda_day3, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers_day1, vector<Passenger> passengers_day2, vector<Passenger> passengers_day3, map<string, double>& from_to_company) {
	double check_fixed_cost = 0.0;



	//variabili per la stampa disaggregata dei costi
	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;
	double COSTO_transhipment = 0.0;



	//creo le routes per goni giornata
	vector<Route> routes_company_solution_day1 = fillRoute(route_azienza_day1);
	vector<Route> routes_company_solution_day2 = fillRoute(route_azienza_day2);
	vector<Route> routes_company_solution_day3 = fillRoute(route_azienza_day3);
	//for (Route r : routes_company_solution) cout << "codice volo: " << r.aircraft_code << endl;
	//system("pause");
	//creo la mappa COD_AEREO ---> route delle compagnia
	map <string, Route> codice_routeAzienda_day1;
	map <string, Route> codice_routeAzienda_day2;
	map <string, Route> codice_routeAzienda_day3;
	for (Route r : routes_company_solution_day1) {
		codice_routeAzienda_day1.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day2) {
		codice_routeAzienda_day2.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day3) {
		codice_routeAzienda_day3.insert(make_pair(r.aircraft_code_company_comparison, r));
	}

	/*for (auto x : codice_routeAzienda_day3) {
		cout << x.first << endl;
		x.second.printCompany();
	}
	system("pause");*/

	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;


	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < airstrips.size(); i++) {
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
		//cout << "- " << airstrips[i].code_string << endl;
	}
	map<string, Airplane> airplanes_map;
	for (int i = 0; i < airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
		//cout << "- " << airplanes[i].code_company << endl;
	}
	//system("pause");





	cout << "giorno 1: " << endl;
	double giorno1 = 0.0;
	for (int r = 0; r < routes_company_solution_day1.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day1[r].places_company.size(); i++) {
			places += routes_company_solution_day1[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day1[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno1 += c;
	}
	cout << "costo --> " << giorno1 << endl;
	cout << "giorno 2: " << endl;
	double giorno2 = 0.0;
	for (int r = 0; r < routes_company_solution_day2.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day2[r].places_company.size(); i++) {
			places += routes_company_solution_day2[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day2[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno2 += c;
	}
	cout << "costo --> " << giorno2 << endl;
	cout << "giorno 3: " << endl;
	double giorno3 = 0.0;
	for (int r = 0; r < routes_company_solution_day3.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day3[r].places_company.size(); i++) {
			places += routes_company_solution_day3[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day3[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno3 += c;
	}
	cout << "costo --> " << giorno3 << endl;



	for (Route r1 : routes_company_solution_day1) {
		double c = airplanes_map[r1.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r1.aircraft_code_company_comparison].fixed_cost;

		for (Route r2 : routes_company_solution_day2) {
			if (r1.aircraft_code_company_comparison == r2.aircraft_code_company_comparison) {
				if (r1.places_company[r1.index - 1] != r2.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += from_to_FuelConsumed_company[r1.aircraft_code_company_comparison + ";" + r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += airstrips_map[r2.places_company[0]].landing_cost;
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r2 : routes_company_solution_day2) {
		double c = airplanes_map[r2.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;

		for (Route r3 : routes_company_solution_day3) {
			if (r2.aircraft_code_company_comparison == r3.aircraft_code_company_comparison) {
				if (r2.places_company[r2.index - 1] != r3.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------>" << from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------------->" << from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += airstrips_map[r3.places_company[0]].landing_cost;
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r3 : routes_company_solution_day3) {
		double c = airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
		check_fixed_cost += airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
	}
	//ricordati di aggiungere il costo fisso più quello dell'arco aggiuntivo
	//se uso un nuovo aereo metto il costo fisso se uso quello vecchio pago il tratto aggiuntivo se manca
	cout << "il costo fisso di uso degli aerei e' " << check_fixed_cost << endl;
	COSTO_fisso += check_fixed_cost;
	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing + costo fisso + eventuali archi aggiuntivi per unire gli aerei per la compagnia: " << costo_routing << endl;
	double costo_Soluzione = 0.0;


	//********************************************_day1*****************************************************************************************
	vector<Passenger> passengers_solution_day1; //il code_flight qui è l'aereo
	ifstream file_day1;
	//cout << passengers_azienda << endl;
	file_day1.open(passengers_azienda_day1);
	if (file_day1.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day1.eof()) {
		string row;
		getline(file_day1, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day1.push_back(p);

	}
	file_day1.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day1.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day1[p].name.begin(), passengers_day1[p].name.end(), passengers_day1[p].name.begin(), ::tolower);
		transform(passengers_day1[p].surname.begin(), passengers_day1[p].surname.end(), passengers_day1[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day1[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day1.size(); j++) {
			trovato_pass_name = passengers_solution_day1[j].name.find(passengers_day1[p].name);
			if (trovato_pass_name <= passengers_solution_day1[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day1[j].name.find(passengers_day1[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day1[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day1[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day1[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day1[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day1[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day1[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day1[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day1[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day1[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c* peso_TW);
	}
	//**************************************************end_day1*********************************************************************************
	//********************************************_day2*****************************************************************************************
	vector<Passenger> passengers_solution_day2; //il code_flight qui è l'aereo
	ifstream file_day2;
	//cout << passengers_azienda << endl;
	file_day2.open(passengers_azienda_day2);
	if (file_day2.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day2.eof()) {
		string row;
		getline(file_day2, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day2.push_back(p);

	}
	file_day2.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day2.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day2[p].name.begin(), passengers_day2[p].name.end(), passengers_day2[p].name.begin(), ::tolower);
		transform(passengers_day2[p].surname.begin(), passengers_day2[p].surname.end(), passengers_day2[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day2[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day2.size(); j++) {
			trovato_pass_name = passengers_solution_day2[j].name.find(passengers_day2[p].name);
			if (trovato_pass_name <= passengers_solution_day2[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day2[j].name.find(passengers_day2[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day2[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day2[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day2[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day2[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day2[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day2[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day2[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day2[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day2[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c*peso_TW);
	}
	//**************************************************end_day2*********************************************************************************
	//********************************************_day3*****************************************************************************************
	vector<Passenger> passengers_solution_day3; //il code_flight qui è l'aereo
	ifstream file_day3;
	//cout << passengers_azienda << endl;
	file_day3.open(passengers_azienda_day3);
	if (file_day3.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day3.eof()) {
		string row;
		getline(file_day3, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day3.push_back(p);

	}
	file_day3.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day3.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day3[p].name.begin(), passengers_day3[p].name.end(), passengers_day3[p].name.begin(), ::tolower);
		transform(passengers_day3[p].surname.begin(), passengers_day3[p].surname.end(), passengers_day3[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day3[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day3.size(); j++) {
			trovato_pass_name = passengers_solution_day3[j].name.find(passengers_day3[p].name);
			if (trovato_pass_name <= passengers_solution_day3[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day3[j].name.find(passengers_day3[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day3[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day3[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day3[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day3[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day3[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day3[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day3[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;
			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day3[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day3[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c*peso_TW);
	}
	//**************************************************end_day3*********************************************************************************





	costo_Soluzione += costi_time_windows + costi_intermediate_stop;
	COSTO_tw += costi_time_windows;
	COSTO_intermediate += costi_intermediate_stop;
	COSTO_intermediate = COSTO_intermediate - COSTO_transhipment;
	for (int i = 0; i < cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
		//cout << "->" << cost_route[i] << endl;
	}





	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;


	//******************************************************************************DEVI METTERE QUA LA STAMPA DISAGGREGATA******************************************************************************
	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;n° transhipment;costo_totale" << endl;
	cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / airplanes_map[airplanes[0].code_company].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / peso_TW) << ";" << (COSTO_transhipment / peso_trashipment) << ";" << costo_Soluzione << ";";




	return costo_Soluzione;


};



double cost_single_route(double peso_TW, double peso_intermediate_stop, Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//double cost = 0.0;
	double cost = map_airplane[r.aircraft_code].fixed_cost;
	//for (int i = 1; i < r.index; i++) cost += map_airstrip[r.places[i]].landing_cost;

	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	//double first_hour = 0.0;
	//double second_hour = 0.0;
	for (int i = 0; i < r.index; i++) {
		//double time_travel = 0.0;
		if (i >= 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
		}

		if (i < r.index - 1) {
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}
		}

	}


	cost += mileage;
	//cost += first_hour * map_airplane[r.aircraft_code].fuel_burn_first + second_hour * map_airplane[r.aircraft_code].fuel_burn_second;
	cost += fuel_consumed;

	for (const auto& p : r.passengers_in_route) {

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];
		cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double calculate_ObjectiveFunction(double peso_TW, double peso_intermediate_stop, vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		//cout << "Route-----" << endl;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}

		//cout << "costo fisso: " << cost << endl;

		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				cost_route += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}




		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;




		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (const auto& p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop
			cost_route += ((p.solution_to - p.solution_from - 1) * peso_intermediate_stop) * p.capacity;  //riga aggiunta per le intermediate stop

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost_route += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
		}

		r.cost = cost_route;
	}

	return cost;
}

double cost_time_windows_for_route(Route& r, double peso_TW) {
	double cost = 0.0;

	//cout << "*********************************************************dentro alla funzione*********************************************************" << endl;
	//cout << "indice del vettore degli arrivi " << r.time_arr.size();



	for (const Passenger& p : r.passengers_in_route) {
		//cout << " Indice from: " << p.solution_from << endl;
		//cout << " Indice to: " << p.solution_to << endl;
		//r.print();

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

		cost += (TW_departure + TW_arrival) * p.capacity;
	}

	//cout << "*********************************************************fine dentro funzione*********************************************************" << endl;
	cost = cost * peso_TW;

	return cost;
}

double cost__for_route_passenger_destroyCluster(Route& r, Passenger& p, int fattore_inter_stop, double peso_TW) {
	double cost = 0.0;

	//cout << "**************************************" << endl;
	//cout << "r.time_arr.size()" << r.time_arr.size();
	//p.print();

	//double time_departure = r.time_arr[p.solution_from];
	//double time_arrival = r.time_arr[p.solution_to];

	double TW_departure = 0.0;
	if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
	if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW + fattore_inter_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;

	return cost;
}

double cost_time_windows_for_node(Route& r, vector<Passenger>& pass, double peso_TW) {
	double cost = 0.0;
	for (const Passenger& p : pass) {
		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

		cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
	}

	return cost;
}

double cost_time_windows_for_route_passenger(Route& r, Passenger& p, double peso_TW) {
	double cost = 0.0;



	double TW_departure = 0.0;
	if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
	else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

	double TW_arrival = 0.0;
	if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
	else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

	cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

	return cost;
}



bool relateness_passenger2(int trheshold, vector<vector<double>>& from_to, int idP, Passenger& r, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequenza, string pointer, int y, map<int, Passenger>& map_id_passenger) {
	//cout << " Sono dentro la funzione Relateness " << endl;
	//vector<string> Pass1 = split(idP, '/');
	// Poi questo for si potra togliere
	Passenger pass = map_id_passenger[idP];

	// Questo poi va cambiato
	if (pass.name.size() == 0) {
		cout << " ******************************* ERRORE ***********************************" << endl;
		cout << " Problema con il passeggiero con Pnr: " << idP << endl;
		cin.get();
	}
	double Relateness = from_to[r.arrival_location][pass.arrival_location] + from_to[r.departure_location][pass.departure_location] + abs(r.arrival_time - pass.arrival_time) + abs(r.departure_time - pass.departure_time);
	//cout <<" Relateness ---> " << Relateness << "  Soglia " << trheshold <<endl;
	if (Relateness <= trheshold) {
		/*cout << " Sono dentro Releatensess---> Valore : " << Relateness <<  endl;
		cout << " Sono dentro Releatensess---> Dato il passeggiero : " << endl;
		pass.print();
		cout << " Sono dentro Releatensess---> Devo togliere il passeggiero " << endl;
		r.print();
		cout << " e con lui: ";
		for (string j : agr_pass[pointer]) cout << j << " / " ;
		cout << endl;
		*/
		for (int j : agr_pass[pointer]) passengers_removed.push_back(map_id_passenger[j]);
		//cout << " Numero di passeggieri che sono stati messi nei rimossi RIGA 839 Main " << passengers_removed.size() << endl;
		//cout << " Devo eliminare sia da sequenza sia dalla mappa " << pointer << endl;
		sequenza.erase(sequenza.begin() + y);
		agr_pass.erase(agr_pass.find(pointer));
		//cout << " Sequenza dopo ha: " << sequenza.size() << endl;
		//cout << " Gruppi dopo ha: " << agr_pass.size() << endl;
		return true;
	}
	else {
		return false;
	}

}

//*************************************************************************************COSTRUTTIVI************************************************************************
vector<Route> heuristic_costructive_second_fase(vector<Route>& solution, double end_day, double peso_TW) {
	vector<Route> sol;

	for (Route& r : solution) {
		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, peso_TW);
		if (cost_time_windows_for_route(r, peso_TW) != 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			for (int i = 0; i < r.index - 1; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
				double wait_time = 0;
				if (i == 0) {
					do {
						for (int j = 0; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;  //prima c'era ++ per ottimizzare ho messo pi� due in modo da valutare ogni due minuti non ogni minuto
					} while (wait_time <= 60);
				}
				else {
					do {
						r_support.time_dep[i] += 2;
						for (int j = i + 1; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;
					} while (wait_time <= 60);
				}
				r_support = best_route;
			}
		}
		sol.push_back(best_route);
	}
	return sol;
}

vector<Route> heuristic_costructive_second_fase_SP(vector<Route>& solution, double end_day, double peso_TW) {
	vector<Route> sol;

	for (Route& r : solution) {
		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, peso_TW);
		if (cost_time_windows_for_route(r, peso_TW) != 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			for (int i = 0; i < r.index - 1; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
				double wait_time = 0;
				if (i == 0) {
					do {
						for (int j = 0; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;  //prima c'era ++ per ottimizzare ho messo pi� due in modo da valutare ogni due minuti non ogni minuto
					} while (wait_time <= 30);
				}
				else {
					do {
						r_support.time_dep[i] += 2;
						for (int j = i + 1; j < r.index; j++) {
							r_support.time_arr[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r_support.index - 1] <= end_day) {
							double cost_support = cost_time_windows_for_route(r_support, peso_TW);
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;
					} while (wait_time <= 30);
				}
				r_support = best_route;
			}
		}
		sol.push_back(best_route);
	}
	return sol;
}

vector<Route> heuristic_costructive_first_fase(double peso_TW, double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);

	for (auto p : passengers) {
		//cout << "Sono il passeggero**********************" << endl;
		//p.print();
		int situation = -1;
		int best_route = -1; //index of the best route where can i put my passenger
		int best_from = -1;
		int best_to = -1;
		double best_cost = DBL_MAX;
		int i = 0;
		//cout << "- sto calcolando dove metttere il passeggero: " << p.name << endl;
		//qui calcolo solo dove mettere il passeggero
		for (auto r : solution) {
			if (r.primo_pass == false) {
				if (r.places[r.index - 1] == p.departure_location) {
					//in questo caso c'� solo lui nella route, il costo � dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
					//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


					cost += fuel_consumed;
					//per il check sul fuel:
					double fuel_after_trip = 0.0;
					fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
					//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
							best_cost = cost;
							best_route = i;
							situation = 1;
						}
					}
				}
				else {
					//qui c'� solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi� kilometri e un landing stop
					//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
					//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					cost += fuel_consumed;

					//per il check sul fuel
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
							if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = 2;
							}
						}
					}
				}
			}
			else {
				//nella route c'� almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit� della route
				//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'� il FROM, non cerco il TO
				vector<int> FROM;
				vector<int> TO;
				for (int t = 0; t < r.index; t++) {
					if (r.places[t] == p.departure_location) FROM.push_back(t);
				}

				if (FROM.size() != 0) {
					//dentro questo abbiamo trovato dei FROM 
					for (int t = FROM[0]; t < r.index; t++) {
						if (r.places[t] == p.arrival_location) TO.push_back(t);
					}

					if (TO.size() != 0) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from < to && to - from <= p.stop) {
									//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
									//se sono qua dentro il to � maggiore del from e gli stop intermedi sono minori di tre
									//per prima cosa guardo se ci sto con la capacit�, altrimenti break sul primo for
									bool capacity_satisfy = true;
									for (int c = from; c < to; c++) {
										if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
										if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

										//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

									}
									//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
									if (!capacity_satisfy) break; //check also for the fuel;





									//se sorpasso il break vuol dire che soddisfo la capacit�, in questo caso il solo costo che consider� per l'aggiunta del passeggero qui
									//� la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
									else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

									double TW_arrival = 0.0;
									//double t_arr_arrival = r.time_arr[to];
									if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
									else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

									double cost = ((TW_departure + TW_arrival) * peso_TW + ((to - from - 1) * peso_intermediate_stop)) * p.capacity;
									if (best_cost > cost) {
										best_cost = cost;
										best_route = i;
										situation = 4;
										best_from = from;
										best_to = to;
									}
								}
							}
						}
					}
					//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
					//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
					if (r.places[r.index - 1] != p.arrival_location) {
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (int c = from; c < r.index; c++) {
								if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

								if (c < r.index - 1) {
									//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

									if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
								}
							}
							//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
							//cout << p.stop<< endl;
							if (capacity_satisfy && (r.index - from) <= p.stop) {

								double TW_departure = 0.0;
								//double t_arr_departure = r.time_arr[from];
								if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
								else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

								double cost = (TW_departure * peso_TW);
								// ora ci metto chilometri e fuel dell'ultimo pezzo;
								cost += from_to[r.places[r.index - 1]][p.arrival_location];
								double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];

								cost += fuel_consumed;

								bool fuel_ok = true;
								if (!r.refueling[r.index - 1]) {
									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
									if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
								}

								double TW_arrival = 0.0;
								double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
								if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
								else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
								cost += (TW_arrival * peso_TW) * p.capacity;

								cost += (peso_intermediate_stop * (r.index - from - 1)) * p.capacity;

								if (fuel_ok) {
									if (best_cost > cost) {
										if (t_arr_arrival <= end_day) {
											if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
												best_cost = cost;
												best_route = i;
												best_from = from;
												situation = 5;
											}

										}
									}
								}
							}
						}
					}

				}
				if (r.places[r.index - 1] != p.departure_location) {
					//guardo il costo di aggiungere un pezzo alla fine
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

					double TW_departure = 0.0;
					double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}



					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = 3;
								}
							}
						}
					}

				}
			}
			i++;
		}

		//cout << " caso : " << situation << " for the passeger:" << endl;
		//p.print();
		//ora devo aggiungere il passeggero nel posto migliore, quindi serve valutare in che caso sono A,B,C,D
		if (situation == 1) {
			solution[best_route].primo_pass = true;
			//time
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto � la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1] += p.capacity;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}
			//end for weight




			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);





		}
		else if (situation == 2) {
			solution[best_route].primo_pass = true;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 -
				map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;




			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);




		}
		else if (situation == 3) {

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity,
					solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;


			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60);





			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}


			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 4) {



			for (int h = best_from; h < best_to; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;
			}

			//**************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//**************************************************************************




			p.solution_from = best_from;
			p.solution_to = best_to;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 5) {


			for (int h = best_from; h < solution[best_route].index; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;


			}

			//******************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//******************************************************************************

			p.solution_from = best_from;
			int place_1 = solution[best_route].places[solution[best_route].index - 1];
			double aircraft_speed = map_airplane[solution[best_route].aircraft_code].speed;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}

			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == -1) {
			cout << "*******************************************************il passeggero: " << p.name << " non trova sistemazione" << endl;
			//p.print();
			//for (Route r : solution) r.print();
			//system("pause");
		}
	}


	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}


	return solution_clean;
}

vector<Route> heuristic_costructive_first_fase_sequential(double peso_TW, double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger> passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
	//now i have created only the possible route, now i that one of them and fill it untill the condition stop

	for (Route& r : solution) {
		//cout << "sto valutando la route *******************************************************************************************************************************************************************" << endl;
		int situation = -1;
		do {
			situation = -1; //se situation rimane error vuol dire che nessun passeggero si pu� inserire, quindi che la route � piena e devo passare ad un'altra
			int best_passenger = -1;
			int best_from = -1;
			int best_to = -1;
			double best_cost = DBL_MAX;
			//cout << "sto valutando nella route " << endl;
			//r.print();
			//cout << "passengers.size()= " << passengers.size() << endl;
			for (int p = 0; p < (int)passengers.size(); p++) {

				//cout << "sono il passeggero: " << endl;
				//passengers[p].print();

				if (r.primo_pass == false) {
					if (r.places[r.index - 1] == passengers[p].departure_location) {
						//in questo caso c'� solo lui nella route, il costo � dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[passengers[p].departure_location][passengers[p].arrival_location];
						//double travel_time = from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location] / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;

						//cout << "validit� caso A" << endl;
						//cout << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;

						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
								best_cost = cost;
								best_passenger = p;
								situation = 1;
								//cout << "situation A, cost: " << cost << "passenger: " << p << endl;
							}
						}
					}
					else {
						//qui c'� solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi� kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[passengers[p].departure_location][passengers[p].arrival_location] + from_to[r.places[r.index - 1]][passengers[p].departure_location];
						//double travel_time = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location] + from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];

						cost += fuel_consumed;

						//per il check sul fuel
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];


						//****
						//double travel_secondo_tratto = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];

						//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "validit� caso B" << endl;
						//cout << best_cost << " > " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;


						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
									best_cost = cost;
									best_passenger = p;
									situation = 2;
									//cout << "situation B, cost: " << cost << "passenger: " << p << endl;
								}
							}
						}
					}
				}
				else {
					//nella route c'� almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit� della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'� il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = 0; t < r.index; t++) {
						if (r.places[t] == passengers[p].departure_location) FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++) {
							if (r.places[t] == passengers[p].arrival_location) TO.push_back(t);
						}

						if (TO.size() != 0) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= passengers[p].stop) {
										//se sono qua dentro il to � maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacit�, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][passengers[p].arrival_location];

											if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

										}
										//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;
										//cout << " ho passato la fesibility del caso D " << endl;




										//se sorpasso il break vuol dire che soddisfo la capacit�, in questo caso il solo costo che consider� per l'aggiunta del passeggero qui
										//� la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

										double TW_departure = 0.0;
										double t_arr_departure = r.time_arr[from];
										if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
										else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

										double TW_arrival = 0.0;
										double t_arr_arrival = r.time_arr[to];
										if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
										else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;

										double cost = ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;
										cost += (peso_intermediate_stop * (to - from - 1));

										//cout << "if del caso D " << best_cost << " > " << cost << endl;
										if (best_cost > cost) {
											best_cost = cost;
											best_passenger = p;
											situation = 4;
											best_from = from;
											best_to = to;
											//cout << "situation D, cost: " << cost << "passenger: " << p << endl;
										}
									}
								}
							}
						}
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
						//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.places[r.index - 1] != passengers[p].arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
								//cout << p.stop<< endl;
								//cout << "feasibility caso E " << capacity_satisfy << " && " << (r.index - from) << " <= " << passengers[p].stop << endl;
								if (capacity_satisfy && (r.index - from) <= passengers[p].stop) {

									double TW_departure = 0.0;
									double t_arr_departure = r.time_arr[from];
									if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
									else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

									double cost = (TW_departure * peso_TW);
									// ora ci metto chilometri e fuel dell'ultimo pezzo;
									cost += from_to[r.places[r.index - 1]][passengers[p].arrival_location];
									double travel_time = (from_to[r.places[r.index - 1]][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].arrival_location];

									cost += fuel_consumed;

									bool fuel_ok = true;
									if (!r.refueling[r.index - 1]) {
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
										if (r.weight[r.index - 1] - passengers[p].weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
									}

									double TW_arrival = 0.0;
									double t_arr_arrival = 0.0;
									t_arr_arrival = r.time_arr[r.index - 1] + (travel_time * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
									else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
									cost += ((TW_arrival * peso_TW)) * passengers[p].capacity;
									cost += (peso_intermediate_stop * (r.index - from - 1)) * passengers[p].capacity;

									//cout << "if caso E" << fuel_ok << " && " << best_cost << " > " << cost << " && " << t_arr_arrival << " <= " << end_day << " && " << r.quantity_fuel[r.index - 1] - fuel_consumed << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;
									if (fuel_ok) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
													best_cost = cost;
													best_passenger = p;
													best_from = from;
													situation = 5;
												}
											}
										}
										//cout << "situation E, cost: " << cost << "passenger: " << p << endl;
									}
								}
							}
						}
						//ho aggiunto questo if che c'� quaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
					}
					if (r.places[r.index - 1] != passengers[p].departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location] + from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location] + from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][passengers[p].departure_location] + from_to[passengers[p].departure_location][passengers[p].arrival_location];

						double TW_departure = 0.0;
						//double t_arr_departure = r.time_arr[r.index - 1] + (map_airstrip[r.places[r.index - 1]].ground_time + (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60;
						double t_arr_departure = r.time_dep[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
						else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[passengers[p].departure_location].ground_time + (((from_to[passengers[p].departure_location][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
						else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
						cost += ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}


						//cout << "if caso C " << best_cost << " > " << cost << " && " << t_arr_arrival << " <= " << end_day << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;
						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
										best_cost = cost;
										best_passenger = p;
										situation = 3;
									}
								}
							}
							//cout << "situation C, cost: " << cost << "passenger: " << p << endl;
						}

					}
				}
			}
			//qui dovrei avere tutte le cose per fare la mossa sempre se la route non � piena

			/*if (situation != "Error") {
				cout << "la situation scelta e " << situation << "per il passeggero " << endl;
				passengers[best_passenger].print();
			}*/

			if (situation == 1) {
				r.primo_pass = true;
				//time
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure; // in questo caso anche l'arrivo in quanto � la partenza dal depot

				passengers[best_passenger].solution_from = r.index - 1;
				r.capacity[r.index - 1] += passengers[best_passenger].capacity;

				//for the weight
				r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
					r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				if (r.weight[r.index - 1] < 0) {
					r.quantity_fuel[r.index - 1] += r.weight[r.index - 1];
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;
				}
				//end for weight




				double travel_time = ((from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60;
				double time_from = r.time_dep[r.index - 1];
				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location];

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people - map_airplane[r.aircraft_code].max_fuel;
					//end for weight
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];


					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;
					//end for weight

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);





			}
			else if (situation == 2) {
				r.primo_pass = true;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure - (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60 -
					map_airstrip[r.places[r.index - 1]].ground_time;
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure - (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;

				//for the weight
				r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
					r.quantity_fuel[r.index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				//end for weight

				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];
				if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, passengers[best_passenger].capacity, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];

					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, passengers[best_passenger].capacity, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_from = r.index - 1;
				double time = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time + (from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;




				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel;
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);




			}
			else if (situation == 3) {

				if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, passengers[best_passenger].capacity,
						r.time_dep[r.index - 1] +
						((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60),
						r.time_dep[r.index - 1] + ((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;


				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;


					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, passengers[best_passenger].capacity, r.time_dep[r.index - 1] +
						((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60),
						r.time_dep[r.index - 1] + ((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_from = r.index - 1;
				double time = r.time_dep[r.index - 1] + ((from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60);





				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel;

				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;


					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}


				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == 4) {


				for (int h = best_from; h < best_to; h++) {
					r.capacity[h] += passengers[best_passenger].capacity;
					r.weight[h] -= passengers[best_passenger].weight;
				}


				//**************************************************************************
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = best_from; j < r.index; j++) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					if (r.weight[j] < 0) {
						//cout << " Weigth negativo in " << j << weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
				//**************************************************************************




				passengers[best_passenger].solution_from = best_from;
				passengers[best_passenger].solution_to = best_to;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == 5) {



				for (int h = best_from; h < r.index; h++) {
					r.capacity[h] += passengers[best_passenger].capacity;
					r.weight[h] -= passengers[best_passenger].weight;
				}


				//******************************************************************************
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = best_from; j < r.index; j++) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					if (r.weight[j] < 0) {
						//cout << " Weigth negativo in " << j << weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
				//******************************************************************************



				passengers[best_passenger].solution_from = best_from;
				int place_1 = r.places[r.index - 1];
				double aircraft_speed = map_airplane[r.aircraft_code].speed;
				double time = r.time_dep[r.index - 1] + ((from_to[place_1][passengers[best_passenger].arrival_location] / aircraft_speed) * 60);

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel;

				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}

				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);

			}



			if (situation != -1) passengers.erase(passengers.begin() + best_passenger);



		} while (situation != -1);
		if (passengers.size() == 0) break;
	}





	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}

	cout << "ecco i passeggeri rimasti" << endl;
	for (Passenger rimasto : passengers) rimasto.print();

	return solution_clean;
}

bool route_feasible(Route& r, map<int, Airplane>& map_airplane, double end_day, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool feasible = true;


	//check and day
	if (r.time_arr[r.index - 1] > end_day) {
		return false;
		feasible = false;
	}
	//for the last-trip
	if (r.quantity_fuel[r.index - 1] < (location_fuel[r.aircraft_code][r.places[r.index - 1]] + map_airplane[r.aircraft_code].min_fuel)) {
		return false;
		feasible = false;
	}

	if (feasible) {
		for (int i = 0; i < r.index; i++) {
			if (i >= 1) {
				//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					return false;
					feasible = false;
				}
				if (r.refueling[i] && !r.refueling[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
					if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						return false;
						feasible = false;
					}
				}
			}
			if (i < r.index - 1) {
				if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
					return false;
					feasible = false;
				}
			}

		}
	}

	if (feasible) {
		//check of the intermediate stop
		for (Passenger& p : r.passengers_in_route) {
			if (p.solution_to - p.solution_from > p.stop) {
				return false;
				feasible = false;
			}
		}
	}







	return feasible;
}

map<int, int> Compute_WorstNode(double peso_TW, double peso_intermediate_stop, Route& route, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
	//double costoWorstNode = 0.00;
	//int WorstNode = -1;
	map<double, int> Node;
	map<int, int> NodeOrder;
	set<double, MyCOMP<double>> Myset;
	if (route.index <= 2) {
		//route.print();
		Node.insert(make_pair(100000, 1));
		Myset.insert(100000);
		//cout << " Route troppo piccola fisso indice a " << 1 << endl;
	}
	else {
		for (int n = 1; n < route.index; n++) {
			double dist = 0.0;
			double cost_IS = 0.0;
			vector<Passenger> PassengerNodo;
			if (n != (route.index - 1)) {
				//cout << " Calcolo per nodo n mezzo " << endl;
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]] + from_to[route.places[n]][route.places[n + 1]] - from_to[route.places[n - 1]][route.places[n + 1]];
				for (int p = 0; p < (int)route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n || route.passengers_in_route[p].solution_from == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}

					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += (peso_intermediate_stop)* route.passengers_in_route[p].capacity;
						}
					}
					//****************************************************************************************************************************************


				}
			}
			else {
				//cout << " Calcolo per ultimo nodo " << endl;
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]];
				for (int p = 0; p < (int)route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}


					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += (peso_intermediate_stop)* route.passengers_in_route[p].capacity;
						}
					}
					//****************************************************************************************************************************************

				}


			}


			dist += (cost_time_windows_for_node(route, PassengerNodo, peso_TW)) + cost_IS;
			// / PassengerNodo.size());
			//cout << " Inserisco nodo: " << n << " con il costo: " << dist << endl;
			Node.insert(make_pair(dist, n));
			Myset.insert(dist);
			//cout << " ************** Nodo: " << n << " Ha un costo: " << dist << endl;
			//cout << " ************** Nodo Peggiore finora: " << WorstNode << endl;
			//if (costoWorstNode < dist) {
				//costoWorstNode = dist;
				//WorstNode = n;
			//}
		}
	}

	//cout << " **************Stampo la mappa non ordinata: "<< endl;
	//for (auto x : Node) cout << x.first << " -----> " << x.second << endl;
	//cout << " **************Stampo il Set: " << endl;
	//for (auto setval : Myset) {
		//cout << setval << endl;
	//}

	int i = 0;
	for (auto setval : Myset) {
		NodeOrder.insert(make_pair(i, Node[setval]));
		i++;
	}
	//cout << " **************Stampo la mappa Ordinata: " << endl;
	//for (auto x : NodeOrder) cout << x.first << " -----> " << x.second << endl;
	return NodeOrder;
}




//ECCO I NOSTRI DESTROY
vector<Route> destroy_thanos(double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {


	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {

			//genero il numero random di nodi da distruggere da 2 a place.size()-1;
			double numero_random = (double)rand() / RAND_MAX;
			if (r.places.size() == 2) {
				numero_random = round(1 + (numero_random * (r.places.size() - 2)));
				//numero_random = 0;
			}
			else {
				numero_random = round(2 + (numero_random * (r.places.size() - 3)));
			}


			//cout << "sono la route numero: " << index << " il numero random generato �: " << numero_random << " la route ha places.size-1 pari a: " << (r.places.size() - 1) << endl;

			if (numero_random == r.places.size() - 1) {
				//cout << "sono dentro all'if del numero random uguale a places()-1" << endl;
				// qua devo distruggere tutta la route e lasciare solo il depot
				for (int i = (int)(r.places.size()) - 1; i >= 1; i--) {
					r.places.erase(r.places.begin() + i);
					r.refueling.erase(r.refueling.begin() + i);
					r.quantity_fuel.erase(r.quantity_fuel.begin() + i);
					r.time_arr.erase(r.time_arr.begin() + i);
					r.time_dep.erase(r.time_dep.begin() + i);
					r.capacity.erase(r.capacity.begin() + i);
					r.weight.erase(r.weight.begin() + i);
					r.index--;
					//r.index_cap--;
				}
				//qua ci dovrebbe essere solo il deposito
				//fisso il peso, capacita e fuel ai valori di default e la variabile passeggeri dentro a false
				r.quantity_fuel[0] = map_airplane[r.aircraft_code].max_fuel;
				r.weight[0] = map_airplane[r.aircraft_code].weight_fuel_people - r.quantity_fuel[0];
				r.capacity[0] = 0;
				r.primo_pass = false;

				//ora devo togliere tutti i passeggeri
				for (int p = r.passengers_in_route.size() - 1; p >= 0; p--) {
					r.passengers_in_route[p].route_before = index;
					passenger_removed.push_back(r.passengers_in_route[p]);
					r.passengers_in_route.erase(r.passengers_in_route.begin() + p);
				}

			}
			else {
				//qua devo pescare un numero di nodi da distruggere pari a numero_random
				int nodi_rimossi = 0; //finch� non ho rimosso 
				do {
					//cout << endl;
					//cout << "nodi rimossi: " << nodi_rimossi << endl;
					bool check = true;
					do {
						//if i'm here i can destroy the route
						//at the moment i destroy only a node
						int node_destroy = (int)(round(1 + ((double)rand() / RAND_MAX) * (r.index - 2)));
						//cout << "node destroy: " << node_destroy << endl;
						//cout << "r.index - 1= " << (r.index - 1) << endl;
						if (node_destroy == r.index - 1) { //this in the case of that node is at the end
							check = false;
							//cout << "nodo distrutto sono nel caso della rimozione dell'ultimo nodo: " << node_destroy <<endl;
							//r.print();
							vector<int> int_removed;
							//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
							//cout << " Questa route ha un index = " << r.index << endl; 
							//cout << "route prima della distruzione: " << endl;
							//r.print();

							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t] -= r.passengers_in_route[p].capacity;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							// Qua devo mettere codice che � quello dell'Update
							/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
							cout << "******************* Stampiamo la Route: *******************" << endl;
							cout << " CODE ------> " << r.aircraft_code << endl;
							cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
							cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
							cout << " Node destroyed ------> " << node_destroy << endl;*/
							int index_min_from = 0;
							for (int i = Min_From_Pass; i >= 0; i--) {
								if (r.refueling[i] && i != node_destroy) {
									index_min_from = i;
									break;
								}
							}
							//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
							//Codice che serve per cercare il minimo nel range
							//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;
							for (int k = index_min_from; k < Max_To_Pass; k++) {
								if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
								//cout << " Sto valutando il caso del nodo " << k << endl;
									int Node_min = k;
									double min_weight = r.weight[k];
									int index_updating_from = k;
									int index_updating_to = r.index;  //qua prima c'era -1
									for (int i = k + 1; i <= Max_To_Pass; i++) {
										//cout << " Sto guardando il nodo " << i << endl;
										//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
										//cout << " Ho passato IF di controllo " << endl;
										if (r.refueling[i]) break;
										if (r.weight[i] < min_weight && i != node_destroy) {
											min_weight = r.weight[i];
											Node_min = i;
										}
										//}
									}
									//cout << " Nodo di minimo ---> " << Node_min << endl;
									//cout << " Valore di minimi --> " << min_weight << endl;
									if (Node_min >= 0) {
										for (int i = k + 1; i < r.index; i++) {
											if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
												index_updating_to = i;
												break;
											}
										}
										//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
										//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
										double Fuel_before = r.quantity_fuel[index_updating_from];
										//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
										//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
										r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
										//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
										//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
										r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
										//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
										for (int i = index_updating_from + 1; i < index_updating_to; i++) {
											if (i != node_destroy) {
												r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
												r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
											}
										}
									}
								}

							}

							int nodi_mancanti = (int)(r.places.size());
							r.removePlace(node_destroy, map_airplane);
							nodi_mancanti -= (int)r.places.size();
							nodi_rimossi += nodi_mancanti;
							//cout << "route dopo la distruzione: " << endl;
							//r.print();
							// FORSE QUI CI VUOLE IL CODICE DEI CASI CON PESO NEGATIVO
						}
						else {
							//cout << "sono dentro l'else " << endl;
							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
							//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed;



							if (fuel_consumed <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
								check = false;
								vector<int> int_removed;
								//cout << " route prima di togliere il Nodo: " << node_destroy << endl;
								//r.print();
								int Min_From_Pass = node_destroy;
								int Max_To_Pass = node_destroy;
								for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
									if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
										if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
										if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
										int_removed.push_back(p);
										for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
											r.capacity[t] -= r.passengers_in_route[p].capacity;
											r.weight[t] += r.passengers_in_route[p].weight;
										}

									}
								}

								//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
								//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
								//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
								for (int i = int_removed.size() - 1; i >= 0; i--) {
									// code for repair forbidden***********************************************************
									r.passengers_in_route[int_removed[i]].route_before = index;
									//*************************************************************************************
									passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
									vector<Passenger>::iterator it;
									it = r.passengers_in_route.begin();
									r.passengers_in_route.erase(it + int_removed[i]);
								}

								int nodi_mancanti = (int)r.places.size();
								r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
								//QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA***************************************************************************************

								//double fuel_consumed1 = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy]];

								//double fuel_consumed2 = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy]][r.places[node_destroy + 1]];



								int index_before = node_destroy - 1;
								//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
								//r.print();
								//cout << " Adessos sto iniziando a sistemare i pedi perche togliero nodo " << endl;
								double diff = 0;
								//cout << " Index before: " << index_before << endl;
								for (int i = node_destroy + 1; i < r.index; i++) {
									//cout << " sono al i = " << i << endl;
									if (r.refueling[i]) break;
									//cout << "weight before: " << r.weight[i] << endl;
									if (index_before == (node_destroy - 1)) {
										//cout << " sono al primo giro faccio " << endl;
										diff = r.quantity_fuel[i];
										//cout << " diff: " << diff << endl;
										r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
										diff = diff - r.quantity_fuel[i];
										//cout << " diff aggiornata: " << diff << endl;

									}
									else {
										//cout << " non sono al primo giro " << endl;
										r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
										//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
										//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									}

									if (r.refueling[node_destroy]) {
										//cout << " caso in cui facevo refuiling nel nodo distrutto " << endl;
										//cout << "a r.weight[i]=  " << r.weight[i] << " tolgo questo valore  " << diff << endl;
										r.weight[i] = r.weight[i] + diff;

									}
									else {
										//	cout << " caso odo che ho tolto non era un fuel " << endl;
										//	cout << " weight prima " << r.weight[i] << endl;
										r.weight[i] = r.weight[i] + diff;
										//cout << " weight dopo " << r.weight[i] << endl;
									}


									//cout << "weight after: " << r.weight[i] << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

									//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
									//cout << " inidce before = " << index_before << endl;
									index_before = i + 1;
									//cout << " inidce before = " << index_before << " riparto con un altro giro " << endl;
								}
								//*****************************************************************************************************************************************************************
								r.removePlace(node_destroy, map_airplane);
								nodi_mancanti -= (int)(r.places.size());
								nodi_rimossi += (int)(nodi_mancanti);
								//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weight[j] < 0) {
										//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
										add_fuel = r.weight[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.refueling[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}
										//cout << " Aggiorno da " << index_refueling << endl;
										for (int t = index_refueling; t < r.index; t++) {
											//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
											if (r.refueling[t] && t != index_refueling) break;
											//cout << " Ho passato if aggiorno nodo " << t << endl;
											//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
											r.quantity_fuel[t] += add_fuel;
											r.weight[t] -= add_fuel;
											//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
										}
									}
								}
								//cout << "route dopo la distruzione: " << endl;
								//r.print();
							}
						}
					} while (check);


				} while (nodi_rimossi < numero_random);




			}





		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
	}
	return solution;



}

vector<Route> destroy_casual(double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			bool check = true;
			do {
				//if i'm here i can destroy the route
				//at the moment i destroy only a node
				double n_destroy = (double)rand() / RAND_MAX;
				int node_destroy = (int)(round(1 + n_destroy * (r.index - 2)));
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					//cout << "nodo distrutto: " << node_destroy <<endl;
					//r.print();
					vector<int> int_removed;
					//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
					//cout << " Questa route ha un index = " << r.index << endl; 
					//cout << " siamo nel caso che il nodo distrutto sia l'ultimo " << endl;
					//r.print();

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
								/*
								if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
								r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
								r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
								}
								*/
							}


						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						r.passengers_in_route[int_removed[i]].route_before = index;
						//*************************************************************************************

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}
					// Qua devo mettere codice che � quello dell'Update
					/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
					cout << "******************* Stampiamo la Route: *******************" << endl;
					cout << " CODE ------> " << r.aircraft_code << endl;
					cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
					cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
					//r.print();
					cout << " Node destroyed ------> " << node_destroy << endl;
					*/

					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
					// Codice che serve per cercare il minimo nel range
					//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								if (r.refueling[i]) break;
								//cout << " Ho passato il break " << endl;
								if (r.weight[i] < min_weight && i != node_destroy) {
									//	cout << " Ho passato IF di controllo " << endl;
									min_weight = r.weight[i];
									Node_min = i;
								}
							}
							//cout << " Nodo di minimo ---> " << Node_min << endl;
							//cout << " Valore di minimi --> " << min_weight << endl;
							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
								//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
								double Fuel_before = r.quantity_fuel[index_updating_from];
								//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
								//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
								//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
								//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
					//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi quando tolgo ultimo nodo " << endl;
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {
							//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
									index_refueling = i;
									break;
								}
							}
							//cout << " Aggiorno da " << index_refueling << endl;
							for (int t = index_refueling; t < r.index; t++) {
								//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
								if (r.refueling[t] && t != index_refueling) break;
								//cout << " Ho passato if aggiorno nodo " << t << endl;
								//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
								//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
							}
						}
					}
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {   //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
						check = false;
						vector<int> int_removed;
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
							if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
								if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
								if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
								int_removed.push_back(p);
								for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
									r.capacity[t] -= r.passengers_in_route[p].capacity;
									r.weight[t] += r.passengers_in_route[p].weight;
									/*
									if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
									}
									*/
								}

							}
						}

						/*
						cout << "Nodo: " << node_destroy << endl;
						cout << " Sto tohliendo caso del nodo non ultimo riga 3278 " << endl;
						cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
						cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
						cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
						*/
						for (int i = int_removed.size() - 1; i >= 0; i--) {
							// code for repair forbidden***********************************************************
							r.passengers_in_route[int_removed[i]].route_before = index;
							//*************************************************************************************
							passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
							vector<Passenger>::iterator it;
							it = r.passengers_in_route.begin();
							r.passengers_in_route.erase(it + int_removed[i]);
						}
						r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
						//r.print();





						int index_before = node_destroy - 1;
						//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
						//r.print();
						//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
						double diff = 0;
						//cout << " Index before: " << index_before << endl;
						for (int i = node_destroy + 1; i < r.index; i++) {
							//cout << " sono al i = " << i << endl;
							if (r.refueling[i]) break;
							//cout << "weight before: " << r.weight[i] << endl;
							if (index_before == (node_destroy - 1)) {
								//cout << " sono al primo giro faccio " << endl;
								diff = r.quantity_fuel[i];
								//cout << " diff: " << diff << endl;
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];
								//cout << " diff aggiornata: " << diff << endl;

							}
							else {
								//cout << " non sono al primo giro " << endl;
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
								//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							}

							//cout << " weight prima " << r.weight[i] << endl;
							r.weight[i] = r.weight[i] + diff;
							//cout << " weight dopo " << r.weight[i] << endl;



							//cout << "weight after: " << r.weight[i] << endl;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

							//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
							//cout << " inidce before = " << index_before << endl;
							index_before = i + 1;
							//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
						}
						//*****************************************************************************************************************************************************************


						r.removePlace(node_destroy, map_airplane);
						//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {
								//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}
								//cout << " Aggiorno da " << index_refueling << endl;
								for (int t = index_refueling; t < r.index; t++) {
									//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
									if (r.refueling[t] && t != index_refueling) break;
									//cout << " Ho passato if aggiorno nodo " << t << endl;
									//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
									//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
								}
							}
						}

						//r.print();
					}
				}

			} while (check);
		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
	}
	return solution;
}

vector<Route> destroy_worst(double peso_TW, double peso_intermediate_stop, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			// Primo elemanto la posizione il secondo il numero del nodo
			map<int, int> Node;
			bool check = true;
			int first = 0;
			do {
				// Qui va messa una funzione ch individua il nodo peggiore
				//cout << " ************** Chiamo funzione che mi calcoli il nodo peggiore **************" << endl;
				Node = Compute_WorstNode(peso_TW, peso_intermediate_stop, r, map_airstrip, from_to);
				int node_destroy = Node[first];
				//cout << " La funzione ha finito togli il nodo: " << node_destroy << endl;
				//cout << " Route " << r.aircraft_code << endl;
				//cout << " Node ha una size di " << Node.size() << endl;
				//cout << " Voglio accedere a first = " << first << endl;
				if (node_destroy == 0 || first >= (int)Node.size()) break;
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					//cout << "nodo distrutto: " << node_destroy <<endl;
					//r.print();
					vector<int> int_removed;
					//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
					//cout << " Questa route ha un index = " << r.index << endl; 
					//r.print();

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t] -= r.passengers_in_route[p].capacity;
								r.weight[t] += r.passengers_in_route[p].weight;
								/*
								if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
								r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
								r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
								}
								*/
							}


						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						r.passengers_in_route[int_removed[i]].route_before = index;
						//*************************************************************************************

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}
					// Qua devo mettere codice che � quello dell'Update
					/*
					cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
					cout << "******************* Stampiamo la Route: *******************" << endl;
					cout << " CODE ------> " << r.aircraft_code << endl;
					cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
					cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
					r.print();
					cout << " Node destroyed ------> " << node_destroy << endl;
					*/
					int index_min_from = 0;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
					// Codice che serve per cercare il minimo nel range
					//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
								//cout << " Ho passato IF di controllo " << endl;
								if (r.refueling[i]) break;
								if (r.weight[i] < min_weight && i != node_destroy) {
									min_weight = r.weight[i];
									Node_min = i;
								}
								//}
							}
							//cout << " Nodo di minimo ---> " << Node_min << endl;
							//cout << " Valore di minimi --> " << min_weight << endl;
							if (Node_min >= 0) {
								/*
								for (int i = Node_min; i >= 0; i--) {
								if (r.refueling[i] && i != node_destroy) {
								index_updating_from = i;
								break;
								}
								}
								*/
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
								//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
								double Fuel_before = r.quantity_fuel[index_updating_from];
								//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
								//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
								//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
								//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
				}
				else {

					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
					//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



					if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {   //QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
						check = false;
						vector<int> int_removed;
						//cout << "Nodo: " << node_destroy << endl;
						//r.print();
						int Min_From_Pass = node_destroy;
						int Max_To_Pass = node_destroy;
						for (int p = 0; p < (int)r.passengers_in_route.size(); p++) {
							if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
								if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
								if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
								int_removed.push_back(p);
								for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
									r.capacity[t] -= r.passengers_in_route[p].capacity;
									r.weight[t] += r.passengers_in_route[p].weight;
									/*
									if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
									r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
									r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
									}
									*/
								}

							}
						}




						//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
						//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
						//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
						for (int i = int_removed.size() - 1; i >= 0; i--) {
							// code for repair forbidden***********************************************************
							r.passengers_in_route[int_removed[i]].route_before = index;
							//*************************************************************************************


							passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
							vector<Passenger>::iterator it;
							it = r.passengers_in_route.begin();
							r.passengers_in_route.erase(it + int_removed[i]);
						}
						r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time



						//QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA***************************************************************************************





						int index_before = node_destroy - 1;
						//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
						//r.print();
						//cout << " Adessos sto iniziando a sistemare i pedi perche togliero nodo " << endl;
						double diff = 0;
						//cout << " Index before: " << index_before << endl;
						for (int i = node_destroy + 1; i < r.index; i++) {
							//cout << " sono al i = " << i << endl;
							if (r.refueling[i]) break;
							//cout << "weight before: " << r.weight[i] << endl;
							if (index_before == (node_destroy - 1)) {
								//cout << " sono al primo giro faccio " << endl;
								//cout << "r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed " << r.quantity_fuel[i] <<"="<< r.quantity_fuel[index_before] <<"-"<< fuel_consumed;
								diff = r.quantity_fuel[i];
								//cout << " diff: " << diff << endl;
								r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
								diff = diff - r.quantity_fuel[i];
								//cout << " diff aggiornata: " << diff << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1 + (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);

							}
							else {
								//cout << " non sono al primo giro " << endl;
								r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
								//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							}

							if (r.refueling[node_destroy]) {
								//cout << " caso in cui facevo refuiling nel nodo distrutto " << endl;
								//cout << " al peso del nodo " << i << " aggiorno con fuel_consumed - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]); " << fuel_consumed << "- " << fuel_consumed2 << " - " << (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]) << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
								r.weight[i] = r.weight[i] + diff;
								//cout << "a r.weight[i]=  " << r.weight[i]<<" tolgo questo valore  " <<diff << endl;
							}
							else {
								//cout << " caso odo che ho tolto non era un fuel " << endl;
								//cout << " al peso del nodo " << i << " aggiorno l'attuale " << r.weight[i]<<" con  + "<< fuel_consumed<<" - "<<fuel_consumed2<<" -"<< fuel_consumed1;
								//cout << " weight prima " << r.weight[i] << endl;
								r.weight[i] = r.weight[i] + diff;
								//cout << " weight dopo " << r.weight[i] << endl;
							}


							//cout << "weight after: " << r.weight[i] << endl;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;	

							//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
							//cout << " inidce before = " << index_before << endl;
							index_before = i + 1;
							//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
						}
						//*****************************************************************************************************************************************************************


						r.removePlace(node_destroy, map_airplane);
						//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
						double add_fuel = 0;
						int index_weight_neg = -1;
						for (int j = 0; j < r.index; j++) {
							if (r.weight[j] < 0) {
								//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
								add_fuel = r.weight[j];
								index_weight_neg = j;
								int index_refueling = index_weight_neg;
								for (int i = index_weight_neg; i >= 0; i--) {
									if (r.refueling[i]) { //&& i != node_destroy
										index_refueling = i;
										break;
									}
								}
								//cout << " Aggiorno da " << index_refueling << endl;
								for (int t = index_refueling; t < r.index; t++) {
									//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
									if (r.refueling[t] && t != index_refueling) break;
									//cout << " Ho passato if aggiorno nodo " << t << endl;
									//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
									r.quantity_fuel[t] += add_fuel;
									r.weight[t] -= add_fuel;
									//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
								}
							}
						}

						//r.print();
					}
				}
				//cout << "  Si Esce quando Check = False in questo caso: " << check << endl;
				//cout << " la posizione " << first << " non va bene mi sposto.. " << endl;
				first++;
			} while (check);
		}
		index++;
	}
	return solution;
}

vector<Route> destroy_cluster_aggr2(double peso_TW, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<Passenger> all_passenegr, map<int, Passenger>& map_id_passenger, double& peso_itermediate_stop) {
	int soglia_relateness = 300; // Prima era 100
	vector<int> int_removed;
	vector<Route> route_destroyed;
	vector<Passenger> passengers;

	unordered_map<int, double> CostTWPass;
	set<double, MyCOMP<double>> Myset;


	//srand(time(NULL));
	route_destroyed.insert(route_destroyed.end(), solution.begin(), solution.end());
	passengers.insert(passengers.end(), all_passenegr.begin(), all_passenegr.end());
	//for (Passenger& p : all_passenegr) passengers.push_back(p);

	for (Passenger& p : passengers) CostTWPass.insert(make_pair(p.pnr, 0));
	for (Route& s : route_destroyed) {
		for (Passenger& pass : s.passengers_in_route) {
			int Codpass = pass.pnr;
			CostTWPass[Codpass] += cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW);
			Myset.insert(cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, peso_TW));
		}
	}
	vector<string> OrderVectore;
	for (auto x : Myset) {
		for (auto y : CostTWPass) {
			if (y.second == x) {
				OrderVectore.push_back(to_string(y.first) + "|" + to_string(x));
			}
		}
	}
	/*for (auto x : OrderVectore) {
		cout << " ID: " << split(x, '|')[0] << " con costo: " << split(x, '|')[1] << endl;
		//cout << x << endl;
	}
	system("pause");*/
	map<string, vector<int>> agr_pass;
	vector<string> sequenza;
	for (string x : OrderVectore) {
		//cout << " Devo inserire dentro la mappa che non dovrebbe essere ordinata " << x << endl;
		//cout << "sono split(x, '|')[0]: " << split(x, '|')[0] << endl;
		//system("pause");
		int codice = stoi(split(x, '|')[0]);
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[codice].departure_location) + "|" + to_string(map_id_passenger[codice].arrival_location);
		if (agr_pass.find(code) != agr_pass.end())
		{
			// c'� gia
			agr_pass[code].push_back(map_id_passenger[codice].pnr);
		}
		else {
			// non c'� gia
			sequenza.push_back(code);
			vector<int> id_passeggieri;
			id_passeggieri.push_back(map_id_passenger[codice].pnr);
			agr_pass.insert(make_pair(code, id_passeggieri));
		}

	}

	/*for (string s : sequenza) cout << s << endl;
	for (auto x : agr_pass) {
		cout << " Con queste caratteristiche: " << x.first << " abbiamo : ";
		for (string k : x.second) {
			map_id_passenger[k].print();
			cout << endl;
		}
	}


	//cout << " Aggregati numero di passeggieri pari a ";
	int ndisa = 0;
	for (auto x : agr_pass) ndisa += (int)x.second.size();
	cout << ndisa << endl;
	//cout << " Aggregati numero di passeggieri pari a " << agr_pass.size() << endl;

	*/


	for (int x = 0; x < (int)sequenza.size(); x++) {
		int Codpass = agr_pass[sequenza[x]][0];
		//cout << " Adesso considero String CODPASS da passare alla relateness: " << Codpass << endl;
		//cout << " Tolgo tutti i passeggieri di: " << sequenza[x] << endl;
		// tolgo tutti di questa aggregazione
		for (int p : agr_pass[sequenza[x]]) passenger_removed.push_back(map_id_passenger[p]);
		//cout << " Numero passeggieri rimossi = " << passenger_removed.size() << endl;
		agr_pass.erase(agr_pass.find(sequenza[x]));
		sequenza.erase(sequenza.begin() + x);
		//cout << " Numero di sequenze e grurroi  = " << sequenza.size() << " e " << agr_pass.size()<<  endl;
		for (int y = 0; y < (int)sequenza.size(); y++) {
			//cout << " Sto per chimare funzione relateness per il passeggiero " << i << endl;
			//passengers[i].print();
			// Qui adesso chimao la funzione Relateness !! 
			//cout << " Adesso devo fare il conftonto con  "<< sequenza[y] << endl;
			bool retrocedi = relateness_passenger2(soglia_relateness, from_to, Codpass, map_id_passenger[agr_pass[sequenza[y]][0]], passenger_removed, agr_pass, sequenza, sequenza[y], y, map_id_passenger);
			if (retrocedi) y--;
			//cout << " Abbiamo Valore indice x: " << x << " e Y: " << y << " su: " << sequenza.size() << endl;
		}
		//cout << " Fin'ora ho tolto numero odi passeggieri ----> " << passenger_removed.size() << " su " << num_passenger << endl;
		x--;
		if ((int)passenger_removed.size() >= num_passenger) break;
	}

	//int NRimossi = 0;
	int Npass = 0;
	int Nroute = -1;
	//for (Route r : route_destroyed) Npass += r.passengers_in_route.size();
	// Devo toglierli dalla route 
	for (Route& s : route_destroyed) {
		Nroute++;
		vector<int> int_removed;
		bool Update = false;
		int Min_From_Pass = s.index;
		int Max_To_Pass = -1;
		for (int p = 0; p < (int)s.passengers_in_route.size(); p++) {
			for (Passenger& pass : passenger_removed) {
				//if (s.passengers_in_route[p].name == pass.name && s.passengers_in_route[p].surname == pass.surname && s.passengers_in_route[p].pnr == pass.pnr && s.passengers_in_route[p].code_flight == pass.code_flight && s.passengers_in_route[p].gender == pass.gender  && s.passengers_in_route[p].arrival_location == pass.arrival_location  && s.passengers_in_route[p].departure_location == pass.departure_location && s.passengers_in_route[p].weight == pass.weight) {
				if (s.passengers_in_route[p].pnr == pass.pnr) {
					Update = true;
					pass.route_before = Nroute;
					if (s.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = s.passengers_in_route[p].solution_from;
					if (s.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = s.passengers_in_route[p].solution_to;
					//cout << " devo rimuovere passeggiero dalla route dell'aereo " << s.aircraft_code << endl;
					//s.passengers_in_route[p].print();
					int_removed.push_back(p);
					for (int t = s.passengers_in_route[p].solution_from; t < s.passengers_in_route[p].solution_to; t++) {
						s.capacity[t] -= s.passengers_in_route[p].capacity;
						s.weight[t] += s.passengers_in_route[p].weight;
					}

				}
			}


		}
		for (int i = int_removed.size() - 1; i >= 0; i--) {
			//cout << int_removed[i] << endl;
			vector<Passenger>::iterator it;
			it = s.passengers_in_route.begin();
			s.passengers_in_route.erase(it + int_removed[i]);
			//cout << " Route adesso ha ---> " << s.passengers_in_route.size() << " passeggieri" << endl;
			//cout << " Stampo passeggieri dopo rimozione: " << endl;
			//for (Passenger passeggiero : s.passengers_in_route) passeggiero.print(); 
		}

		if (Update) {


			// Codice che da reduel ai nodi con refuel non massimo 
			int index_min_from = Min_From_Pass;
			for (int i = Min_From_Pass; i >= 0; i--) {
				if (s.refueling[i]) {
					index_min_from = i;
					break;
				}
			}
			for (int k = index_min_from; k < Max_To_Pass; k++) {
				if (s.refueling[k] && s.quantity_fuel[k] < map_airplane[s.aircraft_code].max_fuel) { //&& k!= node_destroy
				//cout << " Sto valutando il caso del nodo " << k << endl;
					int index_updating_from = k;
					int index_updating_to = s.index;
					int Node_min = k;
					double min_weight = s.weight[k];
					//appena aggiunto
					for (int i = Max_To_Pass; i < s.index; i++) {
						if (s.refueling[i]) break;
						Max_To_Pass = i;
					}

					for (int i = k + 1; i <= Max_To_Pass; i++) {
						//cout << " Sto guardando il nodo " << i << endl;
						//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
						//cout << " Ho passato IF di controllo " << endl;
						if (s.refueling[i]) break;
						if (s.weight[i] < min_weight) {
							min_weight = s.weight[i];
							Node_min = i;
						}
						//}
					}
					if (Node_min >= 0) {
						for (int i = k + 1; i < s.index; i++) {
							if (s.refueling[i]) {  // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
								index_updating_to = i;
								break;
							}
						}
						//cout << " Minimo trovato " << min_weight << endl;
						//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
						//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
						double Fuel_before = s.quantity_fuel[index_updating_from];
						//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[aircraft_code].max_fuel << " e " << quantity_fuel[index_updating_from] + min_weight << endl;
						s.quantity_fuel[index_updating_from] = min(map_airplane[s.aircraft_code].max_fuel, s.quantity_fuel[index_updating_from] + min_weight);
						s.weight[index_updating_from] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						for (int i = index_updating_from + 1; i < index_updating_to; i++) {
							s.quantity_fuel[i] += (s.quantity_fuel[index_updating_from] - Fuel_before);
							s.weight[i] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						}
					}

				}

			}




			int index_sup = s.index;
			for (int i = index_sup - 1; i > 1; i--) {

				if (s.capacity[i - 1] != 0) break;

				if (s.capacity[i - 1] == 0) {
					s.places.erase(s.places.begin() + i);
					s.time_arr.erase(s.time_arr.begin() + i);
					s.time_dep.erase(s.time_dep.begin() + i);
					s.refueling.erase(s.refueling.begin() + i);
					s.quantity_fuel.erase(s.quantity_fuel.begin() + i);
					s.weight.erase(s.weight.begin() + i);
					s.capacity.erase(s.capacity.begin() + i);
					s.index = s.index - 1;

				}
			}


			if (s.index == 2 && s.capacity[0] == 0) {
				s.places.erase(s.places.begin() + 1);
				s.time_arr.erase(s.time_arr.begin() + 1);
				s.time_dep.erase(s.time_dep.begin() + 1);
				s.refueling.erase(s.refueling.begin() + 1);
				s.quantity_fuel.erase(s.quantity_fuel.begin() + 1);
				s.weight.erase(s.weight.begin() + 1);
				s.capacity.erase(s.capacity.begin() + 1);
				s.index = s.index - 1;
			}

			// Questa parte qua forse si puo togliere
			//qua � la parte che ho aggiunto io (NELLI) per il problema del nodo al deposito che non si aggiorna
			if ((int)s.places.size() == 1 && s.capacity[0] == 0) {
				s.quantity_fuel[0] = map_airplane[s.aircraft_code].max_fuel;
				s.weight[0] = map_airplane[s.aircraft_code].weight_fuel_people - map_airplane[s.aircraft_code].max_fuel;
			}


		}
	}




	Npass = 0;
	for (Route& s : route_destroyed) {
		Npass += (int)(s.passengers_in_route.size());
		if (s.passengers_in_route.size() == 0) s.primo_pass = false;
	}
	//cout << " Numero di passeggieri nella route dopo averli toglierli " << Npass << endl;



	return route_destroyed;

}




//ECCO I NOSTRI REPAIR
vector <Route> repair_one(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//cout << endl;
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			//cout << "Route: " << r << endl;
			//routes_destroyed[r].print();
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'� solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
					//cout << "costo caso primo passeggero: " << cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				//int arc_from = -1;
				//int arc_to = -1;
				double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
						for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes_destroyed[r].places[n1]) {
								//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
								//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);
								//cout << "route per il check" << endl;
								//r_for_check.print();
								/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
									cout << "caso strano: " << endl;
									r_support.print();
									cin.get();
								}*/


								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//r_support.print();
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//cout << "ristampo i valori" << endl;
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//cout << endl;
									//stampo_caso_strano_single_all(r_support, from_to, map_airplane);
									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
											//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
											if (best_cost > cost) {
												//best_arc_from = n;
												//best_arc_to = n1;
												best_route = r;
												best_cost = cost;
												move_c = false;

												route_best = r_support;
												from_per_route = p.solution_from;
												to_per_route = p.solution_to;
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//for move C
			if (routes_destroyed[r].primo_pass) {
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
					double dist = from_to[p.departure_location][p.arrival_location];
					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
					//cout << "tempo di volo: " << time << endl;
					double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					/*if (time <= 60) {
						fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

					double time_window_cost = 0.0;
					double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
					double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
					if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

					if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

					double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
					//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
					//cout << "cost for move c" << cost << endl;
					//cout << t_arr_arrival << " : tempo arrivo" << endl;
					if (best_cost > cost) {
						if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
							if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
								//best_arc_from = -2;
								//best_arc_to = -2;
							}
						}
					}
				}
				else {
					// the departure is not equals to the last place of the route
					double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
					double TW_departure = 0.0;
					double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;



					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
					//cout << "cost for move c" << cost << endl;
					//cout << t_arr_arrival << " : tempo arrivo" << endl;
					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
								}
							}
						}
					}
				}
			}

		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "caso primo passeggero" << endl;
					//routes_destroyed[best_route].print();
				}
				//else {
					//ci sono pi� nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "caso mossa C" << endl;
					//routes_destroyed[best_route].print();

				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;
					//cout << "route prima dell'inserimento" << endl;
					//routes_destroyed[best_route].print();
					//cout << "caso inserimento speciale ------>" << best_arc_from << " - " << best_arc_to << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//routes_destroyed[best_route].print();
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector<Route> two_regret_repair_aggragati(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//bool routes_infeasible = false;
	vector<double> regret_diff;
	vector<int> regret_move_c; //1 move C, 0 no move C
	vector<int> regret_best_route;
	// if -1 = infeasible, passenger can insert in no-route
	// if 0...route.size() normal inserition with the function
	vector<int> regret_arc_from;
	vector<int> regret_arc_to;
	vector<int> regret_index_pass;

	map<int, int> map_pass_aggragati; //nella stringa, che � la chiave, ci sono from/to/timedep/timearr, nell'int c'� l'indicie di tutti i valori



	//cout << "ecco il numero di passeggeri rimossi " << passengers_removed.size();
	while (!passengers_removed.empty()) {
		int index_p = 0;
		for (Passenger& p : passengers_removed) {
			//p.print();

			vector<double> costs;
			vector<int> route;
			vector<int> arc_from;
			vector<int> arc_to;
			vector<int> move_c;


			//string chiave = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
			int chiave = p.chiave_regret;
			if (map_pass_aggragati.count(chiave) >= 1) {
				//cout << "ho gia calcolato un passeggero simile a lui" << endl;

				regret_diff.push_back(regret_diff[map_pass_aggragati[chiave]]);
				regret_move_c.push_back(regret_move_c[map_pass_aggragati[chiave]]);
				regret_best_route.push_back(regret_best_route[map_pass_aggragati[chiave]]);
				regret_arc_from.push_back(regret_arc_from[map_pass_aggragati[chiave]]);
				regret_arc_to.push_back(regret_arc_to[map_pass_aggragati[chiave]]);
				regret_index_pass.push_back(regret_index_pass[map_pass_aggragati[chiave]]);


			}
			else {


				//Route r_support; //attento qua, � quello di prova
				for (int r = 0; r < (int)routes_destroyed.size(); r++) {
					//cout << "nuova route-----------> " << r << endl;
					//routes_destroyed[r].print();
					if (routes_destroyed[r].primo_pass == false) {
						if (routes_destroyed[r].index == 1) {
							//c'� solo il deposito in questo caso
							double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
							cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
							cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

							//cout << "costo caso primo passeggero: " << cost << endl;

							//cout << "cost: " << cost << " from: " << -1 << " to: " << -1 << " route: " << r << " mossa c: " << 0 << endl;
							costs.push_back(cost);
							route.push_back(r);
							arc_from.push_back(-1);
							arc_to.push_back(-1);
							move_c.push_back(0);
						}
					}
					else {
						//int arc_from1 = -1;
						//int arc_to1 = -1;
						double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						//cout << "inizio for" << endl;
						for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
								for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										Route r_support = routes_destroyed[r];
										//r_support = routes_destroyed[r];
										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
										//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);


										if (r_support.time_arr[r_support.index - 1] <= end_day) {


											r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
													//cout << "prima che calcolo il cost" << endl;
													double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << "dopo che calcolo il cost" << endl;
													if (costs.size() == 0) {
														//cout << "cost: " << cost << " from: " << n << " to: " << n1 << " route: " << r << " mossa c: " << 0 << endl;
														costs.push_back(cost);
														arc_from.push_back(n);
														arc_to.push_back(n1);
														route.push_back(r);
														move_c.push_back(0);
													}

													if (costs.size() >= 1) {
														if (cost != costs[costs.size() - 1]) {
															costs.push_back(cost);
															arc_from.push_back(n);
															arc_to.push_back(n1);
															route.push_back(r);
															move_c.push_back(0);
															//cout << "cost: " << cost << " from: " << n << " to: " << n1 << " route: " << r << " mossa c: " << 0 << endl;
														}
													}
													//cout << "route " << r << "archi: " << arc_from1 << " _ " << n1 << endl;
												}
											}
										}
									}
								}
							}
						}
						//cout << "fine for" << endl;
					}

					//for move C
					if (routes_destroyed[r].primo_pass) {
						//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
						if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
							//cout << "sto considerando la mossa C" << endl;
							double dist = from_to[p.departure_location][p.arrival_location];
							double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
							double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];


							double time_window_cost = 0.0;
							//double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
							//double t_arr_arrival = (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time);
							if (routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] < p.early_departure) time_window_cost += p.early_departure - routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
							else if (routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] > p.late_departure) time_window_cost += routes_destroyed[r].time_arr[routes_destroyed[r].index - 1] - p.late_departure;

							if ((routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) < p.early_arrival) time_window_cost += p.early_arrival - (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time);
							else if ((routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) > p.late_arrival) time_window_cost += (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time) - p.late_arrival;

							double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
							//cout << "cost for move c" << cost << endl;
							//cout << routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time << " <= " << end_day << " && " << routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed << " >= " << map_airplane[routes_destroyed[r].aircraft_code].min_fuel << endl;
							if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
								if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									costs.push_back(cost);
									route.push_back(r);
									move_c.push_back(1);
									arc_from.push_back(-2);
									arc_to.push_back(-2);

									//cout << "cost: " << cost << " from: " << -2 << " to: " << -2 << " route: " << r << " mossa c: " << 1 << endl;
								}
							}
						}
						else {
							// the departure is not equals to the last place of the route
							double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
							double TW_departure = 0.0;
							double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
							if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
							else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

							double TW_arrival = 0.0;
							double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
							if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
							else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
							cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

							//per il check sul fuel:
							//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
							double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
							double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

							cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
							//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
							//refuel
							double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
							double f_after_secondo_tratto = 0.0;
							if (map_airstrip[p.departure_location].fuel) {
								f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
							}
							else {
								f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
							}

							//cout << "cost for move c" << cost << endl;
							//cout << t_arr_arrival << " <= " << end_day << " && " << f_after_primo_tratto << " >= " << map_airplane[routes_destroyed[r].aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << map_airplane[routes_destroyed[r].aircraft_code].min_fuel << endl;
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
										costs.push_back(cost);
										move_c.push_back(1);
										arc_from.push_back(-2);
										arc_to.push_back(-2);
										route.push_back(r);

										//cout << "cost: " << cost << " from: " << -2 << " to: " << -2 << " route: " << r << " mossa c: " << 1 << endl;
									}
								}
							}
						}
					}
				}



				if (costs.size() == 1) {

					//cout << "il passegero:";
					//p.print();
					//cout << " ha solo una mossa di inserimento possibile" << endl;


					//regret_diff.push_back(0);
					regret_diff.push_back(0);  //metto valore zero ma non dovrebbe succedere
					regret_best_route.push_back(route[0]);
					regret_move_c.push_back(move_c[0]);
					regret_arc_from.push_back(arc_from[0]);
					regret_arc_to.push_back(arc_to[0]);
					regret_index_pass.push_back(index_p);
				}
				else if (costs.size() > 1) {
					//qua lo posso ottimizzare****************************************************************************************************************************************
					//con il codice di TED per ordinare, guarda bene come salvare l'indice********************************************************************************************


					vector<double>::iterator index_iterator = min_element(costs.begin(), costs.end());
					int index_mosse = distance(costs.begin(), index_iterator);
					double best_cost = costs[index_mosse];
					costs.erase(costs.begin() + index_mosse);
					vector<double>::iterator index_iterator2 = min_element(costs.begin(), costs.end());
					double best_cost_2 = costs[distance(costs.begin(), index_iterator2)];



					regret_diff.push_back(best_cost_2 - best_cost);
					regret_best_route.push_back(route[index_mosse]);
					regret_move_c.push_back(move_c[index_mosse]);
					regret_arc_from.push_back(arc_from[index_mosse]);
					regret_arc_to.push_back(arc_to[index_mosse]);
					regret_index_pass.push_back(index_p);
				}
				else {
					//caso in cui non ha trovato mosse per quella persona
					//in questo caso vuol dire che la solutione � infeasible
					cout << "LA SOLUZIONE E' INFEASIBLE" << endl;
					vector<Route> route_vuote;
					return route_vuote;

				}

				//cout << "regret.size() - 1 " << (regret_diff.size() - 1) << endl;
				int val = (int)regret_diff.size() - 1;
				//cout << val << endl;
				//string primo_val = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
				int primo_val = p.chiave_regret;
				map_pass_aggragati.insert(make_pair(primo_val, val));
				//cout << map_pass_aggragati[primo_val] << endl;


				costs.clear();
				route.clear();
				arc_from.clear();
				arc_to.clear();
				move_c.clear();

			}

			index_p++;
		}

		//cout << "regret.size(): " << regret_best_route.size() << endl;




		int index = 0;
		double best = DBL_MIN; //prima era -10000
		for (int i = 0; i < (int)regret_diff.size(); i++) {
			//cout << "***********" << regret_diff[i] << endl;

			if (best < regret_diff[i]) {
				index = i;
				best = regret_diff[i];
			}
		}



		if (!routes_destroyed[regret_best_route[index]].primo_pass) {
			//c'� solo il depot
			//cout << "devo mettere il passeggero dentro" << endl;
			routes_destroyed[regret_best_route[index]].primo_pass = true;
			routes_destroyed[regret_best_route[index]].add_update_only_one_node_first_passanger(passengers_removed[regret_index_pass[index]], from_to, map_airplane, map_airstrip, from_to_FuelConsumed);

		}
		else {

			if (regret_move_c[index] == 1) {
				//i have to do the move C
				//cout << "devo mettere il passeggero dentro" << endl;
				routes_destroyed[regret_best_route[index]].move_c(passengers_removed[regret_index_pass[index]], passengers_removed[regret_index_pass[index]].departure_location, passengers_removed[regret_index_pass[index]].arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
			}
			else {
				//cout << "devo mettere il passeggero dentro" << endl;
				routes_destroyed[regret_best_route[index]].update_route_rebuilt_one(regret_arc_from[index], regret_arc_to[index], passengers_removed[regret_index_pass[index]].departure_location, passengers_removed[regret_index_pass[index]].arrival_location, from_to, map_airplane, map_airstrip, passengers_removed[regret_index_pass[index]], from_to_FuelConsumed);
				//cout << "ho messo il passeggero dentro" << endl;
			}
		}


		routes_destroyed[regret_best_route[index]].passengers_in_route.push_back(passengers_removed[regret_index_pass[index]]);


		//QUA DEVO TOGLIERE IL PASSEGGERO CHE HO APPENA INSERITO DAL POOL DI PASSENGERS_REMOVED
		vector<Passenger>::iterator it;
		it = passengers_removed.begin();
		passengers_removed.erase(it + regret_index_pass[index]);



		regret_diff.clear();
		regret_move_c.clear(); //1 move C, 0 no move C
		regret_best_route.clear();
		regret_arc_from.clear();
		regret_arc_to.clear();
		regret_index_pass.clear();
		map_pass_aggragati.clear();


		//cout << "size dei passeggeri rimossi " << passengers_removed.size() << endl;
	}


	return routes_destroyed;

}

vector <Route> repair_forbidden(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (r != p.route_before) {
				if (routes_destroyed[r].primo_pass == false) {
					if (routes_destroyed[r].index == 1) {
						//c'� solo il deposito in questo caso
						double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
						cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
						cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

						//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
						//cout << "costo caso primo passeggero: " << cost << endl;
						//cout << r << " != " << p.route_before << endl;
						if (cost < best_cost) {
							best_cost = cost;
							best_route = r;
							case_first_passenger = false;
						}
					}
				}
				else {
					//int arc_from = -1;
					//int arc_to = -1;
					double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
						if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
							for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
								if (p.arrival_location != routes_destroyed[r].places[n1]) {
									Route r_support = routes_destroyed[r];
									bool non_to = false;
									bool non_to_final = false;
									bool num_equals = false;
									int caso = -1;
									int node_add_from = n;
									int node_add_to = n1;

									r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);
									/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
										cout << "caso strano: " << endl;
										r_support.print();
										cin.get();
									}*/

									if (r_support.time_arr[r_support.index - 1] <= end_day) {
										//p.print();
										//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
										//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
										//r_support.print();
										r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
										//cout << "ristampo i valori" << endl;
										//p.print();
										//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
										//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
										//cout << endl;
										//stampo_caso_strano_single_all(r_support, from_to, map_airplane);

										if ((p.solution_to - p.solution_from <= p.stop)) {
											if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
												double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
												if (best_cost > cost) {
													//best_arc_from = n;
													//best_arc_to = n1;
													best_route = r;
													best_cost = cost;
													move_c = false;

													route_best = r_support;
													from_per_route = p.solution_from;
													to_per_route = p.solution_to;
												}
											}
										}
									}
								}
							}
						}
					}

				}

				//for move C
				if (routes_destroyed[r].primo_pass) {
					//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
					if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
						double dist = from_to[p.departure_location][p.arrival_location];
						double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
						//cout << "tempo di volo: " << time << endl;
						double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
						/*if (time <= 60) {
							fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
						}*/

						double time_window_cost = 0.0;
						double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
						double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
						if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

						if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

						double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
						//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
						//cout << "cost for move c" << cost << endl;
						//cout << t_arr_arrival << " : tempo arrivo" << endl;
						//cout << r << " != " << p.route_before << endl;
						if (best_cost > cost) {
							if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
								if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
								}
							}
						}
					}
					else {
						// the departure is not equals to the last place of the route
						double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
						double TW_departure = 0.0;
						double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
						if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
						if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
						cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
						/*if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
						}*/
						//****
						//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
						cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
						/*if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
						}*/

						//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
						//cout << "cost for move c" << cost << endl;
						//cout << t_arr_arrival << " : tempo arrivo" << endl;
						//cout << r << " != " << p.route_before << endl;
						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
										best_cost = cost;
										best_route = r;
										move_c = true;
										//best_arc_from = -2;
										//best_arc_to = -2;
									}
								}
							}
						}
					}
				}
			}
		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				//else {
					//ci sono pi� nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "dopo la mossa C" << endl;
				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//cout << "dopo dell'inserimento speciale" << endl;
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector <Route> repair_perturbation(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route>& routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger>& passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger& p : passengers_removed) {
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'� solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
					//cout << "costo caso primo passeggero: " << cost << endl;

					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************

					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				//int arc_from = -1;
				//int arc_to = -1;
				double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
						for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes_destroyed[r].places[n1]) {
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
								//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);

								/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
									cout << "caso strano: " << endl;
									r_support.print();
									cin.get();
								}*/

								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//r_support.print();
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//cout << "ristampo i valori" << endl;
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//cout << endl;
									//stampo_caso_strano_single_all(r_support, from_to, map_airplane);

									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;

											//code for casual number [0.8;1.2]*****************************************************
											double f = (double)rand() / RAND_MAX;
											f = 0.8 + (f * 0.4);
											cost = cost * f;
											//*************************************************************************************

											if (best_cost > cost) {
												//best_arc_from = n;
												//best_arc_to = n1;
												best_route = r;
												best_cost = cost;
												move_c = false;
												route_best = r_support;
												from_per_route = p.solution_from;
												to_per_route = p.solution_to;
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//for move C
			if (routes_destroyed[r].primo_pass) {
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
					double dist = from_to[p.departure_location][p.arrival_location];
					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
					//cout << "tempo di volo: " << time << endl;
					double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					/*if (time <= 60) {
						fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

					double time_window_cost = 0.0;
					double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
					double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
					if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

					if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

					double cost = dist + (time_window_cost * peso_TW) + fuel_consumed;

					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************


					if (best_cost > cost) {
						if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
							if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
								//best_arc_from = -2;
								//best_arc_to = -2;
							}
						}
					}
				}
				else {
					// the departure is not equals to the last place of the route
					double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
					double TW_departure = 0.0;
					double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
					/*if (travel_primo_tratto <= 1) {
						fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_primo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/
					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;
					/*if (travel_secondo_tratto <= 1) {
						fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}


					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************


					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
								}
							}
						}
					}
				}
			}

		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				//else {
					//ci sono pi� nodi
					//routes_destroyed[best_route].primo_pass = true;
					//routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "dopo la mossa C" << endl;
				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//cout << "dopo dell'inserimento speciale" << endl;
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}





//in questi ancora non messi

void heuristic_costructive_first_fase_secIter(double peso_TW, double peso_intermediate_stop, vector<Route>& solution, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//cout << " ----------------- Sono al secondo giro adesso ho gia soddisfatto alcuni passeggieri me ne mancano alcuni -----------------" << endl;
	int situation = -1;
	int cont = 0;
	vector<int> AereiUsati;
	for (Route& r : solution) AereiUsati.push_back(r.aircraft_code);
	//cout << "  Stringa: " << AereiUsati << endl;
	do {
		std::vector<int>::iterator it = std::find(AereiUsati.begin(), AereiUsati.end(), airplanes[cont].code);
		if (it == AereiUsati.end()) {
			vector<Passenger> pass_in_solution;
			//cout << " Sto guardando aereo " << airplanes[cont].code << endl;
			Route r(airplanes[cont].code, pass_in_solution);
			r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
			//cout << " Ho cerato un aroute per l'aereo " << r.aircraft_code << endl;
			solution.push_back(r);
		}
		cont++;
	} while (cont < number_of_aircraft);
	//cout << " ---------------------Check: Adesso dovrei avere a disposizione 20 route ---------------  = " << solution.size() << endl;

	for (auto& p : passengers) {
		//cout << "Sono il passeggero**********************" << endl;
		//p.print();
		situation = -1;
		int best_route = -1; //index of the best route where can i put my passenger
		int best_from = -1;
		int best_to = -1;
		double best_cost = DBL_MAX;
		int i = 0;
		//cout << "- sto calcolando dove metttere il passeggero: " << p.name << endl;
		//qui calcolo solo dove mettere il passeggero
		for (auto& r : solution) {
			if (r.primo_pass == false) {
				if (r.places[r.index - 1] == p.departure_location) {
					//in questo caso c'� solo lui nella route, il costo � dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
					//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					/*
					if (travel_time <= 1) {
						fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/

					cost += fuel_consumed;
					//per il check sul fuel:
					double fuel_after_trip = 0.0;
					fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
					//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
							best_cost = cost;
							best_route = i;
							situation = 1;
						}
					}
				}
				else {
					//qui c'� solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi� kilometri e un landing stop
					//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
					double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
					//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					cost += fuel_consumed;

					//per il check sul fuel
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
					/*
					if (travel_primo_tratto <= 1) {
						fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					//****
					//double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
					if (best_cost > cost) {
						if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
							if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = 2;
							}
						}
					}
				}
			}
			else {
				//nella route c'� almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit� della route
				//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'� il FROM, non cerco il TO
				vector<int> FROM;
				vector<int> TO;
				for (int t = 0; t < r.index; t++) {
					if (r.places[t] == p.departure_location) FROM.push_back(t);
				}

				if (FROM.size() != 0) {
					//dentro questo abbiamo trovato dei FROM 
					for (int t = FROM[0]; t < r.index; t++) {
						if (r.places[t] == p.arrival_location) TO.push_back(t);
					}

					if (TO.size() != 0) {
						for (auto from : FROM) {
							for (auto to : TO) {
								if (from < to && to - from <= p.stop) {
									//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
									//se sono qua dentro il to � maggiore del from e gli stop intermedi sono minori di tre
									//per prima cosa guardo se ci sto con la capacit�, altrimenti break sul primo for
									bool capacity_satisfy = true;
									for (int c = from; c < to; c++) {
										if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										/*
										if (travel_time <= 1) {
											fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
										}
										else {
											fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
										}
										*/
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
										if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

										//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

									}
									//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
									if (!capacity_satisfy) break; //check also for the fuel;





									//se sorpasso il break vuol dire che soddisfo la capacit�, in questo caso il solo costo che consider� per l'aggiunta del passeggero qui
									//� la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
									else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

									double TW_arrival = 0.0;
									//double t_arr_arrival = r.time_arr[to];
									if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
									else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

									double cost = (((TW_departure + TW_arrival) * peso_TW) + ((to - from - 1) * peso_intermediate_stop)) * p.capacity;
									if (best_cost > cost) {
										best_cost = cost;
										best_route = i;
										situation = 4;
										best_from = from;
										best_to = to;
									}
								}
							}
						}
					}
					//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
					//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
					if (r.places[r.index - 1] != p.arrival_location) {
						for (auto from : FROM) {
							bool capacity_satisfy = true;
							for (int c = from; c < r.index; c++) {
								if ((r.capacity[c] + p.capacity) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

								if (c < r.index - 1) {
									//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];

									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

									if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
								}
							}
							//se non soddisfa la capacit� non guardo neanche e passo al TO successivo
							//cout << p.stop<< endl;
							if (capacity_satisfy && (r.index - from) <= p.stop) {

								double TW_departure = 0.0;
								//double t_arr_departure = r.time_arr[from];
								if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
								else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;

								double cost = (TW_departure * peso_TW);
								// ora ci metto chilometri e fuel dell'ultimo pezzo;
								cost += from_to[r.places[r.index - 1]][p.arrival_location];
								double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];
								/*
								if (travel_time <= 1) {
									fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
								}
								else {
									fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
								}
								*/
								cost += fuel_consumed;

								bool fuel_ok = true;
								if (!r.refueling[r.index - 1]) {
									double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
									if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
								}

								double TW_arrival = 0.0;
								double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
								if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
								else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
								cost += (TW_arrival * peso_TW) * p.capacity;

								cost += ((peso_intermediate_stop * (r.index - from - 1))) * p.capacity;

								if (fuel_ok) {
									if (best_cost > cost) {
										if (t_arr_arrival <= end_day) {
											if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
												best_cost = cost;
												best_route = i;
												best_from = from;
												situation = 5;
											}

										}
									}
								}
							}
						}
					}

				}
				if (r.places[r.index - 1] != p.departure_location) {
					//guardo il costo di aggiungere un pezzo alla fine
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					/*
					if (travel_time <= 1) {
						fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

					double TW_departure = 0.0;
					double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];

					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
					/*if (travel_secondo_tratto <= 1) {
						fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
					}*/

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}



					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = 3;
								}
							}
						}
					}

				}
			}
			i++;
		}

		//cout << " caso : " << situation << " for the passeger:" << endl;
		//p.print();
		//ora devo aggiungere il passeggero nel posto migliore, quindi serve valutare in che caso sono A,B,C,D
		if (situation == 1) {
			solution[best_route].primo_pass = true;
			//time
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto � la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1] += p.capacity;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}
			//end for weight




			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);





		}
		else if (situation == 2) {
			solution[best_route].primo_pass = true;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 -
				map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;




			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);




		}
		else if (situation == 3) {

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, p.capacity,
					solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;


			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, p.capacity, solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60);





			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}


			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 4) {



			for (int h = best_from; h < best_to; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;
			}

			//**************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//**************************************************************************




			p.solution_from = best_from;
			p.solution_to = best_to;
			solution[best_route].passengers_in_route.push_back(p);
		}
		else if (situation == 5) {


			for (int h = best_from; h < solution[best_route].index; h++) {
				solution[best_route].capacity[h] += p.capacity;
				solution[best_route].weight[h] -= p.weight;


			}

			//******************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//******************************************************************************

			p.solution_from = best_from;
			int place_1 = solution[best_route].places[solution[best_route].index - 1];
			double aircraft_speed = map_airplane[solution[best_route].aircraft_code].speed;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;


				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}

			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);

		}
		else if (situation == -1) {
			//cout << "*******************************************************il passeggero: " << p.name << " non trova sistemazione" << endl;
			//p.print();
			//for (Route r : solution) r.print();
			//system("pause");
		}
	}


	//cout << " -------------- Ho finito euristico seconda fase adesso devo cancellare le route in eccesso  ----------- " << endl;
	//cout << " -- Alla fine in questa fase vado a cancellare le route che non sono state prese ma che eranoo state create -------" << endl;
	//cout << " ------------------------- Ne ho prima di cancellarne:  --------------------- " << solution.size() << endl;
	vector<int> Elimina;
	for (int s = 0; s < (int)solution.size(); s++) {
		if (solution[s].index <= 1) {
			Elimina.push_back(s);
		}
	}

	for (int i = (int)(Elimina.size()) - 1; i >= 0; i--) {
		solution.erase(solution.begin() + Elimina[i]);
	}


	//cout << " ------------------------- Ne ho dopo la cancellazione:  --------------------- " << solution.size() << endl;

}

vector<Route> repairSP(double peso_TW, double peso_intermediate_stop, vector<Route>& route_destroy, vector<Passenger>& passenger_removed, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<Airplane>& airplanes, double end_day, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<vector<Route>> solutionAllSub;
	int NAirplane = 20;
	/* cout << " Stampo Le route distrutte: " << endl;
	int Npass = 0;
	for (Route r : route_destroy) {
		r.print();
		Npass += r.passengers_in_route.size();
	}
	cout << " Dopo la distruzione ho " << Npass << endl;
	*/
	double dividendo = (double)(number_of_aircraft * passenger_removed.size());
	double denominatore = (double)(passengers.size());
	int number_of_aircraft_subset = 10 + (int)(round(dividendo / denominatore));
	if (number_of_aircraft_subset > number_of_aircraft)  number_of_aircraft_subset = number_of_aircraft;
	int iteration_subset = 0;
	int iteration_max_subset = (int)(passenger_removed.size()) / 3;
	//cout << " Numero di Itearazioni per questa distruzione: " << iteration_max_subset << endl;
	int Best_iteration = 0;
	double Best_cost = std::numeric_limits<double>::max();
	do {
		//cout << " ------------------ Sono al Itarzione del sottoinsime numero = " << iteration << " Di: " << iteration_max << " ---------------- " << endl;
		//cout << " ------------------ Sono al Itarzione del Sort numero = " << iteration_subset << " Di: " << iteration_max_subset << " ---------------- " << endl;
		// Lo ordino in modo random
		auto rng2 = default_random_engine{};
		shuffle(passenger_removed.begin(), passenger_removed.end(), rng2);
		vector<Route> solution;
		vector<Route> Route_to_fix = route_destroy;
		//cout << " Route da dare al costruittivo = "<< Route_to_fix.size() << endl;
		heuristic_costructive_first_fase_secIter(peso_TW, peso_intermediate_stop, Route_to_fix, airplanes, map_airplane, map_airstrip, end_day, passenger_removed, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
		double cost_fix_single = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, Route_to_fix, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		//cout << "The cost of the objective function first fase is: " << cost_objectiveFunction_fisrt_fase << endl;
		//cout << "***********************SOLUTION SECOND FASE***********************" << endl;
		solution = heuristic_costructive_second_fase_SP(Route_to_fix, end_day, peso_TW);
		cost_fix_single = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		if (cost_fix_single < Best_cost) {
			Best_cost = cost_fix_single;
			Best_iteration = iteration_subset;
		}
		//cout << "The cost of the objective function second fase is: " << cost_objectiveFunction_second_fase << endl;
		//cout << " Passo al modello una soluzione di route numero " << solution.size();

		solutionAllSub.push_back(solution);
		iteration_subset++;
	} while (iteration_subset < iteration_max_subset);
	//cout << " CHECK -----> Passo al modello un numero di soluzioni ---> " << solutionAllSub.size() << endl;
	//cout << " CHECK -----> Passo al modello un numero di passeggieri ---> " << passengers.size() << endl;
	//cout << " CHECK -----> La migliore iterazione e la numero ---> " << Best_iteration << endl;
	//cout << " CHECK -----> Costo migliore iterazione  ---> " << Best_cost << endl;
	//Mi preparo per chiamare il modello con il Pool di route generato
	vector<Airplane> airplanes_model;
	for (auto& k : map_airplane) {
		airplanes_model.push_back(k.second);
	}
	map<int, vector<Route>> airplane_routes;
	for (Airplane& a : airplanes_model) {
		vector<Route> routes;
		airplane_routes.insert(make_pair(a.code, routes));
	}
	for (auto& solution : solutionAllSub) {
		for (Route& r : solution) {
			airplane_routes[r.aircraft_code].push_back(r);
		}
	}






	vector<vector<vector<int>>> A3;  //matrix A
	vector<vector<double>> C;  //cost
	vector<Route> routes;
	for (auto& airplane : airplanes_model) {
		//cout << "Airplane: " << airplane.code << endl;
		vector<double> c;
		vector<vector<int>> A2;
		for (Route& r : airplane_routes[airplane.code]) {
			//r.print();
			routes.push_back(r);
			vector<int> A1;
			for (Passenger& p : passengers) {
				auto it = find_if(r.passengers_in_route.begin(), r.passengers_in_route.end(),
					[=](Passenger p_)
				{
					return p.pnr == p_.pnr;
				});
				if (it != r.passengers_in_route.end())
				{
					A1.push_back(1);


				}
				else {
					A1.push_back(0);
				}
			}
			A2.push_back(A1);
			//cout << "Costo: " << r.cost << endl;
			c.push_back(r.cost);
		}
		A3.push_back(A2);
		C.push_back(c);
	}
	//cout << "..................Finish to build all the structure for the Model.................." << endl;
	//cout << " Al modello sto passando un numero di Route generate: " << routes.size() << endl;
	//Creating model with gurobi
	//Create new Model object
	//Mi preparo a ricere le route scelte dal modello
	vector<Route> solution_model;
	Model_Cplex* model = new Model_Cplex(A3, C, airplanes_model, airplane_routes, routes, (int)(passengers.size()), passengers, solution_model, map_airplane);
	//Create and solve model with gurobi
	model->create_Model_for_SP_cplex(NAirplane);
	// adesso vettore delle Route non � piu vuoto
	// Non ho piu tutti gli aerei disponibili cout<<endl
	//cout << " Ho finito con il modello ---- Copio le Ruoute --- che sono: " << model->solution_model.size() << endl;
	int n = 0;
	for (Route& r : model->solution_model) n += (int)r.passengers_in_route.size();
	//cout << " Numero passeggieri della soluzione del modello: " << n << endl;
	if ((int)(model->solution_model.size()) > 0 && (n == (int)passengers.size())) {
		for (Route& s : model->solution_model) solution_model.push_back(s);
		//for (auto x : contPass) cout << x.first << " --- " << x.second << " In realta = " << (x.second / ContNomi[x.first]) << endl;
		return solution_model;
	}
	else {
		//cout << " Modello mi ha dato Infeassible stampo soluzione con costo ----> " << calculate_ObjectiveFunction(solutionAllSub[Best_iteration], map_airstrip, map_airplane, from_to,from_to_FuelConsumed) << endl;
		//for (auto x : contPass) cout << x.first << " --- " << x.second << " In realta = " << (x.second / ContNomi[x.first]) << endl;
		n = 0;
		for (Route& r : solutionAllSub[Best_iteration]) n += (int)r.passengers_in_route.size();
		//cout << " Numero di passeggieri della best solution " << n << endl;
		//cout << " Mentre dovrebbero essere: " << passengers.size() << endl;
		if (n == (int)passengers.size())  return solutionAllSub[Best_iteration];
		else {
			vector<Route> vuoto(0);
			//cout << " Ritorno Vettore vuoto " << endl;
			return vuoto;
		}
	}

	delete model;
}









//************************************************************************************** Local Search **************************************************************
void destroy_ls(int index, int node_destroy, vector<Passenger>& passenger_removed, Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
	//cout << " Stampo la route prima di toccarla: " << endl;
	//r.print();
	if (r.index > 1) {
		bool check = true;
		double fuel_consumed_check = 0.0;
		if ((node_destroy + 2) < r.index) {
			//double time_travel = from_to[r.places[node_destroy - 1] + ";" + r.places[node_destroy - 1]] / map_airplane[r.aircraft_code].speed; 
			double time_travel = from_to[r.places[node_destroy - 1]][r.places[node_destroy + 2]] / map_airplane[r.aircraft_code].speed;
			if (time_travel <= 1) fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
			else {
				fuel_consumed_check = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
			}
			//cout << " Deve valere che: " << fuel_consumed_check << " + " << map_airplane[r.aircraft_code].min_fuel << " <= " << r.quantity_fuel[node_destroy - 1] << endl;
		}
		if ((fuel_consumed_check + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
			check = false;
			int number_initial_node = r.index;
			for (int i = 0; i < 2; i++) {
				//cout << " Sono al giro i -> " << i << endl;
				if (i == 1 && (number_initial_node - r.index) > 1) break;
				//cout << " devo distruggere il nodo -> " << node_destroy << endl;
				double fuel_consumed = 0.0;
				double time_travel = from_to[r.places[node_destroy - 1]][r.places[node_destroy + 1]] / map_airplane[r.aircraft_code].speed;
				if (time_travel <= 1) fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first * time_travel;
				else {
					fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (time_travel - 1) * map_airplane[r.aircraft_code].fuel_burn_second;
				}
				vector<int> int_removed;
				int Min_From_Pass = node_destroy;
				int Max_To_Pass = node_destroy;
				for (int p = 0; p < r.passengers_in_route.size(); p++) {
					if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
						if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
						if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
						//cout << " Aggiungo alla lista dei passegggieri il passeggiero ";
						//r.passengers_in_route[p].print();
						int_removed.push_back(p);
						for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
							r.capacity[t] -= r.passengers_in_route[p].capacity;
							r.weight[t] += r.passengers_in_route[p].weight;
							/*
							if (r.refueling[t] && r.quantity_fuel[t] < map_airplane[r.aircraft_code].max_fuel) {
							r.weight[t] -= min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight) - r.quantity_fuel[t];
							r.quantity_fuel[t] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[t] + r.passengers_in_route[p].weight);
							}
							*/
						}

					}
				}

				for (int i = int_removed.size() - 1; i >= 0; i--) {
					// code for repair forbidden***********************************************************
					r.passengers_in_route[int_removed[i]].route_before = index;
					//*************************************************************************************
					passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
					vector<Passenger>::iterator it;
					it = r.passengers_in_route.begin();
					r.passengers_in_route.erase(it + int_removed[i]);
				}
				//cout << " Sono alla riga 5946 dentro al detroy ho numero di passeggieri rimossi pari a " << passenger_removed.size() << endl;
				r.update_route_destroy(node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
				//r.print();
				int index_before = node_destroy - 1;
				//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
				//r.print();
				//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
				double diff = 0;
				//cout << " Index before: " << index_before << endl;
				for (int i = node_destroy + 1; i < r.index; i++) {
					//cout << " sono al i = " << i << endl;
					if (r.refueling[i]) break;
					//cout << "weight before: " << r.weight[i] << endl;
					if (index_before == (node_destroy - 1)) {
						//cout << " sono al primo giro faccio " << endl;
						diff = r.quantity_fuel[i];
						//cout << " diff: " << diff << endl;
						r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
						diff = diff - r.quantity_fuel[i];
						//cout << " diff aggiornata: " << diff << endl;

					}
					else {
						//cout << " non sono al primo giro " << endl;
						r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
						//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
					}
					//cout << " weight prima " << r.weight[i] << endl;
					r.weight[i] = r.weight[i] + diff;
					index_before = i + 1;
					//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
				}
				//*****************************************************************************************************************************************************************

				//cout << " Sono dentro al codice del destroy della ricerca locale sto per fare remove place " << endl;
				//cout << " Devo togliere nodo " << r.places[node_destroy] << endl;
				//r.print();
				r.removePlace(node_destroy, map_airplane);
				//cout << " Ho finito rimozione " << endl;
				//r.print();
				//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weight[j] < 0) {
						//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						//cout << " Aggiorno da " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Ho passato if aggiorno nodo " << t << endl;
							//cout << " Quantit� fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Quantit� fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
						}
					}
				}

			}
			//r.print();
		}

		if (check) {
			r.index = -1;
			//cout << " Attenzione � il caso che non puo toglire il nodo quello dove prima si bloccava " << endl;
			//cout << " Nelle prossime stampe questo giro di A lo devo saltare devo comparire al prossimo A " << endl;
			//system("pause");
		}
	}
	//cout << "passengers removed" << endl;
	//for (Passenger p : passenger_removed) p.print();
}

int sequential_same_node(const Route& r) {
	int node = -1;
	for (int i = 0; i < ((int)r.places.size()) - 1; i++) {
		if (r.places[i] == r.places[i + 1]) {
			node = i;
			break;
		}
	}
	return node;
}

void aggregate_same_nodes(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node))	p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		//cout << " Nodo doppio che devo eliminare risiede nell'ultima posizione" << endl;
		r.time_dep[node + 1] = r.time_dep[node];
		r.time_arr[node + 1] = r.time_arr[node];
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
	else {
		//cout << " Nodo doppio che devo eliminare NON risiede nell'ultima posizione" << endl;
		// I remove the extra time that was considered
		double time_to_remove = r.time_dep[node + 1] - r.time_dep[node];
		for (int i = node + 1; i < r.index; i++) {
			r.time_dep[i] -= time_to_remove;
			r.time_arr[i] -= time_to_remove;
		}
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}



}

void aggregate_same_nodes_inter_ls(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node))	p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		//cout << " Nodo doppio che devo eliminare risiede nell'ultima posizione" << endl;
		r.time_dep[node + 1] = r.time_dep[node];
		r.time_arr[node + 1] = r.time_arr[node];
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}
	else {
		//cout << " Nodo doppio che devo eliminare NON risiede nell'ultima posizione" << endl;
		// I remove the extra time that was considered
		double time_to_remove = r.time_dep[node + 1] - r.time_dep[node];
		for (int i = node + 1; i < r.index; i++) {
			if (i == node + 1) {
				r.time_dep[i] = r.time_dep[node];
				r.time_arr[i] = r.time_arr[node];
			}
			else {
				r.time_dep[i] -= time_to_remove;
				r.time_arr[i] -= time_to_remove;
			}

		}
		r.index--;
		//r.index_cap--;
		r.places.erase(r.places.begin() + node);
		r.time_arr.erase(r.time_arr.begin() + node);
		r.time_dep.erase(r.time_dep.begin() + node);
		r.refueling.erase(r.refueling.begin() + node);
		r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
		r.weight.erase(r.weight.begin() + node);
		r.capacity.erase(r.capacity.begin() + node);
	}



}

int swap_from(int node, const Route& r) {

	int swap_from = 0;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}


	return swap_from;

}

int swap_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}

	}


	return swap_to;
}


bool swap_is_allowed(int A, int B, const Route& r) {
	bool swap = false;
	if (B < swap_to(A, r) && swap_from(B, r) < A) swap = true;
	return swap;
}

//qua purtroppo non posso mettere usare i puntatori
Route update_route_after_swap(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;

	//cout << "sto inserendo i vari posti nella nuova route" << endl;
	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	for (int i = 1; i < r.index; i++) {
		if (i == A) {
			//in questo posto ci devo mettere B
			r_new.addPlace(r.places[B], r.refueling[B], r.quantity_fuel[B], 0.0, 0, r.time_arr[B], r.time_dep[B]);
		}
		else if (i == B) {
			//in questo posto ci devo mettere A
			r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
		}
		else {
			//in questo posto ci devo mettere normalmente i
			r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
		}
	}
	//cout << "ho finito di inserire i vari posti nella nuova route" << endl;


	//cout << "aggiorno i tempi e il fuel" << endl;
	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua � come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			//cout << "from: " << r_new.places[i - 1] << endl;
			//cout << "to: " << r_new.places[i] << endl;
			//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			//cout << "fuel consumed: " << fuel_consumed << endl;

			if (r_new.refueling[i]) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
			}
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}
	//cout << "ho finito di aggiornare i tempi e il fuel" << endl;

	//cout << "aggiorno i i passeggeri" << endl;
	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri spostati
	for (Passenger p : r.passengers_in_route) {
		if (p.solution_from == A) p.solution_from = B;
		else if (p.solution_to == A) p.solution_to = B;
		else if (p.solution_from == B) p.solution_from = A;
		else if (p.solution_to == B) p.solution_to = A;
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
	}
	//cout << "finito" << endl;

	//cout << "aggiorno il fuel se peso negativo" << endl;
	//aggiorno fuel se il peso � negatico
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			//	cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					//	cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			//cout << " r_new.quantity_fuel[index_refueling] : " << r_new.quantity_fuel[index_refueling] << " +=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			//cout << " r_new.weight[index_refueling] : " << r_new.weight[index_refueling] << " -=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.weight[index_refueling] -= r_new.weight[i];
			//cout << " Adesso tolgo ai nodi senza possibilit� di fare refuel successvi " << endl;
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					//cout << " Dopo Fuel era = " << r_new.quantity_fuel[j] << endl;
					//cout << " Prima Weight era = " << r_new.weight[j] << endl;
					r_new.weight[j] -= Update_value;
					//cout << " Dopo Weight era = " << r_new.weight[j] << endl;
				}
			}

			//r_new.weight[i] = 0;

		}
	}



	return r_new;
}

vector <Route> swap(double peso_TW, double peso_intermediate_stop, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_swap;

	for (const Route& r : routes) {
		//cout << " sto provando a fare lo swap in questa route: " << endl;
		//r.print();
		Route r_support = r;
		for (int A = 1; A < r_support.index - 1; A++) {
			for (int B = A + 1; B < r_support.index; B++) {

				if (swap_is_allowed(A, B, r_support)) {
					//cout << "******************************************************Sto provando a fare lo swap fra " << A << " e " << B << endl;
					Route r_new = update_route_after_swap(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
					//cout << "ho finito l'update" << endl;
					//r_new.print();
					double cost_route_support = cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					double cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					//cout << cost_route_support << " c_supp > c_new " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to) << endl;
					if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
						//cout << " OK ! dopo move Migliora ------- Ma prima devo controllare che non sia con nodi doppi " << endl;
						int node = sequential_same_node(r_new);
						bool fatto = false;
						while (node != -1) {
							/*
							cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
							r_new.print();
							// i have two airports close to each other
							cout << " Nodo dove inizia coppia --> " << node << endl;
							system("pause");
							*/
							aggregate_same_nodes(r_new, node);
							//r_new.print();
							cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to) << endl;

							fatto = true;
							node = sequential_same_node(r_new);
						}
						//cout << " Ok non aveva nodi doppi ! " << endl;
						if (fatto == false) {
							r_support = r_new;
							A = 1;
							B = A + 1;
						}
						else {
							if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
								r_support = r_new;
								A = 1;
								B = A + 1;
								cost_route_support = cost_route_new;
							}
						}
					}

				}

			}
		}
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_swap.push_back(r_support);
	}

	return routes_after_swap;
}

int move_from(int node, const Route& r) {

	int swap_from = 0;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}


	return swap_from;

}

int move_to(int node, const Route& r) {
	int swap_to = 1000;

	for (const Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}

	}


	return swap_to;
}

bool move_is_allowed(int A, int B, Route& r) {
	bool move = false;
	if (B > A && B < move_to(A, r))move = true;
	if (B < A && move_from(A, r) < B) move = true;
	return move;
}

bool move_flightleg_is_allowed(int A, Route& r) {
	bool move = true;
	//cout << " Sono dentro al controllo per sapere se � consentito " << endl;
	//cout << " sto guardando la route" << r.aircraft_code << " con il nodo " << A << endl;
	for (Passenger& p : r.passengers_in_route) {
		//cout << " sto facendo il passeggiero: ";
		//p.print();
		if ((p.solution_from == A) && (p.solution_to != (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_from != A) && (p.solution_to == (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_to == A) || (p.solution_from == (A + 1))) {
			move = false;
			break;
		}
		//cout << " Move = " << move << endl;
	}
	return move;
}

Route update_route_after_move(int A, int B, const Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	r_new.primo_pass = r.primo_pass;
	/*cout << " Questa � la vecchia routes: " << endl;
	//r.print();
	*/
	//cout << "dentro a route after move -- A -> " << A << " B --> " << B << endl;
	//cout << "sto inserendo i vari posti nella nuova route" << endl;

	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	if (B > A) {
		//cout << " Sono nel caso in cui B > A " << endl;
		for (int i = 1; i < r.index; i++) {
			//cout << " Sono nel indice i --> " << i << endl;
			if ((i < A) || (i > B)) {
				//cout << " caso (i < A) || (i > B) in questo casorimane come prima devo mettere normalmente quello che c'� in i " << endl;
				//cout << r.places[i] << endl;
				// Caso in cui rimane come prima
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if ((i >= A) && (i < B)) {
				//cout << " caso (i >= A) && (i < B) in questo caso devo metterquello che c'� in i+1 " << endl;
				//cout << r.places[i+1] << endl;
				//cout << "stampo nel caso indicato dal bed allocator i+1" << endl;
				//cout << "i+1: " << i+1 << " r.places[i+1]: " << r.places[i+1] << endl;
				r_new.addPlace(r.places[i + 1], r.refueling[i + 1], r.quantity_fuel[i + 1], 0.0, 0, r.time_arr[i + 1], r.time_dep[i + 1]);
			}
			else if (i == B) {
				//cout << " caso i = (B) in questo caso devo mettere normalmente A " << endl;
				//cout << r.places[A] << endl;
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
		}
	}
	else {
		//cout << " Sono nel caso in cui B < A " << endl;
		for (int i = 1; i < r.index; i++) {
			//cout << " Sono nel indice i --> " << i << endl;
			if (i < (B + 1)) {
				//cout << " caso i < (B + 1) in questo caso devo mettere normalmente quello che c'� in i " << endl;
				//cout << r.places[i] << endl;
				//in questo posto ci devo mettere normalmente i
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if (i == (B + 1)) {
				//cout << " caso i = (B + 1) in questo caso devo mettere normalmente A " << endl;
				//cout << r.places[A] << endl;
				//in questo posto ci devo mettere A
				//cout << "stampo nel caso indicato dal bed allocator A" << endl;
				//cout << "ecco l'index: " << r.index << endl;
				//cout << "indice i: " << i << endl;
				//cout << "A: " << A << " r.places[A]: " << r.places[A] << endl;
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
			else if (i >= A + 1) {
				//in questo posto ci devo mettere normalmente i-1
				//cout << " caso (i >= A +1) in questo caso devo mettere normalmente quello che c'� in i" << endl;
				//cout << r.places[i-1] << endl;
				//cout << "stampo nel caso indicato dal bed allocator" << endl;
				//cout << "i: " << i << " r.places[i]: " << r.places[i] << endl;
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else {
				//cout << " caso i > (B + 1) in questo caso devo mettere normalmente quello che c'� in i - 1" << endl;
				//cout << r.places[i - 1] << endl;
				r_new.addPlace(r.places[i - 1], r.refueling[i - 1], r.quantity_fuel[i - 1], 0.0, 0, r.time_arr[i - 1], r.time_dep[i - 1]);
			}
		}
	}
	//cout << "ho finito di inserire i vari posti nella nuova route" << endl;
	//r_new.print();
	//cout << "aggiorno i tempi e il fuel" << endl;
	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua � come se lo inizializzassi
	for (int i = 0; i < r_new.index; i++) {
		if (i > 0) {
			//cout << "from: " << r_new.places[i - 1] << endl;
			//cout << "to: " << r_new.places[i] << endl;
			//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
			r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

			//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];


			//cout << "fuel consumed: " << fuel_consumed << endl;

			if (r_new.refueling[i]) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
			}
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}
	//cout << "ho finito di aggiornare i tempi e il fuel" << endl;
	//r_new.print();
	//cout << "aggiorno i i passeggeri" << endl;
	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri 
	//cout << " A -> " << A << " B --> " << B << endl;
	for (Passenger p : r.passengers_in_route) {
		//cout << " Ho preso il passeggiero: " << endl;
		//cout << " Nome : " << p.name << " che ha from: " << p.solution_to << " e to: " << p.solution_to << endl;
		if (p.solution_from == A && A > B) {
			//cout << " Modifico from = B+1 pperche A > B " << endl;
			p.solution_from = B + 1;
		}
		else if (p.solution_to == A && A > B) {
			//cout << " Modifico to = B+1 pperche A > B " << endl;
			p.solution_to = B + 1;
		}
		else if (p.solution_to == A && A < B) {
			//cout << " Modifico to = B perch� A < B " << endl;
			p.solution_to = B;
		}
		else if (p.solution_from == A && A < B) {
			//cout << " Modifico from = B perch� A < B  " << endl;
			p.solution_from = B;
		}
		else if (p.solution_from <= B && B < p.solution_to && A > p.solution_to) {
			//cout << " Incremento il to di 1  caso A > B" << endl;
			p.solution_to++;
		}
		else if (p.solution_from <= B && B < p.solution_to && A < p.solution_from) {
			//cout << " Decremento il from di 1  caso A < B" << endl;
			p.solution_from--;
		}
		else if (p.solution_from > B && A > p.solution_to) {
			//cout << " Terza casistica A era dopo richiesta adesso va orima" << endl;
			p.solution_from++;
			p.solution_to++;

		}
		else if (p.solution_from < A && A < p.solution_to && B < p.solution_from) {
			//cout << " Quarta casistica A" << endl;
			p.solution_from++;
		}
		else if (p.solution_from < A && A < p.solution_to && B >= p.solution_to && A < B) {
			//cout << " Quarta casistica B" << endl;
			p.solution_to--;
		}
		else if (p.solution_from > A && A < p.solution_to && B >= p.solution_to) {
			//cout << " Quinta casistica Nodo va da prima della richiesta a dopo" << endl;
			p.solution_to--;
			p.solution_from--;
		}
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] += p.capacity;
			r_new.weight[t] -= p.weight;
		}
		//cout << " Nome : " << p.name << " che ha from AGGIORNATO: " << p.solution_from << " e to AGGIORNATO: " << p.solution_to << endl;
	}
	/*cout << "finito" << endl;
	r_new.print();
	cout << "aggiorno il fuel se peso negativo" << endl;
	//aggiorno fuel se il peso � negatico */
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			//	cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					//	cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			//cout << " r_new.quantity_fuel[index_refueling] : " << r_new.quantity_fuel[index_refueling] << " +=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			//cout << " r_new.weight[index_refueling] : " << r_new.weight[index_refueling] << " -=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.weight[index_refueling] -= r_new.weight[i];
			//cout << " Adesso tolgo ai nodi senza possibilit� di fare refuel successvi " << endl;
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					//cout << " Dopo Fuel era = " << r_new.quantity_fuel[j] << endl;
					//cout << " Prima Weight era = " << r_new.weight[j] << endl;
					r_new.weight[j] -= Update_value;
					//cout << " Dopo Weight era = " << r_new.weight[j] << endl;
				}
			}

			//r_new.weight[i] = 0;

		}
	}
	//cout << "finito" << endl;

	//stampo_caso_strano_single(r, from_to, map_airplane);

	return r_new;
}

vector <Route> move(double peso_TW, double peso_intermediate_stop, vector<Route>& routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;

	for (const Route& r : routes) {
		//cout << " sto provando a fare il move in questa route: " <<r.aircraft_code <<  endl;
		//r.print();
		Route r_support = r;
		for (int A = 1; A < r_support.index; A++) {
			//cout << " Considero di spostare nodo -> " << A << endl;
			// we have to move the Node A form its position to th enext after B
			for (int B = 1; B < r_support.index; B++) {
				if (A != B && B != (A - 1) && B != (A - 2)) {    //Il caso B != (A-1) || B!= (A-2) sono casi che valutiamo gia quando sposriamo avanti
				//cout << " Nella posizione -> " << B << endl;
					if (move_is_allowed(A, B, r_support)) {
						//cout << " CONSENTITO sono nel main -----> provo a fare move da " << A << " e " << B << endl;
						Route r_new = update_route_after_move(A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
						//cout << "ho finito l'update" << endl;
						//r_new.print();
						double cost_route_support = cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						double cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						//cout << " Deve valere che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to,location_fuel) << endl;
						if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
							//cout << " OK ! dopo move Migliora ------- Ma prima devo controllare che non sia con nodi doppi " << endl;
							int node = sequential_same_node(r_new);
							//cout << "node fuori dal while: " << node << endl;
							//r_new.print();
							bool fatto = false;
							int num_aggregazioni = 0;
							while (node != -1) {
								/* cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
								//r_new.print();
								// i have two airports close to each other
								cout << " Nodo dove inizia coppia --> " << node << endl;
								system("pause");
								*/
								//cout << "******* Aggrego nodo in posizione **********" << node << endl;
								aggregate_same_nodes(r_new, node);
								if (A > node) num_aggregazioni++;
								//cout << " Ho finito codice di aggregazione " << endl;
								cost_route_new = cost_single_route(peso_TW, peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to, location_fuel) << endl;
								//system("pause");
								node = sequential_same_node(r_new);
								//cout << "node dentro al while: " << node << endl;
								//r_new.print();
								fatto = true;
							}

							//cout << " Ok non aveva nodi doppi ! " << endl;
							if (fatto == false) r_support = r_new;
							else {
								if (cost_route_support > cost_route_new && route_feasible(r_new, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
									r_support = r_new;
									cost_route_support = cost_route_new;
									//cout << " Ho finito while di aggregazione ma devo aggiornare A tolgo " << num_aggregazioni << endl;
									A -= num_aggregazioni;
									if (A <= 0) A = 1;
								}
							}
						}
					}
				}

			}
		}
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_move.push_back(r_support);
	}

	return routes_after_move;
}



vector <Route> repair_one_inter_move(double peso_TW, double peso_intermediate_stop, double end_day, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool case_first_passenger = false; //if false there are no passengers and only one node in the rotue, otherwise more nodes and no passengers
	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {
		//cout << endl;
		//p.print();
		//int best_arc_from = -1;
		//int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		move_c = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < (int)routes_destroyed.size(); r++) {
			//cout << "Route: " << r << endl;
			//routes_destroyed[r].print();
			if (routes_destroyed[r].primo_pass == false) {
				if (routes_destroyed[r].index == 1) {
					//c'� solo il deposito in questo caso
					double cost = from_to[routes_destroyed[r].places[0]][p.departure_location] + from_to[p.departure_location][p.arrival_location] + map_airplane[routes_destroyed[r].aircraft_code].fixed_cost;
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[0]][p.departure_location];
					cost += from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					//cout << "------------------------------------------sono la route vuota, ho costo: " << cost << endl;
					//cout << "costo caso primo passeggero: " << cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						case_first_passenger = false;
					}
				}
			}
			else {
				//int arc_from = -1;
				//int arc_to = -1;
				double cost_route_before = cost_single_route(peso_TW, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				for (int n = 0; n < routes_destroyed[r].index - 1; n++) {
					if (p.departure_location != routes_destroyed[r].places[n] || n == 0) {
						for (int n1 = n; (n1 < routes_destroyed[r].index) && (n1 - n <= p.stop + 1); n1++) {
							if (p.arrival_location != routes_destroyed[r].places[n1]) {
								//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
								Route r_support = routes_destroyed[r];

								bool non_to = false;
								bool non_to_final = false;
								bool num_equals = false;
								int caso = -1;
								int node_add_from = n;
								int node_add_to = n1;

								r_support.update_route_rebuilt_one_first_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
								//stampo_caso_strano_single_tempi(r_support, from_to, map_airplane);
								//cout << "route per il check" << endl;
								//r_for_check.print();
								/*if (r_support.time_arr[r_support.index - 1] >= 2000) {
									cout << "caso strano: " << endl;
									r_support.print();
									cin.get();
								}*/


								if (r_support.time_arr[r_support.index - 1] <= end_day) {
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//r_support.print();
									r_support.update_route_rebuilt_one_second_fase(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);
									//cout << "ristampo i valori" << endl;
									//p.print();
									//cout << "n: " << n << " n1: " << n1 << " node_add_from: " << node_add_from << " node add_to: " << node_add_to << " caso: " << caso << endl;
									//cout << "non_to: " << non_to << " non_to_final: " << non_to_final << " num_equals: " << num_equals << endl;
									//cout << endl;
									//stampo_caso_strano_single_all(r_support, from_to, map_airplane);
									if ((p.solution_to - p.solution_from <= p.stop)) {
										if (route_feasible(r_support, map_airplane, end_day, location_fuel, from_to_FuelConsumed)) {
											double cost = (cost_single_route(peso_TW, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, peso_TW)) + (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) - cost_route_before;
											//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
											if (best_cost > cost) {
												//best_arc_from = n;
												//best_arc_to = n1;
												best_route = r;
												best_cost = cost;
												move_c = false;

												route_best = r_support;
												from_per_route = p.solution_from;
												to_per_route = p.solution_to;
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//for move C
			if (routes_destroyed[r].primo_pass) {
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C
				if (routes_destroyed[r].places[routes_destroyed[r].index - 1] == p.departure_location) {
					double dist = from_to[p.departure_location][p.arrival_location];
					double time = (from_to[p.departure_location][p.arrival_location] / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60;
					//cout << "tempo di volo: " << time << endl;
					double fuel_consumed = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];
					/*if (time <= 60) {
						fuel_consumed += (time / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + ((time - 60) / 60)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}*/

					double time_window_cost = 0.0;
					double t_arr_departure = routes_destroyed[r].time_arr[routes_destroyed[r].index - 1];
					double t_arr_arrival = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time;
					if (t_arr_departure < p.early_departure) time_window_cost += p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) time_window_cost += t_arr_departure - p.late_departure;

					if (t_arr_arrival < p.early_arrival) time_window_cost += p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) time_window_cost += t_arr_arrival - p.late_arrival;

					double cost = dist + ((time_window_cost * peso_TW) * p.capacity) + fuel_consumed;
					//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
					//cout << "cost for move c" << cost << endl;
					//cout << t_arr_arrival << " : tempo arrivo" << endl;
					if (best_cost > cost) {
						if (routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + time <= end_day) {
							if (routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = r;
								move_c = true;
								//best_arc_from = -2;
								//best_arc_to = -2;
							}
						}
					}
				}
				else {
					// the departure is not equals to the last place of the route
					double cost = from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];
					double TW_departure = 0.0;
					double t_arr_departure = routes_destroyed[r].time_dep[routes_destroyed[r].index - 1] + (((from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
					else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

					double TW_arrival = 0.0;
					double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed) * 60);
					if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
					else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
					cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;

					//per il check sul fuel:
					//double travel_primo_tratto = (from_to[routes_destroyed[r].places[routes_destroyed[r].index - 1] + ";" + p.departure_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_primo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][routes_destroyed[r].places[routes_destroyed[r].index - 1]][p.departure_location];
					/*
					if (travel_primo_tratto <= 1) {
						fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_primo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}
					*/
					//****
					//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[routes_destroyed[r].aircraft_code].speed;
					double fuel_consumed_secondo_tratto = from_to_FuelConsumed[routes_destroyed[r].aircraft_code][p.departure_location][p.arrival_location];

					cost += fuel_consumed_primo_tratto + fuel_consumed_secondo_tratto;

					/*
					if (travel_secondo_tratto <= 1) {
						fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[routes_destroyed[r].aircraft_code].fuel_burn_second;
					}
					*/

					//ora fisso le vairabili che mi servono, questo lo faccio perch� nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
					//refuel
					double f_after_primo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto;
					double f_after_secondo_tratto = 0.0;
					if (map_airstrip[p.departure_location].fuel) {
						f_after_secondo_tratto = map_airplane[routes_destroyed[r].aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
					}
					else {
						f_after_secondo_tratto = routes_destroyed[r].quantity_fuel[routes_destroyed[r].index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
					}

					//cout << "------------------------------------------sono la mossa c, ho costo: " << cost << endl;
					//cout << "cost for move c" << cost << endl;
					//cout << t_arr_arrival << " : tempo arrivo" << endl;
					if (best_cost > cost) {
						if (t_arr_arrival <= end_day) {
							if (f_after_primo_tratto >= map_airplane[routes_destroyed[r].aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[routes_destroyed[r].aircraft_code].min_fuel + location_fuel[routes_destroyed[r].aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = r;
									move_c = true;
									//best_arc_from = -2;
									//best_arc_to = -2;
								}
							}
						}
					}
				}
			}

		}

		//cout << "ho scelto la mossa mossaC (1 si, 0 no) " << move_c << "route: " << best_route << " archi: " << best_arc_from << " : " << best_arc_to << " con costo: " << best_cost << " sono una route vuota 0 si, 1 no " << routes_destroyed[best_route].primo_pass << endl;

		if (best_route == -1) {
			//cout << "********************** ha queste possibilit�******************************************" << endl;
			//for (Route c : routes_destroyed) c.print();
			routes_infeasible = true;
			break;
		}
		else {
			//cout << "la mossa C: " << move_c << endl;
			//cout << "size of the vector of places" << routes_destroyed[best_route].places.size() << endl;
			//cout << "index: "<< routes_destroyed[best_route].index << " index cap: " << routes_destroyed[best_route].index_cap << endl;
			//cout << best_route << " " << best_arc_from << "---" << best_arc_to << " la route ha passeggeri: " << to_string(routes_destroyed[best_route].primo_pass) << endl;
			//p.print();
			//routes_destroyed[best_route].print();


			if (!routes_destroyed[best_route].primo_pass) {
				//i have to do the new things, 
				if (case_first_passenger == false) {
					//c'� solo il depot
					routes_destroyed[best_route].primo_pass = true;
					routes_destroyed[best_route].add_update_only_one_node_first_passanger(p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "caso primo passeggero" << endl;
					//routes_destroyed[best_route].print();
				}
				//else {
					//ci sono pi� nodi
				//	routes_destroyed[best_route].primo_pass = true;
				//	routes_destroyed[best_route].add_update_only_few_node_first_passanger(p, best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				//}
			}
			else {

				if (move_c) {
					//i have to do the move C
					//cout << "sto facendo la mossa C" << endl;
					routes_destroyed[best_route].move_c(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					//cout << "caso mossa C" << endl;
					//routes_destroyed[best_route].print();

				}
				else {
					//cout << "prima dell'inserimento speciale" << endl;
					//cout << "route prima dell'inserimento" << endl;
					//routes_destroyed[best_route].print();
					//cout << "caso inserimento speciale ------>" << best_arc_from << " - " << best_arc_to << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;
					//routes_destroyed[best_route].print();
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector <Route> inter_move(double peso_TW, double peso_intermediate_stop, vector<Route> routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;
	int n_route = -1;
	//cout << " Stampo le route prima di cominciare " << endl;
	//for (Route r : routes) r.print();
	//cout << "************************************************** INCOMINCIO ********************************************************************************************* " << endl;
	//for (Route r : routes) {
	/*int t = 0;
	for (auto x : routes) t += x.passengers_in_route.size();
	cout << "ecco i passeggeri che arrivano in input dalle routes: " << t << endl;
	*/

	for (int r = 0; r < routes.size(); r++) {
		n_route += 1;
		vector<Route> routes_destroyed;



		//cout << " Adesso anallizzo la route numero " << r << " dell'aereo " << routes[r].aircraft_code << endl;
		//Route distrutte sono uguali tranne quella a cui sto togliendo la coppia
		//for (Route s : routes) if (s.aircraft_code != r.aircraft_code) routes_destroyed.push_back(s);
		for (const Route& s : routes) {
			//cout << "if: " << to_string(s.aircraft_code) << "  ----  " << to_string(routes[r].aircraft_code) << endl;
			if (s.aircraft_code != routes[r].aircraft_code) routes_destroyed.push_back(s);
		}
		Route r_support = routes[r];
		//cout << "r_support ha questi passeggeri: " << r_support.passengers_in_route.size() << endl;


		for (int A = 1; A < (r_support.index - 1); A++) {
			//cout << " Sto guardando Nodo A: " << A << " di " << r_support.aircraft_code << " size: " << r_support.index <<  endl;
			// Solution Rebuilt � come se fosse il nostro best improvement
			vector<Route> solution_rebuilt;
			vector<Passenger> passenger_removed;
			//cout << " Considero di spostare nodo -> " << A << " e nodo " << A+1 << endl;
			// Ora devo eliminare i nodi solo se sono allowed
			if (move_flightleg_is_allowed(A, r_support)) {
				//cout << " CONSENTITO sono nel main -----> provo a fare move di " << A << " e " << A+1 << " da " << r_support.aircraft_code << " un altra route " << endl;
				Route r_new = r_support;
				//cout << " La route adesso ha numero di nodi: " << r_new.index << endl;
				int NomeA1 = r_new.places[A + 1];


				destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);

				//cout << " La route adesso ha numero di nodi: " << r_new.index << " con numero di passeggieri rimossi " << passenger_removed.size() << endl;
				//cout << " Stampo la  route distrutta " << endl;
				//r_new.print();
				if (r_new.index != -1) {
					/*if (A != (r_new.index - 1) && r_new.places[A] == NomeA1 && r_new.index > 1) {
						//cout << " Ok vado a togliere anche il nodo A " << endl;
						destroy_ls(n_route, A, passenger_removed, r_new, map_airplane, map_airstrip, from_to);
						//for (Passenger p : passenger_removed) p.print();
						//r_new.print();
					}*/
					if (r_new.index != -1) {


						solution_rebuilt = repair_one_inter_move(peso_TW, peso_intermediate_stop, end_day, routes_destroyed, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);



						if (solution_rebuilt.size() != 0) {
							//int Npass = 0;
							//for (Route p : solution_rebuilt) Npass += p.passengers_in_route.size();
							//cout << " Numero Passeggeri dopo  repair dentro alla ricerca locale riga 6799 : " << Npass << " Sono i passeggieri senza route distrutta " << endl;
							//cout << " Numero Passegger nella route distrutta: " << r_new.passengers_in_route.size() << endl;
							solution_rebuilt.push_back(r_new);





							double before = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed); // Qui non va bene devi considerare che dopo un primo miglioramneto cambi la route
							double after = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (before > after) {
								//cout << " OK ! dopo mossa MIGLIORAAAAAAAAAAAAAAA di " << before <<" - "<<after << "------ Ma prima devo controllare che non sia con nodi doppi " << endl;
								// Qui sto usando tutto solution_rebuilt.back() ma in realta potrei usare r_new e poi un volta che la ho istemanta switcharla con solution_rebuilt.back()
								int node = sequential_same_node(solution_rebuilt.back());
								//cout << "node fuori dal while: " << node << endl;
								//r_new.print();
								bool fatto = false;
								int num_aggregazioni = 0;
								while (node != -1) {
									//cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << solution_rebuilt.back().aircraft_code<< endl;
									//solution_rebuilt.back().print();
									// i have two airports close to each other
									//cout << " Nodo dove inizia coppia --> " << node << endl;
									//if (node == 1) solution_rebuilt.back().print();
									//system("pause");
									//cout << "******* Aggrego nodo in posizione **********" << node << endl;
									//cout << " Numero nodi prima:  " << solution_rebuilt.back().index << endl;
									aggregate_same_nodes_inter_ls(solution_rebuilt.back(), node);
									//cout << " Numero nodi dopo:  " << solution_rebuilt.back().index << endl;
									if (A > node) num_aggregazioni++;
									//cout << " Ho finito codice di aggregazione " << endl;
									after = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible(r_new, map_airplane, end_day, from_to, location_fuel) << endl;
									//system("pause");
									node = sequential_same_node(solution_rebuilt.back());
									//solution_rebuilt.back().print();
									fatto = true;
								}

								if (fatto == false) {
									//cout << " Ok non aveva nodi doppi ! " << endl;
									//cout << " Il for degli A prima andava fino a " << r_support.index  << endl;
									r_support = r_new;
									//cout << " Il for degli A adesso va fino a " << r_support.index  << endl;
									routes_after_move = solution_rebuilt;
									routes = solution_rebuilt;
									// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
									routes_destroyed.clear();
									//routes_destroyed.shrink_to_fit();
									for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
									A = 1;
									r--;
									if (A >= (r_support.index - 1)) {
										//cout << " caso Loop " << endl;
									}
								}
								else {
									if (before > after) {
										// bisognerebbe mettere l'aggiornamento una cosa del tipo
										//cout << " Ok MIGLIORAAAAAAAAAAAAAAA aveva nodi doppi ! " << endl;
										//system("pause");
										//cout << " Il for degli A prima andava fino a " << r_support.index  << endl;
										r_support = r_new;
										//cout << " Il for degli A adesso va fino a " << r_support.index  << endl;
										routes_after_move = solution_rebuilt;
										routes = solution_rebuilt;
										//cout << " Ho finito while di aggregazione ma devo aggiornare A tolgo " << num_aggregazioni << endl;
										// Sicuramente sopra devo tenere conto che ho due vertici in meno nell'arco che sto considerando
										routes_destroyed.clear();
										//routes_destroyed.shrink_to_fit();
										for (int s = 0; s < solution_rebuilt.size() - 1; s++) routes_destroyed.push_back(solution_rebuilt[s]);
										A = 1;
										r--;
									}
								}
							}
						}
					}
				}
			}



		}



	}




	for (Route r_support : routes_after_move) {
		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perch� se no se oltre quel nodo non c'� ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
	}
	if (routes_after_move.size() == 0) return routes;
	else {
		if (calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) != calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
			cout << " Costo Routes: " << calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
			cout << " Costo routes_after_move: " << calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes_after_move, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
			//cin.get();
		}
		return routes_after_move;
	}
}

//************************************************************************************************************************************************************************






double costo_senza_time_windows(vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		//cout << "Route-----" << endl;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		cost += map_airplane[r.aircraft_code].fixed_cost;
		cost_route += map_airplane[r.aircraft_code].fixed_cost;
		//cout << "costo fisso: " << cost << endl;
		for (int i = 1; i < r.index; i++) {
			cost += map_airstrip[r.places[i]].landing_cost;
			cost_route += map_airstrip[r.places[i]].landing_cost;
		}
		//cout << "costo landing: " << cost << endl;



		//second of all calculate the cost of the Km and the fuel burn
		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;
		for (int i = 0; i < r.index - 1; i++) {
			//double time_travel = 0.0;
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}


		}
		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		//cout << "costo milege: " << cost << endl;
		cost += fuel_consumed;
		cost_route += fuel_consumed;
		//cout << "costo fuel: " << cost << endl;
		//now i have to calculate the penalitis regarding the time windows for each passeger
	}

	return cost;


};

double costo_time_windows_and_intermediate_stop(double peso_TW, double peso_intermediate_stop, vector<Route>& solution) {
	double cost = 0.0;

	for (auto& r : solution) {

		for (const auto& p : r.passengers_in_route) {
			double time_departure = r.time_arr[p.solution_from];
			double time_arrival = r.time_arr[p.solution_to];

			double TW_departure = 0.0;
			if (time_departure < p.early_departure) TW_departure = p.early_departure - time_departure;
			else if (time_departure > p.late_departure) TW_departure = time_departure - p.late_departure;

			double TW_arrival = 0.0;
			if (time_arrival < p.early_arrival) TW_arrival = p.early_arrival - time_arrival;
			else if (time_arrival > p.late_arrival) TW_arrival = time_arrival - p.late_arrival;

			cost += ((TW_departure + TW_arrival) * peso_TW) * p.capacity;
			cost += (peso_intermediate_stop * (p.solution_to - p.solution_from - 1)) * p.capacity;
		}
	}

	return cost;
}


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



//funzione che TUTTI GLI ERRORI
void stampo_caso_strano(map<int, Airstrip> map_airstrip, int peso_TW, int peso_intermediate_stop, vector<vector<double>> from_to_costi, vector<vector<vector<double>>> from_to_fuel_costi, vector<Passenger> passengers, vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa) {


	/*double cost_solution = calculate_ObjectiveFunction(peso_TW, peso_intermediate_stop, routes, map_airstrip, map_airplane, from_to_costi, from_to_fuel_costi);
	double cost_single = 0.0;
	for (auto x : routes) {
		if(x.index > 1) cost_single += cost_single_route(peso_TW, peso_intermediate_stop, x, map_airstrip, map_airplane, from_to_costi, from_to_fuel_costi);
	}
	if (cost_single <= (cost_solution - 1)  || cost_single >= (cost_solution + 1) ) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i costi" << endl;
		cout << "costi singoli: " << cost_single << " -- costo totale: " << cost_solution << endl;
		system("pause");
		cin.get();
	}*/




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
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono pi� codici aereo uguali" << endl;
			for (Route r : routes) cout << "codice: " << r.aircraft_code << endl;
			system("pause");
			cin.get();
		}
		else {
			test_codici.insert(make_pair(x.aircraft_code, 1));
		}
	}


	for (Route r : routes) {

		if (r.primo_pass) {
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
					cout << "dovrebbe essere: " << (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece � " << r.time_arr[i] << endl;
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
						cout << "dovrebbe essere: " << fuel_consumed << " mentre invece � " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
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




}


void stampo_caso_strano_destroy(vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa) {




	map<int, int> test_codici;
	for (auto x : routes) {
		if (test_codici.count(x.aircraft_code) >= 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA, nella soluzione ci sono pi� codici aereo uguali" << endl;
			for (Route r : routes) cout << "codice: " << r.aircraft_code << endl;
			system("pause");
			cin.get();
		}
		else {
			test_codici.insert(make_pair(x.aircraft_code, 1));
		}
	}


	for (Route r : routes) {

		if (r.primo_pass) {
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
					cout << "dovrebbe essere: " << (((from_to[legenda_inversa[r.places[i - 1]] + ";" + legenda_inversa[r.places[i]]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece � " << r.time_arr[i] << endl;
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
						cout << "dovrebbe essere: " << fuel_consumed << " mentre invece � " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
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




}



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





	cout << "*****************************SOLUTION MODEL -POSTERIORI- ********************************" << endl;
	for (Route& r : solution_after) {
		r.print();
	}

	return solution_after;



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


void fix_key_passenger_for_regret(vector<Passenger>& passengers_day1, vector<Passenger>& passengers_day2, vector<Passenger>& passengers_day3) {

	int chiave = 1;
	map<string, int> string_key;

	//per i passeggeri del giorno uno
	for (Passenger& p : passengers_day1) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}

	//per i passeggeri del giorno due
	for (Passenger& p : passengers_day2) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}


	//per i passeggeri del giorno tre
	for (Passenger& p : passengers_day3) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}



}


void Build_structure_Model(vector<Airplane> airplane, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
	int tipo = 0;
	for (int j = 0; j < (int)airplane.size(); j++) {
		//cout << " Creo per aereo " << airplane[j].code << endl;
		map<string, int>::iterator it = mappa_aereo_tipo.find(airplane[j].model + to_string(airplane[j].depot));
		if (it == mappa_aereo_tipo.end()) {
			mappa_aereo_tipo.insert(make_pair(airplane[j].model + to_string(airplane[j].depot), tipo));
			tipo++;
		}
	}
	for (int j = 0; j < (int)airplane.size(); j++) {
		map<string, int>::iterator it = mappa_aereo_tipo.find(airplane[j].model + to_string(airplane[j].depot));
		codice_aereo_tipo.insert(make_pair(airplane[j].code, it->second));
	}
	for (auto j : codice_aereo_tipo) {
		//cout << j.second << endl;
		map<int, int>::iterator it = tipo_numero.find(j.second);
		if (it == tipo_numero.end()) tipo_numero.insert(make_pair(j.second, 1));
		else tipo_numero[j.second] += 1;
	}

	for (map<string, int>::iterator it = mappa_aereo_tipo.begin(); it != mappa_aereo_tipo.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}
	for (map<int, int>::iterator it = codice_aereo_tipo.begin(); it != codice_aereo_tipo.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}
	for (map<int, int>::iterator it = tipo_numero.begin(); it != tipo_numero.end(); ++it) {
		cout << it->first << " => " << it->second << '\n';
	}


}

//bambini splittati
void unione_children_INTELLIGENTE(vector<Passenger>& passengers, vector<Passenger>& passengers_for_company_solution) {
	map<string, vector<Passenger>> groups;
	for (Passenger p : passengers) {
		if (groups.count(p.pnr_group) >= 1) {
			//qua vuol dire che c`é gia quel pnr
			groups[p.pnr_group].push_back(p);
		}
		else {
			//in questo caso il pnr va messo
			vector<Passenger> prr;
			prr.push_back(p);
			groups.insert(make_pair(p.pnr_group, prr));

		}
	}
	int numero_in = 0;
	vector<Passenger> passengers_aggr;

	for (auto x : groups) {
		vector<Passenger> genitori_maschi;
		vector<Passenger> genitori_donna;
		vector<Passenger> genitori;
		vector<Passenger> bambini;

		int numero_bimbi = 0;
		cout << "con il codice: " << x.first << endl;
		for (Passenger p : x.second) {
			p.print();
			numero_in++;
			if (p.gender == ("C")) {
				numero_bimbi++;
				bambini.push_back(p);
			}
			else {
				if (p.gender == "F") genitori_donna.push_back(p);
				if (p.gender == "M") genitori_maschi.push_back(p);
			}
		}
		cout << "bimbi in questo gruppo: " << numero_bimbi << endl;
		/*cout << "ora li guardiamo separati"<< endl;
		cout << "i genitori sono: " << endl;
		for(Passenger p: genitori) {
			p.print();
		}
		cout << "i bambini sono: " << endl;
		for(Passenger p: bambini) {
			p.print();
		}*/

		if ((int)bambini.size() > 0) {
			//incomincio a metterli con la mamma
			int indice_parent = 0;
			string precedente = "";
			for (Passenger p : bambini) {
				int loc_dep = p.departure_location;
				int loc_arr = p.arrival_location;
				int time_dep = p.departure_time;
				int time_arr = p.arrival_time;
				string attuale = to_string(p.departure_location) + to_string(p.arrival_location) + to_string(p.departure_time) + to_string(p.arrival_time);
				if (precedente != attuale) indice_parent = 0;
				precedente = attuale;


				vector<Passenger> genitori_gruppo;

				bool non_ha_genitore = true;
				if ((int)genitori_donna.size() > 0) {
					// metti i bambini con la mamma
					for (Passenger& g : genitori_donna) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time && time_arr == g.arrival_time) {
							genitori_gruppo.push_back(g);
							non_ha_genitore = false;
							//g.capacity += p.capacity;
							//g.weight += p.weight;
							//break;
						}
					}
				}
				if ((int)genitori_maschi.size() > 0) {
					//mettili con il papa
					for (Passenger& g : genitori_maschi) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time && time_arr == g.arrival_time) {
							genitori_gruppo.push_back(g);
							non_ha_genitore = false;
							/*g.capacity += p.capacity;
							g.weight += p.weight;
							break;*/
						}
					}


				}
				if (non_ha_genitore) {
					cout << "errore, ci sono dei bambini senza genitore" << endl;
				}

				//ora devo splittare
				bool trovato = false;
				for (Passenger& g : genitori_donna) {
					if (g.pnr == genitori_gruppo[indice_parent].pnr) {
						g.weight += p.weight;
						g.capacity++;
						trovato = true;
					}
				}
				if (!trovato) {
					for (Passenger& g : genitori_maschi) {
						if (g.pnr == genitori_gruppo[indice_parent].pnr) {
							g.weight += p.weight;
							g.capacity++;
							trovato = true;
						}
					}
				}
				if (!trovato) {
					cout << "c'e' un errore, controlla" << endl;
				}

				indice_parent++;
				if (indice_parent == genitori_gruppo.size()) indice_parent = 0;


			}



		}

		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << "ora i genitori sono cosi: " << endl;
		for (Passenger& g : genitori_donna) {
			g.print();
			passengers_aggr.push_back(g);
		}
		for (Passenger& g : genitori_maschi) {
			g.print();
			passengers_aggr.push_back(g);
		}
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		//system("pause");

		//cin.get();
	}
	cout << "ora faccio il check per vedere se le capacita' dei due gruppi tornano" << endl;
	int capacita_aggr = 0;
	for (Passenger p : passengers_aggr) capacita_aggr += p.capacity;
	cout << "le due capacita' sono: " << endl;
	cout << " - " << passengers.size();
	cout << " - " << capacita_aggr << endl;
	//system("pause");
	cout << "ho finito di vedere i gruppi" << endl;
	cout << "i passeggeri nei gruppi sono: " << numero_in << endl;
	if (numero_in != (int)passengers.size()) cout << "ERROREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
	//cin.get();


	passengers_for_company_solution = passengers;
	passengers = passengers_aggr;
}

//tutti i bambini con un genitore
void unione_children_VECCHIA(vector<Passenger>& passengers, vector<Passenger>& passengers_for_company_solution) {

	map<string, vector<Passenger>> groups;
	for (Passenger p : passengers) {
		if (groups.count(p.pnr_group) >= 1) {
			//qua vuol dire che c`é gia quel pnr
			groups[p.pnr_group].push_back(p);
		}
		else {
			//in questo caso il pnr va messo
			vector<Passenger> prr;
			prr.push_back(p);
			groups.insert(make_pair(p.pnr_group, prr));

		}
	}
	int numero_in = 0;
	vector<Passenger> passengers_aggr;

	for (auto x : groups) {
		vector<Passenger> genitori_maschi;
		vector<Passenger> genitori_donna;
		vector<Passenger> bambini;

		int numero_bimbi = 0;
		cout << "con il codice: " << x.first << endl;
		for (Passenger p : x.second) {
			p.print();
			numero_in++;
			if (p.gender == ("C")) {
				numero_bimbi++;
				bambini.push_back(p);
			}
			else {
				if (p.gender == "F") genitori_donna.push_back(p);
				if (p.gender == "M") genitori_maschi.push_back(p);
			}
		}
		cout << "bimbi in questo gruppo: " << numero_bimbi << endl;
		/*cout << "ora li guardiamo separati"<< endl;
		cout << "i genitori sono: " << endl;
		for(Passenger p: genitori) {
			p.print();
		}
		cout << "i bambini sono: " << endl;
		for(Passenger p: bambini) {
			p.print();
		}*/

		if ((int)bambini.size() > 0) {
			//incomincio a metterli con la mamma
			for (Passenger p : bambini) {
				int loc_dep = p.departure_location;
				int loc_arr = p.arrival_location;
				int time_dep = p.departure_time;
				int time_arr = p.arrival_time;



				if ((int)genitori_donna.size() > 0) {
					// metti i bambini con la mamma
					for (Passenger& g : genitori_donna) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time  &&  time_arr == g.arrival_time) {
							g.capacity += p.capacity;
							g.weight += p.weight;
							break;
						}
					}
				}
				else if ((int)genitori_maschi.size() > 0) {
					//mettili con il papa
					for (Passenger& g : genitori_maschi) {
						if (g.departure_location == loc_dep && g.arrival_location == loc_arr && time_dep == g.departure_time  &&  time_arr == g.arrival_time) {
							g.capacity += p.capacity;
							g.weight += p.weight;
							break;
						}
					}


				}
				else {
					cout << "errore, ci sono dei bambini senza genitore" << endl;
				}

			}

		}

		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << "ora i genitori sono cosi: " << endl;
		for (Passenger& g : genitori_donna) {
			g.print();
			passengers_aggr.push_back(g);
		}
		for (Passenger& g : genitori_maschi) {
			g.print();
			passengers_aggr.push_back(g);
		}




		//cin.get();
	}
	cout << "ho finito di vedere i gruppi" << endl;
	cout << "i passeggeri nei gruppi sono: " << numero_in << endl;
	if (numero_in != (int)passengers.size()) cout << "ERROREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
	//cin.get();


	passengers_for_company_solution = passengers;
	passengers = passengers_aggr;



}





//**********************************************************************************per rolling************************************************************************


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

int best_location(map <int, int>& location_request) {
	int best_richieste = 0;
	int best_location = -1;


	for (auto x : location_request) {
		if (best_richieste < x.second) {
			best_richieste = x.second;
			best_location = x.first;
		}
	}


	return best_location;
}

//questa funzione ritorna il posto più vicino alle 4 best location rispetto alle richieste che si hanno in quel giorno ma che non è uno di quei posti
int best_location_4place(map<int, int>& location_request, vector<vector<double>>& from_to, vector<Airstrip>& airstrips) {
	map <int, int> map_support;
	for (auto x : location_request) {
		cout << x.first << " ---> " << x.second << endl;
		map_support.insert(make_pair(x.second, x.first));
	}

	vector <int> best_places;
	int index = 0;
	for (auto x : map_support) {
		//cout << x.first << " ***---> " << x.second << endl;
		if (index >= map_support.size() - 4) best_places.push_back(x.second);
		index++;
	}
	//for (auto x : best_places) cout << "- " << x << endl;

	int best_place = -1;
	double best_distance = DBL_MAX;


	for (const Airstrip& a : airstrips) {
		if (a.code != best_places[0] && a.code != best_places[1] && a.code != best_places[2] && a.code != best_places[3]) {
			double distance = from_to[a.code][best_places[0]] + from_to[a.code][best_places[1]] + from_to[a.code][best_places[2]] + from_to[a.code][best_places[3]];
			if (distance < best_distance) {
				best_distance = distance;
				best_place = a.code;
			}

		}
	}



	return best_place;
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



//PER MODELLO CONNESIONE*****************************************************************************************************************************************************************************************
//creazione dei flightleg
vector <Flightleg> creazione_flightleg(bool fittizio_da_mettere, vector<vector<Route>>& vector_solution_for_FL, map<int, Airplane>& map_airplane, vector<vector<vector<double>>>& from_to_fuelConsumed, vector<vector<double>>& from_to) {

	vector<Flightleg> flightlegs;

	for (vector<Route>& routes : vector_solution_for_FL) {
		for (Route& r : routes) {


			if (r.index > 1) {
				if (r.capacity[0] >= 1) {
					//in questo caso sono obbligato a creare un solo flight leg
					Flightleg f;
					f.cod_airplane = r.aircraft_code;
					f.from = r.places[0];
					f.to = r.places[r.index - 1];
					f.time_arr_from = r.time_arr[0];
					f.time_arr_to = r.time_arr[r.index - 1];
					f.time_dep_from = r.time_dep[0];
					f.time_dep_to = r.time_dep[r.index - 1];
					f.refuel_from = r.refueling[0];
					f.refuel_to = r.refueling[r.index - 1];
					f.quantity_fuel_from = r.quantity_fuel[0];
					f.quantity_fuel_to = r.quantity_fuel[r.index - 1];
					f.cost = r.cost;
					f.passengers_in_flightleg = r.passengers_in_route;
					f.route_padre = r;

					//in questo caso devo calcolare solo il fuel che può essere pescato
					if (f.refuel_to) {
						f.fuel_pescabile = map_airplane[f.cod_airplane].max_fuel - map_airplane[f.cod_airplane].min_fuel;
					}
					else {
						int index_refueling = 0;
						for (int i = r.index - 1; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						double fuel_pesc = r.quantity_fuel[index_refueling] - map_airplane[r.aircraft_code].min_fuel;
						for (int i = index_refueling + 1; i < r.index; i++) {
							fuel_pesc -= from_to_fuelConsumed[r.aircraft_code][i - 1][i];
						}

						f.fuel_pescabile = fuel_pesc;
					}


					flightlegs.push_back(f);
				}
				else {
					//in questo caso posso creare due flightleg
					//faccio la prima creazione uguale a quella precedente anche se il primo arco è 0
					Flightleg f;
					f.cod_airplane = r.aircraft_code;
					f.from = r.places[0];
					f.to = r.places[r.index - 1];
					f.time_arr_from = r.time_arr[0];
					f.time_arr_to = r.time_arr[r.index - 1];
					f.time_dep_from = r.time_dep[0];
					f.time_dep_to = r.time_dep[r.index - 1];
					f.refuel_from = r.refueling[0];
					f.refuel_to = r.refueling[r.index - 1];
					f.quantity_fuel_from = r.quantity_fuel[0];
					f.quantity_fuel_to = r.quantity_fuel[r.index - 1];
					f.cost = r.cost;
					f.passengers_in_flightleg = r.passengers_in_route;
					f.route_padre = r;

					//in questo caso devo calcolare solo il fuel che può essere pescato
					if (f.refuel_to) {
						f.fuel_pescabile = map_airplane[f.cod_airplane].max_fuel - map_airplane[f.cod_airplane].min_fuel;
					}
					else {
						int index_refueling = 0;
						for (int i = r.index - 1; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						double fuel_pesc = r.quantity_fuel[index_refueling] - map_airplane[r.aircraft_code].min_fuel;
						for (int i = index_refueling + 1; i < r.index; i++) {
							fuel_pesc -= from_to_fuelConsumed[r.aircraft_code][i - 1][i];
						}

						f.fuel_pescabile = fuel_pesc;
					}

					flightlegs.push_back(f);

					//ora faccio la seconda creazione se e solo se index è maggiore di 2 
					if (r.index > 2) {

						double costo_da_sottrarre = from_to[r.places[0]][r.places[1]] + from_to_fuelConsumed[r.aircraft_code][r.places[0]][r.places[1]];

						Flightleg f1;
						f1.cod_airplane = r.aircraft_code;
						f1.from = r.places[1];
						f1.to = r.places[r.index - 1];
						f1.time_arr_from = r.time_arr[1];
						f1.time_arr_to = r.time_arr[r.index - 1];
						f1.time_dep_from = r.time_dep[1];
						f1.time_dep_to = r.time_dep[r.index - 1];
						f1.refuel_from = r.refueling[1];
						f1.refuel_to = r.refueling[r.index - 1];
						f1.quantity_fuel_from = r.quantity_fuel[1];
						f1.quantity_fuel_to = r.quantity_fuel[r.index - 1];
						f1.cost = r.cost - costo_da_sottrarre;
						f1.passengers_in_flightleg = r.passengers_in_route;
						f1.route_padre = r;

						//devo calcolare il fuel minimo che necessita per essere percorsa
						if (!f1.refuel_from) {
							double fuel_necess = map_airplane[r.aircraft_code].min_fuel;
							for (int i = 2; i < r.index; i++) {
								fuel_necess += from_to_fuelConsumed[r.aircraft_code][i - 1][i];
								if (r.refueling[i])	break;
							}
							f1.fuel_necessario_per_essere_fatta = fuel_necess;
						}



						if (f1.refuel_to) {
							f1.fuel_pescabile = map_airplane[f1.cod_airplane].max_fuel - map_airplane[f1.cod_airplane].min_fuel;
						}
						else {
							int index_refueling = 0;
							for (int i = r.index - 1; i >= 0; i--) {
								if (r.refueling[i]) {
									index_refueling = i;
									break;
								}
							}
							double fuel_pesc = r.quantity_fuel[index_refueling] - map_airplane[r.aircraft_code].min_fuel;
							for (int i = index_refueling + 1; i < r.index; i++) {
								fuel_pesc -= from_to_fuelConsumed[r.aircraft_code][i - 1][i];
							}

							f1.fuel_pescabile = fuel_pesc;
						}


						flightlegs.push_back(f1);

					}





				}



			}







		}
	}


	//ora aggiungo il flightleg fittizio solo se è necessario aggiungerlo, quindi se la variabile fittizio_da_aggiungere è vera
	if (fittizio_da_mettere) {
		Flightleg f;
		f.flight_leg_fittizzio = true;
		flightlegs.push_back(f);
	}




	return flightlegs;
}
//matrice uno o zero se possono essere collegati i due flightleg
vector<vector<int>> creazione_matrice_connection_FL(double start_day, double end_day, vector<Flightleg>& flightlegs_1, vector<Flightleg>& flightlegs_2, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_fuelConsumed) {


	vector<vector<int>> matrice_da_tornare;

	for (Flightleg& f1 : flightlegs_1) {
		vector<int> riga_f1;
		if (!f1.flight_leg_fittizzio) {
			for (Flightleg& f2 : flightlegs_2) {
				if (map_airplane[f1.cod_airplane].model == map_airplane[f2.cod_airplane].model && map_airplane[f1.cod_airplane].depot == map_airplane[f2.cod_airplane].depot) {
					//se sono qua dentro i due aerei hanno lo stesso modello e deposito
					double fuel_necessario_f2 = f2.fuel_necessario_per_essere_fatta + from_to_fuelConsumed[f1.cod_airplane][f1.to][f2.from]; //fuel per essere fatta più fuel connessione
					if (f1.fuel_pescabile >= fuel_necessario_f2) {
						//se sono qua dentro il fuel va bene, ora devo controllare che il tempo vada bene
						double time_fly_connection = (from_to[f1.to][f2.from] / map_airplane[f1.cod_airplane].speed) * 60;
						bool time_ok = false;

						if (f1.time_dep_to + time_fly_connection <= end_day) time_ok = true;
						if (f2.time_arr_from - time_fly_connection >= start_day) time_ok = true;




						if (time_ok) {
							riga_f1.push_back(1);
						}
						else {
							riga_f1.push_back(0);
						}
					}
					else {
						riga_f1.push_back(0);
					}

				}
				else {
					riga_f1.push_back(0);

				}



			}

		}//qua attacco a matrice da tornare la riga del flightleg1
		else {
			//qua metto tutti uno dove c'è il flight leg fittizio
			for (Flightleg& f2 : flightlegs_2) riga_f1.push_back(1);
		}

		matrice_da_tornare.push_back(riga_f1);
	}





	return matrice_da_tornare;
}

//codice per la risoluzione di un giorno e che salva anche dentro un vettore di route tutte le route create, sarà questo da passare a creazione flightleg
vector<Route> solution_one_day(vector<vector<Route>>& vector_solution_for_FL, int Iter_FirstDo, int iterMAX_FirstDo, int number_airplane, vector<Airplane> airplanes, double peso_TW, double peso_itermediate_stop, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed, vector<Passenger>& passengers_per_casostrano, map<string, double>& from_to_company, map<int, string>& legenda_ritroso, map<int, Passenger>& map_id_passenger, vector<Passenger>& passengers, map<string, int>& mappa_aereo_tipo, map<int, int>& codice_aereo_tipo, map<int, int>& tipo_numero) {
	ofstream outfile; //per il file di input grafico.txt++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	outfile.open("grafico.txt");
	int number_iteration_in_heuristic = 0;

	double time_spent_model = 0.0;
	double tempo_finale;
	time_t time_now = time(NULL);
	//input heuristic
	//int number_airplane = stoi(argv[4]); // Per Modello
	//int number_airplane = 12;
	//cout << "il numero degli aerei e: " << number_airplane << endl;
	//cin.get();
	//int Iter_FirstDo = 0;
	//int iterMAX_FirstDo = 10;
	//double best_solution_ever = DBL_MAX;
	int NDestroy = 4;
	int NRepair = 5;
	int NHeuristic = 2;
	int initial_weigth = 5;
	vector<double> Weigth_destroy;
	vector<double> Weigth_repair;
	vector<double> Weigth_heuristic(NHeuristic);
	for (int i = 0; i < NDestroy; i++) Weigth_destroy.push_back(initial_weigth);
	for (int i = 0; i < NRepair; i++) Weigth_repair.push_back(initial_weigth);
	Weigth_heuristic[0] = 1.00; //parallel
	Weigth_heuristic[1] = 0.00; //sequential
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	vector<vector<Route>> solutionAll;
	//vector<Route> best_solution_route_ever;
	srand(time(NULL));
	int NumeroSA = 0;
	int iterazioni_fallite = 0;
	//double start_day = 360.0;    //ATTENTION, MAY BE YOU WILL HAVE TO USE IT
	double end_day = 1110.0;
	int number_of_aircraft = 20;
	vector<Route> start_solution_route;
	double start_solution = 0.0;
	int start_route_bs = 0;
	int size_route_bs = 0;  //se ci sono dei problemi rimettilo
	int best_iteration = 0;
	do {
		cout << " ************************************************************ Inizio un altro giro nel ciclo grande ************************************************************ " << endl;
		double r;
		double best_solution = DBL_MAX;
		int iteration = 0;
		int iteration_max = 30; //35 se vuopi fare un solo giro 
		int T = 25000;
		int Tmin = 150;    //50 un solo giro 
		int k = 0;
		double MinLevel = 2;
		double MaxLevel = 6;
		// Valore che puo variare 0.8 a 0.99
		double alpha = 0.87; // 0.98 un solo giro
		int DeltaK = 55;      // 25 prima
		if (Iter_FirstDo > 4) {
			T = 2500;        // Almeno 2000
			alpha = 0.95; // Almeno 0.997
			DeltaK = 30; //Almeno 25
			Tmin = 50;
			iteration_max = 80;
		}


		if (Iter_FirstDo == 0) {

			double heuristic_choice = (double)rand() / RAND_MAX;
			//cout << " heuristic_choice =  " << heuristic_choice << endl;
			//for (int i = 0; i < NHeuristic; i++) cout << Accumulated(i, Weigth_heuristic) << " || ";
			//cout << endl;
			int npass = 0;
			do {
				//auto rng1 = default_random_engine{};
				random_shuffle(begin(passengers), end(passengers));

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					//cout << "*************** I am using the Parallel Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
				}
				else {
					//cout << "*************** I am using the Sequential Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase_sequential(peso_TW, peso_itermediate_stop, airplanes, map_airplane, map_airstrip, end_day, passengers, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
				}
				for (Route& r : start_solution_route) npass += (int)r.passengers_in_route.size();
				//cout << "ecco le route" << endl;
				//for (Route r : start_solution_route) r.print();
				cout << "numero di passeggeri " << npass << endl;
				cout << "passengers.size()= " << passengers.size() << endl;
				//system("pause");
				//cin.get();
				number_iteration_in_heuristic++;
			} while ((int)passengers.size() > npass);
		}

		start_solution = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		//cout << "il costo del costruttivo e': " << start_solution << endl;
		//system("pause");
		cout << "ho appena fatto il costruttivo" << endl;
		stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
		solutionAll.push_back(start_solution_route);
		vector_solution_for_FL.push_back(start_solution_route);
		//cout << "Costo Starting Greedy Solution: " << start_solution << endl;
		//for (Route r : start_solution_route) r.print();
		//system("pause");

		//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
		outfile << "soluzione euristica di partenza:;0;" << start_solution << endl;
		//outfile.close();
		//*********************************************************************************************************************************************************************************************************************

		vector<Route> best_solution_route = start_solution_route;
		best_solution = start_solution;
		// SI INIZI IL CILCO CON LA RICERCA LOCALE E SIMULATING ANNEALING
		do {
			NumeroSA++;
			//*************************************heuristic costructive***************************************************************************************
			//cout << "Iteration number: " << iteration << endl;
			//cout << "Temperatura: " << T << endl;
			//cout << "K: " << k << endl;
			//cout << "The cost of the best solution X* is: " << best_solution << endl;
			//cout << "The cost of the current X is: " << start_solution << endl;
			double random = (double)rand() / RAND_MAX;
			double percentage_route_destroy = (MinLevel + (random * (MaxLevel - MinLevel)));
			//cout << "---> Percentuale di distruzione " << percentage_route_destroy << endl;
			int choosen_destroy = -1;
			int choosen_repair = -1;
			vector<Passenger> passenger_removed;
			// REMOVING OF A RANDOM NODE
			vector<Route> Input_destroy = start_solution_route;
			//cout << "***************************DISTRUCTION*******************" << endl;
			double destroy_choice = (double)rand() / RAND_MAX;
			vector<Route> route_destroy;
			//double starting_range = (Weigth_destroy[0]) / (Sum(Weigth_destroy));
			//cout << " destroy_choice =  " << destroy_choice << endl;
			//for (int i = 0; i < Weigth_destroy.size(); i++) cout << Accumulated(i, Weigth_destroy) << " || ";
			//cout << endl;
			//using namespace std::chrono;
			auto start = chrono::high_resolution_clock::now();
			if (destroy_choice < Accumulated(0, Weigth_destroy)) {
				//cout << "*************** I am using the Cluster Destroy ************" << endl;
				int num_passenger_cluster = (int)((passengers.size() * 0.14) + (random * ((passengers.size() * 0.24) - (passengers.size() * 0.14)))); //era 0.24 prima
				route_destroy = destroy_cluster_aggr2(peso_TW, num_passenger_cluster, passenger_removed, Input_destroy, map_airplane, from_to, passengers, map_id_passenger, peso_itermediate_stop);
				choosen_destroy = 0;
			}
			else if (destroy_choice < Accumulated(1, Weigth_destroy)) {
				//cout << "*************** I am using the Worst Destroy ************" << endl;
				route_destroy = destroy_worst(peso_TW, peso_itermediate_stop, percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 1;
			}
			else if (destroy_choice < Accumulated(2, Weigth_destroy)) {
				//cout << "*************** I am using the Casual Destroy ************" << endl;
				route_destroy = destroy_casual(percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 2;
			}
			else {
				//cout << "*************** I am using the Thanos Destroy ************" << endl;
				route_destroy = destroy_thanos(percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 3;
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
			double time_spent = Time_Spend((double)duration.count());
			//cout << " Tempo Destroy vero: " << duration.count() << " ma corrisponde al valore trasformato in double = " << time_spent << endl;
			//cout << "***************************END DISTRUCTION*******************" << endl;
			//cout << "ho appena fatto la distruzione" << endl;
			//vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa
			//stampo_caso_strano_destroy(route_destroy, from_to_company, map_airplane, legenda_ritroso);
			//for (Route p : route_destroy) p.print();
			//for(Route p : route_destroy) p.print();
			//cout << "ecco i passeggeri da reinserire: " << endl;
			//for (Passenger p : passenger_removed) p.print();
			//cout << "********************************REBUILT***************************" << endl;
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;
			//cout << " repair_choice =  " << repair_choice << endl;
			//for (int i = 0; i < Weigth_repair.size(); i++) cout << Accumulated(i, Weigth_repair) << " || ";
			//cout << endl;


			//PER TEST CON PUNTATORI************************************************************************
			/*cout << "sono prima dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;;
			int pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluione " << pass_soluzione << endl;
			int pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl;
			if (passengers.size() != (pass_dist + passenger_removed.size())) {
				cout << "si creano dei passeggeri" << endl;
				system("pause");
			}
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			*/

			auto start_r = chrono::high_resolution_clock::now();
			if (repair_choice < Accumulated(0, Weigth_repair)) {
				//cout << "*************** I am using the Repair One ************" << endl;
				solution_rebuilt = repair_one(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//for (Route r : solution_rebuilt) r.print();
				//system("pause");


				choosen_repair = 0;
			}
			else if (repair_choice < Accumulated(1, Weigth_repair)) {
				//cout << "*************** I am using the Repair SP ************" << endl;
				solution_rebuilt = repairSP(peso_TW, peso_itermediate_stop, route_destroy, passenger_removed, map_airplane, map_airstrip, from_to, airplanes, end_day, passengers, number_of_aircraft, location_fuel, from_to_FuelConsumed);
				choosen_repair = 1;
			}
			else if (repair_choice < Accumulated(2, Weigth_repair)) {
				//cout << "*************** I am using the Repair 2 Regret ************" << endl;
				solution_rebuilt = two_regret_repair_aggragati(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 2;
			}
			else if (repair_choice < Accumulated(3, Weigth_repair)) {
				//cout << "*************** I am using the Repair Forbidden ************" << endl;
				solution_rebuilt = repair_forbidden(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 3;
			}
			else {
				//cout << "*************** I am using the Repair Perturbation ************" << endl;
				solution_rebuilt = repair_perturbation(peso_TW, peso_itermediate_stop, end_day, route_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 4;
			}
			auto stop_r = chrono::high_resolution_clock::now();
			auto duration_r = chrono::duration_cast<chrono::seconds>(stop_r - start_r);
			double time_spent_r = Time_Spend((double)duration_r.count());

			/*cout << "sono dopo dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;
			pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluzione " << pass_soluzione << endl;
			pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl;
			*/


			//cout << "ho appena fatto i repair" << endl;
			//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);


			if (solution_rebuilt.size() > 0) {
				double initial_cost = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt: " << initial_cost << endl;
				//vector<Route> solution_rebuilt_without_LS = solution_rebuilt;
				//cout << "sto iniziando il move" << endl;
				solution_rebuilt = move(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
				//cout << "ho appena fatto il move" << endl;
				//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);
				//cout << "Cost of the solution rebuilt after Move: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				solution_rebuilt = swap(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
				//cout << "ho appena fatto lo swap" << endl;
				//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);
				//cout << "Cost of the solution rebuilt after Move + Swap: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				if (initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
					//cout << "in questo caso ho fatto l'inter-move" << endl;
					//int pass_per_test = 0;
					//for (Route r : solution_rebuilt) cout << "- " << r.aircraft_code << endl;

					solution_rebuilt = inter_move(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
					//cout << "ho appena fatto lo inter move" << endl;
					//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);

					/*cout << "sono dentro l'inter-move" << endl;
					cout << "passeggeri rimossi " << passenger_removed.size() << endl;
					pass_soluzione = 0;
					for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
					cout << "passenger nella soluzione " << pass_soluzione << endl;
					pass_dist = 0;
					for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
					cout << "passenger nella soluzione distrutta " << pass_dist << endl;
					cout << "prima del inter-move erano: " << pass_per_test << endl;
					if(pass_per_test != pass_soluzione) system("pause");
					*/


					//cout << "Cost of the solution rebuilt after Move + Swap: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				}
				//cout << "passato if intermove" << endl;
				solution_rebuilt = heuristic_costructive_second_fase(solution_rebuilt, end_day, peso_TW);
				double cost_objectiveFunction_second_fase_after_rebuilt = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt with Move + Swap + Second phase: " << cost_objectiveFunction_second_fase_after_rebuilt << endl;
				//cout << "la best solution fino a questo punto e': " << best_solution << endl;
				//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
				if (NumeroSA == 1000 || NumeroSA == 2000 || NumeroSA == 3000 || NumeroSA == 4000 || NumeroSA == 5000 || NumeroSA == 6000 || NumeroSA == 7000 || NumeroSA == 8000 || NumeroSA == 9000 || NumeroSA == 10000 ||
					NumeroSA == 11000 || NumeroSA == 12000 || NumeroSA == 13000 || NumeroSA == 14000 || NumeroSA == 15000 || NumeroSA == 16000 || NumeroSA == 17000 || NumeroSA == 18000 || NumeroSA == 19000 || NumeroSA == 20000 ||
					NumeroSA == 21000 || NumeroSA == 22000 || NumeroSA == 23000 || NumeroSA == 24000 || NumeroSA == 25000 || NumeroSA == 26000 || NumeroSA == 27000 || NumeroSA == 28000 || NumeroSA == 29000 || NumeroSA == 30000 ||
					NumeroSA == 31000 || NumeroSA == 32000 || NumeroSA == 33000 || NumeroSA == 34000 || NumeroSA == 35000 || NumeroSA == 36000 || NumeroSA == 37000 || NumeroSA == 38000 || NumeroSA == 39000 || NumeroSA == 40000 ||
					NumeroSA == 41000 || NumeroSA == 42000 || NumeroSA == 43000 || NumeroSA == 44000 || NumeroSA == 45000 || NumeroSA == 46000 || NumeroSA == 47000 || NumeroSA == 48000 || NumeroSA == 49000 || NumeroSA == 50000 ||
					NumeroSA == 51000 || NumeroSA == 52000 || NumeroSA == 53000 || NumeroSA == 54000 || NumeroSA == 55000 || NumeroSA == 56000 || NumeroSA == 57000 || NumeroSA == 58000 || NumeroSA == 59000 || NumeroSA == 60000) {
					//ofstream outfile1;
					//outfile1.open("grafico.txt");
					tempo_finale = difftime(time(NULL), time_now);
					outfile << "NumeroSA:;" << NumeroSA << ";" << best_solution << ";tempo:;" << to_string(tempo_finale) << endl;
					//outfile1.close();
				}
				//*********************************************************************************************************************************************************************************************************************





				//if (cost_objectiveFunction_second_fase_after_rebuilt < initial_cost) solutionAll.push_back(solution_rebuilt_without_LS);
				//cout << "costo routing: " << costo_senza_time_windows(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//cout << "costo time windows: " << costo_time_windows(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				if (cost_objectiveFunction_second_fase_after_rebuilt < start_solution) {
					//cout << "Ottimo aggiorno la soluzione da cui continuare a fare perturbazioni " << endl;
					start_solution_route.clear();
					//start_solution_route.shrink_to_fit();
					//cout << " Ho cancellato " << endl;
					//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					for (Route& so : solution_rebuilt) start_solution_route.push_back(so);
					//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					double award = Ob_Funct_Improvement(cost_objectiveFunction_second_fase_after_rebuilt, start_solution);
					//cout << " Con un miglioramento di " << start_solution - cost_objectiveFunction_second_fase_after_rebuilt << " si ha un award di " << award << endl;
					//cout << " ed un fattore tempo di Destroy di " << time_spent << endl;
					//cout << " ed un fattore tempo di Repair di " << time_spent_r << endl;
					start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
					iteration = 0;
					//cout << " Aggiungo al destroy ad al Repair 1 + award =  " << award << endl;
					Weigth_destroy[choosen_destroy] += (1 + award - time_spent);
					Weigth_repair[choosen_repair] += (1 + award - time_spent_r);
					if (cost_objectiveFunction_second_fase_after_rebuilt < best_solution) {
						//cout << "Aggiorno migliore soluzione " << endl;
						best_solution_route.clear();
						//best_solution_route.shrink_to_fit();
						for (Route& s : solution_rebuilt) best_solution_route.push_back(s);
						best_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						best_iteration = iteration;
						start_route_bs = (int)solutionAll.size();
						size_route_bs = (int)solution_rebuilt.size();
						//cout << " Ho aggiornato la migliore soluzione le route adesso si trovano nella posizione: " << start_route_bs << " con size: " << size_route_bs << endl;
						//system("pause");
					}
				}
				else {

					r = double(rand()) / (double(RAND_MAX) + 1.0);
					//cout << " Ho generato numero random " << r << endl;
					//cout << "-( " << cost_objectiveFunction_second_fase_after_rebuilt << " - " << start_solution << ") / " << T;
					double espo = -(cost_objectiveFunction_second_fase_after_rebuilt - start_solution) / T;
					//cout << " Adesso ho generato espo " << espo << endl;
					//cout << " Se non lo � devo incrementare iterazione se lo � aggiorno valori X con X'  " << endl;
					if (r <= exp(espo) && exp(espo) != 1) {
						//cout << "Confrona i costi sono entrato perche: " << r << " < " << exp(espo) << endl;
						start_solution_route.clear();
						//start_solution_route.shrink_to_fit();
						//cout << " Ho cancellato " << endl;
						//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
						for (Route& s : solution_rebuilt) start_solution_route.push_back(s);
						//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
						start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						iteration = 0;
						Weigth_destroy[choosen_destroy] ++;
						Weigth_repair[choosen_repair] ++;
					}
					else {
						iteration++;
					}
				}
				solutionAll.push_back(solution_rebuilt);
				vector_solution_for_FL.push_back(solution_rebuilt);
				cout << "Numero iterazioni ---> " << NumeroSA << endl;
			}
			else {
				//cout << " ************ ITERAZIONE FALLITA ************************* " << endl;
				iterazioni_fallite++;
				//system("pause");
			}
			k++;
			//cout << " Valore attuale di K = " << k << " aggiorno temperatura dopo: " << DeltaK << endl;
			if (k >= DeltaK) {
				T = (int)(alpha * T);
				//cout << " Aggiorno temperatura A: " << T << endl;
				k = 0;
			}
			//cout << " A T < " << Tmin << " devi fermarti T= " << T << " or " << iteration << " > " << iteration_max << " Ciclo Grande fuori iterazione ------> " << Iter_FirstDo << endl;
		} while ((iteration <= iteration_max) && (T > Tmin));
		if (Iter_FirstDo > 4) cout << " Iterazioni senza migliorare " << iteration << " Temperaruta: " << T << endl;
		tempo_finale = difftime(time(NULL), time_now);
		outfile << "soluzione prima del modello:;" << NumeroSA << ";" << best_solution << ";tempo:; " << to_string(tempo_finale) << endl;
		cout << ".........Build all the structure for the Model........." << endl;
		auto start_model = chrono::high_resolution_clock::now();
		vector<Airplane> airplanes_model;
		for (const auto& k : map_airplane) {
			airplanes_model.push_back(k.second);
		}
		map<int, vector<Route>> airplane_routes;
		map<int, int> solution_warm_up;
		for (const Airplane& a : airplanes_model) {
			vector<Route> routes;
			airplane_routes.insert(make_pair(a.code, routes));
		}
		int counter = 0;
		for (const vector<Route>& solution : solutionAll) {
			if (counter == start_route_bs) {
				for (const Route& r : solution) {
					// If � la modififica
					if (r.passengers_in_route.size() > 0) {
						solution_warm_up[r.aircraft_code] = (int)(airplane_routes[r.aircraft_code].size());
						airplane_routes[r.aircraft_code].push_back(r);
					}
				}
			}
			else {
				for (const Route& r : solution) {
					airplane_routes[r.aircraft_code].push_back(r);
				}
			}

			counter++;
		}

		cout << " CHECK SE LA MAPPA CHE HO CREATO VA BENE " << endl;
		vector<Route> Prova;
		for (const auto& a : airplane_routes) {
			if (solution_warm_up.find(a.first) != solution_warm_up.end()) {
				cout << " Aereo: " << a.first << endl;
				Prova.push_back(a.second[solution_warm_up[a.first]]);
			}
		}
		cout << " il costo della soluzione che mi sono salvato nella mappa: " << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, Prova, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
		int n = 0;
		for (Route& path : Prova) {
			//path.print();
			n += (int)path.passengers_in_route.size();
		}
		cout << " Soluzione contiene numero di passeggieri pari a " << n << endl;
		//finito di fissare gli aerei
		vector<vector<vector<int>>> A3;  //matrix A
		vector<vector<double>> C;  //cost
		vector<Route> routes;
		for (auto& airplane : airplanes_model) {
			//cout << "Airplane: " << airplane.code << endl;
			vector<double> c;
			vector<vector<int>> A2;
			cout << "la size di airplanc routes e': " << endl;
			cout << airplane_routes.size() << endl;
			for (Route& r : airplane_routes[airplane.code]) {
				//cout << "sto stampando il codice" << endl;
				//cout << "-->" << r.aircraft_code << endl;
				//r.print();
				routes.push_back(r);
				vector<int> A1;
				for (Passenger& p : passengers) {
					auto it = find_if(r.passengers_in_route.begin(), r.passengers_in_route.end(),
						[=](Passenger p_)
					{
						return p.pnr == p_.pnr;
					});
					if (it != r.passengers_in_route.end())
					{
						A1.push_back(1);
					}
					else {
						A1.push_back(0);
					}
				}
				A2.push_back(A1);
				//cout << "- " << r.cost << endl;
				c.push_back(r.cost);
			}
			A3.push_back(A2);
			C.push_back(c);
		}
		//Creating model with Cplex
		//Create new Model object
		cout << "********************************************************	0	*********************************************************************************" << endl;
		vector<Route> solution_modello;
		solutionAll.clear();
		cout << " Size del Pool di Routes Prima di chimare modello " << solutionAll.size() << endl;
		Model_Cplex *model = new Model_Cplex(A3, C, airplanes_model, airplane_routes, routes, (int)passengers.size(), passengers, solution_modello, map_airplane);
		//Create and solve model with gurobi
		model->create_Model_cplex(number_airplane, solution_warm_up, mappa_aereo_tipo, codice_aereo_tipo, tipo_numero, solutionAll);
		cout << " Size del Pool di Routes Dopo chiamata del modello " << solutionAll.size() << endl;
		cout << " Size del Pool di Routes salvate " << solutionAll[0].size() << endl;
		for (Route& s : model->solution_model) cout << " Codici aereo della soluzione: " << s.aircraft_code << endl;
		start_solution_route.clear();
		//start_solution_route.shrink_to_fit();
		if (model->solution_model.size() > 0) {
			for (Route& s : model->solution_model) start_solution_route.push_back(s);
		}
		else {
			for (Route& s : best_solution_route) start_solution_route.push_back(s);
		}
		start_solution = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		cout << " Costo del Modello che sara nuovo costo di partenza = " << start_solution << endl;
		
		//********************************************************************************************************************************************************************


		vector_solution_for_FL.push_back(start_solution_route);
		start_route_bs = (int)solutionAll.size();
		size_route_bs = (int)start_solution_route.size();
		solutionAll.push_back(start_solution_route);
		Iter_FirstDo++;
		cout << "*******************************************************************************************************************************************************" << endl;
		cout << " ********************************************** ITERATION WHILE: " << Iter_FirstDo << " OF: " << iterMAX_FirstDo << "**********************************" << endl;
		cout << "*******************************************************************************************************************************************************" << endl;
		auto stop_model = chrono::high_resolution_clock::now();
		auto duration_model = chrono::duration_cast<chrono::seconds>(stop_model - start_model);
		time_spent_model += (double)duration_model.count();
		//cin.get();
	} while ((Iter_FirstDo <= iterMAX_FirstDo));

	cout << "Best solution herurisic: " << start_solution << endl;
	cout << "costo routing: " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << "costo time windows: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	cout << " Alla iterazione numero: " << best_iteration << endl;
	for (Route s : start_solution_route) cout << s.cost << " -- " << endl;
	//*********************************************************************************AGGREGAZIONE SEMPLICE***************************************************************
	cout << "ora faccio l'aggregazione semplice" << endl;
	start_solution_route = aggrezione_simple_after_model(start_solution_route, map_airplane, from_to);
	cout << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************AGGREGAZIONE COMPLESSA***************************************************************
	cout << "ora faccio l'aggregazione complessa" << endl;
	start_solution_route = aggrezione_complex_after_model(start_solution_route, map_airplane, from_to, from_to_FuelConsumed);
	cout << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Routing cost : " << costo_senza_time_windows(start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	cout << " Time windows cost: " << costo_time_windows_and_intermediate_stop(peso_TW, peso_itermediate_stop, start_solution_route) << endl;
	//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers, start_solution_route, from_to_company, map_airplane, legenda_ritroso);
	//*********************************************************************************************************************************************************************
	cout << " ********** Final Weight ************ " << endl;
	cout << " Cluster || Worst || 2-Regret || Thanos " << endl;
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	cout << " One || SP || Regret || Forbidden ||Perturbation " << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	tempo_finale = difftime(time(NULL), time_now);
	cout << "Time at the end: " << tempo_finale << endl;
	cout << "Time model: " << time_spent_model << endl;
	cout << "Numero SA= " << NumeroSA << endl;
	cout << "Iterazioni Fallite= " << iterazioni_fallite << endl;
	cout << "number of iteration in the heuristic: " << number_iteration_in_heuristic << endl;
	outfile.close(); // per il grafico++++++++++++++++++++++++++++++++++++++++++++++++++++++


	cout << " Seleziono solo le migliori che abbiamo ... " << endl;
	map<double, int> cost; 
	map<double, int>::iterator it;


	

	int i = 0; 
	double valore = 0.0;
	for (auto vettore : vector_solution_for_FL) {
		valore = calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, vettore, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		it = cost.find(valore);
		if (it == cost.end()) {
			cost.insert(make_pair(valore, i));
		}	
		i++;
	}
	cout << " Stampo soluzioni con costo ...." << endl;
	//for (auto& elemnt : cost) cout << elemnt.first << " " << elemnt.second << endl;
	vector<vector<Route>> migliori;
	i = 0;
	for (auto& elemnt : cost) {
		if (i == 80) break;
		cout << " Butto dentro route con costo .. " << elemnt.first << " numero .. " << elemnt.second << endl;
		vector<Route> vettore = (vector_solution_for_FL.at(elemnt.second));
		//cout << " Controllo per sicurezza costo .. " << calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, vettore, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
		migliori.push_back(vector_solution_for_FL.at(elemnt.second));
		i++;
	}
		
	vector_solution_for_FL.clear();
	vector_solution_for_FL = migliori;

	return start_solution_route; // ritorno le route migliori

}

//matrice che calcola il costo della connessione
vector<vector<vector<double>>> matrice_connessione(vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed, vector<Airplane>& airplanes, vector<Airstrip>& airstrip) {
	vector<vector<vector<double>>> matrice_da_tornare;

	matrice_da_tornare.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++) {
		matrice_da_tornare[i].resize((size_t)numero_airstrip_const);
		for (int j = 0; j < numero_airstrip_const; j++) {
			matrice_da_tornare[i][j].resize((size_t)numero_airstrip_const);
		}
	}

	for (Airplane& a : airplanes) {
		for (Airstrip& loc1 : airstrip) {
			for (Airstrip& loc2 : airstrip) {
				matrice_da_tornare[a.code][loc1.code][loc2.code] = from_to_FuelConsumed[a.code][loc1.code][loc2.code] + from_to[loc1.code][loc2.code];
			}

		}

	}

	return matrice_da_tornare;

}

//matrice che per ogni flightleg ha 1 se contiente il passenger x
vector<vector<int>> creazione_matrice_flightleg_passengers(vector<Flightleg>& flightlegs, vector<Passenger>& passengers) {

	vector<vector<int>> matrice_da_tornare;
	vector<int> id_passengers;
	for (Passenger& p : passengers) id_passengers.push_back(p.pnr);



	for (Flightleg& f : flightlegs) {
		vector<int> riga_f;
		vector<int> id_FL;
		for (Passenger& p : f.passengers_in_flightleg) id_FL.push_back(p.pnr);

		for (int id_p : id_passengers) {
			std::vector<int>::iterator it = std::find(id_FL.begin(), id_FL.end(), id_p);
			if (it != id_FL.end()) {
				//l'ho trovato
				riga_f.push_back(1);
			}
			else {
				riga_f.push_back(0);
			}
		}


		matrice_da_tornare.push_back(riga_f);
	}

	bool trovato = false;
	for (int j = 0; j < id_passengers.size(); j++) {
		trovato = false;
		for (int i = 0; i < flightlegs.size(); i++) {
			if (matrice_da_tornare[i][j] == 1) trovato = true;
		}
		if (!trovato) {
			cout << " Passeggiero nuermo j " << j << " con codice passeggiero = " << id_passengers[j] << " non ha route " << endl;
			cin.get();
		}

	}
	return matrice_da_tornare;


}

//creazione matrici per il depot
vector<vector<int>> creazione_matrice_for_depot(map<int, Airplane>& map_airplane, vector<Flightleg>& flightlegs_1, vector<Flightleg>& flightlegs_2) {

	vector<vector<int>> matrice_da_tornare;

	for (Flightleg& f1 : flightlegs_1) {
		vector<int> riga_f;
		for (Flightleg& f2 : flightlegs_2) {
			if (f2.to == map_airplane[f1.cod_airplane].depot) {
				riga_f.push_back(1);
			}
			else {
				riga_f.push_back(0);
			}
		}
		matrice_da_tornare.push_back(riga_f);
	}

	return matrice_da_tornare;
}


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



//calcola il valore della funzione obiettivo sui 3 giorni
double calculate_ObjectiveFunction_complete_after_rolling(double peso_TW, double peso_intermediate_stop, vector<Route>& solution_day1, vector<Route>& solution_day2, vector<Route>& solution_day3, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	double cost_time_intermediate = 0.0;



	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;




	for (Route& r : solution_day1) {

		if (r.primo_pass && !r.airplane_day_before) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
			//cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}



		double mileage = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		COSTO_km += mileage;
		cost += fuel_consumed;
		COSTO_fuel += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto& p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_time_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;
			COSTO_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += TW_departure + TW_arrival;
			cost_time_intermediate += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;

		}
	}

	for (Route& r : solution_day2) {

		if (r.primo_pass && !r.airplane_day_before) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
			//cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}



		double mileage = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		COSTO_km += mileage;
		cost += fuel_consumed;
		COSTO_fuel += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto& p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_time_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;
			COSTO_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += TW_departure + TW_arrival;
			cost_time_intermediate += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;

		}
	}

	for (Route& r : solution_day3) {

		if (r.primo_pass && !r.airplane_day_before) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
			//cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}



		double mileage = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}

		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		COSTO_km += mileage;
		cost += fuel_consumed;
		COSTO_fuel += fuel_consumed;

		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto& p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_time_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;
			COSTO_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;

			double TW_departure = 0.0;
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			double TW_arrival = 0.0;
			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;

			cost += TW_departure + TW_arrival;
			cost_time_intermediate += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;

		}
	}



	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;costo_totale" << endl;
	cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / map_airplane[solution_day1[1].aircraft_code].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / peso_TW) << ";" << cost << endl;


	//cout << "costo delle time window e dell'intermediate stop: " << cost_time_intermediate << endl;
	return cost;
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
