#pragma once
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"


class Input
{

public:
	Input();
	
	
private:
	vector<Airstrip> airstrips;
	vector<Airplane> airplanes;
	vector<Passenger> passengers;
public:
	vector<Airstrip> get_airstrips() const;
	vector<Airplane> get_airplanes() const;
	vector<Passenger> get_passengers() const;


	void set_airstrips(const vector<Airstrip>& airstrips);
	void set_airplanes(const vector<Airplane>& airplanes);
	void set_passengers(const vector<Passenger>& passengers);
};

