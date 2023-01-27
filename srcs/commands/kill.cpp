#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	retrieveNickname(std::string msg);
static std::string	retrieveComment(std::string msg);
static void			broadcastQuit(Server *server, std::string reply);
static Client*		clientExists(Server *server, std::string nickname);
static bool			isIrcOperator(Server *server, std::string nickname);
static void			removeFromServer(Server *server, std::string killed_user);
/**
 * @brief The KILL command is used to close the connection between a given client
 * 		and the server they are connected to. KILL is a privileged command and 
 * 		is available only to IRC Operators. Clients can rejoin instantly after 
 * 		this command is performed on them.
 * 	
 * 	Syntax :
 * 	KILL <nickname> <comment>
 * 
 * 		<nickname> represents the user to be ‘killed’, and 
 * 		<comment> is shown to all users and to the user themselves upon being killed.
 * 
 * 		The <source> of the message should be the operator who performed the command.
 */
void		kill(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client &client = retrieveClient(server, client_fd);
	std::string killer = client.getNickname();
	std::string killed = retrieveNickname(cmd_infos.message);
	std::string	comment = retrieveComment(cmd_infos.message);
	Client *killed_user = clientExists(server, killed);
	
	if (killed.empty())
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(killer, cmd_infos.name));
	else if (killed_user == NULL)
		addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(killer, killed));
	else if (comment.empty())
		comment = "default";
	else if (client.getMode().find('o') == std::string::npos && isIrcOperator(server, killer) == false)
		addToClientBuffer(server, client_fd, ERR_NOPRIVILEGES(killer));
	else
	{
		/* the client being killed receives the KILL message, 
		   and the <source> of the message SHOULD be the operator who performed the command. */
		addToClientBuffer(server, killed_user->getClientFd(), RPL_KILL(user_id(killer, client.getUsername()), killed, comment));

		/* The user being killed and every user sharing a channel with them receives 
			a QUIT message representing that they are leaving the network. */
		std::string quit_reason = ":Killed (" + killer + " (" + comment + "))";
		broadcastQuit(server, RPL_QUIT(user_id(killed, killed_user->getUsername()), quit_reason));
		
		/* The user being killed then receives the ERROR message */
		std::string error_reason = ":Closing Link: localhost. Killed (" + killer + " (" + comment + "))";
		addToClientBuffer(server, killed_user->getClientFd(), RPL_ERROR(user_id(killed, killed_user->getUsername()), error_reason));

		/* After this, their connection is closed. */ 
		killed_user->setDeconnexionStatus(true);
		removeFromServer(server, killed);
		
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

static void	broadcastQuit(Server *server, std::string reply)
{
    std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client;
	
	for (it_client = client_list.begin(); it_client != client_list.end(); it_client++)
	{
		addToClientBuffer(server, it_client->second.getClientFd(), reply);
	}
}

static bool	isIrcOperator(Server *server, std::string nickname)
{
	std::vector<server_op> irc_ops = server->getIrcOperators();
	std::vector<server_op>::iterator it_op;
	
	for (it_op = irc_ops.begin(); it_op != irc_ops.end(); it_op++)
	{
		if (it_op->name == nickname)
			return (true);
	}
	return (false);
}

static Client*	clientExists(Server *server, std::string nickname)
{
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client;
	
	for (it_client = client_list.begin(); it_client != client_list.end(); it_client++)
	{
		if (it_client->second.getNickname() == nickname)
			break;
	}
	if (it_client == client_list.end())
		return (NULL);
	return (&it_client->second);
}

static std::string	retrieveNickname(std::string msg)
{
	std::string nickname;

	if (msg[0] == ' ')
		msg.erase(0,1);
	if (msg.empty())
	{
		nickname.clear();
		return (nickname);
	}
	else
	{
		if (msg.find(' ') != std::string::npos)
			nickname.insert(0, msg, 0, msg.find_first_of(' '));
		else
			nickname.insert(0, msg, 0, msg.npos);
	}
	return (nickname);
}

static std::string retrieveComment(std::string msg)
{
	std::string comment;

	if (msg[0] == ' ')
		msg.erase(0,1);
	if (msg.empty() || msg.find(' ') == std::string::npos)
	{
		comment.clear();
		return (comment);
	}
	else
	{
		if (msg.find(' ') != std::string::npos)
			comment.insert(0, msg,  msg.find_first_of(' ') + 1, msg.npos);
	}
	return (comment);
}