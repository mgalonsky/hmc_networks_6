#include "client_listener.hpp"
#include <ctime>

void* listenToClients(void* args) {
	command commandToRecieve;
	serverMessage message;
  	//Loop forever
	for(;;) {
		//Listens for an incoming command
		recieveCommandFrom(CPORT, commandToRecieve);
		
		//Create a server mesasge
		message.time = time(0);
		message.source = myID;
		message.isPing = false;
		message.ClientCommand = commandToRecieve;
		sendToServers(message);
	}

	
