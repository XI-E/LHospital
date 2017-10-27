#include <iostream.h>
#include <fstream.h>

typedef char str[100];

struct procedure{
	str name;
	float cost;
};

void main(){
	ofstream proc ("proc.dat" , ios::out | ios::binary | ios::app);
	procedure a;
	cin.ignore(1000, '\n');
	cout << "\nName:";
	cin.getline(a.name, 100, '\n');
	cout << "\nCost:";
	cin >> a.cost;
	cout << endl << "Procedure : " << a.name << "  $" << a.cost << ".\nEnter next procedure:";
	proc.write( (char*) &a , sizeof(a) );
}