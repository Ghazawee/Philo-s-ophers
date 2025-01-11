#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <sys/time.h>

typedef struct s_phdata t_phdata;

typedef struct s_philo{
	
	pthread_t		thrd;
	int				id;
	int		 		meals_count;
	int         	last_meal;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_phdata		*phdata;
}t_philo;

typedef struct s_phdata{
	
	int     		num_philo;
	int     		time_to_die;
	int     		time_to_eat;
	int     		time_to_sleep;
	int     		eat_limit;
	long			start_time;
	int				stop_sim;
	pthread_mutex_t	*forks;
	t_philo 		*philo;
}t_phdata;

//------------------free_error------------------//
void    gs_error(int err);
void	cleanup_mutexes(t_phdata *phdata);
void	cleanup_all(t_phdata *phdata);
//------------------pharse---------------------//
void    gs_init_phdata(char **av, t_phdata *phdata);
int		get_time();
//------------------utils---------------------//
int     is_digit(char c);
int     ft_atoi(const char *s);
//void	*ft_calloc(size_t count, size_t size);
#endif