#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <bitset>
#include <climits>
#include <cstring>
#include <iostream>

int	main(void)
{
	int				status;
	struct addrinfo	hints;
	struct addrinfo	*servinfo; // will point to the results

	std::memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET; // ipv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	//	1)			connect to   a server       w/ port
	if ((status = getaddrinfo("www.example.net", "3490", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	//  2) SOCKET() - get tthe file descriptor !
	int s;

	freeaddrinfo(servinfo);

	return 0;
}
