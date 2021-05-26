#include "../inc/ft_ping.h"

/*
** Functions directly handling time in the program
*/

/*
** function: wait_interval
** -----------------------
** used in the program's loop, to ensure there's at least [ping.interval] seconds
** between each sent packet. Default interval is 1 second
*/

void			wait_interval(struct timeval start)
{
	struct timeval	current_time;
	struct timeval	goal_time;

	if (ping.interval)
	{
		current_time = start;
		goal_time.tv_sec = current_time.tv_sec + (long)ping.interval;
		goal_time.tv_usec = current_time.tv_usec + (long)((ping.interval - (long)ping.interval) * 1000000);
		while (timercmp(&current_time, &goal_time, <))
		{
			save_current_time(&current_time);
		}
	}
}

/*
** function: calculate_elapsed_time
** --------------------------------
** simply returns the time elapsed between the ICMP echo request being sent and
** the response being received
*/

double			calculate_elapsed_time(struct timeval start, struct timeval end)
{
	return (((double)((double)end.tv_sec - (double)start.tv_sec) * 1000) +
		(double)((double)end.tv_usec - (double)start.tv_usec) / 1000);
}

/*
** function: save_elapsed_time
** ---------------------------
** adds the elapsed time to the total elapsed times (used to calculate the average latency
** when outputting the rtt line at the end), and saves the current elapsed time if it's inferior
** or superior to the current minimum/maximum values
*/

void			save_elapsed_time(double time_elapsed)
{
	ping.sum_time += time_elapsed;
	ping.square_sum_time += time_elapsed * time_elapsed;
	if (ping.min_time > time_elapsed || ping.min_time == 0)
		ping.min_time = time_elapsed;
	if (ping.max_time < time_elapsed || ping.max_time == 0)
		ping.max_time = time_elapsed;
}

/*
** function: save_current_time
** ---------------------------
** saves the current time to the specified timeval structure. Basically saves a line of code
** in multiple locations :)
*/

void			save_current_time(struct timeval *destination)
{
	if (gettimeofday(destination, NULL) == -1)
		error_output_and_exit(TIMEOFDAY_ERROR);
}

/*
** function: time_standard_deviation
** ---------------------------------
** the rtt line requires the standard deviation of the latency (mdev), i.e. the average distance
** between a "ping"'s latency and the average latency. This function returns just this, using
** the formula [mdev = SQRT(average square time - average time ^2)]
*/

double			time_standard_deviation(void)
{
	double		average_time;
	double		average_squared_time;

	average_time = ping.sum_time / ping.received_packets;
	average_squared_time = ping.square_sum_time / ping.received_packets;
	return (sqrt(average_squared_time - (average_time * average_time)));
}