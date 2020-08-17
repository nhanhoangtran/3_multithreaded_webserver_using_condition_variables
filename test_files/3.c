#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>      // for inet_pton, and the like...
#include <sys/socket.h>
#include <stdbool.h>
#include <limits.h>


// useful constants
#define SERVER_PORT    8989
#define MAXLINE        4096
#define BUFFSIZE       4096
#define SOCKETERROR    (-1)
#define SERVER_BACKLOG 1

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;


void handle_connection(int client_socket);
int check(int exp, const char *msg);


int main(int argc, char **argv) {
    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;
    //
    const int opt = 1;

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");

    // initialize the address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    check(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "setsockopt server_socket SO_REUSEADDR failed!");
    check(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)), "setsockopt server_socket SO_REUSEPORT failed!");

    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind failed!");
    check(listen(server_socket, SERVER_BACKLOG), "Listen failed!");

    while (true) {
        printf("Waiting for connections...\n");

        // wait for, and eventually accept an incomming connection
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA*)&client_socket, (socklen_t*)&addr_size)), "accept failed");
        printf("Connected!\n");

        // do whatever we do with connections
        handle_connection(client_socket);

    }   //while

    close(server_socket);

    return 0;
}

int check(int exp, const char *msg) {
    if (exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

void handle_connection(int client_socket) {
    char buffer[BUFFSIZE];
    ssize_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    // read the client's message -- the name of the file to read
    while ((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1)) > 0) {
        msgsize += bytes_read;
        if (msgsize > BUFFSIZE - 1 || buffer[msgsize - 1] == '\n') {
            break;
        }
    }

    check(bytes_read, "receive error");
    buffer[msgsize - 1] = 0;    // null terminate the message and remove \n

    printf("REQUEST: %s\n", buffer);
    fflush(stdout);

    // validity check
    if (realpath(buffer, actualpath) == NULL) {
        printf("ERROR(bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    // read file and send its content to client
    FILE *fp = fopen(actualpath, "r");
    if (fp == NULL) {
        printf("ERROR(open): %s\n", buffer);
        close(client_socket);
        return;
    }

    // read file contents and send them to client
    // note this is a fine example program, but rather insecure
    // a real programm would probably limit the client to certain files
    while ((bytes_read = fread(buffer, 1, BUFFSIZE, fp)) > 0) {
    // while ((bytes_read = fread(buffer, BUFFSIZE, 1, fp)) > 0) {
        printf("sending %zu bytes \n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }
    close(client_socket);
    fclose(fp);
    printf("Closing connection...\n\n");

}