#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "Irc.hpp"
# include "Server.hpp"
# define VALID_LEN 16

class Server;

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

int			parseCommand(std::string cmd_line, cmd_struct &cmd_infos);
void		addToClientBuffer(Server *server, int const client_fd, std::string reply);
Client&		retrieveClient(Server *server, int const client_fd);
std::string	getListOfMembers(Channel &channel);
std::string	getChannelName(std::string msg_to_parse);
std::string	findNickname(std::string msg_to_parse);
bool		isAlreadyUsed(Server *server, int client_fd, std::string new_nickname);
std::string	getReason(std::string msg_to_parse);
Client*		getClient(Server *server, int const client_fd);

// #######################
// ###### COMMANDS #######
// #######################

void	invite(Server *server, int const client_fd, cmd_struct cmd_infos);
void	join(Server *server, int const client_fd, cmd_struct cmd_infos);
void	kick(Server *server, int const client_fd, cmd_struct cmd_infos);
void	kill(Server *server, int const client_fd, cmd_struct cmd_infos);
void	list(Server *server, int const client_fd, cmd_struct cmd_infos);
void	names(Server *server, int const client_fd, cmd_struct cmd_infos);
void	nick(Server *server, int const client_fd, cmd_struct cmd_infos);
void	notice(Server *server, int const client_fd, cmd_struct cmd_infos);
void	oper(Server *server, int const client_fd, cmd_struct cmd_infos);
int		pass(Server *server, int const client_fd, cmd_struct cmd_infos);
void	part(Server *server, int const client_fd, cmd_struct cmd_infos);
int		ping(Server *server, int const client_fd, cmd_struct &cmd);
void	privmsg(Server *server, int const client_fd, cmd_struct cmd_infos);
void	quit(Server *server, int const client_fd, cmd_struct cmd_infos);
void	topic(Server *server, int const client_fd, cmd_struct cmd_infos);
void	user(Server *server, int const client_fd, cmd_struct cmd_infos);

#endif