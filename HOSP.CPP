#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include "base.hpp"
#include "patient.hpp"

//enum access_level {OTHERS, DOCTOR, NURSE, RECEPTIONIST};

class hospital{
	public:
		static float get_bal();
		static transaction deduct_money(float, char*, Date, Time);
		static transaction add_money(float, char*, Date, Time);
		static transaction* get_transaction();
		static void read_balance();
		
		static void pay_salary(employee, Date, Time);
		static void pay_salary(doctor, Date, Time);
		static void pay_salary(nurse, Date, Time);
		static void pay_salary(receptionist, Date, Time);
		
//		static void pay_all_salaries();
	private:
		hospital();
		static double balance;

};

class employee : public person{
	public:
	employee(str, int, Date, address, phone, unsigned long, Time, Time, int, str = "", str = "");  //for all those with user accounts(doctors, nurses, receptionists), last 2 arguments are to be provided as well
	unsigned long get_salary();
	Time get_shift(int);
	userid account;
	protected:
		unsigned id;
		unsigned long salary;
		transaction last_10_transactions[10];
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
		static void purchase(int, int);
};


//////////////////////////////////////////////
///////////////////////////////////////////////// Function definitions for class hospital

float hospital::get_bal(){
	return balance;
}

void hospital::deduct_money(float amt, char* reason, Date d, Time t){
	hospital::balance -= amt;

	ofstream hosp_finances ("transactions.dat", ios::out | ios::binary | ios::app);

	transaction temp = transaction( (-1)*amt, reason, d, t);

	hosp_finances.write( (char*) (&temp) , sizeof(transaction) );

	hosp_finances.close();
	
	return temp;
}

void hospital::add_money(float amt, char* reason, Date d, Time t){
	hospital::balance += amt;

	ofstream hosp_finances ("transactions.dat", ios::out | ios::binary | ios::app);

	transaction temp = transaction( (-1)*amt, reason, d, t);

	hosp_finances.write( (char*) (&temp) , sizeof(transaction) );

	hosp_finances.close();
	
	return temp;
}

static void pay_salary(employee x, Date d1, Time t1)
{
    unsigned long inp1 = x.get_salary();
    char inp2[100] = "Salary paid to ";
    strcat(inp2, x.get_name());
    transaction t = hospital::deduct_money(inp1, inp2, d1, t1);
    transaction null;
    for(int i = 0; i < 10; ++i)
    {
        if(x.last_10_transactions[i] == null)
        {
            x.last_10_transactions[i] = t;
            return;
        }
    }
    for(i = 0; i < 10; ++i)
    {
        x.last_10_transactions[i] = x.last_10_transactions[i + 1];
    }
    x.last_10_transactions[i] = t;
}

static void pay_salary(doctor x, Date d1, Time t1)
{
    unsigned long inp1 = x.get_salary();
    char inp2[100] = "Salary paid to ";
    strcat(inp2, x.get_name());
    transaction t = hospital::deduct_money(inp1, inp2, d1, t1);
    transaction null;
    for(int i = 0; i < 10; ++i)
    {
        if(x.last_10_transactions[i] == null)
        {
            x.last_10_transactions[i] = t;
            return;
        }
    }
    for(i = 0; i < 10; ++i)
    {
        x.last_10_transactions[i] = x.last_10_transactions[i + 1];
    }
    x.last_10_transactions[i] = t;
}

static void pay_salary(nurse x, Date d1, Time t1)
{
    unsigned long inp1 = x.get_salary();
    char inp2[100] = "Salary paid to ";
    strcat(inp2, x.get_name());
    transaction t = hospital::deduct_money(inp1, inp2, d1, t1);
    transaction null;
    for(int i = 0; i < 10; ++i)
    {
        if(x.last_10_transactions[i] == null)
        {
            x.last_10_transactions[i] = t;
            return;
        }
    }
    for(i = 0; i < 10; ++i)
    {
        x.last_10_transactions[i] = x.last_10_transactions[i + 1];
    }
    x.last_10_transactions[i] = t;
}

static void pay_salary(receptionist x, Date d1, Time t1)
{
    unsigned long inp1 = x.get_salary();
    char inp2[100] = "Salary paid to ";
    strcat(inp2, x.get_name());
    transaction t = hospital::deduct_money(inp1, inp2, d1, t1);
    transaction null;
    for(int i = 0; i < 10; ++i)
    {
        if(x.last_10_transactions[i] == null)
        {
            x.last_10_transactions[i] = t;
            return;
        }
    }
    for(i = 0; i < 10; ++i)
    {
        x.last_10_transactions[i] = x.last_10_transactions[i + 1];
    }
    x.last_10_transactions[i] = t;
}

transaction* hospital::get_transaction(){
	transaction a[10];

	ifstream hosp_finances ("transactions.dat", ios::in | ios::binary);

	hosp_finances.seekg( (-1) * sizeof(transaction) , hosp_finances.end );

	for(int i = 0; i < 10; i++){
		hosp_finances.read( (char *) &a[i] , sizeof(transaction) );
		hosp_finances.seekg( hosp_finances.tellg() - ( 2 * sizeof(transaction) ) );
	}

	return a;
}

//////////////////////////////////
/// Function definitions for class employee

employee::employee(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : person(inp1, inp2, inp3, inp4, inp5), account(inp10, inp11)
{
	salary = inp6;
	shift_start = inp7;
	shift_end = inp8;
	id = inp9;
	
	transaction null;
	for(int i = 0; i < 10; ++i)
	    last_10_transactions[i] = null;
}

unsigned long employee::get_salary(){
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

doctor::doctor(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, int inp10, int inp11, str inp12, str inp13) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp12, inp13)
{
	speciality[0] = inp10;
	speciality[1] = inp11;

	for(int i = 0; i < 10; i++){
		patient[i] = 0;
	}
}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp10, inp11)
{
	for(int i = 0; i < 5; i++){
		patient[i] = 0;
	}
}

receptionist::receptionist(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp10, inp11)
{}

/////////////////////////////////////////
//// Pharmacy function definitions

/*
void pharmacy::sale(int pat_id, int code, int quantity){
	patient a = get_pat(pat_id);
	for(int i = 0; i < 50; i++){
		if(a.get_med(i,0) == code){
			quantity += a.get_med(i,1);
			a.set_med(i, code, quantity);
		}
		if(i>0){
			if(a.get_med(i, 0) == 0 &&
				a.get_med(i - 1, 0) != 0){
					a.set_med(i, code, quantity);
				}
		}
	}
}

void pharmacy::purchase(int code, int quantity){
	medicine &a = get_med_by_code(code);
	str b = strcat( strcat("Medicine (", a.name) , ") Purchase" );
	hospital::deduct_money( (a.price * quantity) , b );
} */

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
double hospital::balance = 1000.0;

int main(){
	return 0;
}
