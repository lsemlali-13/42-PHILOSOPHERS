/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsemlali <lsemlali@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 11:00:05 by lsemlali          #+#    #+#             */
/*   Updated: 2022/12/23 11:51:30 by lsemlali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ph_supervisor(t_philo **philo)
{
	while (*philo)
	{
		pthread_mutex_lock(&(*philo)->ph_info->meal_lock);
		if ((curr_time(*philo) - (*philo)->time_since_last_meal) \
		> (*philo)->ph_info->time_to_die)
		{
			pthread_mutex_lock(&((*philo)->ph_info->print_lock));
			printf("%lld %d has died\n", curr_time(*philo), (*philo)->id);
			pthread_mutex_unlock(&((*philo)->ph_info->print_lock));
			ending_sum(philo);
			exit(0);
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
