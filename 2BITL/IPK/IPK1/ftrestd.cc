// IPK 2017
// Project 1
// Author: David Bolvansky xbolva00

#include <iostream>
#include <sstream>
#include <map>
#include <cstdint>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "network_io.h"
#include "file_io.h"

std::string root_folder;

// Handle response, do action - file/folder operation, and create request to be send to client
HTTPRequest handle_response(std::string root_folder, HTTPResponse resp) {
        std::string cmd = resp.get_command();
        std::string user = resp.get_user();

        if (user == "." || user == ".." || resp.get_remote_path().find(user + "/..") != std::string::npos) {
                return HTTPRequest(400, "text/plain", "Unknown error.");
        }

        Storage user_dir = Storage(root_folder + "/" + user);
        if (!user_dir.exists()) {
                return HTTPRequest(404, "text/plain", "User Account Not Found");
        }

        if (cmd.compare("PUT") == 0) {
                if (resp.get_remote_type() == TYPE_FILE) {
                        Storage f = Storage(root_folder + "/" + resp.get_remote_path());

                        if (resp.get_action() == ACTION_COPY) {
                                Storage copy_to = Storage(root_folder + "/" + resp.get_content());
                                if (f.copy_file(copy_to)) {
                                        return HTTPRequest(200, "text/plain", "");
                                } else {
                                        return HTTPRequest(400, "text/plain", "Unknown error.");
                                }
                        } else if (resp.get_action() == ACTION_MOVE) {
                                Storage move_to = Storage(root_folder + "/" + resp.get_content());
                                if (f.move_file(move_to)) {
                                        return HTTPRequest(200, "text/plain", "");
                                } else {
                                        return HTTPRequest(400, "text/plain", "Unknown error.");
                                }
                        }

                        if (f.exists()) {
                                return HTTPRequest(400, "text/plain", "Already exists.");
                        } else if (!Storage(f.get_directory_base_name()).exists()) {
                                return HTTPRequest(404, "text/plain", "Directory not found.");
                        } else {
                                if (f.write_file(resp.get_content())) {
                                        return HTTPRequest(200, "text/plain", "");
                                } else {
                                        return HTTPRequest(400, "text/plain", "Unknown error.");
                                }
                        }
                } else {
                        Storage f = Storage(root_folder + "/" + resp.get_remote_path());
                        if (f.exists()) {
                                return HTTPRequest(400, "text/plain", "Already exists.");
                        } else if (!Storage(f.get_directory_base_name()).exists()) {
                                return HTTPRequest(404, "text/plain", "Directory not found.");
                        } else {
                                if(f.create_directory() == 0) {
                                        return HTTPRequest(200, "text/plain", "");
                                } else {
                                        return HTTPRequest(400, "text/plain", "Unknown error.");
                                }
                        }
                }
        } else if (cmd.compare("DELETE") == 0) {
                if (resp.get_remote_type() == TYPE_FILE) {
                        Storage f = Storage(root_folder + "/" + resp.get_remote_path());
                        if (!f.exists()) {
                                return HTTPRequest(404, "text/plain", "File not found.");
                        } else if (!f.is_file()) {
                                return HTTPRequest(400, "text/plain", "Not a file.");
                        } else {
                                if(f.remove_file() == 0) {
                                        return HTTPRequest(200, "text/plain", "");
                                } else {
                                        return HTTPRequest(400, "text/plain", "Unknown error.");
                                }
                        }
                } else {
                        Storage user_root = Storage(resp.get_remote_path());
                        Storage f = Storage(root_folder + "/" + resp.get_remote_path());
                        if (!f.exists()) {
                                return HTTPRequest(404, "text/plain", "Directory not found.");
                        } else if (!f.is_directory()) {
                                return HTTPRequest(400, "text/plain", "Not a directory.");
                        } else if (user_root.is_user_root_directory()) {
                                return HTTPRequest(400, "text/plain", "Unknown error.");
                        } else if (resp.get_action() == ACTION_REMOVE_DUPLICATES) {
                                f.remove_duplicated_files();
                                return HTTPRequest(200, "text/plain", "");
                        } else if (!f.is_empty_directory()) {
                                return HTTPRequest(400, "text/plain", "Directory not empty.");
                        } else {
                                if (f.remove_directory() == 0) {
                                        return HTTPRequest(200, "text/plain", "");
                                } else {
                                        return HTTPRequest(400, "text/plain", "Unknown error.");
                                }
                        }

                }
        } else if (cmd.compare("GET") == 0) {
                if (resp.get_remote_type() == TYPE_FILE) {
                        Storage f = Storage(root_folder + "/" + resp.get_remote_path());
                        if (!f.exists()) {
                                return HTTPRequest(404, "text/plain", "File not found.");
                        } else if (!f.is_file()) {
                                return HTTPRequest(400, "text/plain", "Not a file.");
                        } else {
                                return HTTPRequest(200, f.get_content_type(), f.read_file());
                        }
                } else {
                        Storage f = Storage(root_folder + "/" + resp.get_remote_path());
                        if (!f.exists()) {
                                return HTTPRequest(404, "text/plain", "Directory not found.");
                        } else if (!f.is_directory()) {
                                return HTTPRequest(400, "text/plain", "Not a directory.");
                        } else {
                                if (resp.get_action() == ACTION_FILTER) {
                                        return HTTPRequest(200, "text/plain", f.list_directory_filtered(resp.get_content()));
                                } else {
                                        return HTTPRequest(200, "text/plain", f.list_directory());
                                }
                        }
                }
        }
}

