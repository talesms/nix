#include "CacheManager.h"

int main()
{
	CacheManager cm;

	cm.run(Configuration::getConfig("cache_manager_key").c_str());

	return 0;
}