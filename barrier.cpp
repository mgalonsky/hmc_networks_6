#include "barrier.hpp"

typedef pair<SID, bool> ServBool;
typedef pair<CID, ServBool> elem;

Barrier::Barrier(CID client, SID server) {
	elem newElem = elem(client, ServBool(SID, false));
	dara.insert(newElem);
}

void Barrier::addClient(CIDclient, SID server) {
	elem newElem = elem(client, ServBool(SID, false));
	dara.insert(newElem);
}

bool Barrier::clientWait(CID client) {
	auto iter = data.find(client);
	client->second.second = true;
	//Return true if every registered client is now waiting
	bool allWaiting = true;
	for(auto& currPair : data) {
		if (data.second.second == false) {
			allWaiting = false;
			break;
		}
	}
	return allWaiting;
}

void clientsToNotify(SID server, list<CID>& clients) {
	for(auto& currPair : data) {
		if (currPair.second.first == server) {
			clients.push_back(currPair.first);
		}
	}
}
