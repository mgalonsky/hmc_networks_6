#include "utilities.hpp"

#ifndef LOCK
#define LOCK

class Lock {
	CID holder;
	queue<pair<CID, SID>> waiting;

	public: 
		//Creates a new lock
		Lock();

		//try and grab the lock
		//returns true if the lock was immediately obtained
		//returns false if the client was put int the waiting queue
		bool getLock(CID client);
		
		//release the lock
		//this returns the SID of the server responsible for the client from waiting which was granted the lock
		//if the client does not hold the lock then this does nothing and returns the same SID as if there was no one waiting
		SID releaseLock(CID client);

		//getter for holder
		CID lockHolder();

#endif
