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

person::person()
{
	strcpy(name, "");
	dob = Date();
	strcpy(phone_no, "");
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

Date person::get_dob()
{
	return dob;
}

address person::get_address()
{
	return adr;
}

char* person::get_phone()
{
	return phone_no;
}

void person::calc_age(Date dnow)
{
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

void person::set_dob(Date bday, Date dnow)
{
	dob = bday;
	calc_age(dnow);
}

void person::set_address(address a)
{
	adr = a;
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
	hour = h;
	minute = m;
	second = s;
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

Date system::get_date()
{
	time_t t = time(0);
	struct tm *now = localtime(&t);
	Date dnow(now->tm_mday, (now->tm_mon + 1) ,(now->tm_year + 1900));
	return dnow;
}

Time system::get_time()
{
	time_t t = time(0);
	struct tm *now = localtime(&t);
	Time tnow(now->tm_hour, now->tm_min ,now->tm_sec);
	return tnow;
}

address::address(const char *hno, const char *strt, const char *cty, const char *dist, const char *stat)
{
	strcpy(house_no, hno);
	strcpy(street, strt);
	strcpy(city, cty);
	strcpy(district, dist);
	strcpy(state, stat);
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

userid::userid()
{
	strcpy(username, "");
	strcpy(passcipher, "");
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
	int keylen = random(len/2 + 1) + len/2;	//so that the key is not too short
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

char * userid::get_username()
{
	return username;
}

void userid::set_username(char * inp)
{
	strcpy(username, inp);
}

int userid::login(char * password)
{
	if(!strcmp(password, decipher()))
		return 1;
	else
		return 0;
}

transaction::transaction(float a, Date d, Time t, char * b)
{
	amount = a;
	strcpy(reason, b);
	_date = d;
	_time = t;
}

transaction::transaction()
{
	amount = 0;
	strcpy(reason, "NA");
	_date = Date();
	_time = Time();
}

box & operator<<(box &output, sex s)
{
	switch(s)
	{
		case MALE:
			return output << "Male";
		case FEMALE:
			return output << "Female";
		case TRANS:
			return output << "Transsexual";
		default:
			return output << "Invalid";
	}
}

box & operator<<(box &output, body_parts b)
{
	switch(b)
	{
		case BRAIN:
			return output << "Brain";
		case HEART:
			return output << "Heart";
		case SKIN:
			return output << "Skin";
		case LUNG:
			return output << "Lung";
		case BONE:
			return output << "Bone";
		case EYE:
			return output << "Eye";
		case THROAT:
			return output << "Throat";
		case TEETH:
			return output << "Teeth";
		case STOMACH:
			return output << "Stomach";
		case BLOOD:
			return output << "Blood";
		case GUT:
			return output << "Gastrointestinal tract";
		case GEN:
			return output << "General ailments";
		default:
			return output << "Invalid";
	}
}

box & operator<<(box &output, Time & t)
{
	return output << (unsigned long)t.hour << ':' << (unsigned long)t.minute << ':' << (unsigned long)t.second;
}

box & operator<<(box &output, Date & d)
{
	return output << (unsigned long)d.day << '/' << (unsigned long)d.month << '/' << (unsigned long)d.year;
}

box & operator<<(box &output, address & a)
{
	return output << a.house_no << ", " << a.street << ", " 
		   << a.city << ", " << a.district << ", " << a.state;
}