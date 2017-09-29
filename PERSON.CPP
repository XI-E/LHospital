#include "base.hpp"
//#include <time.h>    //for time_t, localtime() and tm structure

/////////////////////////////////////////////
/// Function definitions for class person

person::person(str inp1, int inp2, date inp3, adr inp4, phn inp5)
{
	strcpy(name, inp1);
	sex = inp2;
	dob = inp3;
	address = inp4;
	strcpy(phone, inp5);
	calc_age();
}

char* person::getname()
{
	return name;
}

int person::getage()
{
	return age;
}

int person::getsex()
{
	return sex;
}

int person::getdob(int inp)
{
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

char* person::getadr(int inp)
{
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

char* person::getphn()
{
	return phone;
}

void person::calc_age()
{
    time_t t = time(0);
    struct tm *now = localtime(&t);
    date dnow(now->tm_mday, (now->tm_mon + 1) ,(now->tm_year + 1900));
    if(dnow.month > dob.month || dnow.month == dob.month && dnow.day >= dob.day)
    {
        age = dnow.year - dob.year;
    }
    else
    {
        age = dnow.year - dob.year - 1;
    }
}

char* person::setname(char* a)
{
    strcpy(name, a);
    return name;
}

int person::setsex(int a)
{
    sex = a;
    return sex;
}

date person::setdob(date a)
{
	dob = a;
	calc_age();
	return dob;
}

adr person::setadr(adr a)
{
	address = a;
	return address;
}

char* person::setphn(char* a)
{
	strcpy(phone, a);
	return phone;
}

time::time()
{
	hour = 0;
	minute = 0;
	second = 0;
}

time::time(unsigned h, unsigned m, unsigned s)
{
	if(h < 24 && m < 60 && s < 60)
	{
		hour = h;
		minute = m;
		second = s;
	}
	else
	{
		hour = 0;
		minute = 0;
		second = 0;
	}
}

date::date()
{
	day = 0;
	month = 0;
	year = 0;
};

date::date(unsigned d, unsigned m, unsigned y)
{
	if( d<=31 && m <=12)
	{
		day = d;
		month = m;
		year = y;
	}
	else
	{
		day = 0;
		month = 0;
		year = 0;
	}
}
