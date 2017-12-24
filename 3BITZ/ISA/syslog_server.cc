/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>

#include <ctime>

#include "syslog_server.h"
#include "utils.h"

syslog_server::syslog_server(std::string address) : address{address}, connected{false} {}

void syslog_server::connect() {
    struct addrinfo hints, *res, *address_info;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if (getaddrinfo(address.c_str(), std::to_string(SYSLOG_SERVER_PORT).c_str(), &hints, &address_info) != 0) {
        throw network_exception("syslog: cannot connect, getaddrinfo failed");
    }
   
   for (res = address_info; res != NULL; res = res->ai_next) {
            if ((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
                continue;
            }

            server = *(struct sockaddr_in*)res->ai_addr;
            break;
        }
    
     freeaddrinfo(address_info);
     connected = true;
}

void syslog_server::log_message(std::string nickname, std::string message) {
    if (!connected) {
        throw network_exception("syslog: cannot log message, not connected");
    }
    
    // create Syslog message
    std::string log  = "<" + PRIORITY + ">" + get_timestamp() + " " + get_ip_address() + " isabot " + nickname + ":" + message;
    if (sendto(socket_fd, log.c_str(), log.size(), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
        throw network_exception("syslog: sendto failed");
    }
}

std::string syslog_server::get_timestamp() {
    static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);
    char timestamp[20];

    // make timestamp suitable for Syslog messages
    snprintf(timestamp, sizeof(timestamp), "%s %2d %.2d:%.2d:%.2d", months[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    return timestamp;
}

std::string syslog_server::get_ip_address() {
    if (!my_ip_adress.empty()) {
        return my_ip_adress;
    }

    struct sockaddr_in *addr;
    ifaddrs *addrs, *tmp = NULL;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp)  {
        if ((tmp->ifa_flags & IFF_LOOPBACK) == 0 /* we dont want loopback address */ && tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
            {
                addr = (struct sockaddr_in *)tmp->ifa_addr;
                // found IP address of sender
                my_ip_adress = inet_ntoa(addr->sin_addr);
            }

        tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);
    return my_ip_adress;
}

void syslog_server::disconnect() {
    if (connected) {
        // close connection
        close(socket_fd);
        connected = false;
    }
}