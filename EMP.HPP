/*!
 \file EMP.HPP
 \brief Contains the definitions of the employee class and its derivatives
*/

#ifndef EMP
#define EMP

#include "base.hpp"

enum emp_type {INVALID, OTHERS, DOCTOR, NURSE, RECEPTIONIST};
//!>Identifiers for indication of different types of employees

//!Class storing details of employees of the hospital
class employee : public person{
		int generate_id(); //!>Generates ID of the employee
		static int generate_id_status; //!>0 if the last id generation was unsuccessful
		/*!>Basically ensures that id generation is stopped when an error occurs in id generation,
		otherwise, the files(max_id.dat, id_list.dat) might start storing meaningless data, which 
		will affect future id generation*/
		
	public:
		employee(str, int, Date, address, phone, unsigned long, Time, Time, str = "", str = ""); //!>Explicit constructor
		/*!>for all those with user accounts(doctors, nurses, receptionists),
		last 2 arguments are to be provided as well*/
		employee(); //!>Default constructor
		
		//!@{Getters
		int get_age(); //!>Overridden function
		/*!>Updates the age of the employee and writes the employee object back to file before returning age*/
		unsigned long get_salary();
		Time get_shift(int inp1); /*!>\param inp1 times_of type variable that indicates starting or ending shift time*/
		unsigned long get_id();
		static int get_generate_id_status();
		transaction * get_last_5_transactions(); //!>Gets the last 5 records present in the file TRANS.DAT of the employee's folder
		//!}@
		
		//!@{Setters
		void set_salary(unsigned long);
		void set_shift(int inp1, Time t1);/*!>\param inp1 times_of type variable that indicates starting or ending shift time*/
		//!}@
		
		userid account; //!>Facilitates login mechanism of the employee
	protected:
		unsigned long id; //!>ID of the employee
		unsigned long salary; //!>Salary of the employee
		Time shift_start; //!>Starting shift time of the employee
		Time shift_end; //!>Ending shift time of the employee
};

//!Class storing details of doctors of the hospital
class doctor : public employee{
	public:
		doctor(str, int, Date, address, phone, unsigned long, Time, Time, int, int, str, str); //!>Explicit constructor
		doctor(); //!>Default constructor
		
		//!@{Getters
		int * get_speciality();
		long * get_patients();
		//!}@
		
		//!@{Setters
		void set_speciality(int *);
		void set_patients(long *);
		//!}@
		
	private:
		int speciality[2]; //!>Doctor's specialization
		long patients[10]; //!>Patients currently under care, can take only 10 at once
};

//!Class storing details of nurses of the hospital
class nurse : public employee{
	public:
		nurse(str, int, Date, address, phone, unsigned long, Time, Time, str, str); //!>Explicit constructor
		nurse(); //!>Default constructor
		long * get_patients(); //!>Getter
		
		void set_patients(long *); //!>Setter
	private:
		long patients[5]; //!>Patients currently under care, can take only 5 at once
};

//!Class storing details of receptionists of the hospital
class receptionist : public employee
{
	public:
		receptionist(str, int, Date, address, phone, unsigned long, Time, Time, str, str); //!>Explicit constructor
		receptionist(); //!>Default constructor
};

//!Class that generates objects storing the employee type corresponding to each id
/*!
This class is used to generate objects storing the employee type corresponding to each id,
and then to store these objects to a file EMPLOYEE/ID_LIST.DAT(The ctor itself does all this)
This class is used to get the employee type of an employee having a particular id
*/
class id_to_emp
{
		unsigned long id;//!>ID of employee
		int employee_type;//!>Type of employee
	public:
		int status;	//!>True whenever the constructor runs successfully and succeeds in storing the object to id_list.dat
		id_to_emp(unsigned long, int); //!>Explicit constructor
		id_to_emp(); //!>Default constructor
		static int convert(unsigned long); //!>Converts id to employee type
};

#endif