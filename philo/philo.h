#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_philo{

}t_philo;

typedef struct s_phdata{
    
    t_philo *philo;
}t_phdata;

int     ft_atoi(const char *s);
void	*ft_calloc(size_t count, size_t size);
#endif