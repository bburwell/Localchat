/**
 * Localchat
 * Ben Burwell
 *
 * Peer structure
 */

struct peer {
	char username[32];
	char ip[16];
	char in_chat[2];
	time_t last_seen;
};