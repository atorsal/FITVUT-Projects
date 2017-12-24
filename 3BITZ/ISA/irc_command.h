/**
 * ISA Project - ISABOT
 * Author: David Bolvansky (xbolva00)
 */

#ifndef IRC_COMMAND_H
#define IRC_COMMAND_H

#include <iostream>
#include <vector>

/* Command codes - RFC1459 */
const int RPL_NAMREPLY = 353;
const int ERR_NOSUCHCHANNEL = 403;
const int ERR_CANNOTSENDTOCHAN = 404;
const int ERR_TOOMANYCHANNELS = 405;
const int ERR_NICKNAMEINUSE = 433;
const int ERR_CHANNELISFULL = 471;
const int ERR_INVITEONLYCHAN = 473;
const int ERR_BANNEDFROMCHAN = 474;

/* IRC output line analyzer and parser */
class irc_command {
    std::string command;
    std::vector<std::string> command_parts;
    std::string nickname;
    std::string message;
    std::string channel;
public:
    /**
     * IRC Command Constructor
     * 
     * @line Line containg IRC command
    */
    irc_command(std::string line);
    /**
     * Check if PING command
     * 
     * @return true if PING command
     */
    bool is_ping();
    /**
     * Get PING server
     * 
     * @return server from PING command
     */
    std::string get_ping_server();
    /**
     * Check if PRIVMSG command
     * 
     * @return true if PRIVMSG command
     */
    bool is_privmsg();
    /**
     * Check if NOTICE command
     * 
     * @return true if NOTICE command
     */
    bool is_notice();
    /**
     * Check if JOIN command
     * 
     * @return true if JOIN command
     */
    bool is_join();
    /**
     * Check if QUIT command
     * 
     * @return true if QUIT command
     */
    bool is_quit();
    /**
     * Check if PART command
     * 
     * @return true if PART command
     */
    bool is_part();
    /**
     * Check if KICK command
     * 
     * @return true if KICK command
     */
    bool is_kick();
    /**
     * Check if NICK command
     * 
     * @return true if NICK command
     */
    bool is_nick();
    /**
     * Check if RPL_NAMREPLY message
     * 
     * @return true if RPL_NAMREPLY message
     */
    bool is_names_reply();
    /**
     * Check if critical error
     * 
     * @return true if critical error
     */
    bool is_error();
    /**
     * Parse line of IRC log
     */
    void parse();
    /**
     * Get parsed nickname
     * 
     * @return nickname
     */
    std::string get_nickname();
    /**
     * Get parsed message
     * 
     * @return message
     */
    std::string get_message();
    /**
     * Get parsed channel
     * 
     * @return channel
     */
    std::string get_channel();
    /**
     * Check if message starts with ?today
     * 
     * @return true if message starts with ?today
     */
    bool is_bot_today();
    /**
     * Check if valid ?msg message
     * 
     * @return true if valid ?msg message
     */
    bool is_body_msg();

    /**
     * Get today's date in format dd.mm.yyyy
     * 
     * @return date in format dd.mm.yyyy
     */
    std::string get_today();
};


#endif //IRC_COMMAND_H
