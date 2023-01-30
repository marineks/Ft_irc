#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	findChannel(std::string msg_to_parse);
/**
 * @brief The INVITE command is used to invite a user to a channel. 
 * 	The parameter <nickname> is the nickname of the person to be invited to the 
 * 	target channel <channel>.
 * 
 * 	Syntax : INVITE <nickname> <channel>
 * 
 */
void	invite(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client&		client			= retrieveClient(server, client_fd);
	std::string	client_nickname	= client.getNickname();
	std::string channel_name	= findChannel(cmd_infos.message);
	std::string invited_client	= findNickname(cmd_infos.message);
	
	if (client_nickname.empty() || channel_name.empty())
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_nickname, cmd_infos.name));
		return ;
	}

	// Check if the channel exists
	std::map<std::string, Channel>&			 channels = server->getChannels();
	std::map<std::string, Channel>::iterator channel = channels.find(channel_name);
	if (channel == channels.end())
	{
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(client_nickname, channel_name));
		return ;
	}
	
	// Check that the person inviting is a member of said channel
	if (channel->second.doesClientExist(client_nickname) == false)
	{
		addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client_nickname, channel_name));
		return ;
	}
	
	// Check that the invited user is not already on the channel
	if (channel->second.doesClientExist(invited_client) == true)
	{
		addToClientBuffer(server, client_fd, ERR_USERONCHANNEL(client_nickname, invited_client, channel_name));
		return ;
	}
	
	// If all checks are successful => send a RPL_INVITING + invite to the inviting user 
	addToClientBuffer(server, client_fd, RPL_INVITING(user_id(client_nickname, client.getUsername()), client_nickname, invited_client, channel_name));
	std::map<const int, Client>&			clients 	= server->getClients();

	std::map<const int, Client>::iterator	it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.getNickname() == invited_client)
		{
			addToClientBuffer(server, it->second.getClientFd(),\
				RPL_INVITE(user_id(client_nickname, client.getUsername()), it->second.getNickname(), channel_name));
			break;
		}
	}
}

// Exemple of user input : "INVITE Wiz #foo_bar"
// 							=> msg_to_parse is " Wiz #foo_bar"
std::string	findNickname(std::string msg_to_parse)
{
	std::string nickname;
	nickname.clear();
	
	if (msg_to_parse.empty() == true)
		return (nickname);
	char *str = const_cast<char *>(msg_to_parse.data());
	nickname = strtok(str, " ");
	
	if (nickname.empty() 							// Si pas de token après INVITE
		|| nickname.find("#") != nickname.npos)		// Si le seul token est le channel
		nickname.clear();
	return (nickname);
}

std::string	findChannel(std::string msg_to_parse)
{
	std::string channel;

	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos)
	{
		channel.clear();
	}
	else
	{
		channel.append(msg_to_parse, msg_to_parse.find("#") + 1, std::string::npos);
	}
	return (channel);
}