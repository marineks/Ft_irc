#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void	ban(Server server, cmd_struct cmd_infos)
{
	// TODO: coder le parsing du command.message pour arriver à operator name, channelName et clientName
	std::string operatorName;
	std::string channelName;
	std::string clientName;
	
	std::map<std::string, Channel>::iterator it;
	it = server.getChannels().find(channelName); // TODO: prévoir le cas où le Chan n´existe pas
	if (it->second.doesClientExist(clientName) == true)
	{
		if (it->second.isOperator(operatorName) == false)
		{
			std::cout << operatorName << " is not admin on " << channelName << std::endl;
			return ;
		}
		it->second.removeClientFromChannel(clientName);
		it->second.addToBanned(clientName);
		std::cout << clientName << " has been banned from " << channelName << " by " << operatorName <<std::endl; 
	}	
}