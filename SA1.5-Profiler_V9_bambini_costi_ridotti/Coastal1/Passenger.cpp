#include "Passenger.h"
#include <string>
#include <iostream>
using namespace std;



Passenger::Passenger() {
	origin = 0;
	destination = 0;
	pnr = 0;
	code_flight = "";
	departure_location_company = "";
	arrival_location_company = "";
	pas_class = "";
	name = "";
	surname = "";
	gender = "";
	departure_time = 0;
	arrival_time = 0;
	weight = 0.0;
	stop = 0;
	chiave_regret = -1;
	pnr_group = "";

	//for the time windows
	early_departure = 0;
	late_departure = 0;
	early_arrival = 0;
	late_arrival = 0;
	mean_arrival = 0.0;
	mean_departure = 0.0;

	//for repair forbidden
	route_before = -1;
	capacity = 1;


}

Passenger::Passenger(int departure_location, int arrival_location, int pnr, string code_flight, string departure_location_comapny, string arrival_location_company,
	string pas_class, string name, string surname, string gender, int departure_time, int arrival_time) {

	this->origin = departure_location;
	this->destination = arrival_location;
	this->pnr = pnr;
	this->code_flight = code_flight;
	this->departure_location_company = departure_location_comapny;
	this->arrival_location_company = arrival_location_company;
	this->pas_class = pas_class;
	this->name = name;
	this->surname = surname;
	this->gender = gender;
	this->departure_time = departure_time;
	this->arrival_time = arrival_time;
	weight = 0.0;
	stop = 0;
	chiave_regret = -1;

	early_departure = departure_time - 25;
	late_departure = departure_time + 5;
	mean_departure = (early_departure + late_departure) / 2;

	pnr_group = "";
	late_arrival = arrival_time;
	early_arrival = arrival_time - 30;
	mean_arrival = (early_arrival + late_arrival) / 2;

	//for repair forbidden
	route_before = -1;





	//quando si implementera' bene i bambini, qua bisognera' mettere il peso giusto, la capacita' giusta, la classe del biglietto rimarra' la stessa
	capacity = 1;

	if (gender == "M") weight += 80;
	if (gender == "F") weight += 65;
	if (gender == "C") weight += 40;

	bool extra_baggage = false;
	if (pas_class.find("XL", 0) <= pas_class.size()) extra_baggage = true;
	if (extra_baggage) weight += 30;
	else weight += 15;

	if (pas_class.find("F", 0) <= pas_class.size()) {
		stop = 1;
	}
	else {
		stop = 4;
	}

}



void Passenger::print() {
	cout << "--" << name << " " << surname << " " << pnr << " " << code_flight << " " << pas_class << " " << gender << " dep: " << departure_location_company << "-->" << departure_time << " arr: "
		<< arrival_location_company << "-->" << arrival_time << " con il peso di: " << weight << " solution_from: " << solution_from << " solution_to: " << solution_to << " cap: " << capacity << endl;
	//cout << "Time windows: " << endl;
	//cout << early_departure << " - " << late_departure << " || " << early_arrival << " - " << late_arrival << endl;
}



Passenger::~Passenger()
{
}
