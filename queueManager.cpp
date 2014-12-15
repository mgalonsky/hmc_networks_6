void queueManager(void* args) {
	serverMessage message;
	time_t oldTime;
	pair<SID, time_t> currMin;
	while (recvfrom(serverPort, (void*)&message, sizeof(message), 0, NULL, NULL)) {
		//process the time
		pthread_mutex_lock(&timeLock);
		oldTime = timeMap[message.source];
		if (oldTime < message.time) {
			timeMap[message.source] = message.time;
			if (minTime.first == message.source) {
				//update min time
				currMin = pair<SID, time_t>(message.source, message.time);
				for (auto &curr_pair : timeMap) {
					if (curr_pair.second < currMin.second) {
						currMin = curr_pair;
					}
				}
				minTime = currMin;
				pthread_cond_signal(&timeSignal);
			}
		}
		pthread_mutex_unlock(&timeLock);

		if(!message.isPing) {
			//put command in the queue
			pthread_mutex_lock(&queueLock);
			commandQueue.push(message);
			pthread_mutex_signal(&queueSignal);
			pthread_mutex_unlock(&queueLock);
		}
	}
}
