#include "philo.h"

void	lock_set_unlock(pthread_mutex_t *mutex, int *var, int value)
{
	pthread_mutex_lock(mutex);
	*var = value;
	pthread_mutex_unlock(mutex);
}

int	check_dead(t_phdata *phdata)
{
	pthread_mutex_lock(&phdata->stop_mutex);
	if (phdata->stop_sim)
	{
		pthread_mutex_unlock(&phdata->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phdata->stop_mutex);
	return (0);
}

int	gs_logs(t_phdata *phdata, int id, char *msg)
{
	
	pthread_mutex_lock(&phdata->print);
	if(check_dead(phdata))
	{
		pthread_mutex_unlock(&phdata->print);
		return (1);
	}
	printf("%ld Philosopher %d %s\n", get_time() - phdata->start_time, id, msg);
	pthread_mutex_unlock(&phdata->print);
	return (0);
}

void	unlock_fork_waiter(t_philo *philo, int count)
{
	if(count == 1)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
	else if(count == 2)
	{
		//pthread_mutex_unlock(&philo->phdata->waiter);
		pthread_mutex_unlock(philo->l_fork);
	}
	else if (count == 3)
	{
		//pthread_mutex_unlock(&philo->phdata->waiter);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}

void	philo_eat(t_philo *philo)
{
	//pthread_mutex_lock(&philo->phdata->waiter); // no need for waiter mutex
	pthread_mutex_lock(philo->l_fork);
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		unlock_fork_waiter(philo, 2);
		return ;
	}
	pthread_mutex_lock(philo->r_fork);
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		unlock_fork_waiter(philo, 3);
		return ;
	}
	//pthread_mutex_unlock(&philo->phdata->waiter); 
	if (gs_logs(philo->phdata, philo->id, "is eating"))
	{
		unlock_fork_waiter(philo, 1);
		return ;
	}
	usleep(philo->phdata->time_to_eat * 1000); // need to check during this time if the philo is dead
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++; // maybe i need to increment this after the usleep ??
	pthread_mutex_unlock(&philo->phdata->state);
	if (check_dead(philo->phdata))
	{
		unlock_fork_waiter(philo, 1);
		return ;
	}
	unlock_fork_waiter(philo, 1);
}

void	*handle_one_philo(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is thinking"))
		return (NULL);
	usleep(philo->phdata->time_to_die * 1000);
	return (NULL);
}

void    *gs_routi(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while(!philo->phdata->stop_sim)
	{
		if (philo->phdata->num_philo == 1)
			return (handle_one_philo(philo));
		if (gs_logs(philo->phdata, philo->id, "is thinking"))
			return (NULL);
		philo_eat(philo);
		if (check_dead(philo->phdata))
			return (NULL);
		if (gs_logs(philo->phdata, philo->id, "is sleeping"))
			return (NULL);
		usleep(philo->phdata->time_to_sleep * 1000); //need to check during this time if the philo is dead
	}
	return (NULL);
}
void    *gs_mont(void *arg)
{
	t_phdata *phdata;
	int i;
	int k;

	phdata = (t_phdata *)arg;
	while (!phdata->stop_sim)
	{
		i = 0;
		k = 0;
		while (i < phdata->num_philo)
		{
			pthread_mutex_lock(&phdata->state);
			if (get_time() - phdata->philo[i].last_meal > phdata->time_to_die)
			{
				gs_logs(phdata, phdata->philo[i].id, "died");
				lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
				pthread_mutex_unlock(&phdata->state);
				return (NULL);
			}
			if (phdata->eat_limit != -1 && phdata->philo[i].meals_count >= phdata->eat_limit)
				k++;
			pthread_mutex_unlock(&phdata->state);
			i++;
		}
		if (phdata->eat_limit != -1 && k == phdata->num_philo)
		{
			lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
