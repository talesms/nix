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

    portno = atoi(Configuration::getConfig("region_server1_port").c_str());

    for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
    	key[i] = Configuration::getConfig("region_server1_key").c_str()[i];

    std::cout << "WAITING region_server1_ip to connect, on port " << portno << std::endl;
    sock = baseNet->connectModule(portno, Configuration::getConfig("region_server1_ip").c_str(), key);

    if(sock > 0)
    {
      modules->addModule(sock);

      std::cout << "SUCCESS region_server1_ip has connected, sock: "<< sock << std::endl;
    }
    else
      std::cout << "ERROR region_server1_ip has failed to connect" << std::endl;
}

int main(int argc, char *argv[])
{
  BaseNet baseNet(atoi(Configuration::getConfig("frontend_server_port").c_str()));
  Modules modules;

  connectModules(&modules, &baseNet);

  MessageDelivery messageDelivery(&baseNet);

  Subscription subModules(&modules, MESSAGE_DESTINATION_REGION);
  messageDelivery.subscribe(&subModules);

  baseNet.run(&messageDelivery);

  return 0;
}
