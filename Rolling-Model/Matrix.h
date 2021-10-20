
//matrice uno o zero se possono essere collegati i due flightleg
#include <iostream>
#include <map>
#include <vector>

#include "Airplane.h"
#include "Flightleg.h"

vector<vector<int>> creazione_matrice_connection_FL(ProcessedInput* input, double start_day, double end_day, vector<Flightleg>& flightlegs_1, vector<Flightleg>& flightlegs_2) {

	map<int, Airplane> map_airplane = input->get_map_airplane();
	map<int, Airstrip> map_airstrip = input->get_map_airstrip();
	double2DVector from_to = input->get_from_to();
	double3DVector from_to_FuelConsumed = input->get_from_to_fuel_consumed();
	
	vector<vector<int>> matrice_da_tornare;

	for (Flightleg& f1 : flightlegs_1) {
		vector<int> riga_f1;
		if (!f1.flight_leg_fittizzio) {
			for (Flightleg& f2 : flightlegs_2) {
				if (map_airplane[f1.cod_airplane].model == map_airplane[f2.cod_airplane].model && map_airplane[f1.cod_airplane].depot == map_airplane[f2.cod_airplane].depot) {
					//se sono qua dentro i due aerei hanno lo stesso modello e deposito
					double fuel_necessario_f2 = f2.fuel_necessario_per_essere_fatta + from_to_FuelConsumed[f1.cod_airplane][f1.to][f2.from]; //fuel per essere fatta più fuel connessione
					if (f1.fuel_pescabile >= fuel_necessario_f2) {
						//se sono qua dentro il fuel va bene, ora devo controllare che il tempo vada bene
						double time_fly_connection = (from_to[f1.to][f2.from] / map_airplane[f1.cod_airplane].speed) * 60;
						bool time_ok = false;

						if (f1.time_dep_to + time_fly_connection <= end_day) time_ok = true;
						if (f2.time_arr_from - time_fly_connection >= start_day) time_ok = true;

						if (time_ok) {
							riga_f1.push_back(1);
						}
						else {
							riga_f1.push_back(0);
						}
					}
					else {
						riga_f1.push_back(0);
					}
				}
				else {
					riga_f1.push_back(0);
				}
			}
		}//qua attacco a matrice da tornare la riga del flightleg1
		else {
			//qua metto tutti uno dove c'è il flight leg fittizio
			for (Flightleg& f2 : flightlegs_2) riga_f1.push_back(1);
		}

		matrice_da_tornare.push_back(riga_f1);
	}

	return matrice_da_tornare;
}

//matrice che calcola il costo della connessione
vector<vector<vector<double>>> matrice_connessione(vector<vector<double>>& from_to, double3DVector& from_to_FuelConsumed, vector<Airplane>& airplanes, vector<Airstrip>& airstrip) {
	vector<vector<vector<double>>> matrice_da_tornare;

	matrice_da_tornare.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++) {
		matrice_da_tornare[i].resize((size_t)numero_airstrip_const);
		for (int j = 0; j < numero_airstrip_const; j++) {
			matrice_da_tornare[i][j].resize((size_t)numero_airstrip_const);
		}
	}

	for (Airplane& a : airplanes) {
		for (Airstrip& loc1 : airstrip) {
			for (Airstrip& loc2 : airstrip) {
				matrice_da_tornare[a.code][loc1.code][loc2.code] = from_to_FuelConsumed[a.code][loc1.code][loc2.code] + from_to[loc1.code][loc2.code];
			}
		}
	}

	return matrice_da_tornare;

}

//matrice che per ogni flightleg ha 1 se contiente il passenger x
vector<vector<int>> creazione_matrice_flightleg_passengers(vector<Flightleg>& flightlegs, vector<Passenger>& passengers) {

	vector<vector<int>> matrice_da_tornare;
	vector<int> id_passengers;
	for (Passenger& p : passengers) id_passengers.push_back(p.pnr);

	for (Flightleg& f : flightlegs) {
		vector<int> riga_f;
		vector<int> id_FL;
		for (Passenger& p : f.passengers_in_flightleg) id_FL.push_back(p.pnr);

		for (int id_p : id_passengers) {
			std::vector<int>::iterator it = std::find(id_FL.begin(), id_FL.end(), id_p);
			if (it != id_FL.end()) {
				//l'ho trovato
				riga_f.push_back(1);
			}
			else {
				riga_f.push_back(0);
			}
		}

		matrice_da_tornare.push_back(riga_f);
	}

	bool trovato = false;
	for (int j = 0; j < id_passengers.size(); j++) {
		trovato = false;
		for (int i = 0; i < flightlegs.size(); i++) {
			if (matrice_da_tornare[i][j] == 1) trovato = true;
		}
		if (!trovato) {
			std::cout << " Passeggiero nuermo j " << j << " con codice passeggiero = " << id_passengers[j] << " non ha route " << std::endl;
			std::cin.get();
		}
	}
	return matrice_da_tornare;
}

//creazione matrici per il depot
std::vector<std::vector<int>> creazione_matrice_for_depot(std::map<int, Airplane>& map_airplane, vector<Flightleg>& flightlegs_1, vector<Flightleg>& flightlegs_2) {

	vector<vector<int>> matrice_da_tornare;

	for (Flightleg& f1 : flightlegs_1) {
		vector<int> riga_f;
		for (Flightleg& f2 : flightlegs_2) {
			if (f2.to == map_airplane[f1.cod_airplane].depot) {
				riga_f.push_back(1);
			}
			else {
				riga_f.push_back(0);
			}
		}
		matrice_da_tornare.push_back(riga_f);
	}

	return matrice_da_tornare;
}

