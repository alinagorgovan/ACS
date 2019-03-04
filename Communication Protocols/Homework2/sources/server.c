#include "utils.h"

int last_card = 0;

/* This funcion processes a login request. */
char* login(char *aux, int id) {
    // Parse the input and get the card number and the pin
    char *token;
    token = strtok(aux, " ");
    token = strtok(NULL, " ");
    int card_number = atoi(token);
    token = strtok(NULL, " ");
    int pin = atoi(token);

    // Reset the count if there have been previous attempts to login
    if (last_card != 0 && card_number != last_card) {
        User* user = get_user(last_card);
        user->count = 0;
    }
    char *response = (char*)calloc(BUFLEN, sizeof(char));
    // Search the user that has the received card number
    User* user = get_user(card_number);
    if (user == NULL) {
        // If there isn't a user in the system with that card_number
        // return error code -4
        get_error(-4, response);
        return response;
    }
    // update last card
    last_card = user->card_number;
    if (user->logged_in == 1) {
        // if the user is already logged in from other client
        // return error code -2
        get_error(-2, response);
    } else if (user->blocked == 1) {
        // if the user is blocked
        // return error code -5
        get_error(-5, response);
    } else {

        if (user->pin == pin) {
            // if the pin is the right one then log in the user
            sprintf(response,"Welcome %s %s!", user->name, user->surname);
            user->logged_in = 1;
            user->count = 0;
            // set the client id
            user->client_id = id;
        } else {
            //if the pin is wrong than update the count
            user->count++;
            // if there have been three attempts than block the user
            if(user->count == 3) {
                user->blocked = 1;
                user->count = 0;
                get_error(-5, response);
            // else return error code -3
            } else {
                get_error(-3, response);
            }
        }
    }
    return response;
}
/* This funcion processes a transfer request. */
void transfer(char* aux, int socket) {
    // Parse the input and get the card number and the sum
    char *token;
    token = strtok(aux, " ");
    token = strtok(NULL, " ");
    int card_number = atoi(token);
    token = strtok(NULL, " ");
    double suma = atof(token);

    char *response = (char*)calloc(BUFLEN, sizeof(char));

    // Get the receiver and the sender account to process the transfer
    User* receiver = get_user(card_number);
    User* sender = get_client(socket);
    // if the receiver account doesn't exist than return error code -4
    if (receiver == NULL) {
        get_error(-4, response);
    }
    else if (sender->sold >= suma) {
        // if the sender has enough money than ask for confirmation
        sprintf(response, "Transfer %.2lf catre %s %s ? [y/n]", suma, receiver->name, receiver->surname);
    } else {
        // else return error code -8
        get_error(-8, response);
    }

    send(socket, response, BUFLEN, 0);

    if (strstr(response, "Transfer") != NULL) {
        // If the transfer has been processed and the confirmation is expected
        memset(aux, 0, BUFLEN);
        memset(response, 0, BUFLEN);
        int nb = recv(socket, aux, sizeof(aux), 0);
        if (nb <= 0) {
            printf("-10 : Eroare la apelul functiei recv\n");
        } else {
            token = strtok(aux, " \n\r\t");
            if (strcmp(aux, "y") == 0) {
                // if the confimation is received
                sender->sold -= suma;
                receiver->sold += suma;
                strcpy(response, "Transfer realizat cu succes.");
            } else {
                get_error(-9, response);
            }
        }
        send(socket, response, BUFLEN, 0);
    }
}
/* This funcion closes a client. */
void close_client(int socket) {
    // search the user by the client id
    User* user = get_client(socket);
    if (user != NULL) {
        user->logged_in = 0;
        user->client_id = -1;
    }
    printf("S-a inchis clientul %d.\n", socket);
    close(socket);
}

