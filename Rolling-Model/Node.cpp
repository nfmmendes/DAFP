#include "Node.h"
#include <iostream>
using namespace std;


Node::Node()
{
	cod = -1;
	cod_route_di_appartenenza = -1;
	position_in_route = -1;
	cost = 0.0;
}


Node::Node(int cod, int cod_route_di_appartenenza, int position_in_route)
{
	this->cod = cod;
	this->cod_route_di_appartenenza = cod_route_di_appartenenza;
	this->position_in_route = position_in_route;
}

void Node::print() {

	cout << "Nodo con codice: " << cod << endl;
	cout << "Codice route di appartenenza: " << cod_route_di_appartenenza << endl;
	cout << "Posizione nella route: " << position_in_route << endl;
	cout << "Costo: " << cost << endl;

}


Node::~Node()
{
}
