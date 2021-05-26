#include "../inc/ft_ping.h"

/*
** Functions that handle socket creation and manipulation
*/

/*
** function: create_socket
** -----------------------
** opens a new socket and saves its fd to [ping.socket_fd]
*/

void	create_socket(void)
{
	int				socket_fd;
	struct timeval	timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if ((socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		error_output_and_exit(SOCKET_ERROR);
	if ((setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &(ping.ttl), sizeof(ping.ttl))) == -1)
		error_output_and_exit(SETSOCKOPT_ERROR);
	if ((setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeout, sizeof(timeout))) == -1)
		error_output_and_exit(SETSOCKOPT_ERROR);
	ping.socket_fd = socket_fd;
}