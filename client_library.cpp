#include "client_library.hpp"
#include <cstdlib>

static list<SID> serverIDList;

SID nextServerID() {
	auto iter = serverIDList.begin();
	srand(time(NULL));
	int amountToAdvance = rand() % serverIDList.size(); 
	advance(iter, amountToAdvance);
	return *iter;
}

void init(string& configFileName) {
	parseConfig(serverIDList, configFileName);
	clientID = htonl(INADDR_ANY);

	// Set up server sock addr for 
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short) CPORT);
	for (SID serverID : serverIDList) {
		serveraddr.sin_addr.s_addr = serverID;
		idToSockaddr.insert(pair<SID, sockaddr_in>(serverID, serveraddr));
	}
}


int createLock(int lockNum) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = create_lock;
	commandToSend.name = lockNum;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return error;
}

int getLock(int lockNum) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = get_lock;
	commandToSend.name = lockNum;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return error;
}

int releaseLock(int lockNum) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = release_lock;
	commandToSend.name = lockNum;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return error;
}

int createInt(int intNum, int value) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = create_int;
	commandToSend.name = intNum;
	commandToSend.argument = value;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return error;
}

int getInt(int intNum) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = get_int;
	commandToSend.name = intNum;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int value = recieveIntFrom(CPORT);
	return value;
}

int setInt(int intNum, int value) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = set_int;
	commandToSend.name = intNum;
	commandToSend.argument = value;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return error;
}

int createBarrier(int barrierNum) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = create_barrier;
	commandToSend.name = intNum;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return value;
}

int waitOnBarrier(int barrierNum) {
	command commandToSend;
	commandToSend.serverId = nextServerID();
	sockaddr_in serveraddr = idToSockaddr[commandToSend.serverId];
	commandToSend.clientId = clientID;
	commandToSend.commandType = wait_on_barrier;
	commandToSend.name = intNum;
	sendTo(CPORT, &commandToSend, (sockaddr*)&serveraddr);
	int error = recieveIntFrom(CPORT);
	return value;
}
