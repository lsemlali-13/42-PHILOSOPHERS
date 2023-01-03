/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsemlali <lsemlali@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 11:00:05 by lsemlali          #+#    #+#             */
/*   Updated: 2023/01/03 08:10:48 by lsemlali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define DEAD 0
# define ALIVE 1

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_info
{
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	long long		time_init;
	long long		current_t;
	int				nbr_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_time_to_eat;
	int				count;
}	t_info;

typedef struct s_philo
{
	pthread_t		t;
	pthread_mutex_t	ph_fork;
	t_info			*ph_info;
	int				id;
	int				time_since_last_meal;
	int				eat_count;
	int				curr_state;
	struct s_philo	*next_philo;
}	t_philo;

// proj utils
int				ft_atoi(const char *str);
void			add_front(t_philo **philo, t_philo *head);
void			check_info(int ac, t_info *info);
void			ending_sum(t_philo **ph);
void			philo_init(t_philo **ph, t_info **info);
void			get_info(int ac, char **av, t_info **info);

// routine fun
long long int	curr_time(t_philo *ph);
void			*ph_routine(void *philo);
void			eating_rt(t_philo *ph);
void			u_sleep(t_philo *ph, int time);

#endif
