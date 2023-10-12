#ifndef AGGREGATE_H
#define AGGREGATE_H

#include <iostream>

#include "Route.h"

void remove_node(Route& r, const int &node)
{
	if ((node + 1) == (r.index - 1)) {
		r.get_departure_at(node + 1) = r.get_departures()[node];
		r.get_arrival_at(node + 1) = r.get_arrivals()[node];
		r.eraseAt(node);
	}
	else {
		// I remove the extra time that was considered
		double time_to_remove = r.get_departures()[node + 1] - r.get_departures()[node];
		for (int i = node + 1; i < r.index; i++) {
			r.get_departure_at(i) -= time_to_remove;
			r.get_arrival_at(i) -= time_to_remove;
		}
		r.remove_at(node);
	}
}

void aggregate_same_nodes(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for(int i=0; i< r.get_passengers().size(); i++){
		Passenger &p = r.get_passenger(i);
		
		if (p.solution_from == (1 + node)) 
			p.solution_from = node;
		if (p.solution_to == (1 + node)) 
			p.solution_to = node;
		if (p.solution_from > (1 + node))
			p.solution_from--;
		if (p.solution_to > (1 + node))
			p.solution_to--;
	}

	// Now, we remove the node
	remove_node(r, node);
}

void aggregate_same_nodes_inter_ls(Route& r, int node) {
	// La modicfica del form e to del passeggiero non funziona
	for (int i = 0; i < r.get_passengers().size(); i++) {
		auto& p = r.get_passenger(i);
		
		if (p.solution_from == (1 + node)) 
			p.solution_from = node;
		if (p.solution_to == (1 + node)) 
			p.solution_to = node;
		if (p.solution_from > (1 + node))
			p.solution_from--;
		if (p.solution_to > (1 + node))	
			p.solution_to--;
	}

	// Now, we remove the node
	if ((node + 1) == (r.index - 1)) {
		r.get_departure_at(node + 1) = r.get_departure_at(node);
		r.get_arrival_at(node + 1) = r.get_arrival_at(node);

		r.remove_at(node);
	}
	else {
		// I remove the extra time that was considered
		double time_to_remove = r.get_departure_at(node + 1) - r.get_departure_at(node);
		for (unsigned int i = node + 1; i < r.index; i++) {
			if (i == node + 1) {
				r.get_departure_at(i) = r.get_departure_at(node);
				r.get_arrival_at(i) = r.get_arrival_at(node);
			}
			else {
				r.get_departure_at(i) -= time_to_remove;
				r.get_arrival_at(i) -= time_to_remove;
			}

		}
		r.remove_at(node);
	}
}

vector <Route> aggrezione_simple_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to) {
	vector<Route> solution_after;
	
	//cerco di collegare le route se ? possibile
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		Route* route = &solution_model[r];
		Airplane* airplane = &map_airplane[route->aircraft_code];
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";

			Route* route_r1 = &solution_model[r1];
			Airplane* airplane_r1 = &map_airplane[route_r1->aircraft_code];

			//TODO: Replace it by a list of break if's
			if (r != r1 && route->get_airstrips()[route->index - 1] == route_r1->get_airstrips()[0] &&
				route->get_departure_at(route->index - 1) <= route_r1->get_arrival_at(0) &&
				airplane->model == airplane_r1->model
				&& index_not.find(val, 0) > index_not.size() && index_not.find(val1, 0) > index_not.size()) {

				index_not += to_string(r) + ";" + to_string(r1) + ";";
				Route r_add(route->aircraft_code, route->get_passengers());
				r_add.primo_pass = true;

				for (int i = 0; i < route->index - 1; i++) {
					double travel_time = airplane->travelTime(from_to[route->get_airstrips()[i]][solution_model[r1].get_airstrips()[0]]);
					if (i == route->index - 2) {
						AirplaneStatus airplane_status{ route->fuel[i], route->get_weights()[i], route->get_capacities()[i] };
						r_add.addPlace(route->get_airstrips()[i], route->get_refueling()[i], airplane_status, route->get_arrivals()[i], (route_r1->get_arrivals()[0] - travel_time));
					}
					else {
						AirplaneStatus airplane_status{ route->fuel[i], route->get_weights()[i],route->get_capacities()[i] };
						r_add.addPlace(route->get_airstrips()[i], route->get_refueling()[i], airplane_status, route->get_arrivals()[i], route->get_departures()[i]);
					}
				}

				for (int i = 0; i < route_r1->index; i++) {
					AirplaneStatus airplane_status{ route_r1->fuel[i], route_r1->get_weights()[i],route_r1->get_capacities()[i] };
					r_add.addPlace(route_r1->get_airstrips()[i], route_r1->get_refueling()[i],airplane_status , route_r1->get_arrivals()[i], route_r1->get_departures()[i]);
				}
				
				for (auto& p : solution_model[r1].get_passengers()) {
					auto new_passenger = p;
					new_passenger.solution_from += (route->index - 1);
					new_passenger.solution_to += (route->index - 1);
					r_add.add_passenger(new_passenger);
				}

				solution_after.push_back(r_add);
			}
		}
	}

	for (int r = 0; r < (int)solution_model.size(); r++) {
		string val = ";" + to_string(r) + ";";
		if (index_not.find(val, 0) > index_not.size()) {
			solution_after.push_back(solution_model[r]);
		}
	}
	
	return solution_after;
}

