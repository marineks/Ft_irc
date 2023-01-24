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
    ERR_NOSUCHNICK (401) -OK
    ERR_NOSUCHSERVER (402)
    ERR_CANNOTSENDTOCHAN (404)
    ERR_TOOMANYTARGETS (407) 
    // TODO ? ERR_TOOMANYTARGETS, a checker si on accepte autte chose que le nick avec irssi
    // TODO : Renvoyé à un client qui essaie d'envoyer un PRIVMSG/NOTICE utilisant le format de destination utilisateur@hôte pour lequel utilisateur@hôte a plusieurs occurrences.
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
   https://modern.ircdocs.horse/#errnosuchnick-401
   http://abcdrfc.free.fr/rfc-vf/rfc1459.html (errors)
   https://askubuntu.com/questions/855881/irssi-where-do-private-messages-go (how to use IRSSI)
 * 
 */

static void  broadcastToChannel(int const client_fd, std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel, std::string message)
{
   // TODO: check a faire avec les modes
         // checker si user membre du channel -> si oui : boucle for to send to every user in the channel
         // -> si non : checker si le mode du channel permet d'envoyer des messages
   
   // check si client est kick du channel
   std::vector<std::string> kicked_users = it_channel->second.getKickedUsers();

	for (std::vector<std::string>::iterator it = kicked_users.begin(); it != kicked_users.end(); it++)
	{
		if (*it == it_client->second.getNickname())
		{
			std::cout << it_client->second.getNickname() << " is kicked from the channel and can't send message anymore" << std::endl;
			return ;
		}
	}

   // Envoi le message aux users du channel 
   std::map<std::string, Client>::iterator member = it_channel->second.getClientList().begin(); // debut de la liste des clients du channel
   while (member != it_channel->second.getClientList().end())
   {
      if (member->second.getClientFd() != client_fd)   // prevent to send the message to the sender
          sendServerRpl(member->second.getClientFd(), RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), message));
      member++;
   }
}

void	privmsg(Server *server, int const client_fd, cmd_struct cmd_infos)
{  
   std::map<const int, Client>	client_list = server->getClients();
   std::map<std::string, Channel> channel_list = server->getChannels(); 
   std::map<const int, Client>::iterator it_client = client_list.find(client_fd); // trouver le client qui envoie

   // Parsing message 
   size_t      delimiter = cmd_infos.message.rfind(":");
   std::string target = cmd_infos.message.substr(1, (delimiter - 2)); // end before the space there is before the delimiter ':'
   std::string msg_to_send = cmd_infos.message.substr(delimiter);

   // Error syntaxe message
   if (target.empty())        // pas de destinataire 
      sendServerRpl(client_fd, ERR_NORECIPIENT(it_client->second.getNickname()));
   if (msg_to_send.empty())   // pas de message
      sendServerRpl(client_fd, ERR_NOTEXTTOSEND(it_client->second.getNickname()));

   // Channel case
   if (target[0] == '#')
   {
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target.substr(1)); // find channel name by skipping the '#' character

      if (it_channel == channel_list.end())
         sendServerRpl(client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
      else
      {
         // If channel is on Moderation mode and the client does not have the Voice mode to talk in the channel
         if (it_channel->second.getModeration() == true \
            && it_channel->second.hasVoice(it_client->second.getNickname()) == false)
         {
            std::cout << "[SERVER] The channel is moderated and the user does not have the voice (+v) to talk." << std::endl;
            sendServerRpl(client_fd, ERR_CANNOTSENDTOCHAN(it_client->second.getNickname(), target));
         } 
         else
            broadcastToChannel(client_fd, it_client, it_channel, cmd_infos.message);
      }
         
   }
   // user case
   else
   {
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target); // find channel name
     
      std::map<const int, Client>::iterator it_target = client_list.begin();
      while (it_target!=client_list.end())
      {
         if (it_target->second.getNickname() == target)
             break;
         it_target++;
      }
      if (it_target == client_list.end() && it_channel == channel_list.end()) // user and channel doesn't exist
         sendServerRpl(client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
      else
      {
         if (it_target == client_list.end()) // si le user n'existe pas mais le channel oui (gestion channel actif)
         {
            cmd_infos.message.insert(1, "#");  // ajouter le # before target
            broadcastToChannel(client_fd, it_client, it_channel, cmd_infos.message);
         }
         else
            sendServerRpl(it_target->first, RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), cmd_infos.message));
      }  
   }
}