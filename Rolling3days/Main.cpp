#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <list>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <random>
#include "Airplane.h"
#include "Airstrip.h"
#include "Passenger.h"
#include "Route.h"
#include "Node.h"
#include <float.h>
#include <chrono>
//#include "Model.h"

using namespace std;

template<typename type>
struct MyCOMP {
	bool operator() (
		const type & first, const type & second) const {
		return first > second;
	}

};

template<typename type>
struct MyCOMP1 {
	bool operator() (
		const type & first, const type & second) const {
		return first < second;
	}

};


double Sum(vector<double> Weight) {
	double sum = 0;
	for (double i : Weight) sum += i;
	return sum;
}

double Accumulated(int j, vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) sum += i;
	for (int i = 0; i <= j; i++) accum += ((Weight[i]) / sum);
	return accum;
}

double Ob_Funct_Improvement(double newsolution, double oldsolution) {
	double rate_max_improvement = 50000;
	if (newsolution >= oldsolution) {
		return 0;
	}
	else {
		return ((oldsolution - newsolution) / rate_max_improvement);
	}
}

double Time_Spend(double time) {
	double rate_time = 20;
	return time / rate_time;

}




//STAMPO CASO STRANOO*******************************************************************
void stampo_caso_strano_singleroute(map<string, int>& legenda, map<int, Airstrip> map_airstrip, Route r, vector<vector<double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double end_day1, double start_day2, double end_day2, double start_day3, double end_day3, Route routes_old) {




	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}





	}


	if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == r.index || r.start_end_routes[1][1] - r.start_end_routes[1][0] == r.index || r.start_end_routes[2][1] - r.start_end_routes[2][0] == r.index) {
		cout << " problema della mancanza di nodi rispetto index" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	if (r.primo_pass[0] == false && r.index > r.start_end_routes[0][1] && r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1) {
		cout << " problema della casistica" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}



	bool pass_in_day2 = false;
	bool pass_in_day1 = false;
	for (Passenger pas : r.passengers_in_route) {
		if (pas.giorno == 2) pass_in_day2 = true;
		if (pas.giorno == 1) pass_in_day1 = true;
	}


	if (r.primo_pass[0] == false) {
		if (r.start_end_routes[0][1] - r.start_end_routes[0][0] > 1) {
			cout << " problema nel giorno 1" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}
	if (r.primo_pass[1] == false) {
		if (r.start_end_routes[1][1] - r.start_end_routes[1][0] > 1) {
			cout << " problema nel giorno 2" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}
	if (r.primo_pass[2] == false) {
		if (r.start_end_routes[2][1] - r.start_end_routes[2][0] > 1) {
			cout << " problema nel giorno 3" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}


	/*for (int i = r.start_end_routes[1][0]; i < r.start_end_routes[1][1] - 1; i++) {
		if (r.time_dep[i] < start_day2) {
			if ((i == r.start_end_routes[1][0] && r.places[i] != "ARK" && r.places[i] != "DAR" ) || i > r.start_end_routes[1][0]) {
				cout << " HO UN NODO NEL GIORNO 2 CHE NON E NEL SUO GIORNO " << endl;
				cout << " Nodo: " << r.places[i] << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
	}
	for (int i = r.start_end_routes[2][0]; i < r.start_end_routes[2][1] - 1; i++) {
		if (r.time_dep[i] < start_day3) {
			if ((i == r.start_end_routes[2][0] && r.places[i] != "ARK" && r.places[i] != "DAR") || i > r.start_end_routes[2][0]) {
				cout << " HO UN NODO NEL GIORNO 3 CHE NON E NEL SUO GIORNO " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
	}*/




	if (r.primo_pass[0] && r.start_end_routes[0][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[1] && r.start_end_routes[1][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[2] && r.start_end_routes[2][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[0] && pass_in_day1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno uno" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[1] && pass_in_day2) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno 2" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[2] && r.start_end_routes[2][1] != -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}





	if (r.start_end_routes[0][1] != r.start_end_routes[1][0] && r.start_end_routes[1][0] != -1 && r.start_end_routes[0][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.start_end_routes[1][1] != r.start_end_routes[2][0] && r.start_end_routes[2][0] != -1 && r.start_end_routes[1][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}






	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}




	if (r.index > r.places.size()) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		cout << " Route Vecchia: " << endl;
		//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

	}



	for (int i = 1; i < r.index; i++) {

		/*cout << " Sono dentro stampa caso strano single route analizzo nodo " << i << endl;
		if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/




		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/





		if (r.places[i - 1] != r.places[i]) {
			//cout << " Confontoo " << r.places[i - 1] << " e " << r.places[i] << endl;
			/*if (r.time_dep[i - 1] > r.time_arr[i] || r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << " Tempo che ci vuole " << (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60 << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				cout << " Route Vecchia: " << endl;
				//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					cout << " Route Vecchia: " << endl;
					//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted






		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		if (r.places[i] != legenda["ARK"] && r.places[i] != legenda["DAR"]) {

			if (i == r.start_end_routes[0][0]) {
				if (r.time_dep[i] < start_day1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day1 << endl;
					cout << " Route Vecchia: " << endl;
					//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();


				}
			}
			if (i == r.start_end_routes[1][0]) {
				if (r.time_dep[i] < start_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day2 << endl;
					cout << " Route Vecchia: " << endl;
					//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();


				}

				if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
					cout << " Route Vecchia: " << endl;
					//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}


			}
			if (i == r.start_end_routes[2][0]) {
				if (r.time_dep[i] < start_day3) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day3 << endl;
					cout << " Route Vecchia: " << endl;
					//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();

				}

				if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
					cout << " Route Vecchia: " << endl;
					//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}


			}

		}


		//if (r.places[i] != "SEU") {
			/*if (r.places[i] != "ARK") {
				if (r.places[i] != "DAR") {
					if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
						cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
					if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
						cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
					if (r.time_arr[i] > end_day3) {
						cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
			}*/
			//}







		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
			}
		}

		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}


	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
				r.Print();
				system("pause");
				cin.get();
			}

		}
	}












}

void stampo_caso_strano(map<string, int>& legenda, map<int, Airstrip> map_airstrip, vector<Route> routes, vector<vector< double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double end_day1, double start_day2, double end_day2, double start_day3, double end_day3, vector<Route> routes_old) {

	for (Route r : routes) {

		for (int i = 0; i < r.index; i++) {
			int capacita_arco = 0;
			for (Passenger& p : r.passengers_in_route) {
				if (p.solution_from <= i && p.solution_to > i) {
					capacita_arco++;
				}
			}
			if (r.capacity[i] != capacita_arco) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO PROBLEMA CON LA CAPACITA', dovrebbe essere: " << capacita_arco << " mentre invece e': " << r.capacity[i] << endl;
				cout << "nell'arco: " << i << endl;
				cout << " Route Riparata: " << endl;
				r.Print();
				cout << " Route Vecchia: " << endl;
				for (Route ri : routes_old) if (r.aircraft_code == ri.aircraft_code) ri.Print();
				system("pause");
				cin.get();
			}
		}




		if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
			cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}





		if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1 && r.primo_pass[0] == false && r.capacity[0] == 0 && r.index > 2) {
			cout << " problema della mancanza di spostamento per Nell" << endl;
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

		if (r.start_end_routes[0][1] == 1 && r.time_arr[0] >= end_day1) {
			cout << " problema della mancanza di spostamento per Nell ho indici errato ci deve essere zero " << endl;
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == r.index || r.start_end_routes[1][1] - r.start_end_routes[1][0] == r.index || r.start_end_routes[2][1] - r.start_end_routes[2][0] == r.index) {
			cout << " problema della mancanza di nodi rispetto index" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		if (r.primo_pass[0] == false && r.index > r.start_end_routes[0][1] && r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1) {
			cout << " problema della casistica" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}



		bool pass_in_day2 = false;
		bool pass_in_day1 = false;
		for (Passenger pas : r.passengers_in_route) {
			if (pas.giorno == 2) pass_in_day2 = true;
			if (pas.giorno == 1) pass_in_day1 = true;
		}


		if (r.primo_pass[0] == false) {
			if (r.start_end_routes[0][1] - r.start_end_routes[0][0] > 1) {
				cout << " problema nel giorno 1" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
		if (r.primo_pass[1] == false) {
			if (r.start_end_routes[1][1] - r.start_end_routes[1][0] > 1) {
				cout << " problema nel giorno 2" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
		if (r.primo_pass[2] == false) {
			if (r.start_end_routes[2][1] - r.start_end_routes[2][0] > 1) {
				cout << " problema nel giorno 3" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}


		/*for (int i = r.start_end_routes[1][0]; i < r.start_end_routes[1][1] - 1; i++) {
			if (r.time_dep[i] < start_day2) {
				if ((i == r.start_end_routes[1][0] && r.places[i] != "ARK" && r.places[i] != "DAR" ) || i > r.start_end_routes[1][0]) {
					cout << " HO UN NODO NEL GIORNO 2 CHE NON E NEL SUO GIORNO " << endl;
					cout << " Nodo: " << r.places[i] << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}
		for (int i = r.start_end_routes[2][0]; i < r.start_end_routes[2][1] - 1; i++) {
			if (r.time_dep[i] < start_day3) {
				if ((i == r.start_end_routes[2][0] && r.places[i] != "ARK" && r.places[i] != "DAR") || i > r.start_end_routes[2][0]) {
					cout << " HO UN NODO NEL GIORNO 3 CHE NON E NEL SUO GIORNO " << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}*/



		if ((r.time_dep[0] > (start_day1 - 50)) && r.time_dep[0] < end_day1 && r.start_end_routes[0][0] > 0 && r.index > 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO INDICE DEL NODO ZERO " << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		else if (r.time_dep[0] > (start_day2 - 50) && r.time_dep[0] < end_day2  && r.start_end_routes[1][0] > 0 && r.index > 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO INDICE DEL NODO ZERO " << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();

		}
		else if (r.time_dep[0] > (start_day3 - 50) && r.time_dep[0] < end_day3  && r.start_end_routes[2][0] > 0 && r.index > 1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO INDICE DEL NODO ZERO " << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();

		}



		if (r.primo_pass[0] && r.start_end_routes[0][1] == -1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		if (r.primo_pass[1] && r.start_end_routes[1][1] == -1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		if (r.primo_pass[2] && r.start_end_routes[2][1] == -1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		if (!r.primo_pass[0] && pass_in_day1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno uno" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		if (!r.primo_pass[1] && pass_in_day2) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno 2" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		if (!r.primo_pass[2] && r.start_end_routes[2][1] != -1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}





		if (r.start_end_routes[0][1] != r.start_end_routes[1][0] && r.start_end_routes[1][0] != -1 && r.start_end_routes[0][1] != -1) {
			cout << "index is: " << r.index << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		if (r.start_end_routes[1][1] != r.start_end_routes[2][0] && r.start_end_routes[2][0] != -1 && r.start_end_routes[1][1] != -1) {
			cout << "index is: " << r.index << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}






		if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}




		if (r.index > r.places.size()) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
			cout << " Route Vecchia: " << endl;
			for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		for (Passenger p : r.passengers_in_route) {
			if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				p.print();
				cout << "nella route: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}

			if (p.solution_from >= p.solution_to) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				p.print();
				cout << "nella route: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}

		}



		for (int i = 1; i < r.index; i++) {




			/*if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}*/


			/*if (r.time_dep[i] < r.time_arr[i]) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}*/




			/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}*/





			if (r.places[i - 1] != r.places[i]) {
				//cout << " Confontoo " << r.places[i - 1] << " e " << r.places[i] << endl;
				/*if (r.time_dep[i - 1] > r.time_arr[i] || r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
					cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
					cout << " Route Vecchia: " << endl;
					for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}*/


				double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
				double fuel_consumed = 0.0;
				if (fly_time <= 1) {
					fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
				}

				if (!r.refueling[i]) {
					if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
						cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
			}





		}


		for (int i = 0; i < r.index - 1; i++) {
			if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}


			/*if (r.places[i] == r.places[i + 1]) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
				r.print();
				system("pause");
				cin.get();
			}
			*/

		}



		for (int i = 1; i < r.index; i++) {
			if (r.places[i] == r.places[i - 1]) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}






		for (int i = 0; i < r.index; i++) {










			if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
				cout << " Route Vecchia: " << endl;
				for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}


			if (r.places[i] != legenda["ARK"] && r.places[i] != legenda["DAR"]) {

				if (i == r.start_end_routes[0][0]) {
					if (r.time_dep[i] < start_day1) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day1 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();


					}
				}
				if (i == r.start_end_routes[1][0]) {
					if (r.time_dep[i] < start_day2) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day2 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();


					}

					if (r.time_arr[i] > end_day2) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();
					}


				}
				if (i == r.start_end_routes[2][0]) {
					if (r.time_dep[i] < start_day3) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day3 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();

					}

					if (r.time_arr[i] > end_day2) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();
					}


				}

			}


			//AGGIUNGO QUESTA FUNZIONE PER VEDERE DOVE POTREBBE ESSERE L'ERRORE
			if (r.places[i] != legenda["ARK"] && r.places[i] != legenda["DAR"]) {
				if (i == r.start_end_routes[0][1]) {
					if (r.time_arr[i] > end_day1) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' problema con l'end day 1" << end_day1 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
				if (i == r.start_end_routes[1][1]) {
					if (r.time_arr[i] > end_day2) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' problema con l'end day 2" << end_day2 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
				if (i == r.start_end_routes[2][1]) {
					if (r.time_arr[i] > end_day3) {
						cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' problema con l'end day 3" << end_day3 << endl;
						cout << " Route Vecchia: " << endl;
						for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
						cout << " Route Riparata: " << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
			}


			//if (r.places[i] != "SEU") {
				/*if (r.places[i] != "ARK") {
					if (r.places[i] != "DAR") {
						if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
							cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
							r.Print();
							system("pause");
							cin.get();
						}
						if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
							cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
							r.Print();
							system("pause");
							cin.get();
						}
						if (r.time_arr[i] > end_day3) {
							cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
							r.Print();
							system("pause");
							cin.get();
						}
					}
				}*/
				//}







			double peso_arco_passeggeri = 0.0;
			for (Passenger p : r.passengers_in_route) {
				if (p.solution_from <= i && p.solution_to > i) {
					peso_arco_passeggeri += p.weight;
				}
			}

			if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}


		for (int i = 0; i < r.index - 1; i++) {
			double minimo_peso = r.weight[i];
			if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
				for (int j = i + 1; j < r.index; j++) {
					if (r.refueling[j]) break;
					if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
				}
				if (minimo_peso >= 1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
					r.Print();
					system("pause");
					cin.get();
				}

			}
		}







	}




}

void stampo_caso_strano_single_move(map<string, int>& legenda, map<int, Airstrip> map_airstrip, Route r, vector<vector< double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double end_day1, double start_day2, double end_day2, double start_day3, double end_day3, Route routes_old) {

	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}






	cout << " SONO dentro stampa caso strano ....... " << endl;
	if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1 && r.primo_pass[0] == false && r.capacity[0] == 0 && r.index > 2) {
		cout << " problema della mancanza di spostamento per Nell" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}

	if (r.start_end_routes[0][1] == 1 && r.time_arr[0] >= end_day1) {
		cout << " problema della mancanza di spostamento per Nell ho indici errato ci deve essere zero " << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == r.index || r.start_end_routes[1][1] - r.start_end_routes[1][0] == r.index || r.start_end_routes[2][1] - r.start_end_routes[2][0] == r.index) {
		cout << " problema della mancanza di nodi rispetto index" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	if (r.primo_pass[0] == false && r.index > r.start_end_routes[0][1] && r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1) {
		cout << " problema della casistica" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}



	bool pass_in_day2 = false;
	bool pass_in_day1 = false;
	for (Passenger pas : r.passengers_in_route) {
		if (pas.giorno == 2) pass_in_day2 = true;
		if (pas.giorno == 1) pass_in_day1 = true;
	}


	if (r.primo_pass[0] == false) {
		if (r.start_end_routes[0][1] - r.start_end_routes[0][0] > 1) {
			cout << " problema nel giorno 1" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}
	if (r.primo_pass[1] == false) {
		if (r.start_end_routes[1][1] - r.start_end_routes[1][0] > 1) {
			cout << " problema nel giorno 2" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}
	if (r.primo_pass[2] == false) {
		if (r.start_end_routes[2][1] - r.start_end_routes[2][0] > 1) {
			cout << " problema nel giorno 3" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}


	/*for (int i = r.start_end_routes[1][0]; i < r.start_end_routes[1][1] - 1; i++) {
		if (r.time_dep[i] < start_day2) {
			if ((i == r.start_end_routes[1][0] && r.places[i] != "ARK" && r.places[i] != "DAR" ) || i > r.start_end_routes[1][0]) {
				cout << " HO UN NODO NEL GIORNO 2 CHE NON E NEL SUO GIORNO " << endl;
				cout << " Nodo: " << r.places[i] << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
	}
	for (int i = r.start_end_routes[2][0]; i < r.start_end_routes[2][1] - 1; i++) {
		if (r.time_dep[i] < start_day3) {
			if ((i == r.start_end_routes[2][0] && r.places[i] != "ARK" && r.places[i] != "DAR") || i > r.start_end_routes[2][0]) {
				cout << " HO UN NODO NEL GIORNO 3 CHE NON E NEL SUO GIORNO " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
	}*/




	if (r.primo_pass[0] && r.start_end_routes[0][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[1] && r.start_end_routes[1][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[2] && r.start_end_routes[2][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[0] && pass_in_day1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno uno" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[1] && pass_in_day2) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno 2" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[2] && r.start_end_routes[2][1] != -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}





	if (r.start_end_routes[0][1] != r.start_end_routes[1][0] && r.start_end_routes[1][0] != -1 && r.start_end_routes[0][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.start_end_routes[1][1] != r.start_end_routes[2][0] && r.start_end_routes[2][0] != -1 && r.start_end_routes[1][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}






	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}




	if (r.index > r.places.size()) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

	}



	for (int i = 1; i < r.index; i++) {


		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/




		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/





		if (r.places[i - 1] != r.places[i]) {
			//cout << " Confontoo " << r.places[i - 1] << " e " << r.places[i] << endl;
			/*if (r.time_dep[i - 1] > r.time_arr[i] || r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				cout << " Route Vecchia: " << endl;
				routes_old.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted






		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		if (r.places[i] != legenda["ARK"] && r.places[i] != legenda["DAR"]) {

			if (i == r.start_end_routes[0][0]) {
				if (r.time_dep[i] < start_day1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day1 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();


				}
			}
			if (i == r.start_end_routes[1][0]) {
				if (r.time_dep[i] < start_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day2 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();


				}

				if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}


			}
			if (i == r.start_end_routes[2][0]) {
				if (r.time_dep[i] < start_day3) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day3 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();

				}

				if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day3 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}


			}

		}


		//if (r.places[i] != "SEU") {
			/*if (r.places[i] != "ARK") {
				if (r.places[i] != "DAR") {
					if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
						cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
					if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
						cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
					if (r.time_arr[i] > end_day3) {
						cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
			}*/
			//}







		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
			}
		}

		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}


	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
				r.Print();
				system("pause");
				cin.get();
			}

		}
	}






}

void stampo_caso_strano_single_destroy(map<string, int>& legenda, map<int, Airstrip> map_airstrip, Route r, vector<vector< double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double end_day1, double start_day2, double end_day2, double start_day3, double end_day3, Route routes_old) {

	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}

	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << " Route Vecchia: " << endl;
			//for (Route r2 : routes_old) if (r.aircraft_code == r2.aircraft_code) r2.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}




	cout << " SONO dentro stampa caso strano ....... " << endl;
	if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1 && r.primo_pass[0] == false && r.capacity[0] == 0 && r.index > 2) {
		cout << " problema della mancanza di spostamento per Nell" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}

	if (r.start_end_routes[0][1] == 1 && r.time_arr[0] >= end_day1) {
		cout << " problema della mancanza di spostamento per Nell ho indici errato ci deve essere zero " << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	if (r.start_end_routes[0][1] - r.start_end_routes[0][0] == r.index || r.start_end_routes[1][1] - r.start_end_routes[1][0] == r.index || r.start_end_routes[2][1] - r.start_end_routes[2][0] == r.index) {
		cout << " problema della mancanza di nodi rispetto index" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	if (r.primo_pass[0] == false && r.index > r.start_end_routes[0][1] && r.start_end_routes[0][1] - r.start_end_routes[0][0] == 1) {
		cout << " problema della casistica" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}



	bool pass_in_day2 = false;
	bool pass_in_day1 = false;
	for (Passenger pas : r.passengers_in_route) {
		if (pas.giorno == 2) pass_in_day2 = true;
		if (pas.giorno == 1) pass_in_day1 = true;
	}




	/*for (int i = r.start_end_routes[1][0]; i < r.start_end_routes[1][1] - 1; i++) {
		if (r.time_dep[i] < start_day2) {
			if ((i == r.start_end_routes[1][0] && r.places[i] != "ARK" && r.places[i] != "DAR" ) || i > r.start_end_routes[1][0]) {
				cout << " HO UN NODO NEL GIORNO 2 CHE NON E NEL SUO GIORNO " << endl;
				cout << " Nodo: " << r.places[i] << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
	}
	for (int i = r.start_end_routes[2][0]; i < r.start_end_routes[2][1] - 1; i++) {
		if (r.time_dep[i] < start_day3) {
			if ((i == r.start_end_routes[2][0] && r.places[i] != "ARK" && r.places[i] != "DAR") || i > r.start_end_routes[2][0]) {
				cout << " HO UN NODO NEL GIORNO 3 CHE NON E NEL SUO GIORNO " << endl;
				r.Print();
				system("pause");
				cin.get();
			}
		}
	}*/




	if (r.primo_pass[0] && r.start_end_routes[0][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[1] && r.start_end_routes[1][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[2] && r.start_end_routes[2][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[0] && pass_in_day1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno uno" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[1] && pass_in_day2) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno 2" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[2] && r.start_end_routes[2][1] != -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}





	if (r.start_end_routes[0][1] != r.start_end_routes[1][0] && r.start_end_routes[1][0] != -1 && r.start_end_routes[0][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}
	if (r.start_end_routes[1][1] != r.start_end_routes[2][0] && r.start_end_routes[2][0] != -1 && r.start_end_routes[1][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}






	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}




	if (r.index > r.places.size()) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		cout << " Route Vecchia: " << endl;
		routes_old.Print();
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			p.print();
			cout << "nella route: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}

	}



	for (int i = 1; i < r.index; i++) {


		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/




		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/





		if (r.places[i - 1] != r.places[i]) {
			//cout << " Confontoo " << r.places[i - 1] << " e " << r.places[i] << endl;
			/*if (r.time_dep[i - 1] > r.time_arr[i] || r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				cout << " Route Vecchia: " << endl;
				routes_old.Print();
				cout << " Route Riparata: " << endl;
				r.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted






		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			routes_old.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}


		if (r.places[i] != legenda["ARK"] && r.places[i] != legenda["DAR"]) {

			if (i == r.start_end_routes[0][0]) {
				if (r.time_dep[i] < start_day1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day1 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();


				}
			}
			if (i == r.start_end_routes[1][0]) {
				if (r.time_dep[i] < start_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day2 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();


				}

				if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}


			}
			if (i == r.start_end_routes[2][0]) {
				if (r.time_dep[i] < start_day3) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day3 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();

				}

				if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day3 << endl;
					cout << " Route Vecchia: " << endl;
					routes_old.Print();
					cout << " Route Riparata: " << endl;
					r.Print();
					system("pause");
					cin.get();
				}


			}

		}


		//if (r.places[i] != "SEU") {
			/*if (r.places[i] != "ARK") {
				if (r.places[i] != "DAR") {
					if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
						cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
					if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
						cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
					if (r.time_arr[i] > end_day3) {
						cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
						r.Print();
						system("pause");
						cin.get();
					}
				}
			}*/
			//}







		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
			}
		}

		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
			r.Print();
			system("pause");
			cin.get();
		}
	}


	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
				r.Print();
				system("pause");
				cin.get();
			}

		}
	}






}

void stampo_caso_strano_single_route_tempi(Route r_before, Route r, vector<vector< double>>& from_to, map<int, Airplane> map_airplane, map<int, Airstrip> map_airstrip) {

	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			//cout << "passenger inserito: " << endl;
			//p_inserito.print();
			r.Print();
			cout << "la route prima era" << endl;
			r_before.Print();
			system("pause");
			cin.get();
		}
	}


	for (int i = 1; i < r.index; i++) {

		//cout << "tempo " << i << " --> " << r.time_arr[i] << endl;

		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			r.Print();
			cout << "la route prima era" << endl;
			r_before.Print();
			system("pause");
			cin.get();
		}*/


		/*if (r.places[i - 1] != r.places[i]) {
			if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				r.Print();
				cout << "la route prima era" << endl;
				r_before.Print();
				system("pause");
				cin.get();
			}


		}*/



		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			r_before.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/


	}







}

void stampo_caso_strano_single_all(map<string, int>& legenda, map<int, Airstrip> map_airstrip, Route r_prima, Passenger pi, Route r, vector<vector< double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3) {

	bool pass_in_day2 = false;
	bool pass_in_day1 = false;
	for (Passenger pas : r.passengers_in_route) {
		if (pas.giorno == 2) pass_in_day2 = true;
		if (pas.giorno == 1) pass_in_day1 = true;
	}







	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}

	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << "passenger inserito: " << endl;
			pi.print();
			r.Print();
			cout << "la route prima era" << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}
	}





	if (r.primo_pass[0] && r.start_end_routes[0][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[1] && r.start_end_routes[1][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[2] && r.start_end_routes[2][1] == -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[0] && pass_in_day1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno uno" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[1] && pass_in_day2) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno 2" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[2] && r.start_end_routes[2][1] != -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}


	if (r.start_end_routes[0][1] != r.start_end_routes[1][0] && r.start_end_routes[1][0] != -1 && r.start_end_routes[0][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (r.start_end_routes[1][1] != r.start_end_routes[2][0] && r.start_end_routes[2][0] != -1 && r.start_end_routes[1][1] != -1) {
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}






	//cout << "controllo start ed end day " << endl;

	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}



	//cout << "controllo controllo index " << endl;
	if (r.index > r.places.size()) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}

	//cout << "controllo i passenger " << endl;
	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			p.print();
			cout << "nella route: " << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			p.print();
			cout << "nella route: " << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.print();
			system("pause");
			cin.get();
		}

	}


	//cout << "controllo i tempi ed il fuel consumato " << endl;
	for (int i = 1; i < r.index; i++) {

		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			r_prima.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/



		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}*/



		/*if ((r.time_dep[i] - r.time_arr[i]) > (map_airstrip[r.places[i]].ground_time + 3) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "  ---> " << r.time_dep[i] - r.time_arr[i] << "il gorund time e': " << (map_airstrip[r.places[i]].ground_time) << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.print();
			system("pause");
			cin.get();
		}*/





		if (r.places[i - 1] != r.places[i]) {
			/*if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;

				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;

					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	//cout << "controllo il fuel che cali " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted





		/*
		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		*/


		//cout << "controllo i nodi di partenza " << endl;




		//if (r.places[i] != "SEU") {
		/*if (r.places[i] != "ARK") {
			if (r.places[i] != "DAR") {
				if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
					cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
					cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day3) {
					cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}*/
		//}




		//cout << "controllo la capacita' ed il peso totale " << endl;

		int numero_pass = 0;
		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
				numero_pass++;
			}
		}
		if (pi.solution_from <= i && pi.solution_to > i) {
			peso_arco_passeggeri += pi.weight;
			numero_pass++;
		}

		if (numero_pass != r.capacity[i]) {
			cout << "numero passeggeri: " << numero_pass << endl;
			cout << "capacità: " << r.capacity[i] << endl;
			pi.print();
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO c'e un problema con la capacita' nel nodo: " << i << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}



		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << "weight: " << r.weight[i] << endl;
			cout << "quantity fuel: " << r.quantity_fuel[i] << endl;
			cout << "peso passeggeri: " << peso_arco_passeggeri << endl;
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}
	}


	//cout << "controllo il fuel non massimo " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO ************************* nel nodo: " << i << endl;

				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}

		}
	}













}

void stampo_caso_strano_single_all_primo_pass(map<string, int>& legenda, Route r_prima, Passenger pi, Route r, vector<vector<double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, map<int, Airstrip> map_airstrip) {


	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}


	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << "passenger inserito: " << endl;
			pi.print();
			r.Print();
			cout << "la route prima era" << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}
	}






	if (r.start_end_routes[0][1] != -1 && r.start_end_routes[1][0] != -1) {
		if (r.start_end_routes[0][1] != r.start_end_routes[1][0]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

	}
	if (r.start_end_routes[1][1] != -1 && r.start_end_routes[2][0] != -1) {
		if (r.start_end_routes[1][1] != r.start_end_routes[2][0]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

	}

	//cout << "controllo start ed end day " << endl;

	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.Print();
		system("pause");
		cin.get();
	}



	//cout << "controllo controllo index " << endl;
	if (r.index > r.places.size()) {
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.Print();
		system("pause");
		cin.get();
	}

	//cout << "controllo i passenger " << endl;
	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			p.print();
			r.print();
			cout << "nella route: " << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			p.print();
			r.print();
			cout << "nella route: " << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

	}


	//cout << "controllo i tempi ed il fuel consumato " << endl;
	for (int i = 1; i < r.index; i++) {


		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			r_prima.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/


		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}*/




		if (r.places[i - 1] != r.places[i]) {
			/*if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				r.print();
				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					r.print();
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	//cout << "controllo il fuel che cali " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted





		/*
		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		*/


		//cout << "controllo i nodi di partenza " << endl;

		if (r.places[i] != legenda["ARK"] && r.places[i] != legenda["DAR"]) {

			if (i == r.start_end_routes[0][0]) {
				if (r.time_dep[i] < start_day1) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day1 << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();


				}
			}
			if (i == r.start_end_routes[1][0]) {
				if (r.time_dep[i] < start_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day2 << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();


				}

				/*if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
					r.print();
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}*/


			}
			if (i == r.start_end_routes[2][0]) {
				if (r.time_dep[i] < start_day3) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day3 << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();

				}

				/*if (r.time_arr[i] > end_day3) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day " << end_day3 << endl;
					r.print();
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}*/


			}

		}


		//if (r.places[i] != "SEU") {
		/*if (r.places[i] != "ARK") {
			if (r.places[i] != "DAR") {
				if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
					cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
					cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day3) {
					cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
			}
		}*/
		//}




		//cout << "controllo la capacita' ed il peso totale " << endl;

		int numero_pass = 0;
		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
				numero_pass++;
			}
		}
		if (pi.solution_from <= i && pi.solution_to > i) {
			peso_arco_passeggeri += pi.weight;
			numero_pass++;
		}

		if (numero_pass != r.capacity[i]) {
			cout << "numero passeggeri: " << numero_pass << endl;
			cout << "capacità: " << r.capacity[i] << endl;
			pi.print();
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO c'e un problema con la capacita' nel nodo: " << i << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}



		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << "weight: " << r.weight[i] << endl;
			cout << "quantity fuel: " << r.quantity_fuel[i] << endl;
			cout << "peso passeggeri: " << peso_arco_passeggeri << endl;
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}
	}


	//cout << "controllo il fuel non massimo " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
				r.print();
				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}

		}
	}













}


void stampo_caso_strano_single_route_tempi(Passenger p_inserito, Route r_before, Route r, vector<vector<double>>& from_to, map<int, Airplane> map_airplane, map<int, Airstrip> map_airstrip) {


	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}



	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			r.Print();
			cout << "la route prima era" << endl;
			r_before.Print();
			system("pause");
			cin.get();
		}


		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			r_before.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/




	}




	for (int i = 1; i < r.index; i++) {

		//cout << "tempo " << i << " --> " << r.time_arr[i] << endl;

		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			r.Print();
			cout << "la route prima era" << endl;
			r_before.Print();
			system("pause");
			cin.get();
		}*/


		/*if (r.places[i - 1] != r.places[i]) {
			if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "passenger inserito: " << endl;
				p_inserito.print();
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				r.Print();
				cout << "la route prima era" << endl;
				r_before.Print();
				system("pause");
				cin.get();
			}


		}*/

	}







}

void stampo_caso_strano_single_all(Passenger p_inserito, map<int, Airstrip>& map_airstrip, Route r_prima, Passenger pi, Route r, vector<vector<double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3) {

	bool pass_in_day2 = false;
	bool pass_in_day1 = false;
	for (Passenger pas : r.passengers_in_route) {
		if (pas.giorno == 2) pass_in_day2 = true;
		if (pas.giorno == 1) pass_in_day1 = true;
	}






	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}

	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			r.Print();
			cout << "la route prima era" << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}


		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			r_prima.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/


	}


	if (r.primo_pass[0] && r.start_end_routes[0][1] == -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[1] && r.start_end_routes[1][1] == -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (r.primo_pass[2] && r.start_end_routes[2][1] == -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[0] && pass_in_day1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno uno" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[1] && pass_in_day2) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero nel giorno 2" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (!r.primo_pass[2] && r.start_end_routes[2][1] != -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end e primo passeggero" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}


	if (r.start_end_routes[0][1] != r.start_end_routes[1][0] && r.start_end_routes[1][0] != -1 && r.start_end_routes[0][1] != -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}
	if (r.start_end_routes[1][1] != r.start_end_routes[2][0] && r.start_end_routes[2][0] != -1 && r.start_end_routes[1][1] != -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "index is: " << r.index << endl;
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}






	//cout << "controllo start ed end day " << endl;

	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}



	//cout << "controllo controllo index " << endl;
	if (r.index > r.places.size()) {
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.print();
		system("pause");
		cin.get();
	}

	//cout << "controllo i passenger " << endl;
	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			p.print();
			cout << "nella route: " << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			p.print();
			cout << "nella route: " << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.print();
			system("pause");
			cin.get();
		}

	}


	//cout << "controllo i tempi ed il fuel consumato " << endl;
	for (int i = 1; i < r.index; i++) {


		/*if (r.time_dep[i] < r.time_arr[i]) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}*/



		/*if ((r.time_dep[i] - r.time_arr[i]) > (map_airstrip[r.places[i]].ground_time + 3) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "  ---> " << r.time_dep[i] - r.time_arr[i] << "il gorund time e': " << (map_airstrip[r.places[i]].ground_time) << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.print();
			system("pause");
			cin.get();
		}*/





		if (r.places[i - 1] != r.places[i]) {
			/*if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << "passenger inserito: " << endl;
				p_inserito.print();
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;

				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << "passenger inserito: " << endl;
					p_inserito.print();
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;

					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	//cout << "controllo il fuel che cali " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted





		/*
		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		*/


		//cout << "controllo i nodi di partenza " << endl;




		//if (r.places[i] != "SEU") {
		/*if (r.places[i] != "ARK") {
			if (r.places[i] != "DAR") {
				if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
					cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
					cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day3) {
					cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					system("pause");
					cin.get();
				}
			}
		}*/
		//}




		//cout << "controllo la capacita' ed il peso totale " << endl;

		int numero_pass = 0;
		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
				numero_pass++;
			}
		}
		if (pi.solution_from <= i && pi.solution_to > i) {
			peso_arco_passeggeri += pi.weight;
			numero_pass++;
		}

		if (numero_pass != r.capacity[i]) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "numero passeggeri: " << numero_pass << endl;
			cout << "capacità: " << r.capacity[i] << endl;
			pi.print();
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO c'e un problema con la capacita' nel nodo: " << i << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}



		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "weight: " << r.weight[i] << endl;
			cout << "quantity fuel: " << r.quantity_fuel[i] << endl;
			cout << "peso passeggeri: " << peso_arco_passeggeri << endl;
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;

			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}
	}


	//cout << "controllo il fuel non massimo " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << "passenger inserito: " << endl;
				p_inserito.print();
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO ************************* nel nodo: " << i << endl;

				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}

		}
	}













}

void stampo_caso_strano_single_all_primo_pass(Passenger p_inserito, Route r_prima, Passenger pi, Route r, vector<vector<double>>& from_to, map<int, Airplane> map_airplane, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, map<int, Airstrip>& map_airstrip) {

	if (r.start_end_routes[0][1] >= r.index || r.start_end_routes[1][1] >= r.index || r.start_end_routes[2][1] >= r.index) {
		cout << " problema dell'indice ultimo uguale o maggiore di index della route" << endl;
		cout << " Route Riparata: " << endl;
		r.Print();
		system("pause");
		cin.get();
	}






	for (int i = 1; i < r.index; i++) {
		if (r.places[i] == r.places[i - 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO NODI DOPPI" << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			r.Print();
			cout << "la route prima era" << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}


		/*if (r.time_dep[i] - r.time_arr[i] > (map_airstrip[r.places[i]].ground_time + 1) && i != r.start_end_routes[0][0] && i != r.start_end_routes[0][1] && i != r.start_end_routes[1][0] && i != r.start_end_routes[1][1] && i != r.start_end_routes[2][0] && i != r.start_end_routes[2][1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con gli indici e il nodo di connessione nel nodo: " << i << endl;
			cout << " Route Vecchia: " << endl;
			r_prima.Print();
			cout << " Route Riparata: " << endl;
			r.Print();
			system("pause");
			cin.get();
		}*/


	}


	if (r.start_end_routes[0][1] != -1 && r.start_end_routes[1][0] != -1) {
		if (r.start_end_routes[0][1] != r.start_end_routes[1][0]) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

	}
	if (r.start_end_routes[1][1] != -1 && r.start_end_routes[2][0] != -1) {
		if (r.start_end_routes[1][1] != r.start_end_routes[2][0]) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

	}

	//cout << "controllo start ed end day " << endl;

	if (r.start_end_routes[0][1] < -1 || r.start_end_routes[1][1] < -1 || r.start_end_routes[2][1] < -1) {
		cout << " stampo caso strano del primo passeggero " << endl;
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA gli indici di start ed end" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.Print();
		system("pause");
		cin.get();
	}



	//cout << "controllo controllo index " << endl;
	if (r.index > r.places.size()) {
		cout << " stampo caso strano del primo passeggero " << endl;
		cout << "passenger inserito: " << endl;
		p_inserito.print();
		cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON L'INDICE MASSIMO:" << endl;
		r.print();
		r.Print();
		cout << " la route prima era: " << endl;
		r_prima.Print();
		system("pause");
		cin.get();
	}

	//cout << "controllo i passenger " << endl;
	for (Passenger p : r.passengers_in_route) {
		if (r.places[p.solution_from] != p.departure_location || r.places[p.solution_to] != p.arrival_location) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO:" << endl;
			p.print();
			r.print();
			cout << "nella route: " << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

		if (p.solution_from >= p.solution_to) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PASSEGGERO, INVERSIONE DEL FROM E DEL TO:" << endl;
			p.print();
			r.print();
			cout << "nella route: " << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}

	}


	//cout << "controllo i tempi ed il fuel consumato " << endl;
	for (int i = 1; i < r.index; i++) {


		if (r.time_dep[i] < r.time_arr[i]) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' un problema con i tempi di partenza e arrivo: " << i << endl;
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}




		if (r.places[i - 1] != r.places[i]) {
			/*if (r.time_arr[i] <= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] - 1) || r.time_arr[i] >= (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1] + 1)) {
				cout << " stampo caso strano del primo passeggero " << endl;
				cout << "passenger inserito: " << endl;
				p_inserito.print();
				cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON i TEMPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII nell arco " << (i - 1) << endl;
				cout << "dovrebbe essere: " << (((from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed) * 60) + r.time_dep[i - 1]) << " mentre invece è " << r.time_arr[i] << endl;
				r.print();
				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}*/


			double fly_time = (from_to[r.places[i - 1]][r.places[i]] / map_airplane[r.aircraft_code].speed);
			double fuel_consumed = 0.0;
			if (fly_time <= 1) {
				fuel_consumed = fly_time * map_airplane[r.aircraft_code].fuel_burn_first;
			}
			else {
				fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (fly_time - 1)* map_airplane[r.aircraft_code].fuel_burn_second;
			}

			if (!r.refueling[i]) {
				if ((r.quantity_fuel[i - 1] - r.quantity_fuel[i]) <= (fuel_consumed - 1) || (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) >= (fuel_consumed + 1)) {
					cout << " stampo caso strano del primo passeggero " << endl;
					cout << "passenger inserito: " << endl;
					p_inserito.print();
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL FUEL nell arco DA " << (i - 1) << " A " << i << endl;
					cout << "dovrebbe essere: " << fuel_consumed << " mentre invece è " << (r.quantity_fuel[i - 1] - r.quantity_fuel[i]) << endl;
					r.print();
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
			}
		}





	}


	//cout << "controllo il fuel che cali " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		if (r.quantity_fuel[i] < r.quantity_fuel[i + 1] && !r.refueling[i + 1] || r.weight[i] < -0.005) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA O DEL FUEL CHE NON CALA O DEL PESO NEGATIVO" << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}


		/*if (r.places[i] == r.places[i + 1]) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DI PIu' POSTI UGUALI " << endl;
			r.print();
			system("pause");
			cin.get();
		}
		*/

	}







	for (int i = 0; i < r.index; i++) {



		//fatti mandare email ted





		/*
		if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con la capacità nel nodo: " << i << endl;
			r.Print();
			system("pause");
			cin.get();
		}
		*/


		//cout << "controllo i nodi di partenza " << endl;

		if (r.places[i] != 1 && r.places[i] != 4) {

			if (i == r.start_end_routes[0][0]) {
				if (r.time_dep[i] < start_day1) {
					cout << " stampo caso strano del primo passeggero " << endl;
					cout << "passenger inserito: " << endl;
					p_inserito.print();
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day1 << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();


				}
			}
			if (i == r.start_end_routes[1][0]) {
				if (r.time_dep[i] < start_day2) {
					cout << " stampo caso strano del primo passeggero " << endl;
					cout << "passenger inserito: " << endl;
					p_inserito.print();
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day2 << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();


				}

				/*if (r.time_arr[i] > end_day2) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day" << end_day2 << endl;
					r.print();
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}*/


			}
			if (i == r.start_end_routes[2][0]) {
				if (r.time_dep[i] < start_day3) {
					cout << " stampo caso strano del primo passeggero " << endl;
					cout << "passenger inserito: " << endl;
					p_inserito.print();
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via dello starting day" << start_day3 << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();

				}

				/*if (r.time_arr[i] > end_day3) {
					cout << "ATTENTOOOOOOOOOOOOOOOOO C'E'con il tempo nel nodo: " << i << "per via del end day " << end_day3 << endl;
					r.print();
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}*/


			}

		}


		//if (r.places[i] != "SEU") {
		/*if (r.places[i] != "ARK") {
			if (r.places[i] != "DAR") {
				if (r.time_arr[i] > end_day1 && r.time_arr[i] < start_day2) {
					cout << "caso 1: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day2 && r.time_arr[i] < start_day3) {
					cout << "caso2: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
				if (r.time_arr[i] > end_day3) {
					cout << "caso 3: ATTENTOOOOOOOOOOOOOOOOO C'E'un porblema con i tempi di fine giornata nel nodo: " << i << endl;
					r.Print();
					cout << " la route prima era: " << endl;
					r_prima.Print();
					system("pause");
					cin.get();
				}
			}
		}*/
		//}




		//cout << "controllo la capacita' ed il peso totale " << endl;

		int numero_pass = 0;
		double peso_arco_passeggeri = 0.0;
		for (Passenger p : r.passengers_in_route) {
			if (p.solution_from <= i && p.solution_to > i) {
				peso_arco_passeggeri += p.weight;
				numero_pass++;
			}
		}
		if (pi.solution_from <= i && pi.solution_to > i) {
			peso_arco_passeggeri += pi.weight;
			numero_pass++;
		}

		if (numero_pass != r.capacity[i]) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "numero passeggeri: " << numero_pass << endl;
			cout << "capacità: " << r.capacity[i] << endl;
			pi.print();
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO c'e un problema con la capacita' nel nodo: " << i << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}



		if ((r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) <= (map_airplane[r.aircraft_code].weight_fuel_people - 1) || (r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) >= (map_airplane[r.aircraft_code].weight_fuel_people + 1)) {
			cout << " stampo caso strano del primo passeggero " << endl;
			cout << "passenger inserito: " << endl;
			p_inserito.print();
			cout << "weight: " << r.weight[i] << endl;
			cout << "quantity fuel: " << r.quantity_fuel[i] << endl;
			cout << "peso passeggeri: " << peso_arco_passeggeri << endl;
			cout << "passenger che sto inserendo from: " << pi.solution_from << " passenger to: " << pi.solution_to << endl;
			cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' UN PROBLEMA CON IL PESO TOTALE NEL NODO " << i << " CON PESO TOTALE --> " + to_string(r.quantity_fuel[i] + r.weight[i] + peso_arco_passeggeri) << endl;
			r.print();
			r.Print();
			cout << " la route prima era: " << endl;
			r_prima.Print();
			system("pause");
			cin.get();
		}
	}


	//cout << "controllo il fuel non massimo " << endl;
	for (int i = 0; i < r.index - 1; i++) {
		double minimo_peso = r.weight[i];
		if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].max_fuel && r.refueling[i]) {
			for (int j = i + 1; j < r.index; j++) {
				if (r.refueling[j]) break;
				if (r.weight[j] < minimo_peso) minimo_peso = r.weight[j];
			}
			if (minimo_peso >= 1) {
				cout << " stampo caso strano del primo passeggero " << endl;
				cout << "passenger inserito: " << endl;
				p_inserito.print(); cout << "ATTENTOOOOOOOOOOOOOOOOO C'E' IL PROBLEMA DEL FUEL NON MASSIMO *************************" << endl;
				r.print();
				r.Print();
				cout << " la route prima era: " << endl;
				r_prima.Print();
				system("pause");
				cin.get();
			}

		}
	}













}

//***********************************************************************************************************************************************


//function of split
vector<string> split(string stringa, char separatore) {
	vector<string> words;
	stringstream ss(stringa);
	string individual_string;
	while (getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
}


map<string, int> fillLegenda(string file_input, map<int, string>& legenda_ritroso) {

	map<string, int> legenda;

	ifstream  file;
	file.open(file_input);
	//check for error
	if (file.fail()) {
		cerr << "Error Opening File legenda.csv" << endl;
		exit(1);
	}
	//read the file untill the end
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		legenda.insert(make_pair(e[0], stoi(e[1])));
		legenda_ritroso.insert(make_pair(stoi(e[1]), e[0]));
	}
	file.close();

	return legenda;
};



//function that read the file Airplane and create a vector of them
vector<Airplane> fillAirplane(string file_input, map<string, int> legenda) {

	vector<Airplane> airplanes;
	ifstream  file;
	file.open(file_input);
	//check for error
	if (file.fail()) {
		cerr << "Error Opening File Airplane.csv" << endl;
		exit(1);
	}
	//read the file untill the end
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		Airplane a(stoi(e[13]), e[1], stoi(e[6]), stod(e[5]), e[0], stoi(e[3]), stoi(e[4]), legenda[e[7]], stod(e[10]), stoi(e[8]), stoi(e[9]), stod(e[11]), stod(e[12]));
		airplanes.push_back(a);
	}
	file.close();

	return airplanes;
};


//function that read the file Airstrip and create a vector of them
vector<Airstrip> fillAirstrip(string file_input, map<string, int>& legenda) {

	vector<Airstrip> airstrips;
	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File Airstrip.csv" << endl;
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		bool fuel = false;
		if (e[6] == "1") fuel = true;
		Airstrip a(legenda[e[1]], e[0], e[1], stoi(e[2]), stoi(e[3]), stod(e[4]), stod(e[5]), fuel, stod(e[7]));
		airstrips.push_back(a);
	}
	file.close();

	return airstrips;
}

//function that read the file instance and create a vector of passenger
vector<Passenger> fillPassenger(string file_input, map<string, int>& legenda) {

	vector<Passenger> passengers;
	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File in fill passengers" << endl;
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[0] << " - ";
		//cout << e[9] << endl;
		if (e[9] != "Charter") {
			vector<string> dep_min = split(e[9], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[10], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(legenda[e[3]], legenda[e[4]], stoi(e[0]), e[1], e[3], e[4], e[5], e[6], e[7], e[8], min_dep, min_arr);
		p.pnr_group = e[11];

		passengers.push_back(p);

	}
	file.close();

	return passengers;
}

void fix_key_passenger_for_regret(vector<Passenger>& passengers) {

	int chiave = 1;
	map<string, int> string_key;

	for (Passenger& p : passengers) {
		string conc = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);

		if (string_key.count(conc) >= 1) {
			p.chiave_regret = string_key[conc];
		}
		else {
			p.chiave_regret = chiave;
			string_key.insert(make_pair(conc, chiave));
			chiave++;
		}
	}

}

map <int, Passenger> fillMapPassenger(vector<Passenger>& passengers) {
	map <int, Passenger> map_pass;
	for (Passenger p : passengers)
	{
		map_pass.insert(make_pair(p.pnr, p));
	}



	return  map_pass;
}

//function that read the file instance and create a vector of routes
vector<Route> fillRoute(string file_input) {

	vector<Route> routes;
	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File" << file_input << endl;
		exit(1);
	}
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<Passenger> Pass;
		vector<string> e = split(row, ';');
		Route r(0, Pass);
		r.aircraft_code_company_comparison = e[1];
		for (int i = 2; i < (int)(e.size() - 1); i = i + 2) {
			r.addPlace_companySolution(e[i], stoi(e[i + 1]));
		}
		routes.push_back(r);
	}
	file.close();

	return routes;
}





void fill_from_to_fuel_consumed(vector<vector<vector<double>>>& from_to_fuel_consumed, vector<vector<double>>& from_to, vector<Airplane> airplanes) {


	from_to_fuel_consumed.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++) {
		from_to_fuel_consumed[i].resize((size_t)numero_airstrip_const);
		for (int j = 0; j < numero_airstrip_const; j++) {
			from_to_fuel_consumed[i][j].resize((size_t)numero_airstrip_const);
		}
	}



	for (Airplane a : airplanes) {

		for (int i = 1; i < numero_airstrip_const; i++) {
			for (int j = 1; j < numero_airstrip_const; j++) {
				double time_fly = from_to[i][j] / a.speed;
				double fuel_consumed = 0.0;
				if (time_fly <= 1) {
					fuel_consumed = time_fly * a.fuel_burn_first;
				}
				else {
					fuel_consumed = a.fuel_burn_first + (time_fly - 1) * a.fuel_burn_second;
				}
				from_to_fuel_consumed[a.code][i][j] = fuel_consumed;
				//cout << " da/a: " << x.second << " time fly: " << time_fly << " fuel consumed: " << fuel_consumed << endl;
			}
		}




	}




}

map<string, double> fill_from_to_fuel_consumed_string(map<string, double>& from_to, vector<Airplane> airplanes) {

	map<string, double> from_to_fuel_consumed;

	for (Airplane a : airplanes) {
		for (auto x : from_to) {
			double time_fly = x.second / a.speed;
			double fuel_consumed = 0.0;
			if (time_fly <= 1) {
				fuel_consumed = time_fly * a.fuel_burn_first;
			}
			else {
				fuel_consumed = a.fuel_burn_first + (time_fly - 1) * a.fuel_burn_second;
			}
			from_to_fuel_consumed.insert(make_pair(a.code_company + ";" + x.first, fuel_consumed));
			//cout << " da/a: " << x.second << " time fly: " << time_fly << " fuel consumed: " << fuel_consumed << endl;
		}
	}
	return from_to_fuel_consumed;
}


map<string, double> fillMatrixCompany(string file_input, vector<Airstrip>& airstrips) {

	map<string, double> from_to;

	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File Matrix.csv" << endl;
		exit(1);
	}
	int index_row = 0;
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		for (int i = 0; i < (int)e.size(); i++) {
			istringstream is(e[i]);
			double d;
			is >> d;
			//cout << "String: " << e[i] << "\n";
			//cout << "Double: " << setprecision(14) << d << endl;
			from_to.insert(make_pair(airstrips[index_row].code_string + ";" + airstrips[i].code_string, d));
		}
		index_row++;
	}
	file.close();

	return from_to;
}

void fillMatrix(vector<vector<double>>& from_to, string file_input, vector<Airstrip>& airstrips) {


	from_to.resize((size_t)numero_airstrip_const);
	for (int i = 0; i < numero_airstrip_const; i++)
		from_to[i].resize((size_t)numero_airstrip_const);


	ifstream file;
	file.open(file_input);
	if (file.fail()) {
		cerr << "Error Opening File Matrix.csv" << endl;
		exit(1);
	}
	int index_row = 0;
	while (!file.eof()) {
		string row;
		getline(file, row);
		vector<string> e = split(row, ';');
		for (int i = 0; i < (int)e.size(); i++) {
			istringstream is(e[i]);
			double d;
			is >> d;
			from_to[airstrips[index_row].code][airstrips[i].code] = d;
			//from_to.insert(make_pair(airstrips[index_row].code + ";" + airstrips[i].code, d));
		}
		index_row++;
	}
	file.close();


}



//********************************************************FUNZIONI CHE CALCOLANO I COSTI*******************************************************************************
double calculationCostCompany_three_days(map<string, double>& from_to_FuelConsumed_company, double peso_trashipment, double peso_intermediate_stop, string route_azienza_day1, string route_azienza_day2, string route_azienza_day3, string passengers_azienda_day1, string passengers_azienda_day2, string passengers_azienda_day3, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers_day1, vector<Passenger> passengers_day2, vector<Passenger> passengers_day3, map<string, double>& from_to_company) {
	double check_fixed_cost = 0.0;

	//creo le routes per goni giornata
	vector<Route> routes_company_solution_day1 = fillRoute(route_azienza_day1);
	vector<Route> routes_company_solution_day2 = fillRoute(route_azienza_day2);
	vector<Route> routes_company_solution_day3 = fillRoute(route_azienza_day3);
	//creo la mappa COD_AEREO ---> route delle compagnia
	map <string, Route> codice_routeAzienda_day1;
	map <string, Route> codice_routeAzienda_day2;
	map <string, Route> codice_routeAzienda_day3;
	for (Route r : routes_company_solution_day1) {
		codice_routeAzienda_day1.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day2) {
		codice_routeAzienda_day2.insert(make_pair(r.aircraft_code_company_comparison, r));
		//r.printCompany();
	}
	for (Route r : routes_company_solution_day3) {
		codice_routeAzienda_day3.insert(make_pair(r.aircraft_code_company_comparison, r));
		//r.printCompany();
	}
	/*for (auto x : codice_routeAzienda_day3) {
		cout << x.first << endl;
		x.second.printCompany();
	}
	system("pause");*/

	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;


	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < airstrips.size(); i++) {
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
		//cout << "- " << airstrips[i].code_string << endl;
	}
	map<string, Airplane> airplanes_map;
	for (int i = 0; i < airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));

	}


	cout << "giorno 1: " << endl;
	double giorno1 = 0.0;
	for (int r = 0; r < routes_company_solution_day1.size(); r++) {

		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day1[r].places_company.size(); i++) {
			places += routes_company_solution_day1[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost; //aggiungo il leading cost
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day1[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];

				double time_flight = (from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].speed;

				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				cout << " + " << cost_fuel;
				c += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno1 += c;
	}
	cout << "costo --> " << giorno1 << endl;
	cout << "giorno 2: " << endl;
	double giorno2 = 0.0;
	for (int r = 0; r < routes_company_solution_day2.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day2[r].places_company.size(); i++) {
			places += routes_company_solution_day2[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost; //aggiungo il leading cost
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day2[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno2 += c;
	}
	cout << "costo --> " << giorno2 << endl;
	cout << "giorno 3: " << endl;
	double giorno3 = 0.0;
	for (int r = 0; r < routes_company_solution_day3.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day3[r].places_company.size(); i++) {
			places += routes_company_solution_day3[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost; //aggiungo il leading cost
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day3[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno3 += c;
	}
	cout << "costo --> " << giorno3 << endl;



	for (Route r1 : routes_company_solution_day1) {
		double c = airplanes_map[r1.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r1.aircraft_code_company_comparison].fixed_cost;

		for (Route r2 : routes_company_solution_day2) {
			if (r1.aircraft_code_company_comparison == r2.aircraft_code_company_comparison) {
				if (r1.places_company[r1.index - 1] != r2.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += from_to_FuelConsumed_company[r1.aircraft_code_company_comparison + ";" + r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += airstrips_map[r2.places_company[0]].landing_cost;
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r2 : routes_company_solution_day2) {
		double c = airplanes_map[r2.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;

		for (Route r3 : routes_company_solution_day3) {
			if (r2.aircraft_code_company_comparison == r3.aircraft_code_company_comparison) {
				if (r2.places_company[r2.index - 1] != r3.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------>" << from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------------->" << from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += airstrips_map[r3.places_company[0]].landing_cost;
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r3 : routes_company_solution_day3) {
		double c = airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
		check_fixed_cost += airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
	}
	//ricordati di aggiungere il costo fisso più quello dell'arco aggiuntivo
	//se uso un nuovo aereo metto il costo fisso se uso quello vecchio pago il tratto aggiuntivo se manca
	cout << "il costo fisso di uso degli aerei e' " << check_fixed_cost << endl;
	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing + costo fisso + eventuali archi aggiuntivi per unire gli aerei per la compagnia: " << costo_routing << endl;
	double costo_Soluzione = 0.0;


	//********************************************_day1*****************************************************************************************
	vector<Passenger> passengers_solution_day1; //il code_flight qui è l'aereo
	ifstream file_day1;
	//cout << passengers_azienda << endl;
	file_day1.open(passengers_azienda_day1);
	if (file_day1.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day1.eof()) {
		string row;
		getline(file_day1, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day1.push_back(p);

	}
	file_day1.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day1.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day1[p].name.begin(), passengers_day1[p].name.end(), passengers_day1[p].name.begin(), ::tolower);
		transform(passengers_day1[p].surname.begin(), passengers_day1[p].surname.end(), passengers_day1[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day1[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day1.size(); j++) {
			trovato_pass_name = passengers_solution_day1[j].name.find(passengers_day1[p].name);
			if (trovato_pass_name <= passengers_solution_day1[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day1[j].name.find(passengers_day1[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day1[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day1[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day1[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day1[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day1[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day1[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day1[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;


			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day1[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day1[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += c;
	}
	//**************************************************end_day1*********************************************************************************
	//********************************************_day2*****************************************************************************************
	vector<Passenger> passengers_solution_day2; //il code_flight qui è l'aereo
	ifstream file_day2;
	//cout << passengers_azienda << endl;
	file_day2.open(passengers_azienda_day2);
	if (file_day2.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day2.eof()) {
		string row;
		getline(file_day2, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day2.push_back(p);

	}
	file_day2.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day2.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day2[p].name.begin(), passengers_day2[p].name.end(), passengers_day2[p].name.begin(), ::tolower);
		transform(passengers_day2[p].surname.begin(), passengers_day2[p].surname.end(), passengers_day2[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day2[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day2.size(); j++) {
			trovato_pass_name = passengers_solution_day2[j].name.find(passengers_day2[p].name);
			if (trovato_pass_name <= passengers_solution_day2[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day2[j].name.find(passengers_day2[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day2[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day2[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day2[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day2[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day2[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day2[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day2[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;


			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day2[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day2[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += c;
	}
	//**************************************************end_day2*********************************************************************************
	//********************************************_day3*****************************************************************************************
	vector<Passenger> passengers_solution_day3; //il code_flight qui è l'aereo
	ifstream file_day3;
	//cout << passengers_azienda << endl;
	file_day3.open(passengers_azienda_day3);
	if (file_day3.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day3.eof()) {
		string row;
		getline(file_day3, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day3.push_back(p);

	}
	file_day3.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day3.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day3[p].name.begin(), passengers_day3[p].name.end(), passengers_day3[p].name.begin(), ::tolower);
		transform(passengers_day3[p].surname.begin(), passengers_day3[p].surname.end(), passengers_day3[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day3[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day3.size(); j++) {
			trovato_pass_name = passengers_solution_day3[j].name.find(passengers_day3[p].name);
			if (trovato_pass_name <= passengers_solution_day3[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day3[j].name.find(passengers_day3[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day3[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day3[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day3[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day3[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day3[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day3[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day3[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;


			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day3[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day3[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += c;
	}
	//**************************************************end_day3*********************************************************************************





	costo_Soluzione += costi_time_windows + costi_intermediate_stop;
	for (int i = 0; i < cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
		//cout << "->" << cost_route[i] << endl;
	}





	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;

	return costo_Soluzione;


};

double calculationCostCompany_three_days_disaggregato(double peso_TW, map<string, double>& from_to_FuelConsumed_company, double peso_trashipment, double peso_intermediate_stop, string route_azienza_day1, string route_azienza_day2, string route_azienza_day3, string passengers_azienda_day1, string passengers_azienda_day2, string passengers_azienda_day3, vector<Airstrip> airstrips, vector<Airplane> airplanes, vector<Passenger> passengers_day1, vector<Passenger> passengers_day2, vector<Passenger> passengers_day3, map<string, double>& from_to_company) {
	double check_fixed_cost = 0.0;



	//variabili per la stampa disaggregata dei costi
	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;
	double COSTO_transhipment = 0.0;



	//creo le routes per goni giornata
	vector<Route> routes_company_solution_day1 = fillRoute(route_azienza_day1);
	vector<Route> routes_company_solution_day2 = fillRoute(route_azienza_day2);
	vector<Route> routes_company_solution_day3 = fillRoute(route_azienza_day3);
	//for (Route r : routes_company_solution) cout << "codice volo: " << r.aircraft_code << endl;
	//system("pause");
	//creo la mappa COD_AEREO ---> route delle compagnia
	map <string, Route> codice_routeAzienda_day1;
	map <string, Route> codice_routeAzienda_day2;
	map <string, Route> codice_routeAzienda_day3;
	for (Route r : routes_company_solution_day1) {
		codice_routeAzienda_day1.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day2) {
		codice_routeAzienda_day2.insert(make_pair(r.aircraft_code_company_comparison, r));
	}
	for (Route r : routes_company_solution_day3) {
		codice_routeAzienda_day3.insert(make_pair(r.aircraft_code_company_comparison, r));
	}

	/*for (auto x : codice_routeAzienda_day3) {
		cout << x.first << endl;
		x.second.printCompany();
	}
	system("pause");*/

	int costi_time_windows = 0;
	double costi_intermediate_stop = 0.0;


	vector<double> cost_route;
	//build an hashmap of airstip where the key is the code of the airstip
	map<string, Airstrip> airstrips_map;
	for (int i = 0; i < airstrips.size(); i++) {
		airstrips_map.insert(make_pair(airstrips[i].code_string, airstrips[i]));
		//cout << "- " << airstrips[i].code_string << endl;
	}
	map<string, Airplane> airplanes_map;
	for (int i = 0; i < airplanes.size(); i++) {
		airplanes_map.insert(make_pair(airplanes[i].code_company, airplanes[i]));
		//cout << "- " << airplanes[i].code_company << endl;
	}
	//system("pause");





	cout << "giorno 1: " << endl;
	double giorno1 = 0.0;
	for (int r = 0; r < routes_company_solution_day1.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day1[r].places_company.size(); i++) {
			places += routes_company_solution_day1[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day1[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day1[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day1[r].places_company[i] + ";" + routes_company_solution_day1[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day1[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno1 += c;
	}
	cout << "costo --> " << giorno1 << endl;
	cout << "giorno 2: " << endl;
	double giorno2 = 0.0;
	for (int r = 0; r < routes_company_solution_day2.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day2[r].places_company.size(); i++) {
			places += routes_company_solution_day2[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day2[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day2[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day2[r].places_company[i] + ";" + routes_company_solution_day2[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day2[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno2 += c;
	}
	cout << "costo --> " << giorno2 << endl;
	cout << "giorno 3: " << endl;
	double giorno3 = 0.0;
	for (int r = 0; r < routes_company_solution_day3.size(); r++) {
		double c = 0.0;
		//airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost; //ora il costo fisso più l'eventuale arco aggiuntivo lo aggiungo al costo di routing alla fine
		//cout << airplanes_map[routes_company_solution[r].aircraft_code].fixed_cost ;
		string places = ";";
		for (int i = 0; i < routes_company_solution_day3[r].places_company.size(); i++) {
			places += routes_company_solution_day3[r].places_company[i] + ";";

			if (i >= 1) {
				//ho messo che parto da uno in modo tale da non considerare il leading cost del primo aereoporto
				//cout << " + " << airstrips_map[routes_company_solution[r].places[i]].landing_cost;
				c += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost; //aggiungo il leading cost
				COSTO_landing += airstrips_map[routes_company_solution_day3[r].places_company[i]].landing_cost;
			}

			//aggiungo il costo dei kilometri e del fuel
			if (i < routes_company_solution_day3[r].places_company.size() - 1) {
				//cout << routes_company_solution[r].places[i] << " ; " << routes_company_solution[r].places[i + 1] << endl;
				//cout << " + " << from_to[routes_company_solution[r].places[i] + ";" + routes_company_solution[r].places[i + 1]];
				c += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				COSTO_km += from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]];
				double time_flight = (from_to_company[routes_company_solution_day3[r].places_company[i] + ";" + routes_company_solution_day3[r].places_company[i + 1]]) / airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].speed;
				//cout << " time flight: " << time_flight;
				double cost_fuel = 0;
				if (time_flight <= 1) {
					cost_fuel = time_flight * airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first;
				}
				else {
					cost_fuel = airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_first + (time_flight - 1)*airplanes_map[routes_company_solution_day3[r].aircraft_code_company_comparison].fuel_burn_second;
				}
				//cout << " + " << cost_fuel;
				c += cost_fuel;
				COSTO_fuel += cost_fuel;
			}
		}
		//cout << endl;
		//cout << places << endl;


		//ora devo leggere i passeggeri_soluzione******************************************************************************************
		cost_route.push_back(c);
		giorno3 += c;
	}
	cout << "costo --> " << giorno3 << endl;



	for (Route r1 : routes_company_solution_day1) {
		double c = airplanes_map[r1.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r1.aircraft_code_company_comparison].fixed_cost;

		for (Route r2 : routes_company_solution_day2) {
			if (r1.aircraft_code_company_comparison == r2.aircraft_code_company_comparison) {
				if (r1.places_company[r1.index - 1] != r2.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += from_to_FuelConsumed_company[r1.aircraft_code_company_comparison + ";" + r1.places_company[r1.index - 1] + ";" + r2.places_company[0]];
					c += airstrips_map[r2.places_company[0]].landing_cost;
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r2 : routes_company_solution_day2) {
		double c = airplanes_map[r2.aircraft_code_company_comparison].fixed_cost; //il fixed cost devo aggiungerlo di tutti gli aerei del primo giorno
		check_fixed_cost += airplanes_map[r2.aircraft_code_company_comparison].fixed_cost;

		for (Route r3 : routes_company_solution_day3) {
			if (r2.aircraft_code_company_comparison == r3.aircraft_code_company_comparison) {
				if (r2.places_company[r2.index - 1] != r3.places_company[0]) {
					//qua devo aggiungere i km ed il fuel dell'arco che devo aggiungere e il landing cost del primo aeroporto della seconda rotta
					c += from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------>" << from_to_company[r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]];
					//cout << "------------------->" << from_to_FuelConsumed_company[r2.aircraft_code_company_comparison + ";" + r2.places_company[r2.index - 1] + ";" + r3.places_company[0]] << endl;
					c += airstrips_map[r3.places_company[0]].landing_cost;
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
				else {
					//qua vuol dire che sono uguali i due posti quindi non devo aggiungere il costo dei due archi
					c -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
					check_fixed_cost -= airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
				}
			}
		}

		cost_route.push_back(c);
	}
	for (Route r3 : routes_company_solution_day3) {
		double c = airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
		check_fixed_cost += airplanes_map[r3.aircraft_code_company_comparison].fixed_cost;
	}
	//ricordati di aggiungere il costo fisso più quello dell'arco aggiuntivo
	//se uso un nuovo aereo metto il costo fisso se uso quello vecchio pago il tratto aggiuntivo se manca
	cout << "il costo fisso di uso degli aerei e' " << check_fixed_cost << endl;
	COSTO_fisso += check_fixed_cost;
	double costo_routing = 0.0;
	for (double c : cost_route) costo_routing += c;
	cout << "Costo Routing + costo fisso + eventuali archi aggiuntivi per unire gli aerei per la compagnia: " << costo_routing << endl;
	double costo_Soluzione = 0.0;


	//********************************************_day1*****************************************************************************************
	vector<Passenger> passengers_solution_day1; //il code_flight qui è l'aereo
	ifstream file_day1;
	//cout << passengers_azienda << endl;
	file_day1.open(passengers_azienda_day1);
	if (file_day1.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day1.eof()) {
		string row;
		getline(file_day1, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day1.push_back(p);

	}
	file_day1.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day1.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day1[p].name.begin(), passengers_day1[p].name.end(), passengers_day1[p].name.begin(), ::tolower);
		transform(passengers_day1[p].surname.begin(), passengers_day1[p].surname.end(), passengers_day1[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day1[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day1.size(); j++) {
			trovato_pass_name = passengers_solution_day1[j].name.find(passengers_day1[p].name);
			if (trovato_pass_name <= passengers_solution_day1[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day1[j].name.find(passengers_day1[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day1[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day1[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day1[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day1[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day1[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day1[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day1[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == passengers_day1[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day1[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day1[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day1[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day1[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day1[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c* peso_TW);
	}
	//**************************************************end_day1*********************************************************************************
	//********************************************_day2*****************************************************************************************
	vector<Passenger> passengers_solution_day2; //il code_flight qui è l'aereo
	ifstream file_day2;
	//cout << passengers_azienda << endl;
	file_day2.open(passengers_azienda_day2);
	if (file_day2.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day2.eof()) {
		string row;
		getline(file_day2, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day2.push_back(p);

	}
	file_day2.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day2.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day2[p].name.begin(), passengers_day2[p].name.end(), passengers_day2[p].name.begin(), ::tolower);
		transform(passengers_day2[p].surname.begin(), passengers_day2[p].surname.end(), passengers_day2[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day2[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day2.size(); j++) {
			trovato_pass_name = passengers_solution_day2[j].name.find(passengers_day2[p].name);
			if (trovato_pass_name <= passengers_solution_day2[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day2[j].name.find(passengers_day2[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day2[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day2[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day2[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day2[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day2[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day2[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day2[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == passengers_day2[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day2[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day2[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day2[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day2[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day2[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c*peso_TW);
	}
	//**************************************************end_day2*********************************************************************************
	//********************************************_day3*****************************************************************************************
	vector<Passenger> passengers_solution_day3; //il code_flight qui è l'aereo
	ifstream file_day3;
	//cout << passengers_azienda << endl;
	file_day3.open(passengers_azienda_day3);
	if (file_day3.fail()) {
		cerr << "Error Opening File passenger azienda day 1" << endl;
		system("pause");
		exit(1);
	}
	while (!file_day3.eof()) {
		string row;
		getline(file_day3, row);
		vector<string> e = split(row, ';');
		int min_dep;
		int min_arr;
		//cout << e[1] << endl;
		if (e[2] != "Charter") {
			vector<string> dep_min = split(e[4], ':');
			min_dep = (stoi(dep_min[0]) * 60) + stoi(dep_min[1]);
			vector<string> arr_min = split(e[5], ':');
			min_arr = (stoi(arr_min[0]) * 60) + stoi(arr_min[1]);
		}
		else {
			min_arr = 0;
			min_dep = 0;
		}
		Passenger p(0, 0, 0, e[0], e[2], e[3], "", e[1], "", "", min_dep, min_arr);
		passengers_solution_day3.push_back(p);

	}
	file_day3.close();
	//calcolo matrice A e costo della penalità per essere fuori dall'orario previsto
	for (int p = 0; p < passengers_day3.size(); p++) {
		int c = 0; //costo_time_windows
		size_t trovato_pass_name;
		size_t trovato_pass_cognome;
		int controllo = 0;
		vector<Passenger> pass_trovato;
		transform(passengers_day3[p].name.begin(), passengers_day3[p].name.end(), passengers_day3[p].name.begin(), ::tolower);
		transform(passengers_day3[p].surname.begin(), passengers_day3[p].surname.end(), passengers_day3[p].surname.begin(), ::tolower);
		//cout << "- " << passengers[p].name << endl;

		//cout << " Dato il passeggiero: " << endl;
		//passengers_day3[p].print();
		//cout << endl;
		//cout << " Cerco corrispondenze ...... " << endl;

		for (int j = 0; j < passengers_solution_day3.size(); j++) {
			trovato_pass_name = passengers_solution_day3[j].name.find(passengers_day3[p].name);
			if (trovato_pass_name <= passengers_solution_day3[j].name.size()) {
				trovato_pass_cognome = passengers_solution_day3[j].name.find(passengers_day3[p].surname);
				if (trovato_pass_cognome <= passengers_solution_day3[j].name.size()) {
					pass_trovato.push_back(passengers_solution_day3[j]);
					controllo += 1;
					//cout << " Trovato corrispondenza: " << endl;
					//passengers_solution_day3[j].print();
				}
			}

		}
		//system("pause");
		//cout << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

		//questa è la parte del calcolo delle time window
		if (controllo >= 1) {
			//vuol dire che ho trovato il passeggero
			//controllo se trovo l'andata
			for (int z = 0; z < pass_trovato.size(); z++) {
				if (pass_trovato[z].departure_location_company == passengers_day3[p].departure_location_company) {
					//calcolo la time windows
					int differenza_dep = passengers_day3[p].departure_time - pass_trovato[z].departure_time;
					if (differenza_dep > 25) {

						//cout << " + " << passengers[p].departure_time - routes[r].time[index_dep] - 25;
						c += differenza_dep - 25;
					}

					if (differenza_dep < -5) {
						//cout << " + " << (passengers[p].departure_time - routes[r].time[index_dep] + 5)*(-1);
						c += (differenza_dep + 5)*(-1);
					}
					//cout << "- time: " << c << endl;
				}
				if (pass_trovato[z].arrival_location_company == passengers_day3[p].arrival_location_company) {
					//calcolo la time windows
					int differenza_arr = passengers_day3[p].arrival_time - pass_trovato[z].arrival_time;
					if (differenza_arr > 30) {
						//cout << " + " << passengers[p].arrival_time - routes[r].time[index_arr] - 30;
						c += differenza_arr - 30;
					}
					if (differenza_arr < 0) {
						//cout << " + " << (passengers[p].arrival_time - routes[r].time[index_arr])*(-1);
						c += (differenza_arr)*(-1);
					}

					//cout << "- time: " << c << endl;
				}
			}
		}

		//parte per gli intermediate stop

		//parte per il calcolo del costo degli intermediate stop*********************************************************************************
		if (controllo == 1) {
			vector<int> int_from; //vettore con tutti i from
			vector<int> int_to; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].departure_location_company) {
					int_from.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == passengers_day3[p].arrival_location_company) {
					int_to.push_back(i);
				}
			}

			if (!int_from.empty() && !int_to.empty()) {
				//for (int f : int_from) cout << "f: " << f << endl;
				//for (int t : int_to) cout << "t: " << t << endl;

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1));
			}
			else if (int_from.empty() && !int_to.empty()) {
				//qua ha fatto un transhipment sul from
				//devo cercare tutti i from partendo dal from sulla soluzione
				vector<int> int_from_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
						int_from_soluz.push_back(i);
					}
				}
				if (int_from_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul from ma non trova il from della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from_soluz) {
					for (int to : int_to) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;
			}
			else if (!int_from.empty() && int_to.empty()) {
				//qua ha fatto un transhipment sul to
				//devo cercare tutti i to partendo dal from sulla soluzione
				vector<int> int_to_soluz;
				for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
					if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
						int_to_soluz.push_back(i);
					}
				}
				if (int_to_soluz.empty()) {
					cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  qua fa il trashipment sul to ma non trova il to della soluzione" << endl;
					system("pause");
				}

				double best_differenza = DBL_MAX;
				int best_from = -1;
				int best_to = -1;
				for (int from : int_from) {
					for (int to : int_to_soluz) {
						if (to > from) {
							double diff = to - from;
							if (diff < best_differenza) {
								best_differenza = diff;
								best_from = from;
								best_to = to;
							}
						}
					}
				}
				//cout << "la differenza è: " << best_differenza << endl;
				//cout << "il miglior from è: " << best_from << endl;
				//cout << "il miglior to è: " << best_to << endl;
				costi_intermediate_stop += (peso_intermediate_stop*(best_to - best_from - 1)) + peso_trashipment;
				COSTO_transhipment += peso_trashipment;

			}
			else {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  c'è un problema con il passeggero: ";
				passengers_day3[p].print();
				cout << "in quanto la sua soluzione non trova from e to nel volo" << endl;
				system("pause");
			}
		}
		else if (controllo == 2) {

			costi_intermediate_stop += peso_trashipment;
			COSTO_transhipment += peso_trashipment;

			//*********************************primo passeggero trovato**********************************************************************************************
			vector<int> int_from1; //vettore con tutti i from
			vector<int> int_to1; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[0].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda_day3[pass_trovato[0].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].departure_location_company) {
					int_from1.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[0].code_flight].places_company[i] == pass_trovato[0].arrival_location_company) {
					int_to1.push_back(i);
				}
			}

			if (int_from1.empty() || int_to1.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[0].print();
			}
			double best_differenza1 = DBL_MAX;
			int best_from1 = -1;
			int best_to1 = -1;
			for (int from : int_from1) {
				for (int to : int_to1) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza1) {
							best_differenza1 = diff;
							best_from1 = from;
							best_to1 = to;
						}
					}
				}
			}
			//cout << "la differenza è: " << best_differenza1 << endl;
			//cout << "il miglior from è: " << best_from1 << endl;
			//cout << "il miglior to è: " << best_to1 << endl;
			costi_intermediate_stop += (peso_intermediate_stop*(best_to1 - best_from1 - 1));
			//****************************************************************************************************************************************************************

			//*********************************secondo pezzo del passeggero trovato**********************************************************************************************
			vector<int> int_from2; //vettore con tutti i from
			vector<int> int_to2; // vettore con tutti i to 
			//scorro tutte le località della route
			for (int i = 0; i < codice_routeAzienda_day3[pass_trovato[1].code_flight].index; i++) {
				//salvo tutti i from e tutti i to che trovo
				//cout << codice_routeAzienda[pass_trovato[1].code_flight].places[i] << " - " << endl;
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].departure_location_company) {
					int_from2.push_back(i);
				}
				if (codice_routeAzienda_day3[pass_trovato[1].code_flight].places_company[i] == pass_trovato[1].arrival_location_company) {
					int_to2.push_back(i);
				}
			}

			if (int_from2.empty() || int_to2.empty()) {
				cout << "ATTENDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  problema nel caso controllo == 2 con il passeggero" << endl;
				pass_trovato[1].print();
				system("pause");
			}
			double best_differenza2 = DBL_MAX;
			int best_from2 = -1;
			int best_to2 = -1;
			for (int from : int_from2) {
				for (int to : int_to2) {
					if (to > from) {
						double diff = to - from;
						if (diff < best_differenza2) {
							best_differenza2 = diff;
							best_from2 = from;
							best_to2 = to;
						}
					}
				}
			}

			//cout << "la differenza è: " << best_differenza2 << endl;
			//cout << "il miglior from è: " << best_from2 << endl;
			//cout << "il miglior to è: " << best_to2 << endl;

			costi_intermediate_stop += (peso_intermediate_stop*(best_to2 - best_from2 - 1));
			//****************************************************************************************************************************************************************









		}
		else if (controllo > 2) {
			cout << "c'è un problema con il passeggero: ";
			passengers_day3[p].print();
			cout << "in quanto trova più di due corrispondenze nelle soluzioni" << endl;
			system("pause");
		}
		//*****************************************************************************************************************************************



		costi_time_windows += (c*peso_TW);
	}
	//**************************************************end_day3*********************************************************************************





	costo_Soluzione += costi_time_windows + costi_intermediate_stop;
	COSTO_tw += costi_time_windows;
	COSTO_intermediate += costi_intermediate_stop;
	COSTO_intermediate = COSTO_intermediate - COSTO_transhipment;
	for (int i = 0; i < cost_route.size(); i++) {
		costo_Soluzione += cost_route[i];
		//cout << "->" << cost_route[i] << endl;
	}





	//cout << "the cost of the company solution is:   " << costo_Soluzione << endl;
	//cout << endl;
	cout << "costo delle time windows per la compagnia: " << costi_time_windows << endl;
	cout << "costo delle soste intermedie e del transhipment per la compagnia: " << costi_intermediate_stop << endl;


	//******************************************************************************DEVI METTERE QUA LA STAMPA DISAGGREGATA******************************************************************************
	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;n° transhipment;costo_totale" << endl;
	cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / airplanes_map[airplanes[0].code_company].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / peso_TW) << ";" << (COSTO_transhipment / peso_trashipment) << ";" << costo_Soluzione << ";";




	return costo_Soluzione;


};



double calculate_ObjectiveFunction(double peso_intermediate_stop, vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		double cost_route = 0.0;
		//cout << "Route-----" << endl;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass[0] || r.primo_pass[1] || r.primo_pass[2]) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
		}

		//cout << "costo fisso: " << cost << endl;

		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				cost_route += map_airstrip[r.places[i]].landing_cost;

			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}

		/*for (int i = 1; i < r.index; i++) {
			cost += map_airstrip[r.places[i]].landing_cost;
			cost_route += map_airstrip[r.places[i]].landing_cost;
		}
		//cout << "costo landing: " << cost << endl;
		//second of all calculate the cost of the Km and the fuel burn
		for (int i = 0; i < r.index - 1; i++) {
			//double time_travel = 0.0;
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i] + ";" + r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code + ";" + r.places[i] + ";" + r.places[i + 1]];
			}


		}*/


		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;




		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_route += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop

			double TW_departure = 0.0;
			double TW_arrival = 0.0;


			if (p.giorno == 1) {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			else if (p.giorno == 2) {
				if (p.solution_from == r.start_end_routes[1][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}

			else if (p.giorno == 3) {
				if (p.solution_from == r.start_end_routes[2][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}



			cost += TW_departure + TW_arrival;
			cost_route += TW_departure + TW_arrival;
		}
		//cout << "costo timeW: " << cost << endl;
		//cout << "The cost: " << cost << endl;
		//cout << "- " << cost_route << endl;
		r.cost = cost_route;
	}

	return cost;
}


double calculate_ObjectiveFunction_disaggregato(double peso_intermediate_stop, vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;


	double COSTO_fisso = 0.0;
	double COSTO_landing = 0.0;
	double COSTO_fuel = 0.0;
	double COSTO_km = 0.0;
	double COSTO_intermediate = 0.0;
	double COSTO_tw = 0.0;





	for (auto& r : solution) {
		double cost_route = 0.0;
		//cout << "Route-----" << endl;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		if (r.primo_pass[0] || r.primo_pass[1] || r.primo_pass[2]) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
			cost_route += map_airplane[r.aircraft_code].fixed_cost;
			COSTO_fisso += map_airplane[r.aircraft_code].fixed_cost;
		}

		//cout << "costo fisso: " << cost << endl;

		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;


		for (int i = 0; i < r.index; i++) {

			if (i >= 1) {
				cost += map_airstrip[r.places[i]].landing_cost;
				cost_route += map_airstrip[r.places[i]].landing_cost;
				COSTO_landing += map_airstrip[r.places[i]].landing_cost;
			}
			if (i < r.index - 1) {
				if (i == 0 && r.capacity[i] == 0) {
					mileage = 0.0;
				}
				else {
					mileage += from_to[r.places[i]][r.places[i + 1]];
					//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
					fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
				}
			}
		}

		/*for (int i = 1; i < r.index; i++) {
			cost += map_airstrip[r.places[i]].landing_cost;
			cost_route += map_airstrip[r.places[i]].landing_cost;
		}
		//cout << "costo landing: " << cost << endl;
		//second of all calculate the cost of the Km and the fuel burn
		for (int i = 0; i < r.index - 1; i++) {
			//double time_travel = 0.0;
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i] + ";" + r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code + ";" + r.places[i] + ";" + r.places[i + 1]];
			}


		}*/


		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		cost_route += mileage;
		cost += fuel_consumed;
		cost_route += fuel_consumed;

		COSTO_km += mileage;
		COSTO_fuel += fuel_consumed;


		//now i have to calculate the penalitis regarding the time windows for each passeger
		for (auto p : r.passengers_in_route) {
			//double time_departure = r.time_arr[p.solution_from];
			//double time_arrival = r.time_arr[p.solution_to];
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			cost_route += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop
			COSTO_intermediate += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;

			double TW_departure = 0.0;
			double TW_arrival = 0.0;


			if (p.giorno == 1) {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			else if (p.giorno == 2) {
				if (p.solution_from == r.start_end_routes[1][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}

			else if (p.giorno == 3) {
				if (p.solution_from == r.start_end_routes[2][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}



			cost += TW_departure + TW_arrival;
			cost_route += TW_departure + TW_arrival;
			COSTO_tw += TW_departure + TW_arrival;

		}
		//cout << "costo timeW: " << cost << endl;
		//cout << "The cost: " << cost << endl;
		//cout << "- " << cost_route << endl;
		r.cost = cost_route;
	}


	cout << "costo fisso;costo landing;costo fuel; costo km;costo intermediate;costo tw; n° aerei;fuel;km;n° IS;min TW;costo_totale" << endl;
	cout << COSTO_fisso << ";" << COSTO_landing << ";" << COSTO_fuel << ";" << COSTO_km << ";" << COSTO_intermediate << ";" << COSTO_tw << ";" <<
		(COSTO_fisso / map_airplane[solution[1].aircraft_code].fixed_cost) << ";" << COSTO_fuel << ";" << COSTO_km << ";" << (COSTO_intermediate / peso_intermediate_stop) << ";" <<
		(COSTO_tw / 1) << ";" << cost << endl;





	return cost;
}





double cost_time_windows_for_route(const Route& r, map<int, Airstrip>& map_airstrip) {
	double cost = 0.0;

	//cout << "*********************************************************dentro alla funzione*********************************************************" << endl;
	//cout << "indice del vettore degli arrivi " << r.time_arr.size();



	for (const Passenger& p : r.passengers_in_route) {
		//cout << " Indice from: " << p.solution_from << endl;
		//cout << " Indice to: " << p.solution_to << endl;
		//r.print();

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];
		double TW_departure = 0.0;
		double TW_arrival = 0.0;

		//if (p.giorno == giorno) {
		if (p.giorno == 1) {
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		}
		else if (p.giorno == 2) {
			if (p.solution_from == r.start_end_routes[1][0]) {
				double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
				if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
				else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
			}
			else {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
			}

			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		}

		else if (p.giorno == 3) {
			if (p.solution_from == r.start_end_routes[2][0]) {
				double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
				if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
				else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
			}
			else {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
			}

			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		}
		//}




		cost += TW_departure + TW_arrival;
	}

	//cout << "*********************************************************fine dentro funzione*********************************************************" << endl;


	return cost;
}

double cost_time_windows_for_route_pezzo(const Route& r, int giorno, map<int, Airstrip>& map_airstrip) {
	double cost = 0.0;

	//cout << "*********************************************************dentro alla funzione*********************************************************" << endl;
	//cout << "indice del vettore degli arrivi " << r.time_arr.size();



	for (const Passenger& p : r.passengers_in_route) {
		//cout << " Indice from: " << p.solution_from << endl;
		//cout << " Indice to: " << p.solution_to << endl;
		//r.print();

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];
		double TW_departure = 0.0;
		double TW_arrival = 0.0;

		if (p.giorno == giorno) {
			if (p.giorno == 1) {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			else if (p.giorno == 2) {
				if (p.solution_from == r.start_end_routes[1][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}

			else if (p.giorno == 3) {
				if (p.solution_from == r.start_end_routes[2][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
		}




		cost += TW_departure + TW_arrival;
	}

	//cout << "*********************************************************fine dentro funzione*********************************************************" << endl;


	return cost;
}

double cost__for_route_passenger_destroyCluster(const Route& r, const Passenger& p, int fattore_inter_stop, map<int, Airstrip>& map_airstrip) {
	double cost = 0.0;

	//cout << "**************************************" << endl;
	//cout << "r.time_arr.size()" << r.time_arr.size();
	//p.print();

	//double time_departure = r.time_arr[p.solution_from];
	//double time_arrival = r.time_arr[p.solution_to];

	double TW_departure = 0.0;
	double TW_arrival = 0.0;
	if (p.giorno == 1) {
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
	}
	else if (p.giorno == 2) {
		if (p.solution_from == r.start_end_routes[1][0]) {
			double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
			if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
			else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
		}
		else {
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		}

		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
	}
	else if (p.giorno == 3) {
		if (p.solution_from == r.start_end_routes[2][0]) {
			double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
			if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
			else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
		}
		else {
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		}

		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
	}

	cost += TW_departure + TW_arrival + fattore_inter_stop * (p.solution_to - p.solution_from - 1);

	return cost;
}


double cost_time_windows_for_route_passenger(const Route& r, const Passenger& p, map<int, Airstrip>& map_airstrip) {
	double cost = 0.0;



	double TW_departure = 0.0;
	double TW_arrival = 0.0;

	//if (p.giorno == giorno) {
	if (p.giorno == 1) {
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
	}
	else if (p.giorno == 2) {
		if (p.solution_from == r.start_end_routes[1][0]) {
			double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
			if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
			else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
		}
		else {
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		}

		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
	}

	else if (p.giorno == 3) {
		if (p.solution_from == r.start_end_routes[2][0]) {
			double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
			if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
			else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
		}
		else {
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		}

		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
	}

	cost += TW_departure + TW_arrival;

	return cost;
}


bool relateness_passenger2(int trheshold, vector<vector<double>>& from_to, int idP, const Passenger& r, vector<Passenger>& passengers_removed, map<string, vector<int>>& agr_pass, vector<string>& sequenza, string pointer, int y, map<int, Passenger>& map_id_passenger) {
	//cout << " Sono dentro la funzione Relateness " << endl;
	//vector<string> Pass1 = split(idP, '/');
	// Poi questo for si potra togliere
	Passenger pass = map_id_passenger[idP];
	/*
	for (Passenger p : all_passenger) {
		//if (p.name == Pass1[0] && p.surname == Pass1[1] && p.pnr == Pass1[2] && p.code_flight == Pass1[3] && p.gender == Pass1[4] && p.arrival_location == Pass1[5] && p.departure_location == Pass1[6] && to_string(p.weight) == Pass1[7]) {
		if(p.pnr == idP){
			pass = p;
			break;
		}
	}

	cout << " Il passeggiero da A di confronto " << endl;
	pass.print();
	cout << " Il passeggiero da B di confronto " << endl;
	r.print();
	*/
	// Questo poi va cambiato
	if (pass.name.size() == 0) {
		cout << " ******************************* ERRORE ***********************************" << endl;
		cout << " Problema con il passeggiero con Pnr: " << idP << endl;
		cin.get();
	}
	double Relateness = from_to[r.arrival_location][pass.arrival_location] + from_to[r.departure_location][pass.departure_location] + abs(r.arrival_time - pass.arrival_time) + abs(r.departure_time - pass.departure_time);
	//cout <<" Relateness ---> " << Relateness << "  Soglia " << trheshold <<endl;
	if (Relateness <= trheshold) {
		/*cout << " Sono dentro Releatensess---> Valore : " << Relateness <<  endl;
		cout << " Sono dentro Releatensess---> Dato il passeggiero : " << endl;
		pass.print();
		cout << " Sono dentro Releatensess---> Devo togliere il passeggiero " << endl;
		r.print();
		cout << " e con lui: ";
		for (string j : agr_pass[pointer]) cout << j << " / " ;
		cout << endl;
		*/
		for (int j : agr_pass[pointer]) passengers_removed.push_back(map_id_passenger[j]);
		//cout << " Numero di passeggieri che sono stati messi nei rimossi RIGA 839 Main " << passengers_removed.size() << endl;
		//cout << " Devo eliminare sia da sequenza sia dalla mappa " << pointer << endl;
		sequenza.erase(sequenza.begin() + y);
		agr_pass.erase(agr_pass.find(pointer));
		//cout << " Sequenza dopo ha: " << sequenza.size() << endl;
		//cout << " Gruppi dopo ha: " << agr_pass.size() << endl;
		return true;
	}
	else {
		return false;
	}

}

double costo_senza_time_windows(const vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	double cost = 0.0;

	for (auto& r : solution) {
		//double cost_route = 0.0;
		//cout << "Route-----" << endl;
		//first of all calculate the fixed cost of use the aircraft and the landing cost;
		cost += map_airplane[r.aircraft_code].fixed_cost;
		//cost_route += map_airplane[r.aircraft_code].fixed_cost;
		//cout << "costo fisso: " << cost << endl;
		for (int i = 1; i < r.index; i++) {
			//if (r.places[i] != r.places[i - 1]) {
			cost += map_airstrip[r.places[i]].landing_cost;
			//cost_route += map_airstrip[r.places[i]].landing_cost;
		//}
		}
		//cout << "costo landing: " << cost << endl;



		//second of all calculate the cost of the Km and the fuel burn
		double mileage = 0.0;
		//double first_hour = 0.0;
		//double second_hour = 0.0;
		double fuel_consumed = 0.0;
		for (int i = 0; i < r.index - 1; i++) {
			//double time_travel = 0.0;
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}

			/*if (time_travel <= 1) {
				first_hour += time_travel;
			}
			else {
				first_hour += 1;
				second_hour += (time_travel - 1);
			}
			*/
		}
		//now i add the mileage and the fuel consumption to the objective function
		cost += mileage;
		//cost_route += mileage;
		//cout << "costo milege: " << cost << endl;
		cost += fuel_consumed;
		//cost_route += fuel_consumed;
		//cout << "costo fuel: " << cost << endl;
		//now i have to calculate the penalitis regarding the time windows for each passeger
	}

	return cost;


};

double costo_time_windows_and_intermediate_stop(double peso_intermediate_stop, const vector<Route>& solution, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to) {
	double cost = 0.0;

	for (auto& r : solution) {

		for (auto p : r.passengers_in_route) {
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop

			double TW_departure = 0.0;
			double TW_arrival = 0.0;


			if (p.giorno == 1) {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			else if (p.giorno == 2) {
				if (p.solution_from == r.start_end_routes[1][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			else if (p.giorno == 3) {
				if (p.solution_from == r.start_end_routes[2][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}

			cost += TW_departure + TW_arrival;
		}
	}

	return cost;
}





//*************************************************************************************COSTRUTTIVI************************************************************************
vector<Route> heuristic_costructive_second_fase(const vector<Route>& solution, double end_day1, double end_day2, double end_day3, map<int, Airstrip>& map_airstrip) {
	vector<Route> sol;

	for (const Route& r : solution) {
		//cout << "sto valutando la route: " << endl;
		//r.Print();


		Route best_route = r;
		Route r_support = best_route;
		double best_cost = cost_time_windows_for_route(best_route, map_airstrip);
		if (cost_time_windows_for_route(r, map_airstrip) != 0) { //chiaramente cerco di migliorare la dove le timewindows non sono zero
			//è ragionevole traslare in avanti la route del giorno in cui ci sono i passeggeri, altrimenti sarebbe solo una pedita di tempo computazionale
			if (r.primo_pass[0]) {
				for (int i = r.start_end_routes[0][0]; i < r.start_end_routes[0][1]; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
					double wait_time = 0;
					if (i == r.start_end_routes[0][0]) {
						do {
							for (int j = r.start_end_routes[0][0]; j <= r.start_end_routes[0][1]; j++) {
								r_support.time_arr[j] += 2;
								if (j != r.start_end_routes[0][1] || j == r.index - 1) r_support.time_dep[j] += 2;
							}
							if (r_support.time_arr[r.start_end_routes[0][1]] <= end_day1) {
								double cost_support = cost_time_windows_for_route(r_support, map_airstrip);
								//cout << "caso primo giorno " << cost_support << " ---- " << best_cost << endl;
								if (cost_support < best_cost) {
									best_route = r_support;
									best_cost = cost_support;
								}
							}
							wait_time += 2;  //prima c'era ++ per ottimizzare ho messo più due in modo da valutare ogni due minuti non ogni minuto
						} while (wait_time <= 90);
					}
					else {
						do {
							r_support.time_dep[i] += 2;
							for (int j = i + 1; j <= r.start_end_routes[0][1]; j++) {
								r_support.time_arr[j] += 2;
								if (j != r.start_end_routes[0][1] || j == r.index - 1) r_support.time_dep[j] += 2;
							}
							if (r_support.time_arr[r.start_end_routes[0][1]] <= end_day1) {
								double cost_support = cost_time_windows_for_route(r_support, map_airstrip);
								//cout << "caso primo giorno " << cost_support << " ---- " << best_cost << endl;
								if (cost_support < best_cost) {
									best_route = r_support;
									best_cost = cost_support;
								}
							}
							wait_time += 2;
						} while (wait_time <= 90);
					}
					r_support = best_route;
				}
			}

			if (r.primo_pass[1]) {
				for (int i = r.start_end_routes[1][0]; i < r.start_end_routes[1][1]; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
					double wait_time = 0;
					do {
						r_support.time_dep[i] += 2;
						for (int j = i + 1; j <= r.start_end_routes[1][1]; j++) {
							r_support.time_arr[j] += 2;
							if (j != r.start_end_routes[1][1] || j == r.index-1) r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r.start_end_routes[1][1]] <= end_day2) {
							double cost_support = cost_time_windows_for_route(r_support, map_airstrip);
							//cout << "caso secondo giorno " << cost_support << " ---- " << best_cost << endl;
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;
					} while (wait_time <= 90);
					r_support = best_route;
				}
			}

			if (r.primo_pass[2]) {
				for (int i = r.start_end_routes[2][0]; i < r.start_end_routes[2][1]; i++) {  //ATTENTO, SECONDO ME QUA CI VUOLE UN MENO 1 oppure no!!!! se ci sono dei problemi BUTTA L'OCCCHIO QUA
					double wait_time = 0;
					do {
						r_support.time_dep[i] += 2;
						for (int j = i + 1; j <= r.start_end_routes[2][1]; j++) {
							r_support.time_arr[j] += 2;
							//if (j != r.start_end_routes[2][1]) r_support.time_dep[j] += 2;
							r_support.time_dep[j] += 2;
						}
						if (r_support.time_arr[r.start_end_routes[2][1]] <= end_day3) {
							double cost_support = cost_time_windows_for_route(r_support, map_airstrip);
							//cout << "caso terzo giorno " << cost_support << " ---- " << best_cost << endl;
							if (cost_support < best_cost) {
								best_route = r_support;
								best_cost = cost_support;
							}
						}
						wait_time += 2;
					} while (wait_time <= 90);
					r_support = best_route;
				}
			}

		}



		//cout << "la route che sto inserendo nel pool:" << endl;
		//best_route.Print();
		//system("pause");
		sol.push_back(best_route);
	}
	return sol;
}

vector<Route> heuristic_costructive_first_fase(double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double start_day1, double end_day1, double start_day2, double end_day2, double start_day3, double end_day3, vector<Passenger>& passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		r.starting_locations.push_back(0);
		r.start_end_routes[0][0] = 0;
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);

	for (auto p : passengers) {
		//cout << "Sono il passeggero**********************" << endl;
		//p.print();
		string situation = "Error";
		int best_route = -1; //index of the best route where can i put my passenger
		int best_from = -1;
		int best_to = -1;
		double best_cost = DBL_MAX;
		int i = 0;
		//cout << "- sto calcolando dove metttere il passeggero: " << p.name << endl;
		//qui calcolo solo dove mettere il passeggero

		if (p.giorno == 1) {
			for (auto r : solution) {
				if (r.primo_pass[0] == false) {
					if (r.places[r.index - 1] == p.departure_location) {
						//in questo caso c'è solo lui nella route, il costo è dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location];
						//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
						//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = "A";
							}
						}
					}
					else {
						//qui c'è solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere più kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
						//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						cost += fuel_consumed;

						//per il check sul fuel
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = "B";
								}
							}
						}
					}
				}
				else {
					//nella route c'è almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilità della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'è il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = 0; t < r.index; t++) {
						if (r.places[t] == p.departure_location) FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++) {
							if (r.places[t] == p.arrival_location) TO.push_back(t);
						}

						if (TO.size() != 0) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= p.stop) {
										//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
										//se sono qua dentro il to è maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacità, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
											/*
											if (travel_time <= 1) {
												fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
											}
											else {
												fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
											}
											*/
											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
											if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

											//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
											if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

										}
										//se non soddisfa la capacità non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;





										//se sorpasso il break vuol dire che soddisfo la capacità, in questo caso il solo costo che considerò per l'aggiunta del passeggero qui
										//è la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

										double TW_departure = 0.0;
										//double t_arr_departure = r.time_arr[from];

										if (from == r.start_end_routes[p.giorno - 1][0]) {
											double time_new = r.time_dep[from] - map_airstrip[r.places[from]].ground_time;
											if (time_new < p.early_departure) TW_departure = p.early_departure - time_new;
											else if (time_new > p.late_departure) TW_departure = time_new - p.late_departure;

										}
										else {
											if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
											else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
										}


										double TW_arrival = 0.0;
										//double t_arr_arrival = r.time_arr[to];
										if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
										else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

										double cost = TW_departure + TW_arrival + ((to - from - 1)*peso_intermediate_stop);
										if (best_cost > cost) {
											best_cost = cost;
											best_route = i;
											situation = "D";
											best_from = from;
											best_to = to;
										}
									}
								}
							}
						}
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
						//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.places[r.index - 1] != p.arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										/*
										if (travel_time <= 1) {
											fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
										}
										else {
											fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
										}
										*/
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacità non guardo neanche e passo al TO successivo
								//cout << p.stop<< endl;
								if (capacity_satisfy && (r.index - from) <= p.stop) {

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									//if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
									//else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
									if (from == r.start_end_routes[p.giorno - 1][0]) {
										double time_new = r.time_dep[from] - map_airstrip[r.places[from]].ground_time;
										if (time_new < p.early_departure) TW_departure = p.early_departure - time_new;
										else if (time_new > p.late_departure) TW_departure = time_new - p.late_departure;

									}
									else {
										if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
										else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
									}

									double cost = TW_departure;
									// ora ci metto chilometri e fuel dell'ultimo pezzo;
									cost += from_to[r.places[r.index - 1]][p.arrival_location];
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];
									/*
									if (travel_time <= 1) {
										fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
									}
									else {
										fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
									}
									*/
									cost += fuel_consumed;

									bool fuel_ok = true;
									if (!r.refueling[r.index - 1]) {
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
										if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
									}

									double TW_arrival = 0.0;
									double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
									else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
									cost += TW_arrival;

									cost += (peso_intermediate_stop * (r.index - from - 1));

									if (fuel_ok) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day1) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
													best_cost = cost;
													best_route = i;
													best_from = from;
													situation = "E";
												}

											}
										}
									}
								}
							}
						}

					}
					if (r.places[r.index - 1] != p.departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

						double TW_departure = 0.0;
						double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
						cost += TW_departure + TW_arrival;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}*/

						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}



						if (best_cost > cost) {
							if (t_arr_arrival <= end_day1) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
										best_cost = cost;
										best_route = i;
										situation = "C";
									}
								}
							}
						}

					}
				}
				i++;
			}
		}
		if (p.giorno == 2) {
			for (auto r : solution) {
				if (r.primo_pass[1] == false) {
					if (r.places[r.index - 1] == p.departure_location) {
						//in questo caso c'è solo lui nella route, il costo è dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = from_to[p.departure_location][p.arrival_location];
						if (!r.primo_pass[0]) cost += map_airplane[r.aircraft_code].fixed_cost; //metto questo in quanto se lo sto già usando il giorno prima allora non pago il costo fisso di utilizzo
																								//è un modo per incentivare l'uso di aerei già in "volo"

						//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
						//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = "H";
							}
						}
					}
					else {
						//qui c'è solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere più kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
						if (!r.primo_pass[0]) cost += map_airplane[r.aircraft_code].fixed_cost;


						//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						cost += fuel_consumed;

						//per il check sul fuel
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = "I";
								}
							}
						}
					}
				}
				else {
					//nella route c'è almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilità della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'è il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = r.starting_locations[r.starting_locations.size() - 1]; t < r.index; t++) {
						if (r.places[t] == p.departure_location) FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++) {
							if (r.places[t] == p.arrival_location) TO.push_back(t);
						}

						if (TO.size() != 0) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= p.stop) {
										//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
										//se sono qua dentro il to è maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacità, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
											/*
											if (travel_time <= 1) {
												fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
											}
											else {
												fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
											}
											*/
											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
											if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

											//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
											if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

										}
										//se non soddisfa la capacità non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;





										//se sorpasso il break vuol dire che soddisfo la capacità, in questo caso il solo costo che considerò per l'aggiunta del passeggero qui
										//è la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

										double TW_departure = 0.0;
										//double t_arr_departure = r.time_arr[from];
										if (from == r.start_end_routes[p.giorno - 1][0]) {
											double time_new = r.time_dep[from] - map_airstrip[r.places[from]].ground_time;
											if (time_new < p.early_departure) TW_departure = p.early_departure - time_new;
											else if (time_new > p.late_departure) TW_departure = time_new - p.late_departure;

										}
										else {
											if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
											else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
										}

										double TW_arrival = 0.0;
										//double t_arr_arrival = r.time_arr[to];
										if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
										else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

										double cost = TW_departure + TW_arrival + ((to - from - 1)*peso_intermediate_stop);
										if (best_cost > cost) {
											best_cost = cost;
											best_route = i;
											situation = "D";
											best_from = from;
											best_to = to;
										}
									}
								}
							}
						}
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
						//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.places[r.index - 1] != p.arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										/*
										if (travel_time <= 1) {
											fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
										}
										else {
											fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
										}
										*/
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacità non guardo neanche e passo al TO successivo
								//cout << p.stop<< endl;
								if (capacity_satisfy && (r.index - from) <= p.stop) {

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (from == r.start_end_routes[p.giorno - 1][0]) {
										double time_new = r.time_dep[from] - map_airstrip[r.places[from]].ground_time;
										if (time_new < p.early_departure) TW_departure = p.early_departure - time_new;
										else if (time_new > p.late_departure) TW_departure = time_new - p.late_departure;

									}
									else {
										if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
										else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
									}

									double cost = TW_departure;
									// ora ci metto chilometri e fuel dell'ultimo pezzo;
									cost += from_to[r.places[r.index - 1]][p.arrival_location];
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];
									/*
									if (travel_time <= 1) {
										fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
									}
									else {
										fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
									}
									*/
									cost += fuel_consumed;

									bool fuel_ok = true;
									if (!r.refueling[r.index - 1]) {
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
										if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
									}

									double TW_arrival = 0.0;
									double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
									else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
									cost += TW_arrival;

									cost += (peso_intermediate_stop * (r.index - from - 1));

									if (fuel_ok) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day2) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
													best_cost = cost;
													best_route = i;
													best_from = from;
													situation = "E";
												}

											}
										}
									}
								}
							}
						}

					}
					if (r.places[r.index - 1] != p.departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

						double TW_departure = 0.0;
						double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
						cost += TW_departure + TW_arrival;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}*/

						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}



						if (best_cost > cost) {
							if (t_arr_arrival <= end_day2) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
										best_cost = cost;
										best_route = i;
										situation = "C";
									}
								}
							}
						}

					}
				}
				i++;
			}
		}
		if (p.giorno == 3) {
			for (auto r : solution) {
				if (r.primo_pass[2] == false) {
					if (r.places[r.index - 1] == p.departure_location) {
						//in questo caso c'è solo lui nella route, il costo è dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = from_to[p.departure_location][p.arrival_location];
						if (!r.primo_pass[1]) cost += map_airplane[r.aircraft_code].fixed_cost; //metto questo in quanto se lo sto già usando il giorno prima allora non pago il costo fisso di utilizzo
																								//è un modo per incentivare l'uso di aerei già in "volo"

						//double travel_time = from_to[p.departure_location + ";" + p.arrival_location] / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];


						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;
						//cout << "if A: " << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
								best_cost = cost;
								best_route = i;
								situation = "Z";
							}
						}
					}
					else {
						//qui c'è solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere più kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = from_to[p.departure_location][p.arrival_location] + from_to[r.places[r.index - 1]][p.departure_location];
						if (!r.primo_pass[1]) cost += map_airplane[r.aircraft_code].fixed_cost;


						//double travel_time = (from_to[p.departure_location + ";" + p.arrival_location] + from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						cost += fuel_consumed;

						//per il check sul fuel
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						double travel_secondo_tratto = (from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "if di B " << best_cost << " >  " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + p.arrival_location]) << endl;
						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
									best_cost = cost;
									best_route = i;
									situation = "W";
								}
							}
						}
					}
				}
				else {
					//nella route c'è almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilità della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'è il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = r.starting_locations[r.starting_locations.size() - 1]; t < r.index; t++) {
						if (r.places[t] == p.departure_location) FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++) {
							if (r.places[t] == p.arrival_location) TO.push_back(t);
						}

						if (TO.size() != 0) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= p.stop) {
										//cout << "sono dentro al primo if e sono il passeggero: " << p.name << endl;
										//se sono qua dentro il to è maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacità, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
											/*
											if (travel_time <= 1) {
												fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
											}
											else {
												fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
											}
											*/
											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											//aggiunto questa parte per l'ultimo tratto che deve prevedere l'andare da almeno un'altra parte con il fuel QUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
											if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][p.arrival_location];

											//cout << "if dentro: " << !r.refueling[c] << " && " << r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) << " < " << 0 << endl;
											if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

										}
										//se non soddisfa la capacità non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;





										//se sorpasso il break vuol dire che soddisfo la capacità, in questo caso il solo costo che considerò per l'aggiunta del passeggero qui
										//è la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

										double TW_departure = 0.0;
										//double t_arr_departure = r.time_arr[from];
										if (from == r.start_end_routes[p.giorno - 1][0]) {
											double time_new = r.time_dep[from] - map_airstrip[r.places[from]].ground_time;
											if (time_new < p.early_departure) TW_departure = p.early_departure - time_new;
											else if (time_new > p.late_departure) TW_departure = time_new - p.late_departure;

										}
										else {
											if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
											else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
										}

										double TW_arrival = 0.0;
										//double t_arr_arrival = r.time_arr[to];
										if (r.time_arr[to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[to];
										else if (r.time_arr[to] > p.late_arrival) TW_arrival = r.time_arr[to] - p.late_arrival;

										double cost = TW_departure + TW_arrival + ((to - from - 1)*peso_intermediate_stop);
										if (best_cost > cost) {
											best_cost = cost;
											best_route = i;
											situation = "D";
											best_from = from;
											best_to = to;
										}
									}
								}
							}
						}
						//da qui in giu considero il caso in cui non ci sono TO ma anche se ci sono ma non mi conviene farlo fermare li 
						//quindi il for per tutti i from ma al di fuori della route e poi considero anche il costo di aggiungerlo a parte il pezzetto che manca
						if (r.places[r.index - 1] != p.arrival_location) {
							for (auto from : FROM) {
								bool capacity_satisfy = true;
								for (int c = from; c < r.index; c++) {
									if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										/*
										if (travel_time <= 1) {
											fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
										}
										else {
											fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
										}
										*/
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - p.weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacità non guardo neanche e passo al TO successivo
								//cout << p.stop<< endl;
								if (capacity_satisfy && (r.index - from) <= p.stop) {

									double TW_departure = 0.0;
									//double t_arr_departure = r.time_arr[from];
									if (from == r.start_end_routes[p.giorno - 1][0]) {
										double time_new = r.time_dep[from] - map_airstrip[r.places[from]].ground_time;
										if (time_new < p.early_departure) TW_departure = p.early_departure - time_new;
										else if (time_new > p.late_departure) TW_departure = time_new - p.late_departure;

									}
									else {
										if (r.time_arr[from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[from];
										else if (r.time_arr[from] > p.late_departure) TW_departure = r.time_arr[from] - p.late_departure;
									}

									double cost = TW_departure;
									// ora ci metto chilometri e fuel dell'ultimo pezzo;
									cost += from_to[r.places[r.index - 1]][p.arrival_location];
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.arrival_location];
									/*
									if (travel_time <= 1) {
										fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
									}
									else {
										fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
									}
									*/
									cost += fuel_consumed;

									bool fuel_ok = true;
									if (!r.refueling[r.index - 1]) {
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
										if (r.weight[r.index - 1] - p.weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
									}

									double TW_arrival = 0.0;
									double t_arr_arrival = r.time_arr[r.index - 1] + (((from_to[r.places[r.index - 1]][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
									else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
									cost += TW_arrival;

									cost += (peso_intermediate_stop * (r.index - from - 1));

									if (fuel_ok) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day3) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
													best_cost = cost;
													best_route = i;
													best_from = from;
													situation = "E";
												}

											}
										}
									}
								}
							}
						}

					}
					if (r.places[r.index - 1] != p.departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + p.departure_location] + from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location] + from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][p.departure_location] + from_to[p.departure_location][p.arrival_location];

						double TW_departure = 0.0;
						double t_arr_departure = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][p.departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_departure < p.early_departure) TW_departure = p.early_departure - t_arr_departure;
						else if (t_arr_departure > p.late_departure) TW_departure = t_arr_departure - p.late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[p.departure_location].ground_time + (((from_to[p.departure_location][p.arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < p.early_arrival) TW_arrival = p.early_arrival - t_arr_arrival;
						else if (t_arr_arrival > p.late_arrival) TW_arrival = t_arr_arrival - p.late_arrival;
						cost += TW_departure + TW_arrival;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + p.departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][p.departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[p.departure_location + ";" + p.arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][p.departure_location][p.arrival_location];
						/*if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}*/

						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[p.departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}



						if (best_cost > cost) {
							if (t_arr_arrival <= end_day3) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][p.arrival_location])) {
										best_cost = cost;
										best_route = i;
										situation = "C";
									}
								}
							}
						}

					}
				}
				i++;
			}
		}


		//cout << " caso : " << situation << " for the passeger:" << endl;
		//p.print();
		//ora devo aggiungere il passeggero nel posto migliore, quindi serve valutare in che caso sono A,B,C,D
		if (situation == "A") {
			solution[best_route].primo_pass[0] = true;
			//time
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto è la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1]++;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}
			//end for weight




			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);


			solution[best_route].start_end_routes[0][1] = solution[best_route].index - 1;

			//cout << "mossa A" << endl;
			//stampo_caso_strano_single_all_costruttivo(map_airstrip, solution[best_route], p, solution[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

		}
		else if (situation == "B") {
			solution[best_route].primo_pass[0] = true;
			solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 -
				map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, p.early_departure, p.early_departure + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = p.early_departure + map_airstrip[p.departure_location].ground_time + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;




			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[0][1] = solution[best_route].index - 1;

			//cout << "mossa B" << endl;
			//stampo_caso_strano_single_all_costruttivo(map_airstrip, solution[best_route], p, solution[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

		}
		else if (situation == "C") {

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 1,
					solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;


			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, solution[best_route].time_dep[solution[best_route].index - 1] +
					((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60),
					solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60) + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60);





			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}


			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[p.giorno - 1][1] = solution[best_route].index - 1;

			//cout << "mossa C" << endl;
			//stampo_caso_strano_single_all_costruttivo(map_airstrip, solution[best_route], p, solution[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


		}
		else if (situation == "D") {



			for (int h = best_from; h < best_to; h++) {
				solution[best_route].capacity[h]++;
				solution[best_route].weight[h] -= p.weight;
			}

			//**************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//**************************************************************************




			p.solution_from = best_from;
			p.solution_to = best_to;
			solution[best_route].passengers_in_route.push_back(p);


			//cout << "mossa D" << endl;
			//stampo_caso_strano_single_all_costruttivo(map_airstrip, solution[best_route], p, solution[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



		}
		else if (situation == "E") {


			//cout << "route prima" << endl;
			//solution[best_route].Print();


			for (int h = best_from; h < solution[best_route].index; h++) {
				solution[best_route].capacity[h]++;
				solution[best_route].weight[h] -= p.weight;


			}

			//******************************************************************************
			double add_fuel = 0;
			int index_weight_neg = -1;
			for (int j = best_from; j < solution[best_route].index; j++) {
				//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
				if (solution[best_route].weight[j] < 0) {
					//cout << " Weigth negativo in " << j << weight[j] << endl;
					add_fuel = solution[best_route].weight[j];
					index_weight_neg = j;
					int index_refueling = index_weight_neg;
					for (int i = index_weight_neg; i >= 0; i--) {
						if (solution[best_route].refueling[i]) {
							index_refueling = i;
							break;
						}
					}
					//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
					for (int t = index_refueling; t < solution[best_route].index; t++) {
						if (solution[best_route].refueling[t] && t != index_refueling) break;
						//cout << " Aggiorno nodo " << t << endl;
						solution[best_route].quantity_fuel[t] += add_fuel;
						solution[best_route].weight[t] -= add_fuel;
						//cout << " Weight dopo " << weight[t] << endl;
					}
				}
			}
			//******************************************************************************

			p.solution_from = best_from;
			int place_1 = solution[best_route].places[solution[best_route].index - 1];
			double aircraft_speed = map_airplane[solution[best_route].aircraft_code].speed;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + ((from_to[place_1][p.arrival_location] / aircraft_speed) * 60);

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;

			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/

				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;

			}

			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[p.giorno - 1][1] = solution[best_route].index - 1;

			//cout << "mossa E" << endl;
			//stampo_caso_strano_single_all_costruttivo(map_airstrip, solution[best_route], p, solution[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
		}
		else if (situation == "H") {

			solution[best_route].primo_pass[1] = true;
			solution[best_route].start_end_routes[1][0] = solution[best_route].index - 1;
			if (solution[best_route].primo_pass[0]) {
				solution[best_route].starting_locations.push_back(solution[best_route].index - 1);
				solution[best_route].start_end_routes[1][0] = solution[best_route].index - 1;
			}


			//time
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			//solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto è la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1]++;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}
			//end for weight




			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[1][1] = solution[best_route].index - 1;


		}
		else if (situation == "I") {

			solution[best_route].primo_pass[1] = true;
			solution[best_route].start_end_routes[1][0] = solution[best_route].index - 1;
			if (solution[best_route].primo_pass[0]) {
				solution[best_route].starting_locations.push_back(solution[best_route].index - 1);
				solution[best_route].start_end_routes[1][0] = solution[best_route].index - 1;
				//copio la partenza per il giorno sucessivo
			}

			//solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 - map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			double time_volo = (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_new = p.early_departure - time_volo;
			int location = solution[best_route].places[solution[best_route].index - 1];
			if (time_new < start_day2 && location != 1 && location != 4) time_new = start_day2;

			solution[best_route].time_dep[solution[best_route].index - 1] = time_new;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;




			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[1][1] = solution[best_route].index - 1;
		}
		else if (situation == "Z") {
			solution[best_route].primo_pass[2] = true;
			solution[best_route].start_end_routes[2][0] = solution[best_route].index - 1;
			if (solution[best_route].primo_pass[0] || solution[best_route].primo_pass[1] || (solution[best_route].primo_pass[0] && solution[best_route].primo_pass[1])) {
				solution[best_route].starting_locations.push_back(solution[best_route].index - 1);
				solution[best_route].start_end_routes[2][0] = solution[best_route].index - 1;
			}


			//time
			solution[best_route].time_dep[solution[best_route].index - 1] = p.early_departure + map_airstrip[p.departure_location].ground_time;
			//solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure; // in questo caso anche l'arrivo in quanto è la partenza dal depot

			p.solution_from = solution[best_route].index - 1;
			solution[best_route].capacity[solution[best_route].index - 1]++;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			if (solution[best_route].weight[solution[best_route].index - 1] < 0) {
				solution[best_route].quantity_fuel[solution[best_route].index - 1] += solution[best_route].weight[solution[best_route].index - 1];
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					solution[best_route].quantity_fuel[solution[best_route].index - 1] - p.weight;
			}
			//end for weight




			double travel_time = ((from_to[p.departure_location][p.arrival_location]) / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_from = solution[best_route].time_dep[solution[best_route].index - 1];
			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][p.departure_location][p.arrival_location];

			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people - map_airplane[solution[best_route].aircraft_code].max_fuel;
				//end for weight
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[p.arrival_location].ground_time);
				//for the weight
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + fuel_consumed + p.weight;
				//end for weight

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[2][1] = solution[best_route].index - 1;

		}
		else if (situation == "W") {

			solution[best_route].primo_pass[2] = true;
			solution[best_route].start_end_routes[2][0] = solution[best_route].index - 1;
			if (solution[best_route].primo_pass[0] || solution[best_route].primo_pass[1] || (solution[best_route].primo_pass[0] && solution[best_route].primo_pass[1])) {
				solution[best_route].starting_locations.push_back(solution[best_route].index - 1);
				//copio la partenza per il giorno sucessivo
				solution[best_route].start_end_routes[2][0] = solution[best_route].index - 1;
			}
			double time_volo = (from_to[solution[best_route].places[solution[best_route].index - 1]][p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
			double time_new = p.early_departure - time_volo;
			int location = solution[best_route].places[solution[best_route].index - 1];
			if (time_new < start_day3 && location != 1 && location != 4) time_new = start_day3;


			//solution[best_route].time_arr[solution[best_route].index - 1] = p.early_departure - (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60 - map_airstrip[solution[best_route].places[solution[best_route].index - 1]].ground_time;
			solution[best_route].time_dep[solution[best_route].index - 1] = time_new;

			//for the weight
			solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
				solution[best_route].quantity_fuel[solution[best_route].index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
			//end for weight

			double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.departure_location];

			if (map_airstrip[p.departure_location].fuel) {
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel - p.weight;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];

				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.departure_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;

				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.departure_location, map_airstrip[p.departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, time_new + time_volo, time_new + time_volo + map_airstrip[p.departure_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] - p.weight + fuel_consumed;

			}
			p.solution_from = solution[best_route].index - 1;
			double time = solution[best_route].time_dep[solution[best_route].index - 1] + (from_to[p.departure_location][p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;




			if (map_airstrip[p.arrival_location].fuel) {
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, map_airplane[solution[best_route].aircraft_code].max_fuel, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);

				solution[best_route].weight[solution[best_route].index - 1] = map_airplane[solution[best_route].aircraft_code].weight_fuel_people -
					map_airplane[solution[best_route].aircraft_code].max_fuel;
			}
			else {
				double fuel_before = solution[best_route].quantity_fuel[solution[best_route].index - 1];
				double fuel_consumed = from_to_FuelConsumed[solution[best_route].aircraft_code][solution[best_route].places[solution[best_route].index - 1]][p.arrival_location];
				//double travel_time = (from_to[solution[best_route].places[solution[best_route].index - 1] + ";" + p.arrival_location] / map_airplane[solution[best_route].aircraft_code].speed) * 60;
				/*
				if (travel_time <= 60) {
					fuel_consumed += (travel_time / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_first;
				}
				else {
					fuel_consumed += map_airplane[solution[best_route].aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[solution[best_route].aircraft_code].fuel_burn_second;
				}
				*/
				solution[best_route].addPlace(p.arrival_location, map_airstrip[p.arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[p.arrival_location].ground_time);
				solution[best_route].weight[solution[best_route].index - 1] = solution[best_route].weight[solution[best_route].index - 2] + p.weight + fuel_consumed;

			}
			p.solution_to = solution[best_route].index - 1;
			solution[best_route].passengers_in_route.push_back(p);
			solution[best_route].start_end_routes[2][1] = solution[best_route].index - 1;

		}
		else if (situation == "Error") {
			cout << "*******************************************************il passeggero: " << p.name << " non trova sistemazione" << endl;
			//p.print();
			//for (Route r : solution) r.print();
			//system("pause");
		}
	}


	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}


	return solution_clean;
}

//NON FUNZIONA PER IL ROLLING
vector<Route> heuristic_costructive_first_fase_sequential(double peso_intermediate_stop, vector<Airplane>& airplanes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double start_day, double end_day, vector<Passenger> passengers, int number_of_aircraft, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//creo i punti di partenza
	vector<Route> solution;
	int cont = 0;
	do {
		vector<Passenger> pass_in_solution;
		Route r(airplanes[cont].code, pass_in_solution);
		r.addPlace(airplanes[cont].depot, map_airstrip[airplanes[cont].depot].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 0, 0.0, 0.0);
		solution.push_back(r);
		cont++;
	} while (cont < number_of_aircraft);
	//now i have created only the possible route, now i that one of them and fill it untill the condition stop

	for (Route& r : solution) {
		//cout << "sto valutando la route *******************************************************************************************************************************************************************" << endl;
		string situation = "";
		do {
			situation = "Error"; //se situation rimane error vuol dire che nessun passeggero si può inserire, quindi che la route è piena e devo passare ad un'altra
			int best_passenger = -1;
			int best_from = -1;
			int best_to = -1;
			double best_cost = DBL_MAX;
			//cout << "sto valutando nella route " << endl;
			//r.print();
			//cout << "passengers.size()= " << passengers.size() << endl;
			for (int p = 0; p < passengers.size(); p++) {

				//cout << "sono il passeggero: " << endl;
				//passengers[p].print();

				if (r.primo_pass[0] == false) {
					if (r.places[r.index - 1] == passengers[p].departure_location) {
						//in questo caso c'è solo lui nella route, il costo è dato dalla sua inserzione, quindi, chilometri, costo fisso per uso aereo e fuel
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[passengers[p].departure_location][passengers[p].arrival_location];
						//double travel_time = from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location] / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];

						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						cost += fuel_consumed;
						//per il check sul fuel:
						double fuel_after_trip = 0.0;
						fuel_after_trip = r.quantity_fuel[r.index - 1] - fuel_consumed;

						//cout << "validità caso A" << endl;
						//cout << best_cost << " > " << cost << " && " << fuel_after_trip << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;

						if (best_cost > cost) {
							if (fuel_after_trip >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
								best_cost = cost;
								best_passenger = p;
								situation = "A";
								//cout << "situation A, cost: " << cost << "passenger: " << p << endl;
							}
						}
					}
					else {
						//qui c'è solo lui nell'aereo ma deve fare un pezzo vuoto all'inizio dal deposito alla partenza per il cliente, devo aggiungere più kilometri e un landing stop
						//non considero le time windows, faccio una partenza mirata per loro visto che sono i primi
						double cost = map_airplane[r.aircraft_code].fixed_cost + from_to[passengers[p].departure_location][passengers[p].arrival_location] + from_to[r.places[r.index - 1]][passengers[p].departure_location];
						//double travel_time = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location] + from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location] + from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						cost += fuel_consumed;

						//per il check sul fuel
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];

						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}

						//cout << "validità caso B" << endl;
						//cout << best_cost << " > " << cost << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;


						if (best_cost > cost) {
							if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
								if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
									best_cost = cost;
									best_passenger = p;
									situation = "B";
									//cout << "situation B, cost: " << cost << "passenger: " << p << endl;
								}
							}
						}
					}
				}
				else {
					//nella route c'è almeno un altro passeggero, devo fare le considerazioni riguardo alla fisibilità della route
					//prendo tutte le coppie FROM-TO che ci sono nella route, se non c'è il FROM, non cerco il TO
					vector<int> FROM;
					vector<int> TO;
					for (int t = 0; t < r.index; t++) {
						if (r.places[t] == passengers[p].departure_location) FROM.push_back(t);
					}

					if (FROM.size() != 0) {
						//dentro questo abbiamo trovato dei FROM 
						for (int t = FROM[0]; t < r.index; t++) {
							if (r.places[t] == passengers[p].arrival_location) TO.push_back(t);
						}

						if (TO.size() != 0) {
							for (auto from : FROM) {
								for (auto to : TO) {
									if (from < to && to - from <= passengers[p].stop) {
										//se sono qua dentro il to è maggiore del from e gli stop intermedi sono minori di tre
										//per prima cosa guardo se ci sto con la capacità, altrimenti break sul primo for
										bool capacity_satisfy = true;
										for (int c = from; c < to; c++) {
											if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

											//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
											double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
											/*
											if (travel_time <= 1) {
												fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
											}
											else {
												fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
											}
											*/
											double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

											if (c == to - 1) fuel_i_j += location_fuel[r.aircraft_code][passengers[p].arrival_location];

											if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;

										}
										//se non soddisfa la capacità non guardo neanche e passo al TO successivo
										if (!capacity_satisfy) break; //check also for the fuel;
										//cout << " ho passato la fesibility del caso D " << endl;




										//se sorpasso il break vuol dire che soddisfo la capacità, in questo caso il solo costo che considerò per l'aggiunta del passeggero qui
										//è la time windows, se calcolo la time windows qua vuol dire che sfrutto il tempo in cui mi trovo in queste posizioni

										double TW_departure = 0.0;
										double t_arr_departure = r.time_arr[from];
										if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
										else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

										double TW_arrival = 0.0;
										double t_arr_arrival = r.time_arr[to];
										if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
										else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;

										double cost = TW_departure + TW_arrival;
										cost += (peso_intermediate_stop*(to - from - 1));

										//cout << "if del caso D " << best_cost << " > " << cost << endl;
										if (best_cost > cost) {
											best_cost = cost;
											best_passenger = p;
											situation = "D";
											best_from = from;
											best_to = to;
											//cout << "situation D, cost: " << cost << "passenger: " << p << endl;
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
									if ((r.capacity[c] + 1) > map_airplane[r.aircraft_code].capacity) capacity_satisfy = false;

									if (c < r.index - 1) {
										//double travel_time = (from_to[r.places[c] + ";" + r.places[c + 1]]) / map_airplane[r.aircraft_code].speed;
										double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[c]][r.places[c + 1]];
										/*
										if (travel_time <= 1) {
											fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
										}
										else {
											fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
										}
										*/
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;

										if (!r.refueling[c] && r.weight[c] - passengers[p].weight + (r.quantity_fuel[c] - fuel_i_j) < 0) capacity_satisfy = false;
									}
								}
								//se non soddisfa la capacità non guardo neanche e passo al TO successivo
								//cout << p.stop<< endl;
								//cout << "feasibility caso E " << capacity_satisfy << " && " << (r.index - from) << " <= " << passengers[p].stop << endl;
								if (capacity_satisfy && (r.index - from) <= passengers[p].stop) {

									double TW_departure = 0.0;
									double t_arr_departure = r.time_arr[from];
									if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
									else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

									double cost = TW_departure;
									// ora ci metto chilometri e fuel dell'ultimo pezzo;
									cost += from_to[r.places[r.index - 1]][passengers[p].arrival_location];
									double travel_time = (from_to[r.places[r.index - 1]][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
									double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].arrival_location];
									/*
									if (travel_time <= 1) {
										fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
									}
									else {
										fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
									}
									*/
									cost += fuel_consumed;

									bool fuel_ok = true;
									if (!r.refueling[r.index - 1]) {
										double fuel_i_j = fuel_consumed + map_airplane[r.aircraft_code].min_fuel;
										if (r.weight[r.index - 1] - passengers[p].weight + (r.quantity_fuel[r.index - 1] - fuel_i_j) < 0) fuel_ok = false;
									}

									double TW_arrival = 0.0;
									double t_arr_arrival = 0.0;
									t_arr_arrival = r.time_arr[r.index - 1] + (travel_time * 60) + map_airstrip[r.places[r.index - 1]].ground_time;
									if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
									else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
									cost += TW_arrival;
									cost += peso_intermediate_stop * (r.index - from - 1);

									//cout << "if caso E" << fuel_ok << " && " << best_cost << " > " << cost << " && " << t_arr_arrival << " <= " << end_day << " && " << r.quantity_fuel[r.index - 1] - fuel_consumed << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;
									if (fuel_ok) {
										if (best_cost > cost) {
											if (t_arr_arrival <= end_day) {
												if (r.quantity_fuel[r.index - 1] - fuel_consumed >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
													best_cost = cost;
													best_passenger = p;
													best_from = from;
													situation = "E";
												}
											}
										}
										//cout << "situation E, cost: " << cost << "passenger: " << p << endl;
									}
								}
							}
						}
						//ho aggiunto questo if che c'è quaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
					}
					if (r.places[r.index - 1] != passengers[p].departure_location) {
						//guardo il costo di aggiungere un pezzo alla fine
						//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location] + from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location] + from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_time <= 1) {
							fuel_consumed = travel_time * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed = map_airplane[r.aircraft_code].fuel_burn_first + (travel_time - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						double cost = fuel_consumed + from_to[r.places[r.index - 1]][passengers[p].departure_location] + from_to[passengers[p].departure_location][passengers[p].arrival_location];

						double TW_departure = 0.0;
						//double t_arr_departure = r.time_arr[r.index - 1] + (map_airstrip[r.places[r.index - 1]].ground_time + (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60;
						double t_arr_departure = r.time_dep[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time + (((from_to[r.places[r.index - 1]][passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_departure < passengers[p].early_departure) TW_departure = passengers[p].early_departure - t_arr_departure;
						else if (t_arr_departure > passengers[p].late_departure) TW_departure = t_arr_departure - passengers[p].late_departure;

						double TW_arrival = 0.0;
						double t_arr_arrival = t_arr_departure + map_airstrip[passengers[p].departure_location].ground_time + (((from_to[passengers[p].departure_location][passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed) * 60);
						if (t_arr_arrival < passengers[p].early_arrival) TW_arrival = passengers[p].early_arrival - t_arr_arrival;
						else if (t_arr_arrival > passengers[p].late_arrival) TW_arrival = t_arr_arrival - passengers[p].late_arrival;
						cost += TW_departure + TW_arrival;

						//per il check sul fuel:
						//double travel_primo_tratto = (from_to[r.places[r.index - 1] + ";" + passengers[p].departure_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_primo_tratto = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[p].departure_location];
						/*
						if (travel_primo_tratto <= 1) {
							fuel_consumed_primo_tratto = travel_primo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_primo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_primo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//****
						//double travel_secondo_tratto = (from_to[passengers[p].departure_location + ";" + passengers[p].arrival_location]) / map_airplane[r.aircraft_code].speed;
						double fuel_consumed_secondo_tratto = from_to_FuelConsumed[r.aircraft_code][passengers[p].departure_location][passengers[p].arrival_location];
						/*
						if (travel_secondo_tratto <= 1) {
							fuel_consumed_secondo_tratto = travel_secondo_tratto * map_airplane[r.aircraft_code].fuel_burn_first;
						}
						else {
							fuel_consumed_secondo_tratto = map_airplane[r.aircraft_code].fuel_burn_first + (travel_secondo_tratto - 1)*map_airplane[r.aircraft_code].fuel_burn_second;
						}
						*/
						//ora fisso le vairabili che mi servono, questo lo faccio perchè nella departure per il passeggero potrebbe esserci il caso in cui l'aero possa fare 
						//refuel
						double f_after_primo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto;
						double f_after_secondo_tratto = 0.0;
						if (map_airstrip[passengers[p].departure_location].fuel) {
							f_after_secondo_tratto = map_airplane[r.aircraft_code].max_fuel - fuel_consumed_secondo_tratto;
						}
						else {
							f_after_secondo_tratto = r.quantity_fuel[r.index - 1] - fuel_consumed_primo_tratto - fuel_consumed_secondo_tratto;
						}


						//cout << "if caso C " << best_cost << " > " << cost << " && " << t_arr_arrival << " <= " << end_day << " && " << f_after_primo_tratto << " >= " << map_airplane[r.aircraft_code].min_fuel << " && " << f_after_secondo_tratto << " >= " << (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code + "/" + passengers[p].arrival_location]) << endl;
						if (best_cost > cost) {
							if (t_arr_arrival <= end_day) {
								if (f_after_primo_tratto >= map_airplane[r.aircraft_code].min_fuel) {
									if (f_after_secondo_tratto >= (map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][passengers[p].arrival_location])) {
										best_cost = cost;
										best_passenger = p;
										situation = "C";
									}
								}
							}
							//cout << "situation C, cost: " << cost << "passenger: " << p << endl;
						}

					}
				}
			}
			//qui dovrei avere tutte le cose per fare la mossa sempre se la route non è piena

			/*if (situation != "Error") {
				cout << "la situation scelta e " << situation << "per il passeggero " << endl;
				passengers[best_passenger].print();
			}*/

			if (situation == "A") {
				r.primo_pass[0] = true;
				//time
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure; // in questo caso anche l'arrivo in quanto è la partenza dal depot

				passengers[best_passenger].solution_from = r.index - 1;
				r.capacity[r.index - 1]++;

				//for the weight
				r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
					r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				if (r.weight[r.index - 1] < 0) {
					r.quantity_fuel[r.index - 1] += r.weight[r.index - 1];
					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						r.quantity_fuel[r.index - 1] - passengers[best_passenger].weight;
				}
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


					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time_from + travel_time, time_from + travel_time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					//for the weight
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;
					//end for weight

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);





			}
			else if (situation == "B") {
				r.primo_pass[0] = true;
				r.time_arr[r.index - 1] = passengers[best_passenger].early_departure - (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60 -
					map_airstrip[r.places[r.index - 1]].ground_time;
				r.time_dep[r.index - 1] = passengers[best_passenger].early_departure - (from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;

				//for the weight
				r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
					r.quantity_fuel[r.index - 1];  //here i put max_fuel because at the beginnig in the depot all the airplane have full fuel
				//end for weight

				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];
				if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 1, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;
				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];

					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, passengers[best_passenger].early_departure, passengers[best_passenger].early_departure + map_airstrip[passengers[best_passenger].departure_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] - passengers[best_passenger].weight + fuel_consumed;

				}
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
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;

					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + passengers[best_passenger].weight + fuel_consumed;

				}
				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);




			}
			else if (situation == "C") {

				if (map_airstrip[passengers[best_passenger].departure_location].fuel) {
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, map_airplane[r.aircraft_code].max_fuel, 0.0, 1,
						r.time_dep[r.index - 1] +
						((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60),
						r.time_dep[r.index - 1] + ((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[passengers[best_passenger].departure_location].ground_time);

					r.weight[r.index - 1] = map_airplane[r.aircraft_code].weight_fuel_people -
						map_airplane[r.aircraft_code].max_fuel - passengers[best_passenger].weight;


				}
				else {
					double fuel_before = r.quantity_fuel[r.index - 1];
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[r.index - 1]][passengers[best_passenger].departure_location];
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60;

					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].departure_location, map_airstrip[passengers[best_passenger].departure_location].fuel, fuel_before - fuel_consumed, 0.0, 1, r.time_dep[r.index - 1] +
						((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60),
						r.time_dep[r.index - 1] + ((from_to[r.places[r.index - 1]][passengers[best_passenger].departure_location] / map_airplane[r.aircraft_code].speed) * 60) + map_airstrip[passengers[best_passenger].departure_location].ground_time);

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
					//double travel_time = (from_to[r.places[r.index - 1] + ";" + passengers[best_passenger].arrival_location] / map_airplane[r.aircraft_code].speed) * 60;


					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}


				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == "D") {


				for (int h = best_from; h < best_to; h++) {
					r.capacity[h]++;
					r.weight[h] -= passengers[best_passenger].weight;
				}


				//**************************************************************************
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = best_from; j < r.index; j++) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					if (r.weight[j] < 0) {
						//cout << " Weigth negativo in " << j << weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
				//**************************************************************************




				passengers[best_passenger].solution_from = best_from;
				passengers[best_passenger].solution_to = best_to;
				r.passengers_in_route.push_back(passengers[best_passenger]);
			}
			else if (situation == "E") {



				for (int h = best_from; h < r.index; h++) {
					r.capacity[h]++;
					r.weight[h] -= passengers[best_passenger].weight;
				}


				//******************************************************************************
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = best_from; j < r.index; j++) {
					//cout << " Sono nel rapeir devo aggiunger fuel linea 1015 devo guardare da " << node_add_from << " a " << node_add_to << endl;
					if (r.weight[j] < 0) {
						//cout << " Weigth negativo in " << j << weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) {
								index_refueling = i;
								break;
							}
						}
						//cout << " Paro ad aggiornare in index refule " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Aggiorno nodo " << t << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Weight dopo " << weight[t] << endl;
						}
					}
				}
				//******************************************************************************



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
					/*
					if (travel_time <= 60) {
						fuel_consumed += (travel_time / 60)*map_airplane[r.aircraft_code].fuel_burn_first;
					}
					else {
						fuel_consumed += map_airplane[r.aircraft_code].fuel_burn_first + ((travel_time - 60) / 60)*map_airplane[r.aircraft_code].fuel_burn_second;
					}
					*/
					r.addPlace(passengers[best_passenger].arrival_location, map_airstrip[passengers[best_passenger].arrival_location].fuel, fuel_before - fuel_consumed, 0.0, 0, time, time + map_airstrip[passengers[best_passenger].arrival_location].ground_time);
					r.weight[r.index - 1] = r.weight[r.index - 2] + fuel_consumed + passengers[best_passenger].weight;

				}

				passengers[best_passenger].solution_to = r.index - 1;
				r.passengers_in_route.push_back(passengers[best_passenger]);

			}



			if (situation != "Error") passengers.erase(passengers.begin() + best_passenger);



		} while (situation != "Error");
		if (passengers.size() == 0) break;
	}





	vector<Route> solution_clean;
	for (auto s : solution) {
		//s.print();
		if (s.index != 1) solution_clean.push_back(s);
	}

	cout << "ecco i passeggeri rimasti" << endl;
	for (Passenger rimasto : passengers) rimasto.print();

	return solution_clean;
}


//ECCO I NOSTRI DESTROY
double cost_time_windows_for_node(const Route& r, const vector<Passenger>& pass, map<int, Airstrip>& map_airstrip) {
	double cost = 0.0;
	for (const Passenger& p : pass) {
		double TW_departure = 0.0;
		double TW_arrival = 0.0;
		if (p.giorno == 1) {
			if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
			else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		}
		else if (p.giorno == 2) {
			if (p.solution_from == r.start_end_routes[1][0]) {
				double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
				if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
				else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
			}
			else {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
			}

			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		}

		else if (p.giorno == 3) {
			if (p.solution_from == r.start_end_routes[2][0]) {
				double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
				if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
				else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
			}
			else {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
			}

			if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
			else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		}

		cost += TW_departure + TW_arrival;
	}

	return cost;
}

map<int, int> Compute_WorstNode(double peso_intermediate_stop, Route& route, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to) {
	double costoWorstNode = 0.00;
	int WorstNode = -1;
	map<double, int> Node;
	map<int, int> NodeOrder;
	set<double, MyCOMP<double>> Myset;
	if (route.index <= 2) {
		//route.print();
		Node.insert(make_pair(100000, 1));
		Myset.insert(100000);
		//cout << " Route troppo piccola fisso indice a " << 1 << endl;
	}
	else {
		for (int n = 1; n < route.index; n++) {
			double dist = 0.0;
			double cost_IS = 0.0;
			vector<Passenger> PassengerNodo;
			if (n != (route.index - 1)) {
				//cout << " Calcolo per nodo n mezzo " << endl;
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]] + from_to[route.places[n]][route.places[n + 1]] - from_to[route.places[n - 1]][route.places[n + 1]];
				for (int p = 0; p < route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_from == n || route.passengers_in_route[p].solution_to == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}

					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += peso_intermediate_stop;
						}
					}
					//****************************************************************************************************************************************


				}
			}
			else {
				//cout << " Calcolo per ultimo nodo " << endl;
				dist += map_airstrip[route.places[n]].landing_cost;
				dist += from_to[route.places[n - 1]][route.places[n]];
				for (int p = 0; p < route.passengers_in_route.size(); p++) {
					if (route.passengers_in_route[p].solution_to == n) {
						PassengerNodo.push_back(route.passengers_in_route[p]);
					}


					//pezzo aggiunto per intermediate stop****************************************************************************************************
					if (route.passengers_in_route[p].solution_from < n) {
						if (route.passengers_in_route[p].solution_to > n) {
							cost_IS += peso_intermediate_stop;
						}
					}
					//****************************************************************************************************************************************

				}


			}


			dist += (cost_time_windows_for_node(route, PassengerNodo, map_airstrip)) + cost_IS;
			// / PassengerNodo.size());
			//cout << " Inserisco nodo: " << n << " con il costo: " << dist << endl;
			Node.insert(make_pair(dist, n));
			Myset.insert(dist);
			//cout << " ************** Nodo: " << n << " Ha un costo: " << dist << endl;
			//cout << " ************** Nodo Peggiore finora: " << WorstNode << endl;
			//if (costoWorstNode < dist) {
				//costoWorstNode = dist;
				//WorstNode = n;
			//}
		}
	}

	//cout << " **************Stampo la mappa non ordinata: "<< endl;
	//for (auto x : Node) cout << x.first << " -----> " << x.second << endl;
	//cout << " **************Stampo il Set: " << endl;
	//for (auto setval : Myset) {
		//cout << setval << endl;
	//}

	int i = 0;
	for (auto setval : Myset) {
		NodeOrder.insert(make_pair(i, Node[setval]));
		i++;
	}
	//cout << " **************Stampo la mappa Ordinata: " << endl;
	//for (auto x : NodeOrder) cout << x.first << " -----> " << x.second << endl;
	return NodeOrder;
}


bool aggregate_same_nodes_destoy_rolling(Route& r, int node) {
	//cout << " Sono nel codice di nodi aggregati " << endl;
	//cout << "Ho sistemato il start_end devo fare erase del nodo del punto  " << node << endl;
	r.Print();
	double start_day3 = 3240;
	double start_day1 = 360;
	double start_day2 = 1800;
	double end_day3 = 3990;
	double end_day2 = 2550;
	bool goback = false;
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node)) p.solution_to--;
	}

	//cout << " Dvo rimuovere nodo in posizione " << node << endl;
	int gg = 0;
	if (node <= r.start_end_routes[0][1]) {
		r.start_end_routes[0][1] --;
		if (r.start_end_routes[1][0] > 0) r.start_end_routes[1][0] --;
		if (r.start_end_routes[1][1] > 0) r.start_end_routes[1][1] --;
		if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
		if (r.start_end_routes[2][1] > 0)r.start_end_routes[2][1] --;
		//cout << " Ho abbassato tutti caso 1" << endl;
	}
	else if (node <= r.start_end_routes[1][1]) {
		//cout << " Ho abbassato da 2 in poi  caso 2" << endl;
		/*
		if (r.start_end_routes[1][1] == r.start_end_routes[1][0] && r.start_end_routes[1][1] == 0) {
		r.start_end_routes[1][1] = -1;
		r.start_end_routes[1][0] = -1;
		}
		*/

		if (r.start_end_routes[1][1] > 0) r.start_end_routes[1][1] --;
		if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
		if (r.start_end_routes[2][1] > 0) r.start_end_routes[2][1] --;
		gg = 1;
	}
	else {
		//cout << " Ho abbassato solo fine di 3 caso 3" << endl;
		r.start_end_routes[2][1] --;
		gg = 2;
	}
	//cout << " OKKKKK .. adessso comincio a togliere " << endl;
	//cout << r.time_dep[node] << " - " << r.time_arr[node] << endl;
	double diff = r.time_dep[node] - r.time_arr[node];
	r.time_arr[node + 1] = r.time_arr[node];
	//// Qui devi mettere cosidce ce mette a post tempi all'indietro fino al nodo del [gg][0]
	//aggiungi differenza
	//r.Print();
	/*cout << " Giorno vale: " << gg << endl;
	cout << "start_end_routes[0][0] " << r.start_end_routes[0][0] << endl;
	cout << "start_end_routes[0][1] " << r.start_end_routes[0][1] << endl;
	cout << "start_end_routes[1][0] " << r.start_end_routes[1][0] << endl;
	cout << "start_end_routes[1][1] " << r.start_end_routes[1][1] << endl;
	cout << "start_end_routes[2][0] " << r.start_end_routes[2][0] << endl;
	cout << "start_end_routes[2][1] " << r.start_end_routes[2][1] << endl;
	cout << " Condizione che forse nn mi fa entrare " << r.time_dep[node + 1] << " - " << r.time_arr[node + 1] << " < 480 " << endl;
	*/
	if ((node) != 0 && (node) != r.start_end_routes[0][1] && (node) != r.start_end_routes[1][1] && (node) != r.start_end_routes[2][1] && diff > 0 && (r.time_dep[node + 1] - r.time_arr[node + 1] < 480)) {
		//cout << " Sono nella situazione dei tempi " << endl;
		//cout << " Node: " << node << endl;
		//r.Print();
		r.time_arr[node + 1] += diff;
		for (int i = node - 1; i < r.start_end_routes[gg][0]; i--) {
			//cout << " lo faccio per nodo " << i << endl;
			r.time_arr[i] += diff;
			r.time_dep[i] += diff;
		}
		//cout << " Adesso metto a posto nodo conessione che ha arrivo: " << r.time_arr[r.start_end_routes[gg][0]] << " partenza: " << r.time_arr[r.start_end_routes[gg][0]] << endl;
		r.time_dep[r.start_end_routes[gg][0]] += diff;
		//r.Print();
		//cin.get();
	}
	// bisogna metetre controllo che non parta troppo presto
	double time = r.time_dep[node + 1];
	double limit = start_day3;
	if (gg == 0) limit = start_day2;
	//cout << " Giorno: " << gg << endl;
	if (time > end_day2) limit = start_day3;
	//cout << " Partirebbe alle  " << time << " quando c'è minimo di  " << limit << endl;
	if (time < limit) {
		if (!(1 == r.places[node] || 4 == r.places[node])) {
			//cout << " ----- PERICOLO DI PARTENZA ANTICIPATA ------- " << endl;
			//cout << " Caso in cui devi stare molto attento... node:  " << node << endl;
			if (node == r.start_end_routes[0][1] + 1 && time < end_day2 && r.start_end_routes[1][1] != -1) {
				//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << limit << endl;
				//cout << " Dopo ci dovrebbe essere questo check " << r.time_arr[r.start_end_routes[1][1] + 1] << " + " << (start_day2 - time) << " <=  " << end_day2 << endl;
				//r.Print();
				//cin.get();
				// DOPO dovrai scommentrare la parte sotto perchè non è finita
				if ((r.time_arr[r.start_end_routes[1][1] + 1] + (start_day2 - time)) <= end_day2) {
					r.time_dep[node + 1] = start_day2;
					//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
					for (int a = node + 2; a <= r.start_end_routes[1][1]; a++) {
						//cout << " aggiorno " << r.places[a] << endl;
						r.time_arr[a] += (start_day2 - time);
						r.time_dep[a] += (start_day2 - time);
					}
					r.time_arr[r.start_end_routes[1][1] + 1] += (start_day2 - time);
					if (r.index == (r.start_end_routes[1][1] + 1 + 1)) r.time_dep[r.start_end_routes[1][1] + 1] += (start_day2 - time);
				}
				else goback = true;

				//r.Print();
				//cin.get();
			}else if (node == r.start_end_routes[0][1] + 1 && time < end_day3 && r.start_end_routes[1][1] == -1) {
				//cout << " CAso in cui sto per partite troppo presto 3 (te lo mando da 1) doveva partre ad " << limit << endl;
				//cout << " Dopo ci dovrebbe essere questo check " << r.time_arr[r.start_end_routes[2][1] + 1] << " + " << (start_day3 - time) << " <=  " << end_day3 << endl;
				//r.Print();
				//cin.get();
				// DOPO dovrai scommentrare la parte sotto perchè non è finita
				if ((r.time_arr[r.start_end_routes[2][1] + 1] + (start_day3 - time)) <= end_day3) {
					r.time_dep[node + 1] = start_day3;
					//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
					for (int a = node + 2; a <= r.start_end_routes[2][1]; a++) {
						//cout << " aggiorno " << r.places[a] << endl;
						r.time_arr[a] += (start_day3 - time);
						r.time_dep[a] += (start_day3 - time);
					}
					r.time_arr[r.start_end_routes[2][1] + 1] += (start_day3 - time);
					if (r.index == (r.start_end_routes[2][1] + 1 + 1)) r.time_dep[r.start_end_routes[2][1] + 1] += (start_day3 - time);
				}
				else goback = true;

				//r.Print();
				//cin.get();
			}
			else if (node == r.start_end_routes[1][1] + 1) {
				//cout << " CAso in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
				//cout << " CAso in cui sto per partite troppo presto 3 doveva partre ad " << limit << endl;
				//cout << " Dopo ci dovrebbe essere questo check " << r.time_arr[r.start_end_routes[2][1] + 1] << " + " << (start_day3 - time) << " <=  " << end_day3 << endl;
				// DOPO dovrai scommentrare la parte sotto perchè non è finita
				//r.Print();
				//cin.get();

				if ((r.time_arr[r.start_end_routes[2][1] + 1] + (start_day3 - time)) <= end_day3) {
					r.time_dep[node + 1] = start_day3;
					//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
					for (int a = node + 2; a <= r.start_end_routes[2][1]; a++) {
						//cout << " aggiorno " << r.places[a] << endl;
						r.time_arr[a] += (start_day3 - time);
						r.time_dep[a] += (start_day3 - time);
					}
					r.time_arr[r.start_end_routes[2][1] + 1] += (start_day3 - time);
					if (r.index == (r.start_end_routes[2][1] + 1 + 1)) r.time_dep[r.start_end_routes[2][1] + 1] += (start_day3 - time);
				}
				else goback = true;
				//r.Print();
				//cin.get();
			}
		}
	}
	r.index--;
	//r.index_cap--;
	r.places.erase(r.places.begin() + node);
	r.time_arr.erase(r.time_arr.begin() + node);
	r.time_dep.erase(r.time_dep.begin() + node);
	r.refueling.erase(r.refueling.begin() + node);
	r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
	r.weight.erase(r.weight.begin() + node);
	r.capacity.erase(r.capacity.begin() + node);
	//r.Print();
	//cout << "Ho sistemato il start_end devo fare erase del nodo del punto  " << node << endl;
	return goback;
}

void aggregate_same_nodes_destoy_rolling_local_search(Route& r, int node) {
	//cout << " Sono nel codice di nodi aggregati " << endl;
	//cout << "Ho sistemato il start_end devo fare erase del nodo del punto  " << node << endl;
	//r.Print();
	double start_day3 = 3240;
	double start_day1 = 360;
	double start_day2 = 1800;
	double end_day3 = 3990;
	double end_day2 = 2550;
	bool goback = false;
	// La modicfica del form e to del passeggiero non funziona
	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == (1 + node)) p.solution_from = node;
		if (p.solution_to == (1 + node)) p.solution_to = node;
		if (p.solution_from > (1 + node)) p.solution_from--;
		if (p.solution_to > (1 + node)) p.solution_to--;
	}

	//cout << " Dvo rimuovere nodo in posizione " << node << endl;
	int gg = 0;
	if (node <= r.start_end_routes[0][1]) {
		r.start_end_routes[0][1] --;
		if (r.start_end_routes[1][0] > 0) r.start_end_routes[1][0] --;
		if (r.start_end_routes[1][1] > 0) r.start_end_routes[1][1] --;
		if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
		if (r.start_end_routes[2][1] > 0)r.start_end_routes[2][1] --;
		//cout << " Ho abbassato tutti caso 1" << endl;
	}
	else if (node <= r.start_end_routes[1][1]) {
		//cout << " Ho abbassato da 2 in poi  caso 2" << endl;
		/*
		if (r.start_end_routes[1][1] == r.start_end_routes[1][0] && r.start_end_routes[1][1] == 0) {
		r.start_end_routes[1][1] = -1;
		r.start_end_routes[1][0] = -1;
		}
		*/

		if (r.start_end_routes[1][1] > 0) r.start_end_routes[1][1] --;
		if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
		if (r.start_end_routes[2][1] > 0) r.start_end_routes[2][1] --;
		gg = 1;
	}
	else {
		//cout << " Ho abbassato solo fine di 3 caso 3" << endl;
		r.start_end_routes[2][1] --;
		gg = 2;
	}
	//cout << " OKKKKK .. adessso comincio a togliere " << endl;
	//cout << r.time_dep[node] << " - " << r.time_arr[node] << endl;
	double diff = r.time_dep[node] - r.time_arr[node];
	r.time_arr[node + 1] = r.time_arr[node];
	//// Qui devi mettere cosidce ce mette a post tempi all'indietro fino al nodo del [gg][0]
	//aggiungi differenza
	//r.Print();
	//cout << " Giorno vale: " << gg << endl;
	//cout << "start_end_routes[0][0] " << r.start_end_routes[0][0] << endl;
	//cout << "start_end_routes[0][1] " << r.start_end_routes[0][1] << endl;
	//cout << "start_end_routes[1][0] " << r.start_end_routes[1][0] << endl;
	//cout << "start_end_routes[1][1] " << r.start_end_routes[1][1] << endl;
	//cout << "start_end_routes[2][0] " << r.start_end_routes[2][0] << endl;
	//cout << "start_end_routes[2][1] " << r.start_end_routes[2][1] << endl;
	if ((node) != 0 && (node) != r.start_end_routes[0][1] && (node) != r.start_end_routes[1][1] && (node) != r.start_end_routes[2][1] && diff > 0 && (r.time_dep[node + 1] - r.time_arr[node + 1] < 480)) {
		//cout << " ----------------->>>>>>>>> <><><><><><<<<< Sono nella situazione dei tempi <><><><><><<<<< --------------------------- " << endl;
		//cout << " Node: " << node << endl;
		//r.Print();
		r.time_arr[node + 1] += diff;
		//cout << " Differenza pari a " << diff << endl;
		//cout << " parto da  " << node - 1 << " fino a < " << r.start_end_routes[gg][0] << endl;
		for (int i = node - 1; i > r.start_end_routes[gg][0]; i--) {
			//cout << " lo faccio per nodo " << i << endl;
			r.time_arr[i] += diff;
			r.time_dep[i] += diff;
		}
		//cout << " Adesso metto a posto nodo conessione che ha arrivo: " << r.time_arr[r.start_end_routes[gg][0]] << " partenza: " << r.time_arr[r.start_end_routes[gg][0]] << endl;
		//cout << " Tempo di departure del nodo di conessione = " << r.time_dep[r.start_end_routes[gg][0]] << " gli aggiungero " << diff << endl;
		r.time_dep[r.start_end_routes[gg][0]] += diff;
		//r.Print();
		//cin.get();
	}
	r.index--;
	//r.index_cap--;
	r.places.erase(r.places.begin() + node);
	r.time_arr.erase(r.time_arr.begin() + node);
	r.time_dep.erase(r.time_dep.begin() + node);
	r.refueling.erase(r.refueling.begin() + node);
	r.quantity_fuel.erase(r.quantity_fuel.begin() + node);
	r.weight.erase(r.weight.begin() + node);
	r.capacity.erase(r.capacity.begin() + node);
	//r.Print();
	//cout << "Ho sistemato il start_end devo fare erase del nodo del punto  " << node << endl;

	if (node == r.start_end_routes[0][1] + 1 || node == r.start_end_routes[1][1] + 1 || node == r.start_end_routes[2][1] + 1) {
		if (gg == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
			//cout << " Caso del giorno 1 ho giorno 2 " << endl;
			///prendo da giorno 2 metto in 1
			//r.Print();
			//system("pause");
			r.start_end_routes[0][1] ++;
			if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
			//r.Print();
			//system("pause");
		}
		else if (gg == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] > -1 && r.start_end_routes[2][0] > -1) {
			//cout << " Caso del giorno 1 non ho giorno 2" << endl;
			///prendo da giorno 3 metto in 1
			//r.Print();
			r.start_end_routes[0][1] ++;
			r.start_end_routes[2][0] ++;
			//r.Print();
			//system("pause");
		}
		else if (r.start_end_routes[1][1] >= -1 && r.primo_pass[1] == 1 && gg == 1 && r.start_end_routes[2][0] > -1) {
			//cout << " Caso dove 2 da a 3  " << endl;
			///prendo da giorno 3 metto in 1
			r.start_end_routes[1][1] ++;
			r.start_end_routes[2][0] ++;
		}
		else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && gg == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
			if (r.time_arr[node] > start_day2) {
				//cout << " Situazione dove node_destroy " << r.places[node] << " tempo arrivo: " << r.time_arr[node] << "  > " << start_day2 << endl;
				//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
				//r.Print();
				//cin.get();
				r.start_end_routes[1][1] ++;
				r.start_end_routes[2][0] ++;
			}
			else {
				//cout << " Situazione dove node_destroy " << r.places[node] << " tempo arrivo: " << r.time_arr[node] << "  < " << start_day2 << endl;
				//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
				//r.Print();
				//cin.get();
				r.start_end_routes[0][1] ++;
				r.start_end_routes[2][0] ++;
			}
		}
		else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && gg == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] == -1 && (r.time_dep[r.start_end_routes[1][1]] < end_day2 && r.time_arr[r.start_end_routes[1][1] + 1] < end_day2)) {
			//cout << " Caso dove 2 da a 3 particolare con tempi " << endl;
			///prendo da giorno 3 metto in 1 --------------------->>>> QUA FORSE DEVI CAAMBIARE
			//r.Print();
			//cin.get();
			r.start_end_routes[1][1] ++;
			r.start_end_routes[2][0] ++;
		}
		//((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1)
		else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && gg == 1 && r.primo_pass[1] == 0 && r.time_arr[node] <= end_day2) {
			//cout << " Caso dove 2 da a 3 particolareggiante " << endl;
			///prendo da giorno 3 metto in 1
			//r.Print();
			//cin.get();
			r.start_end_routes[1][1] ++;
			r.start_end_routes[2][0] ++;
		}
		//r.Print();
	}



}






void cleaning_rolling(double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, int node_destroy, vector<Passenger>& passenger_removed, Route& r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool goback = false;
	Route old = r;
	//cout << " sei appena entrato dentro la funxione " << endl;
	//cout << " ------------------------------------->>>>>> CLEANING <<<<<<<< ------------------------------------------------------ " << endl;
	//cout << " Passeggieri rimossi finora: " << passenger_removed.size() << endl;
	//cout << "---------------------------> Voglio provare a distruggere seguente  " << r.aircraft_code << " Qio sei dentro al cleaning " << endl;
	//cout << "---------------------------> Ha numero di passeggieri " << r.passengers_in_route.size() << endl;
	int so_far = passenger_removed.size();
	//cout << " Passeggieri rimossi fino a questo momento " << to_string(so_far) << endl;
	//cout << " IL nodo " << node_destroy << " di " << r.index << endl;
	if (node_destroy == r.index - 1) { //this in the case of that node is at the end
		//cout << "nodo distrutto: " << node_destroy << endl;
		//r.print();
		vector<int> int_removed;
		//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
		//cout << " Questa route ha un index = " << r.index << endl;
		//cout << " siamo nel caso che il nodo distrutto sia l'ultimo " << endl;
		//r.print();

		int Min_From_Pass = node_destroy;
		int Max_To_Pass = node_destroy;
		for (int p = 0; p < r.passengers_in_route.size(); p++) {
			if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
				if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
				int_removed.push_back(p);

				for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
					r.capacity[t]--;
					r.weight[t] += r.passengers_in_route[p].weight;
				}


			}
		}
		for (int i = int_removed.size() - 1; i >= 0; i--) {
			// code for repair forbidden***********************************************************
			//*************************************************************************************

			passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
			vector<Passenger>::iterator it;
			it = r.passengers_in_route.begin();
			r.passengers_in_route.erase(it + int_removed[i]);
		}
		// Qua devo mettere codice che è quello dell'Update
		/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
		cout << "******************* Stampiamo la Route: *******************" << endl;
		cout << " CODE ------> " << r.aircraft_code << endl;
		cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
		cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
		//r.print();
		cout << " Node destroyed ------> " << node_destroy << endl;
		*/

		int index_min_from;
		for (int i = Min_From_Pass; i >= 0; i--) {
			if (r.refueling[i] && i != node_destroy) {
				index_min_from = i;
				break;
			}
		}
		//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
		// Codice che serve per cercare il minimo nel range
		//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


		for (int k = index_min_from; k < Max_To_Pass; k++) {
			if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r.weight[k];
				int index_updating_from = k;
				int index_updating_to = r.index;  //qua prima c'era -1
				for (int i = k + 1; i <= Max_To_Pass; i++) {
					//cout << " Sto guardando il nodo " << i << endl;
					if (r.refueling[i]) break;
					//cout << " Ho passato il break " << endl;
					if (r.weight[i] < min_weight && i != node_destroy) {
						// cout << " Ho passato IF di controllo " << endl;
						min_weight = r.weight[i];
						Node_min = i;
					}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					for (int i = k + 1; i < r.index; i++) {
						if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						if (i != node_destroy) {
							r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
							r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
						}
					}
				}
			}

		}
		r.removePlace(node_destroy, map_airplane);
		//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi quando tolgo ultimo nodo " << endl;
		double add_fuel = 0;
		int index_weight_neg = -1;
		for (int j = 0; j < r.index; j++) {
			if (r.weight[j] < 0) {
				//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
				add_fuel = r.weight[j];
				index_weight_neg = j;
				int index_refueling = index_weight_neg;
				for (int i = index_weight_neg; i >= 0; i--) {
					if (r.refueling[i]) { //&& i != node_destroy
						index_refueling = i;
						break;
					}
				}
				//cout << " Aggiorno da " << index_refueling << endl;
				for (int t = index_refueling; t < r.index; t++) {
					//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
					if (r.refueling[t] && t != index_refueling) break;
					//cout << " Ho passato if aggiorno nodo " << t << endl;
					//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
					r.quantity_fuel[t] += add_fuel;
					r.weight[t] -= add_fuel;
					//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
				}
			}
		}
	}
	else {
		// Non è ultimo nodo
		int giorno = -1;
		if (node_destroy <= r.start_end_routes[0][1]) giorno = 0;
		else if (node_destroy <= r.start_end_routes[1][1]) giorno = 1;
		else giorno = 2;
		bool imp_ref = false;
		//cout << "Nodo: " << node_destroy << endl;
		if (node_destroy != (r.start_end_routes[0][1]) && node_destroy != (r.start_end_routes[1][1])) {
			// Non è nodo di conessione
			//cout << " Ok non è ultimo nodo in nessuna giornata " << endl;
			//cout << " Nodo appartiene alla giornata " << giorno << endl;
			if (giorno != 2 && r.refueling[node_destroy] && !(r.refueling[r.start_end_routes[giorno][1]])) {
				int j = r.index - 1;
				imp_ref = true;
				for (int i = node_destroy + 1; i < r.index; i++) {
					if (r.refueling[i]) {
						j = i;
						break;
					}
				}
				double fuel1 = r.quantity_fuel[node_destroy - 1] - from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
				double diff = r.quantity_fuel[node_destroy + 1] - fuel1;
				if ((r.quantity_fuel[j - 1] - diff - from_to_FuelConsumed[r.aircraft_code][r.places[j - 1]][r.places[j]]) >= map_airplane[r.aircraft_code].min_fuel) imp_ref = false;
			}
			if (imp_ref) {
				//cout << " Qua dic eche è importante peche lo dice ?!?!? " << endl;
				//r.Print();
			}
			//cout << " Y/N nodo importante:  " << imp_ref << endl;
			double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
			if (!imp_ref && (fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
				//cout << " Ok -----------> è definitivamente un nodo che puo essere rimosso " << endl;
				vector<int> int_removed;
				int Min_From_Pass = node_destroy;
				int Max_To_Pass = node_destroy;
				for (int p = 0; p < r.passengers_in_route.size(); p++) {
					if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
						if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
						if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
						int_removed.push_back(p);
						for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
							r.capacity[t]--;
							r.weight[t] += r.passengers_in_route[p].weight;
						}
					}
				}

				//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
				//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
				//cout << " Adesso sto per rimuovere i passeggieri " << endl;
				for (int i = int_removed.size() - 1; i >= 0; i--) {
					// code for repair forbidden***********************************************************
					//*************************************************************************************
					passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
					vector<Passenger>::iterator it;
					it = r.passengers_in_route.begin();
					r.passengers_in_route.erase(it + int_removed[i]);
				}
				r.update_route_destroy(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
				int index_before = node_destroy - 1;
				//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
				//r.print();
				//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
				double diff = 0;
				//cout << " Index before: " << index_before << endl;
				for (int i = node_destroy + 1; i < r.index; i++) {
					//cout << " sono al i = " << i << endl;
					if (r.refueling[i]) break;
					//cout << "weight before: " << r.weight[i] << endl;
					if (index_before == (node_destroy - 1)) {
						//cout << " sono al primo giro faccio " << endl;
						diff = r.quantity_fuel[i];
						//cout << " diff: " << diff << endl;
						r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
						diff = diff - r.quantity_fuel[i];
						//cout << " diff aggiornata: " << diff << endl;

					}
					else {
						//cout << " non sono al primo giro " << endl;
						r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
						//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
					}

					//cout << " weight prima " << r.weight[i] << endl;
					r.weight[i] = r.weight[i] + diff;
					//cout << " weight dopo " << r.weight[i] << endl;



					//cout << "weight after: " << r.weight[i] << endl;
					//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
					//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
					//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

					//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
					//cout << " inidce before = " << index_before << endl;
					index_before = i + 1;
					//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
				}
				//*****************************************************************************************************************************************************************


				r.removePlace(node_destroy, map_airplane);
				//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weight[j] < 0) {
						//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						//cout << " Aggiorno da " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Ho passato if aggiorno nodo " << t << endl;
							//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
						}
					}
				}
				//cout << " Nodo destroyed " << node_destroy << " r.start_end_routes[0][1]: " << r.start_end_routes[0][1] << " r.start_end_routes[1][1]: " << r.start_end_routes[0][1] << endl;
				//if (r.aircraft_code == "5H-RJS") r.print();
				//if (r.index > 2 && (((r.start_end_routes[0][1] - r.start_end_routes[0][0] ==1) && r.primo_pass[0] != 1) || ((r.start_end_routes[1][1] - r.start_end_routes[1][0] == 1) && r.primo_pass[1] != 1))) {
				if (r.index > 2) {
					//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
					//cout << r.start_end_routes[0][1] - r.start_end_routes[0][0] << "  == 1 && " << (r.start_end_routes[1][1] - r.start_end_routes[1][0]) << " >= 1 && " << r.primo_pass[0] << endl;
					if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.primo_pass[0] == 0) {
						//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
						r.start_end_routes[1][0] --;
						r.start_end_routes[0][0] = -1;
						r.start_end_routes[0][1] = -1;
						r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
						r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
						r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
						//r.Print();
						//system("pause");
					}
					else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
						//else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1  && r.capacity[0] == 0 ) {
						//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
						r.start_end_routes[2][0] --;
						r.start_end_routes[1][0] = -1;
						r.start_end_routes[1][1] = -1;
						r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
						r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
						r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
						//r.Print();
						//system("pause");
					}
					else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.primo_pass[0] == 0) {
						//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
						r.start_end_routes[2][0] --;
						r.start_end_routes[0][0] = -1;
						r.start_end_routes[0][1] = -1;
						r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
						r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
						r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
						//r.Print();
						//system("pause");
					}
				}

			}
		}
		else if (!r.refueling[node_destroy]) {
			//cout << " Nodo: " << node_destroy << " Name: " << r.places[node_destroy] << " Refuel: " << r.refueling[node_destroy] << endl;
			//cout << " ---------------------> Caso che tolgo nodo Di collegamento in cui NON devo fare refuel" << endl;
			// --------------------------------------------------------------------------------Inizio codiche che deve togliere il nodo -------------------------------------------------------------------
			double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
			if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
				//cout << " Ok è definitivamente un nodo che puo essere rimosso " << endl;
				vector<int> int_removed;
				int Min_From_Pass = node_destroy;
				int Max_To_Pass = node_destroy;
				for (int p = 0; p < r.passengers_in_route.size(); p++) {
					if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
						if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
						if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
						int_removed.push_back(p);
						for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
							r.capacity[t]--;
							r.weight[t] += r.passengers_in_route[p].weight;
						}
					}
				}

				//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
				//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
				//cout << " Adesso sto per rimuovere i passeggieri " << endl;
				for (int i = int_removed.size() - 1; i >= 0; i--) {
					// code for repair forbidden***********************************************************
					//*************************************************************************************
					passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
					vector<Passenger>::iterator it;
					it = r.passengers_in_route.begin();
					r.passengers_in_route.erase(it + int_removed[i]);
				}
				r.update_route_destroy_middleNode(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
				int index_before = node_destroy - 1;
				//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
				//r.print();
				//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
				double diff = 0;
				//cout << " Index before: " << index_before << endl;
				for (int i = node_destroy + 1; i < r.index; i++) {
					//cout << " sono al i = " << i << endl;
					if (r.refueling[i]) break;
					//cout << "weight before: " << r.weight[i] << endl;
					if (index_before == (node_destroy - 1)) {
						//cout << " sono al primo giro faccio " << endl;
						diff = r.quantity_fuel[i];
						//cout << " diff: " << diff << endl;
						r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
						diff = diff - r.quantity_fuel[i];
						//cout << " diff aggiornata: " << diff << endl;

					}
					else {
						//cout << " non sono al primo giro " << endl;
						r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
						//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
					}

					//cout << " weight prima " << r.weight[i] << endl;
					r.weight[i] = r.weight[i] + diff;
					//cout << " weight dopo " << r.weight[i] << endl;



					//cout << "weight after: " << r.weight[i] << endl;
					//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
					//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
					//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

					//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
					//cout << " inidce before = " << index_before << endl;
					index_before = i + 1;
					//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
				}
				//*****************************************************************************************************************************************************************


				r.removePlace_New(node_destroy, map_airplane);
				//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
				double add_fuel = 0;
				int index_weight_neg = -1;
				for (int j = 0; j < r.index; j++) {
					if (r.weight[j] < 0) {
						//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
						add_fuel = r.weight[j];
						index_weight_neg = j;
						int index_refueling = index_weight_neg;
						for (int i = index_weight_neg; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_refueling = i;
								break;
							}
						}
						//cout << " Aggiorno da " << index_refueling << endl;
						for (int t = index_refueling; t < r.index; t++) {
							//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
							if (r.refueling[t] && t != index_refueling) break;
							//cout << " Ho passato if aggiorno nodo " << t << endl;
							//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
							r.quantity_fuel[t] += add_fuel;
							r.weight[t] -= add_fuel;
							//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
						}
					}
				}
				node_destroy--;
				//cout << " Fintio di togliere il nodo adesso valuto casistiche per il nuovo nodo di conessione " << endl;
				//r.Print();
				//system("pause");
				if (r.places[node_destroy] == r.places[node_destroy + 1] && r.index > (node_destroy - 1)) {
					//cout << " Caso !: in cui ho nodi doppi perchè " << r.places[node_destroy] << "==" << r.places[node_destroy + 1] << endl;
					/// NUOVS PARTE DEL CODICE
					r.time_arr[node_destroy + 1] = r.time_arr[node_destroy];
					/// FINITA NUOVA PARTE DEL CODICE
					//cout << " Inizio codice di aggrregazione nodi doppi " << endl;
					goback = aggregate_same_nodes_destoy_rolling(r, node_destroy);
					//int gg = 2;
					//if (node_destroy <= r.start_end_routes[0][1]) gg = 0;
					//else if (node_destroy <= r.start_end_routes[1][1]) gg = 1;
					//else giorno = 2;
					//r.Print();
					//cout << " Adesso devo mettere a posto gli indici dopo aggregazione dei nodi giorno: " << giorno << " Nodo: " << node_destroy << endl;
					if (node_destroy == r.start_end_routes[0][1] + 1 || node_destroy == r.start_end_routes[1][1] + 1 || node_destroy == r.start_end_routes[2][1] + 1) {
						//cout << " Adesso devo mettere a posto gli indici dopo aggregazione dei nodi giorno: " << giorno << endl;
						if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1 && r.time_arr[r.start_end_routes[0][1] + 1] < end_day1) {
							//if (giorno == 0 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] >= -1 && r.time_dep[0] < end_day1) {
							//cout << " Caso del giorno 1 ho giorno 2 " << endl;
							///prendo da giorno 2 metto in 1
							//r.Print();
							//system("pause");
							if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
							r.start_end_routes[0][1] ++;
							//r.Print();
							//system("pause");
						}
						else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] >= -1) {
							//cout << " Caso del giorno 1 non ho giorno 2" << endl;
							///prendo da giorno 3 metto in 1
							//r.Print();
							if (r.start_end_routes[2][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[2][0] ++;
							r.start_end_routes[0][1] ++;
							//r.Print();
							//system("pause");
						}
						else if (r.start_end_routes[1][1] >= -1 && giorno == 1 && r.primo_pass[1] == 1) {
							//cout << " Caso dove 3 da a 2  " << endl;
							///prendo da giorno 3 metto in 1
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else if (r.start_end_routes[1][0] == r.start_end_routes[1][1] && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][0] != -1 && r.time_arr[node_destroy] < end_day1) {
							//cout << " Caso molto speciale riga 7651 lo hai aggiunto per la casistica scoperta dopo aver commentato " << endl;
							if (r.time_arr[node_destroy] > start_day2) {
								//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
								//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
								//r.Print();
								//cin.get();
								r.start_end_routes[1][1] ++;
								r.start_end_routes[2][0] ++;
							}
							else {
								//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
								//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
								//r.Print();
								//cin.get();
								r.start_end_routes[0][1] ++;
								r.start_end_routes[2][0] ++;
							}
						}
						else if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && giorno == 1 && r.primo_pass[1] == 0) {
							//cout << " Caso dove 3 da a 2 super perticolare " << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else if (r.start_end_routes[1][0] == r.start_end_routes[1][1] && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][0] == -1) {
							//cout << " CAso in cui Primo giorno non ho nulla // Secondo GG un nodo // terzo giorno ho nodi // porta nodo da 2 a 3" << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else if (r.start_end_routes[1][0] == r.start_end_routes[1][1] && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][0] != -1 && r.time_arr[node_destroy] > end_day1) {
							//cout << " Caso molto speciale riga 7651 lo hai aggiunto per la casistica scoperta dopo aver commentato " << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						//r.Print();
						/*
						QUESTA è QUELLA CHE ABBIAMO NEL CASUAL
						if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
						cout << " Caso del giorno 1 ho giorno 2 " << endl;
						///prendo da giorno 2 metto in 1
						//r.Print();
						//system("pause");
						r.start_end_routes[0][1] ++;
						if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
						//r.Print();
						//system("pause");
						}
						else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] > -1) {
						cout << " Caso del giorno 1 non ho giorno 2" << endl;
						///prendo da giorno 3 metto in 1
						//r.Print();
						r.start_end_routes[0][1] ++;
						r.start_end_routes[2][0] ++;
						//r.Print();
						//system("pause");
						}
						else if (r.start_end_routes[1][1] >= -1 && r.primo_pass[1] == 1 && giorno == 1) {
						cout << " Caso dove 2 da a 3  " << endl;
						///prendo da giorno 3 metto in 1
						r.start_end_routes[1][1] ++;
						r.start_end_routes[2][0] ++;
						}
						else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
						if (r.time_arr[node_destroy] > start_day2) {
						cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
						cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
						r.Print();
						//cin.get();
						r.start_end_routes[1][1] ++;
						r.start_end_routes[2][0] ++;
						}
						else {
						cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
						cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
						r.Print();
						//cin.get();
						r.start_end_routes[0][1] ++;
						r.start_end_routes[2][0] ++;
						}
						}
						else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] == -1 && (r.time_dep[r.start_end_routes[1][1]] < end_day2 && r.time_arr[r.start_end_routes[1][1] + 1] < end_day2)) {
						cout << " Caso dove 2 da a 3 particolare con tempi " << endl;
						///prendo da giorno 3 metto in 1 --------------------->>>> QUA FORSE DEVI CAAMBIARE
						r.Print();
						//cin.get();
						r.start_end_routes[1][1] ++;
						r.start_end_routes[2][0] ++;
						}
						else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
						cout << " Caso dove 2 da a 3 particolareggiante " << endl;
						///prendo da giorno 3 metto in 1
						r.Print();
						//cin.get();
						r.start_end_routes[1][1] ++;
						r.start_end_routes[2][0] ++;
						}
						r.Print();
						*/
						//cout << " Sto per valutare casisitiche richieste dal NEl dopo nodi doppi linea 7061 " << endl;
						if (r.index > 2) {
							//cout << " sono alla linea 8193 " << endl;
							if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
								//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
								//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
								r.start_end_routes[1][0] --;
								r.start_end_routes[0][0] = -1;
								r.start_end_routes[0][1] = -1;
								r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
								r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
								//r.Print();
								//system("pause");
							}
							else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
								//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
								//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
								r.start_end_routes[2][0] --;
								r.start_end_routes[1][0] = -1;
								r.start_end_routes[1][1] = -1;
								r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
								r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
								//r.Print();
								//system("pause");
							}
							else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
								//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
								//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
								r.start_end_routes[2][0] --;
								r.start_end_routes[0][0] = -1;
								r.start_end_routes[0][1] = -1;
								r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
								r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
								//r.Print();
								//system("pause");
							}
						}
						//r.Print();
					}
				}
				else if (r.index > (node_destroy - 1)) {
					//cout << " Adesso devo valutare se riesco ad andarci al sera " << endl;
					double time = r.time_dep[node_destroy] + ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
					int limit = 0;
					if (giorno == 0) limit = end_day1;
					else if (giorno == 1) limit = end_day2;
					else limit = end_day3;
					//cout << " CI arriverebbe alle  " << time << " quando c'è un end day " << limit << endl;
					//r.Print();
					if (node_destroy == 0 && r.index > 2) {
						//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
						//r.Print();
						//system("pause");
						if (r.start_end_routes[2][0] == -1 && r.start_end_routes[1][1] >= 2) {
							//cout << " Caso in cui h route del giorno 1  e 2  e non del giorno 3 " << endl;
							//r.start_end_routes[0][1] --;
							//r.start_end_routes[1][0] --;
							r.start_end_routes[2][0] = -1;
							r.start_end_routes[2][1] = -1;
							r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
							//system("pause");
						}
						else if (r.start_end_routes[0][1] >= 1 && r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] >= 1) {
							//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
							//r.start_end_routes[0][1] --;
							//r.start_end_routes[2][0] --;
							r.start_end_routes[1][0] = -1;
							r.start_end_routes[1][1] = -1;
							r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
							//system("pause");
						}
						else {
							//cout << " Caso in cui abbiamo solo gionro 2 e 3 " << endl;
							r.start_end_routes[0][0] = -1;
							r.start_end_routes[0][1] = -1;
							r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
							//system("pause");
							//r.start_end_routes[2][0] --;
						}
						//cout << " DONE " << endl;
						//r.Print();
						//system("pause");
					}
					else if (time <= limit) {
						if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1) {
							//cout << " Caso del giorno 0 ho giorno 2 " << endl;
							//r.Print();
							//system("pause");
							r.start_end_routes[0][1] ++;
							r.start_end_routes[1][0] ++;
							//r.Print();
							//system("pause");
						}
						else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1) {
							//cout << " Caso del giorno 0 non ho giorno 2 " << endl;
							r.start_end_routes[0][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else {
							//cout << " Caso che pesno dal giorno 2 e metto in 3 " << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						r.time_arr[node_destroy + 1] = time;
						//r.Print();
						double limite = start_day1;
						if (r.time_dep[node_destroy + 1] > end_day2) limite = start_day3;
						else if (r.time_dep[node_destroy + 1] > end_day1) limite = start_day2;
						//cout << " Nodo in causa " << r.places[node_destroy + 1] << " limite " << limite << endl;
						if (!(1 == r.places[node_destroy + 1] || 4 == r.places[node_destroy + 1])) if (r.time_dep[node_destroy + 1] < limite) {
							//cout << " Tempo nodo incriminato " << r.time_dep[node_destroy + 1] << " limite " << limite << " giorno " << giorno << endl;
							//r.Print();
							//cin.get();
							goback = true;
						}
					}
					else {
						time = r.time_arr[node_destroy + 1] - ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
						//cout << " Punto di snodo diventa il nodo prima di quello distrutto  riga 6998" << endl;
						//cout << " Siamo considerando il giorno " << giorno << endl;
						//cout << " In nodo destroy ho: " << r.places[node_destroy] << endl;
						//r.Print();
						if (!(1 == r.places[node_destroy] || 4 == r.places[node_destroy])) {
							//cout << " Caso in cui devi stare molto attento... node:  " << node_destroy << endl;
							//cout << "  r.start_end_routes[0][1]: " << r.start_end_routes[0][1] << "  r.start_end_routes[1][1] " << r.start_end_routes[1][1] << "  r.start_end_routes[2][1]: " << r.start_end_routes[2][1] << endl;
							if (node_destroy == r.start_end_routes[0][1] && time < start_day2) {
								//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << time << endl;
								if ((r.time_arr[r.start_end_routes[1][1]] + (start_day2 - time)) <= end_day2) {
									r.time_dep[node_destroy] = start_day2;
									//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
									for (int a = node_destroy + 1; a < r.start_end_routes[1][1]; a++) {
										r.time_arr[a] += (start_day2 - time);
										r.time_dep[a] += (start_day2 - time);
									}
									r.time_arr[r.start_end_routes[1][1]] += (start_day2 - time);
									if (r.index == (r.start_end_routes[1][1] + 1)) r.time_dep[r.start_end_routes[1][1]] += (start_day2 - time);
								}
								else goback = true;
								//cout << " Perche: " << r.time_arr[r.start_end_routes[1][1]] << "+" << start_day2 << "-" << time << " <= " << end_day2 << endl;
								//r.Print();
								//system("pause");
							}
							else if (node_destroy == r.start_end_routes[2][0] && time < start_day3) {
								//cout << " CAso 2 in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
								if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
									r.time_dep[node_destroy] = start_day3;
									//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
									for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
										r.time_arr[a] += (start_day3 - time);
										r.time_dep[a] += (start_day3 - time);
									}
									r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
									if (r.index == (r.start_end_routes[2][1] + 1)) r.time_dep[r.start_end_routes[2][1]] += (start_day3 - time);
								}
								else goback = true;
								//cout << " Perche: " << r.time_arr[r.start_end_routes[2][1]] << "+" << start_day3 << "-" << time << " <= " << end_day3 << endl;
								//r.Print();
								//system("pause");
							}
							else {
								//cout << " CAso 3: Linea 7147 in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
								r.time_dep[node_destroy] = time;
								/*cout << " CAso 3: Linea 7147 in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
								r.Print();
								cin.get();
								if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
								r.time_dep[node_destroy] = start_day3;
								cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
								for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
								r.time_arr[a] += (start_day3 - time);
								r.time_dep[a] += (start_day3 - time);
								}
								r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
								if (r.index == r.time_arr[r.start_end_routes[2][1]] + 1)r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
								}
								else {
								goback = true;
								cout << " Perche: " << r.time_arr[r.start_end_routes[2][1]] << "+" << start_day3 << "-" << time << " <= " << end_day3 << endl;
								}*/

							}
						}
						else r.time_dep[node_destroy] = time;
						//r.Print();
						//cout << " CASOOOOOOOOOOOOOOOOOOOOOOO CHE DEVIIIIIIIIIIIIII VALUTARE COME TRATTARE " << endl;
						//system("pause");
					}
				}
				//system("pause");
				//cout << " Qui sto per andare a rimuovere archi vuoti " << endl;
				r.removePlace_Arc(map_airplane);
				//cout << " Qui ho rimosso archi vuoti  " << endl;

			}

			// -------------------------------------------------------------------   ORA Ho Casistiche su chi sia nuovo nodo di mezzo -------------------------------------------------------------------


		}
		else {
			//cout << "  ---------->  Caso in cui nel nodo di mezzo faccio --------------> refuel " << endl;
			//cout << " Stampo la route chhe adro poi a modificare " << endl;
			int index_fuel_before = node_destroy;
			int index_fuel_after = node_destroy;
			for (int i = node_destroy - 1; i >= 0; i--) {
				if (r.refueling[i]) { //&& i != node_destroy
					index_fuel_before = i;
					break;
				}
			}
			for (int i = node_destroy + 1; i < r.index; i++) {
				if (r.refueling[i]) { //&& i != node_destroy
					index_fuel_after = i;
					break;
				}
			}
			//cout << " Nodo: " << node_destroy << " Fuel prima " << index_fuel_before << " Fuel dopo : " << index_fuel_after << endl;
			//r.Print();
			double fuel1 = r.quantity_fuel[node_destroy - 1] - from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
			double diff = r.quantity_fuel[node_destroy + 1] - fuel1;

			if (r.quantity_fuel[index_fuel_after - 1] - diff >= (map_airplane[r.aircraft_code].min_fuel + from_to_FuelConsumed[r.aircraft_code][r.places[index_fuel_after - 1]][r.places[index_fuel_after]])) {
				//cout << "  ------------- Comincio codice di rimozione  ------------------------" << endl;
				//r.Print();
				//system("pause");
				// --------------------------------------------------------------------------------Inizio codiche che deve togliere il nodo -------------------------------------------------------------------
				double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
				if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
					//cout << " Ok è definitivamente un nodo che puo essere rimosso " << endl;
					vector<int> int_removed;
					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
							int_removed.push_back(p);
							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t]--;
								r.weight[t] += r.passengers_in_route[p].weight;
							}
						}
					}

					//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
					//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
					//cout << " Adesso sto per rimuovere i passeggieri " << endl;
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						//*************************************************************************************
						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}
					r.update_route_destroy_middleNode(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
					int index_before = node_destroy - 1;
					//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
					//r.print();
					//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
					double diff = 0;
					//cout << " Index before: " << index_before << endl;
					for (int i = node_destroy + 1; i < r.index; i++) {
						//cout << " sono al i = " << i << endl;
						if (r.refueling[i]) break;
						//cout << "weight before: " << r.weight[i] << endl;
						if (index_before == (node_destroy - 1)) {
							//cout << " sono al primo giro faccio " << endl;
							diff = r.quantity_fuel[i];
							//cout << " diff: " << diff << endl;
							r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
							diff = diff - r.quantity_fuel[i];
							//cout << " diff aggiornata: " << diff << endl;

						}
						else {
							//cout << " non sono al primo giro " << endl;
							r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
							//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
							//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
						}

						//cout << " weight prima " << r.weight[i] << endl;
						r.weight[i] = r.weight[i] + diff;
						//cout << " weight dopo " << r.weight[i] << endl;



						//cout << "weight after: " << r.weight[i] << endl;
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
						//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

						//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
						//cout << " inidce before = " << index_before << endl;
						index_before = i + 1;
						//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
					}
					//*****************************************************************************************************************************************************************


					r.removePlace_New(node_destroy, map_airplane);
					//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {
							//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
									index_refueling = i;
									break;
								}
							}
							//cout << " Aggiorno da " << index_refueling << endl;
							for (int t = index_refueling; t < r.index; t++) {
								//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
								if (r.refueling[t] && t != index_refueling) break;
								//cout << " Ho passato if aggiorno nodo " << t << endl;
								//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
								//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
							}
						}
					}


				}

				// -------------------------------------------------------------------   ORA Ho Casistiche su chi sia nuovo nodo di mezzo -------------------------------------------------------------------
				node_destroy--;
				//r.Print();
				//system("pause");
				if (r.places[node_destroy] == r.places[node_destroy + 1] && r.index > (node_destroy - 1)) {
					//cout << " Caso !: in cui ho nodi doppi perchè " << r.places[node_destroy] << "==" << r.places[node_destroy + 1] << endl;
					/// NUOVS PARTE DEL CODICE
					r.time_arr[node_destroy + 1] = r.time_arr[node_destroy];
					/// FINITA NUOVA PARTE DEL CODICE

					goback = aggregate_same_nodes_destoy_rolling(r, node_destroy);

					//cout << " Adesso devo mettere a posto gli indici dopo aggregazione dei nodi giorno: " << giorno << endl;
					//cout << " Node: " << node_destroy << endl;
					//cout << " 01 " << r.start_end_routes[0][1] << " 11 " << r.start_end_routes[1][1] << endl;
				
					if (node_destroy == r.start_end_routes[0][1] + 1 || node_destroy == r.start_end_routes[1][1] + 1 || node_destroy == r.start_end_routes[2][1] + 1) {
						//if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
						//cout << " ---------------------- Sono alla Riga 10233 --------------------------" << endl;
						//r.Print();
						if (giorno == 0 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] >= -1) {
							//cout << " Caso del giorno 1 ho giorno 2 " << endl;
							///prendo da giorno 2 metto in 1
							//r.Print();
							//system("pause");
							//cout << " Per incrementare deve valere ... " << r.start_end_routes[1][0] << " != 0 || " << r.start_end_routes[0][1] << " > -1" << endl;
							if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) {
								//cout << " Puoi Farlo ..... " << endl;
								r.start_end_routes[1][0] ++;
							}
							r.start_end_routes[0][1] ++;
							//r.Print();
							//system("pause");
						}
						else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] >= -1) {
							//cout << " Caso del giorno 1 non ho giorno 2" << endl;
							///prendo da giorno 3 metto in 1
							//r.Print();
							if (r.start_end_routes[2][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[2][0] ++;
							r.start_end_routes[0][1] ++;
							//r.Print();
							//system("pause");
						}
						else if (r.start_end_routes[1][1] >= -1 && giorno == 1 && r.primo_pass[1] == 1) {
							//cout << " Caso dove 3 da a 2  " << endl;
							///prendo da giorno 3 metto in 1
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else if (r.start_end_routes[1][0] == r.start_end_routes[1][1] && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][0] != -1 && r.time_arr[node_destroy] < end_day1) {
							//cout << " Caso molto speciale riga 7651 lo hai aggiunto per la casistica scoperta dopo aver commentato " << endl;
							if (r.time_arr[node_destroy] > start_day2) {
								//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
								//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
								//r.Print();
								//cin.get();
								r.start_end_routes[1][1] ++;
								r.start_end_routes[2][0] ++;
							}
							else {
								//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
								//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
								//r.Print();
								//cin.get();
								r.start_end_routes[0][1] ++;
								r.start_end_routes[2][0] ++;
							}
						}
						else if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && giorno == 1 && r.primo_pass[1] == 0) {
							//cout << " Caso dove 3 da a 2 super perticolare " << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else if (r.start_end_routes[1][0] == r.start_end_routes[1][1] && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][0] == -1) {
							//cout << " CAso in cui Primo giorno non ho nulla // Secondo GG un nodo // terzo giorno ho nodi // porta nodo da 2 a 3" << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else if (r.start_end_routes[1][0] == r.start_end_routes[1][1] && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][0] != -1 && r.time_arr[node_destroy] > end_day1) {
							//cout << " Caso molto speciale riga 7651 lo hai aggiunto per la casistica scoperta dopo aver commentato " << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}

						//r.Print();
						if (r.index > 2) {
							//cout << " sono alla linea 7687 " << endl;
							// oppure metto un caso qui dove anche se la differenza è zero ho che i tempi favoriscono lo spostamento perche la departure va in 2
							if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
								//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
								//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
								r.start_end_routes[1][0] --;
								r.start_end_routes[0][0] = -1;
								r.start_end_routes[0][1] = -1;
								r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
								r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
								//r.Print();
								//system("pause");
							}
							else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
								//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
								//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
								r.start_end_routes[2][0] --;
								r.start_end_routes[1][0] = -1;
								r.start_end_routes[1][1] = -1;
								r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
								r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
								//r.Print();
								//system("pause");
							}
							else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
								//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
								//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
								r.start_end_routes[2][0] --;
								r.start_end_routes[0][0] = -1;
								r.start_end_routes[0][1] = -1;
								r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
								r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
								r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
								//r.Print();
								//system("pause");
							}
						}



					}

					//r.Print();
				}
				else if (r.index > (node_destroy - 1)) {
					//cout << " Adesso devo valutare se riesco ad andarci al sera " << endl;
					double time = r.time_dep[node_destroy] + ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
					int limit = 0;
					if (giorno == 0) limit = end_day1;
					else if (giorno == 1) limit = end_day2;
					else limit = end_day3;
					//cout << " CI arriverebbe alle  " << time << " quando c'è un end day " << limit << endl;
					//r.Print();
					if (node_destroy == 0 && r.index > 2 ) {
						//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
						//r.Print();
						//system("pause");
						if (r.start_end_routes[2][0] == -1 && r.start_end_routes[1][1] >= 2) {
							//cout << " Caso in cui h route del giorno 1  e 2  e non del giorno 3 " << endl;
							//r.start_end_routes[0][1] --;
							//r.start_end_routes[1][0] --;
							r.start_end_routes[2][0] = -1;
							r.start_end_routes[2][1] = -1;
							r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
							//system("pause");
						}
						else if (r.start_end_routes[0][1] >= 1 && r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] >= 1) {
							//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
							//r.start_end_routes[0][1] --;
							//r.start_end_routes[2][0] --;
							r.start_end_routes[1][0] = -1;
							r.start_end_routes[1][1] = -1;
							r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
							//system("pause");
						}
						else {
							//cout << " Caso in cui abbiamo solo gionro 2 e 3 " << endl;
							r.start_end_routes[0][0] = -1;
							r.start_end_routes[0][1] = -1;
							r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
							//system("pause");
							//r.start_end_routes[2][0] --;
						}
						//cout << " DONE " << endl;
						//r.Print();
						//system("pause");
					}
					else if (time <= limit) {
						if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1) {
							//cout << " Caso del giorno 0 ho giorno 2 " << endl;
							//r.Print();
							//system("pause");
							r.start_end_routes[0][1] ++;
							r.start_end_routes[1][0] ++;
							//r.Print();
							//system("pause");
						}
						else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1) {
							//cout << " Caso del giorno 0 non ho giorno 2 " << endl;
							r.start_end_routes[0][1] ++;
							r.start_end_routes[2][0] ++;
						}
						else {
							//cout << " Caso che pesno dal giorno 2 e metto in 3 " << endl;
							r.start_end_routes[1][1] ++;
							r.start_end_routes[2][0] ++;
						}
						r.time_arr[node_destroy + 1] = time;
						//r.Print();
						double limite = start_day1;
						if (r.time_dep[node_destroy + 1] > end_day2) limite = start_day3;
						else if (r.time_dep[node_destroy + 1] > end_day1) limite = start_day2;
						//cout << " Nodo in causa " << r.places[node_destroy + 1] << " limite " << limite << endl;
						if (!(1 == r.places[node_destroy + 1] || 4 == r.places[node_destroy + 1])) if (r.time_dep[node_destroy + 1] < limite) {
							//cout << " Tempo nodo incriminato " << r.time_dep[node_destroy + 1] << " limite " << limite << " giorno " << giorno << endl;
							//r.Print();
							//cin.get();
							goback = true;
						}
					}
					else {
						time = r.time_arr[node_destroy + 1] - ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
						//cout << " Punto di snodo diventa il nodo prima di quello distrutto  riga 6998" << endl;
						//cout << " Siamo considerando il giorno " << giorno << endl;
						//cout << " In nodo destroy ho: " << r.places[node_destroy] << endl;
						//r.Print();
						if (!(1 == r.places[node_destroy] || 4 == r.places[node_destroy])) {
							//cout << " Caso in cui devi stare molto attento... node:  " << node_destroy << endl;
							//cout << "  r.start_end_routes[0][1]: " << r.start_end_routes[0][1] << "  r.start_end_routes[1][1] " << r.start_end_routes[1][1] << "  r.start_end_routes[2][1]: " << r.start_end_routes[2][1] << endl;
							if (node_destroy == r.start_end_routes[0][1] && time < start_day2) {
								//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << time << endl;
								if ((r.time_arr[r.start_end_routes[1][1]] + (start_day2 - time)) <= end_day2) {
									r.time_dep[node_destroy] = start_day2;
									//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
									for (int a = node_destroy + 1; a < r.start_end_routes[1][1]; a++) {
										r.time_arr[a] += (start_day2 - time);
										r.time_dep[a] += (start_day2 - time);
									}
									r.time_arr[r.start_end_routes[1][1]] += (start_day2 - time);
									if (r.index == (r.start_end_routes[1][1] + 1)) r.time_dep[r.start_end_routes[1][1]] += (start_day2 - time);
								}
								else goback = true;
								//cout << " Perche: " << r.time_arr[r.start_end_routes[1][1]] << "+" << start_day2 << "-" << time << " <= " << end_day2 << endl;
								//r.Print();
								//system("pause");
							}
							else if (node_destroy == r.start_end_routes[2][0] && time < start_day3) {
								//cout << " CAso 2 in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
								if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
									r.time_dep[node_destroy] = start_day3;
									//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
									for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
										r.time_arr[a] += (start_day3 - time);
										r.time_dep[a] += (start_day3 - time);
									}
									r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
									if (r.index == (r.start_end_routes[2][1] + 1)) r.time_dep[r.start_end_routes[2][1]] += (start_day3 - time);
								}
								else goback = true;
								//cout << " Perche: " << r.time_arr[r.start_end_routes[2][1]] << "+" << start_day3 << "-" << time << " <= " << end_day3 << endl;
								//r.Print();
								//system("pause");
							}
							else {
								//cout << " CAso 3: Linea 7147 in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
								r.time_dep[node_destroy] = time;
								/*cout << " CAso 3: Linea 7147 in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
								r.Print();
								cin.get();
								if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
								r.time_dep[node_destroy] = start_day3;
								cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
								for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
								r.time_arr[a] += (start_day3 - time);
								r.time_dep[a] += (start_day3 - time);
								}
								r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
								if (r.index == r.time_arr[r.start_end_routes[2][1]] + 1)r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
								}
								else {
								goback = true;
								cout << " Perche: " << r.time_arr[r.start_end_routes[2][1]] << "+" << start_day3 << "-" << time << " <= " << end_day3 << endl;
								}*/

							}
						}
						else r.time_dep[node_destroy] = time;
						//r.Print();
						//cout << " CASOOOOOOOOOOOOOOOOOOOOOOO CHE DEVIIIIIIIIIIIIII VALUTARE COME TRATTARE " << endl;
						//system("pause");
					}


					if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
						//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
						//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
						r.start_end_routes[1][0] --;
						r.start_end_routes[0][0] = -1;
						r.start_end_routes[0][1] = -1;
						r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
						//cout << r.time_dep[1] << endl;
						r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
						//cout << r.time_arr[1] << endl;
						r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
						//cout << r.time_dep[0] << endl;
						r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
						//r.Print();
						//cin.get();
					}

					if (r.start_end_routes[0][0] == -1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
						//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
						//cout << " sei nella nuova casistica comparsa durante lo studio gamberini " << endl;
						r.start_end_routes[2][0] --;
						r.start_end_routes[1][0] = -1;
						r.start_end_routes[1][1] = -1;
						r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
						//cout << r.time_dep[1] << endl;
						r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
						//cout << r.time_arr[1] << endl;
						r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
						//cout << r.time_dep[0] << endl;
						r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
						//r.Print();
						//cin.get();
					}

				}
				//cout << " Qui sto per andare a rimuovere archi vuoti " << endl;
				r.removePlace_Arc(map_airplane);
				//cout << " Qui ho rimosso archi vuoti  " << endl;
				//cout << " Finito procedura h fatto tutto per nodo centrale con refuel " << endl;
				//r.Print();
			}
			else {
				//cout << " ------------------!!!!! Non fare nulla !!!!!!!!!!!! -------------------------- " << endl;
			}

			//system("pause");
		}
	}

	//cout << " Qui ho funzione che azzera le flag dei passeggieri e mettee a posto start_end" << endl;
	bool check_Pass = false;
	//r.Print();
	for (int g = 0; g < 3; g++) {
		if ((r.start_end_routes[g][0] == r.start_end_routes[g][1]) && r.start_end_routes[g][0] != -1) {
			//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
			//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
			if (g != 0) r.start_end_routes[g][0] = -1;
			else r.start_end_routes[g][0] = 0;
			r.start_end_routes[g][1] = -1;
		}


		if (r.primo_pass[g] == 1) {
			check_Pass = false;
			for (Passenger& p : r.passengers_in_route) {
				if (p.giorno == (g + 1)) {
					check_Pass = true;
					break;
				}

			}

			if (!check_Pass) r.primo_pass[g] = 0;

		}


	}
	//cout << " -----------------  Ho finito la distruzione della route del cleaning  -------------------------- " << endl;
	if (goback) {
		//system("pause");
		//cin.get();
		int now = passenger_removed.size();
		//cout << " Passeggieri rimossi fino a questo momento " << to_string(so_far) << endl;
		//cout << " Passeggieri rimossi Ora: " << to_string(now) << endl;
		//cout << "---------------------------> Route ora ha numero di passeggieri " << r.passengers_in_route.size() << endl;
		//cout << " Route dell'aereo: " << r.aircraft_code << endl;
		//cout << " La Route modificata contine " << r.passengers_in_route.size() << endl;
		//cout << " La Route modificata contine " << old.passengers_in_route.size() << endl;
		r = old;
		if (r.passengers_in_route.size() < old.passengers_in_route.size()) {
			cout << " ---------------------------->>>>>>>>>> BUGOOOO L CLEANING SI è MANGIATO DEI PASSEGGIERI <<<<<<<<<---------------------------------------------- " << endl;
			cin.get();
		}
	}
	//cout << " ora dovresti uscire dalla funzione per andre a fare altri nodi di questa route " << endl;

}

vector<Route> destroy_cluster_aggr2(map<string, int>& legenda, double start_day1, double start_day2, double start_day3, int end_day1, int end_day2, int end_day3, int num_passenger, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<Passenger> all_passenegr, map<int, Passenger>& map_id_passenger, double& peso_itermediate_stop, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	//cout << "---------------<>>>>>>>>>>>>>>>>>>>>>>><< SONO dentro alla funzione di distruzione del CLUSTER <<<<<<<<<<<<<<<<<---------------------------" << endl;
	//for (Route r : solution) r.Print();
	int soglia_relateness = 100; // Prima era 100
	vector<int> int_removed;
	vector<Route> route_destroyed;
	vector<Passenger> passengers;
	//srand(time(NULL));
	for (Route& r : solution) route_destroyed.push_back(r);
	for (Passenger& p : all_passenegr) passengers.push_back(p);
	unordered_map<int, double> CostTWPass;
	set<double, MyCOMP<double>> Myset;
	for (Passenger& p : passengers) CostTWPass.insert(make_pair(p.pnr, 0));
	for (Route& s : route_destroyed) {
		for (Passenger& pass : s.passengers_in_route) {
			int Codpass = pass.pnr;
			CostTWPass[Codpass] += cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, map_airstrip);
			Myset.insert(cost__for_route_passenger_destroyCluster(s, pass, peso_itermediate_stop, map_airstrip));
			//cout << " con un costo della TW ---> " << cost_time_windows_for_route_passenger(s, pass) << endl;
		}
	}
	vector<string> OrderVectore;
	for (auto x : Myset) {
		for (auto y : CostTWPass) {
			if (y.second == x) {
				OrderVectore.push_back(to_string(y.first) + "|" + to_string(x));
			}
		}
	}
	//for (auto x : OrderVectore) cout << " ID: " << split(x, '|')[0] << " con costo: " << split(x, '|')[1] << endl;
	map<string, vector<int>> agr_pass;
	vector<string> sequenza;
	for (string x : OrderVectore) {
		//cout << " Devo inserire dentro la mappa che non dovrebbe essere ordinata " << x << endl;
		string code = split(x, '|')[1] + "|" + to_string(map_id_passenger[stoi(split(x, '|')[0])].departure_location) + "|" + to_string(map_id_passenger[stoi(split(x, '|')[0])].arrival_location);
		if (agr_pass.find(code) != agr_pass.end())
		{
			// c'è gia
			agr_pass[code].push_back(map_id_passenger[stoi(split(x, '|')[0])].pnr);
		}
		else {
			// non c'è gia
			sequenza.push_back(code);
			vector<int> id_passeggieri;
			id_passeggieri.push_back(map_id_passenger[stoi(split(x, '|')[0])].pnr);
			agr_pass.insert(make_pair(code, id_passeggieri));
		}

	}



	for (int x = 0; x < sequenza.size(); x++) {
		int Codpass = agr_pass[sequenza[x]][0];
		//cout << " Adesso considero String CODPASS da passare alla relateness: " << Codpass << endl;
		//cout << " Tolgo tutti i passeggieri di: " << sequenza[x] << endl;
		// tolgo tutti di questa aggregazione
		for (int p : agr_pass[sequenza[x]]) passenger_removed.push_back(map_id_passenger[p]);
		//cout << " Numero passeggieri rimossi = " << passenger_removed.size() << endl;
		agr_pass.erase(agr_pass.find(sequenza[x]));
		sequenza.erase(sequenza.begin() + x);
		//cout << " Numero di sequenze e grurroi  = " << sequenza.size() << " e " << agr_pass.size()<<  endl;
		for (int y = 0; y < sequenza.size(); y++) {
			//cout << " Sto per chimare funzione relateness per il passeggiero " << i << endl;
			//passengers[i].print();
			// Qui adesso chimao la funzione Relateness !!
			//cout << " Adesso devo fare il conftonto con  "<< sequenza[y] << endl;
			bool retrocedi = relateness_passenger2(soglia_relateness, from_to, Codpass, map_id_passenger[agr_pass[sequenza[y]][0]], passenger_removed, agr_pass, sequenza, sequenza[y], y, map_id_passenger);
			if (retrocedi) y--;
			//cout << " Abbiamo Valore indice x: " << x << " e Y: " << y << " su: " << sequenza.size() << endl;
		}
		//cout << " Fin'ora ho tolto numero odi passeggieri ----> " << passenger_removed.size() << " su " << num_passenger << endl;
		x--;
		if (passenger_removed.size() >= num_passenger) break;
	}

	int NRimossi = 0;
	int Npass = 0;
	int Nroute = -1;
	//for (Route r : route_destroyed) Npass += r.passengers_in_route.size();
	// Devo toglierli dalla route
	for (Route& s : route_destroyed) {
		Nroute++;
		vector<int> int_removed;
		bool Update = false;
		int Min_From_Pass = s.index;
		int Max_To_Pass = -1;
		for (int p = 0; p < s.passengers_in_route.size(); p++) {
			for (Passenger& pass : passenger_removed) {
				//if (s.passengers_in_route[p].name == pass.name && s.passengers_in_route[p].surname == pass.surname && s.passengers_in_route[p].pnr == pass.pnr && s.passengers_in_route[p].code_flight == pass.code_flight && s.passengers_in_route[p].gender == pass.gender  && s.passengers_in_route[p].arrival_location == pass.arrival_location  && s.passengers_in_route[p].departure_location == pass.departure_location && s.passengers_in_route[p].weight == pass.weight) {
				if (s.passengers_in_route[p].pnr == pass.pnr) {
					Update = true;
					pass.route_before = Nroute;
					if (s.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = s.passengers_in_route[p].solution_from;
					if (s.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = s.passengers_in_route[p].solution_to;
					//cout << " devo rimuovere passeggiero dalla route dell'aereo " << s.aircraft_code << endl;
					//s.passengers_in_route[p].print();
					int_removed.push_back(p);
					for (int t = s.passengers_in_route[p].solution_from; t < s.passengers_in_route[p].solution_to; t++) {
						s.capacity[t]--;
						s.weight[t] += s.passengers_in_route[p].weight;
					}

				}
			}


		}
		for (int i = int_removed.size() - 1; i >= 0; i--) {
			//cout << int_removed[i] << endl;
			vector<Passenger>::iterator it;
			it = s.passengers_in_route.begin();
			s.passengers_in_route.erase(it + int_removed[i]);
			//cout << " Route adesso ha ---> " << s.passengers_in_route.size() << " passeggieri" << endl;
			//cout << " Stampo passeggieri dopo rimozione: " << endl;
			//for (Passenger passeggiero : s.passengers_in_route) passeggiero.print();
		}

		if (Update) {


			// Codice che da reduel ai nodi con refuel non massimo
			int index_min_from = Min_From_Pass;
			for (int i = Min_From_Pass; i >= 0; i--) {
				if (s.refueling[i]) {
					index_min_from = i;
					break;
				}
			}
			for (int k = index_min_from; k < Max_To_Pass; k++) {
				if (s.refueling[k] && s.quantity_fuel[k] < map_airplane[s.aircraft_code].max_fuel) { //&& k!= node_destroy
				//cout << " Sto valutando il caso del nodo " << k << endl;
					int index_updating_from = k;
					int index_updating_to = s.index;
					int Node_min = k;
					double min_weight = s.weight[k];
					//appena aggiunto
					for (int i = Max_To_Pass; i < s.index; i++) {
						if (s.refueling[i]) break;
						Max_To_Pass = i;
					}

					for (int i = k + 1; i <= Max_To_Pass; i++) {
						//cout << " Sto guardando il nodo " << i << endl;
						//if (!(weight[i] > 0 && quantity_fuel[i] == 998)) {
						//cout << " Ho passato IF di controllo " << endl;
						if (s.refueling[i]) break;
						if (s.weight[i] < min_weight) {
							min_weight = s.weight[i];
							Node_min = i;
						}
						//}
					}
					if (Node_min >= 0) {
						for (int i = k + 1; i < s.index; i++) {
							if (s.refueling[i]) {  // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
								index_updating_to = i;
								break;
							}
						}
						//cout << " Minimo trovato " << min_weight << endl;
						//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
						//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
						double Fuel_before = s.quantity_fuel[index_updating_from];
						//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[aircraft_code].max_fuel << " e " << quantity_fuel[index_updating_from] + min_weight << endl;
						s.quantity_fuel[index_updating_from] = min(map_airplane[s.aircraft_code].max_fuel, s.quantity_fuel[index_updating_from] + min_weight);
						s.weight[index_updating_from] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						for (int i = index_updating_from + 1; i < index_updating_to; i++) {
							s.quantity_fuel[i] += (s.quantity_fuel[index_updating_from] - Fuel_before);
							s.weight[i] -= (s.quantity_fuel[index_updating_from] - Fuel_before);
						}
					}

				}

			}

			//cout << " Sono dentro la funzione di update adesso guardo se posso tirare via archi in fondo e guardo se devo fare le condizoni del Nell " << endl;
			//s.Print();
			s.removePlace_Arc(map_airplane);
			//cout << " Qui ho funzione che azzera le flag dei passeggieri e mettee a posto start_end" << endl;
			bool check_Pass = false;
			for (int g = 0; g < 3; g++) {
				if ((s.start_end_routes[g][0] == s.start_end_routes[g][1]) && s.start_end_routes[g][0] != -1) {
					//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
					//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
					//if (g != 0) r.start_end_routes[g][0] = -1;
					//else r.start_end_routes[g][0] = 0;
					s.start_end_routes[g][0] = -1;
					s.start_end_routes[g][1] = -1;
				}


				if (s.primo_pass[g] == 1) {
					check_Pass = false;
					for (Passenger& p : s.passengers_in_route) {
						if (p.giorno == (g + 1)) {
							check_Pass = true;
							break;
						}

					}

					if (!check_Pass) s.primo_pass[g] = 0;

				}


			}
			if (((s.start_end_routes[0][1] - s.start_end_routes[0][0] == 1 && s.primo_pass[0] == 0) || (s.start_end_routes[1][1] - s.start_end_routes[1][0] == 1 && s.primo_pass[1] == 0 && (s.start_end_routes[1][0] == 0))) && (s.index > 2)) {
				//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
				//s.Print();
				//cin.get();
				//cout << s.start_end_routes[0][1] << " - " << s.start_end_routes[0][0] << s.start_end_routes[1][1] << " - " << s.start_end_routes[1][0] << s.start_end_routes[2][1] << " - " << s.start_end_routes[2][0] << endl;
				if (s.start_end_routes[1][1] >= 2 && s.capacity[0] == 0) {
					//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
					//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
					if (s.start_end_routes[1][0] > 0)s.start_end_routes[1][0] --;
					s.start_end_routes[0][0] = -1;
					s.start_end_routes[0][1] = -1;
					s.time_dep[1] = s.time_arr[2] - ((from_to[s.places[1]][s.places[2]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[1] = s.time_dep[1] - map_airstrip[s.places[1]].ground_time;
					s.time_dep[0] = s.time_arr[1] - ((from_to[s.places[0]][s.places[1]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[0] = s.time_dep[0] - map_airstrip[s.places[0]].ground_time;


				}
				else if (s.start_end_routes[0][1] >= 1 && s.start_end_routes[1][1] == -1 && s.start_end_routes[2][1] >= 1 && s.capacity[0] == 0) {
					//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
					//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
					//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
					if (s.start_end_routes[2][0] > 0) s.start_end_routes[2][0] --;
					//s.start_end_routes[1][1] --;
					s.start_end_routes[0][0] = -1;
					s.start_end_routes[0][1] = -1;
					s.time_dep[1] = s.time_arr[2] - ((from_to[s.places[1]][s.places[2]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[1] = s.time_dep[1] - map_airstrip[s.places[1]].ground_time;
					s.time_dep[0] = s.time_arr[1] - ((from_to[s.places[0]][s.places[1]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[0] = s.time_dep[0] - map_airstrip[s.places[0]].ground_time;



				}
				else if ((s.start_end_routes[1][1] - s.start_end_routes[1][0]) == 1 && (s.start_end_routes[2][1] - s.start_end_routes[2][0]) >= 1 && s.capacity[0] == 0 && ((s.start_end_routes[0][1] - s.start_end_routes[0][0]) == 0 || ((s.start_end_routes[0][1] - s.start_end_routes[0][0]) == -1 && s.primo_pass[0] == 0))) {
					//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
					//cout << " Caso in cui abbiamo solo gionro da 2 a 3 " << endl;
					//s.start_end_routes[0][1] --;
					if (s.start_end_routes[2][0] > 0) s.start_end_routes[2][0] --;
					s.start_end_routes[1][0] = -1;
					s.start_end_routes[1][1] = -1;
					s.time_dep[1] = s.time_arr[2] - ((from_to[s.places[1]][s.places[2]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[1] = s.time_dep[1] - map_airstrip[s.places[1]].ground_time;
					s.time_dep[0] = s.time_arr[1] - ((from_to[s.places[0]][s.places[1]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[0] = s.time_dep[0] - map_airstrip[s.places[0]].ground_time;

				}
				else if (s.capacity[0] == 0) {
					s.time_dep[0] = s.time_arr[1] - ((from_to[s.places[0]][s.places[1]]) / map_airplane[s.aircraft_code].speed) * 60;
					s.time_arr[0] = s.time_dep[0] - map_airstrip[s.places[0]].ground_time;
				}
				//cout << " DONE " << endl;
				//cin.get();
				//s.Print();
			}
			//r.Print();
			for (int g = 0; g < 3; g++) {
				if ((s.start_end_routes[g][0] == s.start_end_routes[g][1]) && s.start_end_routes[g][0] != -1) {
					//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
					//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
					//if (g != 0) r.start_end_routes[g][0] = -1;
					//else r.start_end_routes[g][0] = 0;
					s.start_end_routes[g][0] = -1;
					s.start_end_routes[g][1] = -1;
				}


				if (s.primo_pass[g] == 1) {
					check_Pass = false;
					for (Passenger& p : s.passengers_in_route) {
						if (p.giorno == (g + 1)) {
							check_Pass = true;
							break;
						}

					}

					if (!check_Pass) s.primo_pass[g] = 0;

				}


			}
			//cout << " -----------------  Ho finito la distruzione della route  -------------------------- " << endl;
			//cout << " POSSO AVERE NODI IN MEZZO DA RIMUOVERE MA SICURAMENTE DEVO AVER SISTEMATO LE CONDIZIONI DEL NELl " << endl;
			//s.Print();
		}
		//cout << "stampo caso strano in destroy_cluster_aggr2" << endl;
		//stampo_caso_strano_single_destroy(legenda, map_airstrip, s, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, s);

	}


	//cout << " ------------------------------------------------------- MI PREPARO ALLA FASE DI CLEANING  ------------------------------------------------------------------- " << endl;

	Npass = 0;
	for (Route& s : route_destroyed) {
		Npass += (int)(s.passengers_in_route.size());
		if (s.passengers_in_route.size() == 0) s.primo_pass[0] = false;
	}
	//cout << " Numero di passeggieri nella route dopo averli toglierli " << Npass << endl;

	map<int, vector<int>> map_nodes;
	for (Route r : route_destroyed) {
		vector<int> nodes;
		map_nodes.insert(make_pair(r.aircraft_code, nodes));
		for (size_t j = 1; j < r.index - 1; j++) {
			if (r.capacity[j - 1] == 0 && r.capacity[j] == 0) {
				map_nodes[r.aircraft_code].push_back(j);
			}
		}
	}

	for (auto p : map_nodes) {
		//cout << " Aereo con codice " << p.first << endl;
		if (p.second.size() > 0) {
			//for (int i : p.second) cout << " Nodo Teorico: " << i << endl;
		}
		//cout << " -------------------" << endl;
	}

	for (Route& r : route_destroyed) {
		//r.Print();
		if (map_nodes[r.aircraft_code].size() > 0) {
			for (int i = map_nodes[r.aircraft_code].size() - 1; i >= 0; i--) {
				//cout << " Devo provare a togliere della Route " << r.aircraft_code << " con Size: " << r.index << " Nodo: " << map_nodes[r.aircraft_code][i] << endl;
				if (r.capacity[map_nodes[r.aircraft_code][i] - 1] == 0 && r.capacity[map_nodes[r.aircraft_code][i]] == 0) {
					//cout << " Autorizzato a rimuovere Nodo " << map_nodes[r.aircraft_code][i] << endl;
					cleaning_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, map_nodes[r.aircraft_code][i], passenger_removed, r, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				}//r.Print();
			}
		}
	}

	for (Route& r : route_destroyed) {
		if (r.time_arr[0] + map_airstrip[r.places[0]].ground_time < r.time_dep[0]) r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
		if (r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time < r.time_dep[r.index - 1]) {
			r.time_dep[r.index - 1] = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time;
		}

		if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) > 1 && r.primo_pass[1] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[1] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
		if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) > 1 && r.primo_pass[0] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[0] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
	}

	//cout << " ------------------------------------------------------- Route dopo aver tolto archi vuoti  ------------------------------------------------------------------- " << endl;

	return route_destroyed;

}


void destroy_casual_rolling(double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		Route old = r;
		bool goback = false;
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			//cout << "---------------------------> Voglio provare a distruggere seguente " << r.aircraft_code << endl;
			//cout << "---------------------------> Ha numero di passeggieri " << r.passengers_in_route.size() << endl;
			int so_far = passenger_removed.size();
			//cout << " Passeggieri rimossi fino a questo momento " << to_string(so_far) << endl;
			// r.Print();
			bool check = true;
			do {
				//if i'm here i can destroy the route
				//at the moment i destroy only a node
				goback = false;
				double n_destroy = (double)rand() / RAND_MAX;
				int node_destroy = (int)(round(1 + n_destroy * (r.index - 2)));
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					//cout << "nodo distrutto: " << node_destroy << endl;
					//r.print();
					vector<int> int_removed;
					//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
					//cout << " Questa route ha un index = " << r.index << endl;
					//cout << " siamo nel caso che il nodo distrutto sia l'ultimo " << endl;
					//r.print();

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t]--;
								r.weight[t] += r.passengers_in_route[p].weight;
							}


						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						r.passengers_in_route[int_removed[i]].route_before = index;
						//*************************************************************************************

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}
					// Qua devo mettere codice che è quello dell'Update
					/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
					cout << "******************* Stampiamo la Route: *******************" << endl;
					cout << " CODE ------> " << r.aircraft_code << endl;
					cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
					cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
					//r.print();
					cout << " Node destroyed ------> " << node_destroy << endl;
					*/

					int index_min_from;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
					// Codice che serve per cercare il minimo nel range
					//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								if (r.refueling[i]) break;
								//cout << " Ho passato il break " << endl;
								if (r.weight[i] < min_weight && i != node_destroy) {
									// cout << " Ho passato IF di controllo " << endl;
									min_weight = r.weight[i];
									Node_min = i;
								}
							}
							//cout << " Nodo di minimo ---> " << Node_min << endl;
							//cout << " Valore di minimi --> " << min_weight << endl;
							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
								//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
								double Fuel_before = r.quantity_fuel[index_updating_from];
								//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
								//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
								//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
								//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
					//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi quando tolgo ultimo nodo " << endl;
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {
							//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
									index_refueling = i;
									break;
								}
							}
							//cout << " Aggiorno da " << index_refueling << endl;
							for (int t = index_refueling; t < r.index; t++) {
								//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
								if (r.refueling[t] && t != index_refueling) break;
								//cout << " Ho passato if aggiorno nodo " << t << endl;
								//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
								//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
							}
						}
					}
				}
				else {
					int giorno = -1;
					if (node_destroy <= r.start_end_routes[0][1]) giorno = 0;
					else if (node_destroy <= r.start_end_routes[1][1]) giorno = 1;
					else giorno = 2;
					bool imp_ref = false;
					//cout << "Nodo: " << node_destroy << endl;
					if (node_destroy != (r.start_end_routes[0][1]) && node_destroy != (r.start_end_routes[1][1])) {
						//cout << " Ok non è ultimo nodo in nessuna giornata " << endl;
						//cout << " Index della Route: " << r.index << endl;
						//r.Print();
						//cout << " Nodo appartiene alla giornata " << giorno << endl;
						if (giorno != 2 && r.refueling[node_destroy] && !(r.refueling[r.start_end_routes[giorno][1]])) {
							int j = r.index - 1;
							imp_ref = true;
							for (int i = node_destroy + 1; i < r.index; i++) {
								if (r.refueling[i]) {
									j = i;
									break;
								}
							}
							double fuel1 = r.quantity_fuel[node_destroy - 1] - from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
							double diff = r.quantity_fuel[node_destroy + 1] - fuel1;
							if ((r.quantity_fuel[j - 1] - diff - from_to_FuelConsumed[r.aircraft_code][r.places[j - 1]][r.places[j]]) >= map_airplane[r.aircraft_code].min_fuel) imp_ref = false;
						}
						if (imp_ref) {
							//cout << " Qua dic eche è importante peche lo dice ?!?!? " << endl;
							//r.Print();
						}
						//cout << " Y/N nodo importante:  " << imp_ref << endl;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
						if (!imp_ref && (fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
							check = false;
							//cout << " Ok -----------> è definitivamente un nodo che puo essere rimosso ed non è un nodo in mezzo " << endl;
							vector<int> int_removed;
							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t]--;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}

							//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
							//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
							//cout << " Adesso sto per rimuovere i passeggieri " << endl;
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							r.update_route_destroy(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
							int index_before = node_destroy - 1;
							//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
							//r.print();
							//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
							double diff = 0;
							//cout << " Index before: " << index_before << endl;
							for (int i = node_destroy + 1; i < r.index; i++) {
								//cout << " sono al i = " << i << endl;
								if (r.refueling[i]) break;
								//cout << "weight before: " << r.weight[i] << endl;
								if (index_before == (node_destroy - 1)) {
									//cout << " sono al primo giro faccio " << endl;
									diff = r.quantity_fuel[i];
									//cout << " diff: " << diff << endl;
									r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
									diff = diff - r.quantity_fuel[i];
									//cout << " diff aggiornata: " << diff << endl;

								}
								else {
									//cout << " non sono al primo giro " << endl;
									r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
									//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								}

								//cout << " weight prima " << r.weight[i] << endl;
								r.weight[i] = r.weight[i] + diff;
								//cout << " weight dopo " << r.weight[i] << endl;



								//cout << "weight after: " << r.weight[i] << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

								//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
								//cout << " inidce before = " << index_before << endl;
								index_before = i + 1;
								//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
							}
							//*****************************************************************************************************************************************************************


							r.removePlace(node_destroy, map_airplane);
							//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
							double add_fuel = 0;
							int index_weight_neg = -1;
							for (int j = 0; j < r.index; j++) {
								if (r.weight[j] < 0) {
									//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
									add_fuel = r.weight[j];
									index_weight_neg = j;
									int index_refueling = index_weight_neg;
									for (int i = index_weight_neg; i >= 0; i--) {
										if (r.refueling[i]) { //&& i != node_destroy
											index_refueling = i;
											break;
										}
									}
									//cout << " Aggiorno da " << index_refueling << endl;
									for (int t = index_refueling; t < r.index; t++) {
										//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
										if (r.refueling[t] && t != index_refueling) break;
										//cout << " Ho passato if aggiorno nodo " << t << endl;
										//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
										r.quantity_fuel[t] += add_fuel;
										r.weight[t] -= add_fuel;
										//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
									}
								}
							}
							/*cout << " Sto per valutare casisitiche appena introdotte linea 7734" << endl;
							cout << "start_end_routes[0][0] " << r.start_end_routes[0][0] << endl;
							cout << "start_end_routes[0][1] " << r.start_end_routes[0][1] << endl;
							cout << "start_end_routes[1][0] " << r.start_end_routes[1][0] << endl;
							cout << "start_end_routes[1][1] " << r.start_end_routes[1][1] << endl;
							cout << "start_end_routes[2][0] " << r.start_end_routes[2][0] << endl;
							cout << "start_end_routes[2][1] " << r.start_end_routes[2][1] << endl;
							cout << "r.primo_pass[0] " << r.primo_pass[0] << endl;
							cout << "r.primo_pass[1] " << r.primo_pass[1] << endl;
							cout << "r.primo_pass[2] " << r.primo_pass[2] << endl;
							cout << " r.index: " << r.index << endl;*/
							if (r.index > 2) {
								//cout << " sono alla linea 7746 " << endl;
								if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
									//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
									//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
									r.start_end_routes[1][0] --;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[0][1] = -1;
									r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
									r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									//r.Print();
									//system("pause");
								}
								else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
									//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
									//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
									r.start_end_routes[2][0] --;
									r.start_end_routes[1][0] = -1;
									r.start_end_routes[1][1] = -1;
									r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
									r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									//r.Print();
									//system("pause");
								}
								else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
									//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
									//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
									r.start_end_routes[2][0] --;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[0][1] = -1;
									r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
									r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									//r.Print();
									//system("pause");
								}
							}
							else if (r.index == 2 && r.capacity[0] == 0) {
								//r.Print();
								//cout << " --------------------------------->>>>> CASO DA VALUTARE  <<<<<----------------------------------------------------" << endl;
								//cout << " --------------------------------->>>>> ENTRO  RIGA 779   <<<<<----------------------------------------------------" << endl;
								r.places.erase(r.places.begin() + 1);
								r.time_arr.erase(r.time_arr.begin() + 1);
								r.time_dep.erase(r.time_dep.begin() + 1);
								r.refueling.erase(r.refueling.begin() + 1);
								r.quantity_fuel.erase(r.quantity_fuel.begin() + 1);
								r.weight.erase(r.weight.begin() + 1);
								r.capacity.erase(r.capacity.begin() + 1);
								r.index = r.index - 1;
								if (r.start_end_routes[0][1] != r.start_end_routes[0][0]) {
									r.start_end_routes[0][1] = -1;
									r.start_end_routes[0][0] = 0;
									r.start_end_routes[1][1] = -1;
									r.start_end_routes[1][0] = -1;
									r.start_end_routes[2][1] = -1;
									r.start_end_routes[2][0] = -1;
								}
								else if (r.start_end_routes[1][1] != r.start_end_routes[1][0]) {
									r.start_end_routes[0][1] = -1;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[1][1] = -1;
									r.start_end_routes[1][0] = 0;
									r.start_end_routes[2][1] = -1;
									r.start_end_routes[2][0] = -1;
								}
								else {
									r.start_end_routes[0][1] = -1;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[1][1] = -1;
									r.start_end_routes[1][0] = -1;
									r.start_end_routes[2][1] = -1;
									r.start_end_routes[2][0] = 0;
								}
							}

						}
					}
					else if (!r.refueling[node_destroy]) {
						//cout << " ---------------------> Caso che tolgo nodo Di collegamento in cui NON devo fare refuel" << endl;
						//r.Print();
						//system("pause");
						// --------------------------------------------------------------------------------Inizio codiche che deve togliere il nodo -------------------------------------------------------------------
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
						if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
							check = false;
							//cout << " Ok è definitivamente un nodo che puo essere rimosso " << endl;
							vector<int> int_removed;
							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t]--;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}

							//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
							//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
							//cout << " Adesso sto per rimuovere i passeggieri " << endl;
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							r.update_route_destroy_middleNode(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
							int index_before = node_destroy - 1;
							//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
							//r.print();
							//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
							double diff = 0;
							//cout << " Index before: " << index_before << endl;
							for (int i = node_destroy + 1; i < r.index; i++) {
								//cout << " sono al i = " << i << endl;
								if (r.refueling[i]) break;
								//cout << "weight before: " << r.weight[i] << endl;
								if (index_before == (node_destroy - 1)) {
									//cout << " sono al primo giro faccio " << endl;
									diff = r.quantity_fuel[i];
									//cout << " diff: " << diff << endl;
									r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
									diff = diff - r.quantity_fuel[i];
									//cout << " diff aggiornata: " << diff << endl;

								}
								else {
									//cout << " non sono al primo giro " << endl;
									r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
									//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								}

								//cout << " weight prima " << r.weight[i] << endl;
								r.weight[i] = r.weight[i] + diff;
								//cout << " weight dopo " << r.weight[i] << endl;



								//cout << "weight after: " << r.weight[i] << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

								//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
								//cout << " inidce before = " << index_before << endl;
								index_before = i + 1;
								//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
							}
							//*****************************************************************************************************************************************************************


							r.removePlace_New(node_destroy, map_airplane);
							//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
							double add_fuel = 0;
							int index_weight_neg = -1;
							for (int j = 0; j < r.index; j++) {
								if (r.weight[j] < 0) {
									//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
									add_fuel = r.weight[j];
									index_weight_neg = j;
									int index_refueling = index_weight_neg;
									for (int i = index_weight_neg; i >= 0; i--) {
										if (r.refueling[i]) { //&& i != node_destroy
											index_refueling = i;
											break;
										}
									}
									//cout << " Aggiorno da " << index_refueling << endl;
									for (int t = index_refueling; t < r.index; t++) {
										//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
										if (r.refueling[t] && t != index_refueling) break;
										//cout << " Ho passato if aggiorno nodo " << t << endl;
										//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
										r.quantity_fuel[t] += add_fuel;
										r.weight[t] -= add_fuel;
										//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
									}
								}
							}

							// -------------------------------------------------------------------   ORA Ho Casistiche su chi sia nuovo nodo di mezzo -------------------------------------------------------------------
							node_destroy--;
							//cout << " Fintio di togliere il nodo adesso valuto casistiche " << endl;
							//r.Print();
							//system("pause");
							if (r.places[node_destroy] == r.places[node_destroy + 1] && r.index > (node_destroy - 1)) {
								//cout << " Caso !: in cui ho nodi doppi perchè " << r.places[node_destroy] << "==" << r.places[node_destroy + 1] << endl;
								/// NUOVS PARTE DEL CODICE
								r.time_arr[node_destroy + 1] = r.time_arr[node_destroy];
								/// FINITA NUOVA PARTE DEL CODICE
								//cout << " Inizio codice di aggrregazione nodi doppi " << endl;
								goback = aggregate_same_nodes_destoy_rolling(r, node_destroy);
								//int gg = 2;
								//if (node_destroy <= r.start_end_routes[0][1]) gg = 0;
								//else if (node_destroy <= r.start_end_routes[1][1]) gg = 1;
								//else giorno = 2;
								/*cout << " Adesso devo mettere a posto gli indici dopo aggregazione dei nodi giorno: " << giorno << endl;
								cout << " Finito codice aggre nodo " << node_destroy << endl;
								cout << "start_end_routes[0][0]" << r.start_end_routes[0][0] << endl;
								cout << "start_end_routes[0][1]" << r.start_end_routes[0][1] << endl;
								cout << "start_end_routes[1][0]" << r.start_end_routes[1][0] << endl;
								cout << "start_end_routes[1][1]" << r.start_end_routes[1][1] << endl;
								cout << "start_end_routes[2][0]" << r.start_end_routes[2][0] << endl;
								cout << "start_end_routes[2][1]" << r.start_end_routes[2][1] << endl;
								cout << " r.index: " << r.index << endl;
								cout << " Giorno: " << giorno << endl;*/
								// IF SOTTO LO HO APPENA MESSO NON SO SE VADA BENE --- è ANCORA IN FORSE
								if (node_destroy == r.start_end_routes[0][1] + 1 || node_destroy == r.start_end_routes[1][1] + 1 || node_destroy == r.start_end_routes[2][1] + 1) {
									if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
										//cout << " Caso del giorno 1 ho giorno 2 " << endl;
										///prendo da giorno 2 metto in 1
										//r.Print();
										//system("pause");
										r.start_end_routes[0][1] ++;
										if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
										//r.Print();
										//system("pause");
									}
									else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] > -1) {
										//cout << " Caso del giorno 1 non ho giorno 2" << endl;
										///prendo da giorno 3 metto in 1
										//r.Print();
										r.start_end_routes[0][1] ++;
										r.start_end_routes[2][0] ++;
										//r.Print();
										//system("pause");
									}
									else if (r.start_end_routes[1][1] >= -1 && r.primo_pass[1] == 1 && giorno == 1) {
										//cout << " Caso dove 2 da a 3  " << endl;
										///prendo da giorno 3 metto in 1
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
										if (r.time_arr[node_destroy] > start_day2) {
											//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
											//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
											//r.Print();
											//cin.get();
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else {
											//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
											//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
											//r.Print();
											//cin.get();
											r.start_end_routes[0][1] ++;
											r.start_end_routes[2][0] ++;
										}
									}
									else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] == -1 && (r.time_dep[r.start_end_routes[1][1]] < end_day2 && r.time_arr[r.start_end_routes[1][1] + 1] < end_day2)) {
										//cout << " Caso dove 2 da a 3 particolare con tempi " << endl;
										///prendo da giorno 3 metto in 1 --------------------->>>> QUA FORSE DEVI CAAMBIARE
										//r.Print();
										//cin.get();
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									//((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1)
									else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.time_arr[node_destroy] <= end_day2) {
										//cout << " Caso dove 2 da a 3 particolareggiante " << endl;
										///prendo da giorno 3 metto in 1
										//r.Print();
										//cin.get();
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									//r.Print();
									//cout << " Sto per valutare casisitiche richieste dal NEl dopo nodi doppi linea 8242 " << endl;
									if (r.index >= 2) {
										//cout << " sono alla linea 8193 " << endl;
										if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
											r.start_end_routes[1][0] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											//r.Print();
											//system("pause");
										}
										else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
											r.start_end_routes[2][0] --;
											r.start_end_routes[1][0] = -1;
											r.start_end_routes[1][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											//r.Print();
											//system("pause");
										}
										else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
											r.start_end_routes[2][0] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											//r.Print();
											//system("pause");
										}
									}
									//r.Print();
								}
							}
							else if (r.index > (node_destroy - 1)) {
								//cout << " Adesso devo valutare se riesco ad andarci al sera " << endl;
								double time = r.time_dep[node_destroy] + ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
								int limit = 0;
								if (giorno == 0) limit = end_day1;
								else if (giorno == 1) limit = end_day2;
								else limit = end_day3;
								//cout << " CI arriverebbe alle  " << time << " quando c'è un end day " << limit << endl;
								if (node_destroy == 0 && r.index > 2) {
									//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
									//r.Print();
									//system("pause");
									//r.Print();
									if (r.start_end_routes[2][0] == -1 && r.start_end_routes[1][1] >= 2 && r.capacity[0] == 0) {
										//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
										//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
										if (r.start_end_routes[1][0] > 0)r.start_end_routes[1][0] --;
										r.start_end_routes[0][0] = -1;
										r.start_end_routes[0][1] = -1;
										r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;


									}
									else if (r.start_end_routes[0][1] >= 1 && r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] >= 1 && r.capacity[0] == 0) {
										//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
										//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
										//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
										if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
										//r.start_end_routes[1][1] --;
										r.start_end_routes[0][0] = -1;
										r.start_end_routes[0][1] = -1;
										r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;



									}
									else if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
										//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
										//cout << " Caso in cui abbiamo solo gionro da 2 a 3 " << endl;
										//r.start_end_routes[0][1] --;
										if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
										r.start_end_routes[1][0] = -1;
										r.start_end_routes[1][1] = -1;
										r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;

									}
									else if (r.capacity[0] == 0) {
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									}
									//cout << " DONE " << endl;
									//r.Print();
								}
								else if (time <= limit) {

									if (giorno == 0 && (r.start_end_routes[1][0] > -1) && (r.start_end_routes[1][1] > -1)) {
										//cout << " Caso del giorno 0 ho giorno 2 " << endl;
										//r.Print();
										r.start_end_routes[0][1] ++;
										r.start_end_routes[1][0] ++;
										r.Print();
									}
									else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1) {
										//cout << " Caso del giorno 0 non ho giorno 2 " << endl;
										r.start_end_routes[0][1] ++;
										r.start_end_routes[2][0] ++;
									}
									else {
										//cout << " Caso che pesno dal giorno 3 e metto in 2 " << endl;
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									r.time_arr[node_destroy + 1] = time;
									double limite = start_day1;
									if (r.time_dep[node_destroy + 1] > end_day2) limite = start_day3;
									else if (r.time_dep[node_destroy + 1] > end_day1) limite = start_day2;
									//cout << " Nodo in causa " << r.places[node_destroy + 1] << " limite " << limite << endl;
									//cout << " Nodo in questione è nodo " << r.places[node_destroy + 1] << " limite: " << limite << " valore monitoraato: " << r.time_dep[node_destroy + 1] << endl;
									if (!(1 == r.places[node_destroy + 1] || 4 == r.places[node_destroy + 1])) if (r.time_dep[node_destroy + 1] < limite) {
										//cout << " Tempo nodo incriminato " << r.time_dep[node_destroy + 1] << " limite " << limite << " giorno " << giorno << endl;
										//r.Print();
										//cin.get();
										//system("pause");
										goback = true;
									}
									//r.Print();
								}
								else {
									time = r.time_arr[node_destroy + 1] - ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
									//cout << " Punto di snodo diventa il nodo prima di quello distrutto  riga 7993" << endl;
									//cout << " Siamo considerando il giorno " << giorno << endl;
									//r.Print();
									if (!(1 == r.places[node_destroy] || 4 == r.places[node_destroy])) {
										//cout << " Caso in cui devi stare molto attento... node:  " << node_destroy << endl;
										if (node_destroy == r.start_end_routes[0][1] && time < start_day2) {
											//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << time << endl;
											if ((r.time_arr[r.start_end_routes[1][1]] + (start_day2 - time)) <= end_day2) {
												r.time_dep[node_destroy] = start_day2;
												//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
												for (int a = node_destroy + 1; a < r.start_end_routes[1][1]; a++) {
													r.time_arr[a] += (start_day2 - time);
													r.time_dep[a] += (start_day2 - time);
												}
												r.time_arr[r.start_end_routes[1][1]] += (start_day2 - time);
												if (r.index == r.start_end_routes[1][1] + 1)r.time_dep[r.start_end_routes[1][1]] += (start_day2 - time);
											}
											else goback = true;
											//r.Print();
											//system("pause");
										}
										else if (node_destroy == r.start_end_routes[2][0] && time < start_day3) {
											//cout << " CAso in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
											if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
												r.time_dep[node_destroy] = start_day3;
												//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
												for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
													r.time_arr[a] += (start_day3 - time);
													r.time_dep[a] += (start_day3 - time);
												}
												r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
												if (r.index == r.start_end_routes[2][1] + 1)r.time_dep[r.start_end_routes[2][1]] += (start_day3 - time);
											}
											else goback = true;
											//r.Print();
											//system("pause");
										}
										else r.time_dep[node_destroy] = time;
									}
									else r.time_dep[node_destroy] = time;
									//r.Print();
									//cout << " CASOOOOOOOOOOOOOOOOOOOOOOO CHE DEVIIIIIIIIIIIIII VALUTARE COME TRATTARE " << endl;
									//system("pause");
								}
							}
							//system("pause");
							r.removePlace_Arc(map_airplane);
							// Codice che toglie archi vuoti dal fondo andrebbe messo qua

						}



					}
					else {
						//cout << "  ---------->  Caso in cui nel nodo di mezzo faccio --------------> refuel " << endl;
						//if (r.aircraft_code == "5H-RJS") r.Print();
						int index_fuel_before = node_destroy;
						int index_fuel_after = node_destroy;
						for (int i = node_destroy - 1; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_fuel_before = i;
								break;
							}
						}
						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_fuel_after = i;
								break;
							}
						}
						//cout << " Nodo: " << node_destroy << " Fuel prima " << index_fuel_before << " Fuel dopo : " << index_fuel_after << endl;
						//r.Print();
						double fuel1 = r.quantity_fuel[node_destroy - 1] - from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
						double diff = r.quantity_fuel[node_destroy + 1] - fuel1;
						/*
						cout << " Nice !!!!! It makes sense !!!!!!! " << endl;
						cout << " r.quantity_fuel[node_destroy-1] " << r.quantity_fuel[node_destroy - 1] << " -  from_to_FuelConsumed[r.aircraft_code + ;  + r.places[node_destroy - 1] + ;  + r.places[node_destroy + 1]] " << from_to_FuelConsumed[r.aircraft_code + ";" + r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] << endl;
						cout << " Fuel del nodo che sto distruffendo " << r.quantity_fuel[node_destroy+1] << endl;
						cout << " Nodo priam del primo fuel aveva fuel pari a " << r.quantity_fuel[index_fuel_after - 1] << " Adesso sara " << r.quantity_fuel[index_fuel_after - 1] - diff << endl;
						cout << " Che deve essere maggriore di sicurezza " << map_airplane[r.aircraft_code].min_fuel  <<" + tratto " << from_to_FuelConsumed[r.aircraft_code + ";" + r.places[index_fuel_after - 1] + ";" + r.places[index_fuel_after]] << endl;
						cout << " QUa biogna metterci codice della rimozione " << endl;
						// Fuel del nodo prima del nodo da distruggere
						// Toglici il fuel da li al nodo dopo il nodo distrutto
						//Prendi il fuel che c'era nel nodo che stai distruggendo
						//FAi fuel del nodo distrutto meno fuel calcoalto nei primi due punti = diff
						// prendi nodo prima del nodo_fuel_after tolgi diff
						// Guarda ancora se è sufficiente a fare arco dopo piu soglia minnima
						r.Print();
						system("pause");
						*/
						if (r.quantity_fuel[index_fuel_after - 1] - diff >= (map_airplane[r.aircraft_code].min_fuel + from_to_FuelConsumed[r.aircraft_code][r.places[index_fuel_after - 1]][r.places[index_fuel_after]])) {
							//cout << "  ------------- Comincio codice di rimozione  ------------------------" << endl;
							//r.Print();
							//system("pause");
							// --------------------------------------------------------------------------------Inizio codiche che deve togliere il nodo -------------------------------------------------------------------
							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
							if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
								check = false;
								//cout << " Ok nodo " << node_destroy << " NAme: " << r.places[node_destroy] << " è definitivamente un nodo che puo essere rimosso " << endl;
								vector<int> int_removed;
								int Min_From_Pass = node_destroy;
								int Max_To_Pass = node_destroy;
								for (int p = 0; p < r.passengers_in_route.size(); p++) {
									if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
										if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
										if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
										int_removed.push_back(p);
										for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
											r.capacity[t]--;
											r.weight[t] += r.passengers_in_route[p].weight;
										}
									}
								}

								//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
								//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
								//cout << " Adesso sto per rimuovere i passeggieri " << endl;
								for (int i = int_removed.size() - 1; i >= 0; i--) {
									// code for repair forbidden***********************************************************
									r.passengers_in_route[int_removed[i]].route_before = index;
									//*************************************************************************************
									passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
									vector<Passenger>::iterator it;
									it = r.passengers_in_route.begin();
									r.passengers_in_route.erase(it + int_removed[i]);
								}
								r.update_route_destroy_middleNode(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
								int index_before = node_destroy - 1;
								//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
								//r.print();
								//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
								double diff = 0;
								//cout << " Index before: " << index_before << endl;
								for (int i = node_destroy + 1; i < r.index; i++) {
									//cout << " sono al i = " << i << endl;
									if (r.refueling[i]) break;
									//cout << "weight before: " << r.weight[i] << endl;
									if (index_before == (node_destroy - 1)) {
										//cout << " sono al primo giro faccio " << endl;
										diff = r.quantity_fuel[i];
										//cout << " diff: " << diff << endl;
										r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
										diff = diff - r.quantity_fuel[i];
										//cout << " diff aggiornata: " << diff << endl;

									}
									else {
										//cout << " non sono al primo giro " << endl;
										r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
										//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
										//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									}

									//cout << " weight prima " << r.weight[i] << endl;
									r.weight[i] = r.weight[i] + diff;
									//cout << " weight dopo " << r.weight[i] << endl;



									//cout << "weight after: " << r.weight[i] << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

									//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
									//cout << " inidce before = " << index_before << endl;
									index_before = i + 1;
									//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
								}
								//*****************************************************************************************************************************************************************


								r.removePlace_New(node_destroy, map_airplane);
								//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weight[j] < 0) {
										//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
										add_fuel = r.weight[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.refueling[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}
										//cout << " Aggiorno da " << index_refueling << endl;
										for (int t = index_refueling; t < r.index; t++) {
											//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
											if (r.refueling[t] && t != index_refueling) break;
											//cout << " Ho passato if aggiorno nodo " << t << endl;
											//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
											r.quantity_fuel[t] += add_fuel;
											r.weight[t] -= add_fuel;
											//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
										}
									}
								}
								// -------------------------------------------------------------------   ORA Ho Casistiche su chi sia nuovo nodo di mezzo -------------------------------------------------------------------

								node_destroy--;
								//r.Print();
								//system("pause");
								if (r.places[node_destroy] == r.places[node_destroy + 1] && r.index > (node_destroy - 1)) {
									//cout << " Caso !: in cui ho nodi doppi perchè " << r.places[node_destroy] << "==" << r.places[node_destroy + 1] << " stampo route prima della aggregazione " << endl;
									//r.Print();
									/// NUOVS PARTE DEL CODICE
									r.time_arr[node_destroy + 1] = r.time_arr[node_destroy];
									/// FINITA NUOVA PARTE DEL CODICE
									//cout << " Inizio codice di aggrregazione nodi doppi " << endl;
									//cout << " Giorno: " << giorno << endl;
									goback = aggregate_same_nodes_destoy_rolling(r, node_destroy);
									/*cout << "start_end_routes[0][0]" << r.start_end_routes[0][0] << endl;
									cout << "start_end_routes[0][1]" << r.start_end_routes[0][1] << endl;
									cout << "start_end_routes[1][0]" << r.start_end_routes[1][0] << endl;
									cout << "start_end_routes[1][1]" << r.start_end_routes[1][1] << endl;
									cout << "start_end_routes[2][0]" << r.start_end_routes[2][0] << endl;
									cout << "start_end_routes[2][1]" << r.start_end_routes[2][1] << endl;
									cout << " Confrontato con Node Destroy " << node_destroy << endl;
									r.Print();*/
									if (node_destroy == r.start_end_routes[0][1] + 1 || node_destroy == r.start_end_routes[1][1] + 1 || node_destroy == r.start_end_routes[2][1] + 1) {
										// Questa dovrebbe essere la versione piu aggiornata nel casual -- Occhio pero che il Cleaning ha roba in piu quando giorno == 1
										if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
											//cout << " Caso del giorno 0 ho giorno 2 " << endl;
											r.start_end_routes[0][1] ++;
											if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
										}
										else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] > -1) {
											//cout << " Caso del giorno 0 non ho giorno 2" << endl;
											//r.Print();
											r.start_end_routes[0][1] ++;
											r.start_end_routes[2][0] ++;

										}
										else if (r.start_end_routes[1][1] >= -1 && r.primo_pass[1] == 1 && giorno == 1) {
											//cout << " Caso dove 3 da a 2  " << endl;
											///prendo da giorno 3 metto in 1
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
											if (r.time_arr[node_destroy] > start_day2) {
												//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
												//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
												//r.Print();
												//cin.get();
												r.start_end_routes[1][1] ++;
												r.start_end_routes[2][0] ++;
											}
											else {
												//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
												//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
												//r.Print();
												//cin.get();
												r.start_end_routes[0][1] ++;
												r.start_end_routes[2][0] ++;
											}
										}
										else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] == -1 && (r.time_dep[r.start_end_routes[1][1]] < end_day2 && r.time_arr[r.start_end_routes[1][1] + 1] < end_day2)) {
											//cout << " Caso dove 2 da a 3 particolare " << endl;
											///prendo da giorno 3 metto in 1 --------------------->>>> QUA FORSE DEVI CAAMBIARE
											//r.Print();
											//cin.get();
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.time_arr[node_destroy] < end_day2) {
											//cout << " Caso dove 2 da a 3 particolareggiante " << endl;
											///prendo da giorno 3 metto in 1
											//r.Print();
											//cin.get();
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										//r.Print();
										if (r.index >= 2) {
											//cout << " sono alla linea 8193 " << endl;
											if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
												//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
												//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
												r.start_end_routes[1][0] --;
												r.start_end_routes[0][0] = -1;
												r.start_end_routes[0][1] = -1;
												r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
												//r.Print();
												//system("pause");
											}
											else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
												//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
												//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
												r.start_end_routes[2][0] --;
												r.start_end_routes[1][0] = -1;
												r.start_end_routes[1][1] = -1;
												r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
												//r.Print();
												//system("pause");
											}
											else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
												//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
												//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
												r.start_end_routes[2][0] --;
												r.start_end_routes[0][0] = -1;
												r.start_end_routes[0][1] = -1;
												r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
												//r.Print();
												//system("pause");
											}
											else if (r.capacity[0] == 0) {
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											}
										}
										//r.Print();
									}
								}
								else if (r.index > (node_destroy - 1)) {
									//cout << " Adesso devo valutare se riesco ad andarci al sera " << endl;
									double time = r.time_dep[node_destroy] + ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
									int limit = 0;
									if (giorno == 0) limit = end_day1;
									else if (giorno == 1) limit = end_day2;
									else limit = end_day3;
									//cout << " CI arriverebbe alle  " << time << " quando c'è un end day " << limit << endl;
									if (node_destroy == 0 && r.index > 2) {
										//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
										//r.Print();
										//system("pause");
										/*cout << "start_end_routes[0][0]" << r.start_end_routes[0][0] << endl;
										cout << "start_end_routes[0][1]" << r.start_end_routes[0][1] << endl;
										cout << "start_end_routes[1][0]" << r.start_end_routes[1][0] << endl;
										cout << "start_end_routes[1][1]" << r.start_end_routes[1][1] << endl;
										cout << "start_end_routes[2][0]" << r.start_end_routes[2][0] << endl;
										cout << "start_end_routes[2][1]" << r.start_end_routes[2][1] << endl;
										cout << " Confrontato con Node Destroy " << node_destroy << endl;
										r.Print();*/
										if (r.start_end_routes[2][0] == -1 && r.start_end_routes[1][1] >= 2 && r.capacity[0] == 0) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
											if (r.start_end_routes[1][0] > 0)r.start_end_routes[1][0] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;


										}
										else if (r.start_end_routes[0][1] >= 1 && r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] >= 1 && r.capacity[0] == 0) {
											//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
											//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
											if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
											//r.start_end_routes[1][1] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;



										}
										else if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Caso in cui abbiamo solo gionro da 2 a 3 " << endl;
											//r.start_end_routes[0][1] --;
											if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
											r.start_end_routes[1][0] = -1;
											r.start_end_routes[1][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;

										}
										else if (r.capacity[0] == 0) {
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
										}
										//cout << " DONE " << endl;
										//r.Print();
										//system("pause");
									}
									else if (time <= limit) {
										if (giorno == 0 && (r.start_end_routes[1][0] > -1) && (r.start_end_routes[1][1] > -1)) {
											//cout << " Caso del giorno 0 ho giorno 2 " << endl;
											r.start_end_routes[0][1] ++;
											r.start_end_routes[1][0] ++;
										}
										else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1) {
											//cout << " Caso del giorno 0 non ho giorno 2 " << endl;
											r.start_end_routes[0][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else {
											//cout << " Caso che pesno dal giorno 2 e metto in 1 " << endl;
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										r.time_arr[node_destroy + 1] = time;
										double limite = start_day1;
										if (r.time_dep[node_destroy + 1] > end_day2) limite = start_day3;
										else if (r.time_dep[node_destroy + 1] > end_day1) limite = start_day2;
										//cout << " Nodo in causa " << r.places[node_destroy + 1] << " limite " << limite << endl;
										if (!(1 == r.places[node_destroy + 1] || 4 == r.places[node_destroy + 1])) if (r.time_dep[node_destroy + 1] < limite) {
											//cout << " Tempo nodo incriminato " << r.time_dep[node_destroy + 1] << " limite " << limite << " giorno " << giorno << endl;
											//r.Print();
											//cin.get();
											goback = true;
										}
									}
									else {
										time = r.time_arr[node_destroy + 1] - ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
										//cout << " Punto di snodo diventa il nodo prima di quello distrutto  riga 8247" << endl;
										//cout << " Siamo considerando il giorno " << giorno << endl;
										//r.Print();
										if (!(1 == r.places[node_destroy] || 4 == r.places[node_destroy])) {
											//cout << " Caso in cui devi stare molto attento... node:  " << node_destroy << endl;
											if (node_destroy == r.start_end_routes[0][1] && time < start_day2) {
												//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << time << endl;
												if ((r.time_arr[r.start_end_routes[1][1]] + (start_day2 - time)) <= end_day2) {
													r.time_dep[node_destroy] = start_day2;
													//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
													for (int a = node_destroy + 1; a < r.start_end_routes[1][1]; a++) {
														r.time_arr[a] += (start_day2 - time);
														r.time_dep[a] += (start_day2 - time);
													}
													r.time_arr[r.start_end_routes[1][1]] += (start_day2 - time);
													if (r.index == (r.start_end_routes[1][1] + 1)) r.time_dep[r.start_end_routes[1][1]] += (start_day2 - time);
												}
												else goback = true;
												//r.Print();
												//system("pause");
											}
											else if (node_destroy == r.start_end_routes[2][0] && time < start_day3) {
												//cout << " CAso in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
												if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
													r.time_dep[node_destroy] = start_day3;
													//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
													for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
														r.time_arr[a] += (start_day3 - time);
														r.time_dep[a] += (start_day3 - time);
													}
													r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
													if (r.index == (r.start_end_routes[2][1] + 1))r.time_dep[r.start_end_routes[2][1]] += (start_day3 - time);
												}
												else goback = true;
												//r.Print();
												//system("pause");
											}
											else r.time_dep[node_destroy] = time;
										}
										else r.time_dep[node_destroy] = time;
										//r.Print();
										//cout << " CASOOOOOOOOOOOOOOOOOOOOOOO CHE DEVIIIIIIIIIIIIII VALUTARE COME TRATTARE " << endl;
										//system("pause");
									}
								}
								r.removePlace_Arc(map_airplane);
								//cout << " Finito procedura h fatto tutto per nodo centrale con refuel " << endl;
								//r.Print();
							}
							// uqa forse ci vuole codice che sosti per favorire il repair
						}
						//else cout << " ------------------!!!!! Non fare nulla !!!!!!!!!!!! -------------------------- " << endl;
					}
				}

			} while (check);
			if (!goback) {
				//cout << " Qui ho funzione che azzera le flag dei passeggieri e mettee a posto start_end" << endl;
				bool check_Pass = false;
				//r.Print();
				for (int g = 0; g < 3; g++) {
					if ((r.start_end_routes[g][0] == r.start_end_routes[g][1]) && r.start_end_routes[g][0] != -1) {
						//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
						//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
						//if (g != 0) r.start_end_routes[g][0] = -1;
						//else r.start_end_routes[g][0] = 0;
						r.start_end_routes[g][0] = -1;
						r.start_end_routes[g][1] = -1;
					}


					if (r.primo_pass[g] == 1) {
						check_Pass = false;
						for (Passenger& p : r.passengers_in_route) {
							if (p.giorno == (g + 1)) {
								check_Pass = true;
								break;
							}

						}

						if (!check_Pass) r.primo_pass[g] = 0;

					}


				}
				//cout << " -----------------  Ho finito la distruzione della route  -------------------------- " << endl;
				//r.Print();
			}
			else {
				//r.Print();
				//old.Print();
				//cin.get();
				int now = passenger_removed.size();
				//cout << " Passeggieri rimossi fino a questo momento " << to_string(so_far) << endl;
				//cout << " Passeggieri rimossi Ora: " << to_string(now) << endl;
				//cout << "---------------------------> Route ora ha numero di passeggieri " << r.passengers_in_route.size() << endl;
				r = old;
				passenger_removed.erase(passenger_removed.begin() + so_far, passenger_removed.end());
				//cout << " Dopo erase size dei paseggieri pari a: " << passenger_removed.size() << endl;
				//cin.get();
				//system("pause");
			}
		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
	}


	//for (Route r : solution) r.Print();
	map<int, vector<int>> map_nodes;
	for (Route r : solution) {
		vector<int> nodes;
		map_nodes.insert(make_pair(r.aircraft_code, nodes));
		for (size_t j = 1; j < r.index - 1; j++) {
			if (r.capacity[j - 1] == 0 && r.capacity[j] == 0) {
				map_nodes[r.aircraft_code].push_back(j);
			}
		}
	}

	for (auto p : map_nodes) {
		//cout << " Aereo con codice " << p.first << endl;
		if (p.second.size() > 0) {
			//for (int i : p.second) cout << " Nodo Teorico: " << i << endl;
		}
		//cout << " -------------------" << endl;
	}

	for (Route& r : solution) {
		//r.Print();
		if (map_nodes[r.aircraft_code].size() > 0) {
			for (int i = map_nodes[r.aircraft_code].size() - 1; i >= 0; i--) {
				//cout << " Devo provare a togliere della Route " << r.aircraft_code << " con Size: " << r.index << " Nodo: " << map_nodes[r.aircraft_code][i] << endl;
				if (r.capacity[map_nodes[r.aircraft_code][i] - 1] == 0 && r.capacity[map_nodes[r.aircraft_code][i]] == 0) {
					//cout << " Autorizzato a rimuovere Nodo " << map_nodes[r.aircraft_code][i] << endl;
					cleaning_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, map_nodes[r.aircraft_code][i], passenger_removed, r, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				}//r.Print();
			}
		}
	}

	for (Route& r : solution) {
		if (r.time_arr[0] + map_airstrip[r.places[0]].ground_time < r.time_dep[0]) r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
		if (r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time < r.time_dep[r.index - 1]) {
			r.time_dep[r.index - 1] = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time;
		}

		if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) > 1 && r.primo_pass[1] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[1] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
		if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) > 1 && r.primo_pass[0] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[0] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
	}


	//cout << " ------------------------------------------------------- Route dopo aver tolto archi vuoti  ------------------------------------------------------------------- " << endl;
	//for (Route r : solution) r.Print();
	//system("pause");
}



void destroy_worst_rolling(double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, double peso_intermediate_stop, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//srand(time(NULL));
	int index = 0;
	for (Route& r : solution) {
		Route old = r;
		bool goback = false;
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			//cout << "---------------------------> Voglio provare a distruggere seguente " << r.aircraft_code << endl;
			//cout << "---------------------------> Ha numero di passeggieri " << r.passengers_in_route.size() << endl;
			int so_far = passenger_removed.size();
			//cout << " Passeggieri rimossi fino a questo momento " << to_string(so_far) << endl;
			// r.Print()
			// Primo elemanto la posizione il secondo il numero del nodo
			map<int, int> Node;
			bool check = true;
			int first = 0;
			do {
				//if i'm here i can destroy the route
				//at the moment i destroy only a node
				goback = false;
				Node = Compute_WorstNode(peso_intermediate_stop, r, map_airplane, map_airstrip, from_to);
				int node_destroy = Node[first];
				/*cout << " La funzione ha finito togli il nodo: " << node_destroy << endl;
				cout << " Route " << r.aircraft_code << endl;
				cout << " Node ha una size di " << Node.size() << endl;
				cout << " Voglio accedere a first = " << first << endl;
				*/
				if (node_destroy == 0 || first >= Node.size()) break;
				if (node_destroy == r.index - 1) { //this in the case of that node is at the end
					check = false;
					//cout << "nodo distrutto: " << node_destroy << endl;
					//r.print();
					vector<int> int_removed;
					//cout << " Sto lavorando sulla Route dell'Aereo " << r.aircraft_code << endl;
					//cout << " Questa route ha un index = " << r.index << endl;
					//cout << " siamo nel caso che il nodo distrutto sia l'ultimo " << endl;
					//r.print();

					int Min_From_Pass = node_destroy;
					int Max_To_Pass = node_destroy;
					for (int p = 0; p < r.passengers_in_route.size(); p++) {
						if (r.passengers_in_route[p].solution_to == node_destroy) {  // ho cambiato questa condizione
							if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
							int_removed.push_back(p);

							for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
								r.capacity[t]--;
								r.weight[t] += r.passengers_in_route[p].weight;
							}


						}
					}
					for (int i = int_removed.size() - 1; i >= 0; i--) {
						// code for repair forbidden***********************************************************
						r.passengers_in_route[int_removed[i]].route_before = index;
						//*************************************************************************************

						passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
						vector<Passenger>::iterator it;
						it = r.passengers_in_route.begin();
						r.passengers_in_route.erase(it + int_removed[i]);
					}
					// Qua devo mettere codice che è quello dell'Update
					/*cout << "************* Sono nel codice che abbiamo fatto **********" << endl;
					cout << "******************* Stampiamo la Route: *******************" << endl;
					cout << " CODE ------> " << r.aircraft_code << endl;
					cout << " Minimo from passeggeri ----> " << Min_From_Pass << endl;
					cout << " Massimo to passeggeri ----> " << Max_To_Pass << endl;
					//r.print();
					cout << " Node destroyed ------> " << node_destroy << endl;
					*/

					int index_min_from;
					for (int i = Min_From_Pass; i >= 0; i--) {
						if (r.refueling[i] && i != node_destroy) {
							index_min_from = i;
							break;
						}
					}
					//cout << " Parto a cercare il minimo da ------> " << index_min_from << endl;
					// Codice che serve per cercare il minimo nel range
					//cout << " Finisco a cercare il minimo in ------> " << Max_To_Pass << endl;


					for (int k = index_min_from; k < Max_To_Pass; k++) {
						if (r.refueling[k] && r.quantity_fuel[k] < map_airplane[r.aircraft_code].max_fuel) { //&& k!= node_destroy
						//cout << " Sto valutando il caso del nodo " << k << endl;
							int Node_min = k;
							double min_weight = r.weight[k];
							int index_updating_from = k;
							int index_updating_to = r.index;  //qua prima c'era -1
							for (int i = k + 1; i <= Max_To_Pass; i++) {
								//cout << " Sto guardando il nodo " << i << endl;
								if (r.refueling[i]) break;
								//cout << " Ho passato il break " << endl;
								if (r.weight[i] < min_weight && i != node_destroy) {
									// cout << " Ho passato IF di controllo " << endl;
									min_weight = r.weight[i];
									Node_min = i;
								}
							}
							//cout << " Nodo di minimo ---> " << Node_min << endl;
							//cout << " Valore di minimi --> " << min_weight << endl;
							if (Node_min >= 0) {
								for (int i = k + 1; i < r.index; i++) {
									if (r.refueling[i]) {   // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
										index_updating_to = i;
										break;
									}
								}
								//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
								//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
								double Fuel_before = r.quantity_fuel[index_updating_from];
								//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
								//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
								r.quantity_fuel[index_updating_from] = min(map_airplane[r.aircraft_code].max_fuel, r.quantity_fuel[index_updating_from] + min_weight);
								//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
								//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
								r.weight[index_updating_from] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
								//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
								for (int i = index_updating_from + 1; i < index_updating_to; i++) {
									if (i != node_destroy) {
										r.quantity_fuel[i] += (r.quantity_fuel[index_updating_from] - Fuel_before);
										r.weight[i] -= (r.quantity_fuel[index_updating_from] - Fuel_before);
									}
								}
							}
						}

					}
					r.removePlace(node_destroy, map_airplane);
					//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi quando tolgo ultimo nodo " << endl;
					double add_fuel = 0;
					int index_weight_neg = -1;
					for (int j = 0; j < r.index; j++) {
						if (r.weight[j] < 0) {
							//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
							add_fuel = r.weight[j];
							index_weight_neg = j;
							int index_refueling = index_weight_neg;
							for (int i = index_weight_neg; i >= 0; i--) {
								if (r.refueling[i]) { //&& i != node_destroy
									index_refueling = i;
									break;
								}
							}
							//cout << " Aggiorno da " << index_refueling << endl;
							for (int t = index_refueling; t < r.index; t++) {
								//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
								if (r.refueling[t] && t != index_refueling) break;
								//cout << " Ho passato if aggiorno nodo " << t << endl;
								//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
								r.quantity_fuel[t] += add_fuel;
								r.weight[t] -= add_fuel;
								//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
							}
						}
					}
				}
				else {
					int giorno = -1;
					if (node_destroy <= r.start_end_routes[0][1]) giorno = 0;
					else if (node_destroy <= r.start_end_routes[1][1]) giorno = 1;
					else giorno = 2;
					bool imp_ref = false;
					//cout << "Nodo: " << node_destroy << endl;
					if (node_destroy != (r.start_end_routes[0][1]) && node_destroy != (r.start_end_routes[1][1])) {
						//cout << " Ok non è ultimo nodo in nessuna giornata " << endl;
						//cout << " Index della Route: " << r.index << endl;
						//r.Print();
						//cout << " Nodo appartiene alla giornata " << giorno << endl;
						if (giorno != 2 && r.refueling[node_destroy] && !(r.refueling[r.start_end_routes[giorno][1]])) {
							int j = r.index - 1;
							imp_ref = true;
							for (int i = node_destroy + 1; i < r.index; i++) {
								if (r.refueling[i]) {
									j = i;
									break;
								}
							}
							double fuel1 = r.quantity_fuel[node_destroy - 1] - from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
							double diff = r.quantity_fuel[node_destroy + 1] - fuel1;
							if ((r.quantity_fuel[j - 1] - diff - from_to_FuelConsumed[r.aircraft_code][r.places[j - 1]][r.places[j]]) >= map_airplane[r.aircraft_code].min_fuel) imp_ref = false;
						}
						if (imp_ref) {
							//cout << " Qua dic eche è importante peche lo dice ?!?!? " << endl;
							//r.Print();
						}
						//cout << " Y/N nodo importante:  " << imp_ref << endl;
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
						if (!imp_ref && (fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1] && r.places[node_destroy - 1] != r.places[node_destroy + 1]) {
							check = false;
							//cout << " Ok -----------> è definitivamente un nodo che puo essere rimosso ed non è un nodo in mezzo " << endl;
							vector<int> int_removed;
							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t]--;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}

							//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
							//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
							//cout << " Adesso sto per rimuovere i passeggieri " << endl;
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							r.update_route_destroy(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
							int index_before = node_destroy - 1;
							//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
							//r.print();
							//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
							double diff = 0;
							//cout << " Index before: " << index_before << endl;
							for (int i = node_destroy + 1; i < r.index; i++) {
								//cout << " sono al i = " << i << endl;
								if (r.refueling[i]) break;
								//cout << "weight before: " << r.weight[i] << endl;
								if (index_before == (node_destroy - 1)) {
									//cout << " sono al primo giro faccio " << endl;
									diff = r.quantity_fuel[i];
									//cout << " diff: " << diff << endl;
									r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
									diff = diff - r.quantity_fuel[i];
									//cout << " diff aggiornata: " << diff << endl;

								}
								else {
									//cout << " non sono al primo giro " << endl;
									r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
									//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								}

								//cout << " weight prima " << r.weight[i] << endl;
								r.weight[i] = r.weight[i] + diff;
								//cout << " weight dopo " << r.weight[i] << endl;



								//cout << "weight after: " << r.weight[i] << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

								//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
								//cout << " inidce before = " << index_before << endl;
								index_before = i + 1;
								//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
							}
							//*****************************************************************************************************************************************************************


							r.removePlace(node_destroy, map_airplane);
							//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
							double add_fuel = 0;
							int index_weight_neg = -1;
							for (int j = 0; j < r.index; j++) {
								if (r.weight[j] < 0) {
									//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
									add_fuel = r.weight[j];
									index_weight_neg = j;
									int index_refueling = index_weight_neg;
									for (int i = index_weight_neg; i >= 0; i--) {
										if (r.refueling[i]) { //&& i != node_destroy
											index_refueling = i;
											break;
										}
									}
									//cout << " Aggiorno da " << index_refueling << endl;
									for (int t = index_refueling; t < r.index; t++) {
										//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
										if (r.refueling[t] && t != index_refueling) break;
										//cout << " Ho passato if aggiorno nodo " << t << endl;
										//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
										r.quantity_fuel[t] += add_fuel;
										r.weight[t] -= add_fuel;
										//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
									}
								}
							}
							/*cout << " Sto per valutare casisitiche appena introdotte linea 7734" << endl;
							cout << "start_end_routes[0][0] " << r.start_end_routes[0][0] << endl;
							cout << "start_end_routes[0][1] " << r.start_end_routes[0][1] << endl;
							cout << "start_end_routes[1][0] " << r.start_end_routes[1][0] << endl;
							cout << "start_end_routes[1][1] " << r.start_end_routes[1][1] << endl;
							cout << "start_end_routes[2][0] " << r.start_end_routes[2][0] << endl;
							cout << "start_end_routes[2][1] " << r.start_end_routes[2][1] << endl;
							cout << "r.primo_pass[0] " << r.primo_pass[0] << endl;
							cout << "r.primo_pass[1] " << r.primo_pass[1] << endl;
							cout << "r.primo_pass[2] " << r.primo_pass[2] << endl;
							cout << " r.index: " << r.index << endl;*/
							if (r.index > 2) {
								//cout << " sono alla linea 7746 " << endl;
								if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
									//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
									//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
									r.start_end_routes[1][0] --;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[0][1] = -1;
									r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
									r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									//r.Print();
									//system("pause");
								}
								else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
									//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
									//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
									r.start_end_routes[2][0] --;
									r.start_end_routes[1][0] = -1;
									r.start_end_routes[1][1] = -1;
									r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
									r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									//r.Print();
									//system("pause");
								}
								else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
									//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
									//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
									r.start_end_routes[2][0] --;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[0][1] = -1;
									r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
									r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
									r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									//r.Print();
									//system("pause");
								}
							}
							else if (r.index == 2 && r.capacity[0] == 0) {
								//r.Print();
								//cout << " --------------------------------->>>>> CASO DA VALUTARE  <<<<<----------------------------------------------------" << endl;
								//cout << " --------------------------------->>>>> ENTRO  RIGA 779   <<<<<----------------------------------------------------" << endl;
								r.places.erase(r.places.begin() + 1);
								r.time_arr.erase(r.time_arr.begin() + 1);
								r.time_dep.erase(r.time_dep.begin() + 1);
								r.refueling.erase(r.refueling.begin() + 1);
								r.quantity_fuel.erase(r.quantity_fuel.begin() + 1);
								r.weight.erase(r.weight.begin() + 1);
								r.capacity.erase(r.capacity.begin() + 1);
								r.index = r.index - 1;
								if (r.start_end_routes[0][1] != r.start_end_routes[0][0]) {
									r.start_end_routes[0][1] = -1;
									r.start_end_routes[0][0] = 0;
									r.start_end_routes[1][1] = -1;
									r.start_end_routes[1][0] = -1;
									r.start_end_routes[2][1] = -1;
									r.start_end_routes[2][0] = -1;
								}
								else if (r.start_end_routes[1][1] != r.start_end_routes[1][0]) {
									r.start_end_routes[0][1] = -1;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[1][1] = -1;
									r.start_end_routes[1][0] = 0;
									r.start_end_routes[2][1] = -1;
									r.start_end_routes[2][0] = -1;
								}
								else {
									r.start_end_routes[0][1] = -1;
									r.start_end_routes[0][0] = -1;
									r.start_end_routes[1][1] = -1;
									r.start_end_routes[1][0] = -1;
									r.start_end_routes[2][1] = -1;
									r.start_end_routes[2][0] = 0;
								}
							}

						}
					}
					else if (!r.refueling[node_destroy]) {
						//cout << " ---------------------> Caso che tolgo nodo Di collegamento in cui NON devo fare refuel" << endl;
						//r.Print();
						//system("pause");
						// --------------------------------------------------------------------------------Inizio codiche che deve togliere il nodo -------------------------------------------------------------------
						double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
						if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
							check = false;
							//cout << " Ok è definitivamente un nodo che puo essere rimosso " << endl;
							vector<int> int_removed;
							int Min_From_Pass = node_destroy;
							int Max_To_Pass = node_destroy;
							for (int p = 0; p < r.passengers_in_route.size(); p++) {
								if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
									if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
									if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
									int_removed.push_back(p);
									for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
										r.capacity[t]--;
										r.weight[t] += r.passengers_in_route[p].weight;
									}
								}
							}

							//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
							//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
							//cout << " Adesso sto per rimuovere i passeggieri " << endl;
							for (int i = int_removed.size() - 1; i >= 0; i--) {
								// code for repair forbidden***********************************************************
								r.passengers_in_route[int_removed[i]].route_before = index;
								//*************************************************************************************
								passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
								vector<Passenger>::iterator it;
								it = r.passengers_in_route.begin();
								r.passengers_in_route.erase(it + int_removed[i]);
							}
							r.update_route_destroy_middleNode(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
							int index_before = node_destroy - 1;
							//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
							//r.print();
							//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
							double diff = 0;
							//cout << " Index before: " << index_before << endl;
							for (int i = node_destroy + 1; i < r.index; i++) {
								//cout << " sono al i = " << i << endl;
								if (r.refueling[i]) break;
								//cout << "weight before: " << r.weight[i] << endl;
								if (index_before == (node_destroy - 1)) {
									//cout << " sono al primo giro faccio " << endl;
									diff = r.quantity_fuel[i];
									//cout << " diff: " << diff << endl;
									r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
									diff = diff - r.quantity_fuel[i];
									//cout << " diff aggiornata: " << diff << endl;

								}
								else {
									//cout << " non sono al primo giro " << endl;
									r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
									//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								}

								//cout << " weight prima " << r.weight[i] << endl;
								r.weight[i] = r.weight[i] + diff;
								//cout << " weight dopo " << r.weight[i] << endl;



								//cout << "weight after: " << r.weight[i] << endl;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
								//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

								//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
								//cout << " inidce before = " << index_before << endl;
								index_before = i + 1;
								//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
							}
							//*****************************************************************************************************************************************************************


							r.removePlace_New(node_destroy, map_airplane);
							//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
							double add_fuel = 0;
							int index_weight_neg = -1;
							for (int j = 0; j < r.index; j++) {
								if (r.weight[j] < 0) {
									//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
									add_fuel = r.weight[j];
									index_weight_neg = j;
									int index_refueling = index_weight_neg;
									for (int i = index_weight_neg; i >= 0; i--) {
										if (r.refueling[i]) { //&& i != node_destroy
											index_refueling = i;
											break;
										}
									}
									//cout << " Aggiorno da " << index_refueling << endl;
									for (int t = index_refueling; t < r.index; t++) {
										//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
										if (r.refueling[t] && t != index_refueling) break;
										//cout << " Ho passato if aggiorno nodo " << t << endl;
										//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
										r.quantity_fuel[t] += add_fuel;
										r.weight[t] -= add_fuel;
										//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
									}
								}
							}

							// -------------------------------------------------------------------   ORA Ho Casistiche su chi sia nuovo nodo di mezzo -------------------------------------------------------------------
							node_destroy--;
							//cout << " Fintio di togliere il nodo adesso valuto casistiche " << endl;
							//r.Print();
							//system("pause");
							if (r.places[node_destroy] == r.places[node_destroy + 1] && r.index > (node_destroy - 1)) {
								//cout << " Caso !: in cui ho nodi doppi perchè " << r.places[node_destroy] << "==" << r.places[node_destroy + 1] << endl;
								/// NUOVS PARTE DEL CODICE
								r.time_arr[node_destroy + 1] = r.time_arr[node_destroy];
								/// FINITA NUOVA PARTE DEL CODICE
								//cout << " Inizio codice di aggrregazione nodi doppi " << endl;
								goback = aggregate_same_nodes_destoy_rolling(r, node_destroy);
								//int gg = 2;
								//if (node_destroy <= r.start_end_routes[0][1]) gg = 0;
								//else if (node_destroy <= r.start_end_routes[1][1]) gg = 1;
								//else giorno = 2;
								/*cout << " Adesso devo mettere a posto gli indici dopo aggregazione dei nodi giorno: " << giorno << endl;
								cout << " Finito codice aggre nodo " << node_destroy << endl;
								cout << "start_end_routes[0][0]" << r.start_end_routes[0][0] << endl;
								cout << "start_end_routes[0][1]" << r.start_end_routes[0][1] << endl;
								cout << "start_end_routes[1][0]" << r.start_end_routes[1][0] << endl;
								cout << "start_end_routes[1][1]" << r.start_end_routes[1][1] << endl;
								cout << "start_end_routes[2][0]" << r.start_end_routes[2][0] << endl;
								cout << "start_end_routes[2][1]" << r.start_end_routes[2][1] << endl;
								cout << " r.index: " << r.index << endl;
								cout << " Giorno: " << giorno << endl;*/
								// IF SOTTO LO HO APPENA MESSO NON SO SE VADA BENE --- è ANCORA IN FORSE
								if (node_destroy == r.start_end_routes[0][1] + 1 || node_destroy == r.start_end_routes[1][1] + 1 || node_destroy == r.start_end_routes[2][1] + 1) {
									if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
										//cout << " Caso del giorno 1 ho giorno 2 " << endl;
										///prendo da giorno 2 metto in 1
										//r.Print();
										//system("pause");
										r.start_end_routes[0][1] ++;
										if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
										//r.Print();
										//system("pause");
									}
									else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] > -1) {
										//cout << " Caso del giorno 1 non ho giorno 2" << endl;
										///prendo da giorno 3 metto in 1
										//r.Print();
										r.start_end_routes[0][1] ++;
										r.start_end_routes[2][0] ++;
										//r.Print();
										//system("pause");
									}
									else if (r.start_end_routes[1][1] >= -1 && r.primo_pass[1] == 1 && giorno == 1) {
										//cout << " Caso dove 2 da a 3  " << endl;
										///prendo da giorno 3 metto in 1
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
										if (r.time_arr[node_destroy] > start_day2) {
											//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
											//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
											//r.Print();
											//cin.get();
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else {
											//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
											//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
											//r.Print();
											//cin.get();
											r.start_end_routes[0][1] ++;
											r.start_end_routes[2][0] ++;
										}
									}
									else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] == -1 && (r.time_dep[r.start_end_routes[1][1]] < end_day2 && r.time_arr[r.start_end_routes[1][1] + 1] < end_day2)) {
										//cout << " Caso dove 2 da a 3 particolare con tempi " << endl;
										///prendo da giorno 3 metto in 1 --------------------->>>> QUA FORSE DEVI CAAMBIARE
										//r.Print();
										//cin.get();
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									//((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1)
									else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.time_arr[node_destroy] <= end_day2) {
										//cout << " Caso dove 2 da a 3 particolareggiante " << endl;
										///prendo da giorno 3 metto in 1
										//r.Print();
										//cin.get();
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									//r.Print();
									//cout << " Sto per valutare casisitiche richieste dal NEl dopo nodi doppi linea 8242 " << endl;
									if (r.index >= 2) {
										//cout << " sono alla linea 8193 " << endl;
										if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
											r.start_end_routes[1][0] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											//r.Print();
											//system("pause");
										}
										else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
											r.start_end_routes[2][0] --;
											r.start_end_routes[1][0] = -1;
											r.start_end_routes[1][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											//r.Print();
											//system("pause");
										}
										else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
											r.start_end_routes[2][0] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											//r.Print();
											//system("pause");
										}
									}
									//r.Print();
								}
							}
							else if (r.index > (node_destroy - 1)) {
								//cout << " Adesso devo valutare se riesco ad andarci al sera " << endl;
								double time = r.time_dep[node_destroy] + ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
								int limit = 0;
								if (giorno == 0) limit = end_day1;
								else if (giorno == 1) limit = end_day2;
								else limit = end_day3;
								//cout << " CI arriverebbe alle  " << time << " quando c'è un end day " << limit << endl;
								if (node_destroy == 0 && r.index > 2) {
									//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
									//r.Print();
									//system("pause");
									//r.Print();
									if (r.start_end_routes[2][0] == -1 && r.start_end_routes[1][1] >= 2 && r.capacity[0] == 0) {
										//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
										//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
										if (r.start_end_routes[1][0] > 0)r.start_end_routes[1][0] --;
										r.start_end_routes[0][0] = -1;
										r.start_end_routes[0][1] = -1;
										r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;


									}
									else if (r.start_end_routes[0][1] >= 1 && r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] >= 1 && r.capacity[0] == 0) {
										//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
										//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
										//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
										if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
										//r.start_end_routes[1][1] --;
										r.start_end_routes[0][0] = -1;
										r.start_end_routes[0][1] = -1;
										r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;



									}
									else if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
										//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
										//cout << " Caso in cui abbiamo solo gionro da 2 a 3 " << endl;
										//r.start_end_routes[0][1] --;
										if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
										r.start_end_routes[1][0] = -1;
										r.start_end_routes[1][1] = -1;
										r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;

									}
									else if (r.capacity[0] == 0) {
										r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
										r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
									}
									//cout << " DONE " << endl;
									//r.Print();
								}
								else if (time <= limit) {

									if (giorno == 0 && (r.start_end_routes[1][0] > -1) && (r.start_end_routes[1][1] > -1)) {
										//cout << " Caso del giorno 0 ho giorno 2 " << endl;
										//r.Print();
										r.start_end_routes[0][1] ++;
										r.start_end_routes[1][0] ++;
										//r.Print();
									}
									else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1) {
										//cout << " Caso del giorno 0 non ho giorno 2 " << endl;
										r.start_end_routes[0][1] ++;
										r.start_end_routes[2][0] ++;
									}
									else {
										//cout << " Caso che pesno dal giorno 3 e metto in 2 " << endl;
										r.start_end_routes[1][1] ++;
										r.start_end_routes[2][0] ++;
									}
									r.time_arr[node_destroy + 1] = time;
									double limite = start_day1;
									if (r.time_dep[node_destroy + 1] > end_day2) limite = start_day3;
									else if (r.time_dep[node_destroy + 1] > end_day1) limite = start_day2;
									//cout << " Nodo in causa " << r.places[node_destroy + 1] << " limite " << limite << endl;
									//cout << " Nodo in questione è nodo " << r.places[node_destroy + 1] << " limite: " << limite << " valore monitoraato: " << r.time_dep[node_destroy + 1] << endl;
									if (!(1 == r.places[node_destroy + 1] || 4 == r.places[node_destroy + 1])) if (r.time_dep[node_destroy + 1] < limite) {
										//cout << " Tempo nodo incriminato " << r.time_dep[node_destroy + 1] << " limite " << limite << " giorno " << giorno << endl;
										//r.Print();
										//cin.get();
										//system("pause");
										goback = true;
									}
									//r.Print();
								}
								else {
									time = r.time_arr[node_destroy + 1] - ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
									//cout << " Punto di snodo diventa il nodo prima di quello distrutto  riga 7993" << endl;
									//cout << " Siamo considerando il giorno " << giorno << endl;
									//r.Print();
									if (!(1 == r.places[node_destroy] || 4 == r.places[node_destroy])) {
										//cout << " Caso in cui devi stare molto attento... node:  " << node_destroy << endl;
										if (node_destroy == r.start_end_routes[0][1] && time < start_day2) {
											//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << time << endl;
											if ((r.time_arr[r.start_end_routes[1][1]] + (start_day2 - time)) <= end_day2) {
												r.time_dep[node_destroy] = start_day2;
												//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
												for (int a = node_destroy + 1; a < r.start_end_routes[1][1]; a++) {
													r.time_arr[a] += (start_day2 - time);
													r.time_dep[a] += (start_day2 - time);
												}
												r.time_arr[r.start_end_routes[1][1]] += (start_day2 - time);
												if (r.index == r.start_end_routes[1][1] + 1)r.time_dep[r.start_end_routes[1][1]] += (start_day2 - time);
											}
											else goback = true;
											//r.Print();
											//system("pause");
										}
										else if (node_destroy == r.start_end_routes[2][0] && time < start_day3) {
											//cout << " CAso in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
											if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
												r.time_dep[node_destroy] = start_day3;
												//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
												for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
													r.time_arr[a] += (start_day3 - time);
													r.time_dep[a] += (start_day3 - time);
												}
												r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
												if (r.index == r.start_end_routes[2][1] + 1)r.time_dep[r.start_end_routes[2][1]] += (start_day3 - time);
											}
											else goback = true;
											//r.Print();
											//system("pause");
										}
										else r.time_dep[node_destroy] = time;
									}
									else r.time_dep[node_destroy] = time;
									//r.Print();
									//cout << " CASOOOOOOOOOOOOOOOOOOOOOOO CHE DEVIIIIIIIIIIIIII VALUTARE COME TRATTARE " << endl;
									//system("pause");
								}
							}
							//system("pause");
							r.removePlace_Arc(map_airplane);
							// Codice che toglie archi vuoti dal fondo andrebbe messo qua

						}



					}
					else {
						//cout << "  ---------->  Caso in cui nel nodo di mezzo faccio --------------> refuel " << endl;
						//if (r.aircraft_code == "5H-RJS") r.Print();
						int index_fuel_before = node_destroy;
						int index_fuel_after = node_destroy;
						for (int i = node_destroy - 1; i >= 0; i--) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_fuel_before = i;
								break;
							}
						}
						for (int i = node_destroy + 1; i < r.index; i++) {
							if (r.refueling[i]) { //&& i != node_destroy
								index_fuel_after = i;
								break;
							}
						}
						//cout << " Nodo: " << node_destroy << " Fuel prima " << index_fuel_before << " Fuel dopo : " << index_fuel_after << endl;
						//r.Print();
						double fuel1 = r.quantity_fuel[node_destroy - 1] - from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
						double diff = r.quantity_fuel[node_destroy + 1] - fuel1;
						/*
						cout << " Nice !!!!! It makes sense !!!!!!! " << endl;
						cout << " r.quantity_fuel[node_destroy-1] " << r.quantity_fuel[node_destroy - 1] << " -  from_to_FuelConsumed[r.aircraft_code + ;  + r.places[node_destroy - 1] + ;  + r.places[node_destroy + 1]] " << from_to_FuelConsumed[r.aircraft_code + ";" + r.places[node_destroy - 1] + ";" + r.places[node_destroy + 1]] << endl;
						cout << " Fuel del nodo che sto distruffendo " << r.quantity_fuel[node_destroy+1] << endl;
						cout << " Nodo priam del primo fuel aveva fuel pari a " << r.quantity_fuel[index_fuel_after - 1] << " Adesso sara " << r.quantity_fuel[index_fuel_after - 1] - diff << endl;
						cout << " Che deve essere maggriore di sicurezza " << map_airplane[r.aircraft_code].min_fuel  <<" + tratto " << from_to_FuelConsumed[r.aircraft_code + ";" + r.places[index_fuel_after - 1] + ";" + r.places[index_fuel_after]] << endl;
						cout << " QUa biogna metterci codice della rimozione " << endl;
						// Fuel del nodo prima del nodo da distruggere
						// Toglici il fuel da li al nodo dopo il nodo distrutto
						//Prendi il fuel che c'era nel nodo che stai distruggendo
						//FAi fuel del nodo distrutto meno fuel calcoalto nei primi due punti = diff
						// prendi nodo prima del nodo_fuel_after tolgi diff
						// Guarda ancora se è sufficiente a fare arco dopo piu soglia minnima
						r.Print();
						system("pause");
						*/
						if (r.quantity_fuel[index_fuel_after - 1] - diff >= (map_airplane[r.aircraft_code].min_fuel + from_to_FuelConsumed[r.aircraft_code][r.places[index_fuel_after - 1]][r.places[index_fuel_after]])) {
							//cout << "  ------------- Comincio codice di rimozione  ------------------------" << endl;
							//r.Print();
							//system("pause");
							// --------------------------------------------------------------------------------Inizio codiche che deve togliere il nodo -------------------------------------------------------------------
							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[node_destroy - 1]][r.places[node_destroy + 1]];
							if ((fuel_consumed + map_airplane[r.aircraft_code].min_fuel) <= r.quantity_fuel[node_destroy - 1]) {
								check = false;
								//cout << " Ok nodo " << node_destroy << " NAme: " << r.places[node_destroy] << " è definitivamente un nodo che puo essere rimosso " << endl;
								vector<int> int_removed;
								int Min_From_Pass = node_destroy;
								int Max_To_Pass = node_destroy;
								for (int p = 0; p < r.passengers_in_route.size(); p++) {
									if (r.passengers_in_route[p].solution_to == node_destroy || r.passengers_in_route[p].solution_from == node_destroy) { // ho cambiato questa condizione
										if (r.passengers_in_route[p].solution_from < Min_From_Pass) Min_From_Pass = r.passengers_in_route[p].solution_from;
										if (r.passengers_in_route[p].solution_to > Max_To_Pass) Max_To_Pass = r.passengers_in_route[p].solution_to;
										int_removed.push_back(p);
										for (int t = r.passengers_in_route[p].solution_from; t < r.passengers_in_route[p].solution_to; t++) {
											r.capacity[t]--;
											r.weight[t] += r.passengers_in_route[p].weight;
										}
									}
								}

								//cout << " (sono dentro al codice del main ) Valore Minimo di From dei passeggieri ---> " << Min_From_Pass << endl;
								//cout << " (sono dentro al codice del main ) Valore Massimo di To dei passeggieri ---> " << Max_To_Pass << endl;
								//cout << " Adesso sto per rimuovere i passeggieri " << endl;
								for (int i = int_removed.size() - 1; i >= 0; i--) {
									// code for repair forbidden***********************************************************
									r.passengers_in_route[int_removed[i]].route_before = index;
									//*************************************************************************************
									passenger_removed.push_back(r.passengers_in_route[int_removed[i]]);
									vector<Passenger>::iterator it;
									it = r.passengers_in_route.begin();
									r.passengers_in_route.erase(it + int_removed[i]);
								}
								r.update_route_destroy_middleNode(giorno, node_destroy, Min_From_Pass, Max_To_Pass, from_to, map_airplane, map_airstrip); //update of the time
								int index_before = node_destroy - 1;
								//cout << " ************************* Ho finito  codice stampo route aggiornata gli devo ancora togliere nodo  **********************************" << endl;
								//r.print();
								//cout << " Adessos sto iniziando a sistemare i pesi perche togliero nodo del aereo " << r.aircraft_code << endl;
								double diff = 0;
								//cout << " Index before: " << index_before << endl;
								for (int i = node_destroy + 1; i < r.index; i++) {
									//cout << " sono al i = " << i << endl;
									if (r.refueling[i]) break;
									//cout << "weight before: " << r.weight[i] << endl;
									if (index_before == (node_destroy - 1)) {
										//cout << " sono al primo giro faccio " << endl;
										diff = r.quantity_fuel[i];
										//cout << " diff: " << diff << endl;
										r.quantity_fuel[i] = r.quantity_fuel[index_before] - fuel_consumed;
										diff = diff - r.quantity_fuel[i];
										//cout << " diff aggiornata: " << diff << endl;

									}
									else {
										//cout << " non sono al primo giro " << endl;
										r.quantity_fuel[i] = r.quantity_fuel[i] - diff;
										//cout << " Al fuel del nodo " << i << " tolgo " << diff << endl;
										//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									}

									//cout << " weight prima " << r.weight[i] << endl;
									r.weight[i] = r.weight[i] + diff;
									//cout << " weight dopo " << r.weight[i] << endl;



									//cout << "weight after: " << r.weight[i] << endl;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed1 - fuel_consumed2 - (map_airplane[r.aircraft_code].max_fuel - r.quantity_fuel[node_destroy]);
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;
									//r.weight[i] = r.weight[i] + fuel_consumed - fuel_consumed2 - fuel_consumed1;

									//r.weight[i] = r.weight[i] - fuel_consumed1 - fuel_consumed2;
									//cout << " inidce before = " << index_before << endl;
									index_before = i + 1;
									//cout << " inidce before = " << index_before << " riparto con un altro giro "<<  endl;
								}
								//*****************************************************************************************************************************************************************


								r.removePlace_New(node_destroy, map_airplane);
								//cout << " Aggiornamento in main qui ho gia rimosso il nodo adesso sistemo i weigth negativi " << endl;
								double add_fuel = 0;
								int index_weight_neg = -1;
								for (int j = 0; j < r.index; j++) {
									if (r.weight[j] < 0) {
										//cout << " Nodo " << j << " della route " << r.aircraft_code << " negativo " << r.weight[j] << endl;
										add_fuel = r.weight[j];
										index_weight_neg = j;
										int index_refueling = index_weight_neg;
										for (int i = index_weight_neg; i >= 0; i--) {
											if (r.refueling[i]) { //&& i != node_destroy
												index_refueling = i;
												break;
											}
										}
										//cout << " Aggiorno da " << index_refueling << endl;
										for (int t = index_refueling; t < r.index; t++) {
											//cout << " Nodo " << t << " controllo se sto dentro o devo uscire " << endl;
											if (r.refueling[t] && t != index_refueling) break;
											//cout << " Ho passato if aggiorno nodo " << t << endl;
											//cout << " Quantità fuel prima : " << r.quantity_fuel[t] << " Weigth prima " << r.weight[t] << endl;
											r.quantity_fuel[t] += add_fuel;
											r.weight[t] -= add_fuel;
											//cout << " Quantità fuel dopo : " << r.quantity_fuel[t] << " Weigth dopo " << r.weight[t] << endl;
										}
									}
								}
								// -------------------------------------------------------------------   ORA Ho Casistiche su chi sia nuovo nodo di mezzo -------------------------------------------------------------------

								node_destroy--;
								//r.Print();
								//system("pause");
								if (r.places[node_destroy] == r.places[node_destroy + 1] && r.index > (node_destroy - 1)) {
									//cout << " Caso !: in cui ho nodi doppi perchè " << r.places[node_destroy] << "==" << r.places[node_destroy + 1] << " stampo route prima della aggregazione " << endl;
									//r.Print();
									/// NUOVS PARTE DEL CODICE
									r.time_arr[node_destroy + 1] = r.time_arr[node_destroy];
									/// FINITA NUOVA PARTE DEL CODICE
									//cout << " Inizio codice di aggrregazione nodi doppi " << endl;
									//cout << " Giorno: " << giorno << endl;
									goback = aggregate_same_nodes_destoy_rolling(r, node_destroy);
									/*cout << "start_end_routes[0][0]" << r.start_end_routes[0][0] << endl;
									cout << "start_end_routes[0][1]" << r.start_end_routes[0][1] << endl;
									cout << "start_end_routes[1][0]" << r.start_end_routes[1][0] << endl;
									cout << "start_end_routes[1][1]" << r.start_end_routes[1][1] << endl;
									cout << "start_end_routes[2][0]" << r.start_end_routes[2][0] << endl;
									cout << "start_end_routes[2][1]" << r.start_end_routes[2][1] << endl;
									cout << " Confrontato con Node Destroy " << node_destroy << endl;
									r.Print();*/
									if (node_destroy == r.start_end_routes[0][1] + 1 || node_destroy == r.start_end_routes[1][1] + 1 || node_destroy == r.start_end_routes[2][1] + 1) {
										// Questa dovrebbe essere la versione piu aggiornata nel casual -- Occhio pero che il Cleaning ha roba in piu quando giorno == 1
										if (giorno == 0 && r.start_end_routes[1][0] > -1 && r.start_end_routes[1][1] > -1 && r.start_end_routes[0][1] > -1) {
											//cout << " Caso del giorno 0 ho giorno 2 " << endl;
											r.start_end_routes[0][1] ++;
											if (r.start_end_routes[1][0] != 0 || r.start_end_routes[0][1] > -1) r.start_end_routes[1][0] ++;
										}
										else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1 && r.start_end_routes[0][1] > -1) {
											//cout << " Caso del giorno 0 non ho giorno 2" << endl;
											//r.Print();
											r.start_end_routes[0][1] ++;
											r.start_end_routes[2][0] ++;

										}
										else if (r.start_end_routes[1][1] >= -1 && r.primo_pass[1] == 1 && giorno == 1) {
											//cout << " Caso dove 3 da a 2  " << endl;
											///prendo da giorno 3 metto in 1
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] != -1) {
											if (r.time_arr[node_destroy] > start_day2) {
												//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  > " << start_day2 << endl;
												//cout << " Caso del giorno 0 non ho giorno 2 particolare A" << endl;
												//r.Print();
												//cin.get();
												r.start_end_routes[1][1] ++;
												r.start_end_routes[2][0] ++;
											}
											else {
												//cout << " Situazione dove node_destroy " << r.places[node_destroy] << " tempo arrivo: " << r.time_arr[node_destroy] << "  < " << start_day2 << endl;
												//cout << " Caso del giorno 0 non ho giorno 2 particolare B" << endl;
												//r.Print();
												//cin.get();
												r.start_end_routes[0][1] ++;
												r.start_end_routes[2][0] ++;
											}
										}
										else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] == r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.start_end_routes[0][1] == -1 && (r.time_dep[r.start_end_routes[1][1]] < end_day2 && r.time_arr[r.start_end_routes[1][1] + 1] < end_day2)) {
											//cout << " Caso dove 2 da a 3 particolare " << endl;
											///prendo da giorno 3 metto in 1 --------------------->>>> QUA FORSE DEVI CAAMBIARE
											//r.Print();
											//cin.get();
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else if ((r.start_end_routes[1][1] >= -1 && r.start_end_routes[1][0] < r.start_end_routes[1][1]) && giorno == 1 && r.primo_pass[1] == 0 && r.time_arr[node_destroy] < end_day2) {
											//cout << " Caso dove 2 da a 3 particolareggiante " << endl;
											///prendo da giorno 3 metto in 1
											//r.Print();
											//cin.get();
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										//r.Print();
										if (r.index >= 2) {
											//cout << " sono alla linea 8193 " << endl;
											if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[1][1] - r.start_end_routes[1][0]) >= 1 && r.capacity[0] == 0) {
												//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
												//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
												r.start_end_routes[1][0] --;
												r.start_end_routes[0][0] = -1;
												r.start_end_routes[0][1] = -1;
												r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
												//r.Print();
												//system("pause");
											}
											else  if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
												//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
												//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
												r.start_end_routes[2][0] --;
												r.start_end_routes[1][0] = -1;
												r.start_end_routes[1][1] = -1;
												r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
												//r.Print();
												//system("pause");
											}
											else  if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0) {
												//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
												//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
												r.start_end_routes[2][0] --;
												r.start_end_routes[0][0] = -1;
												r.start_end_routes[0][1] = -1;
												r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
												//r.Print();
												//system("pause");
											}
											else if (r.capacity[0] == 0) {
												r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
												r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
											}
										}
										//r.Print();
									}
								}
								else if (r.index > (node_destroy - 1)) {
									//cout << " Adesso devo valutare se riesco ad andarci al sera " << endl;
									double time = r.time_dep[node_destroy] + ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
									int limit = 0;
									if (giorno == 0) limit = end_day1;
									else if (giorno == 1) limit = end_day2;
									else limit = end_day3;
									//cout << " CI arriverebbe alle  " << time << " quando c'è un end day " << limit << endl;
									if (node_destroy == 0 && r.index > 2) {
										//cout << " Caso chiesto da Nel adesso stampiamo la route e vediamo se la condizione va bene " << endl;
										//r.Print();
										//system("pause");
										/*cout << "start_end_routes[0][0]" << r.start_end_routes[0][0] << endl;
										cout << "start_end_routes[0][1]" << r.start_end_routes[0][1] << endl;
										cout << "start_end_routes[1][0]" << r.start_end_routes[1][0] << endl;
										cout << "start_end_routes[1][1]" << r.start_end_routes[1][1] << endl;
										cout << "start_end_routes[2][0]" << r.start_end_routes[2][0] << endl;
										cout << "start_end_routes[2][1]" << r.start_end_routes[2][1] << endl;
										cout << " Confrontato con Node Destroy " << node_destroy << endl;
										r.Print();*/
										if (r.start_end_routes[2][0] == -1 && r.start_end_routes[1][1] >= 2 && r.capacity[0] == 0) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei 2 " << endl;
											if (r.start_end_routes[1][0] > 0)r.start_end_routes[1][0] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;


										}
										else if (r.start_end_routes[0][1] >= 1 && r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] >= 1 && r.capacity[0] == 0) {
											//cout << " Caso in cui abbiamo solo gionro 1 e 3 " << endl;
											//cout << " Sei nel caso dove il nodo del secodno giorno va spsostato nei 3 " << endl;
											//cout << " Sei nel caso dove il nodo del primo giorno va spsostato nei gionri nel terzo giorno " << endl;
											if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
											//r.start_end_routes[1][1] --;
											r.start_end_routes[0][0] = -1;
											r.start_end_routes[0][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;



										}
										else if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) == 1 && (r.start_end_routes[2][1] - r.start_end_routes[2][0]) >= 1 && r.capacity[0] == 0 && ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == 0 || ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) == -1 && r.primo_pass[0] == 0))) {
											//cout << " SONO nl caso in cui devo fare uno spostamento per favorire repair " << endl;
											//cout << " Caso in cui abbiamo solo gionro da 2 a 3 " << endl;
											//r.start_end_routes[0][1] --;
											if (r.start_end_routes[2][0] > 0) r.start_end_routes[2][0] --;
											r.start_end_routes[1][0] = -1;
											r.start_end_routes[1][1] = -1;
											r.time_dep[1] = r.time_arr[2] - ((from_to[r.places[1]][r.places[2]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[1] = r.time_dep[1] - map_airstrip[r.places[1]].ground_time;
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;

										}
										else if (r.capacity[0] == 0) {
											r.time_dep[0] = r.time_arr[1] - ((from_to[r.places[0]][r.places[1]]) / map_airplane[r.aircraft_code].speed) * 60;
											r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
										}
										//cout << " DONE " << endl;
										//r.Print();
										//system("pause");
									}
									else if (time <= limit) {
										if (giorno == 0 && (r.start_end_routes[1][0] > -1) && (r.start_end_routes[1][1] > -1)) {
											//cout << " Caso del giorno 0 ho giorno 2 " << endl;
											r.start_end_routes[0][1] ++;
											r.start_end_routes[1][0] ++;
										}
										else if (giorno == 0 && r.start_end_routes[1][0] <= -1 && r.start_end_routes[1][1] <= -1) {
											//cout << " Caso del giorno 0 non ho giorno 2 " << endl;
											r.start_end_routes[0][1] ++;
											r.start_end_routes[2][0] ++;
										}
										else {
											//cout << " Caso che pesno dal giorno 2 e metto in 1 " << endl;
											r.start_end_routes[1][1] ++;
											r.start_end_routes[2][0] ++;
										}
										r.time_arr[node_destroy + 1] = time;
										double limite = start_day1;
										if (r.time_dep[node_destroy + 1] > end_day2) limite = start_day3;
										else if (r.time_dep[node_destroy + 1] > end_day1) limite = start_day2;
										//cout << " Nodo in causa " << r.places[node_destroy + 1] << " limite " << limite << endl;
										if (!(1 == r.places[node_destroy + 1] || 4 == r.places[node_destroy + 1])) if (r.time_dep[node_destroy + 1] < limite) {
											//cout << " Tempo nodo incriminato " << r.time_dep[node_destroy + 1] << " limite " << limite << " giorno " << giorno << endl;
											//r.Print();
											//cin.get();
											goback = true;
										}
									}
									else {
										time = r.time_arr[node_destroy + 1] - ((from_to[r.places[node_destroy]][r.places[node_destroy + 1]]) / map_airplane[r.aircraft_code].speed) * 60;
										//cout << " Punto di snodo diventa il nodo prima di quello distrutto  riga 8247" << endl;
										//cout << " Siamo considerando il giorno " << giorno << endl;
										//r.Print();
										if (!(1 == r.places[node_destroy] || 4 == r.places[node_destroy])) {
											//cout << " Caso in cui devi stare molto attento... node:  " << node_destroy << endl;
											if (node_destroy == r.start_end_routes[0][1] && time < start_day2) {
												//cout << " CAso in cui sto per partite troppo presto 2 doveva partre ad " << time << endl;
												if ((r.time_arr[r.start_end_routes[1][1]] + (start_day2 - time)) <= end_day2) {
													r.time_dep[node_destroy] = start_day2;
													//cout << " dovro aggiungere a tutti " << (start_day2 - time) << endl;
													for (int a = node_destroy + 1; a < r.start_end_routes[1][1]; a++) {
														r.time_arr[a] += (start_day2 - time);
														r.time_dep[a] += (start_day2 - time);
													}
													r.time_arr[r.start_end_routes[1][1]] += (start_day2 - time);
													if (r.index == (r.start_end_routes[1][1] + 1)) r.time_dep[r.start_end_routes[1][1]] += (start_day2 - time);
												}
												else goback = true;
												//r.Print();
												//system("pause");
											}
											else if (node_destroy == r.start_end_routes[2][0] && time < start_day3) {
												//cout << " CAso in cui sto per partite troppo presto nel giorno 3 doveva partre ad " << time << endl;
												if ((r.time_arr[r.start_end_routes[2][1]] + (start_day3 - time)) <= end_day3) {
													r.time_dep[node_destroy] = start_day3;
													//cout << " dovro aggiungere a tutti " << start_day3 - time << endl;
													for (int a = node_destroy + 1; a < r.start_end_routes[2][1]; a++) {
														r.time_arr[a] += (start_day3 - time);
														r.time_dep[a] += (start_day3 - time);
													}
													r.time_arr[r.start_end_routes[2][1]] += (start_day3 - time);
													if (r.index == (r.start_end_routes[2][1] + 1))r.time_dep[r.start_end_routes[2][1]] += (start_day3 - time);
												}
												else goback = true;
												//r.Print();
												//system("pause");
											}
											else r.time_dep[node_destroy] = time;
										}
										else r.time_dep[node_destroy] = time;
										//r.Print();
										//cout << " CASOOOOOOOOOOOOOOOOOOOOOOO CHE DEVIIIIIIIIIIIIII VALUTARE COME TRATTARE " << endl;
										//system("pause");
									}
								}
								r.removePlace_Arc(map_airplane);
								//cout << " Finito procedura h fatto tutto per nodo centrale con refuel " << endl;
								//r.Print();
							}
							// uqa forse ci vuole codice che sosti per favorire il repair
						}
						//else cout << " ------------------!!!!! Non fare nulla !!!!!!!!!!!! -------------------------- " << endl;
					}
				}
				first++;
			} while (check);
			if (!goback) {
				//cout << " Qui ho funzione che azzera le flag dei passeggieri e mettee a posto start_end" << endl;
				bool check_Pass = false;
				//r.Print();
				for (int g = 0; g < 3; g++) {
					if ((r.start_end_routes[g][0] == r.start_end_routes[g][1]) && r.start_end_routes[g][0] != -1) {
						//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
						//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
						//if (g != 0) r.start_end_routes[g][0] = -1;
						//else r.start_end_routes[g][0] = 0;
						r.start_end_routes[g][0] = -1;
						r.start_end_routes[g][1] = -1;
					}


					if (r.primo_pass[g] == 1) {
						check_Pass = false;
						for (Passenger& p : r.passengers_in_route) {
							if (p.giorno == (g + 1)) {
								check_Pass = true;
								break;
							}

						}

						if (!check_Pass) r.primo_pass[g] = 0;

					}


				}
				//cout << " -----------------  Ho finito la distruzione della route  -------------------------- " << endl;
				//r.Print();
			}
			else {
				//r.Print();
				//old.Print();
				//cin.get();
				int now = passenger_removed.size();
				//cout << " Passeggieri rimossi fino a questo momento " << to_string(so_far) << endl;
				//cout << " Passeggieri rimossi Ora: " << to_string(now) << endl;
				//cout << "---------------------------> Route ora ha numero di passeggieri " << r.passengers_in_route.size() << endl;
				r = old;
				passenger_removed.erase(passenger_removed.begin() + so_far, passenger_removed.end());
				//cout << " Dopo erase size dei paseggieri pari a: " << passenger_removed.size() << endl;
				//cin.get();
				//system("pause");
			}
			//cout << " Qui ho funzione che azzera le flag dei passeggieri e mettee a posto start_end" << endl;
			bool check_Pass = false;
			//r.Print();
			for (int g = 0; g < 3; g++) {
				if ((r.start_end_routes[g][0] == r.start_end_routes[g][1]) && r.start_end_routes[g][0] != -1) {
					//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
					//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
					if (g != 0) r.start_end_routes[g][0] = -1;
					else r.start_end_routes[g][0] = 0;
					r.start_end_routes[g][1] = -1;
				}


				if (r.primo_pass[g] == 1) {
					check_Pass = false;
					for (Passenger& p : r.passengers_in_route) {
						if (p.giorno == (g + 1)) {
							check_Pass = true;
							break;
						}

					}

					if (!check_Pass) r.primo_pass[g] = 0;

				}


			}
			//cout << " -----------------  Ho finito la distruzione della route  -------------------------- " << endl;
			//r.Print();

		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
	}


	map<int, vector<int>> map_nodes;
	for (Route r : solution) {
		vector<int> nodes;
		map_nodes.insert(make_pair(r.aircraft_code, nodes));
		for (size_t j = 1; j < r.index - 1; j++) {
			if (r.capacity[j - 1] == 0 && r.capacity[j] == 0) {
				map_nodes[r.aircraft_code].push_back(j);
			}
		}
	}

	for (auto p : map_nodes) {
		//cout << " Aereo con codice " << p.first << endl;
		if (p.second.size() > 0) {
			//for (int i : p.second) cout << " Nodo Teorico: " << i << endl;
		}
		//cout << " -------------------" << endl;
	}

	for (Route& r : solution) {
		//r.Print();
		if (map_nodes[r.aircraft_code].size() > 0) {
			for (int i = map_nodes[r.aircraft_code].size() - 1; i >= 0; i--) {
				//cout << " Devo provare a togliere della Route " << r.aircraft_code << " con Size: " << r.index << " Nodo: " << map_nodes[r.aircraft_code][i] << endl;
				if (r.capacity[map_nodes[r.aircraft_code][i] - 1] == 0 && r.capacity[map_nodes[r.aircraft_code][i]] == 0) {
					//cout << " Autorizzato a rimuovere Nodo " << map_nodes[r.aircraft_code][i] << endl;
					cleaning_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, map_nodes[r.aircraft_code][i], passenger_removed, r, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				}//r.Print();
			}
		}
	}

	for (Route& r : solution) {
		if (r.time_arr[0] + map_airstrip[r.places[0]].ground_time < r.time_dep[0]) r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
		if (r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time < r.time_dep[r.index - 1]) {
			r.time_dep[r.index - 1] = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time;
		}

		if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) > 1 && r.primo_pass[1] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[1] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
		if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) > 1 && r.primo_pass[0] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[0] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
	}


	//cout << " ------------------------------------------------------- Route dopo aver tolto archi vuoti  ------------------------------------------------------------------- " << endl;

}



vector<Route> destroy_thanos(map<string, int>& legenda, double start_day1, double start_day2, double start_day3, int end_day1, int end_day2, int end_day3, double destroy_coef_route, vector<Passenger>& passenger_removed, vector<Route>& solution, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//cout << " INIZIO IL THANOS GOOD LUCK " << endl;
	int index = 0;
	for (Route& r : solution) {
		double f = (double)rand() / RAND_MAX;
		f = f * (10);
		//cout << " Sono alla route: " << r.aircraft_code << " F: " << f << endl;
		if (f <= destroy_coef_route && r.index > 1) {
			//cout << " Devo distruggere la route del aereo " << r.aircraft_code << endl;
			//genero il numero random di nodi da distruggere da 2 a place.size()-1;
			double numero_random = (double)rand() / RAND_MAX;
			if (r.index == 2) {
				numero_random = round(1 + (numero_random*(r.index - 2)));
				//numero_random = 0;
			}
			else {
				numero_random = round(2 + (numero_random*(r.index - 3)));
			}

			//cout << " La route ha index di: " << r.index << " il numero random generato è: " << numero_random << endl;
			//cin.get();
			//system("pause");
			if (numero_random == r.places.size() - 1) {
				//cout << " ------------>>>> DISTRUGGO TUTTO <<<<<------------- " << endl;
				// qua devo distruggere tutta la route e lasciare solo il depot
				for (int i = (int)(r.places.size()) - 1; i >= 1; i--) {
					r.places.erase(r.places.begin() + i);
					r.refueling.erase(r.refueling.begin() + i);
					r.quantity_fuel.erase(r.quantity_fuel.begin() + i);
					r.time_arr.erase(r.time_arr.begin() + i);
					r.time_dep.erase(r.time_dep.begin() + i);
					r.capacity.erase(r.capacity.begin() + i);
					r.weight.erase(r.weight.begin() + i);
					r.index--;
					//r.index_cap--;
					//cout << " start_end_routes[0][1] " << r.start_end_routes[0][1] << " start_end_routes[1][1] " << r.start_end_routes[1][1] << " start_end_routes[2][1] " << r.start_end_routes[2][1] << endl;
				}
				//qua ci dovrebbe essere solo il deposito
				//fisso il peso, capacita e fuel ai valori di default e la variabile passeggeri dentro a false
				r.start_end_routes[0][0] = 0;
				r.start_end_routes[0][1] = -1;
				r.start_end_routes[1][0] = -1;
				r.start_end_routes[1][1] = -1;
				r.start_end_routes[2][0] = -1;
				r.start_end_routes[2][1] = -1;
				r.quantity_fuel[0] = map_airplane[r.aircraft_code].max_fuel;
				r.weight[0] = map_airplane[r.aircraft_code].weight_fuel_people - r.quantity_fuel[0];
				r.capacity[0] = 0;
				//ora devo togliere tutti i passeggeri
				for (int p = r.passengers_in_route.size() - 1; p >= 0; p--) {
					r.passengers_in_route[p].route_before = index;
					passenger_removed.push_back(r.passengers_in_route[p]);
					r.passengers_in_route.erase(r.passengers_in_route.begin() + p);
				}
				//cout << " Ho FINITOOOO di distrugerre tutto adesso ti stampo la  route " << endl;
				//r.Print();
				//system("pause");
			}
			/*
			else if (numero_random > (r.index - 1)*0.8 && (r.primo_pass[0] + r.primo_pass[1] + r.primo_pass[2]) >= 2) {
			cout << " -------->>>>>>> Qua devo distugerre una giornata <<<<<------------ " << endl;
			system("pause");
			}
			*/
			else {
				//cout << " -------->>>>>>> Qua devo pescare un numero di nodi da distruggere pari a numero_random <<<<<------------ " << endl;
				int nodi_rimossi = 0; //finchè non ho rimosso
				int node_destroy = 1;
				int nodi_mancanti = 0;
				//cout << " Sto per iniziare ciclo " << endl;
				do {
					//cout << " HO INIZIATO CICLOOOOOOOOO " << endl;
					node_destroy = (int)(round(1 + ((double)rand() / RAND_MAX) * (r.index - 2)));
					//cout << " Devo provare a togliere della Route " << r.aircraft_code << " il nodo: " << node_destroy << " su un massimo di " << r.index << " Chiamato: " << r.places[node_destroy] << endl;
					nodi_mancanti = (int)(r.index);
					//cout << " ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << endl;
					//cout << " Nodo distrutto " << node_destroy << " size della route " << r.index << endl;
					//cin.get();
					cleaning_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, node_destroy, passenger_removed, r, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
					//cout << " HO FINITO LA FUNZIONE CH EH ADISTRUTTO I NODI ADESSO DEVO CONTROLLARE SE DEVO TOGLIERNE ALTRI " << endl;
					nodi_mancanti -= (int)r.index;
					//cout << " Nodi mancanti dalla Route: " << nodi_mancanti << endl;
					nodi_rimossi += nodi_mancanti;
					//cout << " Nodi rimossi: " << nodi_rimossi << " Nodi da Rimuovere: " << numero_random << endl;
					//cin.get();
				} while (nodi_rimossi <= numero_random);
				//cout << " SONO USCITO DAL CICLOOOOOOOOO " << endl;
			}

			//cout << " Qui ho funzione che azzera le flag dei passeggieri e mettee a posto start_end" << endl;
			bool check_Pass = false;
			//r.Print();
			for (int g = 0; g < 3; g++) {
				if ((r.start_end_routes[g][0] == r.start_end_routes[g][1]) && r.start_end_routes[g][0] != -1) {
					//cout << " r.start_end_routes[i][0] = " << r.start_end_routes[i][0] << endl;
					//cout << " r.start_end_routes[i][1] = " << r.start_end_routes[i][1] << endl;
					if (g != 0) r.start_end_routes[g][0] = -1;
					else r.start_end_routes[g][0] = 0;
					r.start_end_routes[g][1] = -1;
				}


				if (r.primo_pass[g] == 1) {
					check_Pass = false;
					for (Passenger& p : r.passengers_in_route) {
						if (p.giorno == (g + 1)) {
							check_Pass = true;
							break;
						}

					}

					if (!check_Pass) r.primo_pass[g] = 0;

				}


			}
			//cout << " -----------------  Ho finito la distruzione della route  -------------------------- " << endl;



		}
		//cout << "passengers removed" << endl;
		//for (Passenger p : passenger_removed) p.print();
		index++;
		//cout << "stampo caso strano dentro al thanos" << endl;
		//stampo_caso_strano_single_destroy(legenda, map_airstrip, r, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, r);
	}




	map<int, vector<int>> map_nodes;
	for (Route r : solution) {
		vector<int> nodes;
		map_nodes.insert(make_pair(r.aircraft_code, nodes));
		for (size_t j = 1; j < r.index - 1; j++) {
			if (r.capacity[j - 1] == 0 && r.capacity[j] == 0) {
				map_nodes[r.aircraft_code].push_back(j);
			}
		}
	}

	for (auto p : map_nodes) {
		//cout << " Aereo con codice " << p.first << endl;
		if (p.second.size() > 0) {
			//for (int i : p.second) cout << " Nodo Teorico: " << i << endl;
		}
		//cout << " -------------------" << endl;
	}

	for (Route& r : solution) {
		//r.Print();
		if (map_nodes[r.aircraft_code].size() > 0) {
			for (int i = map_nodes[r.aircraft_code].size() - 1; i >= 0; i--) {
				//cout << " Devo provare a togliere della Route " << r.aircraft_code << " con Size: " << r.index << " Nodo: " << map_nodes[r.aircraft_code][i] << endl;
				if (r.capacity[map_nodes[r.aircraft_code][i] - 1] == 0 && r.capacity[map_nodes[r.aircraft_code][i]] == 0) {
					//cout << " Autorizzato a rimuovere Nodo " << map_nodes[r.aircraft_code][i] << endl;
					cleaning_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, map_nodes[r.aircraft_code][i], passenger_removed, r, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				}//r.Print();
			}
		}
	}

	for (Route& r : solution) {
		if (r.time_arr[0] + map_airstrip[r.places[0]].ground_time < r.time_dep[0]) r.time_arr[0] = r.time_dep[0] - map_airstrip[r.places[0]].ground_time;
		if (r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time < r.time_dep[r.index - 1]) {
			r.time_dep[r.index - 1] = r.time_arr[r.index - 1] + map_airstrip[r.places[r.index - 1]].ground_time;
		}

		if ((r.start_end_routes[1][1] - r.start_end_routes[1][0]) > 1 && r.primo_pass[1] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[1] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
		if ((r.start_end_routes[0][1] - r.start_end_routes[0][0]) > 1 && r.primo_pass[0] == 0) {
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
			//r.Print();
			//system("pause");
			r.primo_pass[0] = 1;
			//cout << " CASO IN CUI BISOGNA CAMBIARE PRIMO PASSEGGIERO PERCHE CI SONO DEI NODI " << endl;
		}
	}

	//cout << " ------------------------------------------------------- Route dopo aver tolto archi vuoti  ------------------------------------------------------------------- " << endl;
	return solution;
}





// --------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------- RICERCA LOCALE -------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------

double cost_single_route(double peso_intermediate_stop, Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//double cost = 0.0;
	double cost = map_airplane[r.aircraft_code].fixed_cost;
	//for (int i = 1; i < r.index; i++) cost += map_airstrip[r.places[i]].landing_cost;

	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	//double first_hour = 0.0;
	//double second_hour = 0.0;
	for (int i = 0; i < r.index; i++) {
		//double time_travel = 0.0;
		if (i >= 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
		}

		if (i < r.index - 1) {
			if (i == 0 && r.capacity[i] == 0) {
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}
		}

	}


	cost += mileage;
	//cost += first_hour * map_airplane[r.aircraft_code].fuel_burn_first + second_hour * map_airplane[r.aircraft_code].fuel_burn_second;
	cost += fuel_consumed;

	for (auto& p : r.passengers_in_route) {

		//double time_departure = r.time_arr[p.solution_from];
		//double time_arrival = r.time_arr[p.solution_to];
		cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop

		double TW_departure = 0.0;
		if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
		else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
		double TW_arrival = 0.0;
		if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
		else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
		cost += TW_departure + TW_arrival;
	}

	return cost;
}

bool route_feasible_Alldays(Route& r, map<int, Airplane>& map_airplane, double st1, double st2, double st3, double ed1, double ed2, double ed3, vector<vector<double>>&  from_to, vector<vector<double>>&  location_fuel, vector<vector<vector<double>>>&  from_to_FuelConsumed) {
	double end_day = 0;
	double start_day = 0.0;
	bool feasible = true;
	for (int giorno = 0; giorno < 3; giorno++) {
		if (r.start_end_routes[giorno][1] > -1) {
			if (giorno == 0) {
				end_day = ed1;
				start_day = st1;
			}
			else if (giorno == 1) {
				end_day = ed2;
				start_day = st2;
			}
			else {
				end_day = ed3;
				start_day = st3;
			}
			if (r.places[r.start_end_routes[giorno][0]] != 1 && r.places[r.start_end_routes[giorno][0]] != 4) {
				if (r.time_dep[r.start_end_routes[giorno][0]] < start_day) {
					//cout << "infeasible per l'inizio della giornata, dovrebbe impossibile che arrivi una route così" << endl;
					feasible = false;
				}

			}

			//check and day
			//cout << r.time_arr[r.start_end_routes[giorno][1]] << " > " << end_day << endl;
			if (r.time_arr[r.start_end_routes[giorno][1]] > end_day) {
				feasible = false;
				//cout << "infeasible per l'end day" << endl;
			}

			//for the last-trip
			//cout << r.quantity_fuel[r.start_end_routes[giorno][1]] << " < " << location_fuel[r.aircraft_code + "/" + r.places[r.start_end_routes[giorno][1]]] << endl;
			if (r.quantity_fuel[r.start_end_routes[giorno][1]] < map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][r.places[r.start_end_routes[giorno][1]]]) {
				feasible = false;
				//cout << "infeasible per il last trip" << endl;
			}

			if (feasible) {
				for (int i = r.start_end_routes[giorno][0]; i <= r.start_end_routes[giorno][1]; i++) {
					if (i >= 1) {
						//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
						if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
							feasible = false;
							//cout << "infeasible per la quantità di fuel in " << i << endl;
						}
						if (r.refueling[i] && !r.refueling[i - 1]) {
							double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
							if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
								feasible = false;
								//cout << "infeasible per la quantità di fuel in " << i << endl;
							}
						}
					}
					if (i < r.index - 1) {
						if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
							feasible = false;
							//cout << "infeasible per la capacità" << endl;
						}
					}

				}
			}

			if (feasible) {
				//check of the intermediate stop
				for (Passenger& p : r.passengers_in_route) {
					if (p.giorno == giorno + 1) {
						if (p.solution_to - p.solution_from > p.stop) {
							feasible = false;
							//cout << "infeasible per le intermediate stop" << endl;
						}
					}
				}
			}








		}
	}
	return feasible;
}

// Forti dubbi che le due sopra siano giuste <<<<----------------------------------------------------------------------

int sequential_same_node(Route& r) {
	int node = -1;
	for (int i = 0; i < r.places.size() - 1; i++) {
		if (r.places[i] == r.places[i + 1]) {
			node = i;
			break;
		}
	}
	return node;
}


int swap_from(int node, Route& r) {

	int swap_from = 0;

	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}


	return swap_from;

}

int swap_to(int node, Route& r) {
	int swap_to = 1000;

	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}

	}


	return swap_to;
}


bool swap_is_allowed(int A, int B, Route& r) {
	bool swap = false;
	if (B < swap_to(A, r) && swap_from(B, r) < A) swap = true;
	return swap;
}

int move_from(int node, Route& r) {

	int swap_from = 0;

	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_to == node) {
			if (p.solution_from > swap_from) swap_from = p.solution_from;
		}
	}


	return swap_from;

}


int move_to(int node, Route& r) {
	int swap_to = 1000;

	for (Passenger& p : r.passengers_in_route) {
		if (p.solution_from == node) {
			if (p.solution_to < swap_to) swap_to = p.solution_to;
		}

	}


	return swap_to;
}

bool move_is_allowed(int A, int B, Route& r) {
	bool move = false;
	if (B > A && B < move_to(A, r))move = true;
	if (B < A && move_from(A, r) < B) move = true;
	return move;
}

bool move_flightleg_is_allowed(int A, Route& r) {
	bool move = true;
	//cout << " Sono dentro al controllo per sapere se è consentito " << endl;
	//cout << " sto guardando la route" << r.aircraft_code << " con il nodo " << A << endl;
	for (Passenger& p : r.passengers_in_route) {
		//cout << " sto facendo il passeggiero: ";
		//p.print();
		if ((p.solution_from == A) && (p.solution_to != (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_from != A) && (p.solution_to == (A + 1))) {
			move = false;
			break;
		}
		if ((p.solution_to == A) || (p.solution_from == (A + 1))) {
			move = false;
			break;
		}
		//cout << " Move = " << move << endl;
	}
	return move;
}

Route update_route_after_move(int giorno, int A, int B, Route r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	//cout << " Stampo Route di partenza " << endl;
	//r.Print();
	r_new.aircraft_code = r.aircraft_code;
	for (int g = 0; g < 3; g++) {
		r_new.primo_pass[g] = r.primo_pass[g];
		r_new.start_end_routes[g][0] = r.start_end_routes[g][0];
		r_new.start_end_routes[g][1] = r.start_end_routes[g][1];
		//cout << " r_new.start_end_routes[g][0] " << r_new.start_end_routes[g][0] << " e r_new.start_end_routes[g][1] " << r_new.start_end_routes[g][1] << endl;
	}
	/*cout << " Questa è la vecchia routes: " << endl;
	//r.print();
	*/
	//cout << "dentro a route after move -- A -> " << A << " B --> " << B << endl;
	//cout << "sto inserendo i vari posti nella nuova route" << endl;

	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	if (B > A) {
		//cout << " Sono nel caso in cui B > A " << endl;
		for (int i = 1; i < r.index; i++) {
			//cout << " Sono nel indice i --> " << i << endl;
			if ((i < A) || (i > B)) {
				//cout << " caso (i < A) || (i > B) in questo casorimane come prima devo mettere normalmente quello che c'è in i " << endl;
				//cout << r.places[i] << endl;
				// Caso in cui rimane come prima
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if ((i >= A) && (i < B)) {
				//cout << " caso (i >= A) && (i < B) in questo caso devo metterquello che c'è in i+1 " << endl;
				//cout << r.places[i+1] << endl;
				//cout << "stampo nel caso indicato dal bed allocator i+1" << endl;
				//cout << "i+1: " << i+1 << " r.places[i+1]: " << r.places[i+1] << endl;
				r_new.addPlace(r.places[i + 1], r.refueling[i + 1], r.quantity_fuel[i + 1], 0.0, 0, r.time_arr[i + 1], r.time_dep[i + 1]);
			}
			else if (i == B) {
				//cout << " caso i = (B) in questo caso devo mettere normalmente A " << endl;
				//cout << r.places[A] << endl;
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
		}
	}
	else {
		//cout << " Sono nel caso in cui B < A " << endl;
		for (int i = 1; i < r.index; i++) {
			//cout << " Sono nel indice i --> " << i << endl;
			if (i < (B + 1)) {
				//cout << " caso i < (B + 1) in questo caso devo mettere normalmente quello che c'è in i " << endl;
				//cout << r.places[i] << endl;
				//in questo posto ci devo mettere normalmente i
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else if (i == (B + 1)) {
				//cout << " caso i = (B + 1) in questo caso devo mettere normalmente A " << endl;
				//cout << r.places[A] << endl;
				//in questo posto ci devo mettere A
				//cout << "stampo nel caso indicato dal bed allocator A" << endl;
				//cout << "ecco l'index: " << r.index << endl;
				//cout << "indice i: " << i << endl;
				//cout << "A: " << A << " r.places[A]: " << r.places[A] << endl;
				r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
			}
			else if (i >= A + 1) {
				//in questo posto ci devo mettere normalmente i-1
				//cout << " caso (i >= A +1) in questo caso devo mettere normalmente quello che c'è in i" << endl;
				//cout << r.places[i-1] << endl;
				//cout << "stampo nel caso indicato dal bed allocator" << endl;
				//cout << "i: " << i << " r.places[i]: " << r.places[i] << endl;
				r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
			}
			else {
				//cout << " caso i > (B + 1) in questo caso devo mettere normalmente quello che c'è in i - 1" << endl;
				//cout << r.places[i - 1] << endl;
				r_new.addPlace(r.places[i - 1], r.refueling[i - 1], r.quantity_fuel[i - 1], 0.0, 0, r.time_arr[i - 1], r.time_dep[i - 1]);
			}
		}
	}
	//cout << "ho finito di inserire i vari posti nella nuova route" << endl;
	//cout << "aggiorno i tempi e il fuel" << endl;
	//aggiorno i tempi e fuel senza aver considerato il probabile peso negativo, il paso qua è come se lo inizializzassi
	if (giorno == 0) {
		//cout << " CAso in cui le modifiche le devo applicare al primo giorno " << endl;
		for (int i = 0; i < r_new.index; i++) {
			//cout  << " I : " << i << endl;
			if (i > 0 && i < r.start_end_routes[giorno][1]) {
				//cout << "from: " << r_new.places[i - 1] << endl;
				//cout << "to: " << r_new.places[i] << endl;
				//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i > r.start_end_routes[giorno][1]) {
				r_new.time_arr[i] = r.time_arr[i];
				r_new.time_dep[i] = r.time_dep[i];

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i == 0) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i == r.start_end_routes[giorno][1] && i != (r_new.index - 1)) {
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r.time_dep[i];

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i == r.start_end_routes[giorno][1] && i == (r_new.index - 1)) {
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
		}
	}
	else if (giorno == 1) {
		//cout  << " CAso in cui le modifiche le devo applicare al secodno giorno " << endl;
		for (int i = 0; i < r_new.index; i++) {
			//cout << " I : " << i << endl;
			if (i == 0) {
				//cout  << " I =  0" << endl;
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i > r.start_end_routes[giorno][0] && i < r.start_end_routes[giorno][1]) {
				//cout  << " Aggiornamento nell'aerea modidficata " << endl;
				//cout << "from: " << r_new.places[i - 1] << endl;
				//cout << "to: " << r_new.places[i] << endl;
				//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i > r.start_end_routes[giorno][1] || i < r.start_end_routes[giorno][0]) {
				//cout << " Areee in cui devi lasciare come prima " << endl;
				r_new.time_arr[i] = r.time_arr[i];
				r_new.time_dep[i] = r.time_dep[i];
				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else {
				if (i == r.start_end_routes[giorno][1] && i != (r_new.index - 1)) {
					//cout  << " Caso in cui devo aggiornare il nodo di cong finale della giornata " << endl;
					//cout  << " Tempo di partenza rimane uguale a prima " << endl;
					//cout  << " Cambio tempo di arrivvo lo setto guardando quello prima  " << r_new.time_dep[i - 1] << " +tempo di volo" << endl;
					r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
					r_new.time_dep[i] = r.time_dep[i];
				}
				else if (i == r.start_end_routes[giorno][1] && i == (r_new.index - 1)) {
					r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
					r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;
				}
				else if (i == r.start_end_routes[giorno][0]) {
					//cout  << " Caso in cui devo aggiornare il nodo di cong iniziale della giornata " << endl;
					//cout  << " Tempo di arrivo rimane uguale a prima " << endl;
					//cout  << " Cambio tempo di partenza lo setto guardando arrivo in quello dopo  " << r_new.time_arr[i + 1] << " -tempo di volo" << endl;
					r_new.time_arr[i] = r.time_arr[i];
					r_new.time_dep[i] = r_new.time_arr[i + 1] - (((from_to[r_new.places[i]][r_new.places[i + 1]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				}
				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
		}
	}
	else {
		//cout  << " CAso in cui le modifiche le devo applicare al secodno giorno " << endl;
		for (int i = 0; i < r_new.index; i++) {
			//cout << " I : " << i << endl;
			if (i == 0) {
				//cout  << " Primo nodo " << endl;
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i > r.start_end_routes[giorno][0] && i <= r.start_end_routes[giorno][1]) {
				//cout << "from: " << r_new.places[i - 1] << endl;
				//cout << "to: " << r_new.places[i] << endl;
				//cout << "from to: " << (from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) << endl;
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i < r.start_end_routes[giorno][0]) {
				//cout  << " CAso in cui devo lasciare tutto uguale " << endl;
				r_new.time_arr[i] = r.time_arr[i];
				r_new.time_dep[i] = r.time_dep[i];

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
			else if (i == r.start_end_routes[giorno][0]) {
				//cout << " CAso in cui devo cambiare nodo di inizio della giornaat da modificare " << endl;
				//cout  << " Caso in cui devo aggiornare il nodo di cong iniziale della giornata " << endl;
				//cout  << " Tempo di arrivo rimane uguale a prima " << endl;
				//cout  << " Cambio tempo di partenza lo setto guardando arrivo in quello dopo  " << r_new.time_arr[i + 1] << " -tempo di volo" << endl;
				r_new.time_arr[i] = r.time_arr[i];
				r_new.time_dep[i] = r_new.time_arr[i + 1] - (((from_to[r_new.places[i]][r_new.places[i + 1]]) / map_airplane[r_new.aircraft_code].speed) * 60);

				//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];
				/*if (time_fly <= 60) {
				fuel_consumed += (time_fly / 60)*map_airplane[r_new.aircraft_code].fuel_burn_first;
				}
				else {
				fuel_consumed += map_airplane[r_new.aircraft_code].fuel_burn_first + ((time_fly - 60) / 60)*map_airplane[r_new.aircraft_code].fuel_burn_second;
				}*/

				//cout << "fuel consumed: " << fuel_consumed << endl;

				if (r_new.refueling[i]) {
					r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
				}
				else {
					r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
				}
				r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
			}
		}
	}
	//cout << "ho finito di aggiornare i tempi e il fuel" << endl;
	//if(giorno >0)r_new.Print();
	//cout << "aggiorno i i passeggeri" << endl;
	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri
	//cout << " A -> " << A << " B --> " << B << endl;
	for (Passenger p : r.passengers_in_route) {
		//cout << " Ho preso il passeggiero: " << endl;
		//cout << " Nome : " << p.name << " che ha from: " << p.solution_to << " e to: " << p.solution_to << endl;
		if (p.solution_from == A && A > B) {
			//cout << " Modifico from = B+1 pperche A > B " << endl;
			p.solution_from = B + 1;
		}
		else if (p.solution_to == A && A > B) {
			//cout << " Modifico to = B+1 pperche A > B " << endl;
			p.solution_to = B + 1;
		}
		else if (p.solution_to == A && A < B) {
			//cout << " Modifico to = B perchè A < B " << endl;
			p.solution_to = B;
		}
		else if (p.solution_from == A && A < B) {
			//cout << " Modifico from = B perchè A < B  " << endl;
			p.solution_from = B;
		}
		else if (p.solution_from <= B && B < p.solution_to && A > p.solution_to) {
			//cout << " Incremento il to di 1  caso A > B" << endl;
			p.solution_to++;
		}
		else if (p.solution_from <= B && B < p.solution_to && A < p.solution_from) {
			//cout << " Decremento il from di 1  caso A < B" << endl;
			p.solution_from--;
		}
		else if (p.solution_from > B &&  A > p.solution_to) {
			//cout << " Terza casistica A era dopo richiesta adesso va orima" << endl;
			p.solution_from++;
			p.solution_to++;

		}
		else if (p.solution_from < A &&  A < p.solution_to && B < p.solution_from) {
			//cout << " Quarta casistica A" << endl;
			p.solution_from++;
		}
		else if (p.solution_from < A &&  A < p.solution_to && B >= p.solution_to && A < B) {
			//cout << " Quarta casistica B" << endl;
			p.solution_to--;
		}
		else if (p.solution_from > A &&  A < p.solution_to && B >= p.solution_to) {
			//cout << " Quinta casistica Nodo va da prima della richiesta a dopo" << endl;
			p.solution_to--;
			p.solution_from--;
		}
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] ++;
			r_new.weight[t] -= p.weight;
		}
		//cout << " Nome : " << p.name << " che ha from AGGIORNATO: " << p.solution_from << " e to AGGIORNATO: " << p.solution_to << endl;
	}
	/*cout << "finito" << endl;
	r_new.print();
	cout << "aggiorno il fuel se peso negativo" << endl;
	//aggiorno fuel se il peso è negatico */
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			// cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					// cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			//cout << " r_new.quantity_fuel[index_refueling] : " << r_new.quantity_fuel[index_refueling] << " +=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			//cout << " r_new.weight[index_refueling] : " << r_new.weight[index_refueling] << " -=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.weight[index_refueling] -= r_new.weight[i];
			//cout << " Adesso tolgo ai nodi senza possibilità di fare refuel successvi " << endl;
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					//cout << " Dopo Fuel era = " << r_new.quantity_fuel[j] << endl;
					//cout << " Prima Weight era = " << r_new.weight[j] << endl;
					r_new.weight[j] -= Update_value;
					//cout << " Dopo Weight era = " << r_new.weight[j] << endl;
				}
			}

			//r_new.weight[i] = 0;

		}
	}
	//cout << "finito upgrade della funzione adesso la stampo avra numero di index: " << r_new.index << endl;
	//if (giorno > 0)r_new.Print();

	return r_new;
}


vector <Route> move(map<string, int>& legenda, double peso_intermediate_stop, vector<Route> routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_move;

	for (Route r : routes) {
		//cout << " ---------------------->>>>>>>>>>>>>>>>>> sto provando a fare il move in questa route: " << r.aircraft_code << endl;
		//r.Print();
		Route r_support = r;
		for (int g = 0; g < 3; g++) {
			//cout << " Giorno numero " << g << endl;
			//cout << " Da r_support.start_end_routes[g][0] " << r_support.start_end_routes[g][0] << " a r_support.start_end_routes[g][1]: " << r_support.start_end_routes[g][1] << endl;
			if (r_support.start_end_routes[g][1] > 0) {
				for (int A = r_support.start_end_routes[g][0] + 1; A < r_support.start_end_routes[g][1]; A++) {
					//cout << " Considero di spostare nodo -> " << A << " partenza da " << r_support.start_end_routes[g][0] + 1 << endl;
					// we have to move the Node A form its position to th enext after B
					for (int B = r_support.start_end_routes[g][0] + 1; B < r_support.start_end_routes[g][1]; B++) {
						if (A != B && B != (A - 1) && B != (A - 2)) {    //Il caso B != (A-1) || B!= (A-2) sono casi che valutiamo gia quando sposriamo avanti
							if (move_is_allowed(A, B, r_support)) {
								//r.Print();
								Route r_new = update_route_after_move(g, A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
								//r_new.Print();
								double cost_route_support = cost_single_route(peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								double cost_route_new = cost_single_route(peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								//cout  << "il costo di routing vecchia: " << costo_senza_time_windows_onre_route(r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
								//cout  << "il costo di TW+IS vecchia: " << costo_time_windows_and_intermediate_stop_one_route(peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to) << endl;
								//cout  << "il costo di routing Nuova: " << costo_senza_time_windows_onre_route(r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
								//cout  << "il costo di TW+IS Nuova: " << costo_time_windows_and_intermediate_stop_one_route(peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to) << endl;
								//system("pause");
								//cout << "stampo caso strano in move prima di route feasible all day" << endl;
								//stampo_caso_strano_single_move(legenda, map_airstrip, r_support, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, r);

								if (cost_route_support > cost_route_new && route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed)) {
									//stampo_caso_strano_singleroute(map_airstrip, r_new, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, r_new);
									//cout << " CONSENTITO e Risparmio sono nel main -----> provo a fare move da " << A << " e " << B << " nel gionro: " << g << endl;
									//cout << " OK ! dopo move Migliora di " << cost_route_support - cost_route_new << "------- Ma prima devo controllare che non sia con nodi doppi e fatto stampa caso strano" << endl;
									//cout << " Costo Nuova:   " << cost_route_new << endl;
									//r_new.Print();
									//system("pause");
									int node = sequential_same_node(r_new);
									//cout  << "node fuori dal while: " << node << endl;
									//r_new.Print();
									bool fatto = false;
									int num_aggregazioni = 0;
									while (node != -1) {
										//cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
										//r_new.Print();
										// i have two airports close to each other
										//cout << " Nodo dove inizia coppia --> " << node << endl;
										//cout << "******* Aggrego nodo in posizione **********" << node << endl;
										aggregate_same_nodes_destoy_rolling_local_search(r_new, node);

										//cout << " DOVREI AVER TOLTO IL NODO  " << endl;
										//r_new.Print();
										if (A > node) num_aggregazioni++;
										//cout << " Ho finito codice di aggregazione " << endl;
										cost_route_new = cost_single_route(peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
										//cout  << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible_Alldays(r_new, map_airplane, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed) << endl;
										//system("pause");
										node = sequential_same_node(r_new);
										//cout << "node dentro al while: " << node << endl;
										r_new.Print();
										fatto = true;
									}
									//stampo_caso_strano_singleroute(map_airstrip, r_new, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, r_new);
									//cout << " Ok non aveva nodi doppi ! " << endl;
									if (fatto == false) r_support = r_new;
									else {
										if (cost_route_support > cost_route_new && route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed)) {
											r_support = r_new;
											cost_route_support = cost_route_new;
											//cout << " Ho finito while di aggregazione ma devo aggiornare A tolgo " << num_aggregazioni << endl;
											A -= num_aggregazioni;
											if (A <= 0) A = 1;
											//riga aggiunta da nelli
											if (A == r_support.start_end_routes[g][1]) A = r_support.start_end_routes[g][1] - 1;
											if (A <= r_support.start_end_routes[g][0]) A = r_support.start_end_routes[g][0] + 1;
										}
									}
								}
							}
						}

					}
				}
			}
		}


		//Modifico Fuel quando non massimo
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {  // SECONDO ME QUA NON CI VA <=
				//cout << " Sto guardando il nodo " << i << endl;
				//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
				//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		//cout << " Ho finito per questa Route la stampo " << endl;
		//cout << " Costo Nuova:   " << cost_single_route(peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
		//r_support.Print();
		//system("pause");

		routes_after_move.push_back(r_support);

	}
	//cout << " Ho finito La MOVE Local Search  " << endl;
	//for (Route r : routes_after_move) cout << " Costo Route:   " << cost_single_route(peso_intermediate_stop, r, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	//system("pause");
	return routes_after_move;
}


Route update_route_after_swap(int gg, int A, int B, Route r, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	Route r_new;
	r_new.aircraft_code = r.aircraft_code;
	for (int g = 0; g < 3; g++) {
		r_new.primo_pass[g] = r.primo_pass[g];
		r_new.start_end_routes[g][0] = r.start_end_routes[g][0];
		r_new.start_end_routes[g][1] = r.start_end_routes[g][1];
	}
	//cout << "sto inserendo i vari posti nella nuova route" << endl;
	r_new.addPlace(r.places[0], r.refueling[0], map_airplane[r.aircraft_code].max_fuel, 0.0, 0, r.time_arr[0], r.time_dep[0]);
	for (int i = 1; i < r.index; i++) {
		if (i == A) {
			//in questo posto ci devo mettere B
			r_new.addPlace(r.places[B], r.refueling[B], r.quantity_fuel[B], 0.0, 0, r.time_arr[B], r.time_dep[B]);
		}
		else if (i == B) {
			//in questo posto ci devo mettere A
			r_new.addPlace(r.places[A], r.refueling[A], r.quantity_fuel[A], 0.0, 0, r.time_arr[A], r.time_dep[A]);
		}
		else {
			//in questo posto ci devo mettere normalmente i
			r_new.addPlace(r.places[i], r.refueling[i], r.quantity_fuel[i], 0.0, 0, r.time_arr[i], r.time_dep[i]);
		}
	}
	/*
	cout << "-------------->>>>>>>>>>>>>< ho finito di inserire i vari posti nella nuova route" << endl;
	cout << "start_end_routes[0][0] " << r_new.start_end_routes[0][0] << endl;
	cout << "start_end_routes[0][1] " << r_new.start_end_routes[0][1] << endl;
	cout << "start_end_routes[1][0] " << r_new.start_end_routes[1][0] << endl;
	cout << "start_end_routes[1][1] " << r_new.start_end_routes[1][1] << endl;
	cout << "start_end_routes[2][0] " << r_new.start_end_routes[2][0] << endl;
	cout << "start_end_routes[2][1] " << r_new.start_end_routes[2][1] << endl;
	cout << "r.primo_pass[0] " << r_new.primo_pass[0] << endl;
	cout << "r.primo_pass[1] " << r_new.primo_pass[1] << endl;
	cout << "r.primo_pass[2] " << r_new.primo_pass[2] << endl;
	*/

	for (int i = 0; i < r_new.index; i++) {
		//cout << " Nodo i: " << i << endl;
		if (i > 0) {
			if ((i == r_new.start_end_routes[0][1] || i == r_new.start_end_routes[1][1]) && i != (r_new.index - 1)) {
				//cout << " Situazione nodo di collegamento " << i << endl;
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r.time_dep[i];
			}
			else {
				//cout << " Situazione in mezzo, nodo: " << i << endl;
				r_new.time_arr[i] = r_new.time_dep[i - 1] + (((from_to[r_new.places[i - 1]][r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
				r_new.time_dep[i] = r_new.time_arr[i] + map_airstrip[r_new.places[i]].ground_time;
			}
			//double time_fly = (((from_to[r_new.places[i - 1] + ";" + r_new.places[i]]) / map_airplane[r_new.aircraft_code].speed) * 60);
			double fuel_consumed = from_to_FuelConsumed[r_new.aircraft_code][r_new.places[i - 1]][r_new.places[i]];

			if (r_new.refueling[i]) {
				r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			}
			else {
				r_new.quantity_fuel[i] = r_new.quantity_fuel[i - 1] - fuel_consumed;
			}
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];

		}
		else {
			r_new.quantity_fuel[i] = map_airplane[r_new.aircraft_code].max_fuel;
			r_new.weight[i] = map_airplane[r_new.aircraft_code].weight_fuel_people - r_new.quantity_fuel[i];
		}
	}
	//cout << "ho finito di aggiornare i tempi e il fuel" << endl;

	//cout << "aggiorno i i passeggeri" << endl;
	//aggiorno capacita e peso e indici dei passeggeri
	//ora devo cambiare gli indici dei passeggeri spostati
	for (Passenger p : r.passengers_in_route) {
		if (p.solution_from == A) p.solution_from = B;
		else if (p.solution_to == A) p.solution_to = B;
		else if (p.solution_from == B) p.solution_from = A;
		else if (p.solution_to == B) p.solution_to = A;
		r_new.passengers_in_route.push_back(p);

		for (int t = p.solution_from; t < p.solution_to; t++) {
			r_new.capacity[t] ++;
			r_new.weight[t] -= p.weight;
		}
	}
	//cout << "finito" << endl;

	//cout << "aggiorno il fuel se peso negativo" << endl;
	//aggiorno fuel se il peso è negatico
	for (int i = 0; i < r_new.index; i++) {
		//cout << " Sono all inizio del For valutabdo il nodo " << i << endl;
		if (r_new.weight[i] < 0) {
			// cout << " Trovato nodo con peso negativo in --> " << i << endl;
			int index_refueling = i;
			for (int t = i; t >= 0; t--) {
				if (r_new.refueling[t]) {
					index_refueling = t;
					// cout << " Index dove si fa refuel prima o coincidente a valore di peso negativo " << index_refueling<<  endl;
					break;
				}
			}
			double Update_value = r_new.weight[i];
			//cout << " r_new.quantity_fuel[index_refueling] : " << r_new.quantity_fuel[index_refueling] << " +=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.quantity_fuel[index_refueling] += r_new.weight[i];
			//cout << " r_new.weight[index_refueling] : " << r_new.weight[index_refueling] << " -=  r_new.weight[i] : " << r_new.weight[i] << endl;
			r_new.weight[index_refueling] -= r_new.weight[i];
			//cout << " Adesso tolgo ai nodi senza possibilità di fare refuel successvi " << endl;
			for (int j = index_refueling + 1; j < r_new.index; j++) {
				if (r_new.refueling[j]) break;
				else {
					r_new.quantity_fuel[j] += Update_value;
					//cout << " Dopo Fuel era = " << r_new.quantity_fuel[j] << endl;
					//cout << " Prima Weight era = " << r_new.weight[j] << endl;
					r_new.weight[j] -= Update_value;
					//cout << " Dopo Weight era = " << r_new.weight[j] << endl;
				}
			}

			//r_new.weight[i] = 0;

		}
	}
	//cout << " Finito di aggiornare tutto " << endl;
	//r_new.Print();


	return r_new;
}

vector <Route> swap(map<string, int>& legenda, double peso_intermediate_stop, vector<Route> routes, map<int, Airplane>& map_airplane, map<int, Airstrip>& map_airstrip, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	vector<Route> routes_after_swap;
	bool update = false;
	for (Route r : routes) {
		update = false;
		//cout << " sto provando a fare lo swap in questa route: " << r.aircraft_code << endl;
		//cout << " Con costo iniziale pari a ... " << cost_single_route(peso_intermediate_stop, r, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
		Route r_support = r;
		for (int g = 0; g < 3; g++) {
			
			if (r_support.start_end_routes[g][1] > 0) {
				for (int A = r_support.start_end_routes[g][0] + 1; A < r_support.start_end_routes[g][1]; A++) {
					for (int B = A + 1; B < r_support.start_end_routes[g][1]; B++) {
						if (swap_is_allowed(A, B, r_support)) {
							Route r_new = update_route_after_swap(g, A, B, r_support, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
							//cout << "ho finito l'update" << endl;
							double cost_route_support = cost_single_route(peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							double cost_route_new = cost_single_route(peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "Costo vecchia Route: " << cost_route_support << "  > costo della Nuova: " << cost_route_new << " &&  route feasible: " << route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed) << endl;
							if (cost_route_support > cost_route_new && route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed)) {
								/*cout << " Giorno numero " << g << endl;
								cout << " Da r_support.start_end_routes[g][0] " << r_support.start_end_routes[g][0] << " a r_support.start_end_routes[g][1]: " << r_support.start_end_routes[g][1] << endl;
								cout << "*************************   SWAP fra " << A << " e " << B << " siamo nel giorno " << g << " A dovrebbe poter partire da: " << r_support.start_end_routes[g][0] + 1 << " e finire in al massimo " << r_support.start_end_routes[g][1] - 1 << endl;
								cout << cost_route_support << " c_supp > c_new " << cost_route_new << " &&  route feasible: " << route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed) << endl;
								cout << " OK ! dopo move Migliora ------- Ma prima devo controllare che non sia con nodi doppi " << endl;
								cout << " sto provando a fare lo swap in questa route: " << r.aircraft_code << endl;
								cout << " Con costo iniziale pari a ... " << cost_single_route(peso_intermediate_stop, r, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
								cout << "*************************   SWAP fra " << A << " e " << B << " siamo nel giorno " << g << " A dovrebbe poter partire da: " << r_support.start_end_routes[g][0] + 1 << endl;
								*/
								update = true;
								//stampo_caso_strano_single_move(legenda, map_airstrip, r_new, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, r);
								int node = sequential_same_node(r_new);
								bool fatto = false;
								while (node != -1) {
									//cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ERROR STESSI NODI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
									//r_new.Print();
									//cin.get();
									// i have two airports close to each other
									//cout << " Nodo dove inizia coppia --> " << node << endl;
									//cout << "******* Aggrego nodo in posizione **********" << node << endl;
									aggregate_same_nodes_destoy_rolling_local_search(r_new, node);
									//cout << " DOVREI AVER TOLTO IL NODO  AGGREGATO " << endl;
									//r_new.Print();
									cost_route_new = cost_single_route(peso_intermediate_stop, r_new, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << " Deve valere ANCORA che " << cost_route_support << "  >  " << cost_route_new << " &&  route feasible: " << route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed) << endl;
									//cin.get();
									fatto = true;
									node = sequential_same_node(r_new);
								}
								//cout << " Ok non aveva  piu nodi doppi ! " << endl;
								if (fatto == false) {
									r_support = r_new;
									A = r_support.start_end_routes[g][0] + 1;
									B = A + 1;
								}
								else {
									if (cost_route_support > cost_route_new && route_feasible_Alldays(r_new, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed)) {
										//cout << " Vuol dire che ho finto aggregazioen nodi doppi è sto ancora risparmiano " << endl;
										r_support = r_new;
										A = r_support.start_end_routes[g][0] + 1;
										B = A + 1;
										cost_route_support = cost_route_new;
										//cout << " Nuovo costo della Route pari a: " << cost_route_support << endl;
										//cout << " Stampo la Route non deve avere nodi doppi .. " << endl;
										//r_support.Print();
										//cin.get();
									}
								}
							}
							else {
								//cout << " Non mi conviene rimango con la Route Vecchia .... " << endl;
								//cout << " Con un costo pari a ... " << cost_route_support << endl;

							}
						}

					}
				}
				//Modifico Fuel quando non massimo

			}
		}
		if (update) r_support.Print();
		for (int k = 0; k < r_support.index; k++) {
			if (r_support.refueling[k] && r_support.quantity_fuel[k] < map_airplane[r_support.aircraft_code].max_fuel) { //&& k!= node_destroy
			//cout << " Sto valutando il caso del nodo " << k << endl;
				int Node_min = k;
				double min_weight = r_support.weight[k];
				int index_updating_from = k;
				int index_updating_to = r_support.index;  //qua prima c'era -1
				for (int i = k + 1; i < r_support.index; i++) {
					//cout << " Sto guardando il nodo " << i << endl;
					//if (!(r.weight[i] > 0 && r.quantity_fuel[i] == map_airplane[r.aircraft_code].max_fuel)) {
					//cout << " Ho passato IF di controllo " << endl;
					if (r_support.refueling[i]) break;
					if (r_support.weight[i] < min_weight) {
						min_weight = r_support.weight[i];
						Node_min = i;
					}
					//}
				}
				//cout << " Nodo di minimo ---> " << Node_min << endl;
				//cout << " Valore di minimi --> " << min_weight << endl;
				if (Node_min >= 0) {
					/*
					for (int i = Node_min; i >= 0; i--) {
					if (r.refueling[i] && i != node_destroy) {
					index_updating_from = i;
					break;
					}
					}
					*/
					for (int i = k + 1; i < r_support.index; i++) {
						if (r_support.refueling[i]) {   // && i != node_destroy ho tolto questo perchè se no se oltre quel nodo non c'è ne erano altri di fuell non trovavo un to
							index_updating_to = i;
							break;
						}
					}
					//cout << "Adesso che ho trovato il minimo devo aggiornare da ---> " << index_updating_from << endl;
					//cout << "Adesso che ho trovato il minimo devo aggiornare a ---> " << index_updating_to << endl;
					double Fuel_before = r_support.quantity_fuel[index_updating_from];
					//cout << " Valore Fuel Before ------> " << Fuel_before << endl;
					//cout << " Quindi alla fine parto ad aggiornare " << index_updating_from << " dove prendo il minimo tra " << map_airplane[r.aircraft_code].max_fuel << " e " << r.quantity_fuel[index_updating_from] + min_weight << endl;
					r_support.quantity_fuel[index_updating_from] = min(map_airplane[r_support.aircraft_code].max_fuel, r_support.quantity_fuel[index_updating_from] + min_weight);
					//cout << " Valore Fuel After ------> " << r.quantity_fuel[index_updating_from] << endl;
					//cout << " Valore Weigth Before ------> " << r.weight[index_updating_from] << endl;
					r_support.weight[index_updating_from] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					//cout << " Valore Weigth After ------> " << r.weight[index_updating_from] << endl;
					for (int i = index_updating_from + 1; i < index_updating_to; i++) {
						r_support.quantity_fuel[i] += (r_support.quantity_fuel[index_updating_from] - Fuel_before);
						r_support.weight[i] -= (r_support.quantity_fuel[index_updating_from] - Fuel_before);
					}
				}
			}

		}
		routes_after_swap.push_back(r_support);
	}
	//cout << " Ho finito La SWAP Local Search  " << endl;
	//for (Route r : routes_after_swap) cout << " Costo Route:   " << cost_single_route(peso_intermediate_stop, r, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
	//system("pause");
	return routes_after_swap;
}





// --------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------- FINE RICERCA LOCALE -------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------


//repair per Rolling
double cost_single_route_giorno_primo_pass(int giorno, double peso_intermediate_stop, const Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//double cost = 0.0;
	double cost = 0.0;

	//tutti questi if sembrano strani ma sono solo per mettere il costo fisso dell'aereo solo quando necessario
	if (giorno == 1) {
		if (r.start_end_routes[1][1] == -1 && r.start_end_routes[2][1] == -1) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
		}
	}
	else if (giorno == 2) {
		if (r.start_end_routes[0][1] == -1 && r.start_end_routes[2][1] == -1) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
		}
	}
	else if (giorno == 3) {
		if (r.start_end_routes[0][1] == -1 && r.start_end_routes[1][1] == -1) {
			cost += map_airplane[r.aircraft_code].fixed_cost;
		}
	}



	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	//double first_hour = 0.0;
	//double second_hour = 0.0;
	for (int i = r.start_end_routes[giorno][0]; i <= r.start_end_routes[giorno][1]; i++) {
		//double time_travel = 0.0;
		if (i >= r.start_end_routes[giorno][0] + 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
		}

		if (i < r.start_end_routes[giorno][1]) {
			if (i == 0 && r.capacity[i] == 0) {  //qua ho lasciato i=0 in quanto è solo il primissimo arco, quando si esce dal deposito che deve attivare qualla casisitica
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}
		}

	}


	cost += mileage;
	cost += fuel_consumed;


	return cost;
}

double cost_single_route_giorno(int giorno, double peso_intermediate_stop, const Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//double cost = 0.0;
	double cost = 0.0;

	//tutti questi if sembrano strani ma sono solo per mettere il costo fisso dell'aereo solo quando necessario
	//cost += map_airplane[r.aircraft_code].fixed_cost;




	//second of all calculate the cost of the Km and the fuel burn
	double mileage = 0.0;
	double fuel_consumed = 0.0;
	//double first_hour = 0.0;
	//double second_hour = 0.0;
	for (int i = r.start_end_routes[giorno][0]; i <= r.start_end_routes[giorno][1]; i++) {
		//double time_travel = 0.0;
		if (i >= r.start_end_routes[giorno][0] + 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
		}

		if (i < r.start_end_routes[giorno][1]) {
			if (i == 0 && r.capacity[i] == 0) {  //qua ho lasciato i=0 in quanto è solo il primissimo arco, quando si esce dal deposito che deve attivare qualla casisitica
				mileage = 0.0;
			}
			else {
				mileage += from_to[r.places[i]][r.places[i + 1]];
				//time_travel = from_to[r.places[i] + ";" + r.places[i + 1]] / map_airplane[r.aircraft_code].speed;
				fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
			}
		}

	}


	cost += mileage;
	cost += fuel_consumed;



	for (auto& p : r.passengers_in_route) {



		if (p.giorno == giorno + 1) {
			cost += (p.solution_to - p.solution_from - 1)*peso_intermediate_stop;  //riga aggiunta per le intermediate stop

			double TW_departure = 0.0;
			double TW_arrival = 0.0;

			//if (p.giorno == giorno) {
			if (p.giorno == 1) {
				if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
				else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			else if (p.giorno == 2) {
				if (p.solution_from == r.start_end_routes[1][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}

			else if (p.giorno == 3) {
				if (p.solution_from == r.start_end_routes[2][0]) {
					double time_arr_new = r.time_dep[p.solution_from] - map_airstrip[r.places[p.solution_from]].ground_time;
					if (time_arr_new < p.early_departure) TW_departure = p.early_departure - time_arr_new;
					else if (time_arr_new > p.late_departure) TW_departure = time_arr_new - p.late_departure;
				}
				else {
					if (r.time_arr[p.solution_from] < p.early_departure) TW_departure = p.early_departure - r.time_arr[p.solution_from];
					else if (r.time_arr[p.solution_from] > p.late_departure) TW_departure = r.time_arr[p.solution_from] - p.late_departure;
				}

				if (r.time_arr[p.solution_to] < p.early_arrival) TW_arrival = p.early_arrival - r.time_arr[p.solution_to];
				else if (r.time_arr[p.solution_to] > p.late_arrival) TW_arrival = r.time_arr[p.solution_to] - p.late_arrival;
			}
			//}




			cost += TW_departure + TW_arrival;


		}



	}

	return cost;
}

bool route_feasible_giorno(double start_day, int giorno, const Route& r, map<int, Airplane>& map_airplane, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool feasible = true;

	if (r.places[r.start_end_routes[giorno][0]] != 1 && r.places[r.start_end_routes[giorno][0]] != 4) {
		if (r.time_dep[r.start_end_routes[giorno][0]] < start_day) {
			//cout << "infeasible per l'inizio della giornata, dovrebbe impossibile che arrivi una route così" << endl;
			feasible = false;
			return false;
		}

	}





	//check and day
	//cout << r.time_arr[r.start_end_routes[giorno][1]] << " > " << end_day << endl;
	if (r.time_arr[r.start_end_routes[giorno][1]] > end_day) {
		feasible = false;
		return false;
		//cout << "infeasible per l'end day" << endl;
	}

	//for the last-trip
	//cout << r.quantity_fuel[r.start_end_routes[giorno][1]] << " < " << location_fuel[r.aircraft_code + "/" + r.places[r.start_end_routes[giorno][1]]] << endl;
	if (r.quantity_fuel[r.start_end_routes[giorno][1]] < map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][r.places[r.start_end_routes[giorno][1]]]) {
		feasible = false;
		return false;
		//cout << "infeasible per il last trip" << endl;
	}

	if (feasible) {
		for (int i = r.start_end_routes[giorno][0]; i <= r.start_end_routes[giorno][1]; i++) {
			if (i >= 1) {
				//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					feasible = false;
					return false;
					//cout << "infeasible per la quantità di fuel in " << i << endl;
				}
				if (r.refueling[i] && !r.refueling[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
					if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						feasible = false;
						return false;
						//cout << "infeasible per la quantità di fuel in " << i << endl;
					}
				}
			}
			if (i < r.index - 1) {
				if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) {
					feasible = false;
					return false;
					//cout << "infeasible per la capacità" << endl;
				}
			}

		}
	}

	if (feasible) {
		//check of the intermediate stop
		for (const Passenger& p : r.passengers_in_route) {
			if (p.giorno == giorno + 1) {
				if (p.solution_to - p.solution_from > p.stop) {
					feasible = false;
					return false;
					//cout << "infeasible per le intermediate stop" << endl;
				}
			}
		}
	}







	//cout << " fine funzione feasiblity only one day" << endl;

	return feasible;
}


bool route_feasible_pezzo_after(double start_day1, double start_day2, double start_day3, int giorno, const Route& r, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool feasible = true;

	//in questa funzione il giorno è il giorno in cui ho inserito il passeggero, mi serve solo per sapere da dove partire.
	//in più l'unica cosa che devo valutare se rimane feasible per i giorni a venire è il fuel ed il peso



	//for the last-trip
	//cout << r.quantity_fuel[r.start_end_routes[giorno][1]] << " < " << location_fuel[r.aircraft_code + "/" + r.places[r.start_end_routes[giorno][1]]] << endl;
	if (r.quantity_fuel[r.index - 1] < map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][r.places[r.index - 1]]) {
		feasible = false;
		return false;
		//cout << "fuel last trip" << endl;
	}

	//devo fare il check sulla partenza del giorno dopo
	if (feasible) {
		double start_day = 0.0;
		if (giorno == 0) {
			if (r.start_end_routes[1][1] != -1) start_day = start_day2;
			else start_day = start_day3;
		}
		else if (giorno == 1) {
			start_day = start_day3;
		}


		//qua ho messo 1 in quanto il giorno è il giorno in cui ho fatto l'inserimento quindi devo guardare quello dopo, questo caso non dovrebbe comunque capitare perchè fai il check qunado inserisci
		if (r.places[r.start_end_routes[giorno][1]] != 1 && r.places[r.start_end_routes[giorno][1]] != 4) {
			if (r.time_dep[r.start_end_routes[giorno][1]] < start_day) {
				feasible = false;
				return false;
				//cout << "infeasible per lo starting day" << endl;
			}
		}
	}


	if (feasible) {
		for (int i = r.start_end_routes[giorno][1]; i < r.index; i++) {
			if (i >= 1) {

				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					feasible = false;
					return false;
					//cout << "infeasible per la quantità di fuel in " << i << endl;
				}
				if (r.refueling[i] && !r.refueling[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
					if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						feasible = false;
						return false;
						//cout << "infeasible per la quantità di fuel in " << i << endl;
					}
				}
			}
		}
	}





	//cout << " fine funzione feasiblity pezzo after" << endl;
	return feasible;
}





//l'unica differenza qui è che non controllo gli intermediate stops
bool route_feasible_giorno_moveC(double start_day1, double start_day2, double start_day3, int giorno, const Route& r, map<int, Airplane>& map_airplane, double end_day, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool feasible = true;


	//check and day
	if (r.time_arr[r.start_end_routes[giorno][1]] > end_day) {
		feasible = false;
		return false;
		//cout << "infeasible per l'end_day" << endl;
	}
	//for the last-trip
	if (r.quantity_fuel[r.start_end_routes[giorno][1]] < map_airplane[r.aircraft_code].min_fuel + location_fuel[r.aircraft_code][r.places[r.start_end_routes[giorno][1]]]) {
		feasible = false;
		return false;
		//cout << "infeasible per last trip" << endl;
	}

	//per lo starting day del giorno successivo se c'è
	if (feasible) {

		if (giorno == 0) {
			if (r.start_end_routes[1][1] != -1) {
				if (r.places[r.start_end_routes[1][0]] != 1 && r.places[r.start_end_routes[1][0]] != 4) {
					if (r.time_dep[r.start_end_routes[1][0]] < start_day2) {
						feasible = false;
						return false;
					}
				}
			}
			else if (r.start_end_routes[2][1] != -1) {
				if (r.places[r.start_end_routes[2][0]] != 1 && r.places[r.start_end_routes[2][0]] != 4) {
					if (r.time_dep[r.start_end_routes[2][0]] < start_day3) {
						feasible = false;
						return false;
					}
				}
			}
		}
		else if (giorno == 1) {
			if (r.start_end_routes[2][1] != -1) {
				if (r.places[r.start_end_routes[2][0]] != 1 && r.places[r.start_end_routes[2][0]] != 4) {
					if (r.time_dep[r.start_end_routes[2][0]] < start_day3) {
						feasible = false;
						return false;
					}
				}
			}
		}

	}


	if (feasible) {
		for (int i = r.start_end_routes[giorno][0]; i < r.index; i++) {
			if (i >= 1) {
				//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					feasible = false;
					return false;
					//cout << "infeasible per la quantità di fuel in " << i << endl;
				}
				if (r.refueling[i] && !r.refueling[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
					if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						feasible = false;
						return false;
						//cout << "infeasible per la quantità di fuel in " << i << endl;
					}
				}
			}
			//in questo caso la capacità si può anche non controllare 
			/*
			if (i < r.index - 1) {
				if (r.capacity[i] > map_airplane[r.aircraft_code].capacity) feasible = false;
			}
			*/
		}
	}







	return feasible;
}

//qui non ho messo il check sul giorno dopo sullo starting day in quanto la mossa particolare la faccio solo se i tempi me lo consentono
bool route_feasible_primo_pass(int giorno, double end_day1, double end_day2, double end_day3, const Route& r, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {
	bool feasible = true;
	double end_day = 0.0;

	if (giorno == 2) end_day = end_day3;
	else if (giorno == 1) end_day = end_day2;
	else if (giorno == 0) end_day = end_day1;


	//check and day
	if (r.time_arr[r.start_end_routes[giorno][1]] > end_day) {
		feasible = false;
		return false;
		//cout << "infeasible per il tempo finale" << endl;
	}
	//for the last-trip
	if (r.quantity_fuel[r.index - 1] < location_fuel[r.aircraft_code][r.places[r.index - 1]]) {
		feasible = false;
		return false;
		//cout << "infeasible per last trip" << endl;
	}

	if (feasible) {
		for (int i = r.start_end_routes[giorno][0]; i < r.index; i++) {
			if (i >= 1) {
				//if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel && !r.refueling[i]) feasible = false;
				if (r.quantity_fuel[i] < map_airplane[r.aircraft_code].min_fuel) {
					feasible = false;
					return false;
					//cout << "infeasible per il minimo fuel in " << i << endl;
				}
				if (r.refueling[i] && !r.refueling[i - 1]) {
					double fuel_consumed = from_to_FuelConsumed[r.aircraft_code][r.places[i - 1]][r.places[i]];
					if (r.quantity_fuel[i - 1] - fuel_consumed < map_airplane[r.aircraft_code].min_fuel) {
						feasible = false;
						return false;
						//cout << "infeasible per il minimo fuel in " << i << endl;
					}
				}
			}

		}
	}



	return feasible;
}

bool cost_routing_pezzo_after(int index_partenza, double peso_intermediate_stop, const Route& r, map<int, Airstrip>& map_airstrip, map<int, Airplane>& map_airplane, vector<vector<double>>& from_to, vector<vector<vector<double>>>& from_to_FuelConsumed) {


	double cost = 0.0;



	double mileage = 0.0;
	double fuel_consumed = 0.0;


	for (int i = index_partenza; i < r.index; i++) {

		if (i >= index_partenza + 1) {
			cost += map_airstrip[r.places[i]].landing_cost;
		}

		if (i < r.index - 1) {
			mileage += from_to[r.places[i]][r.places[i + 1]];
			fuel_consumed += from_to_FuelConsumed[r.aircraft_code][r.places[i]][r.places[i + 1]];
		}

	}


	cost += mileage;
	cost += fuel_consumed;


	return cost;
}


vector <Route> repair_one(map<string, int>& legenda, double peso_intermediate_stop, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {
		//system("pause");
		/*cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "sto valutanto il passeggero" << endl;
		p.print();*/
		int best_arc_from = -1;
		int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		bool move_c = false;
		bool move_before = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < routes_destroyed.size(); r++) {
			//cout << "***************************************************************************************************************" << endl;
			//cout << "route:      " << routes_destroyed[r].aircraft_code << endl;
			//routes_destroyed[r].Print();



			if (routes_destroyed[r].primo_pass[p.giorno - 1] == false) {
				//cout << "sono dentro la valutazione del primo passeggero" << endl;


				Route r_support = routes_destroyed[r];
				bool check_pezzo_after = false;
				int index_partenza = 0;
				if (p.giorno == 1) {
					if (r_support.start_end_routes[1][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[1][0];
					}
					else if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}
				}
				else if (p.giorno == 2) {
					if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}
				}

				r_support.add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);


				//cout << "valuto stampo caso strano primo pass" << endl;
				//stampo_caso_strano_single_all_primo_pass(p, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, map_airstrip);



				if (route_feasible_primo_pass((p.giorno - 1), end_day1, end_day2, end_day3, r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
					double cost_before = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					//cout << "cost before: " << cost_before << endl;
					double cost = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					//cout << "cost after: " << cost << endl;
					cost -= cost_before;
					//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
					}
				}

				//system("pause");

			}
			else {
				//cout << "sono dentro la valutazione della mossa in mezzo" << endl;
				//p.print();
				double end_day = 0.0;

				if (p.giorno == 1) {
					end_day = end_day1;

					//valuto se possibile la massa before******************************************************************************************************************************************
					//cout << "valuto l'inserimento del passeggero davanti" << endl;
					Route r_support_MB = routes_destroyed[r];
					double time_check = 0.0;
					if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}
					else {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}

					//cout << "stampo route prima dell'inserimento before" << endl;
					//r_support_MB.print1();
					//cout << "voglio inserire il passeggero" << endl;
					//p.print();
					//cout << "time_check" << time_check << " confrontato con starting day" << start_day1 << endl;
					double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


					if (time_check >= start_day1) {
						r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						//cout << "stampo caso strano dopo mossa before" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						if (route_feasible_giorno(start_day1, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
							cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;
							if (cost_mb < best_cost) {

								best_cost = cost_mb;
								move_c = false;
								move_before = true;
								route_best = r_support_MB;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
								best_route = r;

							}
						}
					}

					//system("pause");
					//***********************************************************************************************************************************************************************




					//cout << "valuto l'inserimento in mezzo" << endl;
					if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
						//cout << "primo if" << endl;
						//ha qualcosa dopo
						double start_day_new = 0.0;
						int gg = 0;
						if (routes_destroyed[r].start_end_routes[1][1] != -1) {
							start_day_new = start_day2;
							gg = 1;
						}
						else {
							start_day_new = start_day3;
							gg = 2;
						}


						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[p.giorno - 1][1], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool messa_connessione = false;
										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;
										double fuel_connection = 0.0;
										double weight_connection = 0.0;

										//system("pause");
										//cout << "route prima della manipolazione dei tempi degli archi: " << n << "  ----  " << n1 << endl;




										r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day_new, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "sto valutando il caso strano inserimento in mezzo tempi" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip);

										/*cout << "route dopo la  manipolazione dei tempi" << endl;
										if (entrato) {
										r_support.print1();
										system("pause");
										}*/


										if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano mossa in mezzo all" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);




											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

														if (best_cost > cost) {
															best_arc_from = n;
															best_arc_to = n1;
															best_route = r;
															best_cost = cost;
															move_c = false;
															move_before = false;

															route_best = r_support;
															from_per_route = p.solution_from;
															to_per_route = p.solution_to;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					else {
						//cout << "secondo if" << endl;
						//non ha niente dopo
						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										//system("pause");
										//cout << "route prima della manipolazione dei tempi" << endl;
										//if (p.giorno == 1) r_support.print1();


										r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);



										//cout << "valuto stampo caso strano in mezzo tempi" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip);


										//cout << "route dopo la  manipolazione dei tempi" << endl;
										//if (p.giorno == 1) r_support.print1();

										//cout << "if di verifica dei tempi" << endl;
										if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

											//cout << "ora aggiorno anche le diverse quantità" << endl;
											r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);


											//cout << "valuto il caso strano in mezzo all" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



											//cout << "route dopo la  manipolazione del fuel e peso" << endl;
											//if (p.giorno == 1) r_support.print1();
											//cout << "prima dell'if per le intermediate stop" << endl;
											if ((p.solution_to - p.solution_from <= p.stop)) {
												//cout << "prima dell'if sulla feasibility" << endl;
												//cout << "r_support size di time_arr: " << r_support.time_arr.size();
												//cout << "indice end_route giorno 1 " << r_support.start_end_routes[p.giorno - 1][1] << endl;
												if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													//cout << "dopo if feasibility" << endl;
													//cout << "costo single route: " << cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
													//cout << " costo time windows passeggero che ho inserito: " << cost_time_windows_for_route_passenger(r_support, p, map_airstrip) << endl;


													double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
													//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
													if (best_cost > cost) {
														best_arc_from = n;
														best_arc_to = n1;
														best_route = r;
														best_cost = cost;
														move_c = false;
														move_before = false;

														route_best = r_support;
														from_per_route = p.solution_from;
														to_per_route = p.solution_to;
													}
												}
											}
											//cout << "dopo blocco if" << endl;
										}
									}
								}
							}
						}



					}
				}
				else if (p.giorno == 3) {
					end_day = end_day3;


					//valuto se possibile la massa before******************************************************************************************************************************************
					//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
					Route r_support_MB = routes_destroyed[r];
					double time_check = 0.0;
					if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}
					else {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}

					//cout << "stampo route prima dell'inserimento before" << endl;
					//r_support_MB.print3();
					//cout << "voglio inserire il passeggero" << endl;
					//p.print();
					//cout << "time_check" << time_check << " confrontato con starting day" << start_day3 << endl;
					double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


					if (time_check >= start_day3) {
						r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						//cout << "stampo caso strano dopo mossa before" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						if (route_feasible_giorno(start_day3, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
							cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;
							if (cost_mb < best_cost) {

								best_cost = cost_mb;
								move_c = false;
								move_before = true;
								route_best = r_support_MB;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
								best_route = r;
							}
						}
					}

					//system("pause");
					//***********************************************************************************************************************************************************************



					//cout << "valuto l'inserimento in mezzo" << endl;
					int arc_from = -1;
					int arc_to = -1;
					double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					for (int n = routes_destroyed[r].start_end_routes[2][0]; n < routes_destroyed[r].start_end_routes[2][1]; n++) {
						if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[2][0]) {
							for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[2][1]) && (n1 - n <= p.stop + 1); n1++) {
								if (p.arrival_location != routes_destroyed[r].places[n1]) {
									//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
									Route r_support = routes_destroyed[r];

									bool non_to = false;
									bool non_to_final = false;
									bool num_equals = false;
									int caso = -1;
									int node_add_from = n;
									int node_add_to = n1;

									r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

									//cout << "stampo caso stano inserimento in mezzo tempi" << endl;
									//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip); //TOGLIOLO DOPO


									if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

										r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "valuto stampo caso strano inserimento in mezzo all" << endl;
										//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


										if ((p.solution_to - p.solution_from <= p.stop)) {
											if (route_feasible_giorno(start_day3, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
												double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
												//cout << " costo  "  << cost << " confrontato con best cost: " << best_cost << endl;
												if (best_cost > cost) {
													best_arc_from = n;
													best_arc_to = n1;
													best_route = r;
													best_cost = cost;
													move_c = false;
													move_before = false;

													route_best = r_support;
													from_per_route = p.solution_from;
													to_per_route = p.solution_to;
												}
											}
										}
									}
								}
							}
						}
					}



				}
				else if (p.giorno == 2) {
					end_day = end_day2;


					//valuto se possibile la massa before******************************************************************************************************************************************
					//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
					Route r_support_MB = routes_destroyed[r];
					double time_check = 0.0;
					if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}
					else {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}

					//cout << "stampo route prima dell'inserimento before" << endl;
					//r_support_MB.print2();
					//cout << "voglio inserire il passeggero" << endl;
					//p.print();
					//cout << "time_check" << time_check << " confrontato con starting day" << start_day2 << endl;
					double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


					if (time_check >= start_day2) {
						r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						//cout << "stampo caso strano dopo mossa before" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						if (route_feasible_giorno(start_day2, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
							cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;
							if (cost_mb < best_cost) {

								best_cost = cost_mb;
								move_c = false;
								move_before = true;
								route_best = r_support_MB;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
								best_route = r;

							}
						}
					}

					//system("pause");
					//***********************************************************************************************************************************************************************	

					//cout << "valuto l'inserimento in mezzo" << endl;
					if (routes_destroyed[r].start_end_routes[2][1] != -1) {
						//ha qualcosa dopo
						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[2][0], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										//p.print();
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;
										double fuel_connection = 0.0;
										double weight_connection = 0.0;
										bool messa_connessione = false;


										r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day3, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "valuto stampo caso strano nell'inserimento in mezzo nel secondo giorno tempi" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip); //TOGLIOLO DOPO

										if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano inserimento in mezzo all" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														cost += cost_routing_pezzo_after(r_support.start_end_routes[2][0], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
														//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
														if (best_cost > cost) {
															best_arc_from = n;
															best_arc_to = n1;
															best_route = r;
															best_cost = cost;
															move_c = false;
															move_before = false;

															route_best = r_support;
															from_per_route = p.solution_from;
															to_per_route = p.solution_to;
														}
													}
												}
											}
										}
									}
								}
							}
						}



					}
					else {
						//non ha niente dopo

						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "sto valutando il caso strano dei tempi quando non c'e niente dopo" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip);


										if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano quando non c'è niente dopo all" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << " costo " << cost << " confrontato con best cost" << best_cost << endl;
													if (best_cost > cost) {
														best_arc_from = n;
														best_arc_to = n1;
														best_route = r;
														best_cost = cost;
														move_c = false;
														move_before = false;

														route_best = r_support;
														from_per_route = p.solution_from;
														to_per_route = p.solution_to;
													}
												}
											}
										}
									}
								}
							}
						}

					}
				}


			}

			//for move C
			if (routes_destroyed[r].primo_pass[p.giorno - 1]) {
				//cout << " sono dentro la valutazione della mossa C" << endl;
				//p.print();
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C

				Route r_support = routes_destroyed[r];
				double end_day = 0.0;

				bool check_pezzo_after = false;
				int index_partenza = 0;
				if (p.giorno == 1) {
					if (r_support.start_end_routes[1][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[1][0];
					}
					else if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}
				}
				else if (p.giorno == 2) {
					if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}

				}

				if (p.giorno == 1) {
					end_day = end_day1;
					if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
						//ha qualcosa dopo


						r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else {
						//non ha niente dopo
						r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
				}
				else if (p.giorno == 3) {
					end_day = end_day3;
					r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				else if (p.giorno == 2) {
					end_day = end_day2;
					if (routes_destroyed[r].start_end_routes[2][1] != -1) {
						//ha qualcosa dopo
						r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else {
						//non ha niente dopo
						r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
				}


				//cout << "valuto stampo caso strano dopo la mossa C" << endl;
				//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

				/*if (p.giorno == 1) {
				r_support.print1();
				system("pause");
				}*/
				//cout << "finito stampo caso strano" << endl;
				//r_support.Print();
				//if(p.giorno == 1 || p.giorno == 2) system("pause");

				if (route_feasible_giorno_moveC(start_day1, start_day2, start_day3, p.giorno - 1, r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {

					double cost_before = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					double cost = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					cost -= cost_before;

					//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						move_c = true;
						move_before = false;
						best_arc_from = -2;
						best_arc_to = -2;
					}

				}

			}

		}


		//cout << "sto per fare la mossa per il passeggero: " << endl;
		//p.print();
		if (best_route == -1) {
			//cout << "il passeggero: " << endl;
			//p.print();
			//cout << "********************** ha queste possibilità******************************************" << endl;
			//for (Route c : routes_destroyed) c.Print();
			//system("pause");
			routes_infeasible = true;
			break;
		}
		else {



			if (!routes_destroyed[best_route].primo_pass[p.giorno - 1]) {
				//cout << "sto facendo la mossa primo passeggero" << endl;
				Route r_test = routes_destroyed[best_route]; //dopo cancella questa riga




				routes_destroyed[best_route].primo_pass[p.giorno - 1] = true;
				routes_destroyed[best_route].add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);

				//cout << "valuto stampo caso strano dopo l'inserimento del primo passeggero vero e prorpio" << endl;
				//stampo_caso_strano_single_all(p, map_airstrip, r_test, p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
			}
			else {

				if (move_c) {

					//cout << "sto facendo la mossa C" << endl;
					if (p.giorno == 1) {
						if (routes_destroyed[best_route].start_end_routes[1][1] != -1 || routes_destroyed[best_route].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							routes_destroyed[best_route].move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}
					else if (p.giorno == 3) {
						routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else if (p.giorno == 2) {
						if (routes_destroyed[best_route].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							routes_destroyed[best_route].move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}

					//cout << "valuto stampo caso strano dopo l'inserimento vero e prorpio in C" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

				}
				else if (move_before) {
					//cout << "sto facendo la mossa dell'inserimento davanti" << endl;
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;

					//cout << "valuto stampo caso strano dopo l'inserimento vero e proprio in davanti" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
				}
				else {
					//cout << "sto facendo la mossa dell'inserimento in mezzo" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;

					//cout << "valuto stampo caso strano dopo l'inserimento vero e proprio in mezzo" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

					//routes_destroyed[best_route].print();
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector <Route> repair_forbidden(double peso_intermediate_stop, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {

		int best_arc_from = -1;
		int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		bool move_c = false;
		bool move_before = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < routes_destroyed.size(); r++) {

			if (r != p.route_before) {

				if (routes_destroyed[r].primo_pass[p.giorno - 1] == false) {
					//cout << "sono dentro la valutazione del primo passeggero" << endl;


					Route r_support = routes_destroyed[r];
					bool check_pezzo_after = false;
					int index_partenza = 0;
					if (p.giorno == 1) {
						if (r_support.start_end_routes[1][1] != -1) {
							check_pezzo_after = true;
							index_partenza = r_support.start_end_routes[1][0];
						}
						else if (r_support.start_end_routes[2][1] != -1) {
							check_pezzo_after = true;
							index_partenza = r_support.start_end_routes[2][0];
						}
					}
					else if (p.giorno == 2) {
						if (r_support.start_end_routes[2][1] != -1) {
							check_pezzo_after = true;
							index_partenza = r_support.start_end_routes[2][0];
						}
					}

					r_support.add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);


					//cout << "valuto stampo caso strano" << endl;
					//stampo_caso_strano_single_all_primo_pass(p, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



					if (route_feasible_primo_pass((p.giorno - 1), end_day1, end_day2, end_day3, r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
						double cost_before = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						//cout << "cost before: " << cost_before << endl;
						double cost = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						//cout << "cost after: " << cost << endl;
						cost -= cost_before;
						//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
						if (cost < best_cost) {
							best_cost = cost;
							best_route = r;
						}
					}

					//system("pause");

				}
				else {
					//cout << "sono dentro la valutazione della mossa in mezzo" << endl;
					//p.print();
					double end_day = 0.0;

					if (p.giorno == 1) {
						end_day = end_day1;

						//valuto se possibile la massa before******************************************************************************************************************************************
						//cout << "valuto l'inserimento del passeggero davanti" << endl;
						Route r_support_MB = routes_destroyed[r];
						double time_check = 0.0;
						if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
							time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
							if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
							if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
							double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
								from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
							double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
							time_check -= time_volo;
						}
						else {
							time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
							if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
							if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
							double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
								from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
							double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
							time_check -= time_volo;
						}

						//cout << "stampo route prima dell'inserimento before" << endl;
						//r_support_MB.print1();
						//cout << "voglio inserire il passeggero" << endl;
						//p.print();
						//cout << "time_check" << time_check << " confrontato con starting day" << start_day1 << endl;
						double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


						if (time_check >= start_day1) {
							r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							//cout << "stampo caso strano dopo mossa before" << endl;
							//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

							if (route_feasible_giorno(start_day1, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
								cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;
								if (cost_mb < best_cost) {

									best_cost = cost_mb;
									move_c = false;
									move_before = true;
									route_best = r_support_MB;
									from_per_route = p.solution_from;
									to_per_route = p.solution_to;
									best_route = r;

								}
							}
						}

						//system("pause");
						//***********************************************************************************************************************************************************************




						//cout << "valuto l'inserimento in mezzo" << endl;
						if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
							//cout << "primo if" << endl;
							//ha qualcosa dopo
							double start_day_new = 0.0;
							int gg = 0;
							if (routes_destroyed[r].start_end_routes[1][1] != -1) {
								start_day_new = start_day2;
								gg = 1;
							}
							else {
								start_day_new = start_day3;
								gg = 2;
							}


							int arc_from = -1;
							int arc_to = -1;
							double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[gg][0], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
								if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
									for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
										if (p.arrival_location != routes_destroyed[r].places[n1]) {
											//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
											Route r_support = routes_destroyed[r];

											bool messa_connessione = false;
											bool non_to = false;
											bool non_to_final = false;
											bool num_equals = false;
											int caso = -1;
											int node_add_from = n;
											int node_add_to = n1;
											double fuel_connection = 0.0;
											double weight_connection = 0.0;

											//system("pause");
											//cout << "route prima della manipolazione dei tempi degli archi: " << n << "  ----  " << n1 << endl;




											r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day_new, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "sto valutando il caso strano" << endl;
											//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane);

											/*cout << "route dopo la  manipolazione dei tempi" << endl;
											if (entrato) {
											r_support.print1();
											system("pause");
											}*/


											if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

												r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "valuto stampo caso strano" << endl;
												//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);




												if ((p.solution_to - p.solution_from <= p.stop)) {
													if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
														if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
															double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
															cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

															if (best_cost > cost) {
																best_arc_from = n;
																best_arc_to = n1;
																best_route = r;
																best_cost = cost;
																move_c = false;
																move_before = false;

																route_best = r_support;
																from_per_route = p.solution_from;
																to_per_route = p.solution_to;
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
						else {
							//cout << "secondo if" << endl;
							//non ha niente dopo
							int arc_from = -1;
							int arc_to = -1;
							double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
								if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
									for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
										if (p.arrival_location != routes_destroyed[r].places[n1]) {
											//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
											Route r_support = routes_destroyed[r];

											bool non_to = false;
											bool non_to_final = false;
											bool num_equals = false;
											int caso = -1;
											int node_add_from = n;
											int node_add_to = n1;

											//system("pause");
											//cout << "route prima della manipolazione dei tempi" << endl;
											//if (p.giorno == 1) r_support.print1();


											r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);



											//cout << "valuto stampo caso strano" << endl;
											//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane);


											//cout << "route dopo la  manipolazione dei tempi" << endl;
											//if (p.giorno == 1) r_support.print1();

											//cout << "if di verifica dei tempi" << endl;
											if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

												//cout << "ora aggiorno anche le diverse quantità" << endl;
												r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);


												//cout << "valuto il caso strano" << endl;
												//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



												//cout << "route dopo la  manipolazione del fuel e peso" << endl;
												//if (p.giorno == 1) r_support.print1();
												//cout << "prima dell'if per le intermediate stop" << endl;
												if ((p.solution_to - p.solution_from <= p.stop)) {
													//cout << "prima dell'if sulla feasibility" << endl;
													//cout << "r_support size di time_arr: " << r_support.time_arr.size();
													//cout << "indice end_route giorno 1 " << r_support.start_end_routes[p.giorno - 1][1] << endl;
													if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
														//cout << "dopo if feasibility" << endl;
														//cout << "costo single route: " << cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
														//cout << " costo time windows passeggero che ho inserito: " << cost_time_windows_for_route_passenger(r_support, p, map_airstrip) << endl;


														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
														//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
														if (best_cost > cost) {
															best_arc_from = n;
															best_arc_to = n1;
															best_route = r;
															best_cost = cost;
															move_c = false;
															move_before = false;

															route_best = r_support;
															from_per_route = p.solution_from;
															to_per_route = p.solution_to;
														}
													}
												}
												//cout << "dopo blocco if" << endl;
											}
										}
									}
								}
							}



						}
					}
					else if (p.giorno == 3) {
						end_day = end_day3;


						//valuto se possibile la massa before******************************************************************************************************************************************
						//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
						Route r_support_MB = routes_destroyed[r];
						double time_check = 0.0;
						if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
							time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
							if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
							if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
							double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
								from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
							double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
							time_check -= time_volo;
						}
						else {
							time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
							if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
							if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
							double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
								from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
							double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
							time_check -= time_volo;
						}

						//cout << "stampo route prima dell'inserimento before" << endl;
						//r_support_MB.print3();
						//cout << "voglio inserire il passeggero" << endl;
						//p.print();
						//cout << "time_check" << time_check << " confrontato con starting day" << start_day3 << endl;
						double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


						if (time_check >= start_day3) {
							r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							//cout << "stampo caso strano dopo mossa before" << endl;
							//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

							if (route_feasible_giorno(start_day3, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
								cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;
								if (cost_mb < best_cost) {

									best_cost = cost_mb;
									move_c = false;
									move_before = true;
									route_best = r_support_MB;
									from_per_route = p.solution_from;
									to_per_route = p.solution_to;
									best_route = r;
								}
							}
						}

						//system("pause");
						//***********************************************************************************************************************************************************************



						//cout << "valuto l'inserimento in mezzo" << endl;
						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = routes_destroyed[r].start_end_routes[2][0]; n < routes_destroyed[r].start_end_routes[2][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[2][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[2][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane); //TOGLIOLO DOPO


										if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day3, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << " costo  "  << cost << " confrontato con best cost: " << best_cost << endl;
													if (best_cost > cost) {
														best_arc_from = n;
														best_arc_to = n1;
														best_route = r;
														best_cost = cost;
														move_c = false;
														move_before = false;

														route_best = r_support;
														from_per_route = p.solution_from;
														to_per_route = p.solution_to;
													}
												}
											}
										}
									}
								}
							}
						}



					}
					else if (p.giorno == 2) {
						end_day = end_day2;


						//valuto se possibile la massa before******************************************************************************************************************************************
						//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
						Route r_support_MB = routes_destroyed[r];
						double time_check = 0.0;
						if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
							time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
							if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
							if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
							double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
								from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
							double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
							time_check -= time_volo;
						}
						else {
							time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
							if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
							if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
							double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
								from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
							double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
							time_check -= time_volo;
						}

						//cout << "stampo route prima dell'inserimento before" << endl;
						//r_support_MB.print2();
						//cout << "voglio inserire il passeggero" << endl;
						//p.print();
						//cout << "time_check" << time_check << " confrontato con starting day" << start_day2 << endl;
						double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


						if (time_check >= start_day2) {
							r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							//cout << "stampo caso strano dopo mossa before" << endl;
							//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

							if (route_feasible_giorno(start_day2, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
								cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;
								if (cost_mb < best_cost) {

									best_cost = cost_mb;
									move_c = false;
									move_before = true;
									route_best = r_support_MB;
									from_per_route = p.solution_from;
									to_per_route = p.solution_to;
									best_route = r;

								}
							}
						}

						//system("pause");
						//***********************************************************************************************************************************************************************	

						//cout << "valuto l'inserimento in mezzo" << endl;
						if (routes_destroyed[r].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							int arc_from = -1;
							int arc_to = -1;
							double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[2][0], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
								if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
									for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
										if (p.arrival_location != routes_destroyed[r].places[n1]) {
											//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
											//p.print();
											Route r_support = routes_destroyed[r];

											bool non_to = false;
											bool non_to_final = false;
											bool num_equals = false;
											int caso = -1;
											int node_add_from = n;
											int node_add_to = n1;
											double fuel_connection = 0.0;
											double weight_connection = 0.0;
											bool messa_connessione = false;


											r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day3, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano nell'inserimento in mezzo nel secondo giorno" << endl;
											//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane); //TOGLIOLO DOPO

											if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

												r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "valuto stampo caso strano" << endl;
												//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



												if ((p.solution_to - p.solution_from <= p.stop)) {
													if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
														if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
															double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
															cost += cost_routing_pezzo_after(r_support.start_end_routes[2][0], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
															//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
															if (best_cost > cost) {
																best_arc_from = n;
																best_arc_to = n1;
																best_route = r;
																best_cost = cost;
																move_c = false;
																move_before = false;

																route_best = r_support;
																from_per_route = p.solution_from;
																to_per_route = p.solution_to;
															}
														}
													}
												}
											}
										}
									}
								}
							}



						}
						else {
							//non ha niente dopo

							int arc_from = -1;
							int arc_to = -1;
							double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
								if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
									for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
										if (p.arrival_location != routes_destroyed[r].places[n1]) {
											//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
											Route r_support = routes_destroyed[r];

											bool non_to = false;
											bool non_to_final = false;
											bool num_equals = false;
											int caso = -1;
											int node_add_from = n;
											int node_add_to = n1;

											r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "sto valutando il caso strano dei tempi" << endl;
											//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane);


											if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

												r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "valuto stampo caso strano" << endl;
												//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


												if ((p.solution_to - p.solution_from <= p.stop)) {
													if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														//cout << " costo " << cost << " confrontato con best cost" << best_cost << endl;
														if (best_cost > cost) {
															best_arc_from = n;
															best_arc_to = n1;
															best_route = r;
															best_cost = cost;
															move_c = false;
															move_before = false;

															route_best = r_support;
															from_per_route = p.solution_from;
															to_per_route = p.solution_to;
														}
													}
												}
											}
										}
									}
								}
							}

						}
					}


				}

				//for move C
				if (routes_destroyed[r].primo_pass[p.giorno - 1]) {
					//cout << " sono dentro la valutazione della mossa C" << endl;
					//p.print();
					//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C

					Route r_support = routes_destroyed[r];
					double end_day = 0.0;

					bool check_pezzo_after = false;
					int index_partenza = 0;
					if (p.giorno == 1) {
						if (r_support.start_end_routes[1][1] != -1) {
							check_pezzo_after = true;
							index_partenza = r_support.start_end_routes[1][0];
						}
						else if (r_support.start_end_routes[2][1] != -1) {
							check_pezzo_after = true;
							index_partenza = r_support.start_end_routes[2][0];
						}
					}
					else if (p.giorno == 2) {
						if (r_support.start_end_routes[2][1] != -1) {
							check_pezzo_after = true;
							index_partenza = r_support.start_end_routes[2][0];
						}

					}

					if (p.giorno == 1) {
						end_day = end_day1;
						if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo


							r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}
					else if (p.giorno == 3) {
						end_day = end_day3;
						r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else if (p.giorno == 2) {
						end_day = end_day2;
						if (routes_destroyed[r].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}


					//cout << "valuto stampo caso strano dopo la mossa C" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

					/*if (p.giorno == 1) {
					r_support.print1();
					system("pause");
					}*/
					//cout << "finito stampo caso strano" << endl;
					//r_support.Print();
					//if(p.giorno == 1 || p.giorno == 2) system("pause");

					if (route_feasible_giorno_moveC(start_day1, start_day2, start_day3, p.giorno - 1, r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {

						double cost_before = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						double cost = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						cost -= cost_before;

						//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
						if (cost < best_cost) {
							best_cost = cost;
							best_route = r;
							move_c = true;
							move_before = false;
							best_arc_from = -2;
							best_arc_to = -2;
						}

					}

				}

			}

		}



		if (best_route == -1) {
			//cout << "il passeggero: " << endl;
			//p.print();
			//cout << "********************** ha queste possibilità******************************************" << endl;
			//for (Route c : routes_destroyed) c.Print();
			//system("pause");
			routes_infeasible = true;
			break;
		}
		else {



			if (!routes_destroyed[best_route].primo_pass[p.giorno - 1]) {
				//cout << "sto facendo la mossa primo passeggero" << endl;
				Route r_test = routes_destroyed[best_route]; //dopo cancella questa riga




				routes_destroyed[best_route].primo_pass[p.giorno - 1] = true;
				routes_destroyed[best_route].add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);

				//cout << "valuto stampo caso strano dopo l'inserimento del primo passeggero vero e prorpio" << endl;
				//stampo_caso_strano_single_all(p, map_airstrip, r_test, p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
			}
			else {

				if (move_c) {

					//cout << "sto facendo la mossa C" << endl;
					if (p.giorno == 1) {
						if (routes_destroyed[best_route].start_end_routes[1][1] != -1 || routes_destroyed[best_route].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							routes_destroyed[best_route].move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}
					else if (p.giorno == 3) {
						routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else if (p.giorno == 2) {
						if (routes_destroyed[best_route].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							routes_destroyed[best_route].move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}

					//cout << "valuto stampo caso strano dopo l'inserimento vero e prorpio in C" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

				}
				else if (move_before) {
					//cout << "sto facendo la mossa dell'inserimento davanti" << endl;
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;

					//cout << "valuto stampo caso strano dopo l'inserimento vero e proprio in davanti" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
				}
				else {
					//cout << "sto facendo la mossa dell'inserimento in mezzo" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;

					//cout << "valuto stampo caso strano dopo l'inserimento vero e proprio in mezzo" << endl;
				//	stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

					//routes_destroyed[best_route].print();
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector <Route> repair_perturbation(double peso_intermediate_stop, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	bool move_c = false;
	bool routes_infeasible = false;

	for (Passenger p : passengers_removed) {
		//system("pause");
		/*cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "***************************************************************************************************************" << endl;
		cout << "sto valutanto il passeggero" << endl;
		p.print();*/
		int best_arc_from = -1;
		int best_arc_to = -1;
		int best_route = -1;
		double best_cost = DBL_MAX;
		bool move_c = false;
		bool move_before = false;

		Route route_best;
		int from_per_route = 0;
		int to_per_route = 0;
		//cout << "sto valutando il pass: ";
		//p.print();

		for (int r = 0; r < routes_destroyed.size(); r++) {
			//cout << "***************************************************************************************************************" << endl;
			//cout << "route:      " << routes_destroyed[r].aircraft_code << endl;
			//routes_destroyed[r].Print();



			if (routes_destroyed[r].primo_pass[p.giorno - 1] == false) {
				//cout << "sono dentro la valutazione del primo passeggero" << endl;


				Route r_support = routes_destroyed[r];
				bool check_pezzo_after = false;
				int index_partenza = 0;
				if (p.giorno == 1) {
					if (r_support.start_end_routes[1][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[1][0];
					}
					else if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}
				}
				else if (p.giorno == 2) {
					if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}
				}

				r_support.add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);


				//cout << "valuto stampo caso strano" << endl;
				//stampo_caso_strano_single_all_primo_pass(p, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



				if (route_feasible_primo_pass((p.giorno - 1), end_day1, end_day2, end_day3, r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
					double cost_before = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					//cout << "cost before: " << cost_before << endl;
					double cost = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					//cout << "cost after: " << cost << endl;
					cost -= cost_before;

					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************


					//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
					}
				}

				//system("pause");

			}
			else {
				//cout << "sono dentro la valutazione della mossa in mezzo" << endl;
				//p.print();
				double end_day = 0.0;

				if (p.giorno == 1) {
					end_day = end_day1;

					//valuto se possibile la massa before******************************************************************************************************************************************
					//cout << "valuto l'inserimento del passeggero davanti" << endl;
					Route r_support_MB = routes_destroyed[r];
					double time_check = 0.0;
					if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}
					else {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}

					//cout << "stampo route prima dell'inserimento before" << endl;
					//r_support_MB.print1();
					//cout << "voglio inserire il passeggero" << endl;
					//p.print();
					//cout << "time_check" << time_check << " confrontato con starting day" << start_day1 << endl;
					double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


					if (time_check >= start_day1) {
						r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						//cout << "stampo caso strano dopo mossa before" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						if (route_feasible_giorno(start_day1, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
							cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;


							//code for casual number [0.8;1.2]*****************************************************
							double f = (double)rand() / RAND_MAX;
							f = 0.8 + (f * 0.4);
							cost_mb = cost_mb * f;
							//**************************************************************************************


							if (cost_mb < best_cost) {

								best_cost = cost_mb;
								move_c = false;
								move_before = true;
								route_best = r_support_MB;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
								best_route = r;

							}
						}
					}

					//system("pause");
					//***********************************************************************************************************************************************************************




					//cout << "valuto l'inserimento in mezzo" << endl;
					if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
						//cout << "primo if" << endl;
						//ha qualcosa dopo
						double start_day_new = 0.0;
						int gg = 0;
						if (routes_destroyed[r].start_end_routes[1][1] != -1) {
							start_day_new = start_day2;
							gg = 1;
						}
						else {
							start_day_new = start_day3;
							gg = 2;
						}


						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[p.giorno - 1][1], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool messa_connessione = false;
										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;
										double fuel_connection = 0.0;
										double weight_connection = 0.0;

										//system("pause");
										//cout << "route prima della manipolazione dei tempi degli archi: " << n << "  ----  " << n1 << endl;




										r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day_new, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "sto valutando il caso strano" << endl;
									//	stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane);

										/*cout << "route dopo la  manipolazione dei tempi" << endl;
										if (entrato) {
										r_support.print1();
										system("pause");
										}*/


										if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano" << endl;
										//	stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);




											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


														//code for casual number [0.8;1.2]*****************************************************
														double f = (double)rand() / RAND_MAX;
														f = 0.8 + (f * 0.4);
														cost = cost * f;
														//**************************************************************************************

														if (best_cost > cost) {
															best_arc_from = n;
															best_arc_to = n1;
															best_route = r;
															best_cost = cost;
															move_c = false;
															move_before = false;

															route_best = r_support;
															from_per_route = p.solution_from;
															to_per_route = p.solution_to;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					else {
						//cout << "secondo if" << endl;
						//non ha niente dopo
						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										//system("pause");
										//cout << "route prima della manipolazione dei tempi" << endl;
										//if (p.giorno == 1) r_support.print1();


										r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);



										//cout << "valuto stampo caso strano" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane);


										//cout << "route dopo la  manipolazione dei tempi" << endl;
										//if (p.giorno == 1) r_support.print1();

										//cout << "if di verifica dei tempi" << endl;
										if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

											//cout << "ora aggiorno anche le diverse quantità" << endl;
											r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);


											//cout << "valuto il caso strano" << endl;
										//	stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



											//cout << "route dopo la  manipolazione del fuel e peso" << endl;
											//if (p.giorno == 1) r_support.print1();
											//cout << "prima dell'if per le intermediate stop" << endl;
											if ((p.solution_to - p.solution_from <= p.stop)) {
												//cout << "prima dell'if sulla feasibility" << endl;
												//cout << "r_support size di time_arr: " << r_support.time_arr.size();
												//cout << "indice end_route giorno 1 " << r_support.start_end_routes[p.giorno - 1][1] << endl;
												if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													//cout << "dopo if feasibility" << endl;
													//cout << "costo single route: " << cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
													//cout << " costo time windows passeggero che ho inserito: " << cost_time_windows_for_route_passenger(r_support, p, map_airstrip) << endl;


													double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
													//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;

													//code for casual number [0.8;1.2]*****************************************************
													double f = (double)rand() / RAND_MAX;
													f = 0.8 + (f * 0.4);
													cost = cost * f;
													//**************************************************************************************


													if (best_cost > cost) {
														best_arc_from = n;
														best_arc_to = n1;
														best_route = r;
														best_cost = cost;
														move_c = false;
														move_before = false;

														route_best = r_support;
														from_per_route = p.solution_from;
														to_per_route = p.solution_to;
													}
												}
											}
											//cout << "dopo blocco if" << endl;
										}
									}
								}
							}
						}



					}
				}
				else if (p.giorno == 3) {
					end_day = end_day3;


					//valuto se possibile la massa before******************************************************************************************************************************************
					//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
					Route r_support_MB = routes_destroyed[r];
					double time_check = 0.0;
					if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}
					else {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}

					//cout << "stampo route prima dell'inserimento before" << endl;
					//r_support_MB.print3();
					//cout << "voglio inserire il passeggero" << endl;
					//p.print();
					//cout << "time_check" << time_check << " confrontato con starting day" << start_day3 << endl;
					double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


					if (time_check >= start_day3) {
						r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						//cout << "stampo caso strano dopo mossa before" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						if (route_feasible_giorno(start_day3, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
							cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;

							//code for casual number [0.8;1.2]*****************************************************
							double f = (double)rand() / RAND_MAX;
							f = 0.8 + (f * 0.4);
							cost_mb = cost_mb * f;
							//**************************************************************************************


							if (cost_mb < best_cost) {

								best_cost = cost_mb;
								move_c = false;
								move_before = true;
								route_best = r_support_MB;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
								best_route = r;
							}
						}
					}

					//system("pause");
					//***********************************************************************************************************************************************************************



					//cout << "valuto l'inserimento in mezzo" << endl;
					int arc_from = -1;
					int arc_to = -1;
					double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					for (int n = routes_destroyed[r].start_end_routes[2][0]; n < routes_destroyed[r].start_end_routes[2][1]; n++) {
						if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[2][0]) {
							for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[2][1]) && (n1 - n <= p.stop + 1); n1++) {
								if (p.arrival_location != routes_destroyed[r].places[n1]) {
									//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
									Route r_support = routes_destroyed[r];

									bool non_to = false;
									bool non_to_final = false;
									bool num_equals = false;
									int caso = -1;
									int node_add_from = n;
									int node_add_to = n1;

									r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

									//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane); //TOGLIOLO DOPO


									if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

										r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "valuto stampo caso strano" << endl;
										//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


										if ((p.solution_to - p.solution_from <= p.stop)) {
											if (route_feasible_giorno(start_day3, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
												double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
												//cout << " costo  "  << cost << " confrontato con best cost: " << best_cost << endl;

												//code for casual number [0.8;1.2]*****************************************************
												double f = (double)rand() / RAND_MAX;
												f = 0.8 + (f * 0.4);
												cost = cost * f;
												//**************************************************************************************


												if (best_cost > cost) {
													best_arc_from = n;
													best_arc_to = n1;
													best_route = r;
													best_cost = cost;
													move_c = false;
													move_before = false;

													route_best = r_support;
													from_per_route = p.solution_from;
													to_per_route = p.solution_to;
												}
											}
										}
									}
								}
							}
						}
					}



				}
				else if (p.giorno == 2) {
					end_day = end_day2;


					//valuto se possibile la massa before******************************************************************************************************************************************
					//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
					Route r_support_MB = routes_destroyed[r];
					double time_check = 0.0;
					if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}
					else {
						time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
						if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
						if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
						double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
							from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
						double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
						time_check -= time_volo;
					}

					//cout << "stampo route prima dell'inserimento before" << endl;
					//r_support_MB.print2();
					//cout << "voglio inserire il passeggero" << endl;
					//p.print();
					//cout << "time_check" << time_check << " confrontato con starting day" << start_day2 << endl;
					double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


					if (time_check >= start_day2) {
						r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						//cout << "stampo caso strano dopo mossa before" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						if (route_feasible_giorno(start_day2, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
							cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;

							//code for casual number [0.8;1.2]*****************************************************
							double f = (double)rand() / RAND_MAX;
							f = 0.8 + (f * 0.4);
							cost_mb = cost_mb * f;
							//**************************************************************************************


							if (cost_mb < best_cost) {

								best_cost = cost_mb;
								move_c = false;
								move_before = true;
								route_best = r_support_MB;
								from_per_route = p.solution_from;
								to_per_route = p.solution_to;
								best_route = r;

							}
						}
					}

					//system("pause");
					//***********************************************************************************************************************************************************************	

					//cout << "valuto l'inserimento in mezzo" << endl;
					if (routes_destroyed[r].start_end_routes[2][1] != -1) {
						//ha qualcosa dopo
						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[2][0], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

						for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										//p.print();
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;
										double fuel_connection = 0.0;
										double weight_connection = 0.0;
										bool messa_connessione = false;


										r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day3, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "valuto stampo caso strano nell'inserimento in mezzo nel secondo giorno" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane); //TOGLIOLO DOPO

										if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														cost += cost_routing_pezzo_after(r_support.start_end_routes[2][0], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

														//code for casual number [0.8;1.2]*****************************************************
														double f = (double)rand() / RAND_MAX;
														f = 0.8 + (f * 0.4);
														cost = cost * f;
														//**************************************************************************************


														//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;
														if (best_cost > cost) {
															best_arc_from = n;
															best_arc_to = n1;
															best_route = r;
															best_cost = cost;
															move_c = false;
															move_before = false;

															route_best = r_support;
															from_per_route = p.solution_from;
															to_per_route = p.solution_to;
														}
													}
												}
											}
										}
									}
								}
							}
						}



					}
					else {
						//non ha niente dopo

						int arc_from = -1;
						int arc_to = -1;
						double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
						for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
							if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
								for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
									if (p.arrival_location != routes_destroyed[r].places[n1]) {
										//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
										Route r_support = routes_destroyed[r];

										bool non_to = false;
										bool non_to_final = false;
										bool num_equals = false;
										int caso = -1;
										int node_add_from = n;
										int node_add_to = n1;

										r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

										//cout << "sto valutando il caso strano dei tempi" << endl;
										//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane);


										if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

											r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "valuto stampo caso strano" << endl;
											//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


											if ((p.solution_to - p.solution_from <= p.stop)) {
												if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
													double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
													//cout << " costo " << cost << " confrontato con best cost" << best_cost << endl;

													//code for casual number [0.8;1.2]*****************************************************
													double f = (double)rand() / RAND_MAX;
													f = 0.8 + (f * 0.4);
													cost = cost * f;
													//**************************************************************************************


													if (best_cost > cost) {
														best_arc_from = n;
														best_arc_to = n1;
														best_route = r;
														best_cost = cost;
														move_c = false;
														move_before = false;

														route_best = r_support;
														from_per_route = p.solution_from;
														to_per_route = p.solution_to;
													}
												}
											}
										}
									}
								}
							}
						}

					}
				}


			}

			//for move C
			if (routes_destroyed[r].primo_pass[p.giorno - 1]) {
				//cout << " sono dentro la valutazione della mossa C" << endl;
				//p.print();
				//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C

				Route r_support = routes_destroyed[r];
				double end_day = 0.0;

				bool check_pezzo_after = false;
				int index_partenza = 0;
				if (p.giorno == 1) {
					if (r_support.start_end_routes[1][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[1][0];
					}
					else if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}
				}
				else if (p.giorno == 2) {
					if (r_support.start_end_routes[2][1] != -1) {
						check_pezzo_after = true;
						index_partenza = r_support.start_end_routes[2][0];
					}

				}

				if (p.giorno == 1) {
					end_day = end_day1;
					if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
						//ha qualcosa dopo


						r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else {
						//non ha niente dopo
						r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
				}
				else if (p.giorno == 3) {
					end_day = end_day3;
					r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
				}
				else if (p.giorno == 2) {
					end_day = end_day2;
					if (routes_destroyed[r].start_end_routes[2][1] != -1) {
						//ha qualcosa dopo
						r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else {
						//non ha niente dopo
						r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
				}


				//cout << "valuto stampo caso strano dopo la mossa C" << endl;
				//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

				/*if (p.giorno == 1) {
				r_support.print1();
				system("pause");
				}*/
				//cout << "finito stampo caso strano" << endl;
				//r_support.Print();
				//if(p.giorno == 1 || p.giorno == 2) system("pause");

				if (route_feasible_giorno_moveC(start_day1, start_day2, start_day3, p.giorno - 1, r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {

					double cost_before = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					double cost = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
					if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

					cost -= cost_before;


					//code for casual number [0.8;1.2]*****************************************************
					double f = (double)rand() / RAND_MAX;
					f = 0.8 + (f * 0.4);
					cost = cost * f;
					//**************************************************************************************

					//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
					if (cost < best_cost) {
						best_cost = cost;
						best_route = r;
						move_c = true;
						move_before = false;
						best_arc_from = -2;
						best_arc_to = -2;
					}

				}

			}

		}


		//cout << "sto per fare la mossa per il passeggero: " << endl;
		//p.print();
		if (best_route == -1) {
			//cout << "il passeggero: " << endl;
			//p.print();
			//cout << "********************** ha queste possibilità******************************************" << endl;
			//for (Route c : routes_destroyed) c.Print();
			//system("pause");
			routes_infeasible = true;
			break;
		}
		else {



			if (!routes_destroyed[best_route].primo_pass[p.giorno - 1]) {
				//cout << "sto facendo la mossa primo passeggero" << endl;
				Route r_test = routes_destroyed[best_route]; //dopo cancella questa riga




				routes_destroyed[best_route].primo_pass[p.giorno - 1] = true;
				routes_destroyed[best_route].add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);

				//cout << "valuto stampo caso strano dopo l'inserimento del primo passeggero vero e prorpio" << endl;
				//stampo_caso_strano_single_all(p, map_airstrip, r_test, p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
			}
			else {

				if (move_c) {

					//cout << "sto facendo la mossa C" << endl;
					if (p.giorno == 1) {
						if (routes_destroyed[best_route].start_end_routes[1][1] != -1 || routes_destroyed[best_route].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							routes_destroyed[best_route].move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}
					else if (p.giorno == 3) {
						routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
					}
					else if (p.giorno == 2) {
						if (routes_destroyed[best_route].start_end_routes[2][1] != -1) {
							//ha qualcosa dopo
							routes_destroyed[best_route].move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else {
							//non ha niente dopo
							routes_destroyed[best_route].move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
					}

					//cout << "valuto stampo caso strano dopo l'inserimento vero e prorpio in C" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

				}
				else if (move_before) {
					//cout << "sto facendo la mossa dell'inserimento davanti" << endl;
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;

					//cout << "valuto stampo caso strano dopo l'inserimento vero e proprio in davanti" << endl;
					//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);
				}
				else {
					//cout << "sto facendo la mossa dell'inserimento in mezzo" << endl;

					//routes_destroyed[best_route].update_route_rebuilt_one(best_arc_from, best_arc_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed);
					routes_destroyed[best_route] = route_best;
					p.solution_from = from_per_route;
					p.solution_to = to_per_route;

					//cout << "valuto stampo caso strano dopo l'inserimento vero e proprio in mezzo" << endl;
					stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[best_route], p, routes_destroyed[best_route], from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

					//routes_destroyed[best_route].print();
				}
			}


			routes_destroyed[best_route].passengers_in_route.push_back(p);
		}
	}


	if (routes_infeasible) {
		vector<Route> route_vuote;
		return route_vuote;
	}
	else {
		return routes_destroyed;
	}
}

vector <Route> repair_two_regret(map<string, int>& legenda, double peso_intermediate_stop, double start_day1, double start_day2, double start_day3, double end_day1, double end_day2, double end_day3, vector<Route> routes_destroyed, map <int, Airplane>& map_airplane, vector <Passenger> passengers_removed, map<int, Airstrip>& map_airstrip, vector<vector<double>>& from_to, vector<vector<double>>& location_fuel, vector<vector<vector<double>>>& from_to_FuelConsumed) {

	//bool routes_infeasible = false;
	vector<double> regret_diff;
	vector<int> regret_best_route_index;
	vector<Route> regret_best_route_object;
	vector<int> regret_from;
	vector<int> regret_to;
	vector<int> regret_index_pass;


	map<int, int> map_pass_aggragati; //nella stringa, che � la chiave, ci sono from/to/timedep/timearr, nell'int c'� l'indicie di tutti i valori

	while (!passengers_removed.empty()) {
		int index_p = 0;

		for (Passenger& p : passengers_removed) {


			/*int best_arc_from = -1;
			int best_arc_to = -1;
			int best_route = -1;
			double best_cost = DBL_MAX;
			bool move_c = false;
			bool move_before = false;

			Route route_best;
			int from_per_route = 0;
			int to_per_route = 0;
			*/

			vector<double> costs;
			vector<int> route_index;
			vector<int> _from;
			vector<int> _to;
			vector<Route> route_object;



			int chiave = p.chiave_regret;
			if (map_pass_aggragati.count(chiave) >= 1) {
				//cout << "ho gia calcolato un passeggero simile a lui" << endl;

				regret_best_route_object.push_back(regret_best_route_object[map_pass_aggragati[chiave]]);
				regret_diff.push_back(regret_diff[map_pass_aggragati[chiave]]);
				regret_best_route_index.push_back(regret_best_route_index[map_pass_aggragati[chiave]]);
				regret_from.push_back(regret_from[map_pass_aggragati[chiave]]);
				regret_to.push_back(regret_to[map_pass_aggragati[chiave]]);
				regret_index_pass.push_back(regret_index_pass[map_pass_aggragati[chiave]]);


			}
			else {

				for (int r = 0; r < (int)routes_destroyed.size(); r++) {
					//cout << "***************************************************************************************************************" << endl;
					//cout << "route:      " << routes_destroyed[r].aircraft_code << endl;
					//routes_destroyed[r].Print();



					if (routes_destroyed[r].primo_pass[p.giorno - 1] == false) {
						//cout << "sono dentro la valutazione del primo passeggero" << endl;


						Route r_support = routes_destroyed[r];
						bool check_pezzo_after = false;
						int index_partenza = 0;
						if (p.giorno == 1) {
							if (r_support.start_end_routes[1][1] != -1) {
								check_pezzo_after = true;
								index_partenza = r_support.start_end_routes[1][0];
							}
							else if (r_support.start_end_routes[2][1] != -1) {
								check_pezzo_after = true;
								index_partenza = r_support.start_end_routes[2][0];
							}
						}
						else if (p.giorno == 2) {
							if (r_support.start_end_routes[2][1] != -1) {
								check_pezzo_after = true;
								index_partenza = r_support.start_end_routes[2][0];
							}
						}

						r_support.add_update_only_one_node_first_passanger_MP(start_day1, start_day2, start_day3, p, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);


						//cout << "valuto stampo caso strano inserimento primo passeggero" << endl;
						//stampo_caso_strano_single_all_primo_pass(p, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, map_airstrip);



						if (route_feasible_primo_pass((p.giorno - 1), end_day1, end_day2, end_day3, r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
							double cost_before = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							//cout << "cost before: " << cost_before << endl;
							double cost = cost_single_route_giorno_primo_pass((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							//cout << "cost after: " << cost << endl;
							cost -= cost_before;
							//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;

							costs.push_back(cost);
							route_index.push_back(r);
							_from.push_back(p.solution_from);
							_to.push_back(p.solution_to);
							route_object.push_back(r_support);



						}

						//system("pause");

					}
					else {
						//cout << "sono dentro la valutazione della mossa in mezzo" << endl;
						//p.print();
						double end_day = 0.0;

						if (p.giorno == 1) {
							end_day = end_day1;

							//valuto se possibile la massa before******************************************************************************************************************************************
							//cout << "valuto l'inserimento del passeggero davanti" << endl;
							Route r_support_MB = routes_destroyed[r];
							double time_check = 0.0;
							if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
								time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
								if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
								if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
								double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
									from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
								double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
								time_check -= time_volo;
							}
							else {
								time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
								if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
								if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
								double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
									from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
								double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
								time_check -= time_volo;
							}

							//cout << "stampo route prima dell'inserimento before" << endl;
							//r_support_MB.print1();
							//cout << "voglio inserire il passeggero" << endl;
							//p.print();
							//cout << "time_check" << time_check << " confrontato con starting day" << start_day1 << endl;
							double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


							if (time_check >= start_day1) {
								r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
								//cout << "stampo caso strano dopo mossa before" << endl;
								//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

								if (route_feasible_giorno(start_day1, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
									cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;

									costs.push_back(cost_mb);
									route_index.push_back(r);
									_from.push_back(p.solution_from);
									_to.push_back(p.solution_to);
									route_object.push_back(r_support_MB);


								}
							}

							//system("pause");
							//***********************************************************************************************************************************************************************




							//cout << "valuto l'inserimento in mezzo" << endl;
							if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
								//cout << "primo if" << endl;
								//ha qualcosa dopo
								double start_day_new = 0.0;
								int gg = 0;
								if (routes_destroyed[r].start_end_routes[1][1] != -1) {
									start_day_new = start_day2;
									gg = 1;
								}
								else {
									start_day_new = start_day3;
									gg = 2;
								}


								int arc_from = -1;
								int arc_to = -1;
								double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[p.giorno - 1][1], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
									if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
										for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
											if (p.arrival_location != routes_destroyed[r].places[n1]) {
												//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
												Route r_support = routes_destroyed[r];

												bool messa_connessione = false;
												bool non_to = false;
												bool non_to_final = false;
												bool num_equals = false;
												int caso = -1;
												int node_add_from = n;
												int node_add_to = n1;
												double fuel_connection = 0.0;
												double weight_connection = 0.0;

												//system("pause");
												//cout << "route prima della manipolazione dei tempi degli archi: " << n << "  ----  " << n1 << endl;




												r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day_new, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "sto valutando il caso strano tempi inserimento in mezzo" << endl;
												//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip);

												/*cout << "route dopo la  manipolazione dei tempi" << endl;
												if (entrato) {
												r_support.print1();
												system("pause");
												}*/


												if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

													r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

													//cout << "valuto stampo caso strano in mezzo all" << endl;
													//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);




													if ((p.solution_to - p.solution_from <= p.stop)) {
														if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
															if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
																double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
																cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

																costs.push_back(cost);
																route_index.push_back(r);
																_from.push_back(p.solution_from);
																_to.push_back(p.solution_to);
																route_object.push_back(r_support);


															}
														}
													}
												}
											}
										}
									}
								}
							}
							else {
								//cout << "secondo if" << endl;
								//non ha niente dopo
								int arc_from = -1;
								int arc_to = -1;
								double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								for (int n = 0; n < routes_destroyed[r].start_end_routes[0][1]; n++) {
									if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[0][0]) {
										for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[0][1]) && (n1 - n <= p.stop + 1); n1++) {
											if (p.arrival_location != routes_destroyed[r].places[n1]) {
												//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
												Route r_support = routes_destroyed[r];

												bool non_to = false;
												bool non_to_final = false;
												bool num_equals = false;
												int caso = -1;
												int node_add_from = n;
												int node_add_to = n1;

												//system("pause");
												//cout << "route prima della manipolazione dei tempi" << endl;
												//if (p.giorno == 1) r_support.print1();


												r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);



												//cout << "valuto stampo caso strano senza after in mezzo tempi" << endl;
												//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip);


												//cout << "route dopo la  manipolazione dei tempi" << endl;
												//if (p.giorno == 1) r_support.print1();

												//cout << "if di verifica dei tempi" << endl;
												if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

													//cout << "ora aggiorno anche le diverse quantità" << endl;
													r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);


													//cout << "valuto il caso strano in mezzo senza after all" << endl;
													//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



													//cout << "route dopo la  manipolazione del fuel e peso" << endl;
													//if (p.giorno == 1) r_support.print1();
													//cout << "prima dell'if per le intermediate stop" << endl;
													if ((p.solution_to - p.solution_from <= p.stop)) {
														//cout << "prima dell'if sulla feasibility" << endl;
														//cout << "r_support size di time_arr: " << r_support.time_arr.size();
														//cout << "indice end_route giorno 1 " << r_support.start_end_routes[p.giorno - 1][1] << endl;
														if (route_feasible_giorno(start_day1, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
															//cout << "dopo if feasibility" << endl;
															//cout << "costo single route: " << cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
															//cout << " costo time windows passeggero che ho inserito: " << cost_time_windows_for_route_passenger(r_support, p, map_airstrip) << endl;


															double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
															//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;
															//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;


															costs.push_back(cost);
															route_index.push_back(r);
															_from.push_back(p.solution_from);
															_to.push_back(p.solution_to);
															route_object.push_back(r_support);

														}
													}
													//cout << "dopo blocco if" << endl;
												}
											}
										}
									}
								}



							}
						}
						else if (p.giorno == 3) {
							end_day = end_day3;


							//valuto se possibile la massa before******************************************************************************************************************************************
							//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
							Route r_support_MB = routes_destroyed[r];
							double time_check = 0.0;
							if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
								time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
								if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
								if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
								double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
									from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
								double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
								time_check -= time_volo;
							}
							else {
								time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
								if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
								if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
								double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
									from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
								double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
								time_check -= time_volo;
							}

							//cout << "stampo route prima dell'inserimento before" << endl;
							//r_support_MB.print3();
							//cout << "voglio inserire il passeggero" << endl;
							//p.print();
							//cout << "time_check" << time_check << " confrontato con starting day" << start_day3 << endl;
							double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


							if (time_check >= start_day3) {
								r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
								//cout << "stampo caso strano dopo mossa before" << endl;
								//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

								if (route_feasible_giorno(start_day3, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
									cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;


									costs.push_back(cost_mb);
									route_index.push_back(r);
									_from.push_back(p.solution_from);
									_to.push_back(p.solution_to);
									route_object.push_back(r_support_MB);


								}
							}

							//system("pause");
							//***********************************************************************************************************************************************************************



							//cout << "valuto l'inserimento in mezzo" << endl;
							int arc_from = -1;
							int arc_to = -1;
							double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							for (int n = routes_destroyed[r].start_end_routes[2][0]; n < routes_destroyed[r].start_end_routes[2][1]; n++) {
								if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[2][0]) {
									for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[2][1]) && (n1 - n <= p.stop + 1); n1++) {
										if (p.arrival_location != routes_destroyed[r].places[n1]) {
											//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
											Route r_support = routes_destroyed[r];

											bool non_to = false;
											bool non_to_final = false;
											bool num_equals = false;
											int caso = -1;
											int node_add_from = n;
											int node_add_to = n1;

											r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

											//cout << "stampo caso strano in mezzo senza after tempi" << endl;
											//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip); //TOGLIOLO DOPO


											if (r_support.time_arr[r_support.start_end_routes[0][1]] <= end_day) {

												r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "valuto stampo caso strano senza after all" << endl;
												//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


												if ((p.solution_to - p.solution_from <= p.stop)) {
													if (route_feasible_giorno(start_day3, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
														double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
														//cout << " costo  "  << cost << " confrontato con best cost: " << best_cost << endl;

														costs.push_back(cost);
														route_index.push_back(r);
														_from.push_back(p.solution_from);
														_to.push_back(p.solution_to);
														route_object.push_back(r_support);

													}
												}
											}
										}
									}
								}
							}



						}
						else if (p.giorno == 2) {
							end_day = end_day2;


							//valuto se possibile la massa before******************************************************************************************************************************************
							//cout << "valuto l'inserimento del primo passeggero davanti" << endl;
							Route r_support_MB = routes_destroyed[r];
							double time_check = 0.0;
							if (r_support_MB.capacity[r_support_MB.start_end_routes[p.giorno - 1][0]] == 0) {
								time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0] + 1] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]].ground_time;
								if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]) time_check -= map_airstrip[p.arrival_location].ground_time;
								if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
								double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
									from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0] + 1]];
								double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
								time_check -= time_volo;
							}
							else {
								time_check = r_support_MB.time_dep[r_support_MB.start_end_routes[p.giorno - 1][0]] - map_airstrip[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]].ground_time;
								if (p.arrival_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.arrival_location].ground_time;
								if (p.departure_location != r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]) time_check -= map_airstrip[p.departure_location].ground_time;
								double distance = from_to[r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]][p.departure_location] +
									from_to[p.departure_location][p.arrival_location] + from_to[p.arrival_location][r_support_MB.places[r_support_MB.start_end_routes[p.giorno - 1][0]]];
								double time_volo = (distance / map_airplane[r_support_MB.aircraft_code].speed) * 60;
								time_check -= time_volo;
							}

							//cout << "stampo route prima dell'inserimento before" << endl;
							//r_support_MB.print2();
							//cout << "voglio inserire il passeggero" << endl;
							//p.print();
							//cout << "time_check" << time_check << " confrontato con starting day" << start_day2 << endl;
							double cost_mb = (-1) * cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);


							if (time_check >= start_day2) {
								r_support_MB.move_before(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
								//cout << "stampo caso strano dopo mossa before" << endl;
								//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support_MB, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

								if (route_feasible_giorno(start_day2, p.giorno - 1, r_support_MB, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
									cost_mb += cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support_MB, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
									//cout << "costo: " << cost << "confrontato con best cost: " << best_cost << endl;

									costs.push_back(cost_mb);
									route_index.push_back(r);
									_from.push_back(p.solution_from);
									_to.push_back(p.solution_to);
									route_object.push_back(r_support_MB);



								}
							}

							//system("pause");
							//***********************************************************************************************************************************************************************	

							//cout << "valuto l'inserimento in mezzo" << endl;
							if (routes_destroyed[r].start_end_routes[2][1] != -1) {
								//ha qualcosa dopo
								int arc_from = -1;
								int arc_to = -1;
								double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								cost_route_before += cost_routing_pezzo_after(routes_destroyed[r].start_end_routes[2][0], peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

								for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
									if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
										for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
											if (p.arrival_location != routes_destroyed[r].places[n1]) {
												//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
												//p.print();
												Route r_support = routes_destroyed[r];

												bool non_to = false;
												bool non_to_final = false;
												bool num_equals = false;
												int caso = -1;
												int node_add_from = n;
												int node_add_to = n1;
												double fuel_connection = 0.0;
												double weight_connection = 0.0;
												bool messa_connessione = false;


												r_support.update_route_rebuilt_one_first_fase_with_after(messa_connessione, start_day3, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "valuto stampo caso strano nell'inserimento in mezzo nel secondo giorno con after tempi" << endl;
												//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip); //TOGLIOLO DOPO

												if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

													r_support.update_route_rebuilt_one_second_fase_with_after(messa_connessione, fuel_connection, weight_connection, caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

													//cout << "valuto stampo caso strano all con after" << endl;
													//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);



													if ((p.solution_to - p.solution_from <= p.stop)) {
														if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
															if (route_feasible_pezzo_after(start_day1, start_day2, start_day3, (p.giorno - 1), r_support, map_airplane, from_to, location_fuel, from_to_FuelConsumed)) {
																double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
																cost += cost_routing_pezzo_after(r_support.start_end_routes[2][0], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
																//cout << " costo fra archi: " << n << " - " << n1 << ", ha costo " << cost << endl;



																costs.push_back(cost);
																route_index.push_back(r);
																_from.push_back(p.solution_from);
																_to.push_back(p.solution_to);
																route_object.push_back(r_support);

															}
														}
													}
												}
											}
										}
									}
								}



							}
							else {
								//non ha niente dopo

								int arc_from = -1;
								int arc_to = -1;
								double cost_route_before = cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
								for (int n = routes_destroyed[r].start_end_routes[1][0]; n < routes_destroyed[r].start_end_routes[1][1]; n++) {
									if (p.departure_location != routes_destroyed[r].places[n] || n == routes_destroyed[r].start_end_routes[1][0]) {
										for (int n1 = n; (n1 <= routes_destroyed[r].start_end_routes[1][1]) && (n1 - n <= p.stop + 1); n1++) {
											if (p.arrival_location != routes_destroyed[r].places[n1]) {
												//cout << "sto valutando l'inserzione in: " << n << " e " << n1 << endl;
												Route r_support = routes_destroyed[r];

												bool non_to = false;
												bool non_to_final = false;
												bool num_equals = false;
												int caso = -1;
												int node_add_from = n;
												int node_add_to = n1;

												r_support.update_route_rebuilt_one_first_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

												//cout << "sto valutando il caso strano dei tempi senza niente dopo" << endl;
												//stampo_caso_strano_single_route_tempi(p, routes_destroyed[r], r_support, from_to, map_airplane, map_airstrip);


												if (r_support.time_arr[r_support.start_end_routes[1][1]] <= end_day) {

													r_support.update_route_rebuilt_one_second_fase_without_after(caso, node_add_from, node_add_to, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, p, from_to_FuelConsumed, non_to, non_to_final, num_equals);

													//cout << "valuto stampo caso strano senza after all" << endl;
													//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);


													if ((p.solution_to - p.solution_from <= p.stop)) {
														if (route_feasible_giorno(start_day2, (p.giorno - 1), r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {
															double cost = (cost_single_route_giorno((p.giorno - 1), peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) + cost_time_windows_for_route_passenger(r_support, p, map_airstrip)) + (peso_intermediate_stop*(p.solution_to - p.solution_from - 1)) - cost_route_before;
															//cout << " costo " << cost << " confrontato con best cost" << best_cost << endl;


															costs.push_back(cost);
															route_index.push_back(r);
															_from.push_back(p.solution_from);
															_to.push_back(p.solution_to);
															route_object.push_back(r_support);
														}
													}
												}
											}
										}
									}
								}

							}
						}


					}

					//for move C
					if (routes_destroyed[r].primo_pass[p.giorno - 1]) {
						//cout << " sono dentro la valutazione della mossa C" << endl;
						//p.print();
						//if there is at least one passenger inside the route, i evaluete the possibility to put him/her inside the route with the move C

						Route r_support = routes_destroyed[r];
						double end_day = 0.0;

						bool check_pezzo_after = false;
						int index_partenza = 0;
						if (p.giorno == 1) {
							if (r_support.start_end_routes[1][1] != -1) {
								check_pezzo_after = true;
								index_partenza = r_support.start_end_routes[1][0];
							}
							else if (r_support.start_end_routes[2][1] != -1) {
								check_pezzo_after = true;
								index_partenza = r_support.start_end_routes[2][0];
							}
						}
						else if (p.giorno == 2) {
							if (r_support.start_end_routes[2][1] != -1) {
								check_pezzo_after = true;
								index_partenza = r_support.start_end_routes[2][0];
							}

						}

						if (p.giorno == 1) {
							end_day = end_day1;
							if (routes_destroyed[r].start_end_routes[1][1] != -1 || routes_destroyed[r].start_end_routes[2][1] != -1) {
								//ha qualcosa dopo


								r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							}
							else {
								//non ha niente dopo
								r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							}
						}
						else if (p.giorno == 3) {
							end_day = end_day3;
							r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
						}
						else if (p.giorno == 2) {
							end_day = end_day2;
							if (routes_destroyed[r].start_end_routes[2][1] != -1) {
								//ha qualcosa dopo
								r_support.move_c_with_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							}
							else {
								//non ha niente dopo
								r_support.move_c_without_after(p, p.departure_location, p.arrival_location, from_to, map_airplane, map_airstrip, from_to_FuelConsumed);
							}
						}


						//cout << "valuto stampo caso strano dopo la mossa C" << endl;
						//stampo_caso_strano_single_all(p, map_airstrip, routes_destroyed[r], p, r_support, from_to, map_airplane, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3);

						/*if (p.giorno == 1) {
						r_support.print1();
						system("pause");
						}*/
						//cout << "finito stampo caso strano" << endl;
						//r_support.Print();
						//if(p.giorno == 1 || p.giorno == 2) system("pause");

						if (route_feasible_giorno_moveC(start_day1, start_day2, start_day3, p.giorno - 1, r_support, map_airplane, end_day, from_to, location_fuel, from_to_FuelConsumed)) {

							double cost_before = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (check_pezzo_after) cost_before += cost_routing_pezzo_after(index_partenza, peso_intermediate_stop, routes_destroyed[r], map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							double cost = cost_single_route_giorno(p.giorno - 1, peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
							if (check_pezzo_after) cost += cost_routing_pezzo_after(r_support.start_end_routes[p.giorno - 1][1], peso_intermediate_stop, r_support, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);

							cost -= cost_before;

							//cout << "costo: " << cost << "   confrontato con best cost " << best_cost << endl;


							costs.push_back(cost);
							route_index.push_back(r);
							_from.push_back(p.solution_from);
							_to.push_back(p.solution_to);
							route_object.push_back(r_support);

						}

					}

				}


				if (costs.size() == 1) {

					//cout << "il passegero:";
					//p.print();
					//cout << " ha solo una mossa di inserimento possibile" << endl;


					//regret_diff.push_back(0);
					regret_diff.push_back(0);  //metto valore zero ma non dovrebbe succedere
					regret_best_route_index.push_back(route_index[0]);
					regret_best_route_object.push_back(route_object[0]);
					regret_from.push_back(_from[0]);
					regret_to.push_back(_to[0]);
					regret_index_pass.push_back(index_p);







				}
				else if (costs.size() > 1) {
					//qua lo posso ottimizzare****************************************************************************************************************************************
					//con il codice di TED per ordinare, guarda bene come salvare l'indice********************************************************************************************


					vector<double>::iterator index_iterator = min_element(costs.begin(), costs.end());
					int index_mosse = distance(costs.begin(), index_iterator);
					double best_cost = costs[index_mosse];
					costs.erase(costs.begin() + index_mosse);
					vector<double>::iterator index_iterator2 = min_element(costs.begin(), costs.end());
					double best_cost_2 = costs[distance(costs.begin(), index_iterator2)];



					regret_diff.push_back(best_cost_2 - best_cost);
					regret_best_route_index.push_back(route_index[index_mosse]);
					regret_best_route_object.push_back(route_object[index_mosse]);
					regret_from.push_back(_from[index_mosse]);
					regret_to.push_back(_to[index_mosse]);
					regret_index_pass.push_back(index_p);
				}
				else {
					//caso in cui non ha trovato mosse per quella persona
					//in questo caso vuol dire che la solutione � infeasible
					cout << "LA SOLUZIONE E' INFEASIBLE" << endl;
					vector<Route> route_vuote;
					return route_vuote;

				}


				int val = (int)regret_diff.size() - 1;
				//cout << val << endl;
				//string primo_val = to_string(p.departure_location) + "/" + to_string(p.arrival_location) + "/" + to_string(p.departure_time) + "/" + to_string(p.arrival_time);
				int primo_val = p.chiave_regret;
				map_pass_aggragati.insert(make_pair(primo_val, val));


				costs.clear();
				route_index.clear();
				_from.clear();
				_to.clear();
				route_object.clear();

			}

			index_p++;
		}

		int index = 0;
		double best = DBL_MIN; //prima era -10000
		for (int i = 0; i < (int)regret_diff.size(); i++) {
			//cout << "***********" << regret_diff[i] << endl;

			if (best < regret_diff[i]) {
				index = i;
				best = regret_diff[i];
			}
		}


		//qua devo fare la mossa vera e propria
		routes_destroyed[regret_best_route_index[index]] = regret_best_route_object[index];
		passengers_removed[regret_index_pass[index]].solution_from = regret_from[index];
		passengers_removed[regret_index_pass[index]].solution_to = regret_to[index];
		routes_destroyed[regret_best_route_index[index]].passengers_in_route.push_back(passengers_removed[regret_index_pass[index]]);
		if (!routes_destroyed[regret_best_route_index[index]].primo_pass[passengers_removed[regret_index_pass[index]].giorno - 1]) {
			routes_destroyed[regret_best_route_index[index]].primo_pass[passengers_removed[regret_index_pass[index]].giorno - 1] = true;
		}




		//QUA DEVO TOGLIERE IL PASSEGGERO CHE HO APPENA INSERITO DAL POOL DI PASSENGERS_REMOVED
		vector<Passenger>::iterator it;
		it = passengers_removed.begin();
		passengers_removed.erase(it + regret_index_pass[index]);


		regret_diff.clear();
		regret_best_route_index.clear();
		regret_best_route_object.clear();
		regret_from.clear();
		regret_to.clear();
		regret_index_pass.clear();
		map_pass_aggragati.clear();
	}


	return routes_destroyed;
}











//data una location mi restituisce la location più vicina
int location_closest_with_fuel(int location, vector<vector<double>>& from_to, map<int, Airstrip>& map_airstrip) {
	int result = -1;
	double value = DBL_MAX;

	for (int i = 1; i < numero_airstrip_const; i++) {
		for (int j = 1; j < numero_airstrip_const; j++) {
			if (i == location && from_to[i][j] < value && map_airstrip[j].fuel) {
				result = j;
				value = from_to[i][j];
			}
		}
	}


	return result;

}

//per il tratto finale
void fillLocation_fuel(vector<vector<double>>& risultato, vector<Airstrip>& airstrips, vector<Airplane>& airplanes, vector<vector<double>>& from_to, map<int, Airstrip>& map_airstrip) {


	risultato.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++)
		risultato[i].resize((size_t)numero_airstrip_const);



	for (Airplane f : airplanes) {
		for (Airstrip a : airstrips) {
			if (a.fuel) {
				risultato[f.code][a.code] = 0.0;
				//risultato.insert(make_pair(f.code + "/" + a.code, 0.0));
			}
			else {
				double fuel_needed = 0;
				double time_fly = from_to[a.code][location_closest_with_fuel(a.code, from_to, map_airstrip)] / f.speed;
				//double time_fly = from_to[a.code + ";" + location_closest_with_fuel(a.code, from_to, map_airstrip)] / f.speed;
				if (time_fly <= 1) {
					fuel_needed = time_fly * f.fuel_burn_first;
				}
				else {
					fuel_needed = f.fuel_burn_first + (time_fly - 1) * f.fuel_burn_second;
				}

				risultato[f.code][a.code] = fuel_needed;
				//risultato.insert(make_pair(f.code + "/" + a.code, fuel_needed));
			}
		}
	}




}


//**********************************************************************************per rolling************************************************************************
map < int, int > fill_to_closeness_fuel(vector<vector<double>>& from_to, vector<Airstrip>& airstrips) {
	map <int, int> to_closeness_fuel;

	for (Airstrip a : airstrips) {
		double best_distance = DBL_MAX;
		int best_location = -1;
		if (a.fuel) to_closeness_fuel.insert(make_pair(a.code, a.code));
		else {
			for (Airstrip a1 : airstrips) {
				if (a.code != a1.code) {
					if (a1.fuel) {
						if (best_distance > from_to[a.code][a1.code]) {
							best_distance = from_to[a.code][a1.code];
							best_location = a1.code;
						}
					}
				}
			}

			to_closeness_fuel.insert(make_pair(a.code, best_location));
		}
	}

	return to_closeness_fuel;
}

map<int, int> fill_location_request(vector<Passenger>& passengers) {

	map<int, int> mappa;

	for (Passenger& p : passengers) {
		if (mappa.count(p.departure_location) >= 1) {
			mappa[p.departure_location]++;
		}
		else {
			mappa.insert(make_pair(p.departure_location, 1));
		}
	}


	return mappa;
}

int best_location(map <int, int>& location_request) {
	int best_richieste = 0;
	int best_location = -1;


	for (auto x : location_request) {
		if (best_richieste < x.second) {
			best_richieste = x.second;
			best_location = x.first;
		}
	}


	return best_location;
}

//questa funzione ritorna il posto più vicino alle 4 best location rispetto alle richieste che si hanno in quel giorno ma che non è uno di quei posti
int best_location_4place(map<int, int>& location_request, vector<vector<double>>& from_to, vector<Airstrip>& airstrips) {
	map <int, int> map_support;
	for (auto x : location_request) {
		//cout << x.first << " ---> " << x.second << endl;
		map_support.insert(make_pair(x.second, x.first));
	}

	vector <int> best_places;
	int index = 0;
	for (auto x : map_support) {
		//cout << x.first << " ***---> " << x.second << endl;
		if (index >= map_support.size() - 4) best_places.push_back(x.second);
		index++;
	}
	//for (auto x : best_places) cout << "- " << x << endl;

	int best_place = -1;
	double best_distance = DBL_MAX;


	for (Airstrip a : airstrips) {
		if (a.code != best_places[0] && a.code != best_places[1] && a.code != best_places[2] && a.code != best_places[3]) {
			double distance = from_to[a.code][best_places[0]] + from_to[a.code][best_places[1]] + from_to[a.code][best_places[2]] + from_to[a.code][best_places[3]];
			if (distance < best_distance) {
				best_distance = distance;
				best_place = a.code;
			}

		}
	}



	return best_place;
}


void fuel_min_80_major_places(vector<vector<double>>& mappa_da_tornare, vector<vector<vector<double>>>& from_to_FuelConsumed, map<int, int>& location_request, vector<Airplane>& airplanes, vector<Airstrip>& airstrips) {

	//ho inizializato la mappa
	mappa_da_tornare.resize((size_t)numero_airplane_const);
	for (int i = 0; i < numero_airplane_const; i++)
		mappa_da_tornare[i].resize((size_t)numero_airstrip_const);




	map <int, int> map_support;
	for (auto x : location_request) {
		//cout << x.first << " ---> " << x.second << endl;
		map_support.insert(make_pair(x.second, x.first));
	}

	vector <int> best_places;
	int index = 0;
	double value = round(map_support.size() * 0.2);
	for (auto x : map_support) {
		//cout << x.first << " ***---> " << x.second << endl;
		if (index >= value) best_places.push_back(x.second);
		index++;
	}
	//for (auto x : best_places) cout << " - " << x << endl;


	//codice che crea la mappa
	for (const Airstrip& a : airstrips) {
		double best_fuel = 0.0;
		int destination = -1;
		for (int a1 : best_places) {
			if (best_fuel < from_to_FuelConsumed[1][a.code][a1]) {
				best_fuel = from_to_FuelConsumed[1][a.code][a1];
				destination = a1;
			}
		}
		for (const Airplane& f : airplanes) {
			mappa_da_tornare[f.code][a.code] = from_to_FuelConsumed[f.code][a.code][destination];
			//mappa_da_tornare.insert(make_pair(f.code + ";" + a.code, from_to_FuelConsumed[f.code + ";" + a.code + ";" + destination]));
		}
	}

	//return mappa_da_tornare;
}

vector <Passenger> fill_all_passenger(vector<Passenger>& pass1, vector<Passenger>& pass2, vector<Passenger>& pass3) {

	vector<Passenger> all_passenger;
	double time_day2 = 24 * 60;
	double time_day3 = 48 * 60;

	/*cout << "time giorno due: " << time_day2 << endl;
	cout << "time giorno tre: " << time_day3 << endl;
	cout << "passeggeri nel giorno 1: " << pass1.size() << endl;
	cout << "passeggeri nel giorno 2: " << pass2.size() << endl;
	cout << "passeggeri nel giorno 3: " << pass3.size() << endl;
	*/
	int index = 0;
	for (Passenger& p : pass1) {
		p.pnr = index;
		p.giorno = 1;
		all_passenger.push_back(p);
		index++;
	}
	for (Passenger& p : pass2) {
		p.pnr = index;
		p.giorno = 2;
		p.arrival_time += time_day2;
		p.departure_time += time_day2;
		p.early_arrival += time_day2;
		p.early_departure += time_day2;
		p.late_arrival += time_day2;
		p.late_departure += time_day2;
		p.mean_arrival += time_day2;
		p.mean_departure += time_day2;

		all_passenger.push_back(p);
		index++;
	}
	for (Passenger& p : pass3) {
		p.pnr = index;
		p.giorno = 3;
		p.arrival_time += time_day3;
		p.departure_time += time_day3;
		p.early_arrival += time_day3;
		p.early_departure += time_day3;
		p.late_arrival += time_day3;
		p.late_departure += time_day3;
		p.mean_arrival += time_day3;
		p.mean_departure += time_day3;

		all_passenger.push_back(p);
		index++;
	}





	//cout << "passeggeri in all_passenger: " << all_passenger.size() << endl;
	return all_passenger;
}









//***********************************MAIN****************************
int main(int argc, char* argv[]) {

	double peso_itermediate_stop = 10.0;  //valore che si da ad una sosta intermedia 
	double peso_transhipment = 20.0;  //valore che si da ad un transhipment
	double peso_TW = 1.0;

	//**************************INPUT***********************************
	//creo una legenda
	map<int, string> legenda_ritroso;
	map<string, int> legenda = fillLegenda("legenda.csv", legenda_ritroso);

	//reading of AIRPLANE.CSV and creation VECTOR OF AIRPLANES
	vector<Airplane> airplanes = fillAirplane("NewAirplaneAll.csv", legenda);
	//cout << "***********************Print airplane***********************" << endl;
	//for (auto f : airplanes) f.print();
	//reading of AIRSTRIP.CSV and creation VECTOR OF AIRSTRIPS
	vector<Airstrip> airstrips = fillAirstrip("NewAirstrip.csv", legenda);
	//cout << "***********************Print airstrips***********************" << endl;
	//for (auto f : airstrips) f.print();
	//reading of INSTANCE.CSV and creation VECTOR OF PASSENGER
	vector<Passenger> passengers_day1 = fillPassenger(argv[1], legenda);
	vector<Passenger> passengers_day2 = fillPassenger(argv[5], legenda);
	vector<Passenger> passengers_day3 = fillPassenger(argv[9], legenda);
	//cout << "***********************Print passengers***********************" << endl;
	//ATTENTO, RICORDATI DI METTERE BAMBINI



	//bulding of hashmap from_to reading the file Matrix.txt
	map<string, double> from_to_company = fillMatrixCompany("Matrix.csv", airstrips);
	//bulding of hashmap from_to reading the file Matrix.txt
	vector<vector<double>> from_to;
	fillMatrix(from_to, "Matrix.csv", airstrips);
	map<int, Airplane> map_airplane;
	for (auto f : airplanes) map_airplane.insert(make_pair(f.code, f));
	map<int, Airstrip> map_airstrip;
	for (auto f : airstrips) map_airstrip.insert(make_pair(f.code, f));

	vector<vector<double>> location_fuel;
	fillLocation_fuel(location_fuel, airstrips, airplanes, from_to, map_airstrip);

	vector<vector<vector<double>>> from_to_FuelConsumed;
	fill_from_to_fuel_consumed(from_to_FuelConsumed, from_to, airplanes);
	map<string, double> from_to_fuel_consumed_string = fill_from_to_fuel_consumed_string(from_to_company, airplanes);

	double Company_Solution = calculationCostCompany_three_days_disaggregato(peso_TW, from_to_fuel_consumed_string, peso_transhipment, peso_itermediate_stop, argv[2], argv[6], argv[10], argv[3], argv[7], argv[11], airstrips, airplanes, passengers_day1, passengers_day2, passengers_day3, from_to_company);
	cout << " Costo della soluzione della compagnia per i tre giorni = " << Company_Solution << endl;
	//system("pause");

	//ora creo la mappa che mi dice per ogni locazione dov'è il posto più vicino per fare benzina, se nel posto considerato posso fare benzina il valore sarà uguale alla chiave
	map < int, int> to_closeness_fuel = fill_to_closeness_fuel(from_to, airstrips);

	map <int, int> location_request_day2 = fill_location_request(passengers_day2);

	int best_location_day2 = best_location(location_request_day2);

	map <int, int> location_request_day3 = fill_location_request(passengers_day3);

	int best_location_day3 = best_location(location_request_day3);

	sort(passengers_day1.begin(), passengers_day1.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
	});

	sort(passengers_day2.begin(), passengers_day2.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
	});

	sort(passengers_day3.begin(), passengers_day3.end(), [](const Passenger& lhs, const Passenger& rhs) {
		return lhs.mean_departure < rhs.mean_departure;
		//return lhs.mean_arrival < rhs.mean_arrival;
	});

	int best_location_4places_day2 = best_location_4place(location_request_day2, from_to, airstrips);
	int best_location_4places_day3 = best_location_4place(location_request_day3, from_to, airstrips);

	vector<vector<double>> min_fuel_day2;
	vector<vector<double>> min_fuel_day3;
	fuel_min_80_major_places(min_fuel_day2, from_to_FuelConsumed, location_request_day2, airplanes, airstrips);
	fuel_min_80_major_places(min_fuel_day3, from_to_FuelConsumed, location_request_day3, airplanes, airstrips);


	vector<Passenger> all_passenger = fill_all_passenger(passengers_day1, passengers_day2, passengers_day3);
	fix_key_passenger_for_regret(all_passenger);

	map<int, Passenger> map_id_passenger = fillMapPassenger(all_passenger);



	cout << "***********************End reading input quindi comincio a contare il tempo***********************" << endl;

	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	// *******************************************************DA QUA INCOMINCIA IL CODICE DEL MAIN, PRIMA CI SONO SOLO STRUTTURE DATI***********************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************
	//******************************************************************************************************************************************************************



	//PER FARE LA PROVA CHE LE COSE FUNZIONANO**********************************************************************************************************************
	double start_day1 = 360.0;
	double end_day1 = 1110.0;
	double start_day2 = start_day1 + (24 * 60);
	double start_day3 = start_day2 + (24 * 60);
	double end_day2 = end_day1 + (24 * 60);
	double end_day3 = end_day2 + (24 * 60);
	cout << endl;
	cout << "ecco i vari start ed end day" << endl;
	cout << start_day1 << " - " << end_day1 << endl;
	cout << start_day2 << " - " << end_day2 << endl;
	cout << start_day3 << " - " << end_day3 << endl;



	ofstream outfile; //per il file di input grafico.txt++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	outfile.open("grafico.txt");
	int number_iteration_in_heuristic = 0;

	double time_spent_model = 0.0;
	double tempo_finale;
	time_t time_now = time(NULL);
	//input heuristic
	//int number_airplane = stoi(argv[4]); // Per Modello
	int number_airplane = 12;  //per il modello
	//cout << "il numero degli aerei e: " << number_airplane << endl;
	//cin.get();
	int Iter_FirstDo = 0;
	int iterMAX_FirstDo = 1;
	//double best_solution_ever = DBL_MAX;
	int NDestroy = 4;
	int NRepair = 4;
	int NHeuristic = 2;
	int initial_weigth = 10;
	vector<double> Weigth_destroy;
	vector<double> Weigth_repair;
	vector<double> Weigth_heuristic(NHeuristic);
	for (int i = 0; i < NDestroy; i++) Weigth_destroy.push_back(initial_weigth);
	for (int i = 0; i < NRepair; i++) Weigth_repair.push_back(initial_weigth);
	Weigth_heuristic[0] = 1.00; //parallel
	Weigth_heuristic[1] = 0.00; //sequential
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	vector<vector<Route>> solutionAll;
	//vector<Route> best_solution_route_ever;
	srand(time(NULL));
	int NumeroSA = 0;
	int iterazioni_fallite = 0;
	int number_of_aircraft = 20;
	vector<Route> start_solution_route;
	double start_solution = 0.0;
	int start_route_bs = 0;
	//int size_route_bs = 0;  //se ci sono dei problemi rimettilo
	int best_iteration = 0;
	do {
		cout << " ************************************************************ Inizio un altro giro nel ciclo grande ************************************************************ " << endl;
		double r;
		double best_solution = DBL_MAX;
		int iteration = 0;
		int iteration_max = 50; //35 se vuopi fare un solo giro 
		int T = 100000;
		int Tmin = 500;    //50 un solo giro
		int k = 0;
		double MinLevel = 2;
		double MaxLevel = 5;
		// Valore che puo variare 0.8 a 0.99
		double alpha = 0.99; // 0.992 un solo giro
		int DeltaK = 10;      // 30 prima
		int choosen_heuristic = -1;
		if (Iter_FirstDo > 0) {
			T = 100;        // Almeno 2000
			alpha = 0.9998; // Almeno 0.997
			DeltaK = 35; //Almeno 50
			Tmin = 50;
			iteration_max = 100;
		}
		if (Iter_FirstDo == 0) {

			double heuristic_choice = (double)rand() / RAND_MAX;
			//cout << " heuristic_choice =  " << heuristic_choice << endl;
			//for (int i = 0; i < NHeuristic; i++) cout << Accumulated(i, Weigth_heuristic) << " || ";
			//cout << endl;
			int npass = 0;
			do {
				//auto rng1 = default_random_engine{};
				//random_shuffle(begin(passengers), end(passengers));

				npass = 0;
				if (heuristic_choice < Accumulated(0, Weigth_heuristic)) {
					//cout << "*************** I am using the Parallel Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase(peso_itermediate_stop, airplanes, map_airplane, map_airstrip, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, all_passenger, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
					choosen_heuristic = 0;
				}
				else {
					//cout << "*************** I am using the Sequential Heuristic ************" << endl;
					start_solution_route = heuristic_costructive_first_fase_sequential(peso_itermediate_stop, airplanes, map_airplane, map_airstrip, start_day1, end_day1, all_passenger, number_of_aircraft, from_to, location_fuel, from_to_FuelConsumed);
					choosen_heuristic = 1;
				}
				for (Route& r : start_solution_route) npass += (int)r.passengers_in_route.size();
				//cout << "ecco le route" << endl;
				//for (Route r : start_solution_route) r.print();
				cout << "numero di passeggeri " << npass << endl;
				cout << "passengers.size()= " << all_passenger.size() << endl;
				//system("pause");
				//cin.get();
				number_iteration_in_heuristic++;
			} while ((int)all_passenger.size() > npass);
		}

		start_solution = calculate_ObjectiveFunction(peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
		//cout << "il costo del costruttivo e': " << start_solution << endl;
		//system("pause");
		//cout << "ho appena fatto il costruttivo" << endl;
		//stampo_caso_strano(map_airstrip, start_solution_route, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3);
		solutionAll.push_back(start_solution_route);
		//cout << "Costo Starting Greedy Solution: " << start_solution << endl;
		//for (Route r : start_solution_route) r.print();
		//system("pause");

		//*************************************************************per scrittura su .txt per grafico**************************************************************************************************************************************************
		outfile << "soluzione euristica di partenza:;0;" << start_solution << endl;
		//outfile.close();
		//*********************************************************************************************************************************************************************************************************************

		vector<Route> best_solution_route = start_solution_route;
		best_solution = start_solution;
		// SI INIZI IL CILCO CON LA RICERCA LOCALE E SIMULATING ANNEALING
		do {
			NumeroSA++;
			//*************************************heuristic costructive***************************************************************************************
			//cout << "Iteration number: " << iteration << endl;
			//cout << "Temperatura: " << T << endl;
			//cout << "K: " << k << endl;
			//cout << "The cost of the best solution X* is: " << best_solution << endl;
			//cout << "The cost of the current X is: " << start_solution << endl;
			double random = (double)rand() / RAND_MAX;
			double percentage_route_destroy = (MinLevel + (random * (MaxLevel - MinLevel)));
			//cout << "---> Percentuale di distruzione " << percentage_route_destroy << endl;
			int choosen_destroy = -1;
			int choosen_repair = -1;
			vector<Passenger> passenger_removed;
			// REMOVING OF A RANDOM NODE
			vector<Route> Input_destroy = start_solution_route;
			//cout << "***************************DISTRUCTION*******************" << endl;
			double destroy_choice = (double)rand() / RAND_MAX;
			//vector<Route> route_destroy;
			//double starting_range = (Weigth_destroy[0]) / (Sum(Weigth_destroy));
			//cout << " destroy_choice =  " << destroy_choice << endl;
			//for (int i = 0; i < Weigth_destroy.size(); i++) cout << Accumulated(i, Weigth_destroy) << " || ";
			//cout << endl;
			//using namespace std::chrono;
			auto start = chrono::high_resolution_clock::now();
			if (destroy_choice < Accumulated(0, Weigth_destroy)) {
				//cout << "*************** I am using the Worst Destroy ************" << endl;
				cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				int num_passenger_cluster = (int)((all_passenger.size() * 0.12) + (random * ((all_passenger.size() * 0.20) - (all_passenger.size() * 0.12)))); //era 0.24 prima
				//route_destroy = destroy_cluster_aggr2(peso_TW, num_passenger_cluster, passenger_removed, Input_destroy, map_airplane, from_to, passengers, map_id_passenger, peso_itermediate_stop);
				destroy_worst_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, peso_itermediate_stop, percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 0;
			}
			else if (destroy_choice < Accumulated(1, Weigth_destroy)) {
				//cout << "*************** I am using the Cluster Destroy ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				int num_passenger_cluster = (int)((all_passenger.size()*0.14) + (random * ((all_passenger.size()*0.34) - (all_passenger.size()*0.14)))); //era 0.24 prima
				cout << " gendo numero di passeggieri pari a " << num_passenger_cluster << " e ne ho totali " << all_passenger.size() << endl;
				Input_destroy = destroy_cluster_aggr2(legenda, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, num_passenger_cluster, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, all_passenger, map_id_passenger, peso_itermediate_stop, from_to_FuelConsumed);
				choosen_destroy = 1;
			}
			else if (destroy_choice < Accumulated(2, Weigth_destroy)) {
				//cout << "*************** I am using the thanos (ora casual) Destroy ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				Input_destroy = destroy_thanos(legenda, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 2;
			}
			else {
				//cout << "*************** I am using the CASUAL Destroy ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				destroy_casual_rolling(start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, percentage_route_destroy, passenger_removed, Input_destroy, map_airplane, map_airstrip, from_to, from_to_FuelConsumed);
				choosen_destroy = 3;
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
			double time_spent = Time_Spend((double)duration.count());
			//cout << " Tempo Destroy vero: " << duration.count() << " ma corrisponde al valore trasformato in double = " << time_spent << endl;
			//cout << "***************************END DISTRUCTION*******************" << endl;
			//cout << "ho appena fatto la distruzione numero: " << to_string(choosen_destroy) << endl;
			//cout << " Dove 0: Worst // 1: Cluster // 2: Thanos // 3: Casual " << endl;
			//vector<Route> routes, map<string, double>& from_to, map<int, Airplane> map_airplane, map<int, string> legenda_inversa
			int controllo = 0;
			for (Route r : Input_destroy) controllo += r.passengers_in_route.size();
			if ((passenger_removed.size() + controllo) < all_passenger.size()) {
				cout << " ----------------------------------------->>>>>> TI SEI MANGIATO DEI PASSEGGIERI MINCHIA FAI <<<<<----------------------------------------------------------- " << endl;
				cout << " ----------------------------------------->>>>>>> Rimasti nella Route: " << controllo << " <<<<<<<<<<<<------------------------------------------------------------------ " << endl;
				cout << " ----------------------------------------->>>>>>> RIMOSSI: " << passenger_removed.size() << " <<<<<<<<<<<<------------------------------------------------------------------ " << endl;
				cout << " ----------------------------------------->>>>>>> DEVONO ESSERE : " << all_passenger.size() << " <<<<<<<<<<<<------------------------------------------------------------------ " << endl;
				cin.get();
			}
			//stampo_caso_strano(legenda, map_airstrip, Input_destroy, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, Input_destroy);
			//for (Route p : route_destroy) p.print();
			//for(Route p : route_destroy) p.print();
			//cout << "ecco i passeggeri da reinserire: " << endl;
			//for (Passenger p : passenger_removed) p.print();
			//cout << "********************************REBUILT***************************" << endl;
			vector <Route> solution_rebuilt;
			double repair_choice = (double)rand() / RAND_MAX;
			//cout << " repair_choice =  " << repair_choice << endl;
			//for (int i = 0; i < Weigth_repair.size(); i++) cout << Accumulated(i, Weigth_repair) << " || ";
			//cout << endl;


			//PER TEST CON PUNTATORI************************************************************************
			/*cout << "sono prima dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;;
			int pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluione " << pass_soluzione << endl;
			int pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl;
			if (passengers.size() != (pass_dist + passenger_removed.size())) {
				cout << "si creano dei passeggeri" << endl;
				system("pause");
			}
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			*/
			vector<Route> iniziali = Input_destroy;
			auto start_r = chrono::high_resolution_clock::now();
			//cout << "iterazione numero:                              " << NumeroSA << endl;
			//cout << " Temperatura attuale ... " << T << " ... Numero iterazioni senza miglioramento .. " << iteration << endl;
			if (repair_choice < Accumulated(0, Weigth_repair)) {
				//cout << "*************** I am using the Repair One ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				solution_rebuilt = repair_one(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//solution_rebuilt = repair_two_regret(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);

				//for (Route r : solution_rebuilt) r.print();
				//system("pause");


				choosen_repair = 0;
			}
			else if (repair_choice < Accumulated(1, Weigth_repair)) {
				//cout << "*************** I am using the Repair two regret ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				solution_rebuilt = repair_two_regret(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//solution_rebuilt = repair_one(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				choosen_repair = 1;
			}
			else if (repair_choice < Accumulated(2, Weigth_repair)) {
				//cout << "*************** I am using the Repair Forbidden ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				solution_rebuilt = repair_forbidden(peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//solution_rebuilt = repair_two_regret(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);

				choosen_repair = 2;
			}
			else if (repair_choice < Accumulated(3, Weigth_repair)) {
				//cout << "*************** I am using the Repair Forbidden ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				solution_rebuilt = repair_forbidden(peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//solution_rebuilt = repair_two_regret(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);

				choosen_repair = 3;
			}
			else {
				//cout << "*************** I am using the Repair Perturbation ************" << endl;
				//cout << " La Start Solution = " << start_solution << " e la Best: " << best_solution << endl;
				solution_rebuilt = repair_perturbation(peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);
				//solution_rebuilt = repair_two_regret(legenda, peso_itermediate_stop, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, Input_destroy, map_airplane, passenger_removed, map_airstrip, from_to, location_fuel, from_to_FuelConsumed);

				choosen_repair = 4;
			}
			auto stop_r = chrono::high_resolution_clock::now();
			auto duration_r = chrono::duration_cast<chrono::seconds>(stop_r - start_r);
			double time_spent_r = Time_Spend((double)duration_r.count());

			/*cout << "sono dopo dei repair" << endl;
			cout << "passeggeri rimossi " << passenger_removed.size() << endl;
			pass_soluzione = 0;
			for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
			cout << "passenger nella soluzione " << pass_soluzione << endl;
			pass_dist = 0;
			for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
			cout << "passenger nella soluzione distrutta " << pass_dist << endl;
			*/


			//cout << "ho appena fatto i repair" << endl;
			//stampo_caso_strano(legenda, map_airstrip, solution_rebuilt, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, iniziali);
			vector<Route> before_ls = solution_rebuilt;
			//stampo_caso_strano(legenda, map_airstrip, before_ls, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, iniziali);
			if (solution_rebuilt.size() > 0) {

				double initial_cost = calculate_ObjectiveFunction(peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt: " << initial_cost << endl;
				vector<Route> solution_rebuilt_without_LS = solution_rebuilt;
				solution_rebuilt = move(legenda, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed);
				//cout << "ho appena fatto il move" << endl;
				//stampo_caso_strano(legenda, map_airstrip, solution_rebuilt, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, before_ls);
				//calculate_ObjectiveFunction(peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				cout << "il costo totale della soluzione trovata e': " << calculate_ObjectiveFunction(peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;

				//system("pause");
				before_ls = solution_rebuilt;
				//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);
				//cout << "Cost of the solution rebuilt after Move: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				solution_rebuilt = swap(legenda, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, start_day1, start_day2, start_day3, end_day1, end_day2, end_day3, from_to, location_fuel, from_to_FuelConsumed);
				//cout << "ho appena fatto lo swap" << endl;
				//stampo_caso_strano(legenda, map_airstrip, solution_rebuilt, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, before_ls);
				//cout << "Cost of the solution rebuilt after Move + Swap: " << calculate_ObjectiveFunction(peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				//initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)
				//if (initial_cost == calculate_ObjectiveFunction(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed)) {
					//cout << "in questo caso ho fatto l'inter-move" << endl;
					//int pass_per_test = 0;
					//for (Route r : solution_rebuilt) cout << "- " << r.aircraft_code << endl;

					//solution_rebuilt = inter_move(peso_TW, peso_itermediate_stop, solution_rebuilt, map_airplane, map_airstrip, end_day, from_to, location_fuel, from_to_FuelConsumed);
					//cout << "ho appena fatto lo inter move" << endl;
					//stampo_caso_strano(map_airstrip, peso_TW, peso_itermediate_stop, from_to, from_to_FuelConsumed, passengers_per_casostrano, solution_rebuilt, from_to_company, map_airplane, legenda_ritroso);

					/*cout << "sono dentro l'inter-move" << endl;
					cout << "passeggeri rimossi " << passenger_removed.size() << endl;
					pass_soluzione = 0;
					for (Route r : solution_rebuilt) pass_soluzione += r.passengers_in_route.size();
					cout << "passenger nella soluzione " << pass_soluzione << endl;
					pass_dist = 0;
					for (Route r : route_destroy) pass_dist += r.passengers_in_route.size();
					cout << "passenger nella soluzione distrutta " << pass_dist << endl;
					cout << "prima del inter-move erano: " << pass_per_test << endl;
					if(pass_per_test != pass_soluzione) system("pause");
					*/


					//cout << "Cost of the solution rebuilt after Move + Swap: " << calculate_ObjectiveFunction(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//}
				//cout << "passato if intermove" << endl;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//C'E' DA SCOMMENTARE QUESTA RIGA QUA SOTTO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				solution_rebuilt = heuristic_costructive_second_fase(solution_rebuilt, end_day1, end_day2,  end_day3, map_airstrip);
				//cout << "ho appena fatto la traslazione" << endl;
				//stampo_caso_strano(legenda, map_airstrip, solution_rebuilt, from_to, map_airplane, start_day1, end_day1, start_day2, end_day2, start_day3, end_day3, before_ls);
				double cost_objectiveFunction_second_fase_after_rebuilt = calculate_ObjectiveFunction(peso_itermediate_stop, solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);
				//cout << "Cost of the solution rebuilt with Move + Swap + Second phase: " << cost_objectiveFunction_second_fase_after_rebuilt << endl;
				//cout << "la best solution fino a questo punto e': " << best_solution << endl;
				//*************************************************************per scrittua su .txt per grafico**************************************************************************************************************************************************
				if (NumeroSA == 100 || NumeroSA == 200 || NumeroSA == 300 || NumeroSA == 400 || NumeroSA == 500 || NumeroSA == 600 || NumeroSA == 700 || NumeroSA == 800 || NumeroSA == 900 || NumeroSA == 1000 ||
					NumeroSA == 1100 || NumeroSA == 1200 || NumeroSA == 1300 || NumeroSA == 1400 || NumeroSA == 1500 || NumeroSA == 1600 || NumeroSA == 1700 || NumeroSA == 1800 || NumeroSA == 1900 || NumeroSA == 2000 ||
					NumeroSA == 21000 || NumeroSA == 22000 || NumeroSA == 23000 || NumeroSA == 24000 || NumeroSA == 25000 || NumeroSA == 26000 || NumeroSA == 27000 || NumeroSA == 28000 || NumeroSA == 29000 || NumeroSA == 30000 ||
					NumeroSA == 31000 || NumeroSA == 32000 || NumeroSA == 33000 || NumeroSA == 34000 || NumeroSA == 35000 || NumeroSA == 36000 || NumeroSA == 37000 || NumeroSA == 38000 || NumeroSA == 39000 || NumeroSA == 40000 ||
					NumeroSA == 41000 || NumeroSA == 42000 || NumeroSA == 43000 || NumeroSA == 44000 || NumeroSA == 45000 || NumeroSA == 46000 || NumeroSA == 47000 || NumeroSA == 48000 || NumeroSA == 49000 || NumeroSA == 50000 ||
					NumeroSA == 51000 || NumeroSA == 52000 || NumeroSA == 53000 || NumeroSA == 54000 || NumeroSA == 55000 || NumeroSA == 56000 || NumeroSA == 57000 || NumeroSA == 58000 || NumeroSA == 59000 || NumeroSA == 60000) {
					//ofstream outfile1;
					//outfile1.open("grafico.txt");
					tempo_finale = difftime(time(NULL), time_now);
					outfile << "NumeroSA:;" << NumeroSA << ";" << start_solution << ";" << best_solution << ";tempo:;" << to_string(tempo_finale) << endl;
					//outfile1.close();
				}
				//*********************************************************************************************************************************************************************************************************************





				//if (cost_objectiveFunction_second_fase_after_rebuilt < initial_cost) solutionAll.push_back(solution_rebuilt_without_LS);
				//cout << "costo routing: " << costo_senza_time_windows(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				//cout << "costo time windows: " << costo_time_windows(solution_rebuilt, map_airstrip, map_airplane, from_to, from_to_FuelConsumed) << endl;
				if (cost_objectiveFunction_second_fase_after_rebuilt < start_solution) {
					//cout << "Ottimo aggiorno la soluzione da cui continuare a fare perturbazioni " << endl;
					start_solution_route.clear();
					//start_solution_route.shrink_to_fit();
					//cout << " Ho cancellato " << endl;
					//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					for (Route& so : solution_rebuilt) start_solution_route.push_back(so);
					//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
					double award = Ob_Funct_Improvement(cost_objectiveFunction_second_fase_after_rebuilt, start_solution);
					//cout << " Con un miglioramento di " << start_solution - cost_objectiveFunction_second_fase_after_rebuilt << " si ha un award di " << award << endl;
					//cout << " ed un fattore tempo di Destroy di " << time_spent << endl;
					//cout << " ed un fattore tempo di Repair di " << time_spent_r << endl;
					start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
					iteration = 0;
					//cout << " Aggiungo al destroy ad al Repair 1 + award =  " << award << endl;
					Weigth_destroy[choosen_destroy] += (1 + award - time_spent);
					Weigth_repair[choosen_repair] += (1 + award - time_spent_r);
					if (cost_objectiveFunction_second_fase_after_rebuilt < best_solution) {
						//cout << "Aggiorno migliore soluzione " << endl;
						best_solution_route.clear();
						start_solution_route.clear();
						//best_solution_route.shrink_to_fit();
						for (Route& s : solution_rebuilt) {
							best_solution_route.push_back(s);
							start_solution_route.push_back(s);
						}
						best_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						best_iteration = iteration;
						start_route_bs = (int)solutionAll.size();
						//size_route_bs = (int)solution_rebuilt.size();
						//cout << " Ho aggiornato la migliore soluzione le route adesso si trovano nella posizione: " << start_route_bs << " con size: " << size_route_bs << endl;
						//system("pause");
					}
				}
				else {

					r = double(rand()) / (double(RAND_MAX) + 1.0);
					//cout << " Ho generato numero random " << r << endl;
					//cout << "-( " << cost_objectiveFunction_second_fase_after_rebuilt << " - " << start_solution << ") / " << T;
					double espo = -(cost_objectiveFunction_second_fase_after_rebuilt - start_solution) / T;
					//cout << " Adesso ho generato espo " << espo << endl;
					//cout << " Se non lo � devo incrementare iterazione se lo � aggiorno valori X con X'  " << endl;
					if (r <= exp(espo) && exp(espo) != 1) {
						//cout << "Confrona i costi sono entrato perche: " << r << " < " << exp(espo) << endl;
						start_solution_route.clear();
						//start_solution_route.shrink_to_fit();
						//cout << " Ho cancellato " << endl;
						//cout << " Adesso ho azzerato start_solution_route stampo la size: " << start_solution_route.size() << endl;
						for (Route& s : solution_rebuilt) start_solution_route.push_back(s);
						//cout << " Adesso ho aggiornato start_solution_route stampo la size: " << start_solution_route.size() << endl;
						start_solution = cost_objectiveFunction_second_fase_after_rebuilt;
						iteration = 0;
						Weigth_destroy[choosen_destroy] ++;
						Weigth_repair[choosen_repair] ++;
					}
					else {
						iteration++;
					}
				}
				solutionAll.push_back(solution_rebuilt);
				cout << "Numero iterazioni ---> " << NumeroSA << endl;
				cout << "iterazioni fallite --> " << iterazioni_fallite << endl;
			}
			else {
				//cout << " ************ ITERAZIONE FALLITA ************************* " << endl;
				iterazioni_fallite++;
				//system("pause");
			}
			k++;
			//cout << " Valore attuale di K = " << k << " aggiorno temperatura dopo: " << DeltaK << endl;
			if (k >= DeltaK) {
				T = (int)(alpha * T);
				//cout << " Aggiorno temperatura A: " << T << endl;
				k = 0;
			}
			//cout << " A T < " << Tmin << " devi fermarti T= " << T << " or " << iteration << " > " << iteration_max << " Ciclo Grande fuori iterazione ------> " << Iter_FirstDo << endl;
		} while ((iteration <= iteration_max) && (T > Tmin));
		// FINITO IL CICLO DELLA PERTURBAZIONE STAMPA RISULTATI
		tempo_finale = difftime(time(NULL), time_now);
		start_solution = best_solution;
		cout << " Temmperatura auttuale: " << T << " Temperatura Minima: " << Tmin << " iterazioni senza migliorare: " << iteration << " Numero massimo: " << iteration_max << endl;
		Iter_FirstDo++;

	} while ((Iter_FirstDo <= iterMAX_FirstDo));

	outfile << "best solution: " << start_solution << " tempo: " << to_string(tempo_finale) << endl;
	cout << " Alla iterazione numero: " << best_iteration << endl;
	
	//*********************************************************************************************************************************************************************
	cout << " ********** Final Weight ************ " << endl;
	cout << " Cluster || Worst || 2-Regret || Thanos " << endl;
	for (double i : Weigth_destroy) cout << i << " || ";
	cout << endl;
	cout << " One || SP || Regret || Forbidden ||Perturbation " << endl;
	for (double i : Weigth_repair) cout << i << " || ";
	cout << endl;
	tempo_finale = difftime(time(NULL), time_now);
	cout << "Time at the end: " << tempo_finale << endl;
	cout << "Time model: " << time_spent_model << endl;
	cout << "Numero SA= " << NumeroSA << endl;
	cout << "Iterazioni Fallite= " << iterazioni_fallite << endl;
	cout << "number of iteration in the heuristic: " << number_iteration_in_heuristic << endl;
	outfile.close(); // per il grafico++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//ECCO LE STAMPE DEL RISULTATO
	double stampa_compagnia = calculationCostCompany_three_days_disaggregato(peso_TW, from_to_fuel_consumed_string, peso_transhipment, peso_itermediate_stop, argv[2], argv[6], argv[10], argv[3], argv[7], argv[11], airstrips, airplanes, passengers_day1, passengers_day2, passengers_day3, from_to_company);
	double stampa_nostra = calculate_ObjectiveFunction_disaggregato(peso_itermediate_stop, start_solution_route, map_airstrip, map_airplane, from_to, from_to_FuelConsumed);







	//cout << "c'è nel main stampo caso strano" << endl;
	cout << "system del main" << endl;
	system("pause");
	return 0;
}

