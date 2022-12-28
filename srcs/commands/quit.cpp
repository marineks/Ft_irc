#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

/**
 * @brief The QUIT command is used to terminate a client’s connection to the server. 
 *  The server acknowledges this by replying with an ERROR message and closing 
 *  the connection to the client.
 * 	
 *  Parameters: [<reason>]
 * 
 *  Example : [CLIENT] QUIT :Gone to have lunch   
 * 			  [SERV] ;Client exiting from the network
 * 
 * 	Source: https://modern.ircdocs.horse/#quit-message
 */
void	quit(Server server, cmd_struct cmd_infos)
{
	// TODO: coder le parsing du cmd.message pour arriver à channelName et clientName
	std::string channelName;
	std::string clientName;

	// FIXME : je crois que QUIT n'a pas vraiment/que cet effet
	// du coté du client ca quitte IRSSI, et du coté server, ca del le Client non?
	std::map<std::string, Channel>			 channels = server.getChannels();
	std::map<std::string, Channel>::iterator it;
	it = channels.find(channelName);
	if (it == channels.end())
	{
		std::cout << "Channel not found\n";
		return ;
	}
	if (it->second.doesClientExist(clientName) == true)
	{
		it->second.removeClientFromChannel(clientName);
		std::cout << clientName << " has now left " << channelName << std::endl; 
	}
}