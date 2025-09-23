/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:40:53 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/23 22:32:11 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define IMAX 2147483647

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				te;
	int				l;
	long long		last_eat;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int		philo_count;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	int		ne;
	int		stop;
	int		full;
	pthread_mutex_t	*print;
	pthread_mutex_t	*s;
	pthread_mutex_t	*l;
	pthread_mutex_t	*eat_lock;
	t_philo	*philos;
}	t_data;

long long	get_ms(long long start);
int			check(char **argv);
void		free_all(t_data *data, pthread_mutex_t *forks);
long		ft_atoi(const char *str);
int			smart_sleep(long long duration_ms);
void		*lone_philo(void *arg);
void		*routine(void *arg);
int			check_stop(t_philo *p);
void		*monitoring(void *arg);

#endif