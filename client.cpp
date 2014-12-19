/*
 * Helen Woodward and Melissa Galonsky
 * Code for the clients for a simple distributed system
 * for HMC CS125 lab 6
 */

#include "client_library.hpp"
#include <iostream>

using namespace std;

sockaddr_in protectedAddr;

void sendToProtectedServer(string message) {
	int len = message.length();
	const char* cstr = message.c_str();

	sendto(udpSock, (void*)&len, sizeof(len), 0, (sockaddr*)&protectedAddr, sizeof(protectedAddr));
	sendto(udpSock, (void*)cstr, sizeof(cstr), 0, (sockaddr*)&protectedAddr, sizeof(protectedAddr));
}

int main(int argc, char**argv) {
	init("configFile"); 
	//note that your id is in clientID and the udp socket is udpSock

	//next set up the sockaddr for the protected server
	protectedAddr.sin_family = AF_INET;
	protectedAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	protectedAddr.sin_port = htons((unsigned short) CPORT);

	//first create an int
	if (createInt(500,0) < 0) {
		cerr << "error creating int with name 500" << endl;
	}
	//then create a lock for the protected server
	if (createLock(42) < 0) {
		cerr << "error creating lock with name 42" << endl;
	}
	
	//create a barrier
	createBarrier(0);

	//now get the protected server lock
	getLock(42);
	//get the int and store the value locally
	int local = getInt(500);
	//send the ints value and your id to the protected server
	sendToProtectedServer("put a better string here");
	//set the int to have a value one larger than the one you got
	setInt(500, local+1);
	//release the lock
	releaseLock(42);

	//wait on the barrier
	waitOnBarrier(0);
	
	//try to wait on a barrier
	//confirm you get an error (thus the barrier is destroyed)
	if(waitOnBarrier(0) >= 0) {
		cerr << "was able to wait on barrier when it should have been destroyed!" << endl;
	}
	else {
		cerr << "barrier was destroyed when I woke up" << endl;
	}

	//create a barrier
	createBarrier(0);

	//create an int with value 0 and name the value you got earlier
	createInt(local, 0);
	//create a lock for the int (with the seame name)
	createLock(local);

	//wait on barrier
	waitOnBarrier(0);
	//create barrier
	createBarrier(0);

	//iterate over all the ints
	int numClients = 5;
	int temp;
	for (int i=0; i < numClients; i++) {
		//get the lock
		getLock(i);
		//increment that int
		temp = getInt(i);
		setInt(i, temp++);		
		//release the lock
		releaseLock(i);
	}
	//if you got an error in the loop above the barriers failed
	//wait on barrier
	waitOnBarrier(0);

	//get your ints value
	temp = getInt(local);
	//get the protected server lock
	getLock(42);
	//tell the protected server your id and your ints value (which should be the number of clients)
	sendToProtectedServer("put a better string here");
	//release protected server lock
	releaseLock(42);
	//all done
	exit(0);
}