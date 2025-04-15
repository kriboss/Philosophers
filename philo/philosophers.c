/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:34:18 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/15 18:03:10 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	start(int n, pthread_t *threads, t_data *d, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (n == 1)
	{
		pthread_create(&threads[0], NULL, lone_philo, &d->philos[0]);
		pthread_join(threads[0], NULL);
	}
	else
	{
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
	}
	free(threads);
	free_all(d, forks);
}

void	init(int argc, char **argv, t_data *d, pthread_mutex_t *forks)
{
	int	i;
	int	n;

	i = 0;
	n = ft_atoi(argv[1]);
	while (i < n)
		pthread_mutex_init(&forks[i++], NULL);
	d->philo_count = n;
	d->time_to_die = ft_atoi(argv[2]);
	d->time_to_eat = ft_atoi(argv[3]);
	d->time_to_sleep = ft_atoi(argv[4]);
	d->stop = 0;
	d->ne = -1;
	if (argc == 6)
		d->ne = ft_atoi(argv[5]);
	i = 0;
	while (i < n)
	{
		d->philos[i].id = i + 1;
		d->philos[i].te = 0;
		d->philos[i].last_eat = 0;
		d->philos[i].left_fork = &forks[i];
		d->philos[i].right_fork = &forks[(i + 1) % n];
		d->philos[i++].data = d;
	}
	printf("Number of threads: %d\n", n);
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
	init(n, argv, d, forks);
	start(n, threads, d, forks);
	return (0);
}
