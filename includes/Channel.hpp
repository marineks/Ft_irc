#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Irc.hpp"
#include "Client.hpp"

class	Client;

class Channel
{
	private:
		std::map<std::string, Client>	_clientList;
		std::vector<std::string>		_banned_users;
		std::vector<std::string>		_operators;
		std::string 					_name;
		std::string						_operatorPassword;
		std::string						_topic;
		int								_limit;
		std::string						_password;
	public:
		Channel(std::string const &name);
		~Channel();

		/* Accessors */
		std::string&					getName();
		std::string&					getTopic() ;
		std::vector<std::string>&		getOperators() ;
		std::vector<std::string>&		getBannedUsers() ;
		std::map <std::string, Client>&	getClientList();
		int								getLimit();
		std::string&					getPassword();

		void							setPassword(std::string &password);
		void							setLimit(int &value);
		void							setTopic(std::string& newTopic);
		bool							doesClientExist(std::string &clientName);
		/* Manage client in Channel */
		int								addClientToChannel(Client &client);
		void							printClientList();
		void							removeClientFromChannel(std::string &clientName);
		/* Operators */
		void							addFirstOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		// void							addOperator(std::string operatorName);
		bool							isOperator(std::string &operatorName);
		void							printOperators(void);
		/* Client status */
		void							addToBanned(std::string &banned_name);
		void							removeFromBanned(std::string &banned_name);
		bool							isBanned(std::string &banned_name);
};

#endif