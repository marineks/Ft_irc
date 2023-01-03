#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# define VALID_LEN 18

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

void	ban(Server server, cmd_struct cmd_infos);
void	join(Server server, int const client_fd, cmd_struct cmd_infos);
void	kick(Server server, cmd_struct cmd_infos);
void	oper(Server server, cmd_struct cmd_infos);
int		ping(int const client_fd, cmd_struct &cmd);
void	quit(Server server, cmd_struct cmd_infos);
void	unban(Server server, cmd_struct cmd_infos);

#endif