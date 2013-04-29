/**
 * Localchat
 * Ben Burwell
 *
 * Thread management functions
 */

void *receive_function(void *arg) {

	char                in_buf[256];
	struct sockaddr_in  server_addr;
	int                 addr_len;
	int                 client_sock = socket(AF_INET, SOCK_DGRAM, 0);
	int                 ret_code;

	if (client_sock < 0) {
		exit(-1);
	}

	server_addr.sin_family = AF_INET;                  // Address family to use
	server_addr.sin_port = htons(CMD_PORT);            // Port num to use
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any IP address
	ret_code = bind(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

	while (1) {

		addr_len = sizeof(server_addr);
		ret_code = recvfrom(client_sock, in_buf, sizeof(in_buf), 0, (struct sockaddr *)&server_addr, &addr_len);

		if (DEBUG) printf("Received from server: %s (%d) \n", in_buf, ret_code);
		parse_command(in_buf);
	}
	
	return;
}

void start_receive_thread() {
	
	pthread_t thread;

	if (pthread_create(&thread, NULL, receive_function, NULL) > 0) {
		printf("Error starting receive thread \n");
		exit(-1);
	}

	return;
}