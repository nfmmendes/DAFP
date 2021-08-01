#include "Input.h"

Input::Input()
{
	
}


vector<Airstrip> Input::get_airstrips() const
{
	return airstrips;
}

vector<Airplane> Input::get_airplanes() const
{
	return airplanes;
}

vector<Passenger> Input::get_passengers() const
{
	return passengers;
}

void Input::set_airstrips(const vector<Airstrip>& airstrips)
{
	this->airstrips = airstrips;
}

void Input::set_airplanes(const vector<Airplane>& airplanes)
{
	this->airplanes = airplanes;
}

void Input::set_passengers(const vector<Passenger>& passengers)
{
	this->passengers = passengers;
}
