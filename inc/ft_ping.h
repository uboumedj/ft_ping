#ifndef FT_PING_H
# define FT_PING_H

# define _GNU_SOURCE
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <errno.h>
# include <math.h>
# include "../libft/inc/libft.h"

/*
** OPTION FLAG DEFINES
*/

# define BAD_FLAG			0b10000000
# define V_FLAG				0b00000001
# define H_FLAG				0b00000010
# define T_FLAG				0b00000100
# define C_FLAG				0b00001000
# define I_FLAG				0b00010000
# define F_FLAG				0b00100000

/*
** ERROR MESSAGE DEFINES
*/

# define USAGE				"Usage: ft_ping [-v verbose] [-h help] [-t ttl] [-c count] [-i interval] [-f flood] destination"
# define BAD_FLAG_ERROR		"ft_ping: invalid option -- '%c'\n"
# define BAD_TTL_ERROR		"ft_ping: can't set time to live: invalid argument"
# define BAD_COUNT_ERROR	"ft_ping: bad number of packets to transmit"
# define BAD_INTERVAL_ERROR	"ft_ping: bad timing interval"
# define PERMISSION_ERROR	"ft_ping: socket: operation not permitted"
# define UNKNOWN_ADDR_ERROR	"ft_ping: %s: failure in name resolution\n"
# define SOCKET_ERROR		"ft_ping: socket: operation not permitted"
# define SETSOCKOPT_ERROR	"ft_ping: setsockopt: error while setting socket options"
# define TIMEOFDAY_ERROR	"ft_ping: gettimeofday: error while trying to access current time"
# define SENDTO_ERROR		"ft_ping: sendto: error while trying to send packet"
# define NO_CONNEXION_ERROR	"ft_ping: sendto: the network is unreachable"
# define RECV_ERROR			"ft_ping: recvmsg: error while trying to access ICMP reply"
# define REPLY_ERROR		"ft_ping: reply is not what was expected (not ICMP)"
# define TIMEOUT_ERROR		"ft_ping: recvmsg: request timed out"

/*
** FUNCTION RETURN DEFINES
*/

# define TTL_EXCEEDED_CODE	2
# define ERROR_CODE			1
# define SUCCESS_CODE		0

/*
** BYTE SWAP MACROS (ENDIANNESS)
*/

# define BSWAP16(x)			((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
# define BSWAP32(x)			((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8) | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))

typedef struct				s_packet
{
	struct icmp				icmp_header;
	char					data_buffer[36];
}							t_packet;

typedef struct				s_reply
{
	int						received_bytes;
	struct msghdr			msghdr;
	struct iovec			iov;
	struct icmp				*icmp;
	char					receive_buffer[84];
	char					control[CMSG_SPACE(sizeof(int))];
}							t_reply;

typedef struct				s_ping
{
	int						flags;
	double					interval;
	char					*user_requested_address;
	char					*address;
	char					*reversed_address;
	int						ttl;
	int						count;
	pid_t					process_id;
	int						socket_fd;
	int						sent_packets;
	int						received_packets;
	int						error_packets;
	int						seq;
	struct sockaddr_in		sockaddr;
	struct timeval			starting_time;
	struct timeval			ending_time;
	double					min_time;
	double					max_time;
	double					sum_time;
	double					square_sum_time;
}							t_ping;

/*
** GLOBAL VARIABLE
*/

t_ping						ping;

/*
** PROGRAM INITIALISING
*/

void						initialise_parameters(char **argv);
int							parse_flags(char **argv);
void						get_ttl(char **argv, int index);
void						get_count(char **argv, int index);
void						get_interval(char **argv, int index);
void						handle_flood(int flags);
void						store_flags(int *flags, char **argv, int i);
void						get_address(char **dest, char **argv);
char						check_previous_argument(char **argv, int i);
void						interpret_address(char *input);
void						reverse_dns(void);
void						create_socket(void);

/*
** LOOP
*/

void						packet_loop(void);
void						initialise_packet(struct s_packet *packet, struct timeval current_time);
char						send_packet(t_packet *packet);
void						initialise_reply(t_reply *reply);
char						receive_reply(t_reply *reply);
char						check_reply(t_reply *replys);
void						save_elapsed_time(double time_elapsed);
void						wait_interval(struct timeval start);

/*
** HELPING FUNCTIONS
*/

unsigned short				checksum(void *address, int len);
void						save_current_time(struct timeval *destination);
void						free_memory(void);
double						calculate_elapsed_time(struct timeval start, struct timeval end);
double						time_standard_deviation(void);
void						set_signals(void);
void						handle_interrupt_signal(int signal);
void						handle_alarm_signal(int signal);

/*
** DISPLAY FUNCTIONS
*/

void						display_header(void);
void						display_flags(void);
void						display_sequence(int received_bytes, t_reply reply, struct timeval start_timestamp, struct timeval end_timestamp);
void						display_exceeded_sequence(t_reply reply);
void						display_ending_stats(void);
void						error_output(char *message);
void						error_output_and_exit(char *message);

#endif
