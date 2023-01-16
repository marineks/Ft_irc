#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief The NAMES command is used to view the nicknames joined to a channel.
 *  If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES 
 * 	numeric containing the given channel name should be returned.
 * 
 * 	Syntax: NAMES <channel>{,<channel>}
 * 	
 * 	Numeric Replies:
 * 	
 * 	RPL_NAMREPLY (353)
 * 	RPL_ENDOFNAMES (366)
 * 
 * 	Examples:
 * 	[CLIENT] /NAMES #test,#42
 * 	[SERVER] <client> <symbol> #test :<nick1> <nick2>
 * 	
 */
void	names(Server *server, int const client_fd, cmd_struct cmd_infos)
{

}