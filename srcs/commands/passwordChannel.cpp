#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	passwordChannel(Server *server, std::string datas[4])
{
	std::map<std::string, Channel>::iterator it;
	it = server->getChannels().find(datas[1]);
	if (it == server->getChannels().end())
	{
		std::cout << RED << "This channel does not exists\n" << RESET;
		return ;
	}	
	if (datas[1][0] == '+')
	{
		it->second.setPassword(datas[3]);
		std::cout << "pswd " << it->second.getPassword() << std::endl;
	}
	else if (datas[1][0] == '-')
	{
		it->second.getPassword().clear();
	}
}