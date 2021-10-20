#ifndef Route_h
#define Route_h



#include <string>
#include <vector>
#include <algorithm>
#include "Passenger.h"
#include <map>
#include "Airplane.h"
#include "Airstrip.h"
class ProcessedInput;
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
	bool airplane_day_before;


	vector<Passenger> passengers_in_route;
	vector<double> time_arr;
	vector<double> time_dep;

	Route();
	Route(int, vector<Passenger>& passengers_in_route);
	~Route();

	void print();
	void printCompany();
	void addPlace_companySolution(string, int);
	void addPlace(int, bool, double, double, int, double, double);
	void removePlace(int, map<int, Airplane>&);
	void update_route_destroy(ProcessedInput* input, int, int, int);
	void update_route_rebuilt_one(ProcessedInput* ,int, int, int, int, Passenger&);
	void add_update_only_one_node_first_passanger(ProcessedInput* input, Passenger&);
	void move_c(ProcessedInput* input, Passenger& p, int location_from, int location_to);
	void update_time_for_check_repair(ProcessedInput* input, int node_add_from, int node_add_to, int location_from, int location_to);
	void update_route_rebuilt_one_first_fase(ProcessedInput* input, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_route_rebuilt_one_second_fase(ProcessedInput* input, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);

};

#endif // Route_h
