#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

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

	// checker en premier si le serveur c'est bien localhost. si pas de server donné
	// on va considérer que c'est localhost par défaut

	// store dans Server le char* file name (path)

	// ouvrir et récupérer dans un vecteur de strings le motd

	// renvoyer l'erreur MOTD s'il n'y a pas de file

	// sinon, lancer un motd start

	// boucler à travers le vecteur de strings pour tout envoyer

	// et envoyer la end
}