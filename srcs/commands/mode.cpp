#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief The MODE command is used to set or remove options (or modes) from a given target.
 * 
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 * 
 * <modestring> starts with a plus ('+', 0x2B) or minus ('-', 0x2D) character, and is made up of the following characters:
 * '+': Adds the following mode(s).
 * '-': Removes the following mode(s).
 * 'a-zA-Z': Mode letters, indicating which modes are to be added/removed.
 * 
 * User case :
 * If <modestring> is given, the supplied modes will be applied : a MODE message will be sent to the user containing the changed modes. 
 * If one or more modes sent are not implemented on the server, 
 * the server MUST apply the modes that are implemented, and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.
 * 
 * Channel case :
 * syntax command : /mode <channel> <+ ou -> <mode> [parametres]
 * If the user has permission to change modes on the target, the supplied modes will be applied based on the type of the mode (see below). 
 * For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. 
 * - If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode. 
 * - If a type A mode has been sent without an argument : 
 * the contents of the list MUST be sent to the user, 
 * unless it contains sensitive information the user is not allowed to access. 
 * When the server is done processing the modes, a MODE command is sent to all members of the channel containing the mode changes. 
 * Servers MAY choose to hide sensitive information when sending the mode changes.
 * 
 * Exemples :
 * - +o : /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
 * Message à send aux users du channel : ':irc.example.com MODE #cool +o MEAT'; 
 * The irc.example.com server gave channel operator privileges to MEAT on #cool.
 * - +k : /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
 * - +s : /mode #cool +s (le channel devient secret)
 * - plusieur modes : /mode #cool +ts (le channel est en mode +t +s)
 * 
 * @param server
 * @param client_fd User sending a msg
 * @param cmd_infos Structure w/ prefix, command name and message
 * 
 * Useful link : https://modern.ircdocs.horse/#mode-message
 * https://www.techbull.com/techbull/guide/internet/irccommande.html
 * 
 */

void	broadcastToAllChannelMembers(Server *server, Channel &channel, std::string reply)
{
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	
	while (member != channel.getClientList().end())
	{
		addToClientBuffer(server, member->second.getClientFd(), reply);
		member++;
	}
}

static void	fillModeInfos(mode_struct &mode_infos, std::string command)
{	
	size_t						pos;

	// TARGET
	pos = command.find(" ");
	if (pos == std::string::npos) // n'a pas trouvé d'espace et donc un seul argument
	{
		mode_infos.target = command.substr(0);
		return ;
	}
	else
	{
		mode_infos.target = command.substr(0, pos);
		command.erase(0, pos + 1); // supprime la target + l'espace
	}

	// MODE
	pos = command.find(" ");
	if (pos == std::string::npos)
	{
		mode_infos.mode = command.substr(0);
		return ;
	}
	else
	{
		mode_infos.mode = command.substr(0, pos);
		command.erase(0, pos + 1); // supprime la target + l'espace
	}

	// PARAM
	mode_infos.params = command.substr(0);
}

static void	changeChannelMode(Server *server, mode_struct mode_infos, int const client_fd)
{
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
	std::map<std::string, Channel>::iterator it_channel_target = server->getChannels().find(mode_infos.target);

	(void)it_client;
	(void)it_channel_target;

	// parse les modes
	std::vector<std::string> vector_modes;
	for (int i = 1; mode_infos.mode[i] != '\0'; i++)
	{
		if (mode_infos.mode[i] == '+' || mode_infos.mode[i] == '-')
		{
			vector_modes.push_back(mode_infos.mode.substr(0, i));
			mode_infos.mode.erase(0, i);
			i = 0;
		}
	}
	vector_modes.push_back(mode_infos.mode.substr(0));
	
	// apply modes
	for (std::vector<std::string>::iterator it = vector_modes.begin(); it != vector_modes.end(); it++)
	{
		std::string str = *it;
		std::string datas[4];
		datas[0] = it_client->second.getNickname();
		datas[1] = mode_infos.target;
		datas[2] = str;
		datas[3] = mode_infos.params;
		if (str.find("b") != std::string::npos)
			banChannelMode(server, datas, client_fd);
		if (str.find("k") != std::string::npos)
			keyChannelMode(server, mode_infos, client_fd, str);
		if (str.find("l") != std::string::npos)
			limitChannelMode(server, datas, client_fd);
		if (str.find("m") != std::string::npos)
			moderateChannelMode(server, mode_infos, client_fd, str);
		if (str.find("o") != std::string::npos)
			operatorChannelMode(server, mode_infos, client_fd, str);
		if (str.find("p") != std::string::npos)
			privateChannelMode(server, mode_infos, client_fd, str);
		if (str.find("s") != std::string::npos)
			secretChannelMode(server, mode_infos, client_fd, str);
		if (str.find("t") != std::string::npos)
			topicChannelMode(server, mode_infos, client_fd, str);
		if (str.find("v") != std::string::npos)
			voiceChannelMode(server, mode_infos, client_fd, str);
	}
}

