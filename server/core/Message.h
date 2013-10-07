#ifndef _NIX_MESSAGE
#define _NIX_MESSAGE

class Message
{
public:
	Message(int clientSocket, char destination, char option, char function, int value);
	~Message();

	int getClientSocket();
	void setClientSocket(int clientSocket);
	char getDestination();
	void setDestination(char destination);
	char getOption();
	void setOption(char option);
	char getFunction();
	void setFunction(char function);
	int getValue();
	void setValue(int value);

private:
	int clientSocket;
	char destination;
	char option;
	char function;
	int value;
};

#endif
