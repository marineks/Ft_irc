#include "Irc.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	addToClientBuffer(Server *server, int const client_fd, std::string reply)
{
	Client &client = retrieveClient(server, client_fd);

	client.setSendBuffer(reply);
}

void	sendServerRpl(int const client_fd, std::string client_buffer)
{
	std::istringstream	buf(client_buffer);
	std::string			reply;
	
	send(client_fd, client_buffer.c_str(), client_buffer.size(), 0);
	while (getline(buf, reply))
	{
		std::cout << std::endl << "[Server] Message sent to client " \
				  << client_fd << "       >> " << CYAN << reply << RESET << std::endl;
	}
}

Client&	retrieveClient(Server *server, int const client_fd)
{
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client = client_list.find(client_fd);
	
	Client &client = it_client->second;
	return (client);
}

Client*	getClient(Server *server, int const client_fd)
{
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client = client_list.find(client_fd);
	
	if (it_client == client_list.end())
		return (NULL);
	return (&it_client->second);
}

std::string	getListOfMembers(std::string client, Channel &channel)
{
	std::map<std::string, Client>&			client_list	= channel.getClientList();
	std::map<std::string, Client>::iterator	it;
	std::string								nick;
	std::string								members_list;

	for (it = client_list.begin(); it != client_list.end(); it++)
	{
		nick.clear();
		nick = it->second.getNickname();
		if (it->second.getMode().find('i') != std::string::npos\
			&& channel.doesClientExist(client) == false)
				continue;
			
		if (channel.isOperator(nick) == true)
			members_list += "@";
		members_list += nick;
		members_list += " ";
	}
	if (members_list.size() >= 1 && members_list[members_list.size() - 1] == ' ')
		members_list.erase(members_list.end()-1);
	return (members_list);
}

std::string	getChannelName(std::string msg_to_parse)
{
	std::string channel_name;
	size_t i = 0;
	while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) && !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
		i++;
	while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
		channel_name += msg_to_parse[i++];
	return (channel_name);
}