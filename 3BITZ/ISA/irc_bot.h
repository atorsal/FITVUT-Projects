#ifndef IRC_BOT_H
#define IRC_BOT_H

#include <iostream>
#include <vector>
#include <map>

#include <cstring>

#include "syslog_server.h"

// default IRC port
const int DEFAULT_IRC_SERVER_PORT = 6667;
// sequence of CLRF
const std::string CLRF = "\r\n";
// my FIT login
const std::string FIT_LOGIN = "xbolva00";

// structure of user data
struct irc_user {
    bool online;
    std::vector<std::string> pending_messages;
};

// case insensitive comparator for maps
struct case_insensitive_comparator {
    bool operator()(const std::string& str1, const std::string& str2) {
        return ::strcasecmp(str1.c_str(), str2.c_str()) < 0;
    }
};

// aliases
using user_map = std::map<std::string, irc_user, case_insensitive_comparator>;
using channel_map = std::map<std::string, user_map, case_insensitive_comparator>;

class irc_bot {
    std::string address;
    int port;
    std::vector<std::string> channels;
    std::vector<std::string> keywords;
    syslog_server syslog;
    int socket_fd;
    bool connected;
    channel_map channel_users;
public:
    /**
     * Default constructor
     */
    irc_bot() {}
    /**
     * Constructor
     * 
     * @param adress irc server adress
     * @param port irc server port
     * @param channels vector of channels
     * @param keywords vector of keywords
     * @param syslog_server syslog server adress
     */
    irc_bot(std::string address, int port, std::vector<std::string> channels, std::vector<std::string> keywords, std::string syslog_server);
    /**
     * Connect to the IRC server
     */
    void connect();
    /**
     * Run bot, receive and parse messages, perform bot actions, log messages
     */
    void run();
    /**
     * Quit (send QUIT command) and disconnect from the IRC server
     */
    void quit_and_disconnect();
    /**
     * Disconnect from the IRC server
     */
    void disconnect();
    /**
     * Send IRC message
     * 
     * @param message message to be sent
     */
    void send_message(std::string message);
    /**
     * Check if message should be logged, i.e. contains any keyword
     * 
     * @param message message to be sent
     */
    bool should_log(std::string message);
    /**
     * Update list of users at the channel
     * 
     * @param channel channel 
     * @param online_users list of connected users at the channel
     */
    void update_users(std::string channel, std::string online_users);
    /**
     * Set the user as online (connected) at the channel
     * 
     * @param channel channel 
     * @param nickname nickname of the user
     */
    void set_user_as_online(std::string channel, std::string nickname);
        /**
     * Set the user as offline (disconnected) at the channel
     * 
     * @param channel channel 
     * @param nickname nickname of the user
     */
    void set_user_as_offline(std::string channel_list, std::string nickname);
    /**
     * Set the user as offline (disconnected) at any channel
     * 
     * @param nickname nickname of the user
     */
    void set_user_as_offline(std::string nickname);
    /**
     * Check if the user is online at the channel
     * 
     * @param channel channel 
     * @param nickname nickname of user
     * @return true of connected on channel, false otherwise
     */
    bool is_online_user(std::string channel, std::string nickname);
    /**
     * Send pending messages for the user at the specified channel
     * 
     * @param channel channel 
     * @param nickname nickname of the user
     */
    void send_pending_messages(std::string channel, std::string nickname);
    /**
     * Send pending messages for the user at any channel
     * 
     * @param channel channel 
     * @param nickname nickname of the user
     */
    void send_pending_messages(std::string nickname);
    /**
     * Get the user at the channel
     * 
     * @param channel channel 
     * @param nickname nickname of the user
     * @return if found, pointer to user data structure, nullptr otherwise
     */
    irc_user * get_user(std::string channel, std::string nickname);
    /**
     * Add pending message for the user at the specified channel
     * 
     * @param channel channel 
     * @param nickname nickname of user
     * @param message message to be saved as pending
     */
    void add_pending_message(std::string channel, std::string nickname, std::string message);
    /**
     * Change nickname of the user
     * 
     * @param old_nickname old user nickname
     * @param new_nickname new user nickname
     */
    void change_nickname(std::string old_nickname, std::string new_nickname);
};


#endif //IRC_BOT_H
