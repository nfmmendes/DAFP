#ifndef UTIL_H
#define UTIL_H


//function of split
#include <string>
#include <vector>
#include <sstream>
using std::vector;
using std::string; 


using double3DVector = vector<vector<vector<double>>>;
using double2DVector = vector<vector<double>>;
using int3DVector = vector<vector<vector<int>>>;

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
	for (double i : Weight) 
		sum += i;
	return sum;
}

inline double Accumulated(int j, vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) 
		sum += i;
	for (int i = 0; i <= j; i++) 
		accum += ((Weight[i]) / sum);
	return accum;
}




inline  std::vector<string> split(std::string stringa, char separatore) {
	vector<std::string> words;
	std::stringstream ss(stringa);
	std::string individual_string;
	while (std::getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
}


#endif