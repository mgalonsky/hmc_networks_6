#include "utilities.hpp"

#ifndef
#define BARRIER

class Barrier {
	map<CID, pair<SID, bool>> data;
	
public:
	//Creates a new barrier
	Barrier(CID client, SID server);
	
	//Adds a client to the barrier
	void addClient(CID client, SID server);
	
	//Notifies the barrier that a client is waiting.  If all clients are
	//waiting, returns true, indicating that the server should figure out
	//which clients it needs to notify that the barrier is destroyed
	bool clientWait(CID client);

	//Finds out which clients the server needs to notify that the barrier
	//is destroyed
	void clientsToNotify(SID server, list<CID>& clients);
};

#endif //BARRIER
