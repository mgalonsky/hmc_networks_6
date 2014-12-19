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
#include "barrier.hpp"
#include "lock.hpp"
#include <arpa/inet.h>

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
	//set up serverMap
	list<SID> serverList;
	parseConfig(serverList, configFile);
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short) SPORT);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons((unsigned short) CPORT);
	for (SID serverID : serverList) {
		cerr << "serverID is:" << serverID << endl;
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

void* listenToClients(void* args) {
	command commandToRecieve;
	serverMessage message;
	//Loop forever listening for incoming commands
	while (recieveCommandFrom(CPORT, commandToRecieve)) {
		//Create a server mesasge
		message.time = time(NULL);
		message.source = myID;
		message.isPing = false;
		message.clientCommand = commandToRecieve;
		sendToServers(message);
	}
	return NULL;
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

void* queueManager(void* args) {
	serverMessage message;
	time_t oldTime;
	pair<SID, time_t> currMin;
	while (recvfrom(serverPort, (void*)&message, sizeof(message), 0, NULL, NULL)) {
		//process the time
		pthread_mutex_lock(&timeLock);
		oldTime = timeMap[message.source];
		if (oldTime < message.time) {
			timeMap[message.source] = message.time;
			if (minTime.first == message.source) {
				//update min time
				currMin = pair<SID, time_t>(message.source, message.time);
				for (auto &curr_pair : timeMap) {
					if (curr_pair.second < currMin.second) {
						currMin = curr_pair;
					}
				}
				minTime = currMin;
				pthread_cond_signal(&timeSignal);
			}
		}
		pthread_mutex_unlock(&timeLock);

		if(message.isPing == false) {
			//put command in the queue
			pthread_mutex_lock(&queueLock);
			commandQueue.push(message);
			pthread_cond_signal(&queueSignal);
			pthread_mutex_unlock(&queueLock);
		}
	}
	return nullptr;
}

command getNextCommand() {
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

void sendToClientIfNec(int* data, SID serverID) {
	if (serverID != myID) {
		return;
	}
	sendto(CPORT, data, sizeof(int), 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
}


void processCommand(command& nextCommand, dataContainer& data) {
	// SendToClientIfNec relies on this value being set here
	clientAddr.sin_addr.s_addr = nextCommand.clientId;

	int neg1 = -1;
	int zero = 0;
	int min = INT_MIN;
	if (nextCommand.type == create_int) {
		if (data.ints.find(nextCommand.name) != data.ints.end()) {
			sendToClientIfNec(&neg1, nextCommand.serverId);
		} else {
			data.ints.insert(pair<int,int>(nextCommand.name, nextCommand.argument));
			sendToClientIfNec(&zero, nextCommand.serverId);
		}
	}
	if (nextCommand.type == get_int) {
		if (data.ints.find(nextCommand.name) == data.ints.end()) {
			sendToClientIfNec(&min, nextCommand.serverId);
		} else {
			int returnVal = data.ints[nextCommand.name];
			sendToClientIfNec(&returnVal, nextCommand.serverId);
		}
	}
	if (nextCommand.type == set_int) {
		if (data.ints.find(nextCommand.name) == data.ints.end()) {
			sendToClientIfNec(&neg1, nextCommand.serverId);
		} else {
			data.ints[nextCommand.name] = nextCommand.argument;
			sendToClientIfNec(&zero, nextCommand.serverId);
		}
	}
	if (nextCommand.type == create_barrier) {
		if (data.barriers.find(nextCommand.name) == data.barriers.end()) {
			Barrier newBar(nextCommand.clientId, nextCommand.serverId);
			data.barriers[nextCommand.name] = newBar;
			sendToClientIfNec(&zero, nextCommand.serverId);
		} else {
			data.barriers[nextCommand.name].addClient(nextCommand.clientId, nextCommand.serverId);
			sendToClientIfNec(&zero, nextCommand.serverId);
		}
	}
	if (nextCommand.type == wait_on_barrier) {
		if (data.barriers.find(nextCommand.name) == data.barriers.end()) {
			sendToClientIfNec(&neg1, nextCommand.serverId);
		}
		Barrier bar = data.barriers[nextCommand.name];
		bool needToNotify = bar.clientWait(nextCommand.clientId);
		if (needToNotify) {
			list<CID> clientsToNotify;
			bar.clientsToNotify(myID, clientsToNotify);
			for (auto& client : clientsToNotify) {
				clientAddr.sin_addr.s_addr = client;
				sendto(CPORT, &zero, sizeof(int), 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
			}
		}
	}
	if (nextCommand.type == create_lock) {
		if (data.locks.find(nextCommand.name) != data.locks.end()) {
			sendToClientIfNec(&neg1, nextCommand.serverId);
		} else {
			Lock newLock;
			data.locks[nextCommand.name] = newLock;
			sendToClientIfNec(&zero, nextCommand.serverId);
		}
	} 
	if (nextCommand.type == get_lock) {
		if (data.locks.find(nextCommand.name) == data.locks.end()) {
			sendToClientIfNec(&neg1, nextCommand.serverId);
		} else {
			Lock& lockToProcess = data.locks[nextCommand.name];
			bool gotLock = lockToProcess.getLock(nextCommand.clientId, nextCommand.serverId);
			if (gotLock) {
				sendToClientIfNec(&zero, nextCommand.serverId);
			}
		}
	}
	if (nextCommand.type == release_lock) {
		if (data.locks.find(nextCommand.name) == data.locks.end()) {
			sendToClientIfNec(&neg1, nextCommand.serverId);
		} else {
			Lock& lockToProcess = data.locks[nextCommand.name];
			SID serverToGrantNext = lockToProcess.releaseLock(nextCommand.clientId);
			sendToClientIfNec(&zero, serverToGrantNext);
		}
	}
}

void* processCommandsThread(void* args) {
	dataContainer data;
	for(;;) {
		command commandToProcess = getNextCommand();
		processCommand(commandToProcess, data);
	}
	return nullptr;
}

int main(int argc, char**argv) {
	//call init for all set up
	if (init("configFile") < 0) {
		exit(1);
	}
	//set my id based on command line input
	in_addr serverIDwrapper;
	inet_pton(AF_INET, argv[1], &serverIDwrapper);
	myID = serverIDwrapper.s_addr;

	cerr << "my id is:" << myID << endl;
	//start all the threads
	pthread_t tid;
	pthread_create( &tid, NULL, pinger, NULL);
	pthread_create( &tid, NULL, processCommandsThread, NULL);
	pthread_create( &tid, NULL, queueManager, NULL);
	pthread_create( &tid, NULL, listenToClients, NULL);

	while (true) {
		sleep(990);
	}
}



