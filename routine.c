/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsemlali <lsemlali@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 11:38:56 by lsemlali          #+#    #+#             */
/*   Updated: 2022/12/23 15:32:24 by lsemlali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long int	curr_time(t_philo *ph)
{
	struct timeval	tv;
	int				curr;

	gettimeofday(&tv, NULL);
	curr = ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000) - ph->ph_info->time_init;
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
	pthread_mutex_lock(&ph->ph_info->meal_lock);
	ph->time_since_last_meal = curr_time(ph);
	pthread_mutex_unlock(&ph->ph_info->meal_lock);
	usleep(ph->ph_info->time_to_eat * 1000);
	pthread_mutex_unlock(&(ph->ph_fork));
	pthread_mutex_unlock(&(ph->next_philo->ph_fork));
}

void	*ph_routine(void *philo)
{
	t_philo	*ph;

	ph = (t_philo *) philo;
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