// Handle connection, get response, do action, send answer to client
void *handle_connection(void *arg)
{
        int comm_socket = (intptr_t) arg;

        pthread_detach(pthread_self());

        HTTPResponse resp = HTTPResponse(comm_socket);
        handle_response(root_folder, resp).send(comm_socket);

        close(comm_socket);
        pthread_exit(NULL);
}

int main (int argc, char * argv[]) {

        /// Process arguments
        int port_number = DEFAULT_PORT;
        root_folder = Storage().get_current_directory();

        int c;
        while((c =  getopt(argc, argv, "r:p:")) != EOF)
        {
                switch (c) {
                case 'r':
                        root_folder = optarg;
                        if (!Storage(root_folder).exists()) {
                                std::cerr << "Error: Root folder does not exist." << std::endl;
                                exit(EXIT_FAILURE);
                        }
                        break;
                case 'p':
                        try {
                                port_number = std::stoi(optarg);
                        }
                        catch (std::invalid_argument& e) {
                                std::cerr << "Error: Not a valid port number." << std::endl;
                                exit(EXIT_FAILURE);
                        }
                        if (port_number < 0 || port_number > 65535) {
                                std::cerr << "Error: Port number out of range." << std::endl;
                                exit(EXIT_FAILURE);
                        }
                        break;
                default:
                        std::cerr << "Error: Unknown option." << std::endl;
                        exit(EXIT_FAILURE);
                }
        }

        argc -= optind;
        argv += optind;

        int rc;
        int welcome_socket;
        struct sockaddr_in6 sa;
        struct sockaddr_in6 sa_client;
        char str[INET6_ADDRSTRLEN];

        // Create welcome socket
        socklen_t sa_client_len = sizeof(sa_client);
        if ((welcome_socket = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
                perror("Error: socket\n");
                exit(EXIT_FAILURE);
        }

        memset(&sa,0,sizeof(sa));
        sa.sin6_family = AF_INET6;
        sa.sin6_addr = in6addr_any;
        sa.sin6_port = htons(port_number);

        // Bind to port
        if ((rc = bind(welcome_socket, reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa))) < 0)
        {
                perror("Error: bind\n");
                exit(EXIT_FAILURE);
        }

        // Start listening
        if ((listen(welcome_socket, 1)) < 0)
        {
                perror("Error: listen\n");
                exit(EXIT_FAILURE);
        }

        pthread_t tid;
        int err;

        // Wait for connection
        while(true) {
                // Connect
                int comm_socket = accept(welcome_socket, reinterpret_cast<struct sockaddr*>(&sa_client), &sa_client_len);
                if (comm_socket > 0) {
                        // Start thread for client request
                        err = pthread_create(&tid, NULL, handle_connection, (void *) (intptr_t) comm_socket);
                        if (err != 0) {
                                // fallback to iterative server implementation when thread creation failed
                                perror("Error: starting thread\n");
                                HTTPResponse resp = HTTPResponse(comm_socket);
                                handle_response(root_folder, resp).send(comm_socket);
                                close(comm_socket);
                        }

                }
        }

        return EXIT_SUCCESS;
}
