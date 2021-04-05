#include <iostream>
#include "auth.h"

bool attempt_login()
{
	std::cout << "Username: ";
	std::cin >> username;
	std::cout << "Password: ";
	std::cin >> password;
	setup();

	if (auth())
		return true;
	else
	{
		std::cout << "[ api ] Login failure. Exiting" << std::endl;
		std::exit(0);
		return false;
	}		
}

int main()
{
	if (attempt_login())
	{
		std::cout << "1: " << get_response(COMMAND_MOTD) << std::endl;
		Sleep(1000);
		std::cout << "2: " << get_response(COMMAND_VERSION) << std::endl;
		Sleep(1000);
		std::cout << "3: " << get_response(COMMAND_STATUS) << std::endl;
		Sleep(1000);
	}
}