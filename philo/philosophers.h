/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:40:53 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/24 11:55:27 by kbossio          ###   ########.fr       */
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
	int				philo_count;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				ne;
	int				stop;
	int				full;
	pthread_mutex_t	*print;
	pthread_mutex_t	*s;
	pthread_mutex_t	*l;
	pthread_mutex_t	*eat_lock;
	pthread_t		*monitor;
	t_philo			*philos;
}	t_data;

long long	get_ms(long long start);
int			check(char **argv);
long		ft_atoi(const char *str);
int			smart_sleep(long long duration_ms, t_philo *p);
void		*lone_philo(void *arg);
void		*routine(void *arg);
void		eat(t_philo *p, int start);
void		lock(t_philo *p, int start);
int			check_alive(t_philo *p);
int			check_stop(t_philo *p);
void		print(char *fmt, int timestamp, t_philo *p);
void		*monitoring(void *arg);
t_data		*allocating(int n, pthread_t *threads);
void		free_all(
				t_data *d, pthread_mutex_t *forks, pthread_t *th, pthread_t *m
				);

#endif