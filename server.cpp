/*
 * Helen Woodward and Melissa Galonsky
 * Code for the servers for a simple distributed system
 * for HMC CS125 lab 6
 */

#include "utilities.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <map>

using namespace std;

map<SID, sockaddr_in> servermap;
SID myID;

void init(map<SID, sockaddr_in> serverMap, string configFile) {
	myID = htonl(INADDR_ANY);
	list<SID> serverList;
	parseConfig(serverList, configFile);
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short) SPORT);
	for (SID serverID : serverList) {
		serveraddr.sin_addr.s_addr = serverID;
		serverMap.insert(pair<SID, sockaddr_in>(serverID, serveraddr));
	}
}

int main(int argc, char**argv) {
}