vector <Route> aggrezione_complex_after_model(vector<Route>& solution_model, map<int, Airplane>& map_airplane, double2DVector& from_to, double3DVector& from_to_FuelConsumed) {
	vector <Route> solution_after;
	//cerco di collegare le route se ? possibile
	string index_not = ";";
	for (int r = 0; r < (int)solution_model.size(); r++) {
		Route* route = &solution_model[r];
		Airplane* airplane = &map_airplane[route->aircraft_code];
		for (int r1 = 0; r1 < (int)solution_model.size(); r1++) {
			Route* route_r1 = &solution_model[r1];
			Airplane* airplane_r1 = &map_airplane[route_r1->aircraft_code];
			
			string val = to_string(r) + ";";
			string val1 = to_string(r1) + ";";
			// TODO: Refactor here
			double time_trascorso = airplane->travelTime(from_to[route->get_airstrips()[route->index - 1]][route_r1->get_airstrips()[0]]);
			double fuel_consumed = from_to_FuelConsumed[route->aircraft_code][route->get_airstrips()[route->index - 1]][route_r1->get_airstrips()[0]];

			double costo_aggiunta = fuel_consumed + from_to[route->get_airstrips()[route->index - 1]][route_r1->get_airstrips()[0]];
			//READ ME: non ? importante fare il check se i due posti (finale di una route, inziale della seconda) siano uguali,
			// questa casistica viene eliminata dall'aggregazione semplice non devo nemmeno fare il check sul fuel per andare
			// in un altro posto in quanto vado sicuramente in un deposito dato che tutte le route partono in un depot
			// CONSIDERAZIONE, SI POTREBBE VALUTARE DI TOGLIERE IL PRIMO ARCO DAL DEPOT ALLA PRIMA LOCATION NEL CASO QUESTO SIA VUOTO,
			// MA QUESTO PORTEREBBE ALLA CREAZIONE DI SCENARI DIFFERENTI:
			//	- posso arrivare a tutte le altre locazioni se tolgo il refuel al depot?????????????????????????????????????
			// ho aggiunto il fatto che il costo del tratto aggiuntivo deve essere minore o uguale al costo fisso dell'aereo
			
			// TODO: Replace it by a list of break if's
			if (r != r1 && route->get_departures()[route->index - 1] + time_trascorso <= route->get_arrivals()[0] &&
				route->fuel[route->index - 1] - fuel_consumed >= airplane->min_fuel &&
				airplane->model == airplane_r1->model
				&& index_not.find(val, 0) > index_not.size() && index_not.find(val1, 0) > index_not.size()
				&& costo_aggiunta <= airplane_r1->fixed_cost) {

				index_not += to_string(r) + ";" + to_string(r1) + ";";
				Route r_add(route->aircraft_code, route->get_passengers());
				r_add.primo_pass = true; //N.B. commentare questa riga se si vuole vedere quelle che aggrega
				for (int i = 0; i <= route->index - 1; i++) {
					double time_to_go = airplane->travelTime(from_to[route->get_airstrips()[i]][route_r1->get_airstrips()[0]]);
					if (i == route->index - 1) {
						AirplaneStatus airplane_status{ route->fuel[i], route->get_weights()[i], route->get_capacities()[i] };
						r_add.addPlace(route->get_airstrips()[i], route->get_refueling()[i], airplane_status, route->get_arrivals()[i], (route_r1->get_arrivals()[0] - time_to_go));
					}
					else {
						AirplaneStatus airplane_status{ route->fuel[i], route->get_weights()[i],route->get_capacities()[i] };
						r_add.addPlace(route->get_airstrips()[i], route->get_refueling()[i], airplane_status, route->get_arrivals()[i], route->get_departures()[i]);
					}
				}

				for (int i = 0; i < route_r1->index; i++) {
					AirplaneStatus airplane_status{ route_r1->fuel[i], route_r1->get_weights()[i],
						route_r1->get_capacities()[i] };
					r_add.addPlace(route_r1->get_airstrips()[i], route_r1->get_refueling()[i], airplane_status, route_r1->get_arrivals()[i], route_r1->get_departures()[i]);
				}
				
				for (const Passenger& p : route_r1->get_passengers()) {
					auto new_passenger = p;
					new_passenger.solution_from += (route->index);
					new_passenger.solution_to += (route->index);
					r_add.add_passenger(new_passenger);

				}
				solution_after.push_back(r_add);
			}
		}
	}

	for (int r = 0; r < (int)solution_model.size(); r++) {
		string val = ";" + to_string(r) + ";";
		if (index_not.find(val, 0) > index_not.size()) {
			solution_after.push_back(solution_model[r]);
		}
	}

	return solution_after;
}

#endif
