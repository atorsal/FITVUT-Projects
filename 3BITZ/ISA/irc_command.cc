/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */

#include <cctype>
#include <ctime>

#include "irc_command.h"
#include "utils.h"

irc_command::irc_command(std::string command) : command{command} , command_parts{utils::string_to_vector(command, ' ')} {}

bool irc_command::is_ping() {
    return command_parts[0] == "PING";
}

std::string irc_command::get_ping_server() {
    return (command_parts.size() > 1) ? command_parts[1] : std::string{};
}

bool irc_command::is_privmsg() {
    if (command_parts.size() > 3) {
        if (command_parts[1] == "PRIVMSG") {
            return true;
        }
    }

    return false;
} 

bool irc_command::is_notice() {
    if (command_parts.size() > 3) {
        if (command_parts[1] == "NOTICE") {
            return true;
        }
    }

    return false;
}

bool irc_command::is_join() {
    if (command_parts.size() > 2) {
        if (command_parts[1] == "JOIN") {
            return true;
        }
    }

    return false;
}

bool irc_command::is_quit() {
    if (command_parts.size() > 2) {
        if (command_parts[1] == "QUIT") {
            return true;
        }
    }

    return false;
}

bool irc_command::is_part() {
    if (command_parts.size() > 2) {
        if (command_parts[1] == "PART") {
            return true;
        }
    }

    return false;
} 

bool irc_command::is_kick() {
    if (command_parts.size() > 2) {
        if (command_parts[1] == "KICK") {
            return true;
        }
    }

    return false;
}

bool irc_command::is_nick() {
    if (command_parts.size() > 2) {
        if (command_parts[1] == "NICK") {
            return true;
        }
    }

    return false;
}

bool irc_command::is_names_reply() {
    if (command_parts.size() > 2) {
        if (command_parts[1] == std::to_string(RPL_NAMREPLY)) {
            return true;
        }
    }

    return false;
}

bool irc_command::is_error() {
    if (command_parts.size() > 2) {
        int error = 0;
        try {
            error = stoi(command_parts[1]);
        } catch(...) {
            return false;
        }

        switch (error) {
            case ERR_NOSUCHCHANNEL:
            case ERR_CANNOTSENDTOCHAN:
            case ERR_TOOMANYCHANNELS:
            case ERR_CHANNELISFULL:
            case ERR_INVITEONLYCHAN:
            case ERR_BANNEDFROMCHAN:
            case ERR_NICKNAMEINUSE:
                return true;
        }
    }
    
    return false;
}
    

void irc_command::parse() {
    nickname = command_parts[0];
    std::size_t channel_pos = 0, msg_pos = 0;
    std::size_t channel_prefix_pos = 0;
    for (std::string & part : command_parts) {
        if (part[0] == ':') {
            channel_prefix_pos = 1;
        }
        if (part[channel_prefix_pos] == '#' || part[channel_prefix_pos] == '&') {
            break;
        }

        if (part == "NOTICE") {
            channel_pos++;
            break;
        }

        channel_prefix_pos = 0;

        channel_pos++;
    }


    // parse nickname
    nickname = nickname.substr(1, nickname.size());
    std::size_t pos = nickname.find("!");
    if (pos == std::string::npos) {
        pos = nickname.find("@");
        if (pos == std::string::npos) {
            pos = nickname.size();
        }
    }

    nickname = nickname.substr(0, pos);

    // parse new nick
    if (is_nick()) {
        message = command_parts[2];
        pos = message.find(":");
        if (pos != std::string::npos) {
            message = message.substr(pos + 1, message.size());
        }
        return;
    }

    // no channel, probably it was quit command
    if (channel_pos == 0 || command_parts.size() == channel_pos) {
        // error parsing
        if (is_error()) {
            if (command_parts.size() > 4) {
                message = command.substr(command.find(command_parts[4]), command.size());
                pos = message.find(":");
                if (pos != std::string::npos) {
                    nickname = command_parts[msg_pos];
                    // get error reason
                    message = message.substr(pos + 1, message.size());
                }
            }
        }

        return;
    }

    // get channel
    channel = command_parts[channel_pos];
    if (channel_prefix_pos) {
        channel = channel.substr(channel_prefix_pos, channel.size());
    }

    // get message position
    msg_pos = channel_pos + 1;
    if (command_parts.size() < msg_pos + 1) {
        return;
    }

    // get message
    std::string msg = command_parts[msg_pos];
    message = command.substr(command.find(channel) + channel.size(), command.size());

    // if kicking, set message to name of kicked user and return
    if (is_kick()) {
        message = msg;
        return; 
    }

    // error parsing
    if (is_error()) {
        pos = message.find(":");
        if (pos != std::string::npos) {
            nickname = command_parts[msg_pos];
            // get error reason
            message = message.substr(pos + 1, message.size());
        }
        return;
    } 
    
    // strip non-message
    message = message.substr(message.find(msg) + 1 , command.size());
}

std::string irc_command::get_nickname() {
    return nickname;
}

std::string irc_command::get_message() {
    return message;
}

std::string irc_command::get_channel() {
    return channel;
}

bool irc_command::is_bot_today() {
    return message == "?today";
}

bool irc_command::is_body_msg() {
    std::vector<std::string> msg_parts = utils::string_to_vector(message, ' ');
    if (msg_parts.size() < 2) {
        return false;
    }
    if (msg_parts[0] != "?msg") {
        return false;
    }

    std::string data = message.substr(message.find(msg_parts[1]), message.size());
    std::size_t index = data.find(":");
    if (index == std::string::npos) {
        return false;
    }

    nickname = data.substr(0, index);
    message = data.substr(index + 1, data.size());

    if (nickname.empty() || message.empty()) {
        return false;
    }
    return true;
}

std::string irc_command::get_today() {
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);
    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp), "%.2d.%.2d.%.4d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    return timestamp;
}