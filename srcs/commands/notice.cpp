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

static void  broadcastToChannel(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel, std::string target, std::string msg_to_send)
{
   
   // check si client est kick du channel
   std::vector<std::string> kicked_users = it_channel->second.getKickedUsers();

	for (std::vector<std::string>::iterator it = kicked_users.begin(); it != kicked_users.end(); it++)
	{
		if (*it == it_client->second.getNickname())
		{
			// std::cout << it_client->second.getNickname() << " is kicked from the channel and can't send message anymore" << std::endl;
			return ;
		}
	}

   // Envoi le message aux users du channel 
   std::map<std::string, Client>::iterator member = it_channel->second.getClientList().begin(); // debut de la liste des clients du channel
   while (member != it_channel->second.getClientList().end())
   {
      if (member->second.getClientFd() != client_fd)   // prevent to send the message to the sender
         addToClientBuffer(server, member->second.getClientFd(), RPL_NOTICE(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));
      member++;
   }
}

static bool isUserinChannel(std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel)
{
   if (it_channel->second.getClientList().find(it_client->second.getNickname()) != it_channel->second.getClientList().end())
      return (true);
   return (false);
}

void	notice(Server *server, int const client_fd, cmd_struct cmd_infos)
{ 
   std::map<const int, Client>	client_list = server->getClients();
   std::map<std::string, Channel> channel_list = server->getChannels(); 
   std::map<const int, Client>::iterator it_client = client_list.find(client_fd); // trouver le client qui envoie

   std::string target;
   std::string msg_to_send;
  
   // Parsing message 
   size_t      delimiter = cmd_infos.message.rfind(":");
   if (delimiter == std::string::npos) // pas de : -> donc pas de message
      return ;
   target = cmd_infos.message.substr(1, delimiter - 1); // s'arrete avant le delimiter
   if (target.empty())
      return ;
   msg_to_send = cmd_infos.message.substr(delimiter);

   delimiter = target.find(" ");
   if (delimiter != std::string::npos) // permet de gérer le double :: et les espaces de la fin
      target.erase(delimiter);  
  
   // Channel case
   if (target[0] == '#')
   {
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target.substr(1)); // find channel name by skipping the '#' character

      if (it_channel == channel_list.end())
         return ;
      else
         broadcastToChannel(server, client_fd, it_client, it_channel, target, msg_to_send);
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
         return ;  
      else
      {
         if (it_target == client_list.end()) // si le user n'existe pas mais le channel oui (gestion channel actif)
         {
            if (isUserinChannel(it_client, it_channel) == true)
            {
               target.insert(1, "#");  // ajouter le # before target
               broadcastToChannel(server, client_fd, it_client, it_channel, target, msg_to_send);
            }
            else
               return ;
         }
         else
            addToClientBuffer(server, it_target->first, RPL_NOTICE(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));    
      }
   }
}