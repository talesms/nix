#include "../core/BaseNet.h"
#include "../core/MessageConfig.h"
#include "../core/MessageDelivery.h"
#include "../core/Subscription.h"
#include "../core/Configuration.h"

#include "InitPlayers.h"


int main(int argc, char *argv[])
{
  int portno;
  int centralSock;
  InitPlayers* initPlayers;
  MessageDelivery* messageDelivery;

  portno = atoi(Configuration::getConfig("login_server_port").c_str());

  BaseNet baseNet(portno);
  centralSock = baseNet.registerModule(portno, Configuration::getConfig("login_server_key").c_str());
  baseNet.setSock(centralSock);

  messageDelivery = new MessageDelivery(&baseNet);
  messageDelivery->setCentralRequestListener();
  initPlayers = new InitPlayers(messageDelivery);

  Subscription subInitPlayers(initPlayers, MESSAGE_DESTINATION_LOGIN);
  messageDelivery->subscribe(&subInitPlayers);

  baseNet.listenCentral(messageDelivery);

  return 0;
}
