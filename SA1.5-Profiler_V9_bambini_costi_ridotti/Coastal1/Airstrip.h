#ifndef Airstrip_h
#define Airstrip_h

#include <string>
using namespace std;


class Airstrip
{
public:
	Airstrip();
	Airstrip(int, string, string, int, int, double, double, bool, double);
	~Airstrip();

	string name;
	string code_string;
	int code;
	int ground_time;
	int landing_cost;
	double lat;
	double lng;
	bool fuel;
	double fuel_cost;




	//function
	void print();





};

#endif // Airstrip_h