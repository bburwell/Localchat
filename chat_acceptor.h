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

	retcode = recv(client_s, in_buf, sizeof(in_buf), 0);

	while (strcmp(in_buf, "SESQ") != 0) {

		if (retcode > 0) {

			char *message;
			message = strtok(in_buf, separators);
			if (strcmp(message, "SESMSG") == 0) {
				printf("\n     > %s", strtok(NULL, separators));
			}

		}
		retcode = recv(client_s, in_buf, sizeof(in_buf), 0);
	}

	printf("Peer closed the session. >\r\n  > ");
	printf(" > ");

	close(client_s);
	strcpy(in_chat, "N");
	strcpy(prompt, ">");

	return;
}

void *chat_acceptor(void *arg) {

	accepted_client = (int)arg;

	retcode = recv(accepted_client, accept_in, sizeof(accept_in), 0);
	
	if (strcmp(in_chat, "Y") == 0) {

		// if already in a chat, reply no and close
		strcpy(accept_out, "SESANS:N");
		send(accepted_client, accept_out, strlen(accept_out)+1, 0);
		close(accepted_client);

	} else {

		// not in a chat, ask the user
		
		printf("\n*** CHAT REQUEST *** \n");
		printf("Accept [y/n]? ");
		fflush(stdout);
		
		respond_to_chat_request = 1;
	}

	return;
}

void accept_callback_accept() {

	client_s = accepted_client;

	strcpy(out_buf, "SESANS:Y");
	send(client_s, out_buf, strlen(out_buf)+1, 0);

	char inp[256];

	strcpy(prompt, "chat>");
	strcpy(in_chat, "Y");

	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, receive_chat_messages, NULL);

	return;
}

void accept_callback_decline() {

	strcpy(out_buf, "SESANS:N");
	send(client_s, out_buf, strlen(out_buf)+1, 0);
	close(client_s);

	return;
}

void *server_loop(void *arg) {
	int server_s;
	struct sockaddr_in server_addr;
	int client_s;
	struct sockaddr_in client_addr;
	struct in_addr client_ip_addr;
	socklen_t addr_len;
	pthread_t thread_id;
	int retcode;

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
		addr_len = sizeof(client_addr);

		client_s = accept(server_s, (struct sockaddr *)&client_addr, &addr_len);

		if (client_s == -1) {
			printf("Network error, quitting... \n");
			exit(-1);
		}

		pthread_create(&thread_id, NULL, chat_acceptor, (void *)client_s);
	}

	return;
}

void start_accepting_chat_requests() {
	pthread_t thread;
	pthread_create(&thread, NULL, server_loop, NULL);
}