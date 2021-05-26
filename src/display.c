#include "../inc/ft_ping.h"

/*
** Functions handling the display of the program's various outputs
*/

/*
** function: display_header
** ------------------------
** displays the ping function's header with the destination address' info
*/

void	display_header(void)
{
	printf("PING %s (%s) 56(84) bytes of data.\n", ping.user_requested_address, ping.address);
}

/*
** funtion: display_ending_stats
** -----------------------------
** displays the ping function's ending stats, i.e. the sent/received/lost packet information
** and the time stats (rtt)
*/

void	display_ending_stats(void)
{
	int		lost_packets = 0;
	double	total_time;

	if (ping.sent_packets != 0)
	{
		lost_packets = 100 - ((ping.received_packets * 100) / ping.sent_packets);
		save_current_time(&ping.ending_time);
		printf("\n--- %s ping statistics ---\n", ping.user_requested_address);
		total_time = calculate_elapsed_time(ping.starting_time, ping.ending_time);
		if (!ping.error_packets)
		{
			printf("%d packets transmitted, %d received, %d%% packet loss, time %.0lfms\n",
				ping.sent_packets, ping.received_packets, lost_packets, total_time);
		}
		else
		{
			printf("%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %.0lfms\n",
				ping.sent_packets, ping.received_packets, ping.error_packets, lost_packets, total_time);
		}
		if (ping.received_packets != 0)
		{
			printf("rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", ping.min_time, ping.sum_time / ping.sent_packets,
				ping.max_time, time_standard_deviation());
		}
	}
}

/*
** function: display_sequence
** --------------------------
** called in the loop to display the current sequence's information after succesfully
** "pinging" the address. prints the sequence number, the packet's ttl and the calculated latency
*/

void	display_sequence(int received_bytes, t_reply reply, struct timeval start_timestamp, struct timeval end_timestamp)
{
	short		reply_ttl;
	double		time_elapsed;
	struct ip	*packet_content;

	packet_content = (struct ip *)reply.receive_buffer;
	reply_ttl = (short)packet_content->ip_ttl;
	time_elapsed = calculate_elapsed_time(start_timestamp, end_timestamp);
	if (!(ping.flags & F_FLAG))
	{
		if (ft_strcmp(ping.address, ping.user_requested_address))
		{
			printf("%lu bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2lf ms\n", received_bytes - sizeof(struct ip),
				ping.reversed_address, ping.address, ping.seq, reply_ttl, time_elapsed);
		}
		else
		{
			printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.2lf ms\n", received_bytes - sizeof(struct ip),
				ping.address, ping.seq, reply_ttl, time_elapsed);
		}
	}
	else
	{
		ft_putchar('\b');
		fflush(stdout);
	}
	save_elapsed_time(time_elapsed);
}

/*
** function: display_exceeded_sequence
** -----------------------------------
** displays information on a failed seq due to an exceeded TTL, i.e. the address where the packet
** stopped and its seq
*/

void	display_exceeded_sequence(t_reply reply)
{
	struct ip			*packet_content;
	char				ip[INET_ADDRSTRLEN];
	char				hostname[NI_MAXHOST];
	struct sockaddr_in	tmp_sockaddr;

	packet_content = (struct ip *)reply.receive_buffer;
	inet_ntop(AF_INET, &(packet_content->ip_src), ip, INET_ADDRSTRLEN);
	tmp_sockaddr.sin_addr = packet_content->ip_src;
	tmp_sockaddr.sin_family = AF_INET;
	tmp_sockaddr.sin_port = 0;
	if (!(ping.flags & F_FLAG))
	{
		if (ping.flags & V_FLAG)
			printf("ft_ping: TTL set to %d seems insufficient.\n", ping.ttl);
		if (getnameinfo((struct sockaddr *)&tmp_sockaddr, sizeof(struct sockaddr_in),
			hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD) >= 0)
		{
			printf("From %s (%s): icmp_seq=%d Time to live exceeded\n", hostname, ip, ping.seq);
		}
		else
		{
			printf("From %s: icmp_seq=%d Time to live exceeded\n", ip, ping.seq);
		}
	}
	else
	{
		ft_putchar('\b');
		ft_putchar('E');
		fflush(stdout);
	}
}

/*
** function: display_flags
** -----------------------
** only if there's a verbose flag, prints all flags at the beginning of the program
** in an understandable way
*/

void	display_flags(void)
{
	printf("ft_ping: verbose mode activated");
	if (ping.flags & T_FLAG)
		printf(", IP Time to live set to %d", ping.ttl);
	if (ping.flags & C_FLAG)
		printf(", packet count set to %d", ping.count);
	if (ping.flags & I_FLAG)
		printf(", interval between packets set to %.3lfs", ping.interval);
	if (ping.flags & F_FLAG)
		printf(", flood mode activated");
	printf("\n");
}

/*
** function: error_output
** ----------------------
** prints the given message on the standard error output
*/

void	error_output(char *message)
{
	fprintf(stderr, "%s\n", message);
}

/*
** function: error_output_and_exit
** -------------------------------
** prints the given message on the standard error output, then frees all freeable
** memory and exits with an error code (1)
*/

void	error_output_and_exit(char *message)
{
	fprintf(stderr, "%s\n", message);
	free_memory();
	exit(1);
}