#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"
#include "package.h"

#define HOST "127.0.0.1"
#define PORT 10000

char seq = 0;

/*
    In this function, the given package is sent for maximum three times.
    After sending the package, it waits for a confirmation. If it
    doesn't receive an ACK message, the package is sent again until
    an ACK message is received. If no ACK is received after 3 times
    the communication is shut down.
*/
int send_package(package* p){

    msg *m = package_to_msg(p);
    send_message(m);

    //initialise counter
    int count = 3;
	while (1) {

        /*check if timeout has been reached 3 time */
		if(count == 0) {
			return -1;
		}

		msg *received = receive_message_timeout(5000);

		if (received != NULL) {

			count = 3;

            /* check if the received message's sequence matches the expected one */

            if(received->payload[2] == seq + 1){

                /* Update sequence number */
                seq = (seq + 2) % 64;
                /* if received message is an ACK than stop the transmission of the package */
    			if (received->payload[3] == 'Y') {

                    printf("Package type %c sent\n", m->payload[3]);
                    printf("Sequence of the sent message: %d\n", m->payload[2]);
    				break;

    			} else {
                /* if received message is an NACK than update the sequence from the payload and
                 resend the package */
                    p->seq = seq;
                    m = package_to_msg(p);
    				send_message(m);
    			}
                received = NULL;
            }
		}
		/* if no message is received than decrement the count contor an resend message */
		else {
            count--;
			send_message(m);
		}
	}
    return 0;
}
/*
    This function parameters are the name of the file that is being sent
    and the send init package that contains the local configurations.
 */
int send_file(char* filename, package* init_s) {

    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
      return -1;
    }
    /* get the size of the opened file */
    struct stat fs;
    fstat(fd, &fs);
    int file_size = fs.st_size;

    /* send a type F package with the current file name */
    package* f_package = new_f_package(filename, seq, init_s->mark);
    send_package(f_package);

    do {
    /* send D type packages that contain the file's data */
    package *d_package;

        char aux[250];

        int number_bytes_read = read(fd, aux, (unsigned char)(init_s->data[0]));

        d_package = new_d_package(number_bytes_read, seq, aux, init_s->mark);

        file_size -= number_bytes_read;

        send_package(d_package);

    }while (file_size > 0);

    /* send a Z type package that marks the end of the file transmission */
    package* z_package = new_b_z_y_n_package(seq, 'Z', init_s->mark);
    send_package(z_package);

    return 0;

}
int main(int argc, char** argv) {

    init(HOST, PORT);

    /* sent the S package that contains the local configurations */
    package* s_package = new_s_package(seq);
    send_package(s_package);

    for (int i = 1; i < argc; i++) {
        send_file(argv[i], s_package);
        printf("File %s was sent successfully\n", argv[i]);
    }

    return 0;
}
