#include "Irc.hpp"
#include "Server.hpp"
#include "Channel.hpp"

// NOTE: dapres la doc irc, ce nest pas une commande mais un mode (-b)
void	unban(Server server, cmd_struct cmd_infos)
{
	// TODO: coder le parsing du command.message pour arriver à operator name, channelName et clientName
	std::string operator_name;
	std::string channel_name;
	std::string name_to_unban;

	std::map<std::string, Channel>::iterator it;
	it = server.getChannels().find(channel_name); // TODO: prévoir cas où Channel n'existe pas
	if (it->second.doesClientExist(name_to_unban) == true)
	{
		if (it->second.isOperator(operator_name) == false)
		{
			std::cout << operator_name << " is not admin on " << channel_name << std::endl;
			return ;
		}
		// it->second.addClientToChannel(name_to_unban); // BUG: L'erreur de type est aussi sur la version de Dim (27/12)
		it->second.removeFromBanned(name_to_unban);
		std::cout << name_to_unban << " has been unbanned from " << channel_name << " by " << operator_name <<std::endl; 
	}	
}