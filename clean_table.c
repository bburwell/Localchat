/**
 * Localchat
 * Ben Burwell
 *
 * Table cleaning functions
 */

#include "localchat.h"
 
void *clean_table(void *arg) {

	int i;
	int j;
	time_t now;
	double diff;

	while (1) {

		pthread_mutex_lock(&peer_table_lock);
		
		time(&now);

		for (i = 0; i < num_peers_in_table; i++) {
			diff = difftime(now, peers[i].last_seen);
			if (diff > 10) {
				for (j = i; j < num_peers_in_table-1; j++) {
					peers[j] = peers[j+1];
				}
				num_peers_in_table--;
			}
		}

		pthread_mutex_unlock(&peer_table_lock);
		sleep(1);
	}

	return NULL;
}

void start_clean_table_thread() {

	pthread_t thread;
	pthread_mutex_init(&peer_table_lock, NULL);

	if (pthread_create(&thread, NULL, clean_table, NULL) > 0) {
		printf("Error starting clean_table thread\n");
		abort();
	}
}