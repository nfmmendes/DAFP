#ifndef Airplane_h
#define Airplane_h


#include <string>
#include <vector>

using namespace std;

struct FuelSpecifications
{
	double max_fuel;
	double min_fuel;
	int fuel_burn_first;
	int fuel_burn_second;
};

struct AirplaneModel
{
	int capacity;
	double speed;
	int weight_airplane;
	int max_weight;
	FuelSpecifications fuel_specifications;
	double fixed_cost;
};

class Airplane
{
public:

	string code_company;
	unsigned int code;
	unsigned int capacity;
	double speed;
	string model;
	unsigned int weight_airplane;
	unsigned int max_weight;
	unsigned int depot;
	double max_fuel;
	int fuel_burn_first;
	int fuel_burn_second;
	double min_fuel;
	double fixed_cost;

	//for the check of the weights
	double load_weight;


	//costructors and distructor
	Airplane();
	Airplane(unsigned int code, string code_company, unsigned int capacity, double speed, string model,
		unsigned int weight_airplane, unsigned int max_weight, unsigned int depot, double max_fuel,
		unsigned int fuel_burn_first, unsigned int fuel_burn_second, double min_fuel, double fixed_cost);
	virtual ~Airplane();

	//function
	void print();

	// Get the time in minutes to travel given a distance
	double travelTime(double distance);

};



#endif // !Airplane_h