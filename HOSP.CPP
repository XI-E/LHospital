#include <iostream.h>
#include <string.h>
#include "base.hpp"

class employee : public person{
	public:
	employee(str, int, date, adr, phn,
				unsigned long, time, time, int);

	unsigned long getsal();
	time getshift(int);

	protected:
		unsigned id;
		unsigned long salary;
		time shift_start;
		time shift_end;
};

class patient : public person{
	public:
		patient(str, int, date, adr, phn,
					date, disease);

		int getadm_date(int);
		disease getdis();
		unsigned long getbill_amt();

	private:
		date admission_date;
		disease dis;
		unsigned long bill_amt;
};

class doctor : public employee{
	public:

	private:
		int spl[2];						// Doctor's specialization
		int patient[10];				// Patients currently under care
										// Can take only 10 at once

};

class nurse : public employee{
	public:

	private:
		int patient[5];
};

/////////////////////////////////////////////
/// Function definitions for class person

person::person(str inp1, int inp2, date inp3, adr inp4, phn inp5){
	strcpy(name, inp1);
	sex = inp2;
	dob = inp3;
	address = inp4;
	strcpy(phone, inp5);
}

char* person::getname(){
	return name;
}

int person::getage(){
	return age;
}

int person::getsex(){
	return sex;
}

int person::getdob(int inp){
	switch(inp){
		case DAY:
			return dob.day;
		case MONTH:
			return dob.month;
		case YEAR:
			return dob.year;
		default:
			return 0;
	}
}

char* person::getadr(int inp){
	switch(inp){
		case HOUSE_NO:
			return address.house_no;
		case STREET:
			return address.street;
		case CITY:
			return address.city;
		case DISTRICT:
			return address.district;
		case STATE:
			return address.state;
		default:
			return 0;
	}
}

char* person::getphn(){
	return phone;
}

//////////////////////////////////////////////
//////////////////////////////////////////////
/// Function definitions for class employee

employee::employee(str inp1, int inp2, date inp3,
					 adr inp4, phn inp5,
					 unsigned long inp6, time inp7,
					 time inp8, int inp9) :
					 person(inp1, inp2, inp3,
					 inp4, inp5)
{
	salary = inp6;
	shift_start = inp7;
	shift_end = inp8;
	id = inp9;
}

unsigned long employee::getsal(){
	return salary;
}

time employee::getshift(int inp){
	switch(inp){
		case START:
			return shift_start;
		case END:
			return shift_end;
		default:
			return time(0,0,0);
	}
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

int main(){
	return 0;
}
