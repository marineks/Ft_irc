#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief The NOTICE command is used to send notices between users, as well as to send notices to channels. 
 * <target> is interpreted the same way as it is for the PRIVMSG command.
 * 
 * The NOTICE message is used similarly to PRIVMSG. 
 * The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message. 
 * This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command. 

   @param server
   @param client_fd User sending a msg
   @param cmd_infos Structure w/ prefix, command name and message

   Useful link : https://modern.ircdocs.horse/#notice-message
 * 
 */

static void  broadcastToChannel(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel, std::string message)
{
   // TODO: check a faire avec les modes
         // checker si user membre du channel -> si oui : boucle for to send to every user in the channel
         // -> si non : checker si le mode du channel permet d'envoyer des messages
   
   std::map<std::string, Client>::iterator member = it_channel->second.getClientList().begin(); // debut de la liste des clients du channel
    while (member != it_channel->second.getClientList().end())
   {
      if (member->second.getClientFd() != client_fd)   // prevent to send the message to the sender
      {
         addToClientBuffer(server, member->second.getClientFd(), RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), message));
         // sendServerRpl(member->second.getClientFd(), RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), message));
      }
          
      member++;
   }
}

void	notice(Server *server, int const client_fd, cmd_struct cmd_infos)
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
      return ;
   if (msg_to_send.empty())   // pas de message
      return ;

   // Channel case
   if (target[0] == '#')
   {
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target.substr(1)); // find channel name by skipping the '#' character
      if (it_channel == channel_list.end())
         return ;
      else
         broadcastToChannel(server, client_fd, it_client, it_channel, cmd_infos.message);
   }
   // user case
   else
   {     
      std::map<const int, Client>::iterator it_target = client_list.begin();
      while (it_target!=client_list.end())
      {
         if (it_target->second.getNickname() == target)
             break;
         it_target++;
      }
      if (it_target == client_list.end())
         return ;
      else
      {
         addToClientBuffer(server, it_target->first, RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), cmd_infos.message));
         // sendServerRpl(it_target->first, RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), cmd_infos.message));
      }
        
   }
}