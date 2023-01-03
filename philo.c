/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsemlali <lsemlali@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 11:00:05 by lsemlali          #+#    #+#             */
/*   Updated: 2023/01/03 08:11:29 by lsemlali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	u_sleep(t_philo *ph, int time)
{
	int	k;

	k = curr_time(ph);
	while (curr_time(ph) - k < time)
		usleep(200);
}

void	ph_supervisor(t_philo **philo)
{
	while (*philo)
	{
		pthread_mutex_lock(&(*philo)->ph_info->meal_lock);
		if ((curr_time(*philo) - (*philo)->time_since_last_meal) \
		>= (*philo)->ph_info->time_to_die)
		{
			pthread_mutex_lock(&((*philo)->ph_info->print_lock));
			printf("%lld %d has died\n", curr_time(*philo), (*philo)->id);
			ending_sum(philo);
		}
		pthread_mutex_unlock(&(*philo)->ph_info->meal_lock);
		if ((*philo)->ph_info->count >= (*philo)->ph_info->nbr_of_philos)
			exit(0);
		(*philo) = (*philo)->next_philo;
	}
}

void	create_threads(t_philo **philo)
{
	int	i;
	int	k;

	i = 0;
	k = (*philo)->ph_info->nbr_of_philos;
	(*philo)->ph_info->time_init = curr_time(*philo);
	(*philo)->ph_info->current_t = curr_time(*philo);
	while (i < k)
	{
		pthread_create(&((*philo)->t), NULL, ph_routine, *philo);
		usleep(100);
		*philo = (*philo)->next_philo;
		i++;
	}
	ph_supervisor(philo);
}

int	main(int ac, char **av)
{
	t_info	*info;
	t_philo	*philo;
	int		i;

	if (ac != 5 && ac != 6)
	{
		printf("u need at least 5 args");
		exit (DEAD);
	}
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
