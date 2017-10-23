#ifndef BASE
#define BASE

#include <string.h>
#include <time.h>

const int K = 14;
typedef char str[80];
typedef char phn[11];

enum sex {MALE, FEMALE, TRANS};
enum date_type {DAY, MONTH, YEAR};
enum time_type {HOUR, MINUTE, SECOND};
enum body_parts {BRAIN, HEART, SKIN, LUNG, BONE, GEN}; // GEN for general problems
enum adr_parts {HOUSE_NO, STREET, CITY, DISTRICT, STATE};
enum times_of {START, END};

struct Time{
	unsigned int hour;
	unsigned int minute;
	unsigned int second;

	Time();
	Time(unsigned h, unsigned m, unsigned s);
};

struct Date{
	unsigned int day;
	unsigned int month;
	unsigned int year;

	Date();
	Date(unsigned d, unsigned m, unsigned y);
};

struct adr{
	str house_no;
	str street;
	str city;
	str district;
	str state;
};

struct disease{
	str name;
	int type;  		    //refers to body part affected (LUNG, HEART, etc)
	str symptoms[4];    //symptoms reported by patient
};

struct insurance{
	str provider;
	unsigned long amount;
	Date expiry;
};

struct medicine{
	int code;
	float price;
	str name;
	float dosage;
}

class person{
	public:
		person(str, int, Date, adr, phn);	// Explicit constructor

		// 'Get's
		char* getname();
		int getage();
		int getsex();
		int getdob(int);
		char* getadr(int);
		char* getphn();

		//Updating functions
		char* setname(char*);
		int setsex(int);
		Date setdob(Date);
		adr setadr(adr);
		char* setphn(char*);

	protected:
		str name;
		unsigned age;
		unsigned sex;
		Date dob;
		adr address;
		phn phone;

	private:
		void calc_age();
};


#endif
