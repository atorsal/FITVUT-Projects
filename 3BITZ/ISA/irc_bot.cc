/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */

#include <iostream>
#include <algorithm>
#include <locale>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "irc_bot.h"
#include "utils.h"
#include "irc_command.h"

irc_bot::irc_bot(std::string address, int port, std::vector<std::string> channels, std::vector<std::string> keywords, std::string syslog_server) : 
    address{address}, port{port}, channels{channels}, keywords{keywords}, syslog{syslog_server}, connected{false} { }


void irc_bot::connect() {
    struct addrinfo hints, *res, *address_info;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &address_info) != 0) {
        throw network_exception("ircbot: cannot connect, getaddrinfo failed");
    }
    
    // try to connect to the IRC server
    for (res = address_info; res != NULL; res = res->ai_next) {
            if ((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
                continue;
            }
            

            if (::connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
                freeaddrinfo(address_info);
                close(socket_fd);
                throw network_exception("ircbot: connect failed");
            } else {
                // successfully connected
                break;
            }
        }
    
     freeaddrinfo(address_info);
     connected = true;
        
     // connect to the syslog server only if we have some keywords
     if (keywords.size() > 0) {
        syslog.connect();
     }
}
void irc_bot::run() {
    if (!connected) {
        throw network_exception("ircbot: cannot run, not connected");
    }

    // join channel(s)
    send_message("NICK " + FIT_LOGIN + CLRF);
    send_message("USER " + FIT_LOGIN + " " + FIT_LOGIN + " " + FIT_LOGIN + " :" + FIT_LOGIN + CLRF);
    send_message("JOIN " + utils::vector_to_string(channels) + CLRF);

    int read = 0;
    char buffer[sizeof(char)];
    std::string line;
    std::size_t crlf_pos;

    while(true) {
            memset(buffer, 0, sizeof(char));
            
            // read char by char
            read = recv(socket_fd, buffer, sizeof(char), 0);

            if (read == 0) {
                break;
            }

            // check for recv failure
            if (read < 0) {
                throw network_exception("ircbot: read failed");
            }

            // append char to string
            line += std::string(buffer, read);

            if ((crlf_pos = line.find(CLRF)) != std::string::npos) {
                line = line.substr(0, crlf_pos);
                // process IRC message
                irc_command command{line};
                
                if (command.is_ping()) {
                    if (command.get_ping_server().empty()) {
                        send_message("PONG" + CLRF);
                    } else {
                        send_message("PONG " + command.get_ping_server() + CLRF);
                    }
                } else if (command.is_privmsg() || command.is_notice()) {
                    command.parse();
                    if (should_log(command.get_message())) {
                        syslog.log_message(command.get_nickname(), command.get_message());
                    }

                    if (command.is_privmsg() && !command.get_channel().empty()) {
                        if (command.is_bot_today()) {
                            send_message("PRIVMSG " + command.get_channel() + " :" + command.get_today() + CLRF);
                        } else if (command.is_body_msg()) {
                            if (is_online_user(command.get_channel(), command.get_nickname())) {
                                send_message("PRIVMSG " + command.get_channel() + " :" + command.get_nickname() + ":" + command.get_message() + CLRF);
                            } else {
                                add_pending_message(command.get_channel(), command.get_nickname(), command.get_message());
                            }
                        }
                    }
                } else if (command.is_join()) {
                    command.parse();
                    set_user_as_online(command.get_channel(), command.get_nickname());
                    send_pending_messages(command.get_channel(), command.get_nickname());
                } else if (command.is_quit()) {
                    command.parse();
                    set_user_as_offline(command.get_nickname());
                } else if (command.is_part()) {
                    command.parse();
                    set_user_as_offline(command.get_channel(), command.get_nickname());
                } else if (command.is_kick()) {
                    command.parse();
                    if (command.get_message() == FIT_LOGIN) {
                        throw network_exception("ircbot: Kicked on channel " + command.get_channel());
                    }
                    set_user_as_offline(command.get_channel(), command.get_message());
                } else if (command.is_nick()) {
                    command.parse();
                    change_nickname(command.get_nickname(), command.get_message());
                    send_pending_messages(command.get_message());
                } else if (command.is_names_reply()) {
                    command.parse();
                    update_users(command.get_channel(), command.get_message());
                } else if (command.is_error()) {
                    command.parse();
                    throw network_exception("ircbot: " + command.get_message());
                }

                // line processed, reset it
                line = "";
            }
    }
}

void irc_bot::send_message(std::string message) {
    if (!connected) {
        throw network_exception("syslog: cannot send command, not connected");
    }

    int length = message.size(); 
    const char *data = message.c_str();
    int sent = 0;
    
    // try to send all data
    while (length > 0) {
        sent = ::send(socket_fd, data, length, 0);

        if (sent == 0) {
            break;
        } 
        
        // check for send failure
        if (sent < 0) {
            throw network_exception("ircbot: send failed");
        }

        data += sent;
        length -= sent;
    }
}

