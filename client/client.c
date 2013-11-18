#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "core/Avatar.h"
#include "core/Configuration.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, sum_n = 0, len =0;
    int i;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char loginInfo[30];
    char avatarNumber;
    Avatar avatarList[10];


    char buffer[102400];

    portno = atoi(Configuration::getConfig("server_port").c_str());
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(Configuration::getConfig("server_ip").c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    sprintf(loginInfo,"testuser");
    sprintf(&(loginInfo[30]),"test");

    write(sockfd, &loginInfo, sizeof(char)*60);
    read(sockfd, avatarList, sizeof(Avatar)*10);

    for(i=0; i<10; ++i)
    {
        fprintf(stderr,"==== Avatar Number: %d ====\n", i);
        fprintf(stderr,"idavatar: %d\n", avatarList[i].idavatar);
        fprintf(stderr,"name: %s\n", avatarList[i].name);
        fprintf(stderr,"level: %d\n", avatarList[i].level);
        fprintf(stderr,"race: %d\n", avatarList[i].race);
        fprintf(stderr,"classNum: %d\n", avatarList[i].classNum);
        fprintf(stderr,"sex: %c\n", avatarList[i].sex);
        fprintf(stderr,"maxhp: %d\n", avatarList[i].maxhp);
        fprintf(stderr,"hp: %d\n", avatarList[i].hp);
        fprintf(stderr,"maxmana: %d\n", avatarList[i].maxmana);
        fprintf(stderr,"mana: %d\n", avatarList[i].mana);
        fprintf(stderr,"positionx: %d\n", avatarList[i].positionx);
        fprintf(stderr,"positiony: %d\n", avatarList[i].positiony);
        fprintf(stderr,"positionz: %d\n", avatarList[i].positionz);
        fprintf(stderr,"rotationx: %d\n", avatarList[i].rotationx);
        fprintf(stderr,"rotationy: %d\n", avatarList[i].rotationy);
        fprintf(stderr,"rotationz: %d\n", avatarList[i].rotationz);
    }

    avatarNumber = 1;

    write(sockfd, &avatarNumber, sizeof(char));



    while(true)
    {
        n = read(sockfd, buffer, 6);

        //printf("Recebido %d \n", n);

        //write(sockfd, "teste", 6);

        if(!n)
          break;
    }

    printf("\nFechando conecao.\n");
    close(sockfd);
    return 0;
}
