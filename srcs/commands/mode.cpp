#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief The MODE command is used to set or remove options (or modes) from a given target.
 * 
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 * 
 * <modestring> starts with a plus ('+', 0x2B) or minus ('-', 0x2D) character, and is made up of the following characters:
 * '+': Adds the following mode(s).
 * '-': Removes the following mode(s).
 * 'a-zA-Z': Mode letters, indicating which modes are to be added/removed.
 * 
 * User case :
 * If <modestring> is given, the supplied modes will be applied : a MODE message will be sent to the user containing the changed modes. 
 * If one or more modes sent are not implemented on the server, 
 * the server MUST apply the modes that are implemented, and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.
 * 
 * Channel case :
 * syntax command : /mode <channel> <+ ou -> <mode> [parametres]
 * If the user has permission to change modes on the target, the supplied modes will be applied based on the type of the mode (see below). 
 * For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. 
 * - If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode. 
 * - If a type A mode has been sent without an argument : 
 * the contents of the list MUST be sent to the user, 
 * unless it contains sensitive information the user is not allowed to access. 
 * When the server is done processing the modes, a MODE command is sent to all members of the channel containing the mode changes. 
 * Servers MAY choose to hide sensitive information when sending the mode changes.
 * 
 * Exemples :
 * - +o : /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
 * Message à send aux users du channel : ':irc.example.com MODE #cool +o MEAT'; 
 * The irc.example.com server gave channel operator privileges to MEAT on #cool.
 * - +k : /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
 * - +s : /mode #cool +s (le channel devient secret)
 * - plusieur modes : /mode #cool +ts (le channel est en mode +t +s)
 * 
 * @param server
 * @param client_fd User sending a msg
 * @param cmd_infos Structure w/ prefix, command name and message
 * 
 * 
 * NUMERIC REPLIES:
 * User mode :
 * - ERR_NOSUCHNICK (401) : If <target> is a nickname that does not exist on the network,
 * - ERR_USERSDONTMATCH (502) : If <target> is a different nick than the user who sent the command
 * - RPL_UMODEIS (221) : If <modestring> is not given, the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.
 * - ERR_UMODEUNKNOWNFLAG (501) : If one or more modes sent are not implemented on the server
 * 
 * 
 * Channel mode :
 * - ERR_NOSUCHCHANNEL (403) : If <target> is a channel that does not exist on the network
 * - RPL_CHANNELMODEIS (324) : If <modestring> is not given.
 * "<client> <channel> <modestring> <mode arguments>..." => Sent to a client to inform them of the currently-set modes of a channel. 
 * <channel> is the name of the channel.
 * <modestring> and <mode arguments> are a mode string and the mode arguments (delimited as separate parameters) as defined in the MODE message description.
 * Servers MAY choose to hide sensitive information such as channel keys when sending the current modes.
 * - ERR_CHANOPRIVSNEEDED (482) :  If a user does not have appropriate privileges to change modes on the target channel, the server MUST NOT process the message, and return this error
 * 
 * Useful link : https://modern.ircdocs.horse/#mode-message
 * https://www.techbull.com/techbull/guide/internet/irccommande.html
 * 
 */


/*
A CODER POUR l'USER : +i / +o
- Invisible User Mode :
This mode is standard, and the mode letter used for it is "+i".
If a user is set to ‘invisible’, they will not show up in commands such as WHO or NAMES unless they share a channel with the user that submitted the command. 
In addition, some servers hide all channels from the WHOIS reply of an invisible user they do not share with the user that submitted the command.

- Oper User Mode :
This mode is standard, and the mode letter used for is it "+o".
If a user has this mode, this indicates that they are a network operator.


A CODER POUR LE CHANNEL : +o / +k / +s / +t
- Key Channel Mode :
This mode is standard, and the mode letter used for it is "+k".
This mode letter sets a ‘key’ that must be supplied in order to join this channel. 
If this mode is set, its’ value is the key that is required. 
Servers may validate the value (eg. to forbid spaces, as they make it harder to use the key in JOIN messages). 
If the value is invalid, they SHOULD return ERR_INVALIDMODEPARAM. However, clients MUST be able to handle any of the following:

    ERR_INVALIDMODEPARAM
    ERR_INVALIDKEY
    MODE echoed with a different key (eg. truncated or stripped of invalid characters)
    the key changed ignored, and no MODE echoed if no other mode change was valid.

If this mode is set on a channel, and a client sends a JOIN request for that channel, they must supply <key> in order for the command to succeed. 
If they do not supply a <key>, or the key they supply does not match the value of this mode, they will receive an ERR_BADCHANNELKEY (475) reply and the command will fail.

- Secret Channel Mode :
This mode is standard, and the mode letter used for it is "+s".
This channel mode controls whether the channel is ‘secret’, and does not have any value.
A channel that is set to secret will not show up in responses to the LIST or NAMES command unless the client sending the command is joined to the channel. 
Likewise, secret channels will not show up in the RPL_WHOISCHANNELS (319) numeric unless the user the numeric is being sent to is joined to that channel.

- Protected Topic Mode :
This mode is standard, and the mode letter used for it is "+t".
This channel mode controls whether channel privileges are required to set the topic, and does not have any value.
If this mode is enabled, users must have channel privileges such as halfop or operator status in order to change the topic of a channel. 
In a channel that does not have this mode enabled, anyone may set the topic of the channel using the TOPIC command.

*/

// syntax command : /mode <channel> <+ ou -> <mode> [parametres]

struct mode_struct
{
	std::string	prefix;
	std::string	name;
	std::string	message;
};

void	mode(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	(void)server;
	(void)client_fd;
	
	std::cout << "\nMessage : |" << cmd_infos.message << "|" << std::endl;


}