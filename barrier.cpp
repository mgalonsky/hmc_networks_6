#include "barrier.hpp"

typedef pair<SID, bool> ServBool;
typedef pair<CID, ServBool> elem;

Barrier::Barrier() {
	//Nothing to do
}

Barrier::Barrier(CID client, SID server) {
	elem newElem(client, ServBool(server, false));
	data.insert(newElem);
}

void Barrier::addClient(CID client, SID server) {
	elem newElem = elem(client, ServBool(server, false));
	data.insert(newElem);
}

bool Barrier::clientWait(CID client) {
	auto iter = data.find(client);
	iter->second.second = true;
	//Return true if every registered client is now waiting
	bool allWaiting = true;
	for(auto& currPair : data) {
		if (currPair.second.second == false) {
			allWaiting = false;
			break;
		}
	}
	return allWaiting;
}

void Barrier::clientsToNotify(SID server, list<CID>& clients) {
	for(auto& currPair : data) {
		if (currPair.second.first == server) {
			clients.push_back(currPair.first);
		}
	}
}
