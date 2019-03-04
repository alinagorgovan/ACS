#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"
#include "protocolUtils.h"

#define HOST "127.0.0.1"
#define PORT 10001



int main(int argc, char** argv) {
    msg r, send;

    init(HOST, PORT);

    /* este asteptat pachetul de tip send init, in cazul in care s-a primit
    timeout de 3 ori succesiv, executia receptorului este intrerupta    */
    int countTimeouts = 0;
    while(1) {
        if(countTimeouts == 3) {
            return -1;
        }

        msg* received = receive_message_timeout(maxWaitingTime);
        if(received != NULL) {
            countTimeouts = 0;

            /* se verifica daca pachetul send init a fost corupt si este trimisa
            o confirmare pozitiva sau negativa catre emitator                 */
            unsigned short crc = crc16_ccitt(received->payload, received->len - 3);
            unsigned short sent;
            memcpy(&sent, received->payload + received->len - 3, 2);

            if(crc == sent) {
                writeAck(&send, 0);
                send_message(&send);
                break;
            }
            else {
                writeNak(&send, 0);
                send_message(&send);
            }
        }
        /* este incrementat contorul ce retine numarul de timeout-uri */
        else {
            countTimeouts++;
        }
    }

    int fid;
    int endConnection = 0;

    /* cat timp mai sunt date de primit de la emitator */
    while(1) {

        /* este initializat numarul secventei asteptate cu 1, pentru a realiza
        alternanta indexarii pachetelor primite, respectiv trimise, de asemenea
        fiind initializate variabile ce indica daca un fisier a fost primit
        integral, respectiv numarul de timeouturi consecutive                */
        unsigned char expectedSequence = 1;
        int finishTransfer = 0;
        int timeoutCount = 0;

        /* este initializat un pachet de confirmare ce nu este de tip nak sau
        ack, pentru a nu influenta in caz de timeout alternanta pachetelor */
        writeNak(&send, expectedSequence);
        send.payload[3] = 'P';

        /* cat timp mai sunt de primit date corespunzatoare unui fisier */
        while(1) {

            /* daca s-a primit timeout de 3 ori succesiv pentru asteptarea unui
            pachet, executia receptorului se intrerupe                       */
            if(timeoutCount == 3) {
                return -1;
            }

            /* se asteapta maxWaitingTime milisecunde pentru primirea unui
            pachet din partea emitatorului                                */
            memset(r.payload, '\0', sizeof(r.payload));
            msg* aux = receive_message_timeout(maxWaitingTime);

            /* in caz de timeout este retrimis ultimul pachet de confirmare */
            if(aux == NULL) {
                timeoutCount++;
                send_message(&send);
            }

            /* in cazul in care s-a primit un pachet de date de la emitator */
            else {
                r = *aux;
                timeoutCount = 0;

                /* este calculat crc-ul pachetului trimis */
                unsigned short crc = crc16_ccitt(r.payload, r.len - 3);
                unsigned short sent;
                memcpy(&sent, r.payload + r.len - 3, 2);

                /* sunt extrase din mesajul primit campurile pachetului */
                unsigned char seq = r.payload[2];
                unsigned char len = r.payload[1];
                unsigned char type = r.payload[3];

                /* in cazul in care pachetul a fost trimis fara erori */
                if(crc == sent) {
                    /* in cazul in care numarul de secventa al pachetului primit
                    corespunde cu numarul de secventa asteptat               */
                    if(seq + 1 == expectedSequence) {

                        /* sunt copiate datele din pachet intr-un buffer */
                        unsigned char* buf = calloc(len, sizeof(char));
                        memcpy(buf, r.payload + 4, len);

                        /* daca pachetul este de tip end of transfer, este
                        activat flagul corespunzator                       */
                        if(type == 0x42) {
                            endConnection = 1;
                        }

                        /* daca pachetul este de tip file header, este adaugat
                        prefixul cerut numelui fisierului, acestia fiind apoi
                        deschis pentru scriere                               */
                        else if(type == 0x46) {
                            char* fileName = calloc(4 + len, sizeof(char));
                            memcpy(fileName, "recv_", 5);
                            strcat(fileName, (char*) buf);
                            fileName[5 + len] = '\0';

                            fid = open(fileName, O_WRONLY | O_CREAT, 0644);
                        }

                        /* daca pachetul contine octeti din fisier, acestia sunt
                        scrisi in fisierul de "output", iar in cazul unui pachet
                        de tip end of file, este activat flagul corespunzator */
                        else {
                            write(fid, buf, len);

                            if(type == 0x45) {
                                finishTransfer = 1;
                            }
                        }

                        /* este scris un pachet de tip ack in mesajul de
                        confirmare si este incrementat numarul secventei asteptate */
                        writeAck(&send, expectedSequence);
                        expectedSequence = (expectedSequence + 2) % 64;

                        free(buf);
                    }

                    /* este trimis mesajul de confirmare (ack sau nak) */
                    send_message(&send);
                }

                /* daca pachetul primti contine erori, este trimis un pachet
                de tip nak si este incrementat numarul secventei asteptate */
                else {
                    writeNak(&send, expectedSequence);
                    expectedSequence = (expectedSequence + 2) % 64;
                    send_message(&send);
                }
            }

            /* daca un fisier a fost primit integral sau daca flagul ce indica
            incheierea conexiunii a fost activat, se iese din bucla          */
            if(finishTransfer || endConnection) {
                break;
            }
        }

        /* daca toate datele au fost trimise, se iese din bucla principala */
        if(endConnection) {
            break;
        }

        /* este inchis fisierul de "output" curent */
        close(fid);
    }

	return 0;
}
