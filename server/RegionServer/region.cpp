#include "../core/BaseNet.h"
#include "../core/MessageConfig.h"
#include "../core/MessageDelivery.h"
#include "../core/Subscription.h"
#include "../core/Configuration.h"

#include "RegionMessageDelivery.h"
#include "AvatarNpcManager.h"


void subcribeModules(RegionMessageDelivery* regionMessageDelivery)
{
	AvatarNpcManager anm;

	Subscription subAnm(&anm, AVATAR_NPC_MANAGER);

	regionMessageDelivery->subscribeModule(&subAnm);
}

int main(int argc, char *argv[])
{
  RegionMessageDelivery regionMessageDelivery;
  MessageDelivery messageDelivery;
  int portno;
  int centralSock;

  portno = atoi(Configuration::getConfig("region_server1_port").c_str());

  BaseNet baseNet(portno);
  centralSock = baseNet.registerModule(portno, Configuration::getConfig("region_server1_key").c_str());
  baseNet.setSock(centralSock);

  subcribeModules(&regionMessageDelivery);

  Subscription subRegionMessageDelivery(&regionMessageDelivery, REGION);
  messageDelivery.subscribe(&subRegionMessageDelivery);

  baseNet.listenCentral(&messageDelivery);

  return 0;
}
