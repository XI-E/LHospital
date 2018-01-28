/*!
 \file BASE.HPP
 \brief Contains the declarations of the basic structs, classes, typedefs
		and enums to be used in the whole program
*/

#ifndef BASE
#define BASE

#include "ui/ui.hpp"
#include <fstream.h>
#include <string.h>
#include <dir.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>			//for random() and randomize()

typedef char str[80]; //!>typedef for general strings
typedef char phone[11]; //!>typedef for strings storing phone numbers

enum sex {MALE, FEMALE, TRANS}; //!>Identifiers for different sexes
enum date_type {DAY, MONTH, YEAR}; //!>Identifiers for different parts of a date
enum time_type {HOUR, MINUTE, SECOND}; //!>Identifiers for different parts of a time
enum body_parts {BRAIN, HEART, SKIN,
	LUNG, BONE, EYE,
	THROAT, TEETH, STOMACH,
	BLOOD, GUT, GEN}; //!>Identifiers for different parts of the human body
/*!>used for recording specialities of doctors(GEN for general problems)*/

enum address_parts {HOUSE_NO, STREET, CITY, DISTRICT, STATE}; //!>Identifiers for different parts of an address
enum times_of {START, END}; //!>Identifiers indicating start or end of something
/*!>(used in get_shift() and set_shift() to get or set starting or ending shift time)*/

struct Time{
	unsigned int hour;
	unsigned int minute;
	unsigned int second;

	Time();
	Time(unsigned h, unsigned m, unsigned s);
};//!>Structure facilitating implementation of a time variable

struct Date{
	unsigned int day;
	unsigned int month;
	unsigned int year;

	Date();
	Date(unsigned d, unsigned m, unsigned y);
};//!>Structure facilitating implementation of a date variable

class system
{
	private:
		system();
	public:
		static Date get_date();
		static Time get_time();
};//!>Contains prototypes of functions that return the system date and time

struct address{
	str house_no;
	str street;
	str city;
	str district;
	str state;
	
	address(const char * = "", const char * = "", const char * = "", const char * = "", const char * = "");
};//!>Structure facilitating implementation of an address variable

struct disease{
	str name;
	int type;			//refers to body part affected (LUNG, HEART, etc)
	str symptoms[4];	//symptoms reported by patient
};//!>Structure facilitating implementation a variable storing details of a disease

struct insurance{
	str provider;
	unsigned long amount;
	Date expiry;
};//!>Structure facilitating implementation a variable storing insurance details of any person

struct medicine{
	int code;
	float price;
	str name;
	float dosage;
	long stock;
};//!>Structure facilitating implementation a variable storing details of a medicine

struct transaction{
	float amount;
	str reason;
	Date _date;
	Time _time;
	transaction(float, Date = Date(), Time = Time(), char* = "NA");
	transaction();
};//!>Structure facilitating implementation a variable storing details of a transaction

struct procedure{
	str name;
	float cost;
};//!>Structure facilitating implementation a variable storing details of a medical procedure

//!Class storing all common data members of a person
/*!
Parent class to all the persons that this program handles, i.e patients, 
and all types of employees.
*/
class person{
	public:
		person(str, int, Date, address, phone);	//!>Explicit constructor
		person(); //!>Default constructor
		
		//!@{Getters
		char* get_name();
		int get_age();
		int get_sex();
		Date get_dob();
		address get_address();
		char* get_phone();
		//!}@

		//!@{Setters
		void set_name(char*);
		void set_sex(int);
		void set_dob(Date, Date = system::get_date());
		void set_address(address);
		void set_phone(char*);
		//!}@

	protected:
		str name; //!>Name of the person
		unsigned age; //!>Age of the person
		unsigned sex; //!>Sex of the person
		Date dob; //!>Date of birth of the person
		address adr; //!>Address of the person
		phone phone_no; //!>Phone number of the person

	private:
		void calc_age(Date d = system::get_date()); //!>Calculates age of the person using dob
		/*!>\param d The date with respect to which age is to be calculated(default value is set to be the system date)*/
};

//!Class managing login features of the program
/*!
This class stores a username and a password in encrypted form, besides 
the inplementation data. This class uses a vigenere cipher to encrypt the
password and store it.
*/
class userid
{
		str username; //!>Username of the login account
		str passcipher; //!>Encrypted password
		str default_key;//!>Key for making the vigenere cipher
		void makecipher(char *); //!>Makes the vigenere cipher
		void set_key(char *); //!>Sets default_key to a random string
		char * decipher(); //!>deciphers the cipher 'passcipher'

	public:
		userid(char *, char *);	//!>Explicit constructor
		userid(); //!>Default constructor
		char * get_username(); //!>Getter
		void set_username(char *); //!>Setter
		int login(char *); //!>\return 1 if the string input in the function is the password, 0 otherwise
};

//!Defines << operator overloads to facilitate printing of some stuff
class enum_to_str
{
		enum_to_str();
	public:
		friend box & operator<<(box &output, sex s);		//!>converts sex enumeration constant into a string and prints it to a box
		friend box & operator<<(box &output, body_parts b);	//!>converts body_parts enumeration constant into a string and prints it to a box
		friend box & operator<<(box &output, Time & t);		//!>converts Time variable into a string and prints it to a box
		friend box & operator<<(box &output, Date & d);		//!>converts Date variable into a string and prints it to a box
		friend box & operator<<(box &output, address & a);	//!>converts address variable into a string and prints it to a box
};

#endif
