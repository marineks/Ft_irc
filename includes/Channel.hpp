#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Irc.hpp"
#include "Client.hpp"

class	Client;

class Channel
{
	private:
		std::map<std::string, Client>	_clientList;
		std::vector<std::string>		_kicked_users;
		std::vector<std::string>		_banned_users;
		std::vector<std::string>		_operators;
		std::string 					_name;
		std::string						_operatorPassword;
		std::string						_topic;
		std::string						_mode;
		std::string						_channel_password;
	public:
		Channel(std::string const &name);
		~Channel();

		/* Accessors */
		std::string&					getName();
		std::string&					getTopic() ;
		std::string&					getMode() ;
		std::string&					getChannelPassword() ;
		std::vector<std::string>&		getOperators() ;
		std::vector<std::string>&		getKickedUsers() ;
		std::vector<std::string>&		getBannedUsers() ;
		std::map <std::string, Client>&	getClientList();
		void							setTopic(std::string& newTopic);
		void							setChannelPassword(std::string password);
		bool							doesClientExist(std::string &clientName);
		/* Manage client in Channel */
		void							addClientToChannel(Client &client);
		void							printClientList();
		void							removeClientFromChannel(std::string &clientName);
		/* Operators */
		void							addFirstOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		// void							addOperator(std::string operatorName);
		bool							isOperator(std::string &operatorName);
		void							printOperators(void);
		/* Modes */
		void							addMode(std::string const mode);
		void							removeMode(std::string const mode);
		void							removeChannelPassword();
		/* Client status */
		void							addToKicked(std::string &banned_name);
		void							addToBanned(std::string &banned_name);
		void							removeFromBanned(std::string &banned_name);
		bool							isBanned(std::string &banned_name);
};

#endif