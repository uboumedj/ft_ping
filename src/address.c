#include "../inc/ft_ping.h"

/*
** Address parsing and checking
*/

/*
** function: get_address
** ---------------------
** parses the program's arguments, stopping at the first occurence of an argument
** that doesn't seem to be an option
*/

void	get_address(char **dest, char **argv)
{
	int i = 1;

	while (argv[i])
	{
		if (argv[i][0] != '-' && check_previous_argument(argv, i) == SUCCESS_CODE)
		{
			*dest = argv[i];
			return;
		}
		i++;
	}
	error_output_and_exit(USAGE);
}

/*
** function: check_previous_argument
** ---------------------------------
** used by get_address to check the previous argument, to make sure it is not an
** option that requires the current argument as a value (-t or -c for example)
*/

char	check_previous_argument(char **argv, int i)
{
	if (i == 1)
		return (SUCCESS_CODE);
	i -= 1;
	if (argv[i][0] != '-')
		return (SUCCESS_CODE);
	if (argv[i][1] != 'c' && argv[i][1] != 't' && argv[i][1] != 'i')
		return (SUCCESS_CODE);
	return (ERROR_CODE);
}

/*
** function: interpret_address
** ---------------------------
** checks the user-requested address with getaddrinfo. If it exists, saves its real IP
** address
*/

void	interpret_address(char *input)
{
	char				address[INET_ADDRSTRLEN];
	struct addrinfo		*res = NULL;
	struct addrinfo		hints;
	

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(input, NULL, &hints, &res) != 0)
		fprintf(stderr, UNKNOWN_ADDR_ERROR, input);
	else
	{
		ft_memcpy(&ping.sockaddr, res->ai_addr, sizeof(struct sockaddr_in));
		inet_ntop(AF_INET, &(ping.sockaddr.sin_addr), address, INET_ADDRSTRLEN);
		ping.address = ft_strdup(address);
		reverse_dns();
	}
	if (res)
		freeaddrinfo(res);
}

/*
** function: reverse_dns
** ---------------------
** as ping also uses the address' hostname sometimes, this function performs a reverse DNS
** lookup to get it. If there isn't one, it simply copies the initial address.
*/

void	reverse_dns(void)
{
	char		reversed_address[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)&ping.sockaddr, sizeof(struct sockaddr_in),
		reversed_address, sizeof(reversed_address), NULL, 0, NI_NAMEREQD) < 0)
	{
		ping.reversed_address = ft_strdup(ping.address);
	}
	else
		ping.reversed_address = ft_strdup(reversed_address);
}