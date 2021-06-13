#ifndef Route_h
#define Route_h



#include <string>
#include <vector>
#include <algorithm>
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
	int aircraft_code;
	string aircraft_code_company_comparison;
	vector<int> places;
	vector<bool> refueling;
	vector<double> quantity_fuel;
	vector<double> weight;
	int index;
	vector<int> capacity;
	
	bool primo_pass[3];
	vector<int> starting_locations;
	int start_end_routes[3][2];


	//bool airplane_day_before;

	vector<Passenger> passengers_in_route;
	vector<double> time_arr;
	vector<double> time_dep;

	Route();
	Route(int, vector<Passenger>& passengers_in_route);
	~Route();





	//void update_route(int, map<string, double>, map<string, Airplane>, map<string, Airstrip>);
	void print();
	void print1();
	void print2();
	void print3();
	void Print();
	void printCompany();
	void addPlace_companySolution(string, int);
	void addPlace(int, bool, double, double, int, double, double);
	void removePlace(int, map<int, Airplane>&);
	void removePlace_New(int position, map<int, Airplane>& map_airplane);
	void removePlace_Arc(map<int, Airplane>& map_airplane);
	void update_route_destroy_middleNode(int giorno, int node_destroy, int min_from_pass, int max_to_pass, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip);


	void update_route_destroy(int giorno, int node_destroy, int min_from_pass, int max_to_pass, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip);
	void update_route_rebuilt_one(int, int, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, Passenger&, vector<vector<vector<double>>>&);




	void update_route_rebuilt_one_first_fase(int&, int&, int&, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, Passenger&, vector<vector<vector<double>>>&, bool&, bool&, bool&);
	void update_route_rebuilt_one_second_fase(int&, int&, int&, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int , Airstrip>&, Passenger&, vector<vector<vector<double>>>&, bool&, bool&, bool&);

	
	void move_c(Passenger&, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, vector<vector<vector<double>>>&);

	void update_time_for_check_repair(int, int, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&);


	//per rolling
	void add_update_only_one_node_first_passanger_MP(double start_day1, double start_day2, double start_day3, Passenger&, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, vector<vector<vector<double>>>&);
	void update_route_rebuilt_one_first_fase_without_after(int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_route_rebuilt_one_second_fase_without_after(int& caso, int& node_add_from, int& node_add_to, int  location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals);
	void move_c_without_after(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed);
	void update_route_rebuilt_one_first_fase_with_after(bool& messa_connessione, double start_day, double&  fuel_connection, double& weight_connection, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_route_rebuilt_one_second_fase_with_after(bool& messa_connessione, double&  fuel_connection, double& weight_connection,int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, Passenger& p, vector<vector<vector<double>>>& from_to_fuelConsumed, bool& non_to, bool& non_to_final, bool& num_equals);
	void move_c_with_after(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed);
	void move_before(Passenger& p, int location_from, int location_to, vector<vector<double>>& from_to, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<vector<double>>>& from_to_fuelConsumed);



};

#endif // Route_h