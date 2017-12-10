#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "ui/ui.hpp"

class interface{
	public:
		static void interface_init();
		static int login_screen();
		static int menu();
		static void patient_management();
		static void employee_management();
		static void stock_management();

		static void error(char*);
		static void clear_error();

	private:
		interface();
		static box window;
};

#endif /* INTERFACE_HPP */