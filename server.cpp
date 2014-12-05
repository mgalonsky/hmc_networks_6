/*
 * Helen Woodward and Melissa Galonsky
 * Code for the servers for a simple distributed system
 * for HMC CS125 lab 6
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/stat.h>

int main(int argc, char **argv) {

}
//set up a udpSock for both listening to and sending out of
int setUpUdpSock() {
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
	clientaddr.sin_port = htons((unsigned short) UDPPORT);
	//bind the udp socket to address
	if (bind(udpSock, (sockaddr *)&clientaddr, sizeof(clientaddr)) < 0) {
		//error, die
		return -1;
	}
	return udpSock;
}
//how to send to a udp socket set up as in setUpUdpSock
ssize_t sendToClient( const void* buf, const size_t len) {
	return sendto( udpSock, buf, len, 0, (struct sockaddr*)&client_addr, client_size);
}
/*
 * This is how you read out of a udpSocket, set up as in setUpUdpSock, and sent to as in sendToClient()
	recvfrom( udpSock, (void*)&pack, sizeof(pack), 0, NULL, NULL) >= 0
*/

//sleep wrapper to sleep in smaller units of time
void sleep(int sleep_time) {
	struct timespec spec;
	spec.tv_sec = sleep_time / 1000;
	spec.tv_nsec = (sleep_time % 1000) * 1000000;
	nanosleep(&spec, NULL);
}
