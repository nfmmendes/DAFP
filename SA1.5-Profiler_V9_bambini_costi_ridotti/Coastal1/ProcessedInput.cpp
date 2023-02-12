#include "ProcessedInput.h"

ProcessedInput::ProcessedInput(map<int, Airplane>& map_airplane, map<unsigned int, Airstrip>& map_airstrip,
	double2DVector& from_to, double2DVector& location_fuel, double3DVector& from_to_fuel_consumed):map_airplane(map_airplane), map_airstrip(map_airstrip), from_to(from_to), location_fuel(location_fuel), from_to_fuel_consumed(from_to_fuel_consumed)
{
}

map<int, Airplane>& ProcessedInput::get_map_airplane()
{
	return map_airplane;
}

const map<unsigned int, Airstrip>& ProcessedInput::get_map_airstrip() const
{
	return map_airstrip;
}

const double2DVector& ProcessedInput::get_from_to() const
{
	return from_to;
}

const double2DVector& ProcessedInput::get_location_fuel() const
{
	return location_fuel;
}

const double3DVector& ProcessedInput::get_from_to_fuel_consumed() const
{
	return from_to_fuel_consumed;
}

void ProcessedInput::set_map_airplane(const map<int, Airplane>& p_map_airplane)
{
	map_airplane = p_map_airplane;
}

void ProcessedInput::set_map_airstrip(const map<unsigned int, Airstrip>& p_map_airstrip)
{
	map_airstrip = p_map_airstrip; 
}

void ProcessedInput::set_from_to(const double2DVector& p_from_to)
{
	from_to = p_from_to;
}

void ProcessedInput::set_location_fuel(const double2DVector& p_location_fuel)
{
	location_fuel = p_location_fuel;
}

void ProcessedInput::set_from_to_fuel_consumed(const double3DVector& p_from_to_fuel_consumed)
{
	from_to_fuel_consumed = p_from_to_fuel_consumed;
}

