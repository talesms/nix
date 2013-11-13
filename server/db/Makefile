compile:
	g++ Cache.cpp DataBase.h DataBase.cpp CacheManager.h CacheManager.cpp ../core/Configuration.h ../core/Configuration.cpp ../core/LoginMessage.h ../core/LoginMessage.cpp ../core/CharacterListMessage.h ../core/CharacterListMessage.cpp -L/user/lib -lmysqlpp -I/usr/include/mysql -I/usr/include/mysql++ -o cache

	g++ cacheClient.cpp ../core/LoginMessage.h ../core/LoginMessage.cpp ../core/CharacterListMessage.h ../core/CharacterListMessage.cpp -o cacheClient

	g++ db.cpp DataBase.h DataBase.cpp ../core/Configuration.h ../core/Configuration.cpp -L/user/lib -lmysqlpp -I/usr/include/mysql -I/usr/include/mysql++ -o db
clean:
	rm cache
	rm cacheClient

#compile:
#	g++ db.cpp DataBase.h DataBase.cpp ../core/Configuration.h ../core/Configuration.cpp -L/user/lib -lmysqlpp -I/usr/include/mysql -I/usr/include/mysql++ -o db

#clean:
#	rm db

# core/MessageDelivery.h core/MessageDelivery.cpp
#core/Publisher.h core/Publisher.cpp core/Subscriber.h core/Subscriber.cpplibmysqlcppconn-static
