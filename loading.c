/**
 * Localchat
 * Ben Burwell
 *
 * Loading display
 */

#include "localchat.h"
 
void *rrf(void * arg) {

	// wait predetermined interval
	sleep(RR);

	// print lyrics
	printf("\n  Never gonna give you up \n");
	printf("  Never gonna let you down \n");
	printf("  Never gonna run around and desert you. \n");

	// display prompt
	printf("%s ", prompt);
	fflush(stdout);

	return;

}

void loading() {

	printf("  Loading... 3");
	fflush(stdout);
	sleep(1);

	printf("\r  Loading... 2");
	fflush(stdout);
	sleep(1);

	printf("\r  Loading... 1");
	fflush(stdout);
	sleep(1);

	printf("\r  Loading... Done.\n\n");

	if (RR) {
		pthread_t rrt;
		pthread_create(&rrt, NULL, rrf, NULL);
	}

}