#ifndef _NIX_MESSAGE
#define _NIX_MESSAGE

class Message
{
public:
	Message(int clientSocket, char destination, char option, char function, int value);
	Message(int clientSocket, char* data);
	~Message();

	int getClientSocket();
	char getDestination();
	char getOption();
	char getFunction();
	int getValue();

private:
	int clientSocket;
	char destination;
	char option;
	char function;
	int value;
};

#endif
