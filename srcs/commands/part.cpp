#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief The PART command removes the client from the given channel(s).
 * 
 * 	Syntax: PART <channel>{,<channel>} [<reason>]
 * 
 *  On sending a successful PART command, the user will receive a PART message 
 *  from the server for each channel they have been removed from. 
 *  <reason> is the reason that the client has left the channel(s).
 * 
 *  Numeric Replies:
 *   ERR_NEEDMOREPARAMS (461)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_NOTONCHANNEL (442)
 * 	
 * 	Example:
 * 	[IRSSI] /PART #test,#hey Dining
 * 	[SERVER] leaves both channels #test and #hey with the reason "Dining"
 * 	[SERVER to CLIENT]"@user_id PART #channel :Dining" (for EACH channel they leave)
 */
void	part(Server *server, int const client_fd, cmd_struct cmd_infos)
{

	// find reason if there is one
	// erase reason from msg

	// tout mettre dans la meme boucle que join

		// find channel
		// erase channel from msg

		
		// if chan exists and client not in it => ERR_NOTONCHANNEL
		// the chan will be ignored

		// if chan does not exist => ERR_NOSUCHCHANNEL
			// chan will be ignored
		
		// si chan valide, on envoie un message PART au client
}