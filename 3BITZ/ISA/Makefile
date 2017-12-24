# ISA Project - ISABOT
# Author: David Bolvansky (xbolva00)
CC=g++
CFLAGS=-std=c++14 -Wall -Werror -Wextra -pedantic -O2

all: isabot

isabot.o: isabot.cc isabot.h irc_bot.h syslog_server.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@
irc_bot.o: irc_bot.cc irc_bot.h syslog_server.h utils.h irc_command.h
	$(CC) $(CFLAGS) -c $< -o $@
syslog_server.o: syslog_server.cc syslog_server.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@
irc_command.o: irc_command.cc irc_bot.h syslog_server.h irc_command.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@
utils.o: utils.cc utils.h
	$(CC) $(CFLAGS) -c $< -o $@

isabot: isabot.o irc_bot.o syslog_server.o irc_command.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f isabot *.o
