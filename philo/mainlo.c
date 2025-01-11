#include "philo.h"

int	main(int ac, char **av)
{
	t_phdata	phdata;
	//t_philo		philo;

	if(ac != 5 && ac != 6)
		gs_error(0);
	gs_init_phdata(av, &phdata);
	cleanup_all(&phdata);
	//init_phdata(&phdata, av);
	return (0);
}