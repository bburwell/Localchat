/**
 * Localchat
 * Ben Burwell
 *
 * Status broadcast thread functionality
 */

void status_broadcast_once() {

	if (username == NULL) {
		return;
	}

	int                  client_sock;        // Client socket descriptor
  	struct sockaddr_in   server_addr;     // Server Internet address
  	struct in_addr       server_ip_addr;  // Server IP Address
  	int                  addr_leng;        // Internet address length
	int                  retrncode;         // Return code
	int                  sOptVal;         // Socket option value
	int                  sOptLen;         // Socket option length
	char                 out_buf[4096];   // Output buffer for data
	char                 in_buf[4096];    // Input buffer for data
	char	             out_temp[4096];
	  
	client_sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (client_sock < 0) {
		printf("Making socket failure \n");
		exit(-1);
	}

	server_addr.sin_family = AF_INET;                  // Address family to use
	server_addr.sin_port = htons(CMD_PORT);            // Port num to use
	server_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP); // IP for Broadcast
	sOptVal = 1;
	sOptLen = sizeof(int);
	setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, (void*)&sOptVal, sOptLen);
	
	strcpy(out_temp, "STATUS:");
	strcat(out_temp, username);
	strcat(out_temp, ":");
	strcat(out_temp, my_ip);
	strcat(out_temp, ":");
	strcat(out_temp, in_chat);
	
	retrncode = sendto(client_sock, out_temp, (strlen(out_temp) + 1), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	return;
}

void *status_broadcast(void *arg) {
	
	while (1) {
		status_broadcast_once();
		sleep(5);
	}
}

void start_status_thread() {

	pthread_t thread;

	if (pthread_create(&thread, NULL, status_broadcast, NULL) > 0) {
		printf("Error starting receive thread \n");
		exit(-1);
	}

	return;
}