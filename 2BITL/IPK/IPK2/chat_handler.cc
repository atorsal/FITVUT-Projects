// IPK 2017
// Project 2
// Author: David Bolvansky xbolva00

#include <iostream>
#include <string>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "chat_handler.h"

// Data for SIGINT handler
chat_t glob_data;

// Constructor, set socket file descriptor
ChatHandler::ChatHandler(int socket_fd) {
        this->data.socket_fd = socket_fd;
}

// Set user name
void ChatHandler::set_user_name(std::string user_name) {
        this->data.user_name = user_name;
}

// Enable logging
void ChatHandler::set_logging(bool logging) {
        this->data.logging = logging;
}

// Set log file
void ChatHandler::set_log_file(std::string log_file) {
        this->data.log_file = log_file;
}

// Start threads for receiving messages and user input
bool ChatHandler::start_threads() {
        signal(SIGINT, sigint_handler);
        // Threads
        pthread_t receiver_thread, input_thread;
        // Run threads
        int err = pthread_create(&receiver_thread, NULL, receive_messages, (void *) &this->data);
        if (err != 0) return false;
        err = pthread_create(&input_thread, NULL, send_messages, (void *) &this->data);
        if (err != 0) return false;

        if (this->data.logging) {
                this->data.log.open(this->data.log_file);
        }

        glob_data.socket_fd = this->data.socket_fd;
        glob_data.user_name = this->data.user_name;

        // Send logged in message
        send_logged_in_message();

        pthread_join(receiver_thread, NULL);
        pthread_join(input_thread, NULL);

        if (this->data.logging) {
                this->data.log.close();
        }

        return true;
}

// Send message to server
void ChatHandler::send_data(int socket_fd, std::string message)
{
        const char *data = message.data();
        int length = message.size();
        int sent = 0;

        while (length > 0) {
                sent = send(socket_fd, data, length, MSG_NOSIGNAL);

                if (sent <= 0) {
                        std::cerr << "Error: Sending data to the char server failed! Server down?" << std::endl;
                        exit(EXIT_FAILURE);
                }

                data += sent;
                length -= sent;
        }
}

// Wait and receive message
void * ChatHandler::receive_messages(void * arg) {
        int read = 0;
        char buffer[BUFFER_SIZE+1];
        std::string message;
        chat_t *data = (chat_t *) arg;

        while(true) {
                memset(buffer, 0, BUFFER_SIZE);
                read = recv(data->socket_fd, buffer, BUFFER_SIZE, 0);

                if (read <= 0) {
                        std::cerr << "Error: Receiving data from the chat server failed! Server down?" << std::endl;
                        exit(EXIT_FAILURE);
                }

                message = std::string(buffer, read);
                std::cout << message;

                if (data->logging) {
                        data->log << message;
                        data->log.flush();
                }
        }
}

// Wait and get user input
void * ChatHandler::send_messages(void * arg) {
        std::string input;
        std::string message;
        chat_t *data = (chat_t *) arg;

        // Read user messages
        while (std::getline(std::cin, input)) {
                if (input.empty()) continue;
                message = data->user_name + ": " + input + "\r\n";
                send_data(data->socket_fd, message);

                if (data->logging) {
                        data->log << message;
                        data->log.flush();
                }
        }

        // End of input, exit client
        exit(EXIT_SUCCESS);
}

// Send looged in message to the chat server
void ChatHandler::send_logged_in_message() {
        std::string message = this->data.user_name + " logged in\r\n";
        send_data(this->data.socket_fd, message);
}

// Handle SIGINT, send logged out message and exit program
void ChatHandler::sigint_handler(int code) {
        std::string message = glob_data.user_name + " logged out\r\n";
        send_data(glob_data.socket_fd, message);
        close(glob_data.socket_fd);
        exit(EXIT_SUCCESS);
}
