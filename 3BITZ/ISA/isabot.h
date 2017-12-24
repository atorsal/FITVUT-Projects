/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */

#ifndef ISABOT_H
#define ISABOT_H

#include <iostream>
#include <vector>

// exit codes
enum isabot {success, argument_error, network_error, signal_error};

// maximal length of channel name
const int MAX_CHANNEL_NAME_LENGTH = 200;

// program usage
const std::string USAGE = "Usage: isabot HOST[:PORT] CHANNELS [-s SYSLOG_SERVER] [-l HIGHLIGHT] [-h|--help]\n"
"HOST is server name (e.g. irc.freenode.net)\n"
"PORT is port number (default is 6667)\n"
"CHANNELS contains one or more channel names (channel name starts with # or &, multiple channel names are separated by comma)\n"
"-s SYSLOG_SERVER is IP address of logging (SYSLOG) server\n"
"-l HIGHLIGHT is list of keywords separated by comma (e.g. \"ip,tcp,udp,isa\")";

// structure for program data
struct params {
    std::string host;
    int port;
    std::vector<std::string> channels;
    std::vector<std::string> keywords;
    std::string syslog_server;
};

/**
 * Parse arguments from command line
 * 
 * @param argc number of arguments
 * @param argv array of arguments
 * @param isabot_params structure which holds processed arguments
 */
void parse_args(int argc, char *argv[], params & isabot_params);
/**
 * Handler for SIGINT signal
 * 
 * @param signo number of signal
 */
void sigint_handler(int signo);

#endif // ISABOT_H