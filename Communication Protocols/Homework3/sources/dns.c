#include "dns.h"

int main(int argc, char** argv) {
    char *address = (char*)malloc(strlen(argv[1]));
    strcpy(address, argv[1]);
    char *type = (char*)malloc(strlen(argv[2]));
    strcpy(type, argv[2]);

    char** dns_servers = get_servers();

    dns_query_t *query;
    int server_flag = 0;
    char *buffer = (char*)malloc(1024);
    int udpsocket = start_clientUDP();
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(udpsocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }

    query = new_query();
    (query->question)->qtype = htons(set_type(type));
    for (int i = 0; i < number_dns_servers; i++) {
        if(!strcmp(type, "PTR")) {
            (query->question)->qname = parse_name(get_ptr_name(address));
            strcpy(address, get_ptr_name(address));

        } else {
            (query->question)->qname = parse_name(address);

        }


        struct sockaddr_in udp_addr = get_udp_addr_client(dns_servers[i]);

        int len = create_buffer(query, buffer);
        write_message(buffer, len);
        unsigned int size = sizeof(udp_addr);
        int c = sendto(udpsocket, buffer, len, 0, (struct sockaddr*)&udp_addr, size);
        if (c < 0) {
            server_flag = 1;
            printf("Error on sendto.\n");
        }
        if (server_flag == 1) {
            continue;
        }
        printf("Package sent.\n");
        memset(buffer, 0, 1024);
        c = recvfrom (udpsocket, buffer , 1024, 0, (struct sockaddr*)&udp_addr , &size);
        if (c <= 0)
        {
            server_flag = 1;
            printf("Error on recvfrom.\n");
        }

        if (server_flag == 1) {
            continue;
        }
        printf("Package received.\n");


        if (server_flag == 0) {
            write_data(dns_servers[i], address, type, buffer, argv[1]);
            break;
        }
    }

    return 0;
}
