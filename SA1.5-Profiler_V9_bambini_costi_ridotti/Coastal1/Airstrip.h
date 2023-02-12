#ifndef Airstrip_h
#define Airstrip_h

#include <string>
using namespace std;

class Airstrip
{
public:
	Airstrip();
	Airstrip(unsigned int, string, string, int, int, double, double, bool, double);
	~Airstrip();

	unsigned int code;
	int ground_time;
	int landing_cost;
	double lat;
	double lng;
	double fuel_cost;
	bool fuel;
	string name;
	string code_string;

	//function
	void print();
};

#endif // Airstrip_h