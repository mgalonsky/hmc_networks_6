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
#include <string>
#include <list>

using namespace std;

#define SPORT 4444
#define CPORT 5555

enum commandType {
	create_lock,
	get_lock,
	release_lock,
	create_int,
	get_int,
	set_int,
	create_barrier,
	wait_on_barrier
};

typedef unsigned long SID;
typedef unsigned long CID;

struct command {
	SID serverId;
	CID clientId;
	commandType type;
	int name;
	int argument;
};

void parseConfig(list<SID>& serverList, string configFile);

int setUpUdpSock(int port); 
size_t sendTo(int socket, command data, sockaddr* addr); 
int recieveIntFrom(int socket);
size_t recieveCommandFrom(int socket, command& result);

void sleep(int sleep_time);

#endif
