#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include <vector>

class Input
{

public:
	Input();
	
	
private:
	std::vector<Airstrip> airstrips;
	std::vector<Airplane> airplanes;
	std::vector<Passenger> passengers;
public:
	std::vector<Airstrip> get_airstrips() const;
	std::vector<Airplane> get_airplanes() const;
	std::vector<Passenger> get_passengers() const;


	void set_airstrips(const std::vector<Airstrip>& airstrips);
	void set_airplanes(const std::vector<Airplane>& airplanes);
	void set_passengers(const std::vector<Passenger>& passengers);
};

#endif