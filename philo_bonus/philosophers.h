/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:40:53 by kbossio           #+#    #+#             */
/*   Updated: 2025/05/20 15:02:16 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <signal.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>

# define IMAX 2147483647

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				te;
	int				last_eat;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int		philo_count;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	int		ne;
	int		start;
	pid_t	*pid;
	sem_t	*forks;
	sem_t	*print;
	sem_t	*s;
	t_philo	*philos;
}	t_data;

int		get_ms(int start);
int		check(char **argv);
long	ft_atoi(const char *str);
int		get_ms(int start);
int		smart_sleep(long duration_ms, long ttd);
void	print(char *str, int start, t_philo *p);
int		lone_philo(t_philo *p);
void	free_sem(t_data *d);
void	free_all(t_data *data, pid_t *pid);
int		start(t_philo *p, int n);

#endif