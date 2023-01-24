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

void	passwordChannel(Server *server, std::string datas[4], int &client_fd)
{
	server->managePassword(datas, datas[2][0], client_fd);
}

void	limit(Server *server, std::string datas[4], int &client_fd)
{
	server->manageLimit(datas, client_fd);
}

void	moderation(Server *server, std::string datas[4], int &client_fd)
{
	server->manageModeration(datas, client_fd);
}

void	operators(Server *server, std::string datas[4], int &client_fd)
{
	server->manageOperator(datas, client_fd);
}

void	priv(Server *server, std::string datas[4], int &client_fd)
{
	server->managePrivate(datas, client_fd);
}

void	secret(Server *server, std::string datas[4], int &client_fd)
{
	server->manageSecret(datas, client_fd);
}

void	topicProtection(Server *server, std::string datas[4], int &client_fd)
{
	server->manageTopicProtection(datas, client_fd);
}

void	voice(Server *server, std::string datas[4], int &client_fd)
{
	server->manageVoice(datas, client_fd);
}