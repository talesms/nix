#include "../core/Configuration.h"
#include "mysql_connection.h"
#include "mysql_driver.h"

#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"

#include <stdio.h>
#include <string>


class DataBase
{
public:
	DataBase();
	~DataBase();

	bool verifyLogin(string user, string password);

private:
	sql::mysql::MySQL_Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;

};