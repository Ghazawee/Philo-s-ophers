#include "philo.h"

void	gs_sleep(int time, t_philo *philo) // maybe return 0/1 if philo is dead or not
{
	long start;
	//long elapsed;

	// action = time;
	start = get_time();
	//elapsed = get_time() - start;
	while (get_time() - start < time)
	{
		if (check_dead(philo->phdata))
			return ;
		usleep(100);
		//action -= 1;
	}
}

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

void	unlock_fork(t_philo *philo) // remove waiter from the function, this will only handle forks
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	philo_eat(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		unlock_fork(philo);
		return ;
	}
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		unlock_fork(philo);
		return ;
	}
	if (gs_logs(philo->phdata, philo->id, "is eating"))
	{
		unlock_fork(philo);
		return ;
	}
	gs_sleep(philo->phdata->time_to_eat, philo); // maybe make gs_sleep return 0/1 if philo is dead or not, and in this case i need to unlock forks before returning
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->phdata->state);
	unlock_fork(philo);
}

void	*handle_one_philo(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is thinking"))
		return (NULL);
	usleep(philo->phdata->time_to_die * 1000);
	return (NULL);
}

int	is_forks_pickable(t_philo *philo)
{
	while (!philo->phdata->stop_sim)
	{
		if(pthread_mutex_lock(philo->l_fork) == 0)
		{
			if(pthread_mutex_lock(philo->r_fork) == 0)
			{
				return (1);
			}
			pthread_mutex_unlock(philo->l_fork);
		}
		if (gs_logs(philo->phdata, philo->id, "is thinking"))
			return (0);
		//usleep(500);
		if(check_dead(philo->phdata))
			return (0);
	}
	return (0);
}
 

void    *gs_routi(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(10000);
	while(!philo->phdata->stop_sim)
	{
		if (philo->phdata->num_philo == 1)
			return (handle_one_philo(philo)); 
		if (is_forks_pickable(philo))
		{
			philo_eat(philo); // need to check if forks avail before hand and only then i eat and sleep, else they think
			if (check_dead(philo->phdata))
				return (NULL);
			if (gs_logs(philo->phdata, philo->id, "is sleeping"))
				return (NULL);
			gs_sleep(philo->phdata->time_to_sleep, philo);
			//continue ;
		}
		//usleep(philo->phdata->time_to_sleep * 1000); //need to check during this time if the philo is dead
		//else
		//{
		// if (gs_logs(philo->phdata, philo->id, "is thinking"))
		// 	return (NULL);
		else
			usleep(500);
			//gs_sleep(1, philo);
		//}
	}
	return (NULL);
}

int	check_philo_death(t_phdata *phdata, int i)
{
	pthread_mutex_lock(&phdata->state);
	if (get_time() - phdata->philo[i].last_meal > phdata->time_to_die)
	{
		gs_logs(phdata, phdata->philo[i].id, "died");
		lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
		pthread_mutex_unlock(&phdata->state);
		return (1);
	}
	pthread_mutex_unlock(&phdata->state);
	return (0);
}

int	check_eat_limit(t_phdata *phdata)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (i < phdata->num_philo)
	{
		pthread_mutex_lock(&phdata->state);
		if (phdata->philo[i].meals_count >= phdata->eat_limit)
			count++;
		pthread_mutex_unlock(&phdata->state);
		i++;
	}
	if (count == phdata->num_philo)
		return (1);
	return (0);
}

void    *gs_mont(void *arg)
{
	t_phdata *phdata;
	int i;
	//int k;

	phdata = (t_phdata *)arg;
	while (!phdata->stop_sim)
	{
		i = 0;
		//k = 0;
		while (i < phdata->num_philo)
		{
			if (check_philo_death(phdata, i))
				return (NULL);
			// pthread_mutex_lock(&phdata->state);
			// if (get_time() - phdata->philo[i].last_meal > phdata->time_to_die)
			// {
			// 	gs_logs(phdata, phdata->philo[i].id, "died");
			// 	lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
			// 	pthread_mutex_unlock(&phdata->state);
			// 	return (NULL);
			// }
			// if (phdata->eat_limit != -1 && phdata->philo[i].meals_count >= phdata->eat_limit)
			// 	k++;
			// pthread_mutex_unlock(&phdata->state);
			i++;
		}
		// if (phdata->eat_limit != -1 && k == phdata->num_philo)
		if (phdata->eat_limit != -1 && check_eat_limit(phdata))
		{
			lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
