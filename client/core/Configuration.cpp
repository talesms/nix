#include "Configuration.h"

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

string Configuration::getConfig(string atribbute)
{
	string line;
	string result;
	ifstream file(CONFIGURATION_FILE_PATH);
	int findResult;

	if(file.is_open())
	{
		while (! file.eof())
		{
			getline(file, line);
			findResult = line.find(atribbute);

			if(findResult != string::npos)
			{
				result =  line.substr(findResult+atribbute.length()+3,
					line.length());
				return result;
			}
		}
		return "NULL";
	}
}