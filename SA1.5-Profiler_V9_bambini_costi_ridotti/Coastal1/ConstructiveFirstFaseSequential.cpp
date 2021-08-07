#include "ConstructiveFirstFaseSequential.h"
#include <algorithm>
#include <vector>

ConstructiveFirstFaseSequential::ConstructiveFirstFaseSequential(const Penalty_Weights& _penalties, 
																Fleet& _airplanes, 
																const Input_maps& _maps,
																const double _end_day, 
																vector<Passenger> _passengers, 
																int _number_of_aircraft, 
																vector<vector<double>>& _from_to, 
																vector<vector<double>>& _location_fuel, 
																Double3DVector& _from_to_FuelConsumed)
			: maps(_maps), penalties(_penalties), airplanes(_airplanes),end_day(_end_day),
			  passengers(_passengers), number_of_aircraft(_number_of_aircraft), from_to(_from_to),
			  location_fuel(_location_fuel), from_to_FuelConsumed(_from_to_FuelConsumed)
{	
}


double ConstructiveFirstFaseSequential::get_travel_time(Passenger passenger, Airplane& aircraft)
{
	return (((from_to[passenger.departure_location][passenger.arrival_location]) / aircraft.speed) * 60);
}


void ConstructiveFirstFaseSequential::evaluate_weight(Route& r, int best_passenger)
{
	map<int, Airplane> map_airplane = maps.map_airplane;
	//for the weight
	r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
		r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
	if (r.weight[r.index - 1] < 0) {
		r.quantity_fuel[r.index - 1] += r.weight[r.index - 1];
		r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
			r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;
	}
}

void ConstructiveFirstFaseSequential::do_work_1(Route& r, int best_passenger)
{
	map<int, Airplane> map_airplane = maps.map_airplane;
	map<int, Airstrip> map_airstrip = maps.map_airstrip;
	int departure_location = passengers[best_passenger].departure_location;
	double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];
	if (map_airstrip[departure_location].fuel) {
		r.addPlace(departure_location, map_airstrip[departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, passengers[best_passenger].capacity, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);

		r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
			map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;
	}
	else {
		double fuel_before = r.quantity_fuel[r.index - 1];
		r.addPlace(departure_location, map_airstrip[departure_location].fuel, fuel_before - fuel_consumed, 0.0, passengers[best_passenger].capacity, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);
		r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;
	}
}

