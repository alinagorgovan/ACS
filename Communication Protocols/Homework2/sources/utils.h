#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
/* User's data */
typedef struct {
    char name[12];
    char surname[12];
    int card_number;
    int pin;
    char password[8];
    double sold;
    int logged_in;
    int blocked;
    int count;
    int client_id;
} User;

#define BUFLEN 256
#define MAXCLIENTS 10
int number_of_clients;
/* All users from the system */
User **users;

/* Creates a new User structure */
User* new_user(char* name, char* surname, int card_number, int pin,
                                            char* password, double sold) {
    User *user = (User*)malloc(sizeof(User));
    strcpy(user->name, name);
    strcpy(user->surname, surname);
    user->card_number = card_number;
    user->pin = pin;
    strcpy(user->password, password);
    user->sold = sold;
    user->logged_in = 0;
    user->blocked = 0;
    user->count = 0;
    user->client_id = -1;

    return user;
}

/* Reads users' data from the input file */
void read_users_data(char* filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return;
    }

    int users_number;
    fscanf(f, "%d", &users_number);
    users = (User**)malloc(users_number * sizeof(User*));

    char name[12], surname[12], password[8];
    int card_number, pin;
    double sold;

    for(int i = 0; i < users_number; i++) {
        fscanf(f, "%s %s %d %d %s %lf", name, surname, &card_number, &pin, password, &sold);
        users[i] = new_user(name, surname, card_number, pin, password, sold);
    }
    number_of_clients = users_number;
    fclose(f);
}
/* This function opens a new TCP socket on the given port that will listen
   to the maximum MAXCLIENTS clients. */
int start_serverTCP(int portno) {

    struct sockaddr_in serv_addr;

    int ssocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (ssocketfd < 0) {
        printf("-10 : Eroare la apelul functiei socket.");
        exit(-1);
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(ssocketfd, (struct sockaddr *) &serv_addr,
        sizeof(struct sockaddr)) < 0) {
        printf("-10 : Eroare la apelul functiei bind.\n");
        exit(-1);
    }

    listen(ssocketfd, MAXCLIENTS);

    return ssocketfd;
}
/* This function opens a new UDP socket and returns its descriptor. */
int start_serverUDP() {

	int fds = socket(AF_INET, SOCK_DGRAM, 0);
    if (fds < 0) {
        printf("-10 : Eroare la apelul functiei socket.\n");
        exit(-1);;
    }
    return fds;
}
/* This function connects the UDP socket on the given port and makes the
    connection. The new sockaddr_in structure will be returned. */
struct sockaddr_in get_udp_addr_server(int portno, int fds) {

    struct sockaddr_in serv_addr;
	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	int check = bind (fds, (struct sockaddr*)(&serv_addr), sizeof(struct sockaddr));
	if (check < 0) {
		printf("-10 : Eroare la apelul functiei bind.\n");
        exit(-1);
    }

    return serv_addr;

}
/* This function opens a new TCP socket on the given port and connects it
    and the socket descriptor will be returned. */
int start_clientTCP(int portno, char* ip_addr) {
    struct sockaddr_in serv_addr;

    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
        printf("-10 : ERROR opening socket.");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    inet_aton(ip_addr, &serv_addr.sin_addr);

    if (connect(tcp_socket,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("-10 : ERROR on connect TCP.");
        exit(-1);
    }

    return tcp_socket;
}
/* This function opens a new UDP socket and returns its descriptor. */
int start_clientUDP() {
    int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udp_socket < 0) {
		printf("-10 : Error opening socket\n");
		return -1;
	}

	return udp_socket;
}
/* This function creates a new sockaddr_in structure to be used by the udp
    socket.*/
struct sockaddr_in get_udp_addr_client(int portno, char* ip_addr) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	inet_aton(ip_addr, &serv_addr.sin_addr);
    return serv_addr;
}
/* Depending on the error code that is given the function returns a
   certain error descrpion. */
void get_error(int err, char *error) {
    switch (err) {
        case -1 : strcpy(error, "-1 : Clientul nu este autentificat"); break;
        case -2 : strcpy(error, "-2 : Sesiune deja deschisa."); break;
        case -3 : strcpy(error, "-3 : Pin gresit."); break;
        case -4 : strcpy(error, "-4 : Numar card inexistent."); break;
        case -5 : strcpy(error, "-5 : Card blocat."); break;
        case -6 : strcpy(error, "-6 : Operatie esuata."); break;
        case -7 : strcpy(error, "-7 : Deblocare esuata."); break;
        case -8 : strcpy(error, "-8 : Fonduri insuficiente."); break;
        case -9 : strcpy(error, "-9 : Operatie anulata."); break;
    }
}
/* Search the user with the certain card number and returns a pointer
   to the found account or NULL if there is no user with that card number.*/
User* get_user(int card_number) {
    for (int i = 0; i < number_of_clients; i++) {
        if (users[i]->card_number == card_number) {
            return users[i];
        }
    }
    return NULL;
}
/* Search the user with the certain id and returns a pointer
   to the found account or NULL if there is no user with that id.*/
User* get_client(int id) {
    for (int i = 0; i < number_of_clients; i++) {
        if (users[i]->client_id == id) {
            return users[i];
        }
    }
    return NULL;
}
