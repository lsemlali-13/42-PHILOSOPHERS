#ifndef PHILO_H
#define PHILO_H

#define DEAD 0
#define ALIVE 1
#include "libft/libft.h"

typedef struct	s_info
{
	int nbr_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_time_to_eat;
	pthread_mutex_t print_lock;
	int	count;
	long long	time_init;
	long long	current_t;
}	t_info;

typedef struct s_philo
{
	pthread_t  t;
	pthread_mutex_t ph_fork;
	t_info	*ph_info;
	int	id;
	int time_since_last_meal;
	struct s_philo	*next_philo;
	int	eat_count;
	int	curr_state;
}	t_philo;

#endif