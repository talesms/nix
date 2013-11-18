#ifndef _NIX_CONFIG
#define _NIX_CONFIG

#include <iostream>
#include <fstream>
#include <string>

#define CONFIGURATION_FILE_PATH "Configuration.txt"

using namespace std;

class Configuration
{
public:
	Configuration();
	~Configuration();

	static string getConfig(string atribbute);
};

#endif