#include "Airstrip.h"
#include <iostream>
using namespace std;

Airstrip::Airstrip()
{
	code = 0;
	name = "";
	code_string = "";
	ground_time = 0;
	landing_cost = 0;
	lat = 0.0;
	lng = 0.0;
	fuel = false;
	fuel_cost = 0.0;
}

Airstrip::Airstrip(int code, string name, string code_string, int ground_time, int leading_cost, double lat, double lng, bool fuel, double fuel_cost) {
	this->code = code;
	this->name = name;
	this->code_string = code_string;
	this->ground_time = ground_time;
	this->landing_cost = leading_cost;
	this->lat = lat;
	this->lng = lng;
	this->fuel = fuel;
	this->fuel_cost = fuel_cost;
}

void Airstrip::print() {
	cout << "--" << name << " " << to_string(code) << " " << ground_time << " " << landing_cost
		 << " " << lat << " " << lng << " " << fuel << " " << fuel_cost << endl;
}


Airstrip::~Airstrip()
{
}
