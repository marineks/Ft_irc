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
		std::vector<std::string>		_voiced;
		std::string 					_name;
		std::string						_operatorPassword;
		std::string						_topic;
		int								_limit;
		std::string						_password;
		bool							_keyMode;
		bool							_secret;
		bool							_private;
		bool							_topic_protection;
		bool							_moderation;
	public:
		Channel(std::string const &name);
		~Channel();

		/* Accessors */
		std::string&					getName();
		std::string&					getTopic() ;
		std::vector<std::string>&		getOperators() ;
		std::vector<std::string>&		getKickedUsers() ;
		std::vector<std::string>&		getBannedUsers() ;
		std::map <std::string, Client>&	getClientList();
		int								getLimit();
		std::string&					getPassword();
		bool							getKeyMode() const;
		bool							getSecret()const;
		bool							getPrivate()const;
		bool							getTopicProtection()const;
		bool							getModeration()const;

		void							setPassword(std::string &password);
		void							setKeyMode(bool value);
		void							setLimit(int &value);
		void							setTopic(std::string& newTopic);
		void							setSecret(int i);
		void							setPrivate(int i);
		void							setTopicProtection(int i);
		void							setModeration(bool i);
		bool							doesClientExist(std::string &clientName);
		/* Manage client in Channel */
		int								addClientToChannel(Client &client);
		void							printClientList();
		void							removeClientFromChannel(std::string &clientName);
		/* Operators */
		void							addFirstOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		void							addOperator(std::string operatorName);
		bool							isOperator(std::string &operatorName);
		void							printOperators(void);
		/* Client status */
		void							addToKicked(std::string &banned_name);
		void							addToBanned(std::string &banned_name);
		void							removeFromBanned(std::string &banned_name);
		bool							isBanned(std::string &banned_name);
		bool							goodPassword(std::string &password);
		void							addVoice(std::string &clientName);
		void							removeVoice(std::string &clientName);
		bool							hasVoice(std::string &clientName);
};

#endif