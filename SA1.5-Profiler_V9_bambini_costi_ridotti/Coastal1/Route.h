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

struct Place
{
	string code_company; 
	int airstrip;
	bool has_refueling;
	double fuel;
	double weight;
	double capacity;
	double arrival;
	double departure;
};


class ProcessedInput; 
//TODO: Transform in a list of airstrips 
class Route
{
public:

	vector<string> places_company;
	double cost;
	int aircraft_code;  //questo dovrebbe diventare un interoper un accesso più veloce 
	string aircraft_code_company_comparison;
	vector<int> airstrips;
	vector<bool> refueling;
	vector<double> fuel;
	vector<double> weights;
	int index;
	vector<int> capacities;
	bool primo_pass;

	vector<Passenger> passengers_in_route;
	vector<double> arrival;
	vector<double> departure;

	Route();
	Route(int, vector<Passenger>& passengers_in_route);
	~Route();

	//void update_route(int, map<string, double>, map<string, Airplane>, map<string, Airstrip>);
	void print();
	void printCompany();
	void addPlace_companySolution(string, int);
	void addPlace(int, bool, double, double, int, double, double);
	void removePlace(int, map<int, Airplane>&);
	void update_route_destroy(ProcessedInput*, int, int, int);
	void update_route_rebuilt_one(ProcessedInput*, int, int, int, int, Passenger&);
	void add_update_only_one_node_first_passanger(ProcessedInput* , Passenger&);
	void move_c(ProcessedInput*, Passenger& p, int location_from, int location_to);
	//void add_update_only_few_node_first_passanger(Passenger&, int, int, int, int, vector<vector<double>>&, map<int, Airplane>&, map<int, Airstrip>&, vector<vector<vector<double>>>&);
	void update_time_for_check_repair(ProcessedInput* ,int node_add_from, int node_add_to, int location_from, int location_to);
	void update_route_rebuilt_one_first_fase(ProcessedInput*, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_route_rebuilt_one_second_fase(ProcessedInput*, int& caso, int& node_add_from, int& node_add_to, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);
};

#endif // Route_h
