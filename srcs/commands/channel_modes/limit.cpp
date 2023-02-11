#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void   limitChannelMode(Server *server, std::string datas[4], int const client_fd)
{
    if (server->isChannel(datas[1]) == false)
    {
        addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(datas[0], datas[1]));
        return ;
    }
    std::map<std::string, Channel>::iterator it;
    it = server->getChannels().find(datas[1]);
    int limit = atoi(datas[3].c_str());
    if (datas[2][0] == '+')
    {
		// check limit is valid
		if (limit < 0 || (int)it->second.getClientList().size() > limit)
			return ;
        it->second.setCapacityLimit(limit);
		size_t pos = it->second.getMode().find("l");
		if (pos != std::string::npos) // le mode est deja présent
			return;
		it->second.addMode("l");
        // convert integer in a string because can't use std::tostring in C++98
        std::ostringstream ss; 
        ss << limit;
		broadcastToAllChannelMembers(server, it->second, MODE_CHANNELMSGWITHPARAM(datas[1], "+l", ss.str()));
    }
    else if (datas[2][0] == '-')
    {
        limit = -1;
        it->second.setCapacityLimit(limit);
		size_t pos = it->second.getMode().find("l");
		if (pos == std::string::npos) // le mode est pas présent
			return;
		it->second.removeMode("l");
		broadcastToAllChannelMembers(server, it->second, MODE_CHANNELMSG(datas[1], "-l"));
    }
}