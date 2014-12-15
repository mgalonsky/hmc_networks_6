/*
 * Helen Woodward and Melissa Galonsky
 * Code for the protected server for testing
 * for HMC CS125 lab 6
 */

#include "utilities.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char**argv) {
	int udpSock = setUpUdpSock(CPORT);
	int strlen;

	while(recvfrom(udpSock, (void*)&strlen, sizeof(strlen), 0, NULL, NULL) >= 0) {
		char* buf = new char[strlen];
		recvfrom(udpSock, (void*)&buf, sizeof(buf), 0, NULL, NULL);
		cout << buf << endl;
	}
}
