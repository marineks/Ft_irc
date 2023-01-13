#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "Irc.hpp"
# include "Server.hpp"
# define VALID_LEN 17

class Server;

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

int		parseCommand(std::string cmd_line, cmd_struct &cmd_infos);
Client&	retrieveClient(Server *server, int const client_fd);
std::string	findNickname(std::string msg_to_parse);
// void	ban(Server server, cmd_struct cmd_infos);
void	invite(Server *server, int const client_fd, cmd_struct cmd_infos);
void	join(Server *server, int const client_fd, cmd_struct cmd_infos);
// void	kick(Server server, cmd_struct cmd_infos);
void	list(Server *server, int const client_fd, cmd_struct cmd_infos);
void	nick(Server *server, int const client_fd, cmd_struct cmd_infos);
// void	oper(Server server, cmd_struct cmd_infos);
int		pass(Server *server, int const client_fd, cmd_struct cmd_infos);
int		ping(int const client_fd, cmd_struct &cmd);
// void	quit(Server server, cmd_struct cmd_infos);
void	topic(Server *server, int const client_fd, cmd_struct cmd_infos);
// void	unban(Server server, cmd_struct cmd_infos);

#endif