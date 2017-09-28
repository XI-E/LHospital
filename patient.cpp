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
    
    bill_amt = 0;   //bill_amt will be set by doctor after treatment
}

disease getdis()
{
    return dis;
}

char* getguardian_name()
{
    return guardian_name;
}

char* getemergency_contact()
{
    return emergency_contact;
}

char* getemer_contact_no()
{
    return emer_contact_no;
}

insurance getinsur_info()
{
    return insur_info;
}

int getadmission_date(int inp)
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

unsigned long getbill_amt()
{
    return bill_amt;
}

disease setdis(disease a)
{
    dis = a;
    return dis;
}

char* setguardian_name(char *a)
{
    guardian_name = a
    return guardian_name;
}

char* setemergency_contact(char *a)
{
    emergency_contact = a;
    return emergency_contact;
}

char* setemer_contact_no(char *a)
{
    emer_contact_no = a;
    return emer_contact_no;
}

insurance setinsur_info(insurance a)
{
    insur_info = a;
    return insur_info;
}

date setadmission_date(date a)
{
    admission_date = a;
    return admission_date;
}

unsigned long setbill_amt(unsigned long a)
{
    bill_amt = a;
    return bill_amt;
}
