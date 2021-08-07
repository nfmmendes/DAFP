#ifndef Airplane_h
#define Airplane_h

#include <string>

using namespace std;


class Airplane
{
public:

	string code_company;
	int code;
	int capacity;
	double speed;
	string model;
	int weight_airplane;
	int max_weight;
	int depot;
	double max_fuel;
	int fuel_burn_first;
	int fuel_burn_second;
	double min_fuel;
	double fixed_cost;

	//for the check of the weight
	double weight_fuel_people;


	//costructors and distructor
	Airplane();
	Airplane(int, string, int, double, string, int, int, int, double, int, int, double, double);
	virtual ~Airplane();

	//function
	void print();


};



#endif // !Airplane_h