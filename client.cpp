/*
 * Helen Woodward and Melissa Galonsky
 * Code for the clients for a simple distributed system
 * for HMC CS125 lab 6
 */

#include "client_library.hpp"

using namespace std;

sockaddr_in protectedSock;

sendToProtectedServer(string message) {
	//take a string and send it to the protected server
	//first convert the string to a char array
	//then send a udp packet with the size of that array
	//then send a udp packet with that array
}

int main(int argc, char**argv) {
	//first init yourself
	//note that your id is in clientID
	//next set up the sockaddr for the protected server

	//first create an int
	//if you got an error, say so
	//then create a lock for the protected server
	//if you got an error, say so
	
	//create a barrier

	//now get the protected server lock
	//get the int and store the value locally
	//send the ints value and your id to the protected server
	//set the int to have a value one larger than the one you got
	//release the lock

	//wait on the barrier
	
	//try to wait on a barrier
		//confirm you get an error (thus the barrier is destroyed)

	//create a barrier

	//create an int with value 0 and name the value you got earlier
	//create a lock for the int (with the seame name)

	//wait on barrier
	//create barrier

	//iterate over all the ints
		//get the lock
		//set them to your value
		//release the lock
	//if you got an error in the loop above the barriers failed
	//wait on barrier

	//get your ints value
	//get the protected server lock
	//tell teh protected server your id and your ints value (which should be the number of clients)
	//release protected server lock

	//all done
}
