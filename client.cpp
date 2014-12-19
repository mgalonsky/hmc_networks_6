/*
 * Helen Woodward and Melissa Galonsky
 * Code for the clients for a simple distributed system
 * for HMC CS125 lab 6
 */

#include "client_library.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

sockaddr_in protectedAddr;

void sendToProtectedServer(string message) {
	char buf[50];
	const char* cstr = message.c_str();
	strcpy(buf, cstr);
//	cerr << "message is:" << buf << endl;

	sendto(udpSock, (void*)&buf, sizeof(buf), 0, (sockaddr*)&protectedAddr, sizeof(protectedAddr));
}

int main(int argc, char**argv) {
	init("configFile", argv[1]); 
	//note that your id is in clientID and the udp socket is udpSock
	cerr << "finished init. My id is " << clientID << endl;

	//next set up the sockaddr for the protected server
	in_addr inaddr;
	protectedAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "10.99.0.9", &inaddr);
	protectedAddr.sin_addr = inaddr;
	protectedAddr.sin_port = htons((unsigned short) CPORT);
	//check in with protected server
	sendToProtectedServer("test message");

	//first create an int
	cerr << "going to test ints" << endl;
	if (createInt(500,0) < 0) {
		cerr << "error creating int with name 500" << endl;
		int test = getInt(500);
		if (test != 0) {
			cerr << "error in getInt" << endl;
		}
		setInt(500, 5);
		test = getInt(500);
		if (test != 5) {
			cerr << "error in setInt" << endl;
		}
	}
	else {
		cerr << "successfully created int 500" << endl;
	}
/*
	cerr << "lock creation test" << endl;
	if (createLock(42) < 0) {
		cerr << "error creating lock with name 42" << endl;
	}
	else {
		cerr << "successfully created lock 42" << endl;
	}
	//create a barrier
	if (createBarrier(0) < 0) {
		cerr << "error in barrier creation" << endl;
	}

	//now get the protected server lock
	getLock(42);
	//get the int and store the value locally
	int local = getInt(500);
	//send the ints value and your id to the protected server
	sendToProtectedServer("(myID here) got (intValue here)");
	//set the int to have a value one larger than the one you got
	setInt(500, local+1);
	//release the lock
	releaseLock(42);

	//wait on the barrier
	waitOnBarrier(0);
	//check in again with protected server
	sendToProtectedServer("(myID here) waiting on barreir");
	
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
	sendToProtectedServer("(int name) ended up with the value (temp)");
	//release protected server lock
	releaseLock(42);
*/	
	cerr << "all done" << endl;
	exit(0);
}
