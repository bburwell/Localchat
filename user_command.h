/**
 * Localchat
 * Ben Burwell
 *
 * Process user input
 */

void process_user_command() {

	int i;

	if (respond_to_chat_request) {

		// reset the global variable
		respond_to_chat_request = 0;

		// first check that we haven't expired
		time_t now;
		time(&now);

		double diff = difftime(now, chat_requested_time);

		if (diff > REQUEST_TIMEOUT) {

			printf("The request has timed out. \n");

		} else {

			if (strcmp(command, "y\n") == 0) {
				accept_callback_accept();
			} else {
				accept_callback_decline();
			}

		}

		return;
	}

	if (strcmp(in_chat, "Y") == 0) {

		// in chat, send input to the peer
		
		if (strcmp(command, "/q\n") == 0) {

			// send the quit message
			strcpy(out_buf, "SESQ");
			send(client_s, out_buf, strlen(out_buf)+1, 0);

			// close the socket
			close(client_s);

			// set my flag
			strcpy(in_chat, "N");
			status_broadcast_once();

			// reset prompt
			strcpy(prompt, ">");

		} else {

			strcpy(out_buf, "SESMSG:");
			strcat(out_buf, command);

			out_buf[strlen(out_buf)-1] = '\0';

			send(client_s, out_buf, strlen(out_buf)+1, 0);

		}

		return;

	}

	if (strcmp(command, "quit\n") == 0 || strcmp(command, "q\n") == 0) {
		exit(0);
	} else if (strcmp(command, "who\n") == 0 || strcmp(command, "w\n") == 0) {

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

	} else if (strcmp(command, "chat\n") == 0 || strcmp(command, "c\n") == 0) {

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


	} else if (strcmp(command, "ip\n") == 0) {

		char user[32];

		printf("user: ");
		fgets(user, 32, stdin);

		user[strlen(user) - 1] = 0;

		get_ip_by_username(user);

	} else if (strcmp(command, "help\n") == 0 || strcmp(command, "?\n") == 0) {

		printf("Command Summary: \n");
		printf("  who:  display a list of online peers \n");
		printf("  chat: initiate a chat session \n");
		printf("  quit: exit the localchat program \n");
		printf("  ip:   display the IP address of a user \n");

	} else if (strcmp(command, "\n") == 0) {
		// no command, do nothing.
	} else {
		printf("Unrecognized command. Type `help` or `?` for help. \n");
	}
}