#include "Network.h"
#include "core/MessageDelivery.h"

int main(int argc, char *argv[])
{
    Network* net = new Network();
    MessageDelivery* messageDelivery = new MessageDelivery(net);

    net->run(messageDelivery);

}
