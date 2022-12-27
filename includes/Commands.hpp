#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# define VALID_LEN 17

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

int	ping(int const client_fd, cmd_struct &cmd);

#endif