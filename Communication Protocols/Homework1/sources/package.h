#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

typedef struct __attribute__((__packed__)) {
    char soh;
    char len;
    char seq;
    char type;
    char *data;
    short int check;
    char mark;
} package;

typedef struct __attribute__((__packed__)) {
    unsigned char maxl;
    unsigned char time;
    unsigned char npad;
    unsigned char padc;
    unsigned char eol;
    unsigned char qctl;
    unsigned char qbin;
    unsigned char chkt;
    unsigned char rept;
    unsigned char capa;
    unsigned char r;
} data_s;

/*
    This functions allocs and initialises a package struct.
*/
package* init_package() {

    package *p = (package*)malloc(sizeof(package));

    p->soh = 0x01;
    p->len = 0;
    p->seq = 0;
    p->type = 0;
    p->data = NULL;
    p->check = 0;
    p->mark = 0;

    return p;
}
/*
    This functions allocs and initialises a data_s struct,
    structure that represents the data from the send init package.
*/
data_s* init_data_s() {

    data_s* data = (data_s*)malloc(sizeof(data_s));

    data->maxl = 250;
    data->time = 5;
    data->npad = 0;
    data->padc = 0;
    data->eol = 0x0D;
    data->qctl = 0;
    data->qbin = 0;
    data->chkt = 0;
    data->rept = 0;
    data->capa = 0;
    data->r = 0;

    return data;
}

/*
    This functions returns a type S package with the given
    sequence and updates its fields with the right values.
*/
package* new_s_package(int seq) {

    package* s_package = init_package();

    s_package->len = 16;
    s_package->type = 'S';
    s_package->data = (char*)init_data_s();
    s_package->seq = seq;
    s_package->mark = s_package->data[4];

    return s_package;
}
/*
    This functions returns a type F package with the given
    sequence, mark character and file name.

*/
package* new_f_package(char* filename, int seq, char mark) {

    package* f_package = init_package();

    f_package->type = 'F';
    f_package->seq = seq;
    f_package->data = (char*)malloc(strlen(filename) + 1);
    memcpy(f_package->data, filename, strlen(filename) + 1);
    f_package->len = 5 + strlen(f_package->data);
    f_package->mark = mark;

    return f_package;
}
/*
    This functions returns a type F package with the given
    sequence, size of the data field (number of bytes contained
    by the package), data and mark character.
*/
package* new_d_package(int size, int seq, char* data, char mark) {

    package* d_package = init_package();

    d_package->type = 'D';
    d_package->seq = seq;
    d_package->len = 5 + size;
    d_package->data = (char*)malloc(size);
    d_package->mark = mark;
    memcpy(d_package->data, data, size);

    return d_package;
}
/*
    This functions returns a package of the given type : B, Z, Y, or N.
    The sequence and the mark character are also updated with
    the given parameters.
*/
package* new_b_z_y_n_package(int seq, char type, char mark) {

    package* package = init_package();

    package->type = type;
    package->len = 5;
    package->seq = seq;
    package->mark = mark;

    return package;
}
/*
    This function converts a package struct into a msg struct.
    All the fields from the package are copied in the payload of
    the message. The CRC is calculated and is updated in the right
    place of the payload.
*/
msg* package_to_msg(package* p) {

    msg* m = (msg*)malloc(sizeof(msg));

    if (m == NULL)
        return NULL;

    unsigned char data_len = p->len - 5;

    /* copy the first fields of the package struct*/
    memcpy(m->payload, p, 4);
    /* copy the data field of the package struct*/
    memcpy(m->payload + 4, p->data, data_len);

    /* the length of the payload is updated */
    m->len = p->len + 2;

    /* compute the crc of the payload */
    unsigned short int crc = crc16_ccitt(m->payload, m->len - 3);

    /* copy the crc and the mark in the payload */
    memcpy(m->payload + 4 + data_len, &crc, 2);
    memcpy(m->payload + 4 + data_len + 2, &(p->mark), 1);

    return m;

}
/*
    Depending on the given parameter the function returns a
    type Y msg, that represents the ACK for the received package.

*/
msg *send_ack_for_received_message(msg *received) {

    /* Create a new Y type package with the right values */
    package* y_package = new_b_z_y_n_package((received->payload[2] + 1) % 64, 'Y',
                        received->payload[received->len - 1]);

    /* if it's a type S package than copy the configurations in the data field */
    if (received->payload[3] == 'S') {

        y_package->data = (char*)malloc(received->len - 5);
        memcpy(y_package->data, received->payload + 4, received->len - 5);
        y_package->len = received->payload[1];

    }

    /* copy the last two fields */
    memcpy(&(y_package->check), received->payload + received->len - 3, 2);

    return package_to_msg(y_package);
}
/*
    Depending on the given parameter the function returns a
    type N msg, that represents the NACK for the received package.

*/
msg *send_nack_for_received_message(msg *received) {

    /* create a new N type package with the right values */
    package* n_package = new_b_z_y_n_package((received->payload[2] + 1) % 64, 'N',
                        received->payload[received->len - 1]);

    return package_to_msg(n_package);
}
