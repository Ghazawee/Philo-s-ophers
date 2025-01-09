#include "philo.h"

int parse_args(char **av)
{
    int i;
    int j;

    i = 1;
    while (av[i])
    {
        if(av[i][0] == '\0') // should i consider an empty string as an invalid argument??
            return (1);
        j = 0;
        while (av[i][j])
        {
            if (!is_digit(av[i][j]))
                return (1);
            j++;
        }
        i++;
    }
    return (0);
}

void    gs_init_philo(char **av, t_phdata *phdata)
{
    if (parse_args(av) == 1)
    {
        printf("Error: Invalid argument\n");
        exit(1);
    }
    phdata->num_philo = ft_atoi(av[1]);
    phdata->time_to_die = ft_atoi(av[2]);
    phdata->time_to_eat = ft_atoi(av[3]);
    phdata->time_to_sleep = ft_atoi(av[4]);
    if(av[5])
        phdata->eat_limit = ft_atoi(av[5]);
    else
        phdata->eat_limit = -1;
    printf("num_philo: %d\ntime_to_die: %d\ntime_to_eat: %d\ntime_to_sleep: %d\n", phdata->num_philo, phdata->time_to_die, phdata->time_to_eat, phdata->time_to_sleep);
    if(phdata->eat_limit != -1)
        printf("eat_limit: %d\n", phdata->eat_limit);
}
