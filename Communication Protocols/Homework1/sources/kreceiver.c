#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"
#include "package.h"

#define HOST "127.0.0.1"
#define PORT 10001

char expectedseq = 0;
/*
    This function returns the received message after three trials. If no
    message was received than it returns NULL.

*/
msg* receive_package() {

    msg *received;

    int receivedseq;
    int count = 3;
    while (1) {

        /* check if after three times no message was received */
        if (count == 0) {
            return NULL;
        }

        received = receive_message_timeout(5000);

        if (received != NULL) {
            count = 3;

            receivedseq = received->payload[2];

            if(receivedseq == expectedseq){

                /* check if the package was received correctly and it isn't corrupt */
                unsigned short local_crc = crc16_ccitt(received->payload, received->len - 3);
                unsigned short received_crc;
                memcpy(&received_crc, received->payload + received->len - 3, 2);

                expectedseq = (expectedseq + 2) % 64;
                if (local_crc == received_crc) {

                    /* send ACK message if the payload is correct */
                    printf("Package with seq: %d received.\n", receivedseq);
                    msg *send = send_ack_for_received_message(received);
                    printf("[%d] Send ACK for package with seq: %d\n", send->payload[2], receivedseq);
                    send_message(send);
                    break;
                }
                else {
                    /* send NACK message if the payloadis not correct */
                    msg *send = send_nack_for_received_message(received);
                    printf("[%d] Send NACK for package with seq: %d\n", send->payload[2], receivedseq);
                    send_message(send);
                }
                received = NULL;
            }
        }
        else {
            count--;
        }
    }

    return received;
}
int main(int argc, char** argv) {

    init(HOST, PORT);

    /* expect the send init package */
    msg* check = receive_package();
    if (check == NULL)
        return -1;

    int fd;
    msg* received;

    /* transmission started */
    do{

        received = receive_package();
        /* check if timeout has been reached */
        if (received == NULL)
            break;

        /* if the received message is a type F package than open a new file
           to copy the future data into it.
        */
        if (received->payload[3] == 'F'){
            char filename[20];
            strcpy(filename, "recv_");
            strncat(filename, received->payload + 4, received->len - 7);
            filename[14] = '\0';
            fd = open(filename, O_CREAT | O_WRONLY, 0777);

            if (fd < 0) {
                printf("File failed to create\n");
                return -1;
            }
        /* if the received message is a type D package than write to the currently
            opened file all the data recived.
        */
        } else if (received->payload[3] == 'D'){

            int number_bytes_written = write(fd, received->payload + 4, (unsigned char)(received->len - 7));
            if (number_bytes_written < 0){
                return -1;
            }
        /* if the received message is a type Z package than close the current file*/
        } else if (received->payload[3] == 'Z'){
            close(fd);
        /* if the received message is a type B package than close the transmission*/
        } else if (received->payload[3] == 'B'){
            break;
        }
    } while(received->payload[3] != 'B');

	return 0;
}
