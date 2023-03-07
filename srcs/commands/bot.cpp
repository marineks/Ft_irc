#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static void	botHour(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot)
{
	std::stringstream ss;
    std::time_t t = std::time(NULL);
    std::tm* tm_local = std::localtime(&t);

    ss << "Current local time: " << tm_local->tm_hour << ":" 
       << tm_local->tm_min << ":" << tm_local->tm_sec;
    
    std::string time = ss.str();
	addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), time));
}

static void	botRandom(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot)
{
	srand(time(NULL)); // initializes the random number generator with a seed value based on the current time
    int index = rand() % 10 + 1; // number between 1 and 10
	
	std::string str;
	switch (index)
	{
		case 1: str = "Wearing headphones for just an hour could increase the bacteria in your ear by 700 times."; break;
		case 2: str = "Google images was literally created after Jennifer Lopez wore that infamous dress at the 2000 Grammys"; break;
		case 3: str = "Los Angeles' full name is 'El Pueblo de Nuestra Senora la Reina de los Angeles de Porciuncula'"; break;
		case 4: str = "Tigers have striped skin, not just striped fur."; break;
		case 5: str = "Like fingerprints, everyone's tongue print is different."; break;
		case 6: str = "Cat urine glows under a black-light."; break;
		case 7: str = "A shrimp's heart is in its head."; break;
		case 8: str = "The Spice Girls were originally a band called Touch."; break;
		case 9: str = "The unicorn is the national animal of Scotland"; break;
		case 10: str = "In 2014, there was a Tinder match in Antarctica"; break;
	}
	addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), str));
}

void	bot(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot_cmd)
{
	std::string bot = "chatGTP";
	std::string	validCmds[4] = {
		":HELP",
		":HOUR",
		":LOVE",
		":RANDOM",
		};

	// change bot_cmd into caps letter
	for (size_t i = 0; i < bot_cmd.size(); i++)
		bot_cmd[i] = std::toupper(bot_cmd[i]);
	
	int index = 0;
	while (index < 4)
	{
		if (bot_cmd == validCmds[index])
			break;
		index++;
	}
	switch (index + 1)
	{
		case 1: addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Ask me 'HOUR', 'LOVE' or 'RANDOM'")); break;
		case 2: botHour(server, client_fd, it_client, bot); break;
		case 3: addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "dyoula, msanjuan, tmanolis and myself send you love through this terminal <3")); break;
		case 4: botRandom(server, client_fd, it_client, bot); break;
		default:
			addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Invalid request, ask 'HELP' for more infos"));
	}
}
