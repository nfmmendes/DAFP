#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>

template<typename type>
struct MyCOMP {
	bool operator() (
		const type& first, const type& second) const {
		return first > second;
	}

};

template<typename type>
struct MyCOMP1 {
	bool operator() (
		const type& first, const type& second) const {
		return first < second;
	}

};


double Sum(std::vector<double> Weight) {
	double sum = 0;
	for (double i : Weight) sum += i;
	return sum;
}

double Accumulated(int j, std::vector<double> Weight) {
	double accum = 0.0;
	double sum = 0.0;
	for (double i : Weight) sum += i;
	for (int i = 0; i <= j; i++) accum += ((Weight[i]) / sum);
	return accum;
}

double Ob_Funct_Improvement(double newsolution, double oldsolution) {
	double rate_max_improvement = 10000;
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

//function of split
std::vector<std::string> split(std::string stringa, char separatore) {
	std::vector<std::string> words;
	std::stringstream ss(stringa);
	std::string individual_string;
	while (getline(ss, individual_string, separatore)) {
		words.push_back(individual_string);
	}

	return words;
}


#endif
