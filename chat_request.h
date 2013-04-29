/**
 * Localchat
 * Ben Burwell
 *
 * Send chat request
 */

void send_chat_request(char * username) {

	int i;
	char ip[16];

	pthread_mutex_lock(&peer_table_lock);

	for (i = 0; i < num_peers_in_table; i++) {
		if (strcmp(peers[i].username, username) == 0) {
			strcpy(ip, peers[i].ip);
		}
	}

	pthread_mutex_unlock(&peer_table_lock);

	struct sockaddr_in server_addr;
	char out_buf[4096];
	char in_buf[4096];
	int retcode;

	// create the socket
	client_s = socket(AF_INET, SOCK_STREAM, 0);

	if (client_s < 0) {
		printf("Network error, quitting... \n");
		exit(-1);
	}

	// fill in server info
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(CHAT_PORT);
	server_addr.sin_addr.s_addr = inet_addr(ip);

	retcode = connect(client_s, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if (retcode < 0) {
		printf("Network error, quitting... \n");
		exit(-1);
	}

	// send the session request
	strcpy(out_buf, "SESREQ");
	send(client_s, out_buf, strlen(out_buf), 0);

	retcode = recv(client_s, in_buf, sizeof(in_buf), 0);

	if (strcmp(in_buf, "SESANS:Y") == 0) {
		printf("Request accepted, type /q to leave. \n");

		// set my in_chat flag
		strcpy(in_chat, "Y");

		// keep looping until no more input
		int keep_going = 1;

		while (keep_going) {

			char inp[256];
			printf("chat> ");
			fgets(inp, 140, stdin);

			if (strcmp(inp, "/q\n") == 0) {

				// send quit message to peer
				strcpy(out_buf, "SESQ");
				send(client_s, out_buf, sizeof(out_buf), 0);
				close(client_s);

				// exit the loop
				keep_going = 0;

				// print message to user
				printf("Session closed. \n");

				// set my in_chat flag
				strcpy(in_chat, "N");

			} else {

				// send the message
				strcpy(out_buf, "SESMSG:");
				strcat(out_buf, inp);
				send(client_s, out_buf, sizeof(out_buf), 0);

				// testing...
				printf("%s \n", out_buf);

			}

		}

	} else {
		printf("Session declined. \n");
	}

	close(client_s);

	return;
}