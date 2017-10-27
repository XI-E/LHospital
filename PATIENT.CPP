#include "patient.hpp"

///////FUNCTION DEFINITIONS FOR CLASS PATIENT/////////

patient::patient(str inp1, int inp2 , Date inp3, address inp4, phone inp5, disease inp6, str inp7, str inp8, phone inp9, insurance inp10, Date inp11 = system::get_date()) : person(inp1, inp2, inp3, inp4, inp5)    //if date_of_admission is the current system date, last argument is not needed
{
	dis = inp6;
	strcpy(guardian_name, inp7);
	strcpy(emergency_contact, inp8);
	strcpy(emer_contact_no, inp9);
	insur_info = inp10;

	admission_date = inp11;
	
	if(admission_date != system::get_date())
	    set_dob(inp3, inp11);

	for(int i = 0; i < 50; i++){
		med[i][1] = med[i][2] = 0;
	}
	
	bill_amt = 0;   //bill_amt will be set by doctor after treatment
}

disease patient::get_dis()
{
	return dis;
}

char* patient::get_guardian_name()
{
	return guardian_name;
}

char* patient::get_emergency_contact()
{
	return emergency_contact;
}

char* patient::get_emer_contact_no()
{
	return emer_contact_no;
}

insurance patient::get_insur_info()
{
	return insur_info;
}

int patient::get_admission_date(int inp)
{
	switch(inp)
	{
		case DAY:
			return admission_date.day;
		case MONTH:
			return admission_date.month;
		case YEAR:
			return admission_date.year;
		default:
			return 0;
	}
}

unsigned long patient::get_bill_amt()
{
	return bill_amt;
}

int patient::get_med(int a, int b){
	return med[a][b];
}

void patient::set_dis(disease a)
{
	dis = a;
}

void patient::set_guardian_name(char *a)
{
	strcpy(guardian_name, a);
}

void patient::set_emergency_contact(char *a)
{
	strcpy(emergency_contact, a);
}

void patient::set_emer_contact_no(char *a)
{
	strcpy(emer_contact_no, a);
}

void patient::set_insur_info(insurance a)
{
	insur_info = a;
}

void patient::set_admission_date(Date a)
{
	admission_date = a;
	set_dob(dob, admission_date);
}

void patient::set_bill_amt(unsigned long a)
{
	bill_amt = a;
}

void patient::set_med(int a, int b, int c){
	med[a][0] = b;
	med[a][1] = c;
}
