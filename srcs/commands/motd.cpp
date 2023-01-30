#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	getServer(std::string msg);
/**
 * @brief The MOTD command is used to get the “Message of the Day” of the given 
 * server. If <target> is not given, the MOTD of the server the client is 
 * connected to should be returned.
 * 
 *  Syntax : MOTD [<target>]
 * 
 *  If the MOTD can be found, one RPL_MOTDSTART numeric is returned, followed by
 *  one or more RPL_MOTD numeric, then one RPL_ENDOFMOTD numeric.
 * 
 * 	If the MOTD does not exist or could not be found, the ERR_NOMOTD numeric is returned.
 * 
 *  Numeric replies:
 *   ERR_NOSUCHSERVER (402)
 *   ERR_NOMOTD (422)
 *   RPL_MOTDSTART (375)
 *   RPL_MOTD (372)
 *   RPL_ENDOFMOTD (376)
 * 
 * 
 */
void	motd(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::string client = retrieveClient(server, client_fd).getNickname();
	std::string servername = getServer(cmd_infos.message);

	// checker en premier si le serveur c'est bien localhost
	if (servername.empty() == true) // par défaut, on doit afficher le motd de notre serveur
		servername = "localhost";
	if (servername != "localhost")
	{
		addToClientBuffer(server, client_fd, ERR_NOSUCHSERVER(client, servername));
		return ;
	}

	// récupérer le fichier et stocker les lignes dans un vecteur de strings
	std::ifstream		data;
	char				filepath[24] = "srcs/config/motd.config";

	data.open(filepath);
	if (!data)
	{
		addToClientBuffer(server, client_fd, ERR_NOMOTD(client));
		return ;
	}
	else
	{
		std::string		motd_lines;
		std::string		buf;
		
		buf = RPL_MOTDSTART(client, servername);
		while (getline(data, motd_lines))
		{
			buf += RPL_MOTD(client, motd_lines);
		}
		buf += RPL_ENDOFMOTD(client);
		addToClientBuffer(server, client_fd, buf);
	}
	data.close();
}

/* Possible tests:
	- ""
	- " r"
	- " bla foo bar" => on considère que les 3 mots sont le serveur
	- " "
*/
static std::string	getServer(std::string msg)
{
	std::string servername;
	servername.clear();

	if (msg.empty() == true)
		return (servername);
	
	if (msg.find(' ') == std::string::npos) // on a "r" / "bla foo"
	{
		servername.insert(0, msg, 0, std::string::npos);
	}
	else // Cas du " r" / " bla foo"
	{
		servername.insert(0, msg, 1, std::string::npos);
	}
	return (servername);
}