int main(int argc, char** argv) {

    if (argc < 3) {
        fprintf(stderr,"Usage : %s port\n", argv[0]);
        exit(-1);
    }


    read_users_data(argv[2]);

    // Opening the TCP and UDP sockets.
    int serverfd_TCP = start_serverTCP(atoi(argv[1]));
    int serverfd_UDP = start_serverUDP();
    struct sockaddr_in udp_addr = get_udp_addr_server(atoi(argv[1]), serverfd_UDP);

    // Create a set of sockets for using in select
    fd_set read_fds;
    // Create a temporary set
    fd_set tmp_fds;
    int fdmax = 0;
    // Empty the two sets
    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);
    // Add the file descriptors (the connection sockets) into the set
    FD_SET(0, &read_fds);
    FD_SET(serverfd_TCP, &read_fds);
    FD_SET(serverfd_UDP, &read_fds);

    struct sockaddr_in cli_addr;

    // Buffers for receiving and sending messages
    char buffer[BUFLEN], buf_sent[BUFLEN], aux[BUFLEN];

    // Get the maximum value between the file descriptors
    if (serverfd_UDP > serverfd_TCP)
        fdmax = serverfd_UDP;
    else
        fdmax = serverfd_TCP;
    int c;
    int card_number;
    int exit_flag = 0;
    // Wait for messages as long as no quit was received
    while (exit_flag == 0) {
		tmp_fds = read_fds;
		if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1) {
			printf("-10 : Eroare la apelul functiei select.");
            exit(-1);
        }
        // Clean the buffers
        memset(buffer, 0, BUFLEN);
        memset(buf_sent, 0, BUFLEN);
        memset(aux, 0, BUFLEN);
		for (int i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {

				if (i == serverfd_TCP) {
                    // Something came through the tcp socket
                    // A new connection is made
                    unsigned int clilen = sizeof(cli_addr);
                    // The new client is accepted by the server
                    int client_socket = accept(serverfd_TCP, (struct sockaddr *)&cli_addr, &clilen);
                    if (client_socket < -1) {
                        printf("-10 : Eroare la apelul functiei accept.");
                    } else {
                        // The new file descriptor is added in the fds set
                        FD_SET(client_socket, &read_fds);
                        // The fdmax is updated
						if (client_socket > fdmax ) {
							fdmax = client_socket;
						}
                    }
                    printf("S-a conectat clientul %d.\n", client_socket);
				} else if (i == serverfd_UDP) {
                    // something cam through the UDP socket
                    memset(buffer, 0, BUFLEN);
                    memset(buf_sent, 0, BUFLEN);
                    memset(aux, 0, BUFLEN);
                    unsigned int size = sizeof(udp_addr);
                    // Get the buffer received
                    c = recvfrom(serverfd_UDP, buffer, BUFLEN, 0,
                        (struct sockaddr*) &udp_addr, &size);
					if(c < 0) {
						printf("-10: Eroare la apelul functiei recvfrom\n");
						exit(-1);
                    }
                    strcpy(aux, buffer);
                    char *token;
                    // If the buffer contains the unlock string
                    if (strstr(buffer, "unlock") != NULL) {
                        // The string is parsed to get the card number
                        // for the unlocking process
                        token = strtok(aux, " ");
                        token = strtok(NULL, " ");
                        card_number = atoi(token);
                        // search the user with that card number
                        User* user = get_user(card_number);
                        if (user == NULL) {
                            // if the user was not found in the system
                            // return error code -4
                            get_error(-4, buf_sent);
                        } else if (user->blocked == 0) {
                            // if the user is not blocked
                            // return error code -6
                            get_error(-6, buf_sent);
                        } else {
                            // ask for password
                            strcpy(buf_sent, "Trimite parola secreta.");
                        }
                        // send the right message
                        c = sendto(serverfd_UDP, buf_sent, strlen(buf_sent), 0,
                            (struct sockaddr*) &udp_addr, sizeof(udp_addr));
                        if (c < 0) {
                            printf("-10 : Eroare la apelul functiei sendto\n");
                        }
                    } else {
                        // if no "unlock" string was found than the received
                        // buffer contains the password
                        // Parse to get the card number and the password
                        token = strtok(aux, " \n");
                        card_number = atoi(token);
                        token = strtok(NULL, " \n");
                        // Search the user with the given card number
                        User* user = get_user(card_number);
                        if (user == NULL) {
                            get_error(-4, buf_sent);
                        } else if (user->blocked == 0) {
                            get_error(-6, buf_sent);
                        } else {
                            // If the passwords match than unlock the card
                            if (strcmp(user->password, token) == 0) {
                                user->blocked = 0;
                                strcpy(buf_sent, "Card deblocat");
                            } else {
                                get_error(-7, buf_sent);
                            }
                        }
                        // Send the right message
                        c = sendto(serverfd_UDP, buf_sent, strlen(buf_sent), 0,
                            (struct sockaddr*) &udp_addr, sizeof(udp_addr));
                        if (c < 0) {
                            printf("-10 : Eroare la apelul functiei sendto\n");
                        }
                    }
				} else if (i == 0) {
                    // If something came from the stdin
                    memset(buffer, 0, BUFLEN);
                    fgets(buffer, BUFLEN, stdin);
                    buffer[strlen(buffer) - 1] = '\0';
                    // If the received string is "quit" than close all the clients
                    // and send a closing message
                    if (strstr(buffer, "quit") != NULL) {
                        for (int j = 1; j <= fdmax; j++) {
                            if (FD_ISSET(j, &read_fds) && j != serverfd_UDP && j != serverfd_TCP) {

                                strcpy(buffer, "Serverul se inchide.");
                                printf("Serverul se inchide.\n");
                                c = send(j, buffer, sizeof(buffer), 0);
                                if (c < 0) {
                                    printf("-10: Eroare la apelul functiei send\n");
									exit(-1);
                                }
                                close(j);
                            }
                        }
                        // Set the exit flag for stopping the server
                        exit_flag = 1;
                    }
                } else {
                    // If something came from a client socket
                    memset(buffer, 0, BUFLEN);
                    // Get the received message
                    c = recv(i, buffer, BUFLEN, 0);
					if (c < 0) {
						printf("-10 : Eroare la apelul functiei recv\n");
						return -1;
					} else if ( c == 0) {
                        // The client was shut down
                        close_client(i);
                        FD_CLR(i, &read_fds);
                    }
                    strcpy(aux, buffer);
                    if (strstr(buffer, "login") != NULL) {
                        // if a user tries to login
                        char *b = login(aux, i);
                        send(i, b, strlen(b), 0);

                    } else if (strstr(buffer, "logout") != NULL) {
                        // if a logout command is received
                        // search the user where the client is connected
                        User* user = get_client(i);
                        // set the fields
                        user->logged_in = 0;
                        user->client_id = -1;
                        // Send the response to the client
                        strcpy(buf_sent, "Deconectare de la bancomat.");
                        send(i, buf_sent, strlen(buf_sent), 0);

                    } else if (strstr(buffer, "listsold") != NULL) {
                        memset(buf_sent, 0, BUFLEN);
                        // search the user where the client is connected
                        User* user = get_client(i);
                        sprintf(buf_sent, "%.2lf", user->sold);
                        // Send the response to the client
                        send(i, buf_sent, strlen(buf_sent), 0);

                    } else if (strstr(buffer, "transfer") != NULL) {
                        transfer(aux, i);
                    } else if (strstr(buffer, "quit") != NULL) {
                        close_client(i);
                        FD_CLR(i, &read_fds);
                    }

                }
			}
		}
     }

     close(serverfd_TCP);
     close(serverfd_UDP);
     return 0;
}
