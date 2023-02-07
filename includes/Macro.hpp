#ifndef	MACRO_HPP
#define MACRO_HPP

# include "Irc.hpp"

# define SUCCESS 0
# define FAILURE -1
# define BREAK 2
# define CONTINUE 3

# define BACKLOG 10          // Nb max of clients queuing
# define MAX_CLIENT_NB 4     // Nb max of clients overall
# define BUF_SIZE_MSG 4096

/*		MESSAGE		*/
# define ERR_FULL_SERV "[Server] You cannot join, the server is already full"

#endif