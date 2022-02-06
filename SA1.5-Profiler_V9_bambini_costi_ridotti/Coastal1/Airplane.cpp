#include "Airplane.h"
#include <string>
#include <iostream>
using namespace std;



Airplane::Airplane()
{
	code_company = "";
	code = 0;
	capacity = 0;
	speed = 0.0;
	model = "";
	weight_airplane = 0;
	max_weight = 0;
	depot = 0;
	max_fuel = 0.0;
	fuel_burn_first = 0;
	fuel_burn_second = 0;
	min_fuel = 0.0;
	fixed_cost = 0.0;

	//for the check of the weights
	load_weight = 0.0;
}

Airplane::Airplane(int code, string code_company, int capacity, double speed, string model, 
				  int weight_airplane, int max_weight, int depot, double max_fuel, 
				  int fuel_burn_first, int fuel_burn_second, double min_fuel, double fixed_cost) {

	this->code_company = code_company;
	this->code = code;
	this->capacity = capacity;
	this->speed = speed;
	this->model = model;
	this->weight_airplane = weight_airplane;
	this->max_weight = max_weight;
	this->depot = depot;
	this->max_fuel = max_fuel;
	this->fuel_burn_first = fuel_burn_first;
	this->fuel_burn_second = fuel_burn_second;
	this->min_fuel = min_fuel;
	this->fixed_cost = fixed_cost;

	//check for the weights
	load_weight = max_weight - weight_airplane;


}

void Airplane::print() {
	cout << "--" << to_string(code) << " m:" << model << " c:" << capacity << " s:" << speed
		 << " W:" << weight_airplane << " m_W" << max_weight << " d:" <<to_string(depot)
		 << " maxF" << max_fuel << " " << fuel_burn_first << " " << fuel_burn_second
		 << " minF" << min_fuel << " w_available: " << load_weight << endl;
}

Airplane::~Airplane()
{
}

