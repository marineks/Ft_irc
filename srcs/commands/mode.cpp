#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::string	getTarget(std::string line)
{
	line.erase(0, line.find('#'));
	std::string substr_hashtag;
	substr_hashtag = line.substr(line.find('#'), line.find(" ") + 1);
	return (substr_hashtag);
}

std::string getFlags(std::string line)
{
	line.erase(0, line.find('#'));
	line.erase(0, line.find(' ') + 1);
	line.erase(0, line.find('-'));
	line.erase(line.find(' '), line.size() - 1);
	return (line);
}

std::string	getArg(std::string line)
{
	line.erase(0, line.find('#'));
	line.erase(0, line.find(' ') + 1);
	line.erase(0, line.find(' ') + 1);
	return (line);
}

// void	executeFlags(Server *server, int index, std::string datas[4])
// {
	// on va use channel et target
// 	switch (index)
// 	{
// 		case 0: ban() ; break; // ban
// 		// case 1: invite(this, client_fd, cmd_infos); break; // invite
// 		// case 2: join(this, client_fd, cmd_infos); break; // key protect 
// 		// case 3: kick(this, cmd_infos); break; // limit_size channel
// 		// case 4: kill(cmd_infos); break; //modéré
// 		// case 5: list(this, client_fd, cmd_infos); break; // no external message
// 		// case 6: mode(this, cmd_infos); break; // donne statut operateur 
// 		// case 7: nick(this, client_fd, cmd_infos); break; // private
// 		// case 8: nick(this, client_fd, cmd_infos); break; // secret
// 		// case 9: nick(this, client_fd, cmd_infos); break; // private
// 		// case 10: nick(this, client_fd, cmd_infos); break; // topic protection 
// 		// case 11: nick(this, client_fd, cmd_infos); break; // voiced 
// 		// case 12: nick(this, client_fd, cmd_infos); break; // private
// 	}
// }
void	iterFlags(Server *server, std::string datas[4])
{
	if (datas[2].size() < 2)
		return ;
	std::string flag = "biklmnopstv";
	for (int i = 1; datas[2][i]; i++)
	{
		// std::cout << "flags[i]" << (int)flags[i] << std::endl;
		std::cout << "flags[i]" << flag.find(datas[2][i]) << std::endl;
		// flag.find(flags[i]);
		// executeFlags(server, flag.find(datas[2][i]), datas);
	}
}

void	mode(Server *server, int client_fd, cmd_struct cmd_infos)
{
	// std::string	prefix;
	// std::string	name;
	// std::string	message;
	(void)server;
	if (cmd_infos.message.find('#') == 0 || cmd_infos.message.find('#') > cmd_infos.message.length())
		return ;
	std::string datas[4]; // 0 nick_operator, 1 channel, 2 Flags, 3 arg
	// std::string channel;
	// std::string flags;
	// std::string arg;
	
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
	datas[0] = it_client->second.getNickname();
	datas[1] = getTarget(cmd_infos.message);
	datas[2] = getFlags(cmd_infos.message);
	datas[3] = getArg(cmd_infos.message);
	iterFlags(server, datas);
	// std::cout << cmd_infos.prefix << std::endl;
	// std::cout << cmd_infos.name << std::endl;
	// std::cout << cmd_infos.message << std::endl;
}