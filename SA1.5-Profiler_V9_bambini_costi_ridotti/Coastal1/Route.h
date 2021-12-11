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
	int aircraft_code;  //questo dovrebbe diventare un interoper un accesso pi� veloce 
	string aircraft_code_company_comparison;
	vector<int> airstrips;
	vector<bool> refueling;
	vector<double> fuel;
	vector<double> weights;
	int index;
	
	bool primo_pass;
	vector<double> arrival;
	vector<double> departure;

	Route();
	Route(int, const vector<Passenger>& passengers_in_route);
	~Route();

	const vector<int> getCapacities() { return capacities;  }
	void setCapacityAt(int position, int value) { if (position >= 0 && position < static_cast<int>(capacities.size())) capacities[position] = value; }
	void addCapacityAt(int position, int value) { if (position >= 0 && position < static_cast<int>(capacities.size())) capacities[position] += value; }

	vector<string> get_places_company() { return places_company;  }
	vector<string> setPlacesCompanyAt(int position, string value) { if (position >= 0 && position < static_cast<int>(places_company.size()))  places_company[position] = value;  }

	const vector<Passenger> &get_passengers() const { return passengers_in_route;  };
	Passenger& get_passenger(int position) { return passengers_in_route[position]; }
	void add_passenger(const Passenger& p) { passengers_in_route.push_back(p);  }
	void erase_passenger(int position) { passengers_in_route.erase(passengers_in_route.begin()+ position);  }
	
	void remove_at(int position)
	{		
		airstrips.erase(airstrips.begin() + position);
		refueling.erase(refueling.begin() + position);
		fuel.erase(fuel.begin() + position);
		arrival.erase(arrival.begin() + position);
		departure.erase(departure.begin() + position);
		capacities.erase(capacities.begin() + position);
		weights.erase(weights.begin() + position);
		index--;
	}
	
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
	void update_rebuilt_one_first_fase(ProcessedInput*, int& caso, int& node_add_from, int& node_add_to, int location_from, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);
	void update_a_b(const int& node_add_from,const int& node_add_to, bool& non_to, int& A, int& B);
	void update_fuel_and_weight(const int& node_add_to, const bool& non_to, double add_fuel, int index_weight_neg);
	void update_solution_from_to(int& node_add_from);
	void update_rebuilt_one_second_fase(ProcessedInput*, int& caso, int& node_add_from, int& node_add_to, int location_to, Passenger& p, bool& non_to, bool& non_to_final, bool& num_equals);

private:
	vector<int> capacities;
	vector<string> places_company;
	vector<Passenger> passengers_in_route;
};

#endif // Route_h
