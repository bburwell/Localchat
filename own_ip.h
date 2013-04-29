/**
 * Localchat
 * Ben Burwell
 *
 * Gets host IP
 */

void get_own_ip() {

	char name_buf[256];
    unsigned int  buf_len = 256;
    struct hostent *host;
    int  ret_code;

    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *rptr;     // Pointer to  linked list.
    struct sockaddr_in *ipv4;
    void * addr;
    char   ipstr[64];

    // Get the name of the host.
    ret_code = gethostname(name_buf, buf_len);
    if (ret_code < 0) {
        exit(1);
    }
    
    if (strcmp(name_buf, "mathcs") == 0) {

        strcpy(my_ip, "192.168.130.10");

    } else {

        // Get the IP addresses of this host.
        memset((void *) &hints, 0, (size_t) sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = 0;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

        ret_code = getaddrinfo(name_buf, NULL, &hints, &result);
        if (ret_code != 0) {
            perror("Error from getaddrinfo\n");
            exit(1);
        }

        rptr = result;
        while (rptr != NULL) {
            ipv4 = (struct sockaddr_in *) rptr->ai_addr;
            addr = &(ipv4->sin_addr);
            inet_ntop(rptr->ai_family, addr, ipstr, sizeof(ipstr));

            rptr = rptr->ai_next;
        }

        strcpy(my_ip, ipstr);

        freeaddrinfo(result);  // free the linked list
    }

    return;
}