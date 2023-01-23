#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::string	getTargettedChannel(std::string targettedChannelName)
{
	// for (int i = 0; targettedChannelName[i]; i++)
	// 	std::cout << "str[i] = " << (int)targettedChannelName[i] << std::endl;
	targettedChannelName.erase(0, targettedChannelName.find(' ') + 1);
	std::string substr_hashtag;
	substr_hashtag = targettedChannelName.substr(0, targettedChannelName.find(" "));
	// std::cout << "targettedChannelName " << substr_hashtag << "|" << std::endl;
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

void	executeFlags(Server *server, int index, std::string datas[4])
{
	// on va use channel et target
	switch (index)
	{
		case 0: ban(server, datas) ; break; // ban
		// case 1: invite(this, client_fd, cmd_infos); break; // invite
		case 2: passwordChannel(server, datas); break; // limit_size channel
		case 3: limit(server, datas); break;  // limit size
		case 4: moderation(server, datas); break; // no external message
		// case 6: extern(this, cmd_infos); break; // donne statut operateur 
		case 6: operators(server, datas); break;
		case 7: priv(server, datas); break; // secret
		case 8: secret(server, datas); break; // topic protection 
		case 9: topicProtection(server, datas); break; // voiced 
		case 10: voice(server, datas); break; // private
	}
}
void	iterFlags(Server *server, std::string datas[4])
{
	if (datas[2].size() < 2)
		return ;
	std::string flag = "biklmnopstv";
	int index;
	for (int i = 1; datas[2][i]; i++)
	{
		// std::cout << "flags[i]" << (int)flags[i] << std::endl;
		// std::cout << "flags[i]" << flag.find(datas[2][i]) << std::endl;
		index = flag.find(datas[2][i]);
		executeFlags(server, index, datas);
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
	iterFlags(server, datas);
}