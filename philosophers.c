/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:34:18 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/01 12:38:36 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *free_all(t_data *data, pthread_mutex_t *forks)
{
	int i;

	if (forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&forks[i]);
			i++;
		}
		free(forks);
	}
	if (data->philos)
		free(data->philos);
	free(data);
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = (t_philo *)arg;
	while (i < philos->data->philo_count)
	{
		printf("Philosopher %d is thinking\n", philos->id);
        usleep(1000); // Simula il tempo di pensiero

        /* Per evitare deadlock, potresti far
           variare l'ordine del lock in base all'id */
        pthread_mutex_lock(philos->left_fork);
        pthread_mutex_lock(philos->right_fork);
        printf("Philosopher %d is eating\n", philos->id);
        usleep(philos->data->time_to_eat); // Simula il tempo di mangiare
        pthread_mutex_unlock(philos->right_fork);
        pthread_mutex_unlock(philos->left_fork);

        usleep(philos->data->time_to_sleep); // Simula il tempo di riposo
		i++;
	}
	pthread_exit(NULL);
}

int	main(int argc, char **argv)
{
	int			n;
	int			i;
	pthread_t	*threads;
	t_data		*d;
	pthread_mutex_t	*forks;

	if (argc != 5 && argc != 6)
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
		return (printf("Error\n"), free(threads), free(d->philos) ,free(d), 1);
	while (i < n)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	d->philo_count = n;
	d->time_to_eat = ft_atoi(argv[2]);
	d->time_to_sleep = ft_atoi(argv[3]);
	d->time_to_die = ft_atoi(argv[4]);
	if (argc == 6)
		d->num_eat = ft_atoi(argv[5]);
	else
		d->num_eat = -1;
	i = 0;
    while (i < n)
    {
        d->philos[i].id = i;
		d->philos[i].num_eat = 0;
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
	free_all(d, forks);
	return (0);
}