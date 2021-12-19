#ifndef Route_h
#define Route_h



#include <string>
#include <vector>
#include <algorithm>
#include "Passenger.h"
#include <map>
#include "Airplane.h"
#include "Airstrip.h"
#include "Util.h"
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

	vector<Place> places; 
	
	double cost;
	int aircraft_code;  //questo dovrebbe diventare un interoper un accesso più veloce 
	string aircraft_code_company_comparison;
	vector<int> airstrips;
	vector<double> fuel;
	int index;
	
	bool primo_pass;


	Route();
	Route(int, const vector<Passenger>& passengers_in_route);
	~Route();


	double& get_weight_at(int position) { return weights[position];  }
	vector<double> get_weights() { return weights;  }
	void set_weights(const vector<double>& p_weights) { weights = p_weights; }
	
	double& get_departure_at(int position) { return departure[position];  };
	double& get_arrival_at(int position) { return arrival[position];  };

	void set_departure_at(int position, double value) { if (position < 0 || position >= departure.size()) return; departure[position] = value; };
	void set_arrival_at(int position, double value) { if (position < 0 || position >= arrival.size()) return;  arrival[position] = value;  };

	const vector<double>& get_departures() const { return departure;  }
	const vector<double>& get_arrivals() const { return arrival;  }
	
	const vector<bool> get_refueling() const { return refueling; }
	
	const vector<int> get_capacities();
	void set_capacity_at(int position, int value);
	void add_capacity_at(int position, int value);

	vector<string> get_places_company();
	vector<string> set_places_company_at(int position, string value);

	const vector<Passenger>& get_passengers() const;;
	Passenger& get_passenger(int position);
	void add_passenger(const Passenger& p);
	void erase_passenger(int position);

	void remove_at(int position);

	void print();
	void printCompany();
	void addPlace_companySolution(string, int);
	void addPlace(int, bool, double, double, int, double, double);
	void eraseAt(int i);
	void removePlace(int, map<int, Airplane>&);
	void update_from_to(int node_destroy, int sense = 1);
	void update_route_destroy(ProcessedInput*, int, int, int);
	void do_work_3(int node_add_to);
	void update_route_rebuilt_one(ProcessedInput*, int, int, int, int, Passenger&);
	void add_update_only_one_node_first_passanger(ProcessedInput* , Passenger&);
	void move_c(ProcessedInput*, Passenger& p, int location_from, int location_to);
	void update_time_for_check_repair(ProcessedInput* ,int node_add_from, int node_add_to, int location_from, int location_to);
	void do_work(int& node_add_to, int location_to, bool& non_to, bool& non_to_final, bool& num_equals);
	void do_work2(int& node_add_to, int location_to, bool& non_to, bool& non_to_final, bool& num_equals);
	void do_work3(int& node_add_from, int& node_add_to, int location_from, int location_to, bool& non_to,
				  bool& non_to_final, bool& num_equals);
	void do_work4(int& node_add_from, int location_from, map<int, Airstrip> map_airstrip, double2DVector from_to,
				  Airplane* airplane);
	void do_work5(int& node_add_to, int location_to, bool& non_to_final, map<int, Airplane> map_airplane,
				  map<int, Airstrip> map_airstrip, double2DVector from_to);
	void update_rebuilt_one_first_fase(ProcessedInput*, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_a_b(const int& node_add_from,const int& node_add_to, bool& non_to, int& A, int& B);
	void manage_rebuilt_case_1(int& node_add_from, int& node_add_to, Passenger& p, bool& non_to);
	void manage_rebuilt_case_2(int& node_add_from, int& node_add_to, Passenger& p, bool& non_to);
	void manage_rebuilt_case_3(int& node_add_from, int& node_add_to, Passenger& p, bool& non_to, Airplane* airplane,
							   double3DVector from_to_fuelConsumed);
	void update_fuel_and_weight(const int& node_add_to, const bool& non_to, double add_fuel, int index_weight_neg);
	void update_solution_from_to(int& node_add_from);
	void update_rebuilt_one_second_fase(ProcessedInput*, int& node_add_from, int& node_add_to, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);

private:
	vector<int> capacities;
	vector<string> places_company;
	vector<Passenger> passengers_in_route;
	vector<bool> refueling;
	vector<double> arrival;
	vector<double> departure;
	vector<double> weights;
	int rebuilt_case; 
};

#endif // Route_h
