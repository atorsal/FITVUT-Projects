// IPK 2017
// Project 2
// Author: David Bolvansky xbolva00

#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H

#include <string>
#include <fstream>

// Constants
#define BUFFER_SIZE 1024
#define DEFAULT_PORT 21011

// Structure to collect some data to be parsed to threads
typedef struct {
        std::string user_name;
        std::ofstream log;
        std::string log_file;
        bool logging;
        int socket_fd;
} chat_t;

// Class to handle chat functionality, start chat threads, etc..
class ChatHandler {
    chat_t data;
    public:
    ChatHandler(int socket_fd);
    void set_user_name(std::string user_name);
    void set_logging(bool logging);
    void set_log_file(std::string log_file);
    bool start_threads();

    private:
    void send_logged_in_message();
    static void * receive_messages(void * arg);
    static void * send_messages(void * arg);
    static void send_data(int socket_fd, std::string message);
    static void sigint_handler(int code);
};


#endif // CHAT_HANDLER_H
