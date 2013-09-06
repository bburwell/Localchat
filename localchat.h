// Localchat
// 
// Header file
// Ben Burwell

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
#define CMD_PORT           6060
#define CHAT_PORT          6061
#define BROADCAST_IP       "192.168.130.255"
#define GLOBAL_MSG_LENGTH  1024
#define REQUEST_TIMEOUT    5
#define SESSION_TIMEOUT    30
#define RR                 0

// peer structure
struct peer {
	char username[32];
	char ip[16];
	char in_chat[2];
	time_t last_seen;
};

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

// for timeout of chat requests
time_t              chat_requested_time;

// clean table function
void *clean_table(void *arg);
void start_clean_table_thread();

// send online broadcast
void send_online_broadcast();

// get own ip
void get_own_ip();

// status broadcast
void status_broadcast_once();
void *status_broadcast(void *arg);
void start_status_thread();

// parsing
void parse_command(char packet[256]);

// receive
void *receive_function(void *arg);
void start_receive_thread();

// username checking
int check_user_name(const char * username);

// loading
void *rrf(void * arg);
void loading();

// chat acceptor
void *receive_chat_messages(void *arg);
void *chat_acceptor(void *arg);
void accept_callback_accept();
void accept_callback_decline();
void *server_loop(void *arg);
void start_accepting_chat_requests();

// chat request
void send_chat_request(char * username);

// user command
void process_user_command();


