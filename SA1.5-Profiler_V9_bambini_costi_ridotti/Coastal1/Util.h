#pragma once


#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "Airplane.h"
#include "Route.h"
using std::vector;
using std::string;
using Double3DVector = vector<vector<vector<double>>>;
using Int3DVector = vector<vector<vector<int>>>;
using Solution = vector<Route>;
using Fleet = vector<Airplane>; 

struct Penalty_Weights
{
	double stop_weight;
	double time_window_weight;
	double transhipment_weight;
};


struct Input_maps
{
	std::map<int, Airplane> map_airplane;
	map<int, Airstrip> map_airstrip;
};


//function of split



template<typename type>
struct MyCOMP {
	bool operator() (const type& first, const type& second) const {
		return first > second;
	}

};


template<typename type>
struct MyCOMP1 {
	bool operator() (const type& first, const type& second) const {
		return first < second;
	}

};


inline double Sum(std::vector<double> Weight) {
	double sum = 0;
	for (double i : Weight) sum += i;
	return sum;
}

inline double Accumulated(int j, vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) sum += i;
	for (int i = 0; i <= j; i++) accum += ((Weight[i]) / sum);
	return accum;
}




inline std::vector<string> split(std::string stringa, char separatore) {
	vector<std::string> words;
	std::stringstream ss(stringa);
	std::string individual_string;
	while (std::getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
}


#endif