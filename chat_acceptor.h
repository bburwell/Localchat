/**
 * Localchat
 * Ben Burwell
 *
 * Chat request accepting functionality
 */

void *receive_chat_messages(void *arg) {

	char in_buf[1024];
	int  length = 1024;
	int  retcode;

	retcode = recv(client_s, in_buf, length, 0);
	printf("%s \n", in_buf);

	while (strcmp(in_buf, "SESQ")) {
		retcode = recv(client_s, in_buf, length, 0);
		if (retcode > 0) {
			printf("%s \n", in_buf);
		}
	}

}

void *chat_acceptor(void *arg) {

	int                 client_s;
	char                in_buf[4096];
	char                out_buf[4096];
	int                 length = 4096;
	char                command[4096];
	int                 retcode;
	int                 i;
	char *              token;
	struct sockaddr_in  client_addr;

	client_s = (int) arg;

	retcode = recv(client_s, in_buf, length, 0);
	
	if (strcmp(in_chat, "Y") == 0) {

		// if already in a chat, reply no and close
		strcpy(out_buf, "SESANS:N");
		send(client_s, out_buf, strlen(out_buf)+1, 0);
		close(client_s);

	} else {

		// not in a chat, ask the user
		
		char ans[4];
		printf("\n*** CHAT REQUEST *** \n");
		printf("Accept [y/n]? ");
		fflush(stdout);
		fgets(ans, 2, stdin);

		if (strcmp(ans, "y") == 0) {
			strcpy(out_buf, "SESANS:Y");
			send(client_s, out_buf, strlen(out_buf)+1, 0);

			char inp[256];

			pthread_t recv_thread;
			pthread_create(&recv_thread, NULL, receive_chat_messages, NULL);

			while (strcmp(inp, "/q\n") != 0) {
				printf("chat> ");
				fgets(inp, 256, stdin);

				if (strcmp(inp, "/q\n") != 0) {
					strcpy(out_buf, "SESMSG:");
					strcat(out_buf, inp);
					send(client_s, out_buf, strlen(out_buf)+1, 0);
				} else {
					strcpy(out_buf, "SESQ");
					send(client_s, out_buf, strlen(out_buf)+1, 0);
				}

			}

		} else {
			strcpy(out_buf, "SESANS:N");
			send(client_s, out_buf, strlen(out_buf)+1, 0);
		}
	}

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