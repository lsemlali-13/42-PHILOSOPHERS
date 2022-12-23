#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "philo.h"

void	add_front(t_philo **philo, t_philo *head)
{
	t_philo *tmp;

	head->next_philo = NULL;
	if (!*philo)
		*philo = head;
	else
	{
		tmp = *philo;
		while (tmp->next_philo)
			tmp = tmp->next_philo;
		tmp->next_philo = head;
	}
}

long long int curr_time(t_philo *ph)
{
	struct timeval tv;
	int	curr;

	gettimeofday(&tv, NULL);
	curr = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000 - ph->ph_info->time_init;
	return (curr);
}
void	eating_rt(t_philo *ph)
{
	pthread_mutex_lock(&(ph->ph_fork));
	pthread_mutex_lock(&(ph->ph_info->print_lock));
	printf("%lld %d has taken a fork\n", curr_time(ph), ph->id);
	pthread_mutex_unlock(&(ph->ph_info->print_lock));
	pthread_mutex_lock(&(ph->next_philo->ph_fork));
	pthread_mutex_lock(&(ph->ph_info->print_lock));
	printf("%lld %d has taken a fork\n", curr_time(ph), ph->id);
	printf("%lld %d is eating\n", curr_time(ph), ph->id);
	pthread_mutex_unlock(&(ph->ph_info->print_lock));
	ph->eat_count++;
	if (ph->eat_count == ph->ph_info->nb_time_to_eat)
		ph->ph_info->count++;
	ph->time_since_last_meal = curr_time(ph);
	usleep(ph->ph_info->time_to_eat * 1000);
	pthread_mutex_unlock(&(ph->ph_fork));
	pthread_mutex_unlock(&(ph->next_philo->ph_fork));
}
void *rt(void * philo)
{
	t_philo	*ph = (t_philo *) philo;

	while (1)
	{
		eating_rt(ph);
		pthread_mutex_lock(&(ph->ph_info->print_lock));
		printf("%lld %d is sleeping\n", curr_time(ph), ph->id);
		pthread_mutex_unlock(&(ph->ph_info->print_lock));
		usleep(ph->ph_info->time_to_sleep * 1000);
		pthread_mutex_lock(&(ph->ph_info->print_lock));
		printf("%lld %d is thinking\n", curr_time(ph), ph->id);
		pthread_mutex_unlock(&(ph->ph_info->print_lock));
	}
	return (NULL);
}

void	get_info(int ac, char **av, t_info **info)
{
	int	i;

	i = 0;
	*info = malloc(sizeof(t_info));
	(*info)->nbr_of_philos = ft_atoi(av[1]);
	(*info)->time_to_die = ft_atoi(av[2]);
	(*info)->time_to_eat = ft_atoi(av[3]);
	(*info)->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		(*info)->nb_time_to_eat = ft_atoi(av[5]);
	else
		(*info)->nb_time_to_eat = -1;
	pthread_mutex_init(&(*info)->print_lock, NULL);
	(*info)->count = 0;
}

void	philo_init(t_philo **ph, t_info **info)
{
	int	i;
	int	k;
	t_philo	*tmp;
	t_philo	*save;

	i = 0;
	k = (*info)->nbr_of_philos;
	while (i < k)
	{
		tmp = malloc(sizeof(t_philo));
		tmp->id = i + 1;
		tmp->ph_info = *info;
		tmp->time_since_last_meal = 0;
		tmp->curr_state = ALIVE;
		tmp->eat_count = 0;
		pthread_mutex_init(&tmp->ph_fork, NULL);
		add_front(ph, tmp);
		tmp = NULL;
		i++;
	}
	save = *ph;
	while (save->next_philo)
		save = save->next_philo;
	save->next_philo = *ph;
}

void	mutex_kill(t_philo **ph)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&(*ph)->ph_info->print_lock);
	while (i < (*ph)->ph_info->nbr_of_philos)
	{
		pthread_mutex_destroy(&(*ph)->ph_fork);
		*ph = (*ph)->next_philo;
		i++;
	}
}

void create_threads(t_philo **philo)
{
	int	i;
	int	k;

	i = 0; 
	k = (*philo)->ph_info->nbr_of_philos;
	(*philo)->ph_info->time_init = curr_time(*philo);
	(*philo)->ph_info->current_t = curr_time(*philo) - (*philo)->ph_info->time_init;
	while (i < k)
	{
		pthread_create(&((*philo)->t), NULL, rt, *philo);
		usleep(100);
		*philo = (*philo)->next_philo;
		i++;
	}
	while (*philo)
	{
		if (curr_time(*philo) - (*philo)->time_since_last_meal > (*philo)->ph_info->time_to_die)
		{
			pthread_mutex_lock(&((*philo)->ph_info->print_lock));
			printf("%lld %d has died\n", curr_time(*philo), (*philo)->id);
			pthread_mutex_unlock(&((*philo)->ph_info->print_lock));
			mutex_kill(philo);
			exit(0);
		}
		if ((*philo)->ph_info->count >= (*philo)->ph_info->nbr_of_philos)
			exit(0);
		(*philo) = (*philo)->next_philo;
	}
}

void	check_info(int ac, t_info *info)
{
	if (info->nbr_of_philos <= 0 || info->time_to_die <= 0 || info->time_to_eat <= 0 || info->time_to_sleep <= 0 || (info->nb_time_to_eat <= 0 && ac == 6))
	{
		printf("check args and  try again\n");
		exit (0);
	}
}

int	main(int ac, char **av)
{
	t_info	*info;
	t_philo *philo;
	int	i;

	get_info(ac, av, &info);
	check_info(ac, info);
	philo_init(&philo, &info);
	create_threads(&philo);
	i = 0;
	while (i < philo->ph_info->nbr_of_philos)
	{
		pthread_join(philo->t, NULL);
		philo = philo->next_philo;
		i++;
	}
	return (0);
}
