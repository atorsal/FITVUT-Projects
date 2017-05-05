// IPK 2017
// Project 1
// Author: David Bolvansky xbolva00

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "network_io.h"
#include "file_io.h"

// Create http request according to file/folder operation
HTTPRequest process_command(std::string command, std::string host, std::string remote_path, std::string local_path) {
	remote_path = "/" + remote_path;

    // Replace spaces with %20 to make it server-compatible
	remote_path = HTTPResponse::replace(remote_path, " ", "%20");

	if (command == "put") {
		Storage f = Storage(local_path);
		if (!f.exists()) {
			std::cerr << "File not found." << std::endl;
			exit(EXIT_FAILURE);
		} else if (!f.is_file()) {
			std::cerr << "Not a file." << std::endl;
			exit(EXIT_FAILURE);
		} else {
			return HTTPRequest("PUT", remote_path + "?type=file", host, "text/plain", f.get_content_type(), f.read_file());
		}
	} else if (command == "mkd") {
		return HTTPRequest("PUT", remote_path + "?type=folder", host, "text/plain", "text/plain", "");
	} else if (command == "rmd") {
		return HTTPRequest("DELETE", remote_path + "?type=folder", host, "text/plain", "", "");
	} else if (command == "del") {
		return HTTPRequest("DELETE", remote_path + "?type=file", host, "text/plain", "", "");
	} else if (command == "lst") {
		std::string filter = local_path;
		if (!filter.empty()) {
			return HTTPRequest("GET", remote_path + "?type=folder/filter", host, "text/plain", "text/plain", filter);
		} else {
			return HTTPRequest("GET", remote_path + "?type=folder", host, "text/plain", "text/plain", "");
		}
	} else if (command == "get") {
		return HTTPRequest("GET", remote_path + "?type=file", host, "text/plain", "application/octet-stream", "");
	} else if (command == "copy") {
		std::string copy_path = local_path;
		return HTTPRequest("PUT", remote_path + "?type=file/copy", host, "text/plain", "text/plain", copy_path );
	} else if (command == "move") {
		std::string move_path = local_path;
		return HTTPRequest("PUT", remote_path + "?type=file/move", host, "text/plain", "text/plain", move_path);
	} else if (command == "rdf") {
		return HTTPRequest("DELETE", remote_path + "?type=folder/removeduplicates", host, "text/plain", "text/plain", "");
	}
}
int main (int argc, char * argv[]) {
	int client_socket, port_number, bytestx, bytesrx;
	port_number = -1;
    socklen_t serverlen;
    char server_hostname[PATH_MAX+1];
    struct hostent *server;
    struct sockaddr_in server_address;
    char buf[TEMP_SIZE];

	// Too many arguments, exit
	if (argc > 5) {
		std::cerr << "Error: Too many arguments." << std::endl;
		exit(EXIT_FAILURE);
	}

    // First argument is command, ie. file/folder operation
	std::string command = argv[1];

	if (!(command == "put" || command == "del" || command == "lst" || command == "mkd" || command == "rmd" || command == "get" || command == "copy" || command == "move" || command == "rdf")) {
		std::cerr << "Error: Unknown command." << std::endl;
		exit(EXIT_FAILURE);
	}

	char user[TEMP_SIZE] = {};
	char remote_path[TEMP_SIZE] = {};
	char protocol[TEMP_SIZE] = {};

	std::string local_path;
	if (argc > 3) {
		if (command == "del" || command == "mkd" || command == "rmd") {
			std::cerr << "Error: Wrongly entered " << command << " command." << std::endl;
			exit(EXIT_FAILURE);
		}
		local_path = argv[3];
	} else {
		if (command == "put" || command == "copy" || command == "move") {
			std::cerr << "Error: Wrongly entered " << command << " command." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	std::string uri = argv[2];


	// If no specified protocol, use http as default one
	if (uri.find("://") == std::string::npos) {
		uri = "http://" + uri;
	}

	// Parse second argument, get hostname, port, and remote path
	sscanf(uri.c_str(), "%4096[^:]://%[^:]:%d/%4096[^\n]", protocol, server_hostname, &port_number, remote_path);

	if (port_number == -1) {
		// no port specified, parse it without port
		sscanf(uri.c_str(), "%4096[^:]://%[^/]/%4096[^\n]", protocol, server_hostname, remote_path);

		// set implicit port
		port_number = 6677;
	}

	// Get server adress
	if ((server = gethostbyname(server_hostname)) == NULL) {
        std::cerr << "Error: no such host as " << server_hostname << std::endl;
        exit(EXIT_FAILURE);
    }

	memset(&server_address,0,sizeof(server_address));

	server_address.sin_family = AF_INET;
    bcopy(static_cast<char *>(server->h_addr), reinterpret_cast<char *>(&server_address.sin_addr.s_addr), server->h_length);
	server_address.sin_port = htons(port_number);

    // Server socket info
    // printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

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

	// Construct host
	std::string host = server_hostname;
	host += ":" + std::to_string(port_number);

	// Send request on server
	HTTPRequest req = process_command(command, host, remote_path, local_path);
	req.send(client_socket);

    // Receiver response
	HTTPResponse resp = HTTPResponse(client_socket);

	// Handle errors, print them to stderr
	if (resp.get_http_code() != 200) {
		std::cerr << resp.get_content() << std::endl;
		exit(EXIT_FAILURE);
	}

	// Handle data from content
	if (command == "lst") {
		std::cout << resp.get_content();
	} else if (command == "get") {
		Storage f;
		std::string rpath = remote_path;
		int pos = rpath.find_last_of("/") + 1;
		std::string file = rpath.substr(pos, rpath.size());

		if (local_path.empty()) {
			f = Storage(file);
		} else {
			f = Storage(local_path);
			if ((f.exists() && f.is_directory()) || local_path.substr(local_path.size()-1,1) == "/") {
				f = Storage(local_path + "/" + file);
			}
		}

		if (!f.write_file(resp.get_content())) {
			std::cerr << "Unknown error." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	close(client_socket);
    return EXIT_SUCCESS;
}
