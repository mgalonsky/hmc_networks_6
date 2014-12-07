/*
 * Header file declaring various functions a client using our distributed
 * system may want to use.  All calls are synchronous and hang until the 
 * server responds.
 */

#ifndef CLIENT_LIBRARY
#define CLIENT_LIBRARY

/*
 * Creates a lock of the specified number.  Returns 0 if succesful and -1 on
 * error.
 */
int createLock(int lockNum);

/* 
 * Asks to be granted the specified lock.  Returns -1 on error.
 */
int getLock(int lockNum);

/*
 * Releases the lock of the specified number.  Returns -1 on error.
 */
int releaseLock(int lockNum);

/*
 * Creates an integer with the given key and value.  Returns -1 on error.
 */
int createInt(int intNum, int value);

/*
 * Gets the value of the int with the given key.  Returns INT_MIN on error.
 */
int getInt(int intNum);

/*
 * Sets the value of the int.  Can't set to INT_MIN.  Returns -1 on error.
 */
int setInt(int intNum, int val);

/*
 * Signs up for a barrier.  Creates the barrier if necessary.
 */
void createBarrier(int barrierNum);

/*
 * Waits for the specified barrier to expire.
 */
void waitOnBarrier(int barrierNum);

#endif CLIENT_LIBRARY
