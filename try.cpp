#include <iostream.h>
#include <conio.h>
#include <fstream.h>

void main(){
	clrscr();
    ofstream id ("patient/max_id.dat", ios::out | ios::binary);
    long a = 0;
    id.write( (char*) &a, sizeof(a));
	getch();
}