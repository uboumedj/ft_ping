#include "../inc/ft_ping.h"

/*
** Signal handling
*/

/*
** function: set_signals
** ---------------------
** sets the required signal overrides for the program
*/

void	set_signals(void)
{
	signal(SIGINT, &handle_interrupt_signal);
	signal(SIGALRM, &handle_alarm_signal);
}

/*
** function: handle_interrupt_signal
** ---------------------------------
** the override function for the SIGINT (CTRL-C) signal. Displays the ending stats
** before freeing the memory and leaving with exit code 0 as the program worked correctly
*/

void	handle_interrupt_signal(int signal)
{
	(void)signal;
	display_ending_stats();
	free_memory();
	exit(0);
}

/*
** function: handle_alarm_signal
** -----------------------------
** the override function for the SIGINT (CTRL-C) signal. Displays the ending stats
** before freeing the memory and leaving with exit code 0 as the program worked correctly
*/

void	handle_alarm_signal(int signal)
{
	(void)signal;
	fprintf(stderr, "Alarm used\n");
}