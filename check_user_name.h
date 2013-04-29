/**
 * Localchat
 * Ben Burwell
 *
 * Checks whether a username is in use
 */

int check_user_name(const char * username) {

	int i;

	pthread_mutex_lock(&peer_table_lock);

	for (i = 0; i < num_peers_in_table; i++) {
		if (strcmp(peers[i].username, username) == 0) {

			pthread_mutex_unlock(&peer_table_lock);
			return 1;
			
		}
	}

	pthread_mutex_unlock(&peer_table_lock);

	return 0;

}