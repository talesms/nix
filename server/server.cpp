#include <vector>
#include <stdio.h>
#include "core/BaseNet.h"
#include "core/MessageDelivery.h"
#include "core/Configuration.h"
#include "core/Subscription.h"
#include "Modules.h"

#define NUM_MODULES 1
#define SIZE_AUTORIZATION_KEY 14


void connectModules(Modules* modules, BaseNet* baseNet)
{
    int sock;
    int portno;
    char key[SIZE_AUTORIZATION_KEY];

    portno = atoi(Configuration::getConfig("login_server_port").c_str());

    for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
      key[i] = Configuration::getConfig("login_server_key").c_str()[i];

    std::cout << "WAITING login server to connect, on port " << portno << std::endl;
    sock = baseNet->connectModule(portno, Configuration::getConfig("login_server_ip").c_str(), key);

    if(sock > 0)
    {
      modules->addModule(sock);

      std::cout << "SUCCESS login server has connected, sock: "<< sock << std::endl;
    }
    else
      std::cout << "ERROR login server has failed to connect" << std::endl;


    portno = atoi(Configuration::getConfig("region_server1_port").c_str());

    for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
    	key[i] = Configuration::getConfig("region_server1_key").c_str()[i];

    std::cout << "WAITING region server1 to connect, on port " << portno << std::endl;
    sock = baseNet->connectModule(portno, Configuration::getConfig("region_server1_ip").c_str(), key);

    if(sock > 0)
    {
      modules->addModule(sock);

      std::cout << "SUCCESS region server1 has connected, sock: "<< sock << std::endl;
    }
    else
      std::cout << "ERROR region server1 has failed to connect" << std::endl;
}

int main(int argc, char *argv[])
{
  BaseNet baseNet(atoi(Configuration::getConfig("frontend_server_port").c_str()));
  Modules modules;

  baseNet.setRequestListener(atoi(Configuration::getConfig("frontend_server_request_port").c_str()), Configuration::getConfig("frontend_server_request_key").c_str());
  connectModules(&modules, &baseNet);

  MessageDelivery messageDelivery(&baseNet);

  Subscription subChat(&modules, MESSAGE_DESTINATION_CHAT);
  Subscription subLogin(&modules, MESSAGE_DESTINATION_LOGIN);
  Subscription subCreation(&modules, MESSAGE_DESTINATION_CREATION);
  Subscription subRegion(&modules, MESSAGE_DESTINATION_REGION);
  messageDelivery.subscribe(&subChat);
  messageDelivery.subscribe(&subLogin);
  messageDelivery.subscribe(&subCreation);
  messageDelivery.subscribe(&subRegion);

  baseNet.run(&messageDelivery);

  return 0;
}
