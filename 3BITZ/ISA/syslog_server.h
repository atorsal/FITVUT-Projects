/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */


#ifndef SYSLOG_SERVER_H
#define SYSLOG_SERVER_H

#include <iostream>
#include <netinet/in.h>


// Default Syslog port
const int SYSLOG_SERVER_PORT = 514;
// Local0 Facility 
const int LOCAL0_FACILITY = 16;
// Informational Severity
const int INFORMATIONAL_SEVERITY = 6;
// Syslog message priority
const std::string PRIORITY = std::to_string((LOCAL0_FACILITY * 8) + INFORMATIONAL_SEVERITY); // RFC 3164

// class to control connection to syslog server
class syslog_server {
    // server adress
    std::string address;
    // socket file descriptor
    int socket_fd;
    // server data info
    struct sockaddr_in server;
    // my ip adress
    std::string my_ip_adress;
    bool connected;
public:
    /**
     * Constructor
     * @param address server adress
     */
    syslog_server(std::string address);
    /**
     * Default cnstructor
     */
    syslog_server() {}
    /**
     * Connect to the Syslog server
     */
    void connect();
    /**
     * Log a message to Syslog server
     * @param nickname nickname of user
     * @param message message to log
     */
    void log_message(std::string nickname, std::string message);
    /**
     * Get the current timestamp for Syslog message
     * @return the current timestamp
     */
    std::string get_timestamp();
    /**
     * Get IP address of the sender
     * @return IP adress of the sender
     */
    std::string get_ip_address();
    /**
     * Disconnect from the Syslog server
     */
    void disconnect();
};

#endif // SYSLOG_SERVER_H
