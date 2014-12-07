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

//sleep wrapper to sleep in smaller units of time
void sleep(int sleep_time) {
	struct timespec spec;
	spec.tv_sec = sleep_time / 1000;
	spec.tv_nsec = (sleep_time % 1000) * 1000000;
	nanosleep(&spec, NULL);
}
