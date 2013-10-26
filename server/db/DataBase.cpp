#include "DataBase.h"

DataBase::DataBase()
{
	driver = sql::mysql::get_mysql_driver_instance();
	con = driver->connect(Configuration::getConfig("db_url").c_str(),
		Configuration::getConfig("db_user").c_str(),
		Configuration::getConfig("db_password").c_str());
	stmt = con->createStatement();
}

DataBase::~DataBase()
{
	delete stmt;
	delete con;
}

bool DataBase::verifyLogin(string user, string password)
{
	res = stmt->executeQuery("SELECT iduser FROM user WHERE username='" 
			+ user + "' and password='" + password + "'");

	return res->getInt(0) > 0;
}
