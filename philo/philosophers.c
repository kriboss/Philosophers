/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:34:18 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/10 16:03:39 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>

int	get_ms(int start)
{
	struct timeval	tv;
	int				ms;

	gettimeofday(&tv, NULL);
	ms = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start;
	return (ms);
}

void	smart_sleep(long duration_ms)
{
	long	start;
	long	now;

	start = get_ms(0);
	now = get_ms(start);
	while (now < duration_ms)
	{
		usleep(100);
		now = get_ms(start);
	}
}

void	*routine(void *arg)
{
	t_philo	*philos;
	int		start;

	philos = (t_philo *)arg;
	start = get_ms(0);
	philos->last_eat = start;
	while (philos->data->time_to_die > get_ms(philos->last_eat)
		&& (philos->data->num_eat == -1 || philos->data->num_eat > philos->num_eat))
	{
		pthread_mutex_lock(philos->left_fork);
		pthread_mutex_lock(philos->right_fork);
		if (philos->data->time_to_die <= get_ms(philos->last_eat) || philos->data->stop)
			break ;
		printf("%dms %d is eating\n", get_ms(start), philos->id);
		smart_sleep(philos->data->time_to_eat);
		if (philos->data->time_to_die <= get_ms(philos->last_eat) || philos->data->stop)
			break ;
		philos->last_eat = get_ms(0);
		philos->num_eat++;
		printf("%dms %d has eaten %d times\n", get_ms(start), philos->id, philos->num_eat);
		pthread_mutex_unlock(philos->right_fork);
		pthread_mutex_unlock(philos->left_fork);
		if (philos->data->time_to_die <= get_ms(philos->last_eat) || philos->data->stop
			|| (philos->data->num_eat != -1 && philos->num_eat >= philos->data->num_eat))
			break ;
		printf("%dms %d is sleeping\n", get_ms(start), philos->id);
		smart_sleep(philos->data->time_to_sleep);
		printf("%dms %d is thinking\n", get_ms(start), philos->id);
	}
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
	if (philos->data->time_to_die <= get_ms(philos->last_eat))
	{
		if (philos->data->stop == 0)
		{
			printf("%dms %d died\n", get_ms(start), philos->id);
			philos->data->stop = 1;
		}
		pthread_exit(NULL);
	}
	pthread_exit(NULL);
}

int	main(int argc, char **argv)
{
	int				n;
	int				i;
	pthread_t		*threads;
	t_data			*d;
	pthread_mutex_t	*forks;

	if ((argc != 5 && argc != 6) || check(argv) || ft_atoi(argv[1]) > 200)
		return (printf("Error\n"), 1);
	n = ft_atoi(argv[1]);
	i = 0;
	threads = malloc(sizeof(pthread_t) * n);
	if (!threads)
		return (printf("Error\n"), 1);
	d = malloc(sizeof(t_data));
	if (!d)
		return (printf("Error\n"), free(threads), 1);
	d->philos = malloc(sizeof(t_philo) * n);
	if (!d->philos)
		return (printf("Error\n"), free(threads), free(d), 1);
	forks = malloc(sizeof(pthread_mutex_t) * n);
	if (!forks)
		return (printf("Error\n"), free(threads), free(d->philos), free(d), 1);
	while (i < n)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	d->philo_count = n;
	d->time_to_die = ft_atoi(argv[2]);
	d->time_to_eat = ft_atoi(argv[3]);
	d->time_to_sleep = ft_atoi(argv[4]);
	d->stop = 0;
	if (argc == 6)
		d->num_eat = ft_atoi(argv[5]);
	else
		d->num_eat = -1;
	i = 0;
	while (i < n)
	{
		d->philos[i].id = i + 1;
		d->philos[i].num_eat = 0;
		d->philos[i].last_eat = 0;
		d->philos[i].left_fork = &forks[i];
		d->philos[i].right_fork = &forks[(i + 1) % n];
		d->philos[i].data = d;
		i++;
	}
	printf("Number of threads: %d\n", n);
	(void)argc;
	i = 0;
	while (i < n)
	{
		pthread_create(&threads[i], NULL, routine, &d->philos[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
	free_all(d, forks);
	return (0);
}
