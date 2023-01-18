#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief PRIVMSG is used to send private messages between users, as well as to
   send messages to channels.  <msgtarget> is usually the nickname of
   the recipient of the message, or a channel name.
   
   Parameters: <msgtarget> <text to be sent>

   The <msgtarget> parameter may also be a host mask (#<mask>) or server
   mask ($<mask>).  In both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask MUST
   have at least 1 (one) "." in it and no wildcards following the last
   ".".  This requirement exists to prevent people sending messages to
   "#*" or "$*", which would broadcast to all users.  Wildcards are the
   '*' and '?'  characters.  This extension to the PRIVMSG command is
   only available to operators.

   Numeric Replies:
    ERR_NOSUCHNICK (401)
    ERR_NOSUCHSERVER (402)
    ERR_CANNOTSENDTOCHAN (404)
    ERR_TOOMANYTARGETS (407)
    ERR_NORECIPIENT (411) -OK
    ERR_NOTEXTTOSEND (412) -OK
    ERR_NOTOPLEVEL (413)
    ERR_WILDTOPLEVEL (414)
    RPL_AWAY (301)

   @param server
   @param client_fd User sending a msg
   @param cmd_infos Structure w/ prefix, command name and message

   EXAMPLES :
   Examples:

   :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
    ; Message from Angel to Wiz.

   PRIVMSG Angel :yes I'm receiving it !
	; Command to send a message to Angel.

   PRIVMSG jto@tolsun.oulu.fi :Hello !
	; Command to send a message to a user on server tolsun.oulu.fi with
    username of "jto".

   PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
   	; Message to a user on server irc.stealth.net with username of "kalt", 
	and connected from the host millennium.stealth.net.

   PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
    ; Message to a user on the local server with username of "kalt", and
    connected from the host millennium.stealth.net.

   PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
   	; Message to the user with nickname Wiz who is connected from the host
    tolsun.oulu.fi and has the username "jto".

   PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
    ; Message to everyone on a server which has a name matching *.fi.

   PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
    ; Message to all users who come from a host which has a name matching *.edu.

	useful link : https://irssi.org/documentation/help/msg/
	https://modern.ircdocs.horse/#privmsg-message
   http://abcdrfc.free.fr/rfc-vf/rfc1459.html (errors)
 * 
 */


void	privmsg(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	// std::cout << "prefix : " << cmd_infos.prefix << std::endl;
	// std::cout << "cmd_name : " << cmd_infos.name << std::endl;
	std::cout << YELLOW << "message : |" << cmd_infos.message << "|" << RESET << std::endl;
   (void)client_fd;
   std::map<const int, Client>	client_list = server->getClients();
   std::map<std::string, Channel> channel_list = server->getChannels();


   size_t      delimiter = cmd_infos.message.rfind(":");
   std::string target = cmd_infos.message.substr(1, (delimiter - 2)); // end before the space there is before the delimiter ':'
   std::string msg_to_send = cmd_infos.message.substr(delimiter);
   
   std::cout << "\ntarget to send : |" << target << "|" << std::endl;
   std::cout << "Message to send : |" << msg_to_send << "|" << std::endl;

   // if (target.empty()) // pas de destinataire
      // send : 411 ERR_NORECIPIENT ":No recipient given (<commande>)"
   // if (msg_to_send.empty())
      // send : 412 ERR_NOTEXTTOSEND : ":No text to send" 

   // Channel case
   if (target[0] == '#')
   {
      std::map<std::string, Channel>::iterator it;
      it = channel_list.find(target.substr(1)); // skip the '#' character
      if (it == channel_list.end())
      {
         std::cout << "channel doesn't exist" << std::endl;
         // send :401 ERR_NOSUCHNICK : "<pseudonyme> :No such nick/channel"
      }
      else
      {
         std::cout << "channel exist" << std::endl;
         // checker si user membre du channel -> si oui : boucle for to send to every user in the channel
         // -> si non : checker si le mode du channel permet d'envoyer des messages
      }
   }
   else // user case
   {
      std::map<const int, Client>::iterator it = client_list.begin();

      while (it!=client_list.end())
      {
         if (it->second.getNickname() == target)
            break;
         it++;
      }

      if (it == client_list.end())
      {
         std::cout << "user doesn't exist" << std::endl;
         // send :401 ERR_NOSUCHNICK : "<pseudonyme> :No such nick/channel"
      }
      else
      {
         std::string reply;

         reply = user_id(it->second.getNickname(), it->second.getUsername()) + " " + cmd_infos.name + " " + cmd_infos.message;
         std::cout << "reply to send to server : " << reply << std::endl;
         std::cout << "client_fd target : " << it->first << std::endl;
         sendServerRpl(it->first, reply);
      }
   }

}