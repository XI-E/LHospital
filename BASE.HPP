#ifndef BASE
#define BASE

const int K = 14;
typedef char str[80];
typedef char phn[11];

enum sex {MALE, FEMALE, TRANS};
enum date_type {DAY, MONTH, YEAR};
enum time_type {HOUR, MINUTE, SECOND};
enum body_parts {BRAIN, HEART, SKIN, LUNG, BONE, GEN}; // GEN for general problems
enum adr_parts {HOUSE_NO, STREET, CITY, DISTRICT, STATE};
enum times_of {START, END};

struct time{
	unsigned int hour;
	unsigned int minute;
	unsigned int second;

	time(){
		hour = 0;
		minute = 0;
		second = 0;
	}
	time(unsigned h, unsigned m, unsigned s){
		if(h < 24 && m < 60 && s < 60){
			hour = h;
			minute = m;
			second = s;
		}
		else{
			hour = 0;
			minute = 0;
			second = 0;
		}
	}
};

struct date{
	unsigned int day;
	unsigned int month;
	unsigned int year;

	date(){
		day = 0;
		month = 0;
		year = 0;
	};

	date(unsigned d, unsigned m, unsigned y){
		if( d<=31 && m <=12){
			day = d;
			month = m;
			year = y;
		}
		else{
			day = 0;
			month = 0;
			year = 0;
		}
	}

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
	int type;  		    // refers to body part affected (LUNG, HEART, etc)
	str symptoms[4];    //symptoms reported by patient
	str allergies[2];   //patient's known allergies
};

struct insurance{
	str provider;
	unsigned long amount;
	date expiry;
};

class person{
	public:
		person(str, int, date, adr, phn);	// Explicit constructor

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
		date setdob(date);
		adr setadr(adr);
		char* setphn(char*);

	protected:
		str name;
		unsigned age;
		unsigned sex;
		date dob;
		adr address;
		phn phone;
	
	private:
	    void calc_age();
};


#endif
