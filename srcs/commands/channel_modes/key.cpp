#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static bool isAlpha(std::string str) 
{
    for (size_t i = 0; i < str.length(); i++) 
	{
        if (!isalpha(str[i])) 
            return (false);
    }
    return (true);
}

void	keyChannelMode(Server *server, mode_struct mode_infos, int const client_fd, std::string mode_str)
{
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
	std::map<std::string, Channel>::iterator it_channel_target = server->getChannels().find(mode_infos.target);


	size_t pos = it_channel_target->second.getMode().find("k");
	if (mode_infos.params.empty() == true)
		return ;
	
	if (mode_str[0] == '+')
	{
		if (pos != std::string::npos) // le mode est deja présent
			return;
		if (isAlpha(mode_infos.params) == false) // check si le mdp est conforme
		{
			addToClientBuffer(server, client_fd, ERR_INVALIDMODEPARAM(it_client->second.getNickname(), mode_infos.target, "+k", mode_infos.params));
			return ;
		}
		it_channel_target->second.addMode("k");
		it_channel_target->second.setChannelPassword(mode_infos.params);
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSGWITHPARAM(mode_infos.target, "+k", mode_infos.params));
	}
	else // si c'est '-'
	{
		if (pos == std::string::npos) // le mode n'est pas présent
			return;
		it_channel_target->second.removeMode("k");
		it_channel_target->second.removeChannelPassword();
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSGWITHPARAM(mode_infos.target, "-k", mode_infos.params));
	}
}