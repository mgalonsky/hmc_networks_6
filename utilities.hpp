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
	create_lock,
	get_lock,
	create_lock,
	create_int,
	get_int,
	set_int,
	create_barrier,
	wait_on_barrier
};

typedef int SID;
typedef int CID;

struct command {
	SID serverId;
	CID clientId;
	commandType type;
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
