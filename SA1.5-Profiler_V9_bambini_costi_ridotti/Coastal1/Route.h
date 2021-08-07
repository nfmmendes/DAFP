#ifndef Route_h
#define Route_h



#include <string>
#include <vector>
#include "Passenger.h"
#include <map>
#include "Airplane.h"
#include "Airstrip.h"
using namespace std;
const int numero_airstrip_const = 58;
const int numero_airplane_const = 36;


class Route
{
public:

	vector<string> places_company;
	double cost;
	int aircraft_code;  //questo dovrebbe diventare un interoper un accesso più veloce 
	string aircraft_code_company_comparison;
	vector<int> places;
	vector<bool> refueling;
	vector<double> quantity_fuel;
	vector<double> weight;
	int index;
	vector<int> capacity;
	bool primo_pass;

	vector<Passenger> passengers_in_route;
	vector<double> time_arr;
	vector<double> time_dep;

	Route();
	Route(int, vector<Passenger> passengers_in_route=vector<Passenger>());
	~Route();

	//void update_route(int, map<string, double>, map<string, Airplane>, map<string, Airstrip>);
	void print();
	void printCompany();
	void addPlace_companySolution(string, int);
	void addPlace(int place, bool refuel, double fuel, double weight = 0.0 , int capacity = 0, double minute_arr = 0.0, double minute_dep = 0.0);
	void removePlace(int, map<int, Airplane>&);
	void update_route_destroy(int, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&);
	void update_route_rebuilt_one(int, int, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, Passenger&, vector<vector<vector<double>>>&);
	void add_update_only_one_node_first_passanger(Passenger&, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, vector<vector<vector<double>>>&);
	void move_c(Passenger& p, int location_from, int location_to, vector<vector<double>>&, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>&);
	void update_time_for_check_repair(int node_add_from, int node_add_to, int location_from, int location_to, vector<vector<double>>&, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip);
	void update_route_rebuilt_one_first_fase(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>&, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>&, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_route_rebuilt_one_second_fase(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>&, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>&, bool& non_to, bool& non_to_final, bool& num_equals);



};

#endif // Route_h
