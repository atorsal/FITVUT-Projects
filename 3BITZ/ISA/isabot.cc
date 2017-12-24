/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */

#include <iostream>
#include <sstream>
#include <vector>

#include <csignal>
#include <cctype>
#include <getopt.h>

#include "isabot.h"
#include "irc_bot.h"
#include "utils.h"

// control isabot, for SIGINT handler
static irc_bot isabot;

void parse_args(int argc, char *argv[], params & isabot_params) {
    isabot_params.syslog_server = "127.0.0.1";

    struct option long_options[] =
    {
        { "help", no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, 0 }
    };

    opterr = 0; // off getopt messages
    int opt;
    while ((opt = getopt_long(argc, argv, "hs:l:", long_options, nullptr)) != -1) {
        switch (opt) {
             case 'l':
                 isabot_params.keywords = utils::string_to_vector(optarg, ',');
                 break;
             case 's':
                 isabot_params.syslog_server = optarg;
                 break;
             case 'h':
                 std::cout << USAGE << std::endl;
                 exit(isabot::success);
             default:
                 throw argument_exception("isabot: argument error\n\n" + USAGE);
        }
    }

    if (argc - optind != 2) {
        throw argument_exception("isabot: no required arguments\n\n" + USAGE);
    }

    if (isabot_params.syslog_server.empty()) {
        throw argument_exception("isabot: no syslog address\n\n" + USAGE);
    }

    // IRC host, port
    std::string arg = argv[optind++];
    std::size_t index = arg.find("://");

    if (index != std::string::npos) {
        arg = arg.substr(index + 3, arg.size());
    }

    // get port
    index = arg.find(":");
    if (index != std::string::npos) {
        isabot_params.host = arg.substr(0, index);
        std::string port = arg.substr(index + 1, arg.size());
        if (!isdigit(port[0])) {
            throw argument_exception("isabot: invalid port value\n\n" + USAGE);
        }

        try {
            isabot_params.port = stoi(port);
        } catch (...) {
            throw argument_exception("isabot: invalid port value\n\n" + USAGE);
        }
    } else {
        isabot_params.host = arg; 
        isabot_params.port = DEFAULT_IRC_SERVER_PORT;
    }

     // IRC Channels
     isabot_params.channels = utils::string_to_vector(argv[optind], ',');
     if (isabot_params.channels.empty()) {
        throw argument_exception("isabot: no channel(s)\n\n" + USAGE);
     }
     
     // Channel names checks
     for (std::string channel : isabot_params.channels) {
         if (channel[0] != '#' && channel[0] != '&') {
            throw argument_exception("isabot: invalid channel name\n\n" + USAGE);
         } else if (channel.size() > MAX_CHANNEL_NAME_LENGTH) {
             throw argument_exception("isabot: channel name too long\n\n" + USAGE);
         } else if (channel.size() == 1) {
            throw argument_exception("isabot: missing channel name\n\n" + USAGE);
        }
     }
}

void sigint_handler(int signo) {
    if (signo == SIGINT) {
        isabot.quit_and_disconnect();
        exit(isabot::success);
    }
}

int main(int argc, char *argv[]) {
    params isabot_params;
    
    // try to parse args
    try {
        parse_args(argc, argv, isabot_params);
    } catch (argument_exception & e) {
        std::cerr << e.what() << std::endl;
        return isabot::argument_error;
    }

    // set up SIGINT handler
    if (std::signal(SIGINT, sigint_handler) == SIG_ERR) {
        std::cerr << "isabot: unable to set SIGINT handler" << std::endl;
        return isabot::signal_error;
    }

    // create IRC bot
    isabot = irc_bot{isabot_params.host, isabot_params.port, isabot_params.channels, isabot_params.keywords, isabot_params.syslog_server};

    // run run
    try {
        isabot.connect();
        isabot.run();
        isabot.quit_and_disconnect();
    } catch (network_exception & e) {
        isabot.disconnect();
        std::cerr << e.what() << std::endl;
        return isabot::network_error;
    }

    return isabot::success;
}