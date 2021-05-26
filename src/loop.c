#include "../inc/ft_ping.h"

/*
** Functions related to the ping loop
*/

/*
** function: initialise_packet
** --------------------------
** initialises the different parts of the ICMP echo request packet to be sent to the
** destination address, performing byte swaps on necessary parts for endianness compliance
*/

void			initialise_packet(struct s_packet *packet, struct timeval current_time)
{
	ft_bzero(packet, sizeof(t_packet));
	packet->icmp_header.icmp_type = ICMP_ECHO;
	packet->icmp_header.icmp_code = 0;
	packet->icmp_header.icmp_seq = BSWAP16(ping.seq);
	packet->icmp_header.icmp_id = BSWAP16(ping.process_id);
	ft_memcpy(&packet->icmp_header.icmp_dun, &(current_time.tv_sec), sizeof(current_time.tv_sec));
	packet->icmp_header.icmp_cksum = checksum(packet, sizeof(*packet));
}

/*
** function: checksum
** ------------------
** performs a checksum of the unsigned short at the address and returns the result. Used
** during the packet creation, doing a 16-bit ones's complement of the one's complement sum
** of the ICMP message 
*/

unsigned short	checksum(void *address, int len)
{
	unsigned short	*buff;
	unsigned long	sum;

	buff = (unsigned short *)address;
	sum = 0;
	while (len > 1)
	{
		sum += *buff;
		buff++;
		len -= sizeof(unsigned short);
	}
	if (len)
		sum += *(unsigned char *)buff;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((unsigned short)~sum);
}

/*
** function: initialise_reply
** --------------------------
** initialises the different parts of the t_reply structure that will be used to
** store the destination IP's response to our sent packet.
*/

void			initialise_reply(t_reply *reply)
{
	ft_bzero(reply, sizeof(t_reply));
	reply->iov.iov_base = reply->receive_buffer;
	reply->iov.iov_len = sizeof(reply->receive_buffer);
	reply->msghdr.msg_name = ping.address;
	reply->msghdr.msg_iov = &reply->iov;
	reply->msghdr.msg_iovlen = 1;
	reply->msghdr.msg_control = &reply->control;
	reply->msghdr.msg_controllen = sizeof(reply->control);
}

/*
** function: send_packet
** ---------------------
** sends packets to the destination address and handles various errors encountered
** by the sendto function
*/

char			send_packet(t_packet *packet)
{
	ssize_t sent_bytes;

	sent_bytes = sendto(ping.socket_fd, packet, sizeof(*packet), 0,
		(struct sockaddr*)&ping.sockaddr, sizeof(ping.sockaddr));
	if (sent_bytes <= 0)
	{
		if (errno == ENETUNREACH)
			error_output(NO_CONNEXION_ERROR);
		else
			error_output(SENDTO_ERROR);
		return (ERROR_CODE);
	}
	if (ping.flags & F_FLAG)
	{
		ft_putchar('.');
		fflush(stdout);
	}
	return (SUCCESS_CODE);
}

/*
** function: receive_reply
** -----------------------
** stores the response inside the reply structure, and handles the various errors
** encountered by the recvmsg function
*/

char			receive_reply(t_reply *reply)
{
	reply->received_bytes = recvmsg(ping.socket_fd, &(reply->msghdr), 0);
	if (reply->received_bytes > 0)
	{
		return (check_reply(reply));
	}
	else
	{
		if ((errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
		{
			if (ping.flags & V_FLAG)
				error_output(TIMEOUT_ERROR);
		}
		else
			error_output(RECV_ERROR);
		return (ERROR_CODE);
	}
	return (SUCCESS_CODE);
}

/*
** function: check_reply
** ---------------------
** reads the content of the response packet to check that it is what was expected
*/

char			check_reply(t_reply *reply)
{
	struct ip	*packet_content;

	packet_content = (struct ip *)reply->receive_buffer;
	if (packet_content->ip_p != IPPROTO_ICMP)
	{
		if (ping.flags & V_FLAG)
			error_output(REPLY_ERROR);
		return (ERROR_CODE);
	}
	reply->icmp = (struct icmp *)(reply->receive_buffer + (packet_content->ip_hl << 2));
	if (reply->icmp->icmp_type == 11 && reply->icmp->icmp_code == 0)
	{
		return (TTL_EXCEEDED_CODE);
	}
	else if (BSWAP16(reply->icmp->icmp_id) != ping.process_id || BSWAP16(reply->icmp->icmp_seq) != ping.seq)
	{
		initialise_reply(reply);
		return (receive_reply(reply));
	}
	return (SUCCESS_CODE);
}

/*
** function: packet_loop
** ---------------------
** loop function to send packets to the destination address and wait for a response, repeatedly,
** until either encountering a program-stopping error or a CTRL-C
*/

void			packet_loop(void)
{
	t_packet			packet;
	struct timeval		current_start_timestamp;
	struct timeval		current_ending_timestamp;
	t_reply				reply;
	char				check;

	save_current_time(&ping.starting_time);
	while(1)
	{
		save_current_time(&current_start_timestamp);
		initialise_packet(&packet, current_start_timestamp);
		ping.sent_packets++;
		check = send_packet(&packet);
		if (check == SUCCESS_CODE)
		{
			initialise_reply(&reply);
			check = receive_reply(&reply);
			if (check == SUCCESS_CODE)
			{
				ping.received_packets++;
				save_current_time(&current_ending_timestamp);
				display_sequence(reply.received_bytes, reply, current_start_timestamp, current_ending_timestamp);;
			}
			else if (check == TTL_EXCEEDED_CODE)
			{
				display_exceeded_sequence(reply);
				ping.error_packets++;
			}
		}
		ping.seq++;
		if (ping.count)
		{
			ping.count--;
			if (ping.count == 0)
				break;
		}
		wait_interval(current_start_timestamp);
	}
}