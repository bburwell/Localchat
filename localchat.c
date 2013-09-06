/**
 * Localchat
 * Ben Burwell
 *
 * Main Program
 */

#include "localchat.h"

int main(int argc, char const *argv[]) {

	if (argc != 2) {
		printf("Usage: %s [username]\n", argv[0]);
		return -1;
	}

	// figure out IP address
	get_own_ip();

	// start some threads we need
	start_receive_thread();
	start_clean_table_thread();

	// send an ONLINE message
	send_online_broadcast();

	// wait for responses
	loading();

	// check that the username they have requested
	// is not already in use
	if (check_user_name(argv[1])) {
		printf("Sorry, the username you selected is already in use. \n");
		printf("Please choose another. \n\n");
		exit(1);
	}

	// now we can set the username field
	username = argv[1];

	// start broadcasting statuses
	start_status_thread();

	// we can accept chats
	start_accepting_chat_requests();

	// enter the user input loop
	while (1) {
		printf("%s ", prompt);
		fgets(command, 256, stdin);
		process_user_command();
	}

	return 0;
}
