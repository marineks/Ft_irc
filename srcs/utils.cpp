#include "Irc.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	sendServerRpl(int const client_fd, std::string reply)
{
	send(client_fd, reply.c_str(),reply.size(), 0);
	std::cout << "[Server] Message sent to client " \
			  << client_fd << " >> " << CYAN << reply << RESET << std::endl;
}


Client&	retrieveClient(Server *server, int const client_fd)
{
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client = client_list.find(client_fd);
	;
	Client &client = it_client->second;
	return (client);
}

std::string	getListOfMembers(Channel &channel)
{
	std::map<std::string, Client>&			client_list	= channel.getClientList();
	std::map<std::string, Client>::iterator	it			= client_list.begin();
	std::string								nick;
	std::string								members_list;

	while (it != client_list.end())
	{
		nick.clear();
		nick = it->second.getNickname();
		members_list += nick;
		members_list += " ";
		it++;
	}
	if (members_list[members_list.size() - 1] == ' ')
		members_list.erase(members_list.end()-1);
	return (members_list);
}

std::string	getChannelName(std::string msg_to_parse)
{
	std::cout << "The msg_to_parse looks like this : |" << msg_to_parse << "|" << std::endl;
	// Expected output : | #foobar|
	// Expected output 2 : | #foo,#bar fubar,foobar|

	std::string channel_name;
	size_t i = 0;
	while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) && !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
		i++;
	while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
		channel_name += msg_to_parse[i++];
	std::cout << "The channel name is : |" << channel_name << "|" << std::endl;
	return (channel_name);
}