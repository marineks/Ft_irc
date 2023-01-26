#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	getName(std::string msg_to_parse);
static std::string	getPassword(std::string msg_to_parse);
static bool			isNameValid(Server *server, std::string name);
static bool			isPasswordValid(Server *server, std::string password, std::string pwd);


/**
 * @brief The OPER command is used by a normal user to obtain IRC operator privileges.
 * 	Both parameters are required for the command to be successful.
 *
 *  Command: OPER
 *  Parameters: <name> <password>
 *
 * Numeric Replies:
 *
 * ERR_NEEDMOREPARAMS (461)
 * ERR_PASSWDMISMATCH (464)
 * ERR_NOOPERHOST (491)
 * RPL_YOUREOPER (381)
 *
 * Example:
 *  [CLIENT] OPER foo bar
 *  [SERVER] ; Attempt to register as an operator using a name of "foo" and the password "bar".
 */
void oper(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client&		client		= retrieveClient(server, client_fd);
	std::string name		= getName(cmd_infos.message);
	std::string password	= getPassword(cmd_infos.message);

	if (name.empty() || password.empty())
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
	}
	else if (isNameValid(server, name) == false)
	{
		addToClientBuffer(server, client_fd, ERR_NOOPERHOST(client.getNickname()));
	}
	else if (isPasswordValid(server, password, name) == false)
	{
		addToClientBuffer(server, client_fd, ERR_PASSWDMISMATCH(client.getNickname()));
	}
	else
	{
		addToClientBuffer(server, client_fd, RPL_YOUREOPER(client.getNickname()));
		client.addMode("o");
		addToClientBuffer(server, client_fd, MODE_USERMSG(client.getNickname(), "+o"));
	}
}

static bool			isNameValid(Server *server, std::string name)
{
	std::vector<server_op>&				operators = server->getIrcOperators();
	std::vector<server_op>::iterator	op;

	for (op = operators.begin(); op != operators.end(); op++)
	{
		if (op->name == name)
		{
			return (true);
		}
	}
	return (false);
}

static bool			isPasswordValid(Server *server, std::string password, std::string name)
{
	std::vector<server_op>&				operators = server->getIrcOperators();
	std::vector<server_op>::iterator	op;

	for (op = operators.begin(); op != operators.end(); op++)
		if (op->name == name)
			break;

	return (op->password == password ? true : false);
}

static std::string	getName(std::string msg_to_parse)
{
	std::string name;

	name.clear();
	if (msg_to_parse.empty() == false || msg_to_parse.find(" ") != msg_to_parse.npos)
	{
		name.insert(0, msg_to_parse, 1,\
					msg_to_parse.find_last_of(" ") - 1);
	}
	return (name);
}

static std::string	getPassword(std::string msg_to_parse)
{
	std::string password;

	password.clear();
	if (msg_to_parse.empty() == false || msg_to_parse.find(" ") != msg_to_parse.npos)
	{
		password.insert(0, msg_to_parse,\
					msg_to_parse.find_last_of(" ") + 1,\
					msg_to_parse.size() - 1);
	}
	return (password);
}
