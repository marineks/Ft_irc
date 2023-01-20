#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	ban(Server *server, std::string datas[4])
{
	server->printChannels();
	server->banClientFromChannel(datas[1], datas[3], datas[0]);
	server->printClientList();
}

void	passwordChannel(Server *server, std::string datas[4])
{
	server->managePassword(datas, datas[2][0]);
}

void	limit(Server *server, std::string datas[4])
{
	std::map<std::string, Channel>::iterator it;
	it = server->getChannels().find(datas[1]);
	if (it == server->getChannels().end())
	{
		std::cout << RED << "This channel does not exists\n" << RESET;
		return ;
	}
	int limit = atoi(datas[3].c_str());
	if (datas[1][0] == '+')
	{
		it->second.setLimit(limit);
	}
	else if (datas[1][0] == '-')
	{
		limit = -1;
		it->second.setLimit(limit);
	}
}

void	moderation(Server *server, std::string datas[4])
{
	server->manageModeration(datas);
}

void	operators(Server *server, std::string datas[4])
{
	server->manageOperator(datas);
}

void	priv(Server *server, std::string datas[4])
{
	server->managePrivate(datas);
}

void	secret(Server *server, std::string datas[4])
{
	server->manageSecret(datas);
}

void	topicProtection(Server *server, std::string datas[4])
{
	server->manageTopicProtection(datas);
}

void	voice(Server *server, std::string datas[4])
{
	server->manageVoice(datas);
}