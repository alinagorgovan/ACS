#include "utils.h"


int main(int argc, char** argv) {

    if (argc < 3) {
       fprintf(stderr,"Usage %s server_address server_port\n", argv[0]);
       exit(0);
    }
    // Opening the TCP and UDP sockets.
    int socket_TCP = start_clientTCP(atoi(argv[2]), argv[1]);
    int socket_UDP = start_clientUDP();
    struct sockaddr_in udp_addr = get_udp_addr_client(atoi(argv[2]), argv[1]);

    // Create a set of sockets for using in select
    fd_set read_fds;
    // Create a temporary set
    fd_set tmp_fds;
    // Empty the two sets
    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);
    // Add the file descriptors (the connection sockets) into the set
    FD_SET(0, &read_fds);
    FD_SET(socket_TCP, &read_fds);
    FD_SET(socket_UDP, &read_fds);
    int fdmax;
    char buffer[BUFLEN];
    // Get the maximum value between the file descriptors
    if (socket_TCP > socket_UDP)
        fdmax = socket_TCP;
    else
        fdmax = socket_UDP;

    // Make the connection to the UDP socket
    if (connect(socket_UDP, (struct sockaddr*) &udp_addr, sizeof(udp_addr)) < 0) {
        printf("-10: ERROR on connect UDP.\n");
        exit(-1);
    }
    // Open the file for writting
    int id = getpid();
    sprintf(buffer, "client-%d.log", id);

    FILE* g = fopen(buffer, "w");
    if (g == NULL) {
        printf("-10 : Error on opening.\n");
        exit(-1);
    }
    int unlock_request = 0;
    char aux[BUFLEN];
    int last_card;
    int c;
    int open_session = 0;
    while(1){

        tmp_fds = read_fds;
        if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1) {
            printf("-10 : Error on select.\n");
            exit(-10);
        }
        if (FD_ISSET(0, &tmp_fds)) {
            // If something came from stdin
            memset(buffer, 0 , BUFLEN);
            fgets(buffer, BUFLEN - 1, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            fprintf(g, "%s", buffer);

            if (unlock_request == 1) {
                // if a previous unlock command was given and the password is expected
                memset(aux, 0, BUFLEN);
                strcpy(aux, buffer);
                // Send the card number and the pin received from stdin
                sprintf(buffer, "%d %s", last_card, aux);
                int c = sendto(socket_UDP, buffer, strlen(buffer), 0,
                    (struct sockaddr*) &udp_addr,sizeof(udp_addr));
				if(c < 0) {
					printf("-10: Error in sendto\n");
					return -1;
				}
                // reset flag
				unlock_request = 0;
            } else if (strstr(buffer, "unlock")) {
                // if an unlock command is received than set flag and send the
                // command an the card number to the server
                unlock_request = 1;
                sprintf(aux, "unlock %d", last_card);
                c = sendto(socket_UDP, aux, strlen(aux), 0,
                    (struct sockaddr*) &udp_addr, sizeof(udp_addr));
                if (c < 0) {
                    printf("IBANK> -10 : Eroare la apel sendto\n");
                    fprintf(g, "IBANK> -10 : Eroare la apel sendto\n");
                }
            } else if (strstr(buffer, "login") != NULL) {
                // if a login command was received check if the client is
                // connected to other user
                if (open_session == 1) {
                    // if other user is logged in return error code -2
                    memset(buffer, 0, BUFLEN);
                    get_error(-2, buffer);
                    printf("IBANK> %s\n", buffer);
					fprintf(g, "IBANK> %s\n", buffer);
                } else {
                    // Parse the input in order to update the last_card
                    memset(aux, 0, BUFLEN);
                    strcpy(aux, buffer);
					char* token = strtok(aux," ");
					token = strtok(NULL," ");
					last_card = atoi(token);
                    // send the login message to the server
                    c = send(socket_TCP, buffer, strlen(buffer), 0);
					if (c < 0) {
						printf("IBANK> -10 : Eroare la apel send\n");
						fprintf(g, "IBANK> -10 : Eroare la apel send\n");
					}
                }
            } else if (strstr(buffer, "quit") != NULL) {
                // if a quit command was received send it to the server and then close
                // the process
                c = send(socket_TCP, buffer , strlen(buffer), 0);
                if (c < 0) {
                    printf("IBANK> -10 : Eroare la apel send\n");
                    fprintf(g, "IBANK> -10 : Eroare la apel send\n");
                }
                return 0;
            } else if (strstr(buffer, "login") == NULL && open_session == 0) {
                // if there isn't a session opened and other command except for
                // login was received than return error code -1
                memset(buffer, 0, BUFLEN);
                get_error(-1, buffer);
                printf("IBANK> %s\n" , buffer);
                fprintf(g, "IBANK>%s\n", buffer);
            } else {
                // in any other cases just send the command to the server
                c = send(socket_TCP, buffer, strlen(buffer), 0);
                if (c < 0) {
                    printf("IBANK> -10 : Eroare la apel send\n");
                    fprintf(g, "IBANK> -10 : Eroare la apel send\n");
                }
            }
        }
        if (FD_ISSET(socket_TCP, &tmp_fds)) {
            // If a response came from the TCP socket
            memset(buffer, 0, BUFLEN);
            // Get the received message
            c = recv(socket_TCP, buffer, sizeof(buffer), 0);
            if (c < 0) {
                printf("IBANK> -10 : Eroare la apel recv\n");
                fprintf(g, "IBANK> -10 : Eroare la apel recv\n");
            } else if (c > 0) {
                if (strstr(buffer, "Welcome") != NULL) {
                    // Login was succesful and set the open_session flag
                    open_session = 1;
                } else if (strstr(buffer, "Deconectare") != NULL) {
                    // Logout was succesful and reset the open_session flag
                    open_session = 0;
                }
                // Just print the message from the server
                printf("IBANK> %s\n", buffer);
                fprintf(g, "IBANK> %s\n", buffer);
            } else if (c == 0) {
                // the server was shut down
				FD_CLR(socket_TCP, &read_fds);
				close(socket_TCP);
				return 0;
            }
        }
        if (FD_ISSET(socket_UDP, &tmp_fds)) {
            // If a response came from the UDP socket
            memset(buffer, 0, BUFLEN);
			unsigned int size = sizeof(udp_addr);
            // Get the message from the UDP socket
			c = recvfrom(socket_UDP, buffer, sizeof(buffer), 0,
                (struct sockaddr *) &udp_addr, &size);

            if (c < 0) {
				printf("UNLOCK> -10 : Eroare la apel recvfrom\n");
				fprintf(g, "UNLOCK> -10 : Eroare la apel recvfrom\n");
            } else {
                printf("UNLOCK> %s\n", buffer);
				fprintf(g, "UNLOCK> %s\n", buffer);

				if (strstr(buffer, "Trimite") != NULL) {
                    // The server processed the unlock request and waits for the password
					unlock_request = 1;
				} else {
                    // In any other case reset the unlock_request flag
					unlock_request = 0;
				}
            }
        }

    }
    close(socket_TCP);
    close(socket_UDP);
    return 0;

}
