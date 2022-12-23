/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsemlali <lsemlali@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 11:22:35 by lsemlali          #+#    #+#             */
/*   Updated: 2022/12/23 11:50:04 by lsemlali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	add_front(t_philo **philo, t_philo *head)
{
	t_philo	*tmp;

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

void	check_info(int ac, t_info *info)
{
	if (info->nbr_of_philos <= 0 || info->time_to_die <= 0 \
	|| info->time_to_eat <= 0 || info->time_to_sleep <= 0 \
	|| (info->nb_time_to_eat <= 0 && ac == 6))
	{
		printf("check args and  try again\n");
		exit (0);
	}
}

void	ending_sum(t_philo **ph)
{
	t_philo	*tmp;
	int		i;
	int		k;

	k = (*ph)->ph_info->nbr_of_philos;
	i = 0;
	pthread_mutex_destroy(&(*ph)->ph_info->print_lock);
	pthread_mutex_destroy(&(*ph)->ph_info->meal_lock);
	free((*ph)->ph_info);
	while (i < k)
	{
		tmp = (*ph)->next_philo;
		pthread_mutex_destroy(&(*ph)->ph_fork);
		free(*ph);
		*ph = tmp;
		i++;
	}
}

void	philo_init(t_philo **ph, t_info **info)
{
	t_philo	*tmp;
	t_philo	*save;
	int		k;
	int		i;

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
	pthread_mutex_init(&(*info)->meal_lock, NULL);
	(*info)->count = 0;
}
