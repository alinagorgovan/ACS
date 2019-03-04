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

/* -- Query & Resource Record Type: -- */
#define A 1 /* IPv4 address */
#define NS 2 /* Authoritative name server */
#define CNAME 5 /* Canonical name for an alias */
#define MX 15 /* Mail exchange */
#define SOA 6 /* Start Of a zone of Authority */
#define TXT 16 /* Text strings */
#define PTR 12

typedef struct {
    // schimba (LITTLE/BIG ENDIAN) folosind htons/ntohs
    unsigned short id; // identification number

    // LITTLE -> BIG ENDIAN: inversare ’manuala’ ptr byte-ul 1 din flag-uri
    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag: 0=query; 1=response

    // LITTLE -> BIG ENDIAN: inversare ’manuala’ ptr byte-ul 2 din flag-uri
    unsigned char rcode :4;
    unsigned char z :3;
    unsigned char ra :1;// LITTLE -> BIG ENDIAN: inversare ’manuala’ ptr byte-ul 2 din flag-uri

    // schimba (LITTLE/BIG ENDIAN) folosind htons/ntohs
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
} dns_header_t;

typedef struct {
    char *qname;
    unsigned short qtype;
    unsigned short qclass;
} dns_question_t;

typedef struct {
    char* name;
    unsigned short type;
    unsigned short classs;
    unsigned int ttl;
    unsigned short rdlength;
    unsigned char *rdata;
} dns_rr_t;

typedef struct {
    dns_header_t *header;
    dns_question_t *question;
    dns_rr_t **records;
} dns_query_t;

int number_dns_servers = 0;
char* reverse_ip(char* ip)
{
    int a, b, c, d;
    char* ip2 = (char*)malloc(16);
    sscanf(ip,"%d.%d.%d.%d",&a,&b,&c,&d);
    sprintf(ip2, "%d.%d.%d.%d", d, c, b, a);

    return ip2;
}
char* parse_name(char* name) {
    char* qname = (char*)malloc(strlen(name) + 1);
    int count = 0;
    qname[0] = '.';
    strcpy(qname + 1, name);
    for(unsigned int i = 1; i <= strlen(qname); i++) {
        count++;
        if(qname[i] == '.' || qname[i] == '\0') {
            qname[i - count] = count - 1;
            count = 0;
        }
    }
    return qname;
}
char* get_ptr_name(char* ip) {
    char *name = (char*)malloc(100);
    strcpy(name, reverse_ip(ip));
    strcat(name, ".in-addr.arpa");

    return name;
}
dns_header_t *init_header() {
    dns_header_t *header = (dns_header_t*)malloc(sizeof(dns_header_t));
    header->id = (unsigned short)htons(getpid());
    header->rd = 1;
    header->tc = 0;
    header->aa = 0;
    header->opcode = 0;
    header->qr = 0;
    header->rcode = 0;
    header->z = 0;
    header->ra = 0;
    header->qdcount = htons(1);
    header->ancount = 0;
    header->nscount = 0;
    header->arcount = 0;
    return header;
}
dns_question_t *init_question() {
    dns_question_t *question = (dns_question_t*)malloc(sizeof(dns_question_t));
    question->qname = NULL;
    question->qtype = 0;
    question->qclass = htons(1);
    return question;
}
dns_rr_t *init_rr() {
    dns_rr_t *rr = (dns_rr_t*)malloc(sizeof(dns_rr_t));
    rr->name = NULL;
    rr->type = 0;
    rr->classs = 0;
    rr->ttl = 0;
    rr->rdlength = 0;
    rr->rdata = NULL;
    return rr;
}
dns_query_t *new_query() {
    dns_query_t* query = (dns_query_t*)malloc(sizeof(dns_query_t));
    query->header = init_header();
    query->question = init_question();
    // query->records = init_rr();

    return query;
}
int create_buffer(dns_query_t* query, char* buffer) {
    memcpy(buffer, query->header, sizeof(dns_header_t));
    // memcpy(buffer, query->header, sizeof(dns_header_t));
    memcpy(buffer + sizeof(dns_header_t), (query->question)->qname, strlen((query->question)->qname) + 1);
    memcpy(buffer + sizeof(dns_header_t) + strlen((query->question)->qname) + 1, &(query->question->qtype), 4);
    return sizeof(dns_header_t) + strlen((query->question)->qname) + 1 + 4;
}
char **get_servers() {
    int count = 0;
    char **dns_servers = (char**)malloc(100 * sizeof(char*));
    FILE *f = fopen("dns_servers.conf", "r");
    if (f == NULL) {
        printf("Error on opening file.\n");
        exit(-1);
    }
    char line[100];
    while (fgets(line, 100, f)) {
        if (line[0] >= 48 && line[0] <= 57) {
            line[strlen(line) - 1] = '\0';
            dns_servers[count] = (char*)malloc(strlen(line));
            strcpy(dns_servers[count], line);
            count++;
        }
    }
    number_dns_servers = count;
    return dns_servers;
}
int start_clientUDP() {
    int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udp_socket < 0) {
		printf("-10 : Error opening socket\n");
		return -1;
	}

	return udp_socket;
}
struct sockaddr_in get_udp_addr_client(char* ip_addr) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(53);
	inet_aton(ip_addr, &serv_addr.sin_addr);
    return serv_addr;
}