static void	modeForChannel(Server *server, mode_struct mode_infos, int const client_fd)
{
	// syntax command : /mode <channel> <+ ou -> <mode> [parametres]
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);

	mode_infos.target.erase(0,1); // erase '#'

	// Check si le chan existe
	std::map<std::string, Channel>::iterator it_channel_target = server->getChannels().find(mode_infos.target);
	if (it_channel_target == server->getChannels().end())
	{
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(it_client->second.getNickname(), mode_infos.target));
		return ;
	}

	// If <mode> is not given
	if (mode_infos.mode.empty() == true)
	{
		// Check si le client est membre du channel et si le channel a un mdp
		if (it_channel_target->second.getClientList().find(it_client->second.getNickname()) != it_channel_target->second.getClientList().end() \
		&& it_channel_target->second.getChannelPassword().empty() == false)
			addToClientBuffer(server, client_fd, RPL_CHANNELMODEISWITHKEY(it_client->second.getNickname(), mode_infos.target, it_channel_target->second.getMode(), it_channel_target->second.getChannelPassword()));
		else
			addToClientBuffer(server, client_fd, RPL_CHANNELMODEIS(it_client->second.getNickname(), mode_infos.target, it_channel_target->second.getMode()));
		return ;
	}

	// Check si le client est operator du channel
	std::vector<std::string>::iterator it;
	for (it = it_channel_target->second.getOperators().begin(); it != it_channel_target->second.getOperators().end(); it++)
	{
		if (*it == it_client->second.getNickname())
			break;
	}
	if (it == it_channel_target->second.getOperators().end())
	{
		addToClientBuffer(server, client_fd, ERR_CHANOPRIVSNEEDED(it_client->second.getNickname(), mode_infos.target));
		return ;
	}
	if (mode_infos.mode[0] == '+' || mode_infos.mode[0] == '-')
		changeChannelMode(server, mode_infos, client_fd);
}

static void	modeForUser(Server *server, mode_struct mode_infos, int const client_fd)
{
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);

	// Check si la target existe parmis les clients
	std::map<const int, Client>::iterator it_user_target = server->getClients().begin();
    while (it_user_target != server->getClients().end())
    {
    	if (it_user_target->second.getNickname() == mode_infos.target)
        	break;
    	it_user_target++;
    }
	if (it_user_target == server->getClients().end())
	{
		addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), mode_infos.target));
		return ;
	}
	//  If <target> is a different nick than the user who sent the command
	if (it_user_target->second.getNickname() != it_client->second.getNickname())
	{
		addToClientBuffer(server, client_fd, ERR_USERSDONTMATCH(it_client->second.getNickname()));
		return ;
	}

	// If <mode> is not given
	if (mode_infos.mode.empty() == true)
		addToClientBuffer(server, client_fd, RPL_UMODEIS(it_client->second.getNickname(), it_client->second.getMode()));
	
	// Attribue un mode à l'user (modes autorisés 'i' & 'o' ?)
	if (mode_infos.mode[0] == '+' || mode_infos.mode[0] == '-')
	{
		std::string::iterator pos = mode_infos.mode.begin();
		while (pos != mode_infos.mode.end())
		{
			if (*pos == '+')
			{
				pos++;
				while (*pos != '+' && *pos != '-' && pos != mode_infos.mode.end())
				{
					if (*pos == 'i')
					{
						if (it_user_target->second.getMode().find("i") == std::string::npos)
						{
							it_user_target->second.addMode("i");
							addToClientBuffer(server, client_fd, MODE_USERMSG(it_client->second.getNickname(), "+i"));
						}
					}
					pos++;
				}
			}
			if (*pos == '-')
			{
				pos++;
				while (*pos != '+' && *pos != '-' && pos != mode_infos.mode.end())
				{
					if (*pos == 'i')
					{
						if (it_user_target->second.getMode().find("i") != std::string::npos)
						{
							it_user_target->second.removeMode("i");
							addToClientBuffer(server, client_fd, MODE_USERMSG(it_client->second.getNickname(), "-i"));
						}
					}
					if (*pos == 'o')
					{
						if (it_user_target->second.getMode().find("o") != std::string::npos)
						{
							it_user_target->second.removeMode("o");
							addToClientBuffer(server, client_fd, MODE_USERMSG(it_client->second.getNickname(), "-o"));
						}
					}
					pos++;
				}
			}
		}
		if (mode_infos.mode.find("O") != std::string::npos || mode_infos.mode.find("r") != std::string::npos || mode_infos.mode.find("w") != std::string::npos)
			addToClientBuffer(server, client_fd, ERR_UMODEUNKNOWNFLAG(it_client->second.getNickname()));
	}
}

void	modeFunction(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	mode_struct	mode_infos;
	
	cmd_infos.message.erase(0,1);
	// parse la commande
	fillModeInfos(mode_infos, cmd_infos.message);

	if (mode_infos.target[0] == '#') // channel case
		modeForChannel(server, mode_infos, client_fd);
	else // user case
		modeForUser(server, mode_infos, client_fd);
}