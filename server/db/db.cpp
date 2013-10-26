#include "DataBase.h"
#include <string>


int main()
{
	DataBase dataBase;

	if (dataBase.verifyLogin(string("testuser"), string("test")))
	{
		std::cout << "Existe usuario!" << std::endl;
	}
	else
	{
		std::cout << "Nao Existe usuario!" << std::endl;
	}

	std::cout << "Fim do programa..." << std::endl;

}