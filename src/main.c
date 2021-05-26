#include "../inc/ft_ping.h"

/*
** function: initialise_parameters
** -------------------------------
** sets up the whole [ping] data structure for the program, parsing the arguments
** and storing them in their rightful places
*/

void	initialise_parameters(char **argv)
{
	ping.ttl = 64;
	ping.count = 0;
	ping.interval = 1;
	ping.flags = parse_flags(argv);
	if (ping.flags & H_FLAG || ping.flags & BAD_FLAG)
	{
		error_output(USAGE);
		exit(1);
	}
	else
	{
		ping.process_id = getpid();
		ping.seq = 1;
		ping.sent_packets = 0;
		ping.received_packets = 0;
		ping.error_packets = 0;
		ping.socket_fd = -1;
		ping.sum_time = 0;
		ping.max_time = 0;
		ping.min_time = 0;
		ping.square_sum_time = 0;
		get_address(&(ping.user_requested_address), argv);
		interpret_address(ping.user_requested_address);
	}
}

/*
** function: free_memory
** ---------------------
** frees allocated memory, as the name says it
*/

void	free_memory(void)
{
	if (ping.address)
		free(ping.address);
	if (ping.reversed_address)
		free(ping.reversed_address);
}

/*
** function: main
** --------------
** MAIN FUNCTION !!
*/

int		main(int argc, char **argv)
{
	if (argc == 1)
		error_output(USAGE);
	else
	{
		initialise_parameters(argv);
		if (ping.flags & V_FLAG)
			display_flags();
		if (ping.address)
			create_socket();
		if (ping.socket_fd != -1)
		{
			set_signals();
			display_header();
			packet_loop();
			display_ending_stats();
		}
		free_memory();
	}
	return (0);
}