#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

typedef struct s_philo{
	
	pthread_t   thrd;
	int         id;
	int         is_dead;
	int         time_to_die;
	int         time_to_eat;
	int         time_to_sleep;
	int         eat_limit;
}t_philo;

typedef struct s_phdata{
	
	int     num_philo;
	int     time_to_die;
	int     time_to_eat;
	int     time_to_sleep;
	int     eat_limit;
	t_philo *philo;
}t_phdata;

int     is_digit(char c);
void    gs_init_philo(char **av, t_phdata *phdata);
int     ft_atoi(const char *s);
//void	*ft_calloc(size_t count, size_t size);
#endif