#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Irc.hpp"

class Client
{
	private:
		int				_client_fd;
		std::string		_nickname;
		std::string		_old_nickname;
		std::string		_fullname;
		std::string		_realname;
		std::string		_mdp;
	
	public:
		Client(int client_fd);
		~Client();
		
		int				getClientFd()const;
		void			setNickname(std::string const &nickname);
		std::string&	getNickname();
		void			setOldNickname(std::string const &nickname);
		std::string&	getOldNickname();
		void			setUsername(std::string const &username);
		std::string		getUsername()const;
		void			setRealname(std::string const &realname);
		std::string		getRealname()const;
		
		void			printClient()const;
		int				is_valid()const;
};

#endif