bool irc_bot::should_log(std::string message) {
    // if empty message or no keywords, nothing to log
    if (message.empty() || keywords.empty()) {
        return false;
    }

    // Split message using space char delimeter
    std::vector<std::string> words = utils::string_to_vector(message, ' ');
    
    for (std::string & keyword : keywords) {
        // if there is a keyword in words, we want to log this message
        if (std::find(words.begin(), words.end(), keyword) != words.end()) {
            return true;
        }
    }
    
    return false;
}

void irc_bot::update_users(std::string channel, std::string online_users) {
    // remove special chars from nicknames
    for (char & c : online_users) {
        if (c == '@' || c == '+') {
            c = ' ';
        }
    }

    // if new channel, add it to our map
    if (channel_users.find(channel) == channel_users.end()) {
        channel_users.insert(std::pair<std::string, user_map>(channel, user_map{}));
    }

    // refresh list of users and their state of activity on the channel (online or not)
    user_map & users = channel_users.at(channel);
    std::vector<std::string> nicknames = utils::string_to_vector(online_users, ' ');

    for (std::string & nickname : nicknames) {
        if (users.find(nickname) == users.end()) {
            users.insert(std::pair<std::string, irc_user>(nickname, irc_user{}));
        }
    }
    for (auto & user : users) {
        user.second.online = std::find(nicknames.begin(), nicknames.end(), user.first) != nicknames.end();
    }
}

void irc_bot::set_user_as_online(std::string channel, std::string nickname) {
    irc_user * user = get_user(channel, nickname);
    if (user == nullptr) {
        return;
    }

    // set user as online at the channel
    user->online = true;    
}

void irc_bot::set_user_as_offline(std::string channel_list, std::string nickname) {
    std::vector<std::string> channels;
    channels = utils::string_to_vector(channel_list, ',');

    irc_user * user;
    // set user as offline at the list of channels
    for (std::string & channel : channels) {
        user = get_user(channel, nickname);
        if (user == nullptr) {
            continue;
        }

        user->online = false;
    }    
}

void irc_bot::set_user_as_offline(std::string nickname) {
    for (auto & channel : channel_users) {
        user_map & channel_users = channel.second;
        if (channel_users.find(nickname) != channel_users.end()) {
            channel_users.at(nickname).online = false;
        }
    }
}

bool irc_bot::is_online_user(std::string channel, std::string nickname) {
    irc_user * user = get_user(channel, nickname);
    if (user == nullptr) {
        return false;
    }

    return user->online;    
}
void irc_bot::change_nickname(std::string old_nickname, std::string new_nickname) {
    if (old_nickname == new_nickname) {
        return;
    }

    for (auto & channel : channel_users) {
        auto & channel_users = channel.second;
        if (channel_users.find(new_nickname) != channel_users.end()) {
            return;
        }

        const auto & user = channel.second.find(old_nickname);
        if (user != channel_users.end()) { 
            channel_users[new_nickname] = channel_users[old_nickname];
            channel_users.erase(user);
        }
    }
}

void irc_bot::send_pending_messages(std::string channel, std::string nickname) {
    irc_user * user = get_user(channel, nickname);
    if (user == nullptr) {
        return;
    }
    
    for (std::string & message : user->pending_messages) {
        send_message("PRIVMSG " + channel + " :" + nickname + ":" + message + CLRF);
    }

    user->pending_messages.clear();
}

void irc_bot::send_pending_messages(std::string nickname) {
    for (auto & channel : channel_users) { 
        send_pending_messages(channel.first, nickname);
    }
}

void irc_bot::add_pending_message(std::string channel, std::string nickname, std::string message) {
    irc_user * user = get_user(channel, nickname);
    if (user == nullptr) {
        return;
    }
    user->pending_messages.push_back(message);
}

irc_user * irc_bot::get_user(std::string channel, std::string nickname) {
    if (channel_users.find(channel) == channel_users.end()) {
        return nullptr;
    }

    user_map & users = channel_users.at(channel);
    if (users.find(nickname) == users.end()) {
        users.insert(std::pair<std::string, irc_user>(nickname, irc_user{}));
    }

    return &users.at(nickname);
}

void irc_bot::quit_and_disconnect() {
    if (connected) {
        std::string quit_msg = "QUIT" + CLRF;
        ::send(socket_fd, quit_msg.c_str(), quit_msg.size(), 0);
        disconnect();
    }
}
void irc_bot::disconnect() {
    if (connected) {
        close(socket_fd);
        connected = false;
    }

    syslog.disconnect();
}