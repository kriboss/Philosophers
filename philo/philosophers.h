/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:40:53 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/15 18:55:04 by kbossio          ###   ########.fr       */
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
	int				last_eat;
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
	t_philo	*philos;
}	t_data;

int		get_ms(int start);
int		check(char **argv);
void	*free_all(t_data *data, pthread_mutex_t *forks);
long	ft_atoi(const char *str);
int		get_ms(int start);
int		smart_sleep(long duration_ms, long ttd);
void	*lone_philo(void *arg);
void	*routine(void *arg);

#endif