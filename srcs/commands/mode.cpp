#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"


// VERSION DE DIM
// std::string	getTargettedChannel(std::string targettedChannelName)
// {
// 	// for (int i = 0; targettedChannelName[i]; i++)
// 	// 	std::cout << "str[i] = " << (int)targettedChannelName[i] << std::endl;
	
// 	std::string substr_hashtag;
	
// 	targettedChannelName.erase(0, targettedChannelName.find(' ') + 1);
	
// 	// substr_hashtag = targettedChannelName.substr(0, targettedChannelName.find(" "));
		
// 	std::cout << "targettedChannelName |" << substr_hashtag << "|" << std::endl;
// 	return (substr_hashtag);
// }

std::string	getTargettedChannel(std::string targettedChannelName)
{
	std::string substr_hashtag;
	substr_hashtag.clear();

	targettedChannelName.erase(0, targettedChannelName.find(' ') + 1);

	if (targettedChannelName.find("#") != targettedChannelName.npos)
		substr_hashtag.insert(0, targettedChannelName, targettedChannelName.find("#") + 1, targettedChannelName.find(" ") - 1);
	else // exemple : si user (/MODE dyoula +i) => on veut juste l'ignorer ou on store le nom qd meme?
		substr_hashtag.insert(0, targettedChannelName, 0, targettedChannelName.find(" "));
	std::cout << "targettedChannelName |" << substr_hashtag << "|" << std::endl;
	return (substr_hashtag);
}

std::string getFlags(std::string flags)
{
	// flags.erase(0, flags.find('#'));
	// for (int i = 0; flags[i]; i++)
	// 	std::cout << "str[i] = " << (int)flags[i] << std::endl;
	if (flags.find(' ') == std::string::npos)
		return (flags);
	flags.erase(0, flags.find(' ') + 1);
	if (flags.find(' ') == std::string::npos)
		return (flags);
	flags.erase(0, flags.find(' ') + 1);
	if (flags.find(' ') == std::string::npos)
		return (flags);
	flags.erase(flags.find(' '), flags.size() - 1);
	// flags.erase(0, flags.find(' ') + 1);
	return (flags);
}

std::string	getArg(std::string arg)
{
	// for (int i = 0; arg[i]; i++)
	// 	std::cout << "arg[i] = " << (int)arg[i] << std::endl;
	if (arg.find(' ') == std::string::npos)
		return (arg);
	arg.erase(0, arg.find(' ') + 1);
	if (arg.find(' ') == std::string::npos)
		return (arg);
	arg.erase(0, arg.find(' ') + 1);
	if (arg.find(' ') == std::string::npos)
		return (arg);
	arg.erase(0, arg.find(' ') + 1);
	// std::cout << "arg = " << arg << std::endl;
	return (arg);
}

void	executeFlags(Server *server, int index, std::string datas[4], int &client_fd)
{
	// on va use channel et target
	switch (index)
	{
		case 0: ban(server, datas) ; break;
		case 1: passwordChannel(server, datas, client_fd); break;
		case 2: limit(server, datas, client_fd); break;  // limit size
		case 3: moderation(server, datas, client_fd); break; // no external message
		// case 4: extern(this, cmd_infos); break; // donne statut operateur 
		case 5: operators(server, datas, client_fd); break;
		case 6: priv(server, datas, client_fd); break;
		case 7: secret(server, datas, client_fd); break; 
		case 8: topicProtection(server, datas, client_fd); break;
		case 9: voice(server, datas, client_fd); break;
	}
}

void	iterFlags(Server *server, std::string datas[4], int &client_fd)
{
	if (datas[2].size() < 2)
		return ;
	std::string flag = "bklmnopstv";
	int index;
	for (int i = 1; datas[2][i]; i++)
	{
		// std::cout << "flags[i]" << (int)flags[i] << std::endl;
		std::cout << "flags[i]" << flag.find(datas[2][i]) << std::endl;
		index = flag.find(datas[2][i]);
		if (index == (int)std::string::npos)
			sendServerRpl(client_fd, ERR_UNKNOWNMODE(datas[0], datas[2][i]));
		else
			executeFlags(server, index, datas, client_fd);
	}
}

void	mode(Server *server, int client_fd, cmd_struct &cmd_infos)
{
	// check if client is operator.
	// if (cmd_infos.message.find('#') == 0 || cmd_infos.message.find('#') > cmd_infos.message.length())
	// {
	// 	std::cout << RED << "Mode out\n" << RESET;
	// 	return ;
	// }
	std::cout << "CMD MSG |" << cmd_infos.message << "|" << std::endl;
	std::string datas[4]; // 0 nick_operator, 1 channel, 2 Flags, 3 arg	
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
	datas[0] = it_client->second.getNickname();
	datas[1] = getTargettedChannel(cmd_infos.message); // new parsing working 
	datas[2] = getFlags(cmd_infos.message); // new parsing ok 
	datas[3] = getArg(cmd_infos.message); // new parsing ok 
	// std::cout << "flags = " << datas[2] << "|" << std::endl;
	if (server->is_operator(datas[1], datas[0]) == false)
	{
		std::cout << datas[0] << " is not an Operator sorray on " << datas[1] << std::endl;
		return ;
	}
	iterFlags(server, datas, client_fd);
}