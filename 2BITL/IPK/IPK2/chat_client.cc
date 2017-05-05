// IPK 2017
// Project 2
// Author: David Bolvansky xbolva00

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "chat_handler.h"


int main (int argc, const char * argv[]) {
        int client_socket;
        int port_number = DEFAULT_PORT; // default port number
        struct hostent *server;
        struct sockaddr_in server_address;
        // Hostname
        const char *server_hostname;
        // Username
        std::string user_name;

        // Log chat if enabled
        bool logging = false;
        std::string log_file;

        // Check arguments
        if (argc == 5 || argc == 7) {
                // Arguments ok
                if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-u") == 0) {
                        server_hostname = argv[2];
                        user_name = argv[4];
                } else if (strcmp(argv[1], "-u") == 0 && strcmp(argv[3], "-i") == 0) {
                        server_hostname = argv[4];
                        user_name = argv[2];
                } else { // Bad arguments
                        std::cerr << "usage: chat_client -i <hostname> -u <username>" << std::endl;
                        exit(EXIT_FAILURE);
                }

                // log file
                if (argc == 7) {
                        if (strcmp(argv[5], "-f") == 0) {
                                logging = true;
                                log_file = argv[6];
                        }
                        else {
                                std::cerr << "usage: chat_client -i <hostname> -u <username> -f <file>" << std::endl;
                                exit(EXIT_FAILURE);
                        }
                }
        } else {
                std::cerr << "usage: chat_client -i <hostname> -u <username>" << std::endl;
                exit(EXIT_FAILURE);
        }

        // Get server adress
        if ((server = gethostbyname(server_hostname)) == NULL) {
                std::cerr << "Error: no such host as " << server_hostname << std::endl;
                exit(EXIT_FAILURE);
        }

        // Clear structs
        memset(&server_address, 0, sizeof(server_address));

        server_address.sin_family = AF_INET;
        bcopy(static_cast<char *>(server->h_addr), reinterpret_cast<char *>(&server_address.sin_addr.s_addr), server->h_length);
        server_address.sin_port = htons(port_number);

        // Create socket, exit if failure
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
                perror("Error: socket\n");
                exit(EXIT_FAILURE);
        }

        // Create using socket, exit if failure
        if (connect(client_socket, reinterpret_cast<const struct sockaddr *>(&server_address), sizeof(server_address)) != 0) {
                perror("Error: connect\n");
                exit(EXIT_FAILURE);
        }


        // Handler for chat messages
        ChatHandler chat_handler { client_socket};
        chat_handler.set_user_name(user_name);
        chat_handler.set_logging(logging);
        chat_handler.set_log_file(log_file);

        // Start threads for receiving messages and user input
        if (!chat_handler.start_threads()) {
                std::cerr << "Unable to start chat client threads" << std::endl;
                exit(EXIT_FAILURE);
        }

        // Close socket
        close(client_socket);
        return 0;
}
