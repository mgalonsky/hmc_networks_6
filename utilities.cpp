/*
 * Helen Woodward and Melissa Galonsky
 * Basic utilities for dealing with network traffic
 * for HMC CS125 lab 6
 */

#include "utilities.hpp"
#include <climits>
#include <stdio.h>
#include <sys/socket.h>

int setUpUdpSock(int port) {
	int udpSock;
	struct sockaddr_in clientaddr;
	//create the udp socket
	if ((udpSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		//error, die
		return -1;
	}
	//create the client address info
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientaddr.sin_port = htons((unsigned short) port);
	//bind the udp socket to address
	if (bind(udpSock, (sockaddr *)&clientaddr, sizeof(clientaddr)) < 0) {
		//error, die
		return -1;
	}
	return udpSock;
}

ssize_t sendTo(int socket, command data, sockaddr* addr) {
// just wrap the sendto function here
}

int recieveFrom(int socket) {
	int result;
	if (recvfrom(socket, (void*)&result, sizeof(result), 0, NULL, NULL) >= 0) {
		return result;
	}
	return INT_MIN;
}

size_t recieveFrom(int socket, command result) {
	return recvfrom(socket, (void*)&result, sizeof(result), 0, NULL, NULL);
}

//sleep wrapper to sleep in smaller units of time
void sleep(int sleep_time) {
	struct timespec spec;
	spec.tv_sec = sleep_time / 1000;
	spec.tv_nsec = (sleep_time % 1000) * 1000000;
	nanosleep(&spec, NULL);
}
