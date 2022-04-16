#include <string>
#include <vector>
using namespace std;



class Node
{
public:

	string from;
	string to;
	bool refueling_from;
	bool refueling_to;
	double time_from;
	double time_to;
	double traveling_time;


	Node();
	Node(string from,string to,bool refueling_from,bool refueling_to, double traveling_time);
	~Node();
	void print();
};

