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
#include <time.h>
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

int sendToServers(serverMessage message) {
	for (auto &curr_pair : serverMap) {
		sockaddr* addr = (sockaddr *) &curr_pair.second;
		if (sendto(serverPort, (void*)&message, sizeof(message), 0, addr, sizeof(curr_pair.second)) < 0) {
			return -1;
		}
	}
	return 0;
}

void* pinger(void* ptr) {
	serverMessage ping;
	//detach thread
	pthread_detach(pthread_self());
	ping.isPing = true;
	ping.source = myID;
	while (true) {
		//update the ping's time
		ping.time = time(NULL);
		//send the message to all the servers (helper function!)
		if (sendToServers(ping) < 0) {
			//there was an error, stop
			return NULL;
		}
		//wait for 900 milliseconds before repeating
		sleep(900);
	}

}

int main(int argc, char**argv) {
	//call init for all set up
	if (init("configFile") < 0) {
		exit(1);
	}
	//start all the threads
}



