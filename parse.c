/**
 * Localchat
 * Ben Burwell
 *
 * Parse received command packets
 */

#include "localchat.h"
 
void parse_command(char packet[256]) {
	
	char separators[4] = ":";
	char *type;
	char *peer_username;
	char *peer_ip;
	char *peer_in_chat;
	int updated_peer;
	int i;

	type = strtok(packet, separators);

	if (strcmp(type, "ONLINE") == 0) {

		// send a status broadcast
		status_broadcast_once();

	} else if (strcmp(type, "STATUS") == 0) {

		// lock the peers
		pthread_mutex_lock(&peer_table_lock);

		// get username from packet
		peer_username = strtok(NULL, separators);
		peer_ip = strtok(NULL, separators);
		peer_in_chat = strtok(NULL, separators);

		// so we know whether to add a peer later
		updated_peer = 0;

		for (i = 0; i < num_peers_in_table; i++) {
			if (strcmp(peer_username, peers[i].username) == 0) {

				// found a matching username, update the peer
				strcpy(peers[i].in_chat, peer_in_chat);
				time( &(peers[i].last_seen) );

				// we updated the peers, so we won't add it later
				updated_peer = 1;

			}
		}

		if (!updated_peer) {
			// we didn't update a peer, so we need to add it to the table
			
			strcpy(peers[num_peers_in_table].username, peer_username);
			strcpy(peers[num_peers_in_table].ip, peer_ip);
			strcpy(peers[num_peers_in_table].in_chat, peer_in_chat);
			time( &(peers[num_peers_in_table].last_seen) );

			num_peers_in_table++;

			//printf("*** %s has come online. \n", peer_username);

		}

		pthread_mutex_unlock(&peer_table_lock);

	} else {
		if (DEBUG) printf("Unknown message type: %s \n", type);
	}

}