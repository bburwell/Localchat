/**
 * Localchat
 * Ben Burwell
 *
 * Send online broadcast
 */

#include "localchat.h"
 
void send_online_broadcast() {
	int                  client_sock;        // Client socket descriptor
	struct sockaddr_in   server_addr;     // Server Internet address
	struct in_addr       server_ip_addr;  // Server IP Address
	int                  addr_leng;        // Internet address length
	int                  retrncode;         // Return code
	int                  sOptVal;         // Socket option value
	int                  sOptLen;         // Socket option length
	char                 out_temp[10];

	client_sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (client_sock < 0) {
		exit(-1);
	}

	server_addr.sin_family = AF_INET;                  // Address family to use
	server_addr.sin_port = htons(CMD_PORT);            // Port num to use
	server_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP); // IP for Broadcast
	sOptVal = 1;
	sOptLen = sizeof(int);
	setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, (void*)&sOptVal, sOptLen);
	
	strcpy(out_temp, "ONLINE");
	retrncode = sendto(client_sock, out_temp, (strlen(out_temp) + 1), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	return;
}