int set_type(char *type) {
    if (!strcmp(type, "A")) {
        return A;
    } else if (!strcmp(type, "NS")) {
        return NS;
    } else if (!strcmp(type, "CNAME")) {
        return CNAME;
    } else if (!strcmp(type, "MX")) {
        return MX;
    } else if (!strcmp(type, "SOA")) {
        return SOA;
    } else if (!strcmp(type, "TXT")) {
        return TXT;
    } else if (!strcmp(type, "PTR")) {
        return PTR;
    }
    return 0;
}

dns_query_t* buffer_to_query(char* name, char* buffer) {
    dns_query_t *query = new_query();
    int n = 0;
    // header
    memcpy(query->header, buffer, sizeof(dns_header_t));
    n += sizeof(dns_header_t);
    // question
    (query->question)->qname = (char*)malloc(strlen(name) + 1);
    memcpy((query->question)->qname, buffer + n, strlen(name) + 1);
    n += strlen(name) + 3;
    memcpy(&((query->question)->qtype), buffer + n, 2);
    n += 1;
    memcpy(&((query->question)->qclass), buffer + n, 2);
    n += 4;

    // answers
    int number_rrs = (ntohs((query->header)->ancount)) + (ntohs((query->header)->nscount)) + (ntohs((query->header)->arcount));
    query->records = (dns_rr_t**)malloc(number_rrs * sizeof(dns_rr_t*));
    for (int i = 0; i < number_rrs; i++){

        (query->records)[i] = init_rr();
        int c = buffer[n];
        ((query->records)[i])->name = (char*)malloc(strlen(name) + 1);
        memcpy(((query->records)[i])->name, buffer + c, strlen(name) + 1);
        n += 1;
        memcpy(&(((query->records)[i])->type), buffer + n, 4);
        n += 3;
        memcpy(&(((query->records)[i])->ttl), buffer + n, 6);
        n += 6;
        ((query->records)[i])->rdata = (unsigned char*)malloc(ntohs(((query->records)[i])->rdlength));
        memcpy(((query->records)[i])->rdata, buffer + n, ntohs(((query->records)[i])->rdlength));
        n += ntohs(((query->records)[i])->rdlength) + 2;
    }
    return query;
}
void write_aux(FILE *g, dns_query_t* response, int i, char* name) {

    char domain[50];
    int type = ((response->records)[i])->type;
    unsigned short len = ntohs(((response->records)[i])->rdlength);
    unsigned char *aux = ((response->records)[i])->rdata;

    if (type == 12) {

        for (int j = 1; j < len; j++) {
            if (aux[j] < 0x30 ){
                fprintf(g, ".");
            } else {
                fprintf(g, "%c", aux[j]);
            }
        }
        if (ntohs((response->question)->qclass) == 1) {
            fprintf(g, " IN ");
        }
        fprintf(g, "PTR ");

        char *p = strtok(name, "i");
        char *address = reverse_ip(p);
        fprintf(g, "%s ", address);
        fprintf(g, "\n");
    } else {
        fprintf(g, "%s ", name);
        if (ntohs((response->question)->qclass) == 1) {
            fprintf(g, "IN ");
        }

        if (type == 1) {
            fprintf(g, "A ");
            fprintf(g, "%d.%d.%d.%d\n", aux[0], aux[1], aux[2], aux[3]);
        }
        else if (type == 2) {
            fprintf(g, "NS ");
            for (int j = 1; j < len - 2; j++) {
                if (aux[j] < 0x30 ){
                    fprintf(g, ".");
                } else {
                    fprintf(g, "%c", aux[j]);
                }
            }
            fprintf(g, ".%s\n", name);
        }
        else if (response->question->qtype == 5) {
            fprintf(g, "CNAME ");
            for (int j = 1; j < len; j++) {
                if (aux[j] == 0xc0)
                break;
                else if (aux[j] < 0x0a ){
                    fprintf(g, ".");
                } else {
                    fprintf(g, "%c", aux[j]);
                }
            }
            fprintf(g, ".%s\n", name);
        }
        else if (type == 15) {
            fprintf(g, "MX ");
            unsigned short pref;
            if (i == 0) {
                memcpy(domain, ((response->records)[i])->rdata + 7, len -7);
            }
            memcpy(&pref, ((response->records)[i])->rdata, 2);
            fprintf(g, "%d ", ntohs(pref));
            for (int j = 3; j < 7; j++) {
                if (aux[j] < 0x30 ){
                    fprintf(g, ".");
                } else {
                    fprintf(g, "%c", aux[j]);
                }
            }
            for (unsigned int j = 0; j < strlen(domain); j++) {
                if (domain[j] < 0x30 ){
                    fprintf(g, ".");
                } else {
                    fprintf(g, "%c", domain[j]);
                }
            }

            fprintf(g, "\n");
        }
        else if (type == 6) {
            fprintf(g, "SOA ");

            int count = 0;
            for (int j = 1; j < len; j++) {
                if (aux[j] == 0xc0){
                    count = j;
                    break;
                }
                else if (aux[j] < 0x0a ){
                    fprintf(g, ".");
                } else {
                    fprintf(g, "%c", aux[j]);
                }
            }
            fprintf(g, ".%s ", name);

            for (int j = count + 3; j < len; j++) {

                if (aux[j] == 0xc0){
                    count = j;
                    break;
                }
                if (aux[j] < 0x30 ){
                    fprintf(g, ".");
                } else {
                    fprintf(g, "%c", aux[j]);
                }
            }
            fprintf(g, ".%s ", name);
            unsigned int number;
            memcpy(&number, aux + count + 2, 4);
            fprintf(g, "%u ", htonl(number));

            count += 6;
            memcpy(&number, aux + count, 4);
            fprintf(g, "%u ", htonl(number));

            count += 4;
            memcpy(&number, aux + count, 4);
            fprintf(g, "%u ", htonl(number));

            count += 4;
            memcpy(&number, aux + count, 4);
            fprintf(g, "%u ", htonl(number));

            count += 4;
            memcpy(&number, aux + count, 4);
            fprintf(g, "%u ", htonl(number));

            fprintf(g, "\n");


        }
        else if (type == 16) {
            fprintf(g, "TXT \"");
            for (int j = 1; j < len; j++) {
                fprintf(g, "%c", aux[j]);
            }
            fprintf(g, "\"\n");
        }

    }

}
void write_data(char* used_dns, char* name, char* type, char* buffer, char* query_name) {
    dns_query_t *response = buffer_to_query(name, buffer);
    FILE *g = fopen("dns.log", "a");
    if (g == NULL) {
        printf("Error on opening file\n");
        exit(-1);
    }
    fprintf(g, "; %s - %s %s\n", used_dns, query_name, type);

    int count = 0;
    if (ntohs((response->header)->ancount)) {
        fprintf(g, "\n;; ANSWER SECTION:\n");
        for (int i = 0; i < ntohs((response->header)->ancount); i++) {
            write_aux(g, response, count, name);
            count++;
        }
        fprintf(g, "\n\n");

    }
    if (ntohs((response->header)->nscount)) {
        fprintf(g, "\n;; AUTHORITY SECTION:\n");
        for (int i = 0; i < ntohs((response->header)->nscount); i++) {
            write_aux(g, response, count, name);
            count++;
        }
        fprintf(g, "\n\n");
    }
    if (ntohs((response->header)->arcount)) {
        fprintf(g, "\n;; ADDITIONAL SECTION:\n");
        for (int i = 0; i < ntohs((response->header)->nscount); i++) {
            write_aux(g, response, count, name);
            count++;
        }
        fprintf(g, "\n\n");

    }

}
void write_message(char* buffer, int length) {
    FILE *g = fopen("message.log", "a");
    for(int i = 0; i < length; i++) {
        fprintf(g, "%02x ", buffer[i]);
    }
    fprintf(g, "\n");
}
