/**
 * Localchat
 * Ben Burwell
 *
 * Main Program
 */

// include necessary libraries
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

// define constants
#define DEBUG              0
#define MAX_NUM_PEERS      100
#define CMD_PORT           6062
#define CHAT_PORT          6063
#define BROADCAST_IP       "192.168.130.255"
#define GLOBAL_MSG_LENGTH  1024
#define REQUEST_TIMEOUT    10
#define SESSION_TIMEOUT    30
#define RR                 0

// structs
#include "peer.h"

// global variables
struct peer         peers[MAX_NUM_PEERS];
int                 num_peers_in_table = 0;
pthread_mutex_t     peer_table_lock;
     
// my flags               
char                my_ip[64];
char                in_chat[4] = "N";
const char *        username;

// command line vars
char                command[256];
char                prompt[16] = ">";

// client socket
int                 client_s;
char                in_buf[GLOBAL_MSG_LENGTH];
char                out_buf[GLOBAL_MSG_LENGTH];
struct sockaddr_in  client_addr;

// for accepting connections pending user approval
int                 accepted_client;
struct sockaddr_in  accepted_addr;

// indicates whether needs to respond to a request
int                 respond_to_chat_request = 0;

// include functions
#include "clean_table.h"
#include "online.h"
#include "own_ip.h"
#include "status.h"
#include "parse.h"
#include "receive.h"
#include "check_user_name.h"
#include "loading.h"
#include "chat_acceptor.h"
#include "chat_request.h"
#include "user_command.h"

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