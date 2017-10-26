#include <iostream.h>
#include <string.h>
#include "base.hpp"
#include "patient.hpp"

//enum access_level {OTHERS, DOCTOR, NURSE, RECEPTIONIST};

class employee : public person{
	public:
	employee(str, int, Date, address, phone, unsigned long, Time, Time, int, str = "", str = "");  //for all those with user accounts(doctors, nurses, receptionists), last 2 arguments are to be provided as well
	unsigned long getsal();
	Time getshift(int);
 	userid account;
	protected:
		unsigned id;
		unsigned long salary;
		Time shift_start;
		Time shift_end;
};

class doctor : public employee{
	public:
	doctor(str, int, Date, address, phone, unsigned long, Time, Time, int, int, int, str, str);

	private:
		int speciality[2];				// Doctor's specialization
		int patient[10];				// Patients currently under care, can take only 10 at once
};

class nurse : public employee{
	public:
	nurse(str, int, Date, address, phone, unsigned long, Time, Time, int, str, str);

	private:
		int patient[5];
};

class receptionist : public employee
{
    public:
        receptionist(str, int, Date, address, phone, unsigned long, Time, Time, int, str, str);
//		doctor assign_doctor(patient);
};

class pharmacy{
	public:
		static void sale(int, int, int);
};


//////////////////////////////////////////////
//////////////////////////////////////////////
/// Function definitions for class employee

employee::employee(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : person(inp1, inp2, inp3, inp4, inp5), account(inp10, inp11)
{
	salary = inp6;
	shift_start = inp7;
	shift_end = inp8;
	id = inp9;
}

unsigned long employee::get_sal(){
	return salary;
}

Time employee::get_shift(int inp){
	switch(inp){
		case START:
			return shift_start;
		case END:
			return shift_end;
		default:
			return Time(0,0,0);
	}
}

/////////////////////////////////////////
//// Doctor, Nurse and Receptionist class member defs

doctor::doctor(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, int inp10, int inp11, str inp12, str inp13) : employee(inp1, inp2, inp3, inp4, inp5, inp6, in7, inp8, inp9, inp12, inp13)
{
    speciality[0] = inp10;
    speciality[1] = inp11;
    
    for(int i = 0; i < 10; i++){
        patient[i] = 0;
    }
}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, in7, inp8, inp9, inp10, inp11)
{
    for(int i = 0; i < 5; i++){
        patient[i] = 0;
    }
}

receptionist::receptionist(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, in7, inp8, inp9, inp10, inp11)
{}

/////////////////////////////////////////
//// Pharmacy function definitions

void pharmacy::sale(int pat_id, int code, int quantity){
	patient a = get_pat(pat_id);
	for(int i = 1; i < 50; i++){
		if(a.get_med(i, 0) == 0 && a.get_med(i - 1, 0) != 0){
				a.set_med(i, code, quantity);
	    }
	}
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

int main(){
	return 0;
}