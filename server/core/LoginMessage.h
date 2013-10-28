#ifndef _NIX_LOGINMESSAGE
#define _NIX_LOGINMESSAGE

class LoginMessage
{
public:
	LoginMessage(clientSocket, char* user, char* pass);
	~LoginMessage();

	int getClientSocket();
	char* getUsername();
	char* getPassword();
private:
	int clientSocket;
	char username[30];
	char password[30];
};

#endif