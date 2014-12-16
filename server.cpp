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
#include "barrier.h"

using namespace std;

struct serverMessage {
	time_t time;
	SID source;
	bool isPing;
	command clientCommand;
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

struct dataContainer {
	map<int, Barrier> barriers;
	map<int, Lock> locks;
	map<int, int> ints;
};

typedef priority_queue<serverMessage, vector<serverMessage>, CompareMessage> message_pq;

map<SID, sockaddr_in> serverMap;
SID myID;
int serverPort;
int clientPort;
map<SID, time_t> timeMap;
pair<SID, time_t> minTime;
message_pq commandQueue;
pthread_mutex_t queueLock;
pthread_mutex_t timeLock;
pthread_cond_t queueSignal;
pthread_cond_t timeSignal;
sockaddr_in clientAddr;

int init(string configFile) {
	//set myID
	myID = htonl(INADDR_ANY);
	//set up serverMap
	list<SID> serverList;
	parseConfig(serverList, configFile);
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short) SPORT);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons((unsigned short) CPORT);
	for (SID serverID : serverList) {
		serveraddr.sin_addr.s_addr = serverID;
		serverMap.insert(pair<SID, sockaddr_in>(serverID, serveraddr));
	}
	//already have a timeMap and priority queue set up and currently empty
	//set all the times in timeMap to zero to start with

	//initialize queuelock, timelock, newcommandsignal, newmintimesignal
	pthread_mutex_init(&queueLock, NULL);
	pthread_mutex_init(&timeLock, NULL);

	pthread_cond_init(&queueSignal, NULL);
	pthread_cond_init(&timeSignal, NULL);

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

command getNextComand() {
	serverMessage message;
	pthread_mutex_lock(&queueLock);
	if (commandQueue.empty()) {
		 pthread_cond_wait(&queueSignal, &queueLock);
	}
	message = commandQueue.top();
		
	pthread_mutex_lock(&timeLock);
	while (!(message.time < minTime.second)) {
		pthread_mutex_unlock(&queueLock);
		pthread_cond_wait(&timeSignal, &timeLock);
		pthread_mutex_lock(&queueLock);
		message = commandQueue.top();
	}
	pthread_mutex_lock(&timeLock);
	commandQueue.pop();
	pthread_mutex_unlock(&queueLock);
	return message.clientCommand;
}

void processCommand(command& nextCommand, dataContainer& data) {
	clientAddr.sin_addr.s_addr = nextCommand.clientID;
	if (command.commandType == createInt) {
		if (data.ints.find(nextCommand.name) != data.ints.end()) {
			sendto(CPORT, &(-1), sizeof(int), 0, clientAddr, sizeof(clientAddr));
		}
		data.ints.insert(pair<int,int>(nextCommand.name, nextCommand.argument);
		sendto(CPORT, &(0), sizeof(int), 0, clientAddr, sizeof(clientAddr));
	}
	if (command.commandType == getInt) {
		if (data.ints.find(nextCommand.name) == data.ints.end()) {
			sendto(CPORT, &(INT_MIN), sizeof(int), 0, clientAddr, sizeof(clientAddr));
		}
		int returnVal = *data.ints.find(nextCommand.name);
		sendto(CPORT, &(returnVal), sizeof(int), 0, clientAddr, sizeof(clientAddr));
	}
	if (command.commandType == setInt) {
		if (data.ints.find(nextCommand.name) == data.ints.end()) {
			sendto(CPORT, &(-1), sizeof(int), 0, clientAddr, sizeof(clientAddr));
		}
		data.ints[nextCommand.name] = nextCommand.argument;
		sendto(CPORT, &(0), sizeof(int), 0, clientAddr, sizeof(clientAddr));
	}
	if (command.commandType == createBarrier) {
		if (data.barriers.find(nextCommand.name) != data.barriers.end()) {
			sendto(CPORT, &(-1), sizeof(int), 0, clientAddr, sizeof(clientAddr));
		}
		Barrier newBar(nextCommand.clientID, myID);
		data.barriers[nextCommand.name] = newBar;
		sendto(CPORT, &(0), sizeof(int), 0, clientAddr, sizeof(clientAddr));
	}
	if (command.commandType == wait_on_barrier) {
		if (data.barriers.find(nextCommand.name) == data.barriers.end()) {
			sendto(CPORT, &(-1), sizeof(int), 0, clientAddr, sizeof(clientAddr));
		}
		Barrier bar = data.barriers[nextCommand.name];
		bool needToNotify = bar.clientWait(nextCommand.clientID);
		if (needToNotify) {
			list<CID> clientsToNotify;
			bar.clientsToNotify(myID, clientsToNotify);
			for (auto& client : clientsToNotify) {
				clientAddr.sin_addr.s_addr = client;
				sendto(CPORT, &(0), sizeof(int), 0, clientAddr, sizeof(clientAddr));
			}
		}
	}
}

void processCommandsThread(void* args) {
	dataContainer data;
	for(;;) {
		command commandToProcess = getNextCommand();
		processCommand(commandToProcess);
	}
}

int main(int argc, char**argv) {
	//call init for all set up
	if (init("configFile") < 0) {
		exit(1);
	}
	//start all the threads
}



