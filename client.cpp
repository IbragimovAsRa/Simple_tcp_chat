#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>

#define ERROR_S "CLIENT ERROR: "
#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1615
#define BUFFER_SIZE 1024
#define SERVER_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_close(const char* msg);

int main(int argc, char const * argv[]) {
    int client;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout<<"establishing socket error";
        exit(0);
    }

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    std::cout << "\n=> Client socket created ";

    int ret = connect(client,
               reinterpret_cast<const struct sockaddr*>(&server_address),
               sizeof (server_address));
    if(ret == 0) {
        std::cout << "=> Connection to server "
                    << inet_ntoa(server_address.sin_addr)
                    << " with port number: " << DEFAULT_PORT << "\n";
    }

    char buffer[BUFFER_SIZE];

    std::cout << "Waiting for server  confirmation ...\n";
    recv(client, buffer, BUFFER_SIZE, 0);
    std::cout << "=> Connection established.\n"
                << "Enter" << SERVER_CLOSE_CONNECTION_SYMBOL
                << "for close the connection\n";
    while (true) {
        std::cout << "Client: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(client, buffer, BUFFER_SIZE, 0);
        if(is_client_connection_close(buffer)) {
            break;
        }

        std::cout << "Server: ";
        recv(client, buffer, BUFFER_SIZE, 0);
        std::cout << buffer;
        if(is_client_connection_close(buffer)) {
            break;
        }
        std::cout << std::endl;
    }
    close(client);
    std::cout << "\n GoodBye..." << std::endl;
    return 0;
}


bool is_client_connection_close(const char* msg) {
    for (int i = 0; i < strlen(msg); ++i) {
        if(msg[i] == SERVER_CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }
    return false;
}
