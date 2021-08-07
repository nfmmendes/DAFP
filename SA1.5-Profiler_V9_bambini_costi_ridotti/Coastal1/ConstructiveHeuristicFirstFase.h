#pragma once
#include "ConstructiveHeuristic.h"
#include "Util.h"

class ConstructiveHeuristicFirstFase : public ConstructiveHeuristic
{
	public:
		ConstructiveHeuristicFirstFase(Penalty_Weights penalties, Fleet& airplanes, Input_maps maps, double end_day, vector<Passenger>& passengers,
										int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, 
										Double3DVector& from_to_fuelConsumed);
		void insert_place_with_new_passenger(const Passenger& p, int route);
		void insert_place_removing_passenger(const Passenger& p, int route, double time);
		void insert_place_with_new_passenger_(const Passenger& p, int best_route);
		void do_work_4(Passenger p, Route r, int from, int to, bool& capacity_satisfy);
		void do_work_5(Passenger& p, int best_route, int& index);
		void do_work_6(Passenger& p, int best_route, int index);
		void update_solution_considering_fuel(Passenger& p, int best_route, double time);
		void create_connection_on_solution(Passenger& p, int best_route);
		void deal_with_situation_4(Passenger& p, int best_route, int best_from,int best_to);
		void do_work_10(Passenger& p, int best_route, int best_from);
		void do_work_11(int best_route, double add_fuel, int index_weight_neg, int j);
		void do_work_12(Passenger& p, int best_route);
		void do_work_13(Passenger p, Route r, double& cost, double& t_arr_arrival);
		void do_work_14(Passenger p, Route r, double& f_after_primo_tratto, double& f_after_secondo_tratto);
		void do_work_15(Passenger p, Route r, int from, double& cost, double& fuel_consumed, bool& fuel_ok);
		void do_work_16(Passenger p, int& situation, int& best_route, double& best_cost, int i, Route r);
		void do_work_17(Passenger p, Route r, double& cost, double& f_after_primo_tratto, double& f_after_secondo_tratto);
		void deal_with_situation_2(Passenger & p, int best_route);
		void deal_with_situation_3(Passenger p, int best_route);
		void deal_with_situation_5(vector<Route> solution, Passenger p, int best_route, int best_from);
		void deal_with_situation_1(Passenger p, int best_route);
		void do_work_1(vector<Passenger>::value_type p, int& situation, int& best_route, double& best_cost, int i,
					   vector<Route>::value_type r);
		void do_work_2(vector<Passenger>::value_type p, int& situation, int& best_route, double& best_cost, int i,
					   vector<Route>::value_type r);
		void choose_situation(Passenger p, int& situation, int& best_route, int& best_from, int& best_to);
		void ugly_do_while();
		virtual vector<Route> Execute() override;
private:
	Solution solution;
	map<int, Airplane>map_airplane;
	map<int, Airstrip>map_airstrip;
	double peso_TW;
	double peso_intermediate_stop;
	Penalty_Weights penalties;
	Fleet& airplanes;
	double end_day;
	vector<Passenger>& passengers;
	int number_of_aircraft;
	vector<vector<double>>& from_to;
	vector<vector<double>>& location_fuel;
	Double3DVector from_to_FuelConsumed;
};

