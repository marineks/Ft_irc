#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static void	broadcastToChan(Server *server, Channel &channel, int const client_fd, std::string nick, std::string user, std::string reason);
static void	removeFromServer(Server *server, std::string killed_user);
/**
 * @brief The QUIT command is used to terminate a client’s connection to the server. 
 *  The server acknowledges this by replying with an ERROR message and closing 
 *  the connection to the client.
 * 
 * 	When connections are terminated by a client-sent QUIT command, 
 *  servers SHOULD prepend <reason> with the ASCII string "Quit: " when sending 
 *  QUIT messages to other clients, to represent that this user terminated the 
 *  connection themselves. This applies even if <reason> is empty, in which case 
 *  the reason sent to other clients SHOULD be just this "Quit: " string. 
 * 	
 *  Parameters: [<reason>]
 * 
 *  Example : [CLIENT] QUIT :Gone to have lunch   
 * 			  [SERV] ;Client exiting from the network
 * 
 * 	Source: https://modern.ircdocs.horse/#quit-message
 */
void		quit(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client& 								  client   = retrieveClient(server, client_fd);
	std::string								  reason   = getReason(cmd_infos.message);
	std::map<std::string, Channel>&			  channels = server->getChannels();
	std::map<std::string, Channel>::iterator  chan	   = channels.begin();

	// inform the concerned user
	addToClientBuffer(server, client_fd, RPL_QUIT(user_id(client.getNickname(), client.getUsername()), reason));
	// inform all the users that share a channel w/ the user quitting
	for (; chan != channels.end(); chan++) // check all channels
	{
		std::map<std::string, Client>& 			chan_members = chan->second.getClientList();
		std::map<std::string, Client>::iterator	member		 = chan_members.begin();
		for (; member != chan_members.end(); member++) // check all chan_members
		{
			if (member->second.getClientFd() == client_fd) // erase user from the chan + inform the others 
			{
				chan_members.erase(client.getNickname());
				broadcastToChan(server, chan->second, client_fd, client.getNickname(), client.getUsername(), reason);
				break ;
			}
		}
	}
	// close the connection (no need for irssi, but nc needs it)
	client.setDeconnexionStatus(true);
	removeFromServer(server, client.getNickname());
}

static void	broadcastToChan(Server *server, Channel &channel, int const client_fd, std::string nick, std::string user, std::string reason)
{
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	
	(void) client_fd;
	while (member != channel.getClientList().end())
	{
		if (member->second.getClientFd() != client_fd)
		{
			addToClientBuffer(server, member->second.getClientFd(), \
				RPL_QUIT(user_id(nick, user), reason));
		}
			
		member++;
	}
}

static void	removeFromServer(Server *server, std::string killed_user)
{
	std::map<std::string, Channel>&				channels = server->getChannels();
	std::map<std::string, Channel>::iterator	chan;
	for (chan = channels.begin(); chan != channels.end(); chan++)
	{
		if (chan->second.doesClientExist(killed_user))
		{
			chan->second.getClientList().erase(killed_user);
		}
	}
}