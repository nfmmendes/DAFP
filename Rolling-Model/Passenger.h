#ifndef Passenger_h
#define Passenger_h


#include <string>
using namespace std;


class Passenger
{
public:
	Passenger();
	Passenger(int, int, int, string, string, string, string, string, string, string, int, int);
	~Passenger();

	int departure_location;
	int arrival_location;
	int pnr;
	string code_flight;
	string departure_location_company;
	string arrival_location_company;
	string pas_class;
	string name;
	string surname;
	string gender;
	int departure_time;
	int arrival_time;
	double weight;
	int stop;
	int chiave_regret;
	int capacity;
	string pnr_group;

	//for the passenger in the solution route to calculate the objective function
	int solution_from;
	int solution_to;

	//for repair forbidden
	int route_before;


	int early_departure;
	double mean_departure;
	int late_departure;
	int early_arrival;
	double mean_arrival;
	int late_arrival;




	void print();



};
#endif // !Passenger_h
#ifndef Passenger_h
#define Passenger_h


#include <string>
using namespace std;


class Passenger
{
public:
	Passenger();
	Passenger(int, int, int, string, string, string, string, string, string, string, int, int);
	~Passenger();

	int departure_location;
	int arrival_location;
	int pnr;
	string code_flight;
	string departure_location_company;
	string arrival_location_company;
	string pas_class;
	string name;
	string surname;
	string gender;
	int departure_time;
	int arrival_time;
	double weight;
	int stop;
	int chiave_regret;
	int capacity;
	string pnr_group;

	//for the passenger in the solution route to calculate the objective function
	int solution_from;
	int solution_to;

	//for repair forbidden
	int route_before;


	int early_departure;
	double mean_departure;
	int late_departure;
	int early_arrival;
	double mean_arrival;
	int late_arrival;




	void print();



};
#endif // !Passenger_h
