#ifndef BASE
#define BASE

const int K = 14;
typedef char str[80];
struct date
{
	int day;
	int month;
	int year;
};

struct time
{
	int hour;
	int min;
	int sec;
};

struct address
{
	str state;
	str district;
	str city;
	str street;
	str house_no;
};

struct insurance
{
	str provider;
	unsigned long int amount;
	unsigned long int id_no;
};



#endif
