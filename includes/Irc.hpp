#ifndef IRC_HPP
#define IRC_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <iostream>
# include <vector>
# include <map>
# include <poll.h>
# include <unistd.h>

# include "Macro.hpp"
# include "Colors.hpp"
// #include "Server.hpp" // atttention toi on te surveille

struct cmd_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

#endif