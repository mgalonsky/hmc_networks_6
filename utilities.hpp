/**
 * Helen Woodward and Melissa Galonsky
 * Basic utilities for dealing with network traffic
 * for HMC CS125 lab 6
 */
#ifndef utilites
#define utilities 1

struct command {
	int serverId;
	int clientId;
	byte commandType;
	int argument;
};

void parseConfig(Map<int, sockaddr> serverList, string configFile);
void setOwnId(int id);

int setUpUdpSock(int port); 
ssize_t sendTo(int socket, command data, sockaddr* addr); 
int recieveIntFrom(int socket);
int recieveCommandFrom(int socket);

#endif
