#include "base.hpp"

/////////////////////////////////////////////
/// Function definitions for class person

person::person(str inp1, int inp2, Date inp3, address inp4, phone inp5)
{
	strcpy(name, inp1);
	sex = inp2;
	dob = inp3;
	adr = inp4;
	strcpy(phone_no, inp5);
	calc_age();
}

char* person::get_name()
{
	return name;
}

int person::get_age()
{
	return age;
}

int person::get_sex()
{
	return sex;
}

int person::get_dob(int inp)
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

char* person::get_address(int inp)
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

char* person::get_phn()
{
	return phone;
}

void person::calc_age()
{
    time_t t = time(0);
    struct tm *now = localtime(&t);
    Date dnow(now->tm_mday, (now->tm_mon + 1) ,(now->tm_year + 1900));
    if(dnow.month > dob.month || dnow.month == dob.month && dnow.day >= dob.day)
    {
        age = dnow.year - dob.year;
    }
    else
    {
        age = dnow.year - dob.year - 1;
    }
}

void person::set_name(char* a)
{
    strcpy(name, a);
}

void person::set_sex(int a)
{
    sex = a;
}

void person::set_dob(Date a)
{
	dob = a;
	calc_age();
}

void person::set_address(address a)
{
	address = a;
}

void person::set_phone(char* a)
{
	strcpy(phone_no, a);
}

Time::Time()
{
	hour = 0;
	minute = 0;
	second = 0;
}

Time::Time(unsigned h, unsigned m, unsigned s)
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

Date::Date()
{
	day = 0;
	month = 0;
	year = 0;
};

Date::Date(unsigned d, unsigned m, unsigned y)
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

userid::userid(str name ,str plaintext)	//plaintext is the unencrypted password
{
	strcpy(username, name);
	set_key(plaintext);
	makecipher(plaintext);
//	cout<<plaintext<<' '<<default_key<<endl;
//	cout<<strlen(plaintext)<<' '<<strlen(default_key)<<' '<<strlen(passcipher)<<endl;
//	decipher();
}

void userid::makecipher(str plaintext)
{
	int len = strlen(plaintext);
	int keylen = strlen(default_key);
	for(int i = 0; i < len; ++i)
	{
	  int plntext_i = (int)plaintext[i] + 127;
	  int key_i = (int)default_key[i % keylen] + 127;
	  passcipher[i] = (char) ( ( (plntext_i + key_i) % 256 ) - 127);
	}
	passcipher[i] = '\0';
//	cout<<passcipher<<endl<<endl;
}

void userid::set_key(char * plaintext)
{
	 randomize();
	 int len = strlen(plaintext);
	 int keylen = random(len/2 + 1) + len/2;    //so that the key is not too short
	 for (int i = 0; i <=keylen; ++i)
	 {
		  default_key[i] = (char)( random(256) - 127 );
	 }
	 default_key[i] = '\0';
}

char * userid::decipher()
{
	str decryptedpass;
	int len = strlen(passcipher);
	int keylen = strlen(default_key);
	for(int i = 0; i < len; ++i)
	{
	  int cipher_i = (int)passcipher[i] + 127;
	  int key_i = (int)default_key[i % keylen] + 127;
	  decryptedpass[i] = (char) ( ( (cipher_i - key_i + 256) % 256 ) - 127);
	}
	decryptedpass[i] = '\0';
//	cout<<endl<<decryptedpass<<endl;
	return decryptedpass;
}

char * userid::getusername()
{
	return username;
}

int userid::login(char * password)
{
	if(!strcmp(password, decipher()))
		return 1;
	else
		return 0;
}
