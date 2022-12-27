#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# define VALID_LEN 17

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

void	ban(Server server, cmd_struct cmd_infos);
int		ping(int const client_fd, cmd_struct &cmd);

#endif