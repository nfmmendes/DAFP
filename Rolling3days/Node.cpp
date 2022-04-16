#include "Node.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

Node::Node()
{
	from = "";
	to = "";
	refueling_from = false;
	refueling_to = false;
	time_from = 0.0;
	time_to = 0.0;
	traveling_time = 0.0;
}

Node::Node(string from, string to, bool refueling_from, bool refueling_to, double traveling_time)
{
	this->from = from;
	this->to = to;
	this->refueling_from = refueling_from;
	this->refueling_to = refueling_to;
	time_from = 0.0;
	time_to = 0.0;
	this->traveling_time = traveling_time;


}

Node::~Node()
{
}




void Node::print() {
	cout << "from: " << from << " to: " << to << " traveling time: " << traveling_time << endl;
}