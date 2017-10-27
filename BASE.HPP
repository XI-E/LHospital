#ifndef BASE
#define BASE

#include <string.h>
#include <time.h>
#include <stdlib.h> //for random() and randomize()

const int K = 14;
typedef char str[80];
typedef char phone[11];

enum sex {MALE, FEMALE, TRANS};
enum date_type {DAY, MONTH, YEAR};
enum time_type {HOUR, MINUTE, SECOND};
enum body_parts {BRAIN, HEART, SKIN,
	LUNG, BONE, EYE,
	THROAT, TEETH, STOMACH,
	BLOOD, ASSMAN, GEN}; // GEN for general problems
enum address_parts {HOUSE_NO, STREET, CITY, DISTRICT, STATE};
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

class system
{
    private:
        system();
    public:    
        static Date get_date();
        static Time get_time();
};

struct address{
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
	long stock;
};

struct transaction{
	float amount;
	str reason;
	Date _date;
	Time _time;
	transaction(float, Date = Date(), Time = Time(), char* = "NA");
	transaction();
};

struct procedure{
	str name;
	float cost;
};

class person{
	public:
		person(str, int, Date, address, phone);	// Explicit constructor

		// 'Get's
		char* get_name();
		int get_age();
		int get_sex();
		int get_dob(int);
		char* get_address(int);
		char* get_phone();

		//Updating functions
		void set_name(char*);
		void set_sex(int);
		void set_dob(Date, Date = system::get_date());
		void set_address(address);
		void set_phone(char*);

	protected:
		str name;
		unsigned age;
		unsigned sex;
		Date dob;
		address adr;
		phone phone_no;

	private:
		void calc_age(Date = system::get_date());
};

class userid
{
		str username;
		str passcipher;             //encrypted password
		str default_key;            //key for making the vigenere cipher
		void makecipher(char *);    //makes the vigenere cipher
		void set_key(char *);       //sets default_key
		char * decipher();          //deciphers the cipher 'passcipher'

	public:
		userid(char *, char *);
		char * getusername();
		int login(char *);
};

#endif
