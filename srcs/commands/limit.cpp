#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

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
