/**
 * Helen Woodward and Melissa Galonsky
 * Basic utilities for dealing with network traffic
 * for HMC CS125 lab 6
 */
#ifndef utilities
#define utilities 1

#include <stdlib.h>
#include <climits>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <list>

using namespace std;

enum commandType {
	//is this correct?
};

typedef int SID;
typedef int CID;

struct command {
	SID serverId;
	CID clientId;
	enum commandType type;
	int name;
	int argument;
};

void parseConfig(list<SID>& serverList, int configFile);
void setOwnId(int id);

int setUpUdpSock(int port); 
size_t sendTo(int socket, command data, sockaddr* addr); 
int recieveIntFrom(int socket);
size_t recieveCommandFrom(int socket, command result);

void sleep(int sleep_time);

#endif
