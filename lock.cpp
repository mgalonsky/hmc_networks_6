#include "lock.hpp"

typedef pair<CID, SID> waiter;

Lock::Lock() { 
	holder = 0;
}

bool Lock::getLock(CID client, SID server) {
	if (holder == 0) {
		holder = client;
		return true;
	}
	waiting.push(pair<CID, SID>(client, server));
	return false;
}

SID Lock::releaseLock(CID client) {
	if (holder != client) {
		return 0;
	}
	holder = 0;
	if (waiting.empty()) {
		return 0;
	}
	pair<CID, SID> next = waiting.front();
	waiting.pop();
	holder = next.first;
	return next.second;
}

CID Lock::lockHolder() {
	return holder;
}
