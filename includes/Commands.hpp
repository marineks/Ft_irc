#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "Irc.hpp"
# include "Server.hpp"
# define VALID_LEN 15

class Server;

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

int			parseCommand(std::string cmd_line, cmd_struct &cmd_infos);
Client&		retrieveClient(Server *server, int const client_fd);
std::string	getListOfMembers(Channel &channel);
std::string	getChannelName(std::string msg_to_parse);
std::string	findNickname(std::string msg_to_parse);
void		ban(Server *server, std::string datas[4]);
bool		isAlreadyUsed(Server *server, int client_fd, std::string new_nickname);
std::string	getReason(std::string msg_to_parse);

// #######################
// ###### COMMANDS #######
// #######################

void	invite(Server *server, int const client_fd, cmd_struct cmd_infos);
void	join(Server *server, int const client_fd, cmd_struct cmd_infos);
void	kick(Server *server, int const client_fd, cmd_struct cmd_infos);
void	list(Server *server, int const client_fd, cmd_struct cmd_infos);
void	passwordChannel(Server *server, std::string datas[4], int &client_fd);
void	limit(Server *server, std::string datas[4], int &client_fd);
void 	mode(Server *server, int client_fd, cmd_struct &cmd_infos);
void	moderation(Server *server, std::string datas[4], int &client_fd);
void	names(Server *server, int const client_fd, cmd_struct cmd_infos);
void	nick(Server *server, int const client_fd, cmd_struct cmd_infos);
// void	oper(Server server, cmd_struct cmd_infos);
void	operators(Server *server, std::string datas[4], int &client_fd);
int		pass(Server *server, int const client_fd, cmd_struct cmd_infos);
void	priv(Server *server, std::string datas[4], int &client_fd);
void	secret(Server *server, std::string datas[4], int &client_fd);
void	topic(Server *server, int const client_fd, cmd_struct cmd_infos);
void	topicProtection(Server *server, std::string datas[4], int &client_fd);
void	voice(Server *server, std::string datas[4], int &client_fd);
// void	unban(Server server, cmd_struct cmd_infos);
void	part(Server *server, int const client_fd, cmd_struct cmd_infos);
int		ping(Server *server, int const client_fd, cmd_struct &cmd);
void	privmsg(Server *server, int const client_fd, cmd_struct cmd_infos);
void	quit(Server *server, int const client_fd, cmd_struct cmd_infos);

#endif