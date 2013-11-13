#ifndef _NIX_LOGINMESSAGE
#define _NIX_LOGINMESSAGE

class LoginMessage
{
public:
	LoginMessage(char* user, char* pass);
	~LoginMessage();

	char* getUsername();
	char* getPassword();
private:
	char username[30];
	char password[30];
};

#endif