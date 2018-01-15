#ifndef INTERFACE_HPP
#define INTERFACE_HPP


class interface{
	public:
		static void init();
		static int login_screen();
		static int menu();
		static void patient_management();
		static void employee_management();
		static void stock_management();
		
		static void doctor_screen();
		static void nurse_screen();
		static void receptionist_screen();

		static void error(char*);
		static void clear_error();
		
		static int log_this(char *);

	private:
		interface();
		class validate_menu			//for creating a validation function to use in menus
		{							//to validate the choice input of the menu option to be accessed
				static int lowest_choice, greatest_choice;
				validate_menu();
			public:
				static int input(const char *);
				static void set_menu_limits(int, int);
		};
		static box window;
};

#endif /* INTERFACE_HPP */