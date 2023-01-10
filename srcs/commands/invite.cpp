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
	Client		client			= retrieveClient(server, client_fd);
	std::string	client_nickname	= client.getNickname();
	std::string channel_name	= findChannel(cmd_infos.message);
	std::string invited_client	= findNickname(cmd_infos.message);
	std::string	client_reply;
	
	if (client_nickname.empty() || channel_name.empty())
	{
		std::string ERR_NEEDMOREPARAMS = "461 " + client_nickname + " " + cmd_infos.name + " :Not enough parameters\r\n";
		client_reply = ERR_NEEDMOREPARAMS;
		send(client_fd, client_reply.c_str(), client_reply.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << client_reply << RESET << std::endl;
		return ;
	}

	// Check if the channel exists
	std::map<std::string, Channel>			 channels = server->getChannels();
	std::map<std::string, Channel>::iterator channel = channels.find(channel_name);
	if (channel == channels.end())
	{
		std::string ERR_NOSUCHCHANNEL = "403 " + client_nickname + " " + channel_name + " :No such channel\r\n";
		client_reply = ERR_NOSUCHCHANNEL;
		send(client_fd, client_reply.c_str(), client_reply.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << client_reply << RESET << std::endl;
		return ;
	}
	
	// Check that the person inviting is a member of said channel
	if (channel->second.doesClientExist(client_nickname) == false)
	{
		std::string ERR_NOTONCHANNEL = "442 " + client_nickname + " " + channel_name + " :You're not on that channel\r\n";
		client_reply = ERR_NOTONCHANNEL;
		send(client_fd, client_reply.c_str(), client_reply.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << client_reply << RESET << std::endl;
		return ;
	}
	
	// Check that, if the channel is on invite-only mode, the user inviting is an operator
	// TODO: attendre les modes de Dim pour cette condition

	// Check that the invited user is not already on the channel
	if (channel->second.doesClientExist(invited_client) == true)
	{
		std::string ERR_USERONCHANNEL = "442 " + client_nickname + " " + invited_client + " " + channel_name + " :Is already on channel\r\n";
		client_reply = ERR_USERONCHANNEL;
		send(client_fd, client_reply.c_str(), client_reply.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << client_reply << RESET << std::endl;
		return ;
	}
	
	// If all checks are successful => send a RPL_INVITING + invite to the inviting user 
	std::string RPL_INVITING = "442 " + client_nickname + " " + invited_client + " " + channel_name + " :Is invited to a channel!\r\n";
	client_reply = RPL_INVITING;

	send(client_fd, client_reply.c_str(), client_reply.size(), 0);
	std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << client_reply << RESET << std::endl;

	std::map<std::string, Client> clients = channel->second.getClientList();
	std::map<std::string, Client>::iterator invited = clients.find(invited_client);
	std::string user_id = ":" +	invited->second.getNickname() + "!" + invited->second.getUsername() + "@localhost";
	std::string invite = user_id + ":Knock knock! You are invited to join the channel #" + channel_name + " by " + client_nickname + " .\r\n";
	send(invited->second.getClientFd(), invite.c_str(), invite.size(), 0);
}

// Exemple of user input : "INVITE Wiz #foo_bar"
// 							=> msg_to_parse is " Wiz #foo_bar"
std::string	findNickname(std::string msg_to_parse)
{
	std::string nickname;
	
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