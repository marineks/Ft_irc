#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	findAnyChannel(std::string msg_to_parse);

/**
 * @brief If the exact name of a channel is given, the only information about 
 * 		this channel is requested; otherwise, a list of all channels will be 
 * 		displayed.
 * 
 * 	Syntax : 
 * 	[IRSSI] LIST [-yes] [<channel>]
 * 	
 * 	Numeric Replies:
 * 		RPL_LISTSTART (321) : marks the start of a channel list. 
 * 		RPL_LIST (322) : sends information about a channel to the client.
 * 		RPL_LISTEND (323) : indicates the end of a LIST response.
 * 
 * 	Examples:
 * 		/LIST
 * 		/LIST -yes => "LIST" when received by server
 * 		/LIST #ubuntu
 */
void		list(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::string channel_to_display = findAnyChannel(cmd_infos.message);

	std::cout << "Le channel found est : " << YELLOW << (channel_to_display.empty()? "Y a R!" : channel_to_display) << RESET << std::endl;
	// if (channel_to_display.empty())
	// {
	// 	// list of channels w/ some relevant info
	// 	return ;
	// }
	// else
	// {
	// 	// check if channel found exists

	// 	// if it exists, display
	// 	// else, print END of LIST
	// }
}

static std::string	findAnyChannel(std::string msg_to_parse)
{
	std::string	channel;
	channel.clear();

	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos) // Si pas d'arg ou pas de chan (#)
		return (channel);
	else
	{
		size_t i = 0;
		while (!isalpha(msg_to_parse[i]))
			i++;
		while (isalpha(msg_to_parse[i]))
			channel += msg_to_parse[i++];
		return (channel);
	}
}
