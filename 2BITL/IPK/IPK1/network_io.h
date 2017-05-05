// IPK 2017
// Project 1
// Author: David Bolvansky xbolva00

#ifndef NETWORK_IO_H
#define NETWORK_IO_H

#include <map>

// Contstants
const int ONE_CHAR = 1; // read one char
const int BUFFER_SIZE = 4096; // block size to read
const int DEFAULT_PORT = 6677; // default port for connection
const int TEMP_SIZE = 1024; // size for reading values

// File types
const int TYPE_FILE = 1;
const int TYPE_FOLDER = 2;


const int ACTION_FILTER = 1;
const int ACTION_COPY = 2;
const int ACTION_MOVE = 3;
const int ACTION_REMOVE_DUPLICATES = 4;

class HTTPResponse {
        int socket_fd;
        std::string header;
        std::string command;
        std::string url;
        std::string http;
        std::string content;
        std::string user;
        std::string remote_path;
        int remote_type;
        int http_code;
        int action;
        std::map<std::string, std::string> data;
        public:
        HTTPResponse(int socket_fd);
        std::string get_header();
        std::string get_response();

        std::string get_command();

        std::string get_url() ;
        std::string get_user();
        std::string get_remote_path();
        int get_remote_type();
        int get_http_code();
        int get_action();
        int get_content_length();

        bool has_content();

        std::string get_content_type();

        std::string get_content();

        static std::string replace(std::string& str, const std::string& from, const std::string& to);

        private:
        std::string trim(std::string& str);
};

class HTTPRequest {
        std::string message;
        public:
        HTTPRequest(std::string command, std::string url, std::string host, std::string accept, std::string content_type, std::string content);
        HTTPRequest(int http_code, std::string content_type, std::string content);
            int get_length();

            int send(int socked_fd);

            std::string get_request() ;
};

#endif // NETWORK_IO_H
