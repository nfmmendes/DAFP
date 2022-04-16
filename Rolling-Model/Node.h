#ifndef Node_h
#define Node_h
using namespace std;
class Node
{
public:
	Node();
	~Node();

	int cod;
	int cod_route_di_appartenenza;
	int position_in_route;
	double cost;
	Node(int cod, int cod_route_di_appartenenza, int position_in_route);
	void print();


};

#endif // Route_h