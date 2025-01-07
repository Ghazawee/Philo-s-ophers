#include "philo.h"

int	main(int ac, char **av)
{
	t_phdata	phdata;
	t_philo		philo;

	if(ac != 5 && ac != 6)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		return (1);
	}
	//init_phdata(&phdata, av);
	return (0);
}