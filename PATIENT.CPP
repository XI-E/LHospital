#include "patient.hpp"

///////FUNCTION DEFINITIONS FOR CLASS PATIENT/////////
patient::patient(str inp1, int inp2 , date inp3, adr inp4, phn inp5, disease inp6, str inp7, str inp8, phn inp9, insurance inp10) : person(inp1, inp2, inp3, inp4, inp5)
{
	dis = inp6;
	strcpy(guardian_name, inp7);
	strcpy(emergency_contact, inp8);
	strcpy(emer_contact_no, inp9);
	insur_info = inp10;

	time_t t = time(0);
	struct tm *now = localtime(&t);
	date dnow(now->tm_mday, (now->tm_mon + 1) ,(now->tm_year + 1900));
	admission_date = dnow;

	bill_amt = 0;   //bill_amt will be set by doctor after treatment
}

patient::patient(str inp1, int inp2 , date inp3, adr inp4, phn inp5, disease inp6, str inp7, str inp8, phn inp9, insurance inp10, date inp11) : person(inp1, inp2, inp3, inp4, inp5)    //if date_of_admission is not the current system date
{
	dis = inp6;
	strcpy(guardian_name, inp7);
	strcpy(emergency_contact, inp8);
	strcpy(emer_contact_no, inp9);
	insur_info = inp10;

	admission_date = inp11;

	for(int i = 0; i < 50; i++){
		med[i][1] = med[i][2] = 0;
	}

	bill_amt = 0;   //bill_amt will be set by doctor after treatment
}

disease patient::getdis()
{
	return dis;
}

char* patient::getguardian_name()
{
	return guardian_name;
}

char* patient::getemergency_contact()
{
	return emergency_contact;
}

char* patient::getemer_contact_no()
{
	return emer_contact_no;
}

insurance patient::getinsur_info()
{
	return insur_info;
}

int patient::getadmission_date(int inp)
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

unsigned long patient::getbill_amt()
{
	return bill_amt;
}

int patient::get_med(int a, int b){
	return med[a][b];
}

void patient::setdis(disease a)
{
	dis = a;
}

void patient::setguardian_name(char *a)
{
	guardian_name = a;
}

void patient::setemergency_contact(char *a)
{
	emergency_contact = a;
}

void patient::setemer_contact_no(char *a)
{
	emer_contact_no = a;
}

void patient::setinsur_info(insurance a)
{
	insur_info = a;
}

void patient::setadmission_date(date a)
{
	admission_date = a;
}

void patient::setbill_amt(unsigned long a)
{
	bill_amt = a;
}

void patient::set_med(int a, int b, int c){
	med[a][0] = b;
	med[a][1] = c;
}
