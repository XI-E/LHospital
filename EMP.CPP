#include "hosp.hpp"
#include "iface.hpp"
#include "emp.hpp"
#include <fstream.h>

//////////////////////////////////
/// Function definitions for class employee

int employee::generate_id()
{
	mkdir("employee");
	unsigned long max_id;
	ifstream fin;
	fin.open("employee/max_id.dat", ios::binary);
	if(!fin)
		max_id = 0;
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
//			cerr<<"ERROR WHILE READING FROM FILE!!! ";
			id = 0;
			return 0;
		}
	}
	fin.close();
	++max_id;
	id = max_id;
	ofstream fout;
	fout.open("employee/max_id.dat", ios::binary);
	fout.write((char *) &max_id, sizeof(unsigned long));
	if(fout.fail())
	{
//		cerr<<"ERROR WHILE WRITING TO FILE!!! ";
		return 0;
	}
	else
		return 1;
}

int employee::generate_id_status = 1;

employee::employee(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp9, str inp10) : person(inp1, inp2, inp3, inp4, inp5), account(inp9, inp10)
{
	if(!generate_id_status)
	{
//		cerr<<"ID cannot be generated for this employee..";
	}
	else
	{
		employee::generate_id_status = generate_id();
		id_to_emp i1(id, OTHERS);
		salary = inp6;
		shift_start = inp7;
		shift_end = inp8;
	}
}

employee::employee() : person()
{
	id = 0;
}

int employee::get_age()
{
	////////////////Updating age to present age///////////
	set_dob(dob);		//This function is used here to invoke calc_age() in it only(because calc_age is directly not accessible)
	employee temp;
	if(hospital::get_employee_by_id(id, &temp))		//if employee's file exists on disk
		hospital::write_employee(this);				//overwrite that file
	return age;
}

unsigned long employee::get_salary(){
	return salary;
}

void employee::set_salary(unsigned long inp)
{
	salary = inp;
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

void employee::set_shift(int inp1, Time inp2)
{
	switch (inp1)
	{
		case START:
			shift_start = inp2;
			return;
		case END:
			shift_end = inp2;
			return;
		default:
			return;
	}
}

unsigned long employee::get_id()
{
	return id;
}

transaction * employee::get_last_10_transactions()
{
	transaction t[10];
	str temp;
	sprintf(temp, "employee/%d/trans.dat", id);
	ifstream fin ( temp ,ios::binary | ios::ate );
	fin.seekg(( (-10) * sizeof(transaction) ), ios::end);
	for(int i = 0; i < 10; ++i)
	{
		fin.read((char *) &t[i], sizeof(transaction));
	}
	return t;
}

int employee::get_generate_id_status()
{
	return generate_id_status;
}

/////////////////////////////////////////
//// Doctor, Nurse and Receptionist class member defs

doctor::doctor(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp10, int inp11, str inp12, str inp13) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp12, inp13)
{
	id_to_emp i1(get_id(), DOCTOR);
	speciality[0] = inp10;
	speciality[1] = inp11;

	for(int i = 0; i < 10; i++){
		patients[i] = 0;
	}
}

doctor::doctor() : employee()
{
	speciality[0] = speciality[1] = GEN + 1;	//storing an invalid value in speciality
	for(int i = 0; i < 10; ++i)
	{
		patients[i] = 0;
	}
}

int * doctor::get_speciality()
{
	return speciality;
}

long * doctor::get_patients()
{
	return patients;
}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), NURSE);
	for(int i = 0; i < 5; i++){
		patients[i] = 0;
	}
}

nurse::nurse() : employee()
{
	for(int i = 0; i < 5; ++i)
	{
		patients[i] = 0;
	}
}

long * nurse::get_patients()
{
	return patients;
}

receptionist::receptionist(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), RECEPTIONIST);
}

receptionist::receptionist() : employee()
{}


//////////////////////////////////
/// Function definitions for class id_to_emp

id_to_emp::id_to_emp(unsigned long inp1, int inp2)
{
	id = inp1;
	if(!id)
	{
		employee_type = INVALID;
	}
	else
	{
		employee_type = inp2;
	}
	mkdir("employee");
	ofstream fout;
	fout.open("employee/id_list.dat", ios::binary | ios::ate);
	if(!fout)
	{
//		cerr<<"File id_list.dat couldn't be opened...";
	}
	else
	{
		fout.seekp(id * sizeof(id_to_emp), ios::beg);
		fout.write((char *) this, sizeof(id_to_emp));
		if(fout.fail())
		{
//			cerr<<"Error while writing to id_list.dat";
		}
	}
}

id_to_emp::id_to_emp()
{
	id = employee_type = 0;
}

int id_to_emp::convert(unsigned long ID)
{
	id_to_emp a;
	ifstream fin;
	fin.open("employee/id_list.dat", ios::binary);
	if(!fin)
	{
		interface::log_this("id_to_emp::convert() : File id_list.dat not found!!");
		return INVALID;
	}
	fin.seekg( (ID * sizeof(id_to_emp)) );
	fin.read((char *) &a, sizeof(id_to_emp));
	if(fin.fail())
	{
		interface::log_this("id_to_emp::convert() : Error while reading from file id_list.dat");
		return INVALID;
	}
	fin.close();
	if(a.id != ID)
	{
		interface::log_this("id_to_emp::convert() : (For dev only)Error in the code... Recheck it!!");
		return INVALID;
	}
	return a.employee_type;
}