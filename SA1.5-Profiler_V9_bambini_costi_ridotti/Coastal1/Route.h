#ifndef Route_h
#define Route_h



#include <string>
#include <vector>
#include "Passenger.h"
#include <map>
#include "Airplane.h"
#include "Airstrip.h"
#include "Util.h"
using namespace std;
const int numero_airstrip_const = 58;
const int numero_airplane_const = 36;

struct AirplaneStatus
{
	double fuel;
	double weight;
	unsigned int capacity;
};

struct Place
{
	string code_company; 
	int airstrip;
	bool has_refueling;
	AirplaneStatus airplane_status;
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
	unsigned int aircraft_code;  //questo dovrebbe diventare un interoper un accesso più veloce 
	string aircraft_code_company_comparison;
	vector<double> fuel;
	unsigned int index;
	
	bool primo_pass;


	Route();
	Route(int, const vector<Passenger>& passengers_in_route);
	~Route();

	vector<unsigned int> get_airstrips() const { return airstrips;  }

	double& get_weight_at(unsigned int position) { return weights[position];  }
	vector<double> get_weights() { return weights;  }
	
	double& get_departure_at(unsigned int position) { return departure[position];  };
	double& get_arrival_at(unsigned position) { return arrival[position];  };

	const vector<double>& get_departures() const { return departure;  }
	const vector<double>& get_arrivals() const { return arrival;  }
	
	const vector<bool> get_refueling() const { return refueling; }
	
	const vector<unsigned int> get_capacities();
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
	void addPlace(int, bool, AirplaneStatus, double, double);
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
	void do_work4(int& node_add_from, int location_from, map<unsigned int, Airstrip> map_airstrip, double2DVector from_to,
				  Airplane* airplane);
	void do_work5(int& node_add_to, int location_to, bool& non_to_final, map<int, Airplane> map_airplane,
				  map<unsigned int, Airstrip> map_airstrip, double2DVector from_to);
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
	vector<unsigned int> airstrips;
	vector<unsigned int> capacities;
	vector<string> places_company;
	vector<Passenger> passengers_in_route;
	vector<bool> refueling;
	vector<double> arrival;
	vector<double> departure;
	vector<double> weights;
	int rebuilt_case; 
};

#endif // Route_h
