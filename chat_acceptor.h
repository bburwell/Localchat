/**
 * Localchat
 * Ben Burwell
 *
 * Chat request accepting functionality
 */

void *receive_chat_messages(void *arg) {

	char   in_buf[GLOBAL_MSG_LENGTH];
	int    retcode;
	int    i;
	char   separators[4] = ":";
	char * tok;

	// set the timeout of the socket
	struct timeval tv;
	tv.tv_sec  = SESSION_TIMEOUT;
	tv.tv_usec = 0;
	setsockopt(client_s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

	retcode = recv(client_s, in_buf, sizeof(in_buf), 0);

	while (strcmp(in_buf, "SESQ") != 0) {

		if (retcode > 0) {

			char *message;
			message = strtok(in_buf, separators);
			if (strcmp(message, "SESMSG") == 0) {
				printf("\n----> %s", strtok(NULL, separators));
				printf("%s ", prompt);
				fflush(stdout);
			}

		} else {
			break;
		}

		strcpy(in_buf, "");

		retcode = recv(client_s, in_buf, sizeof(in_buf), 0);
	}

	// display appropriate error message
	if (retcode == -1) {
		printf("\n*** Session timed out. *** \n");
	} else {
		printf("\n*** Peer closed the session. *** \n");
	}

	// close the socket
	close(client_s);
	
	// update chat flag and broadcast
	strcpy(in_chat, "N");
	status_broadcast_once();

	// reset the prompt
	strcpy(prompt, ">");
	printf("%s ", prompt);
	fflush(stdout);

	return;
}

void *chat_acceptor(void *arg) {

	accepted_client = (int) arg;
	int  retcode;
	char buf_i[GLOBAL_MSG_LENGTH];
	char buf_o[GLOBAL_MSG_LENGTH];

	retcode = recv(accepted_client, buf_i, sizeof(buf_i), 0);
	
	if (strcmp(in_chat, "Y") == 0) {

		// if already in a chat, reply no and close
		strcpy(buf_o, "SESANS:N");
		send(accepted_client, buf_o, strlen(buf_o)+1, 0);
		close(accepted_client);

	} else {

		// not in a chat, ask the user
		
		printf("\n*** CHAT REQUEST *** \n");
		printf("Accept [y/n]? ");
		fflush(stdout);

		// update the request time
		time(&chat_requested_time);
		
		respond_to_chat_request = 1;
	}

	return;
}

void accept_callback_accept() {

	client_s = accepted_client;

	strcpy(out_buf, "SESANS:Y");
	send(client_s, out_buf, strlen(out_buf)+1, 0);

	char inp[256];

	// update the prompt
	strcpy(prompt, "chat>");

	// update chat flag and broadcast
	strcpy(in_chat, "Y");
	status_broadcast_once();

	// print info message
	printf("Entering chat mode. Type /q to exit. \n");

	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, receive_chat_messages, NULL);

	return;
}

void accept_callback_decline() {

	char buf[GLOBAL_MSG_LENGTH];

	strcpy(buf, "SESANS:N");
	send(accepted_client, buf, strlen(buf)+1, 0);
	close(accepted_client);

	return;
}

void *server_loop(void *arg) {
	int                 server_s;
	struct sockaddr_in  server_addr;
	struct sockaddr_in  client_addr;
	struct in_addr      client_ip_addr;
	socklen_t           addr_len;
	pthread_t           thread_id;
	int                 retcode;

	server_s = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(CHAT_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	retcode = bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	if (retcode < 0) {
		printf("Network error, quitting... \n");
		exit(-1);
	}

	listen(server_s, 100);

	while (1) {
		addr_len = sizeof(accepted_addr);

		accepted_client = accept(server_s, (struct sockaddr *)&accepted_addr, &addr_len);

		if (accepted_client == -1) {
			printf("Network error, quitting... \n");
			exit(-1);
		}

		pthread_create(&thread_id, NULL, chat_acceptor, (void *)accepted_client);
	}

	return;
}

void start_accepting_chat_requests() {
	pthread_t thread;
	pthread_create(&thread, NULL, server_loop, NULL);
}