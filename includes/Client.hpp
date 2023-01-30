#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Irc.hpp"

class Client
{
	private:
		int				_client_fd;
		std::string		_readbuf;
		std::string		_sendbuf;
		bool			_to_deconnect;
		std::string		_nickname;
		std::string		_old_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_mode;
		bool			_connexion_password;
		bool			_registrationDone;
		bool			_welcomeSent;
		bool			_hasAllInfo;
		int				_nbInfo;
	
	public:
		Client(int client_fd);
		~Client();
		
		// Server infos
		int				getClientFd()const;
		void			setNickname(std::string const &nickname);
		std::string&	getReadBuffer();
		void			setReadBuffer(std::string const &buf);
		void			resetReadBuffer(std::string const &str);
		std::string&	getSendBuffer();
		void			setSendBuffer(std::string const &buf);
		bool&			getDeconnexionStatus();
		void			setDeconnexionStatus(bool status);
		// Client Registration infos
		std::string&	getNickname();
		void			setOldNickname(std::string const &nickname);
		std::string&	getOldNickname();
		void			setUsername(std::string const &username);
		std::string		getUsername()const;
		void			setRealname(std::string const &realname);
		std::string		getRealname()const;
		bool&			getConnexionPassword();
		void			setConnexionPassword(bool boolean);
		// User modes
		std::string&	getMode();
		void			addMode(std::string const mode);
		void			removeMode(std::string const mode);
		// Client registration system
		bool&			isRegistrationDone();
		void			setRegistrationDone(bool boolean);
		bool&			isWelcomeSent();
		void			setWelcomeSent(bool boolean);
		bool&			hasAllInfo();
		void			sethasAllInfo(bool boolean);
		int				getNbInfo() const;
		void			setNbInfo(int n);
		int				is_valid()const;
};

#endif