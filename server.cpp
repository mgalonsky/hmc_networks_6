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
#include <queue>

using namespace std;

struct serverMessage {
	time_t time;
	SID source;
	bool isPing;
	command ClientCommand;
};

//comparison class for priority queue
class CompareMessage {
	public:
	bool operator()(serverMessage& message1, serverMessage& message2) {
		if(message1.time == message2.time) {
			return message1.source < message2.source;
		}
		else {
			return message1.time < message2.time;
		}
	}
};

typedef priority_queue<serverMessage, vector<serverMessage>, CompareMessage> message_pq;

map<SID, sockaddr_in> serverMap;
SID myID;
int serverPort;
int clientPort;
time_t* timeTable;
message_pq commandQueue;

int init(string configFile) {
	//set myID
	myID = htonl(INADDR_ANY);
	//set up serverMap
	list<SID> serverList;
	parseConfig(serverList, configFile);
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short) SPORT);
	for (SID serverID : serverList) {
		serveraddr.sin_addr.s_addr = serverID;
		serverMap.insert(pair<SID, sockaddr_in>(serverID, serveraddr));
	}
	//set up table of times
		//variable size, so need it to be on the heap
	timeTable = new time_t[serverMap.size()];
	//don't need to do any extra pq set up because of typedef

	//set up locks for priority queue
	//set up signal for priority queue
	//set up ports (both Server and Client)
	if ((serverPort = setUpUdpSock(SPORT)) < 0) {
		cerr << "error setting up server port" << endl;
		return -1;
	}
	if ((clientPort = setUpUdpSock(CPORT)) < 0) {
		cerr << "error setting up client port" << endl;
		return -1;
	}
	return 0;
}

int main(int argc, char**argv) {
	//call init for all set up
	if (init("configFile") < 0) {
		exit(1);
	}
}



