#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


typedef struct			s_timecode
{
	int					id;
	time_t				start_time;
	time_t				end_time;
	struct 	s_timecode	*next;
}						t_timecode;

time_t		starting_time;
t_timecode	*chain;

void register_time(t_timecode *chain, char *rfid)
{
	int irfid = atoi(rfid);
	t_timecode element = chain;
	while (element)
	{
		if (element->id == irfid && element->end_time == 0)
		{
			element->end_time = time(0);
			return ;
		}
		element = element->next;
	}
}

int main (int ac, char **av)
{
	starting_time = time(0);
	printf("%lld\n", (long long)starting_time);


	int index 			= 0;
	t_timecode *element	= malloc(sizeof(t_timecode) + 1);
	chain = malloc(sizeof(t_timecode) + 1);
	chain = element;
	char *splitted;

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
		element->id = atoi(splitted);
		element->start_time = starting_time;
		element = element->next;

		index++;
	}
	add_event(register_time, register_time, chain);
	add_event(end, display_time);
}
