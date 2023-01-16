#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void	ban(Server *server, std::string datas[4])
{
	server->printChannels();
	server->banClientFromChannel(datas[1], datas[3], datas[0]);
	server->printClientList();
}