// IPK 2017
// Project 1
// Author: David Bolvansky xbolva00

#include <iostream>
#include <sstream>
#include <map>
#include <linux/limits.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include "network_io.h"

// Parse response
HTTPResponse::HTTPResponse(int socket_fd) {
        this->socket_fd = socket_fd;
        char buff[ONE_CHAR+1];

        // Read char by char until we find twice clrf
        while (true) {
                memset(buff, 0, ONE_CHAR+1);
                int read = recv(socket_fd, buff, ONE_CHAR, 0);

                if (!read) break;                  // done reading
                if (read < 0) {
                        std::cerr << "Error: reading header" << std::endl;
                        break;
                }

                this->header += buff;

                if (this->header.find("\r\n\r\n") != std::string::npos) break;
        }

        std::istringstream header_stream(header);
        std::string rest_op;
        std::getline(header_stream, rest_op);
        std::istringstream rest_op_stream(rest_op);

        // Parse first line, rest commands
        rest_op_stream >> this->command >> this->url >> this->http;

        if (this->command == "HTTP/1.1") {
                // Get http code
                this->http_code = std::stoi(this->url);
        } else {
                char remote_path[PATH_MAX+1];
                char type[TEMP_SIZE+1];
                char user[TEMP_SIZE+1];
                char action[TEMP_SIZE+1];

                // Parse url, get remote_path and type (file or folder)

                sscanf(this->url.c_str(), "/%4096[^?]?type=%100[^/]/%s", remote_path,type, action);

                if (strcmp(action, "filter") == 0) {
                        this->action = ACTION_FILTER;
                } else if (strcmp(action, "copy") == 0) {
                        this->action = ACTION_COPY;
                } else if (strcmp(action, "move") == 0) {
                        this->action = ACTION_MOVE;
                } else if (strcmp(action, "removeduplicates") == 0) {
                        this->action = ACTION_REMOVE_DUPLICATES;
                }

                if (strcmp(type, "file") == 0) {
                        this->remote_type = TYPE_FILE;
                } else {
                        this->remote_type = TYPE_FOLDER;
                }

                std::string path = remote_path;

                // Get spaces back
                path = replace(path, "%20", " ");
                this->remote_path = path;

                // Get user name
                sscanf(path.c_str(), "%100[^/]/", user);
                this->user = user;
        }
        std::string line;
        std::string::size_type index;

        // Read http header items
        while (std::getline(header_stream, line)) {
                index = line.find(':');
                if(index != std::string::npos) {
                        std::string value = line.substr(index + 1);
                        this->data.insert(std::pair<std::string, std::string> (line.substr(0, index), trim(value)));
                }
        }

        // Read content, if any
        if (has_content()) {
                int content_length = get_content_length();
                char buff[BUFFER_SIZE+1];
                int total = 0;
                while (total < content_length) {
                        memset(buff, 0, BUFFER_SIZE+1);
                        int read = recv(socket_fd, buff, BUFFER_SIZE, 0);
                        if (!read) break;
                        if (read < 0) {
                                std::cerr << "Error: reading content" << std::endl;
                                break;
                        }

                        total +=read;

                        this->content += std::string(buff,read);
                }
        }
}

// Get message header
std::string HTTPResponse::get_header() {
        return this->header;
}

// Get whole response
std::string HTTPResponse::get_response() {
        return this->header + this->content;
}

// Get file op command
std::string HTTPResponse::get_command() {
        return this->command;
}

// Get url
std::string HTTPResponse::get_url() {
        return this->url;
}

// Get user
std::string HTTPResponse::get_user() {
        return this->user;
}

// Get remote path on server
std::string HTTPResponse::get_remote_path() {
        return this->remote_path;
}

// Get remote path type
int HTTPResponse::get_remote_type() {
        return this->remote_type;
}

// Get http code
int HTTPResponse::get_http_code() {
        return this->http_code;
}

int HTTPResponse::get_action() {
        return this->action;
}

// Get content length
int HTTPResponse::get_content_length() {
        return std::stoi(this->data["Content-Length"]);
}

// Check if any content
bool HTTPResponse::has_content() {
        return this->data.find("Content-Length") != data.end();
}

// Get content type
std::string HTTPResponse::get_content_type() {
        return this->data["Content-Type"];
}

// Get content
std::string HTTPResponse::get_content() {
        return this->content;
}

// Trim spaces
std::string HTTPResponse::trim(std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos)
                return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last-first+1));
}

// Replace strings
std::string HTTPResponse::replace(std::string& str, const std::string& from, const std::string& to) {
        if(from.empty()) return str;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length();
        }

        return str;
}

// Create request message to be send to server from client
HTTPRequest::HTTPRequest(std::string command, std::string url, std::string host, std::string accept, std::string content_type, std::string content) {
        int content_length = content.size();
        this->message = command + " " + url + " HTTP/1.1\r\n";
        this->message += "Host: " + host + "\r\n";
        time_t tt = time(nullptr);
        char str[40];
        time_t now = time(NULL);
        strftime(str, 40, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now));
        std::string date = str;
        this->message += "Date: " + date + "\r\n";
        this->message += "Accept: " + accept + "\r\n";
        this->message += "Accept-Encoding: identity\r\n";

        if (command.compare("PUT") == 0 || command.compare("GET") == 0) {
                this->message += "Content-Type: " + content_type + "\r\n";
                this->message += "Content-Length: " + std::to_string(content_length) + "\r\n";
                this->message += "\r\n";
                if (content_length > 0) {
                        this->message += content;
                }
        } else {
                this->message += "\r\n";
        }
}

// Create server message to be send to client
HTTPRequest::HTTPRequest(int http_code, std::string content_type, std::string content) {
        int content_length = content.size();
        std::string http_status;
        switch (http_code) {
            case 200:
                    http_status = "OK";
                    break;
            case 400:
                    http_status = "Bad Request";
                    break;
            case 404:
                    http_status = "Not Found";
                    break;
            default:
                    break;
        }
        this->message = "HTTP/1.1 "+ std::to_string(http_code) + " " + http_status + "\r\n";
        char formated_date[40];
        time_t now = time(nullptr);
        strftime(formated_date, 40, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now));
        std::string date = formated_date;
        this->message += "Date: " + date + "\r\n";
        this->message += "Content-Type: " + content_type + "\r\n";
        this->message += "Content-Length: " + std::to_string(content_length) + "\r\n";
        this->message += "Content-Encoding: identity\r\n";
        this->message += "\r\n";

        if (content_length > 0) {
                this->message += content;
        }
}

// Get length of message
int HTTPRequest::get_length() {
        return this->message.size();
}

// Send request
int HTTPRequest::send(int socked_fd) {
        int total = 0;
        int content_length = get_length();
        std::string content  = std::string(this->message.data(), content_length);
        while(total < content_length) {
                int sent = ::send(socked_fd, content.data(), content_length, 0);
                if (sent < 0) {
                        std::cerr << "Error: send failed" << std::endl;
                        break;
                }
                total += sent;

        }
}

//  Get request
std::string HTTPRequest::get_request() {
        return this->message;
}
