#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


typedef struct			s_timecode
{
	char				*id;
	time_t				start_time;
	time_t				end_time;
	struct 	s_timecode	*next;
}						t_timecode;

time_t		starting_time;
t_timecode	*chain;
int			counter;
time_t		final_start;

void register_time(char *rfid)
{
	t_timecode *element = chain;
	while (element)
	{
		if (!strcmp(element->id, rfid) && element->end_time == 0)
		{
			element->end_time = time(0);
			return ;
		}
		element = element->next;
	}
}

void display_time()
{
	t_timecode *element = chain;
	long long int total_time = 0;
	int hours	= 0;
	int minutes	= 0;
	int seconds	= 0;

	while (element)
	{
		total_time = element->end_time - element->start_time;
		hours = total_time / 3600;
		minutes = (total_time % 3600) / 60;
		seconds = total_time % 60;
		if (hours != 0)
		{
			printf("%dh ", hours);
			printf("%dm ", minutes);
			printf("%ds\n", seconds);
		}
		else if (minutes != 0)
		{
			printf("%dm ", minutes);
			printf("%ds\n", seconds);
		}
		else
		{
			printf("%ds\n", seconds);
		}
	}
}

void sighandler(int input)
{
	t_timecode *element = chain;

	int i = 0;
	while (i < counter && element)
	{
		element = element->next;
	}
	if (!element)
	{
		display_time();
		exit(0);
	}
	else
	{
		register_time(element->id);
	}
}

int main (int ac, char **av)
{
	starting_time = time(0);
	chain = malloc(sizeof(t_timecode) + 1);
	counter = 0;

	int index 			= 0;
	t_timecode *element	= malloc(sizeof(t_timecode) + 1);
	chain = element;
	char *splitted;
	if (ac == 1)
		return -1;
	while (av[index] != NULL)
	{
		if (av[index + 1] != NULL)
		{
			element->next = malloc(sizeof(t_timecode) + 1);
			element->id = 0;
			element->start_time = 0;
			element->end_time = 0;
		}
		splitted = strtok(av[index], "::");
		element->id = splitted;
		if (atoi(av[index]) < 0)
		{
			printf("BadValueException\n");
			exit (-1);
		}
		element->start_time = atoi(strtok(NULL, ":"));
		if (element->start_time > final_start)
		{
			final_start = element->start_time;
		}
		printf("%s, %d", element->id, element->start_time);
		element = element->next;

		index++;
	}
	signal(SIGINT, sighandler);
	sleep(20);
}