vector<Route> ConstructiveFirstFaseSequential::Execute()
{
	double peso_TW = penalties.time_window_weight;
	double peso_intermediate_stop = penalties.stop_weight;

	map<int, Airplane> map_airplane = maps.map_airplane;
	map<int, Airstrip> map_airstrip = maps.map_airstrip;

	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	//TODO: Replace by for
	do {
		Route r(airplanes[cont].code);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
	//now i have created only the possible route, now i that one of them and fill it untill the condition stop

	for (Route& r : solution) {
		int situation = -1;
		do {
			situation = -1; //se situation rimane error vuol dire che nessun passeggero si pu? inserire, quindi che la route ? piena e devo passare ad un'altra
			int best_passenger = -1;
			int best_from = -1;
			int best_to = -1;
			double best_cost = DBL_MAX;

			for (int p = 0; p < (int)passengers.size(); p++) {
				auto current_passenger = passengers[p];
				if (r.primo_pass == false) {
					if (r.places[r.index - 1] == current_passenger.departure_location) {
						auto& departure_location = current_passenger.departure_location;
						auto& arrival_location = current_passenger.arrival_location;
						auto& aircraft_code = r.aircraft_code;

						//in questo caso c'? solo lui nella route, il costo ? dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = map_airplane[aircraft_code].fixed_cost + from_to[departure_location][arrival_location];
						double fuel_consumed = from_to_FuelConsumed[aircraft_code][departure_location][arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
						Airplane aircraft = map_airplane[r.aircraft_code];

						if (best_cost > cost) {
							if (fuel_after_trip >= (aircraft.min_fuel + location_fuel[r.aircraft_code][arrival_location])) {
								best_cost = cost;
								best_passenger = p;
								situation = 1;
							}
						}
					}
					else {
						//qui c'? solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere pi? kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi

						const int departure_location = current_passenger.departure_location;
						const int arrival_location = current_passenger.arrival_location;
						Airplane airplane = map_airplane[r.aircraft_code];
						double from_to_departure_arrival = from_to[departure_location][arrival_location];

						double cost = airplane.fixed_cost + from_to_departure_arrival + from_to[r.places[r.index - 1]][departure_location];
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][departure_location][arrival_location] +
							from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][departure_location];
						cost += fuel_consumed;

						//per il check sul fuel
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][departure_location];
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][departure_location][arrival_location];
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = (map_airstrip[departure_location].fuel ?
							airplane.max_fuel : r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto)
							- fuel_consumed_secondo_tratto;

						if (best_cost > cost) {
							if (f_after_primo_tratto >= airplane.min_fuel) {
								if (f_after_secondo_tratto >= (airplane.min_fuel + location_fuel[r.aircraft_code][arrival_location])) {
									best_cost = cost;
									best_passenger = p;
									situation = 2;
								}
							}
						}
					}
				}
				else {
					//nella route c'? almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilit? della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'? il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = 0; t < r.index; t++)
						if (r.places[t] == current_passenger.departure_location) FROM.push_back(t);


					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++)
							if (r.places[t] == current_passenger.arrival_location)
								TO.push_back(t);


						if (!TO.empty()) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= current_passenger.stop) {
										//se sono qua dentro il to ? maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacit?, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + current_passenger.capacity) > map_airplane[r.aircraft_code].capacity)
												capacity_satisfy = false;

											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											if (c == to - 1)
												fuel_i_j += location_fuel[r.aircraft_code][current_passenger.arrival_location];

											if (!r.refueling[c] && r.weight[c] - current_passenger.weight + (r.quantity_fuel[c] - fuel_i_j) < 0)
												capacity_satisfy = false;

										}
										//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;

										//se sorpasso il break vuol dire che soddisfo la capacit?, in questo caso il solo costo che consider? per l'aggiunta del passeggero qui
										//? la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni


										double t_arr_departure = r.time_arr[from];
										double TW_departure = ConstructiveHeuristic::evaluate_tw_departure(passengers[p], t_arr_departure);
										double t_arr_arrival = r.time_arr[to];
										double TW_arrival = evaluate_tw_arrival(passengers[p], t_arr_arrival);
										double cost = ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;
										cost += (peso_intermediate_stop * (to - from - 1));

										if (best_cost > cost) {
											best_cost = cost;
											best_passenger = p;
											situation = 4;
											best_from = from;
											best_to = to;
										}
									}
								}
							}
						}
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
						//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.places[r.index - 1] != passengers[p].arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + passengers[p].capacity) > map_airplane[r.aircraft_code].capacity)
										capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0)
											capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacit? non guardo neanche e passo al TO successivo
								if (capacity_satisfy && (r.index - from) <= passengers[p].stop) {
									const int departure_location = current_passenger.departure_location;
									const int arrival_location = current_passenger.arrival_location;
									double t_arr_departure = r.time_arr[from];
									double TW_departure = ConstructiveHeuristic::evaluate_tw_departure(passengers[p], t_arr_departure);
									double travel_time = (from_to[r.places[r.index - 1]][arrival_location]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][arrival_location];

									double cost = (TW_departure * peso_TW) + fuel_consumed + from_to[r.places[r.index - 1]][arrival_location];

									double TW_arrival = 0.0;
									double t_arr_arrival = 0.0;
									t_arr_arrival = r.time_arr[r.index - 1] + (travel_time * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
									else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
									cost += ((TW_arrival * peso_TW)) * current_passenger.capacity;
									cost += (peso_intermediate_stop * (r.index - from - 1)) * current_passenger.capacity;

									if (is_fuel_ok(map_airplane, r, current_passenger, fuel_consumed)) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][arrival_location])) {
													best_cost = cost;
													best_passenger = p;
													best_from = from;
													situation = 5;
												}
											}
										}
									}
								}
							}
						}

					}
					if (r.places[r.index - 1] != passengers[p].departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location] + from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][passengers[p].departure_location] + from_to[passengers[p].departure_location][passengers[p].arrival_location];


						//double t_arr_departure = r.time_arr[r.index - 1] + (map_airstrip[r.places[r.index - 1]].ground_time + (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60;
						double t_arr_departure = r.time_dep[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						double TW_departure = ConstructiveHeuristic::evaluate_tw_departure(passengers[p], t_arr_departure);

						double t_arr_arrival = t_arr_departure + map_airstrip[passengers[p].departure_location].ground_time + get_travel_time(passengers[p], map_airplane[r.aircraft_code]);
						double TW_arrival = evaluate_tw_arrival(passengers[p], t_arr_arrival);
						cost += ((TW_departure + TW_arrival) * peso_TW) * passengers[p].capacity;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						//ora fisso le vairabili che mi servono, questo lo faccio perch? nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = (map_airstrip[passengers[p].departure_location].fuel ?
							map_airplane[r.aircraft_code].max_fuel : r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto)
							- fuel_consumed_secondo_tratto;

						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
										best_cost = cost;
										best_passenger = p;
										situation = 3;
									}
								}
							}
						}
					}
				}
			}
			//qui dovrei avere tutte le cose per fare la mossa sempre se la route non ? piena
			if (situation == 1) {
				r.primo_pass = true;
				//time
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure; // in questo caso anche l'arrivo in quanto ? la partenza dal depot

				passengers[best_passenger].solution_from = r.index - 1;
				r.capacity[r.index - 1] += passengers[best_passenger].capacity;

				evaluate_weight(r, best_passenger);
				//end for weight


				double travel_time = ((from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60;
				double time_from = r.time_dep[r.index - 1];
				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location];

				if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people - map_airplane[r.aircraft_code].max_fuel;
					//end for weight
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];

					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;
					//end for weight

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else {
				double travel_time = (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;
				if (situation == 2) {
					r.primo_pass = true;
					r.time_arr[r.index - 1] = passengers[best_passenger].early_departure - travel_time -
						map_airstrip[r.places[r.index - 1]].ground_time;
					r.time_dep[r.index - 1] = passengers[best_passenger].early_departure - travel_time;

					//for the weight
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						r.quantity_fuel[r.index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
					//end for weight

					do_work_1(r, best_passenger);
					passengers[best_passenger].solution_from = r.index - 1;

					double time = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time + (from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

					if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
						r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);

						r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
							map_airplane[r.aircraft_code].max_fuel;
					}
					else {
						double fuel_before = r.quantity_fuel[r.index - 1];
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];

						r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
						r.weight[r.index - 1] = r.weight[r.index - 2] + passengers[best_passenger].weight + fuel_consumed;

					}
					passengers[best_passenger].solution_to = r.index - 1;
					r.passengers_in_route.push_back(passengers[best_passenger]);

				}
				else if (situation == 3) {

					if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
						r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, passengers[best_passenger].capacity,
							r.time_dep[r.index - 1] + travel_time,
							r.time_dep[r.index - 1] + travel_time + map_airstrip[passengers[best_passenger].departure_location].ground_time);

						r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
							map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;

					}
					else {
						double fuel_before = r.quantity_fuel[r.index - 1];
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];

						r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, passengers[best_passenger].capacity, r.time_dep[r.index - 1] +
							travel_time,
							r.time_dep[r.index - 1] + travel_time + map_airstrip[passengers[best_passenger].departure_location].ground_time);

						r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

					}
					passengers[best_passenger].solution_from = r.index - 1;
					double time = r.time_dep[r.index - 1] + ((from_to[passengers[best_passenger].departure_location][passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60);

					if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
						r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
						r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
							map_airplane[r.aircraft_code].max_fuel;
					}
					else {
						double fuel_before = r.quantity_fuel[r.index - 1];
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];

						r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
						r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

					}

					passengers[best_passenger].solution_to = r.index - 1;
					r.passengers_in_route.push_back(passengers[best_passenger]);
				}
				else if (situation == 4) {
					for (int h = best_from; h < best_to; h++) {
						r.capacity[h] += passengers[best_passenger].capacity;
						r.weight[h] -= passengers[best_passenger].weight;
					}

					//**************************************************************************
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = best_from; j < r.index; j++) {

						if (r.weight[j] < 0) {
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) {
									index_refueling = i;
									break;
								}
							}

							for (int t = index_refueling; t < r.index; t++) {
								if (r.refueling[t] && t != index_refueling)
									break;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
							}
						}
					}

					passengers[best_passenger].solution_from = best_from;
					passengers[best_passenger].solution_to = best_to;
					r.passengers_in_route.push_back(passengers[best_passenger]);
				}
				else if (situation == 5) {
					for (int h = best_from; h < r.index; h++) {
						r.capacity[h] += passengers[best_passenger].capacity;
						r.weight[h] -= passengers[best_passenger].weight;
					}

					//******************************************************************************
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = best_from; j < r.index; j++) {
						if (r.weight[j] < 0) {
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) {
									index_refueling = i;
									break;
								}
							}

							for (int t = index_refueling; t < r.index; t++) {
								if (r.refueling[t] && t != index_refueling)
									break;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
							}
						}
					}

					passengers[best_passenger].solution_from = best_from;
					int place_1 = r.places[r.index - 1];
					double aircraft_speed = map_airplane[r.aircraft_code].speed;
					double time = r.time_dep[r.index - 1] + ((from_to[place_1][passengers[best_passenger].arrival_location] / aircraft_speed) * 60);

					if (map_airstrip[passengers[best_passenger].arrival_location].fuel) {
						r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
						r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
							map_airplane[r.aircraft_code].max_fuel;

					}
					else {
						double fuel_before = r.quantity_fuel[r.index - 1];
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].arrival_location];
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

						r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
						r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

					}

					passengers[best_passenger].solution_to = r.index - 1;
					r.passengers_in_route.push_back(passengers[best_passenger]);

				}
			}

			if (situation != -1)
				passengers.erase(passengers.begin() + best_passenger);

		} while (situation != -1);
		if (passengers.empty())
			break;
	}

	vector<Route> solution_clean;
	for (auto s : solution) {
		if (s.index != 1)
			solution_clean.push_back(s);
	}

	return solution_clean;
}
