/**
 * Localchat
 * Ben Burwell
 *
 * Process user input
 */

void process_user_command() {

	int i;

	if (strcmp(command, "quit\n") == 0) {
		exit(0);
	} else if (strcmp(command, "who\n") == 0) {

		// print a list of online peers
		pthread_mutex_lock(&peer_table_lock);

		printf("  +---------+-----------------+----------------------------------+ \n");
		printf("  | In Chat | IP              | Username                         | \n");
		printf("  +---------+-----------------+----------------------------------+ \n");

		for (i = 0; i < num_peers_in_table; i++) {
			printf("  |       %s | %-15s | %-32s | \n", peers[i].in_chat, peers[i].ip, peers[i].username);
		}

		printf("  +---------+-----------------+----------------------------------+ \n");

		pthread_mutex_unlock(&peer_table_lock);

	} else if (strcmp(command, "chat\n") == 0) {

		char user[32];
		int exists;

		printf("user: ");
		fgets(user, 32, stdin);

		user[strlen(user) - 1] = 0;

		exists = check_user_name(user);

		if (exists) {
			printf("Sending chat request to %s... \n", user);
			send_chat_request(user);
		} else {
			printf("%s does not exist. \n", user);
		}


	} else if (strcmp(command, "help\n") == 0 || strcmp(command, "?\n") == 0) {

		printf("Command Summary: \n");
		printf("  who:  display a list of online peers \n");
		printf("  chat: initiate a chat session \n");

	} else if (strcmp(command, "\n") == 0) {
		// no command, do nothing.
	} else {
		printf("Unrecognized command. Type `help` or `?` for help. \n");
	}
}