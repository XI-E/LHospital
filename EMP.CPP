#include "hosp.hpp"
#include "iface.hpp"
#include "emp.hpp"
#include "base.hpp"
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
	{
		interface::log_this("employee::generate_id() : File max_id.dat not found or error while loading file\nmax_id will be set to zero");
		max_id = 0;
	}
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
			interface::log_this("employee::generate_id() : Error while reading from file max_id.dat (fin.fail())\nFunction aborted");
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
		interface::log_this("employee::generate_id() : Error while writing to file max_id.dat (fout.fail())\nFunction aborted");
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
		interface::error("ID cannot be generated for this employee. Check log");
		interface::log_this("employee::employee() : ID generation using generate_id() unsuccessful as generate_id_status is set to zero.\nThis is because some error was encountered during the last ID generation");
	}
	else
	{
		employee::generate_id_status = generate_id();
		id_to_emp i1(id, OTHERS);
		if(!i1.status)
		{
			interface::error("ID not generated properly for this employee. Check log");
			interface::log_this("employee::employee() : i1.status was set to zero, i.e id_list.dat doesn't have a record of the employee's id");
		}
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
	void * temp = malloc( sizeof(doctor) );
	if(hospital::get_employee_by_id(id, temp))			//if employee's file exists on disk
	{
		hospital::write_employee( this );				//overwrite that file
	}
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

transaction * employee::get_last_5_transactions()
{
	transaction * t = (transaction *)malloc(5 * sizeof(transaction));
	for(int i = 0; i < 5; ++i)
	{
		t[i] = transaction();
	}
	str temp;
	switch( id_to_emp::convert(id) )
	{
		case INVALID:
		{
			char log_msg[300];
			sprintf(log_msg, "employee::get_last_5_transactions() : The object has invalid id (%lu)\nFunction aborted", id);
			interface::log_this(log_msg);
			return NULL;
		}
		case DOCTOR:
		{
			sprintf(temp, "employee/doctor/%lu/trans.dat", id);
			break;
		}
		case NURSE:
		{
			sprintf(temp, "employee/nurse/%lu/trans.dat", id);
			break;
		}
		case RECEPTIONIST:
		{
			sprintf(temp, "employee/receptionist/%lu/trans.dat", id);
			break;
		}
		case OTHERS:
		{
			sprintf(temp, "employee/%lu/trans.dat", id);
			break;
		}
	}
	ifstream fin ( temp ,ios::binary | ios::in | ios::nocreate | ios::ate);
	if(!fin)
	{
		char log_msg[300];
		sprintf(log_msg, "employee::get_last_5_transactions() : Failed to open file trans.dat for id %lu\nFunction aborted", id);
		interface::log_this(log_msg);
		return NULL;
	}
	int max_i, size_of_file = fin.tellg();
	if( size_of_file >= ( 5 * sizeof(transaction) ) )
	{
		const int a = (-5) * sizeof(transaction);
		fin.seekg(a, ios::end);
		max_i = 5;
	}
	else
	{
		fin.seekg(0, ios::beg);
		max_i = (int)( size_of_file / sizeof(transaction) );
	}
	for(i = 0; i < max_i && !fin.eof(); ++i)
	{
		fin.read((char *) (t+i), sizeof(transaction));
		if(fin.fail())
		{
			char log_msg[300];
			sprintf(log_msg, "employee::get_last_5_transactions() : Failed to read file trans.dat for id %lu(loop failed at i = %i)\nFunction aborted", id, i);
			interface::log_this(log_msg);
			return NULL;
		}
	}
	fin.close();
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
	if(!i1.status)
	{
		interface::error("ID not generated properly for this employee. Check log");
		interface::log_this("doctor::doctor() : i1.status was set to zero, i.e id_list.dat doesn't have a record of the employee's id");
	}
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

void doctor::set_speciality(int inp[2])
{
	speciality[0] = inp[0];
	speciality[1] = inp[1];
}

void doctor::set_patients(long inp[10])
{
	for(int i = 0; i < 10; ++i)
	{
		patients[i] = inp[i];
	}
}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), NURSE);
	if(!i1.status)
	{
		interface::error("ID not generated properly for this employee. Check log");
		interface::log_this("nurse::nurse() : i1.status was set to zero, i.e id_list.dat doesn't have a record of the employee's id");
	}
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

void nurse::set_patients(long inp[5])
{
	for(int i = 0; i < 5; ++i)
	{
		patients[i] = inp[i];
	}
}

receptionist::receptionist(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), RECEPTIONIST);
	if(!i1.status)
	{
		interface::error("ID not generated properly for this employee. Check log");
		interface::log_this("receptionist::receptionist() : i1.status was set to zero, i.e id_list.dat doesn't have a record of the employee's id");
	}
}

receptionist::receptionist() : employee()
{}


//////////////////////////////////
/// Function definitions for class id_to_emp

id_to_emp::id_to_emp(unsigned long inp1, int inp2)
{
	status = 0;
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
		interface::log_this("id_to_emp::id_to_emp() : File id_list.dat couldn't be opened...\nFunction aborted");
	}
	else
	{
		fout.seekp(id * sizeof(id_to_emp), ios::beg);
		fout.write((char *) this, sizeof(id_to_emp));
		if(fout.fail())
		{
			interface::log_this("id_to_emp::id_to_emp() : Error while writing to file id_list.dat (fout.fail())\nFunction aborted");
		}
		else
		{
			status = 1;
		}
	}
}

id_to_emp::id_to_emp()
{
	id = employee_type = status = 0;
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
		interface::log_this("id_to_emp::convert() : Error while reading from file id_list.dat (fin.fail())");
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