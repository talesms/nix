
#include "../core/CharacterListMessage.h"
#include "../core/LoginMessage.h"
#include "../core/Skill.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char const *argv[])
{
	int sockfd, portno, n, sum_n = 0, len =0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    Avatar avatar;
    CharacterListMessage* listBuffer = new CharacterListMessage(&avatar, 0);
    LoginMessage* loginBuffer = new LoginMessage("", "");

    portno = 1446;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket" << std::endl;
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        std::cout << "ERROR, no such host" << std::endl;
        return 0;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
         std::cout << "ERROR connecting" << std::endl;

    int var[2];
    Skill skill;
    var[0] = 1;
    var[1] = 2;
    send(sockfd, var, 2*sizeof(int), 0);
    recv(sockfd, &skill, sizeof(Skill), 0);
    std::cout << "===== skill =====: " << std::endl;
	std::cout << "skill id: " << skill.idskill << std::endl;
	std::cout << "skill name: " << skill.name << std::endl;
	std::cout << "skill note: " << skill.note << std::endl;
	std::cout << "skill mana: " << skill.mana << std::endl;
	std::cout << "skill castTime: " << skill.castTime << std::endl;
	std::cout << "skill areaRadius: " << skill.areaRadius << std::endl;
	std::cout << "skill BaseDamage: " << skill.BaseDamage << std::endl;
	std::cout << "skill BaseHeal: " << skill.BaseHeal << std::endl;
	std::cout << "skill modifier1: " << skill.modifier1 << std::endl;
	std::cout << "skill modifierValue1: " << skill.modifierValue1 << std::endl;
	std::cout << "skill modifier2: " << skill.modifier2 << std::endl;
	std::cout << "skill modifierValue2: " << skill.modifierValue2 << std::endl;
	std::cout << "skill modifier3: " << skill.modifier3 << std::endl;
	std::cout << "skill modifierValue3: " << skill.modifierValue3 << std::endl;

	close(sockfd);


    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket" << std::endl;
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        std::cout << "ERROR, no such host" << std::endl;
        return 0;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
         std::cout << "ERROR connecting" << std::endl;

    send(sockfd, "5SD1fsKbh5hll1m", 14, 0);

	LoginMessage* loginMsg = new LoginMessage("testuser", "test");
    send(sockfd, loginMsg, sizeof(LoginMessage), 0);

	recv(sockfd, listBuffer, sizeof(CharacterListMessage), 0);

	for(int i=0; i<10; i++)
	{
		avatar = listBuffer->getCharacterList()[i];
		std::cout << "==========" << i << "==========" << std::endl;
		std::cout << "id: " << avatar.idavatar << std::endl;
		std::cout << "name: " << avatar.name << std::endl;
		std::cout << "level: " << avatar.level << std::endl;
		std::cout << "race: " << avatar.race << std::endl;
		std::cout << "classNum: " << avatar.classNum << std::endl;
		std::cout << "sex: " << avatar.sex << std::endl;
		std::cout << "maxhp: " << avatar.maxhp << std::endl;
		std::cout << "hp: " << avatar.hp << std::endl;
		std::cout << "maxmana: " << avatar.maxmana << std::endl;
		std::cout << "mana: " << avatar.mana << std::endl;
		std::cout << "positionx: " << avatar.positionx << std::endl;
		std::cout << "positiony: " << avatar.positiony << std::endl;
		std::cout << "positionz: " << avatar.positionz << std::endl;
		std::cout << "rotationx: " << avatar.rotationx << std::endl;
		std::cout << "rotationy: " << avatar.rotationy << std::endl;
		std::cout << "rotationz: " << avatar.rotationz << std::endl;
		std::cout << "================================= " << std::endl;
	}

	return 0;
}